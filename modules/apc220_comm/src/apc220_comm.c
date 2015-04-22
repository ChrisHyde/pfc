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

int 	apcFd			= 0;
int     controllerFd	= 0;
int     loopCompleted   = 0;
int     sendToXplaneCounter = 0;
bool    PANIC_MESSAGE   = false;



int udpWriteSocket		= 0;
struct sockaddr_in 		xplaneWriteAddress;
socklen_t 				addr_size;



ST_alivetasks  		 	aliveTasks;
ST_apc220_inputVectors  inputVectors;
//struct js_event 		jse;

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
int sendToXplane(ST_apc220_inputVectors inputVectors)
{

    int returnValue;
	char buffer[BUFFERSIZE];

	/*init values*/
	returnValue = RETURN_OK;
	buffer[0] 	= '\0';


		             union UN_Byte_float_transformation data;

		            // fprintf(stderr,"buffer %f\n",inputVectors.M2);
		             //BUILD UDP MESSAGE
		             //HEADER
		             strcpy(data.byteValue,UDP_HEADER_INSTRUCTION);
		             buffer[0]=data.byteValue[0];
		             buffer[1]=data.byteValue[1];
		             buffer[2]=data.byteValue[2];
		             buffer[3]=data.byteValue[3];
		             buffer[UDP_HEADER_SIZE]=0;
		             buffer[UDP_INDEX_START]=XPLANE_ANGULAR_VELOCITY_INDEX;
		             buffer[UDP_INDEX_START+1]=0;
		             buffer[UDP_INDEX_START+2]=0;
		             buffer[UDP_INDEX_START+3]=0;
		             data.floatValue = inputVectors.GyX;
		             buffer[UDP_DATA_START]=data.byteValue[0];
		             buffer[UDP_DATA_START+1]=data.byteValue[1];
		             buffer[UDP_DATA_START+2]=data.byteValue[2];
		             buffer[UDP_DATA_START+3]=data.byteValue[3];
		             data.floatValue = inputVectors.GyY;
		             buffer[UDP_DATA_START+UDP_DATA_SIZE]=data.byteValue[0];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+1]=data.byteValue[1];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+2]=data.byteValue[2];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+3]=data.byteValue[3];
					 data.floatValue = inputVectors.GyZ;
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE]=data.byteValue[0];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE+1]=data.byteValue[1];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE+2]=data.byteValue[2];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE+3]=data.byteValue[3];
					 data.floatValue = XPLANE_UNSED_VALUE;
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*3)]=data.byteValue[0];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*3)+1]=data.byteValue[1];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*3)+2]=data.byteValue[2];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*3)+3]=data.byteValue[3];
					 data.floatValue = XPLANE_UNSED_VALUE;
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*4)]=data.byteValue[0];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*4)+1]=data.byteValue[1];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*4)+2]=data.byteValue[2];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*4)+3]=data.byteValue[3];
					 data.floatValue =  XPLANE_UNSED_VALUE;
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*5)]=data.byteValue[0];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*5)+1]=data.byteValue[1];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*5)+2]=data.byteValue[2];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*5)+3]=data.byteValue[3];
					 data.floatValue = XPLANE_UNSED_VALUE;
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*6)]=data.byteValue[0];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*6)+1]=data.byteValue[1];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*6)+2]=data.byteValue[2];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*6)+3]=data.byteValue[3];
					 data.floatValue = XPLANE_UNSED_VALUE;
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*7)]=data.byteValue[0];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*7)+1]=data.byteValue[1];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*7)+2]=data.byteValue[2];
					 buffer[UDP_DATA_START+(UDP_DATA_SIZE*7)+3]=data.byteValue[3];

					 //SECOND DATA
					 buffer[UDP_SECOND_INDEX_START]=XPLANE_ENGINE_POWER_INDEX;
					 buffer[UDP_SECOND_INDEX_START+1]=0;
					 buffer[UDP_SECOND_INDEX_START+2]=0;
					 buffer[UDP_SECOND_INDEX_START+3]=0;
					 data.floatValue = inputVectors.M1;
					 buffer[UDP_SECOND_DATA_START]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+3]=data.byteValue[3];
					 data.floatValue = inputVectors.M2;
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+3]=data.byteValue[3];
					 data.floatValue = inputVectors.M3;
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE+3]=data.byteValue[3];
					 data.floatValue = inputVectors.M4;
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*3)]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*3)+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*3)+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*3)+3]=data.byteValue[3];
					 data.floatValue = XPLANE_UNSED_VALUE;
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*4)]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*4)+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*4)+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*4)+3]=data.byteValue[3];
					 data.floatValue =  XPLANE_UNSED_VALUE;
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*5)]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*5)+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*5)+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*5)+3]=data.byteValue[3];
					 data.floatValue = XPLANE_UNSED_VALUE;
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*6)]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*6)+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*6)+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*6)+3]=data.byteValue[3];
					 data.floatValue = XPLANE_UNSED_VALUE;
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*7)]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*7)+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*7)+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+(UDP_DATA_SIZE*7)+3]=data.byteValue[3];

					 fprintf(stderr,"%d\n", sendToXplaneCounter++);

		  sendto(udpWriteSocket,buffer,BUFFERSIZE,0,(struct sockaddr *)&xplaneWriteAddress,addr_size);
		  tcflush(udpWriteSocket,TCIOFLUSH);
		  memset(&buffer[0], 0, sizeof(buffer));
         return returnValue;


}
/* (END) SEND_TO_XPLANE*/



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
void controller_read_task_func(void *arg)
{
	int 	returnValue;
	int 	connectionAttempts;
	int 	controllerBytesRead;
	struct 	js_event jse;
	int 	*axis;
	int 	num_of_axis;
	int 	num_of_buttons;
	char 	name_of_controller[80];

	/*init values*/
	returnValue		      = RETURN_ERROR;
	connectionAttempts    = 0;
	controllerBytesRead   = 0;
	axis				  = NULL;
    num_of_axis			  = 0;
	num_of_buttons		  = 0;
	name_of_controller[0]   = '\0';


	  while (connectionAttempts < MAX_CONNECTION_ATTEMPTS && returnValue == RETURN_ERROR)
	   {
		/*Open Port */
		  controllerFd = open (CONTROLLER_ADDRESS_PORT, O_RDONLY);
		  fprintf(stderr,"Open Controller port: OK.\n");
		 if(controllerFd>=0)
		 {
			 /*Configure settings in address struct*/
			 ioctl( controllerFd, JSIOCGAXES, &num_of_axis );
			 ioctl( controllerFd, JSIOCGBUTTONS, &num_of_buttons );
			 ioctl( controllerFd, JSIOCGNAME(80), &name_of_controller );
			 axis = (int *) calloc( num_of_axis, sizeof( int ) );

			 returnValue = RETURN_OK;
		 }
		 else
		 {
			 sleep(2);
			 fprintf(stderr," Openning port Failed: %d, %d \r",connectionAttempts,
					 returnValue);
			 returnValue = RETURN_ERROR;
		 }
	   }

	  fprintf(stderr,"Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
	                 , name_of_controller
	                 , num_of_axis
	                 , num_of_buttons );


  rt_task_set_periodic(NULL, TM_NOW, TM_INFINITE);

  while(1)
  {
   if (returnValue == RETURN_OK)
   {
        controllerBytesRead = read(controllerFd, &jse, sizeof(jse));

		if (controllerBytesRead < 0)
		{
			returnValue=RETURN_ERROR;
		}

		else
		{
			 returnValue = RETURN_OK;

			 switch (jse.type & ~JS_EVENT_INIT)
			 		{
			 			case JS_EVENT_AXIS:
			 				axis   [ jse.number ] = jse.value;
			 				break;
			 		}
			 fprintf(stderr, "X: %6d  Y: %6d Z: %6d  R: %6d  \r ", axis[0], axis[1], axis[2], axis[3] );
             fflush(stdout);
		}
	}



      tcflush(controllerFd,TCIOFLUSH);
   	  rt_task_wait_period(NULL);
  }



  close(controllerFd);

}/*(END) CONTROLLER_READ_TASK_FUNC*/

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
	int connectionAttempts;



	/*init values*/
    returnValue				   = RETURN_ERROR;
	length		               = 0;
	connectionAttempts 		   = 0;
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



	  while (connectionAttempts < MAX_CONNECTION_ATTEMPTS && returnValue == RETURN_ERROR)
	   {


		/*Create UDP socket*/
		  udpWriteSocket = socket(PF_INET, SOCK_DGRAM, 0);
		  fprintf(stderr,"Open Xplane WSocket: OK.\n");
		 if(udpWriteSocket>=0)
		 {
			 /*Configure settings in address struct*/
			 xplaneWriteAddress.sin_family = AF_INET;
			 xplaneWriteAddress.sin_port = htons(XPLANE_SEND_PORT);
			 xplaneWriteAddress.sin_addr.s_addr = inet_addr(XPLANE_IP_ADDRESS);
			 memset(xplaneWriteAddress.sin_zero,'\0', sizeof(xplaneWriteAddress.sin_zero));

			 /*Initialize size variable to be used later on*/
			 addr_size = sizeof xplaneWriteAddress;
			 returnValue = RETURN_OK;
		 }
		 else
		 {
			 sleep(2);
			 fprintf(stderr," Openning socket Failed: %d, %d \r",connectionAttempts,
					 returnValue);
			 returnValue=RETURN_ERROR;
		 }
	   }


	rt_task_set_periodic(NULL, TM_NOW, TM_INFINITE);

	  while(1)
	  {

		  if (returnValue == RETURN_OK)
		  {
		  buffer[0] 	  = '\0';
		  length		  = 0;
		  num 			  = 0.0;
		  fcntl(apcFd,F_SETFL,0);
		  aliveTasks.isReadTaskAlive=false;
		  length = read(apcFd, &buffer, sizeof(buffer));
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
						//fprintf(stderr,"acx:%f--->acy:%f--->acz:%f--->gyx:%f--->gyy:%f--->gyz:%f--->"
							//	"motor1:%f--->motor2:%f--->motor3:%f--->motor4:%f\n\n",acx,acy, acz, gyx, gyy, gyz,motor1,motor2,motor3,motor4);

						returnValue = sendToXplane(inputVectors);
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

          tcflush(apcFd,TCIOFLUSH);
	      rt_task_wait_period(NULL);
          }
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
   apcFd = open(PORT, O_RDWR | O_NOCTTY | O_NONBLOCK | O_FSYNC);
     if(apcFd>=0)
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
    	 tcflush(apcFd, TCIFLUSH);
    	 //cfsetospeed(&portSettings, BAUDRATE);
    	 cfsetispeed(&portSettings, BAUDRATE);
    	 tcsetattr(apcFd,TCSANOW,&portSettings);
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

	 returnValue = rt_task_create(&controller_read_task,
								   "controller_read_task",
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
		 returnValue=open_port();
	   }

     if (returnValue == RETURN_OK)
       {
     	  fprintf(stderr,"Open Apc220 port: OK.\n");
     	  /*start tasks*/
     	  returnValue = rt_task_start(&apc220_read_task,
     		   	 	 	 	 	 	  &apc220_read_task_func,
     			 	 	 	 	 	  NULL);
     	  /*returnValue = rt_task_start(&apc220_check_read_alive_task,
						 			  &apc220_check_read_alive_task_func,
									  NULL);*/
       }
     if (returnValue == RETURN_OK)
	   {
		 returnValue = rt_task_start(&controller_read_task,
									  &controller_read_task_func,
									  NULL);
	   }

       else
       {

     	  fprintf(stderr,"Open_port: Unable to open /dev/ttyUSB0\n");
       }



	return (returnValue);
}/* (END) APC_220_COMM_INIT*/





