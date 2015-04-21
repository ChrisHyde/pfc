/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : Reas input from apc220 and writes to xplaneOutputQueue the values read
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/
void apc220_interface_read_task_func(void *arg)
{
	int	returnValue;
	int bytesRead;

	/*init values*/
	returnValue = RETURN_OK;
	bytesRead   = 0;

	    rt_task_set_periodic(NULL, TM_NOW, TM_INFINITE);

	while(1)
	{
	if(returnValue==RETURN_OK)
	{
	 if (returnValue == RETURN_OK)
		   {
			  /*returnValue= rt_queue_bind (&acp220_inputQueue,
										  acp220_inputQueueName,
										  TM_INFINITE);*/
		   }

		  if (returnValue == RETURN_OK)
		  {
			apc220_inputQueueValues=malloc(sizeof(*apc220_inputQueueValues));
			bytesRead=rt_queue_read(&acp220_inputQueue,
									apc220_inputQueueValues,
									sizeof(*apc220_inputQueueValues),
									TM_INFINITE);
			//fprintf(stderr," %f\n",apc220_inputQueueValues->GyY);
				   fprintf(stderr,"Data recieved:%f<--->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%d<---->%d\n",
											apc220_inputQueueValues->AcX,
											apc220_inputQueueValues->AcY,
											apc220_inputQueueValues->AcZ,
											apc220_inputQueueValues->GyX,
											apc220_inputQueueValues->GyY,
											apc220_inputQueueValues->GyZ,
											apc220_inputQueueValues->M1,
											apc220_inputQueueValues->M2,
											apc220_inputQueueValues->M3,
											apc220_inputQueueValues->M4,
											bytesRead,
											sizeof(apc220_inputQueueValues));
			/* fprintf(stderr,"Motors recieved:%f<--->%f<---->%f<---->%f\n",
								apc220_inputQueueValues->M1,
								apc220_inputQueueValues->M2,
								apc220_inputQueueValues->M3,
								apc220_inputQueueValues->M4);*/
			if(bytesRead<0)
			{
				returnValue=RETURN_ERROR;
			}
			else
			{
				 /*TODO: transform value*/
				xplane_outputQueueValues.AcX=apc220_inputQueueValues->AcX;
				xplane_outputQueueValues.AcY=apc220_inputQueueValues->AcY;
				xplane_outputQueueValues.AcZ=apc220_inputQueueValues->AcZ;
				xplane_outputQueueValues.GyX=apc220_inputQueueValues->GyX;
				xplane_outputQueueValues.GyY=apc220_inputQueueValues->GyY;
				xplane_outputQueueValues.GyZ=apc220_inputQueueValues->GyZ;
				xplane_outputQueueValues.M1=apc220_inputQueueValues->M1/100;
				xplane_outputQueueValues.M2=apc220_inputQueueValues->M2/100;
				xplane_outputQueueValues.M3=apc220_inputQueueValues->M3/100;
				xplane_outputQueueValues.M4=apc220_inputQueueValues->M4/100;//xplane engine thrust 0 to 1 range, arduino 0 to 100 range
				/*fprintf(stderr,"SENDING:%f<--->%f<---->%f<---->%f<---->%f<---->%f<---->%d<---->%d\n",
						xplane_outputQueueValues.AcX,
						xplane_outputQueueValues.AcY,
						xplane_outputQueueValues.AcZ,
						xplane_outputQueueValues.GyX,
						xplane_outputQueueValues.GyY,
						xplane_outputQueueValues.GyZ,
						bytesRead,
						sizeof(ST_Xplane_outputQueue));*/


				free(apc220_inputQueueValues);
						 //treatvalues();
				 if (returnValue == RETURN_OK)
				 {
					 if(SIMULATOR_ACTIVE==1)
					 {
				        /*returnValue = rt_queue_write(&xplane_outputQueue,
				        							&xplane_outputQueueValues,
											       sizeof(xplane_outputQueueValues),
											       Q_URGENT);*/
					 }
				 }
			}


		  }

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
void xplane_interface_read_task_func(void *arg)
{
	int	returnValue;
	int bytesRead;
    int failCounter;
	/*init values*/
	returnValue = RETURN_OK;
	bytesRead   = 0;
	failCounter = 0;


	rt_task_set_periodic(NULL, TM_NOW, 20000000);

	while(1)
	{
		 if (returnValue == RETURN_OK)
			 {
			 returnValue= rt_queue_bind (&xplane_inputQueue,
										 xplane_inputQueueName,
										 TM_INFINITE);
			 }
		 xplane_inputQueueValues = malloc(sizeof(*xplane_inputQueueValues));
		 if (returnValue == RETURN_OK)
		 {
		 bytesRead=rt_queue_read(&xplane_inputQueue,
								 xplane_inputQueueValues,
								 sizeof(*xplane_inputQueueValues),
								 TM_NONBLOCK);
		 }
		 /*fprintf(stderr,"Data recieved:%f<--->%f %f %f %f<---->%d----->%d\n",
		 				 xplane_inputQueueValues->xplaneInputQueueDecimalValues[0][0],
		 				 xplane_inputQueueValues->xplaneInputQueueDecimalValues[1][3],
		 				 xplane_inputQueueValues->xplaneInputQueueDecimalValues[2][0],
		 				 xplane_inputQueueValues->xplaneInputQueueDecimalValues[3][0],
		 				 xplane_inputQueueValues->xplaneInputQueueDecimalValues[2][8],
		 				bytesRead,
		 				returnValue);*/

		 free(xplane_inputQueueValues);
		 if(bytesRead==-11)
			{
				failCounter++;
			}
		 else if(bytesRead<0 || failCounter>5)
		 {
			 failCounter=0;
			 returnValue=RETURN_ERROR;
		 }

		rt_task_wait_period(NULL);
	}

}/*(END) XPLANE_INTERFACE_READ_TASK_FUNC*/
