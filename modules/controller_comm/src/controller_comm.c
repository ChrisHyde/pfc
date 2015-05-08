/*
 ============================================================================
 Name        : controller_comm.c
 Author      : christopher@cthyde.com
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include "controller_comm.h"



/*global variables*/
int 	controllerStartFlag1	= 0;
int 	controllerInFlight		= 0;
int     controllerFd			= 0;


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
int xplane_controller_motors(int X,int Y,int Z,int R)
{
	int 	returnValue;
	int motor1,motor2,motor3,motor4;
	int xplane4MotorValue;
	int xplane2XMotorValue;
	int xplane2YMotorValue;
	char buffer[CONTROLLER_BUFFER_SIZE];
	char bufferMotor1[CONTROLLER_MOTOR_BUFFER_SIZE];
	char bufferMotor2[CONTROLLER_MOTOR_BUFFER_SIZE];
	char bufferMotor3[CONTROLLER_MOTOR_BUFFER_SIZE];
	char bufferMotor4[CONTROLLER_MOTOR_BUFFER_SIZE];


	/*init values*/
	returnValue		   = RETURN_ERROR;
	motor1			   = 0;
	motor2			   = 0;
	motor3			   = 0;
	motor4			   = 0;
	xplane4MotorValue  = 0;
	xplane2XMotorValue = 0;
	xplane2YMotorValue = 0;
	buffer[0]   	   = '\0';
	bufferMotor1[0]	   = '\0';
	bufferMotor2[0]	   = '\0';
	bufferMotor3[0]	   = '\0';
	bufferMotor4[0]	   = '\0';



	//fprintf(stderr,"\nZ:%d\n",Z);
	//fprintf(stderr,"flag1:%d\n",controllerStartFlag1);
	//fprintf(stderr,"Inflight:%d\n",controllerInFlight);

	//Start Fligth

	//Takeoff sequence***********************************************
	if(controllerInFlight == 0 )
	{
		if(Z == CONTROLLER_AXIS_MAX_VALUE)
		{
			controllerStartFlag1 = 1;

		}
	}
	if(controllerStartFlag1 == 1 && controllerInFlight == 0)
	{
		controllerInFlight = 1;
		//fprintf(stderr,"TAKEOFF\n");
	}



	//Landing sequence***********************************************
	if(controllerInFlight == 1 && controllerStartFlag1 == 1)
	{
		if(Z == CONTROLLER_AXIS_MIN_VALUE)
		{
			controllerStartFlag1 = 0;
			controllerInFlight = 0;
			//fprintf(stderr,"LAND\r");
		}
	}

	//Vertical Speed assignment
	if(controllerInFlight == 1 && controllerStartFlag1 == 1)
	{
		if (R==0 && X==0 && Y==0)
		{
			motor1 = HOVER_SPEED;
			motor2 = HOVER_SPEED;
			motor3 = HOVER_SPEED;
			motor4 = HOVER_SPEED;
		}
		else
		{
			//climb
			if(R<0)
			{
			xplane4MotorValue=(1)*(HOVER_SPEED+((R*(100-HOVER_SPEED))/CONTROLLER_AXIS_MIN_VALUE));
			motor1=xplane4MotorValue;
			motor2=xplane4MotorValue;
			motor3=xplane4MotorValue;
			motor4=xplane4MotorValue;
			}
			//descent
			if(R>0)
			{
			xplane4MotorValue=(HOVER_SPEED-((R*HOVER_SPEED)/CONTROLLER_AXIS_MAX_VALUE));
			motor1=xplane4MotorValue;
			motor2=xplane4MotorValue;
			motor3=xplane4MotorValue;
			motor4=xplane4MotorValue;
			}
			//yaw
			if (X<0)
			{   xplane2XMotorValue = (HOVER_SPEED+((X*(100-HOVER_SPEED))/CONTROLLER_AXIS_MIN_VALUE));
			    motor1=xplane2XMotorValue;
			    motor2=HOVER_SPEED;
			    motor3=xplane2XMotorValue;
				motor4=HOVER_SPEED;
			}
			if (X>0)
			{   xplane2XMotorValue =(HOVER_SPEED+((X*(100-HOVER_SPEED))/CONTROLLER_AXIS_MAX_VALUE));
				motor1=HOVER_SPEED;
				motor2=xplane2XMotorValue;
				motor3=HOVER_SPEED;
				motor4=xplane2XMotorValue;
			}
			//pitch
			if (Y<0)
			{   xplane2YMotorValue = (HOVER_SPEED+((Y*(100-HOVER_SPEED))/CONTROLLER_AXIS_MIN_VALUE));
				motor1=HOVER_SPEED;
				motor2=HOVER_SPEED;
				motor3=xplane2YMotorValue;
				motor4=xplane2YMotorValue;
			}
			if (Y>0)
			{   xplane2YMotorValue =(HOVER_SPEED+((Y*(100-HOVER_SPEED))/CONTROLLER_AXIS_MAX_VALUE));
				motor1=xplane2YMotorValue;
				motor2=xplane2YMotorValue;
				motor3=HOVER_SPEED;
				motor4=HOVER_SPEED;
			}
		}


	}

	if(controllerStartFlag1 == 0 && controllerInFlight == 0)
		{
			controllerInFlight = 0;
			controllerStartFlag1 = 0;
			//fprintf(stderr,"OFF\r");
		}
	//fprintf(stderr,"\nmotor1 :%d,motor2 :%d,motor3 :%d,motor4 :%d, \n",motor1,motor2,motor3,motor4);

	buffer[0] = '\0';
	sprintf(bufferMotor1, "%d", motor1);
	if(motor1<100)
	{
		bufferMotor1[2] = bufferMotor1[1];
		bufferMotor1[1] = bufferMotor1[0];
		bufferMotor1[0] = '0';
	}
	sprintf(bufferMotor2, "%d", motor2);
	if(motor2<100)
		{
				bufferMotor2[2] = bufferMotor2[1];
				bufferMotor2[1] = bufferMotor2[0];
				bufferMotor2[0] = '0';
		}
	sprintf(bufferMotor3, "%d", motor3);
	if(motor3<100)
		{
				bufferMotor3[2] = bufferMotor3[1];
				bufferMotor3[1] = bufferMotor3[0];
				bufferMotor3[0] = '0';
		}
	sprintf(bufferMotor4, "%d", motor4);
	if(motor4<100)
		{
				bufferMotor4[2] = bufferMotor4[1];
				bufferMotor4[1] = bufferMotor4[0];
				bufferMotor4[0] = '0';
		}

	strcpy(buffer,bufferMotor1);
	strcat(buffer,bufferMotor2);
	strcat(buffer,bufferMotor3);
	strcat(buffer,bufferMotor4);

	//fprintf(stderr,"sending...%s\n",buffer);

	returnValue = rt_queue_write(&controller_inputQueue,
								 &buffer,
								 sizeof(buffer),
								 Q_URGENT);

	return(returnValue);
}/*(END) XPLANE_CONTROLLER_MOTORS*/



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
	int 	controllerBytesRead;
	bool    connected;
	struct 	js_event jse;
	int 	*axis;
	int 	num_of_axis;
	int 	num_of_buttons;
	char 	name_of_controller[80];

	/*init values*/
	returnValue		      = RETURN_ERROR;
	controllerBytesRead   = 0;
	axis				  = NULL;
	connected 			  = FALSE;
    num_of_axis			  = 0;
	num_of_buttons		  = 0;
	name_of_controller[0]   = '\0';


  rt_task_set_periodic(NULL, TM_NOW, 1000*1000*10);

  while(1)
  {

	  if(!connected)
	  {
		  controllerFd = open (CONTROLLER_ADDRESS_PORT, O_RDONLY);
		 if(controllerFd>=0)
		 {
			 /*Configure settings in address struct*/
			 ioctl( controllerFd, JSIOCGAXES, &num_of_axis );
			 ioctl( controllerFd, JSIOCGBUTTONS, &num_of_buttons );
			 ioctl( controllerFd, JSIOCGNAME(80), &name_of_controller );
			 axis = (int *) calloc( num_of_axis, sizeof( int ) );
			 returnValue = RETURN_OK;
			 fprintf(stderr,"Open Controller port: OK.\n");
			 fprintf(stderr,"Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
								 , name_of_controller
								 , num_of_axis
								 , num_of_buttons );
			 connected = TRUE;
		 }
	  }



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
			 //fprintf(stderr, "X: %6d  Y: %6d Z: %6d  R: %6d  \r ", axis[0], axis[1], axis[2], axis[3] );

             xplane_controller_motors(axis[0], axis[1], axis[2], axis[3]);
		}
	}

   else
   {
	   fprintf(stderr,"Open Controller port: Error.\r");
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
 int controller_comm_init()
{
	  int returnValue;

	  /*init values*/
	  returnValue 	= RETURN_OK;

	 returnValue = rt_task_create(&controller_read_task,
								   "controller_read_task",
								   0,
								   10,
								   T_JOINABLE);



	 if (returnValue == RETURN_OK)
	   {
		 returnValue = rt_queue_create(&controller_inputQueue,
				 	 	 	 	 	 	controller_inputQueueName,
				 	 	 	 	 	 	CONTROLLER_BUFFER_SIZE,
				 	 	 	 	 	 	Q_UNLIMITED,
				 	 	 	 	 	 	Q_SHARED);
	   }

     if (returnValue == RETURN_OK)
       {

     	  /*start tasks*/

     	  returnValue = rt_task_start(&controller_read_task,
     	      		   	 	 	 	 &controller_read_task_func,
     	      			 	 	 	 NULL);

       }
       else
       {

     	  fprintf(stderr,"Controller module error\n");
       }



	return (returnValue);
}/* (END) XPLANE_COMM_INIT*/



