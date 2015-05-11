
#include "autopilot.h"
#include <math.h>
#include <stdio.h>

ST_Queue_Packet queue_Packet;


/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : Starts the readXplane tasks and the output queue if SIMULATOR_ACTIVE = true
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/
void read_input_queues_task_func(void *arg)
{
	int	returnValue;
    int apc220bytesRead;
    int controllerBytesRead;
    int panelBytesRead;
    char acp220buffer[APC220_OUTPUT_BUFFER_SIZE];
    char controllerBuffer[CONTROLLER_OUTPUT_BUFFER_SIZE];
    float panelBuffer[4];
    //char xplaneBuffer[XPLANE_OUTPUT_BUFFER_SIZE];
    char stringMotor1[3];
    char stringMotor2[3];
    char stringMotor3[3];
    char stringMotor4[3];
    float m1,m2,m3,m4;
    int controllerAliveCounter;

	/*init values*/
	returnValue 			= RETURN_OK;
	apc220bytesRead			= 0;
	controllerBytesRead 	= 0;
	panelBytesRead 			= 0;
	controllerAliveCounter 	= 0;
	acp220buffer[0]			= '\0';
	controllerBuffer[0] 	= '\0';
	panelBuffer[0] 			= '\0';
	//xplaneBuffer[0] 		= '\0';
	stringMotor1[0]			= '\0';
	stringMotor2[0]			= '\0';
	stringMotor3[0]			= '\0';
	stringMotor4[0]			= '\0';
	m1						= 0.0;
	m2						= 0.0;
	m3						= 0.0;
	m4						= 0.0;

	returnValue= rt_queue_bind (&acp220_inputQueue,
							   acp220_inputQueueName,
							   TM_NONBLOCK);
	 returnValue= rt_queue_bind (&controller_inputQueue,
							   controller_inputQueueName,
							   TM_NONBLOCK);
   if (returnValue == RETURN_OK)
	{
   returnValue= rt_queue_bind (&read_from_panel_queue,
							   read_from_panel_queue_Name,
							   TM_NONBLOCK);
	}

	rt_task_set_periodic(NULL, TM_NOW, 1000*1000*10);

	  while(1)
	    {
			apc220bytesRead		= 0;
			controllerBytesRead = 0;
			acp220buffer[0]		= '\0';
			panelBuffer[0] 		= '\0';
			controllerBuffer[0] = '\0';
			//xplaneBuffer[0] 	= '\0';
			stringMotor1[0]		= '\0';
			stringMotor2[0]		= '\0';
			stringMotor3[0]		= '\0';
			stringMotor4[0]		= '\0';

			m1					= 0.0;
			m2					= 0.0;
			m3					= 0.0;
			m4					= 0.0;
		   if (returnValue == RETURN_OK)
		   {


			   if (returnValue == RETURN_OK)
			   {
				   apc220bytesRead=rt_queue_read(&acp220_inputQueue,
											acp220buffer,
											sizeof(acp220buffer),
											TM_NONBLOCK);
/*******************************************************************************************************************
 ******************************************************************************************************************/

				   //APC220 Disconnected /////////////////////////////////////////////////////
				   if(apc220bytesRead<=0)
				   {



					   if (returnValue == RETURN_OK)
					   {

						   controllerBytesRead=rt_queue_read(&controller_inputQueue,
															 controllerBuffer,
															 sizeof(controllerBuffer),
															 TM_NONBLOCK);
						   if (returnValue == RETURN_OK)
						   	{
							   panelBytesRead=rt_queue_read(&read_from_panel_queue,
									   	   	   	   	   	   panelBuffer,
															 sizeof(panelBuffer),
															 TM_NONBLOCK);
						   	}
						   //Controller Disconnected/////////////////
						   if(controllerBytesRead<=0)
						   {
							   controllerAliveCounter++;
							   //make sure controller is inactive
							    if (controllerAliveCounter > 50)
							    {
							    	if (panelBytesRead<=0)
							    	{
							    		//returnError
							    	}
							    	else
							    	{
							    	   queue_Packet.AcX 	= XPLANE_UNSED_VALUE;
									   queue_Packet.AcY		= XPLANE_UNSED_VALUE;
									   queue_Packet.AcZ		= XPLANE_UNSED_VALUE;
									   queue_Packet.Pitch	= XPLANE_UNSED_VALUE;
									   queue_Packet.Roll	= XPLANE_UNSED_VALUE;
									   queue_Packet.Yaw		= XPLANE_UNSED_VALUE;

									   if(m1<=1.0 && m2<=1.0 && m3<=1.0 && m4<=1.0)
									   {
									   queue_Packet.M1=panelBuffer[0]/100;
									   queue_Packet.M2=panelBuffer[1]/100;
									   queue_Packet.M3=panelBuffer[2]/100;
									   queue_Packet.M4=panelBuffer[3]/100;
									   }
							    	}
							    }

						   }
						   //Controller Connected/////////////////
						   else
						   {

							   controllerAliveCounter=0;
							   if (strlen(controllerBuffer)>8){
							   stringMotor1[0]=controllerBuffer[0];
							   stringMotor1[1]=controllerBuffer[1];
							   stringMotor1[2]=controllerBuffer[2];
							   stringMotor2[0]=controllerBuffer[3];
							   stringMotor2[1]=controllerBuffer[4];
							   stringMotor2[2]=controllerBuffer[5];
							   stringMotor3[0]=controllerBuffer[6];
							   stringMotor3[1]=controllerBuffer[7];
							   stringMotor3[2]=controllerBuffer[8];
							   stringMotor4[0]=controllerBuffer[9];
							   stringMotor4[1]=controllerBuffer[10];
							   stringMotor4[2]=controllerBuffer[11];

							   m1=atof(stringMotor1)/100;
							   m2=atof(stringMotor2)/100;
							   m3=atof(stringMotor3)/100;
							   m4=atof(stringMotor4)/100;
							   }
							   else
							   {
								   stringMotor1[0]=controllerBuffer[0];
								   stringMotor1[1]=controllerBuffer[1];
								   stringMotor2[0]=controllerBuffer[2];
								   stringMotor2[1]=controllerBuffer[3];
								   stringMotor3[0]=controllerBuffer[4];
								   stringMotor3[1]=controllerBuffer[5];
								   stringMotor4[0]=controllerBuffer[6];
								   stringMotor4[1]=controllerBuffer[7];
								   m1=atof(stringMotor1)/1000;
								   m2=atof(stringMotor2)/1000;
								   m3=atof(stringMotor3)/1000;
								   m4=atof(stringMotor4)/1000;
							   }
							   queue_Packet.AcX 	= XPLANE_UNSED_VALUE;
							   queue_Packet.AcY		= XPLANE_UNSED_VALUE;
							   queue_Packet.AcZ		= XPLANE_UNSED_VALUE;
							   queue_Packet.Pitch	= XPLANE_UNSED_VALUE;
							   queue_Packet.Roll	= XPLANE_UNSED_VALUE;
							   queue_Packet.Yaw		= XPLANE_UNSED_VALUE;

							   if(m1<=1.0 && m2<=1.0 && m3<=1.0 && m4<=1.0)
							   {
							   queue_Packet.M1=m1;
							   queue_Packet.M2=m2;
							   queue_Packet.M3=m3;
							   queue_Packet.M4=m4;
							   }



						   }
						      /* fprintf(stderr," %f\n",queue_Packet.M1);
							   fprintf(stderr," %f\n",queue_Packet.M2);
							   fprintf(stderr," %f\n",queue_Packet.M3);
							   fprintf(stderr," %f\n\n\n",queue_Packet.M4);*/
						   returnValue = rt_queue_write(&write_to_xplane_queue,
														 &queue_Packet,
														 sizeof(queue_Packet),
														 Q_URGENT);
						   returnValue=rt_queue_flush(&read_from_panel_queue);
						   returnValue=rt_queue_flush(&acp220_inputQueue);
                           }


                               //send controller Data to Xplane
							   //send Xplane to Qt
							   //send Qt to Xplane
				    }


/*******************************************************************************************************************
******************************************************************************************************************/
				   //APC220 Connected /////////////////////////////////////////////////////
				   else
				   {

					   //send controller Data to APC220
					   //send Qt to Apc220
					   //send APC220 data to xplane & Qt
				   }
			   }
			   else
			   {
				   returnValue = RETURN_ERROR;
				   //APC queue binding error
			   }

		   }
		   else
		   {

		   }

	        rt_task_wait_period(NULL);
	    }


}/*(END) READ_INPUT__QUEUES_TASK_FUNC*/


