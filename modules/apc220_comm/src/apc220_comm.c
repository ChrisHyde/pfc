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

int 	fd			    = 0;

int     loopCompleted   = 0;
bool    PANIC_MESSAGE   = false;


ST_alivetasks   aliveTasks;
ST_apc220_inputVectors inputVectors;




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
void apc220_check_read_alive_task_func(void *arg)
{

	int   readAliveCounter;
	bool  PANIC_MESSAGE;

	/*init values*/
	    readAliveCounter	= 0;
	    PANIC_MESSAGE 		= false;

	    rt_task_set_periodic(NULL, TM_NOW, 70000000);

	    while(1)
	    {
	    	fprintf(stderr,"aliveTasks.isReadTaskAlive %d\n",aliveTasks.isReadTaskAlive);
	    	fprintf(stderr,"readAliveCounter %d\n",readAliveCounter);
	        if (aliveTasks.isReadTaskAlive==true && PANIC_MESSAGE==false)
	        {
	        	readAliveCounter=0;
	        }
	        else
	        {
	        	readAliveCounter++;
	        }
	        if(readAliveCounter>=APC220_PANIC_CONNECTION_LOSS)
	        {
	        	PANIC_MESSAGE=true;
	        	fprintf(stderr,"CONNECTION LOST!!!, write panic package\n");
	        }

	        rt_task_wait_period(NULL);
	    }


}/*(END) APC_CHECK_READ_ALIVE_TASK*/


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
	int returnValue;
	ssize_t length;
    char buffer[ACK_INPUT_MSG_SIZE+3];
    int recivedValuesCounter;
    float acx,acy,acz;
    float gyx,gyy,gyz;
    float motor1,motor2,motor3,motor4;
    float num;

	/*init values*/
    returnValue				   = 0;
	length		               = 0;
	buffer[0] 	               = '\0';
	recivedValuesCounter   	   = 0;
	acx					       = 0.0;
	acy					       = 0.0;
	acz					       = 0.0;
	gyx					   	   = 0.0;
	gyy					   	   = 0.0;
	gyz					   	   = 0.0;
	motor1					   = 0.0;
	motor2					   = 0.0;
	motor3					   = 0.0;
	motor4					   = 0.0;
	num 			  		   = 0.0;
	rt_task_set_periodic(NULL, TM_NOW, TM_INFINITE);

	  while(1)
	  {
		  buffer[0] 	  = '\0';
		  length		  = 0;
		  num 			  = 0.0;
		  fcntl(fd,F_SETFL,0);
		  aliveTasks.isReadTaskAlive=false;
		  length = read(fd, &buffer, sizeof(buffer));
		  aliveTasks.isReadTaskAlive=true;
				if (length == RETURN_ERROR)
				{
					aliveTasks.isReadTaskAlive=false;
				}
				else if (length == RETURN_OK)
				{
				  /*no data recieved*/
					//fprintf(stderr,"No more data\n");
				}
				else
				{

					num = atof(buffer);
					recivedValuesCounter++;

					if(length>=ACK_INPUT_MSG_SIZE)
					{
						inputVectors.AcX=acx;
						inputVectors.AcY=acy;
						inputVectors.AcZ=acz;
						inputVectors.GyX=gyx;
						inputVectors.GyY=gyy;
						inputVectors.GyZ=gyz;
						inputVectors.M1=motor1;
						inputVectors.M2=motor2;
						inputVectors.M3=motor3;
						inputVectors.M4=motor4;
						recivedValuesCounter=0;
						fprintf(stderr,"acx:%f--->acy:%f--->acz:%f--->gyx:%f--->gyy:%f--->gyz:%f--->"
								"motor1:%f--->motor2:%f--->motor3:%f--->motor4:%f\n\n",acx,acy, acz, gyx, gyy, gyz,motor1,motor2,motor3,motor4);
			         returnValue = rt_queue_write(&acp220_inputQueue,
													 &inputVectors,
													 sizeof(inputVectors),
													 Q_URGENT);
						//fprintf(stderr,"APC220 write to QUEUE return: %d\n",returnValue);
						if(returnValue>0)
						{
							returnValue=RETURN_OK;
						}

					}
					else
					{
						if(recivedValuesCounter==READ_POSITION_AcX)
						{
							if(num>acx+1 || num<acx-1)
							{
								acx=acx;
							}
							else
							{
							    acx=num;
							}
						}
						if(recivedValuesCounter==READ_POSITION_AcY)
						{
							if(num>acy+1 || num<acy-1)
							{
								acy=acy;
							}
							else
							{
							acy=num;
							}
						}
						if(recivedValuesCounter==READ_POSITION_AcZ)
						{
							if(num>acz+1 || num<acz-1)
							{
								acz=acz;
							}
							else
							{
							   acz=num;
							}//acz=num;
						}
						if(recivedValuesCounter==READ_POSITION_GyX)
						{
							if(num>gyx+1 || num<gyx-1)
							{
								gyx=gyx;

							}
							else
							{
								gyx=num;
								//fprintf(stderr," %f\n",gyx);
							}//gyx=num;
						}
						if(recivedValuesCounter==READ_POSITION_GyY)
						{
							if(num>gyy+1 || num<gyy-1)
							{
								gyy=gyy;
							}
							else
							{
								gyy=num;
								//fprintf(stderr," %f\n",gyy);
							}//gyy=num;
						}
						if(recivedValuesCounter==READ_POSITION_GyZ)
						{
							if(num>gyz+1 || num<gyz-1)
							{
								gyz=gyz;
							}
							else
							{
								gyz=num;
							}//gyz=num;
						}
						if(recivedValuesCounter==READ_POSITION_M1)
						{

								motor1=num/100;
							//fprintf(stderr," %f\n",motor1);
						}
						if(recivedValuesCounter==READ_POSITION_M2)
						{
							motor2=num/100;//fprintf(stderr," %f\n",motor2);
						}
						if(recivedValuesCounter==READ_POSITION_M3)
						{
							motor3=num/100;//fprintf(stderr," %f\n",motor3);
						}
						if(recivedValuesCounter==READ_POSITION_M4)
						{
							motor4=num/100;//fprintf(stderr," %f ENNNNND\n",motor4);
						}
					}
				}

          tcflush(fd,TCIOFLUSH);
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
int open_port(void)
{

    int returnValue;
    int connectionAttempts;

    /*init values*/
    connectionAttempts = 0;
    returnValue        = RETURN_ERROR;



   while (connectionAttempts!=MAX_CONNECTION_ATTEMPTS &&
		  returnValue == RETURN_ERROR)
   {
   connectionAttempts++;
   fd = open(PORT, O_RDWR | O_NOCTTY | O_NONBLOCK | O_FSYNC);
     if(fd>=0)
     {

    	 bzero(&portSettings, sizeof(portSettings));
    	 portSettings.c_cflag &= ~PARENB;
		 portSettings.c_cflag &= ~CSTOPB;
		 portSettings.c_cflag &= ~CSIZE;
		 portSettings.c_cflag |= CS8 | CLOCAL;
		 portSettings.c_lflag |= (ICANON);
		 //portSettings.c_lflag &= ~ICANON;
		 portSettings.c_oflag &= ~OPOST;
		 portSettings.c_cc[VMIN] = 0;
		 portSettings.c_cc[VTIME] = 250;
    	 tcflush(fd, TCIFLUSH);
    	 //cfsetospeed(&portSettings, BAUDRATE);
    	 cfsetispeed(&portSettings, BAUDRATE);
    	 tcsetattr(fd,TCSANOW,&portSettings);
    	 returnValue=RETURN_OK;
     }
     else
     {
    	 sleep(2);
    	 fprintf(stderr,"APC220 Openning port: %d, %d \r",connectionAttempts,
    			 returnValue);
    	 returnValue=RETURN_ERROR;

     }
   }

  return (returnValue);
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
	  returnValue 						  = 0;
	  aliveTasks.isReadTaskAlive		  = true;

	 returnValue = rt_task_create(&apc220_read_task,
								   "apc220_read_task",
								   0,
								   99,
								   T_JOINABLE);
	 returnValue = rt_task_create(&apc220_check_read_alive_task,
								   "apc220_check_read_alive_task",
								   0,
								   20,
								   T_JOINABLE);

if (returnValue == RETURN_OK)
	   {
		 returnValue = rt_queue_create(&acp220_inputQueue,
				 	 	 	 	 	    acp220_inputQueueName,
				  	  	  	  	  	  sizeof(inputVectors),
				  	  	  	  	       Q_UNLIMITED,
				  	  	  	  	       Q_FIFO);
	   }
	 if (returnValue == RETURN_OK)
	   {
		 returnValue=open_port();
	   }

     if (returnValue == RETURN_OK)
       {
     	  fprintf(stderr,"Open port : OK. Defined at:%d\n",fd);
     	  /*start tasks*/
     	  returnValue = rt_task_start(&apc220_read_task,
     		   	 	 	 	 	 	  &apc220_read_task_func,
     			 	 	 	 	 	  NULL);
     	  /*returnValue = rt_task_start(&apc220_check_read_alive_task,
						 			  &apc220_check_read_alive_task_func,
									  NULL);*/
       }
       else
       {

     	  fprintf(stderr,"Open_port: Unable to open /dev/ttyUSB0\n");
       }



	return (returnValue);
}/* (END) APC_220_COMM_INIT*/





