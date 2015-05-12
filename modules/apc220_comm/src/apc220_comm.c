/*
 ============================================================================
 Name        : serialComm.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include "apc220_comm.h"


/*global variables*/

int 	fd = 0;


/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/
void apc220_write_task_func(void *arg)
{

    char *buffer;
    int bytesRead;
    int returnValue;

	/*init values*/
    returnValue 	=  RETURN_OK;
	bytesRead		= 0;
	buffer  	    = NULL;
    //int counter     = 0;

	rt_task_set_periodic(NULL, TM_NOW, 6000*1000*10);

	fprintf(stderr,"\n");
	if (returnValue == RETURN_OK)
	  {
	  returnValue= rt_queue_bind (&acp220_outputQueue,
								  acp220_outputQueueName,
								  TM_NONBLOCK);
	  }
	  while(1)
	  {

				  if (returnValue == RETURN_OK)
				  {
					  buffer = malloc(sizeof(*buffer));

				  			bytesRead=rt_queue_read(&acp220_outputQueue,
				  									buffer,
													sizeof(buffer),
													TM_NONBLOCK);

					  if (bytesRead>0)
					  {
						  write(fd, buffer, sizeof(buffer));
					  }
					  else
					  {

					  }
				  }
				  else
				  {
					 // counter++;
					 //fprintf(stderr,"aCounter %d\r",counter);
					 returnValue=RETURN_OK; //start again
				  }

          tcflush(fd,TCIOFLUSH);
          fflush(stdout);
          fflush(stderr);
	      rt_task_wait_period(NULL);
      }
}
/* (END) APC220_WRITE_TASK_FUNC*/


/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/
void apc220_read_task_func(void *arg)
{

	ssize_t length;
    char buffer[ACK_INPUT_MSG_SIZE+3];


	/*init values*/
	length		    = 0;
	buffer[0] 	    = '\0';
	//int counter 	= 0;

	rt_task_set_periodic(NULL, TM_NOW, 6000*1000*10);


	  while(1)
	  {
		  buffer[0] 	  = '\0';
		  length		  = 0;
		  //fcntl(fd,F_SETFL,0);
		  length = read(fd, &buffer, sizeof(buffer));

				if (length <= ACK_INPUT_MSG_SIZE)
				{
					//fprintf(stderr,"bcounter %d... \r",counter);
					//counter++;
				}
				else
				{
					//fprintf(stderr,"input: %s\n",buffer);
					rt_queue_write(&acp220_inputQueue,
									 &buffer,
									 sizeof(buffer),
									 Q_URGENT);
				}

          tcflush(fd,TCIOFLUSH);
          fflush(stdout);
          fflush(stderr);
	      rt_task_wait_period(NULL);
      }
}
/* (END) APC220_READ_TASK_FUNC*/

/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/
void apc220_connect_task_func(void *arg)
{


    bool areTasksInitilized;
    bool connected;

    /*init values*/
    connected      		= FALSE;
    areTasksInitilized  = FALSE;

	rt_task_set_periodic(NULL, TM_NOW, 9000*1000*100);

   //Connect to port
   while (1)
   {
	   if(!connected)
	   {
		   fd = open(PORT, O_RDWR | O_NOCTTY | O_NONBLOCK | O_FSYNC);
		   int flags = fcntl(fd, F_GETFL, 0);
		       fcntl(fd, F_SETFL, flags | O_NONBLOCK | FNDELAY);

			 if(fd>=0)
			 {
				 bzero(&portSettings, sizeof(portSettings));
				 portSettings.c_cflag &= ~PARENB;
				 portSettings.c_cflag &= ~CSTOPB;
				 portSettings.c_cflag &= ~CSIZE;
				 portSettings.c_cflag &= ~CRTSCTS;
				 portSettings.c_cflag |= CS8 | CLOCAL;
				 portSettings.c_lflag |= (ICANON);
				 //portSettings.c_lflag &= ~ICANON;
				 portSettings.c_oflag &= ~OPOST;
				 portSettings.c_cc[VMIN] = 0;
				 portSettings.c_cc[VTIME] = 0;
				 tcflush(fd, TCIFLUSH);
				 cfsetospeed(&portSettings, BAUDRATE);
				 cfsetispeed(&portSettings, BAUDRATE);
				 tcsetattr(fd,TCSANOW,&portSettings);
				 connected = TRUE;
				 fprintf(stderr,"Open Radio Port : OK.\n");
			 }
			 else
			 {
				 fprintf(stderr,"Open Radio Port : Error.\r");
				 connected = FALSE;
			 }
	   }
	   else if(!areTasksInitilized)
	   {
		   rt_task_start(&apc220_read_task,
						  &apc220_read_task_func,
						  NULL);
		   rt_task_start(&apc220_write_task,
						  &apc220_write_task_func,
						  NULL);
		   areTasksInitilized = TRUE;
	   }
	   fflush(stdout);
	   fflush(stderr);

     rt_task_wait_period(NULL);
   }



}/* (END) OPEN_PORT*/


/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/
 int apc220_comm_init()
{
	  int returnValue;

	  /*init values*/
	  returnValue 						  = RETURN_OK;


	  returnValue = rt_task_create(&apc220_connect_task,
								   "apc220_connect_task",
								   0,
								   1,
								   T_JOINABLE);

	  if (returnValue == RETURN_OK)
	  {
		 returnValue = rt_task_create(&apc220_read_task,
									   "apc220_read_task",
									   0,
									   3,
									   T_JOINABLE);
	  }
	  if (returnValue == RETURN_OK)
	  {
		 returnValue = rt_task_create(&apc220_write_task,
									   "apc220_write_task",
									   0,
									   3,
									   T_JOINABLE);
	  }

     if (returnValue == RETURN_OK)
	 {
		 returnValue = rt_queue_create(&acp220_inputQueue,
										acp220_inputQueueName,
									   ACK_INPUT_MSG_SIZE+3,
									   Q_UNLIMITED,
									   Q_SHARED);
	 }
	 if (returnValue == RETURN_OK)
	 {
		 returnValue = rt_task_start(&apc220_connect_task,
									 &apc220_connect_task_func,
									  NULL);

	 }



	return (returnValue);
}/* (END) APC_220_COMM_INIT*/