int main (int argc, char *argv[])
{
	  int returnValue;


	  /*init values*/
	  returnValue 			    = RETURN_OK;


	 mlockall(MCL_CURRENT | MCL_FUTURE);

	 /*Create Tasks*////////////////////////////////////////////////
	  if (returnValue == RETURN_OK)
	  {
	  returnValue = rt_task_create(&read_input_queues_task,
								  "read_input_queues_task",
								  0,
								  5,
								  T_JOINABLE);
	  }

	  /*Create Output Queues*//////////////////////////////////////
	  if (returnValue == RETURN_OK)
	  {
	  returnValue = rt_queue_create(&acp220_outputQueue,
			  	  	  	  	  	    acp220_outputQueueName,
			  	  	  	  	        APC220_OUTPUT_BUFFER_SIZE,
								    Q_UNLIMITED,
								    Q_SHARED);
	  }
	  if (returnValue == RETURN_OK)
	  {
	  returnValue = rt_queue_create(&write_to_xplane_queue,
			  	  	  	  	  	  	write_to_xplane_queue_Name,
			  	  	  	  	  	  	XPLANE_OUTPUT_BUFFER_SIZE,
								    Q_UNLIMITED,
								    Q_SHARED);
	  }




	  /*Initialize Modules */////////////////////////////////////////
	  if (returnValue == RETURN_OK)
	  {
	    returnValue = apc220_comm_init();
	  }

	  if (returnValue == RETURN_OK)
	  {
		  returnValue = controller_comm_init();
	  }
	  if (returnValue == RETURN_OK)
	  {
	     returnValue = xplane_comm_init();
	  }
	  if (returnValue == RETURN_OK)
	  {
		 returnValue = panel_comm_init();
	  }
	  if (returnValue == RETURN_OK)
	  {

	  returnValue = rt_task_start(&read_input_queues_task,
	  							  &read_input_queues_task_func,
	  							  NULL);
	  }

	  else
	  {
		  fprintf(stderr,"retunrValue %d\n",returnValue);
	  }

	while(1)
	  {
        /*do nothing*/
	  }

		fprintf(stderr,"retunrValue %d\n",returnValue);

	return (returnValue);
}
