/*
 ============================================================================
 Name        : xplane_comm.c
 Author      : christopher@cthyde.com
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include "xplane_comm.h"



/*global variables*/

int udpWriteSocket	= 0;
int udpReadSocket	= 0;

uint8_t xplaneByteInputValues[XPLANE_BASIC_INDEXES_USED][UDP_DATA_TOTAL_SIZE+1];
ST_Xplane_inputDecimalVectors  xplaneInputDecimalValues;

ST_Xplane_outputDecimalVectors *xplaneOutputDecimalValues = NULL;

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
int byte_to_float_func(uint8_t xplaneByteInputValues[XPLANE_BASIC_INDEXES_USED][UDP_DATA_TOTAL_SIZE+1])
{
	int returnValue;
	int indexCounter;
	int pos,byteCounter, counter8floats;
	uint8_t inByte[4]={0,0,0,0};
	float f1;

	/*init values*/
	returnValue		 = RETURN_ERROR;
	indexCounter	 = 0;
	byteCounter		 = 0;
	f1			 	 = 0;
	counter8floats	 = 0;

	/*Store Indexes*/
	xplaneInputDecimalValues.xplaneInputDecimalValues[0][0] = xplaneByteInputValues[0][0];
	xplaneInputDecimalValues.xplaneInputDecimalValues[1][0] = xplaneByteInputValues[1][0];
	xplaneInputDecimalValues.xplaneInputDecimalValues[2][0] = xplaneByteInputValues[2][0];
	xplaneInputDecimalValues.xplaneInputDecimalValues[3][0] = xplaneByteInputValues[3][0];

	for(indexCounter=0;indexCounter<XPLANE_BASIC_INDEXES_USED;indexCounter++)
	{
		counter8floats = 0;
		byteCounter    = 0;

        for(pos=1;pos<UDP_DATA_TOTAL_SIZE+1;pos++)
		 {
        	 inByte[byteCounter]=xplaneByteInputValues[indexCounter][pos];
        	 byteCounter++;
        	 if(byteCounter==UDP_DATA_SIZE)
        	 {
        		 //Convert byte array to integer
        		 counter8floats++;
        		 memcpy(&f1, inByte, sizeof f1);
        		 if(f1==XPLANE_UNSED_VALUE)
        		 {
        			 f1=0.0;

        		 }
        		 xplaneInputDecimalValues.xplaneInputDecimalValues[indexCounter][counter8floats]=f1;
        		 byteCounter = 0;
        		 returnValue		 = RETURN_OK;
        	 }
		 }
	}
return returnValue;
}/*(END) BYTE_TO_FLOAT_FUNC*/





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
void xplane_read_task_func(void *arg)
{

	char buffer[BUFFERSIZE];
	char UdpHeader[UDP_HEADER_SIZE];
	//char UdpInbyteValues[UDP_DATA_SIZE];
	int returnValue;
	int connectionAttempts;
	int headerPosCounter;
	int dataPosCounter;
	int indexCounter;
	int bytesRecieved;
	int currentDataPos;
	struct sockaddr_in xplaneReadAddress;
	socklen_t addr_size;



	/*init values*/
	connectionAttempts 			= 0;
	bytesRecieved	   			= 0;
	headerPosCounter   			= 0;
	dataPosCounter			    = 0;
	indexCounter				= 0;
	currentDataPos				= 0;
	buffer[BUFFERSIZE]			= '\0';
	UdpHeader[UDP_HEADER_SIZE]  = '\0';
	returnValue        			= RETURN_OK;


	    /*Create UDP socket*/
	      udpReadSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	     if(udpReadSocket>=0)
	     {
             /*Configure settings in address struct*/
	    	 memset((char*) &xplaneReadAddress, 0, sizeof(xplaneReadAddress));

	    	 xplaneReadAddress.sin_family      = AF_INET;
	    	 xplaneReadAddress.sin_port        = htons(XPLANE_RECIEVE_PORT);
	    	 xplaneReadAddress.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);

	    	 /*Initialize size variable to be used later on*/
	    	 addr_size   = sizeof xplaneReadAddress;
	    	 returnValue = bind(udpReadSocket,
	    			 	 	 	(struct sockaddr*)&xplaneReadAddress,
	    			 	 	 	sizeof(xplaneReadAddress));
	     }
	     else
	     {
	    	 sleep(2);
	    	 fprintf(stderr," Openning socket Failed: %d, %d \r",
	    			 connectionAttempts,returnValue);
	    	 returnValue=RETURN_ERROR;
	     }

if(returnValue==RETURN_OK)
{
  rt_task_set_periodic(NULL, TM_NOW, 2000*1000*10);

  while(1)
  {
	  fflush(stdout);
	  fflush(stderr);
	  memset(buffer,'\0', BUFFERSIZE);
	  memset(UdpHeader,'\0', UDP_HEADER_SIZE);
	  bytesRecieved = recvfrom(udpReadSocket,
							   buffer,
							   BUFFERSIZE,
							   0,
							   (struct sockaddr *) &xplaneReadAddress,
							   &addr_size);
	if(bytesRecieved<0)
	{
	 returnValue=RETURN_ERROR;
	}
	else
	{
	 returnValue=RETURN_OK;
	}

    if(returnValue==RETURN_OK)
    {
    for(headerPosCounter=0;headerPosCounter<UDP_HEADER_SIZE;headerPosCounter++)
	{
	  UdpHeader[headerPosCounter]=buffer[headerPosCounter];

	}
	  UdpHeader[UDP_HEADER_SIZE]='\0';
	  currentDataPos=UDP_INDEX_START;

	  if(strcmp(UdpHeader,UDP_HEADER_INSTRUCTION)==RETURN_OK)
	  {
	    for(indexCounter=0;indexCounter<XPLANE_BASIC_INDEXES_USED;indexCounter++)
		{
	    	xplaneByteInputValues[indexCounter][0]=buffer[currentDataPos];
	    	//fprintf(stderr,"\n Index: %d \n",buffer[currentDataPos]);
	        currentDataPos+=UDP_INDEX_SIZE;


              //UDP_DATA_TOTAL_SIZE + 1 added because first position is for index
	      for(dataPosCounter=1;dataPosCounter<UDP_DATA_TOTAL_SIZE+1;dataPosCounter++)
	      {
	    	  xplaneByteInputValues[indexCounter][dataPosCounter]=buffer[currentDataPos];
	    	  currentDataPos++;
	    	  //fprintf(stderr," %d ",xplaneByteInputValues[indexCounter][dataPosCounter]);
	      }

		}
	    returnValue = byte_to_float_func(xplaneByteInputValues);
				/*int i,j=0;
				for (i=0;i<XPLANE_BASIC_INDEXES_USED;i++)
				{
					for(j=0;j<UDP_DATA_GROUPS+1;j++)
					{
						fprintf(stderr,"%f\n",xplaneInputDecimalValues.xplaneInputDecimalValues[i][j]);
					}
				}
				fprintf(stderr,"\n\n");*/

		   if (returnValue==RETURN_OK)
		   {
			  returnValue = rt_queue_write(&read_from_xplane_queue,
										 &xplaneInputDecimalValues,
										 sizeof(xplaneInputDecimalValues),
										 Q_URGENT);
		   }
      }
	  else
	  {
       //Do nothing
	  }
	}


    tcflush(udpReadSocket,TCIOFLUSH);
    fflush(stdout);
    fflush(stderr);
    rt_task_wait_period(NULL);
  }
 }
}
/* (END) XPLANE_READ_TASK_FUNC*/


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
void xplane_write_task_func(void *arg)
{
	char buffer[BUFFERSIZE];
	int returnValue;
	int bytesRead;
	int connectionAttempts;
	struct sockaddr_in xplaneWriteAddress;
	socklen_t addr_size;
	//int counter=0;


	    /*init values*/
	    connectionAttempts 		  = 0;
	    bytesRead		   		  = 0;
	    returnValue        		  = RETURN_ERROR;
	    buffer[0]		   		  = '\0';
	    //xplaneOutputDecimalValues = NULL;

	    /*Create UDP socket*/
	      udpWriteSocket = socket(PF_INET, SOCK_DGRAM, 0);
	      fprintf(stderr,"Xplane Socket: OK.\n");
	     if(udpWriteSocket>=0)
	     {
	    	 /*Configure settings in address struct*/
	    	 xplaneWriteAddress.sin_family = AF_INET;
	    	 xplaneWriteAddress.sin_port = htons(XPLANE_SEND_PORT);
	    	 xplaneWriteAddress.sin_addr.s_addr = inet_addr(XPLANE_IP_ADDRESS);
	    	 memset(xplaneWriteAddress.sin_zero,'\0', sizeof(xplaneWriteAddress.sin_zero));

	    	 /*Initialize size variable to be used later on*/
	    	 addr_size = sizeof xplaneWriteAddress;
	    	 returnValue=RETURN_OK;
	     }
	     else
	     {
	    	 sleep(2);
	    	 fprintf(stderr,"Xplane Socket: ERROR: %d, %d \r",connectionAttempts,
	    			 returnValue);
	    	 returnValue=RETURN_ERROR;
	     }

	if(returnValue==RETURN_OK)
	{
		rt_task_set_periodic(NULL, TM_NOW, 1000*1000*10);

	  while(1)
	  {


		  if (returnValue == RETURN_OK)
		  {
		  returnValue= rt_queue_bind (&write_to_xplane_queue,
				  	  	  	  	  	  write_to_xplane_queue_Name,
									  TM_NONBLOCK);
		  }
		  if (returnValue == RETURN_OK)
		  {
			  xplaneOutputDecimalValues = malloc(sizeof(*xplaneOutputDecimalValues));

		  			bytesRead=rt_queue_read(&write_to_xplane_queue,
		  									xplaneOutputDecimalValues,
											sizeof(*xplaneOutputDecimalValues),
											TM_NONBLOCK);
		  }



		 /*fprintf(stderr,"Data recieved:%f<--->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%d<---->%d\n",
		  											xplaneOutputDecimalValues->AcX,
		  											xplaneOutputDecimalValues->AcY,
		  											xplaneOutputDecimalValues->AcZ,
		  											xplaneOutputDecimalValues->Pitch,
		  											xplaneOutputDecimalValues->Roll,
		  											xplaneOutputDecimalValues->Yaw,
		  											xplaneOutputDecimalValues->M1,
		  											xplaneOutputDecimalValues->M2,
		  											xplaneOutputDecimalValues->M3,
		  											xplaneOutputDecimalValues->M4,
		  											bytesRead,
		  											sizeof(*xplaneOutputDecimalValues));*/
		  if (bytesRead>0)
		  {
			  /*fprintf(stderr,"D: %f, %f, %f, %f\n",xplaneOutputDecimalValues->M1,
			  					xplaneOutputDecimalValues->M2,
			  					xplaneOutputDecimalValues->M3,
			  					xplaneOutputDecimalValues->M4);*/

		             union UN_Byte_float_transformation data;


		             //BUILD UDP MESSAGE
		             //HEADER
		             strcpy(data.byteValue,UDP_HEADER_INSTRUCTION);
		             buffer[0]=data.byteValue[0];
		             buffer[1]=data.byteValue[1];
		             buffer[2]=data.byteValue[2];
		             buffer[3]=data.byteValue[3];
		             buffer[UDP_HEADER_SIZE]=0;
		             buffer[UDP_INDEX_START]=XPLANE_PITCH_ROLL_INDEX;
		             buffer[UDP_INDEX_START+1]=0;
		             buffer[UDP_INDEX_START+2]=0;
		             buffer[UDP_INDEX_START+3]=0;
		             data.floatValue = xplaneOutputDecimalValues->Pitch;
		             buffer[UDP_DATA_START]=data.byteValue[0];
		             buffer[UDP_DATA_START+1]=data.byteValue[1];
		             buffer[UDP_DATA_START+2]=data.byteValue[2];
		             buffer[UDP_DATA_START+3]=data.byteValue[3];
		             data.floatValue = xplaneOutputDecimalValues->Roll;
		             buffer[UDP_DATA_START+UDP_DATA_SIZE]=data.byteValue[0];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+1]=data.byteValue[1];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+2]=data.byteValue[2];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+3]=data.byteValue[3];
					 data.floatValue = xplaneOutputDecimalValues->Yaw;
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
					 data.floatValue = xplaneOutputDecimalValues->M1;
					 buffer[UDP_SECOND_DATA_START]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+3]=data.byteValue[3];
					 data.floatValue = xplaneOutputDecimalValues->M2;
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+3]=data.byteValue[3];
					 data.floatValue = xplaneOutputDecimalValues->M3;
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE]=data.byteValue[0];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE+1]=data.byteValue[1];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE+2]=data.byteValue[2];
					 buffer[UDP_SECOND_DATA_START+UDP_DATA_SIZE+UDP_DATA_SIZE+3]=data.byteValue[3];
					 data.floatValue = xplaneOutputDecimalValues->M4;
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

					//counter++;
					//fprintf(stderr,"\r%d",counter);


					sendto(udpWriteSocket,buffer,BUFFERSIZE,0,(struct sockaddr *)&xplaneWriteAddress,addr_size);

		 }
		 else
         {
           //fprintf(stderr,"transfer error \r");
			 returnValue=RETURN_OK; //start again
         }
		  tcflush(udpWriteSocket,TCIOFLUSH);
		  fflush(stdout);
		  fflush(stderr);
		  data.byteValue=NULL;
		  memset(&buffer[0], 0, sizeof(buffer));
		  free(xplaneOutputDecimalValues);
	      rt_task_wait_period(NULL);
      }
	}
}
/* (END) XPLANE_WRITE_TASK_FUNC*/




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
 int xplane_comm_init()
{
	  int returnValue;

	  /*init values*/
	  returnValue 	= RETURN_OK;

	 returnValue = rt_task_create(&xplane_read_task,
								   "xplane_read_task",
								   0,
								   10,
								   T_JOINABLE);

	 returnValue = rt_task_create(&xplane_write_task,
								   "xplane_write_task",
								   0,
								   10,
								   T_JOINABLE);


   if (returnValue == RETURN_OK)
   {
	 returnValue = rt_queue_create(&read_from_xplane_queue,
									read_from_xplane_queue_Name,
									BUFFERSIZE,
									Q_UNLIMITED,
									Q_SHARED);
   }

   if (returnValue == RETURN_OK)
   {
	  /*start tasks*/
	  returnValue = rt_task_start(&xplane_write_task,
								 &xplane_write_task_func,
								 NULL);

	  returnValue = rt_task_start(&xplane_read_task,
								  &xplane_read_task_func,
								  NULL);

   }
   else
   {
	  fprintf(stderr,"X-Plane module error\n");
   }

	return (returnValue);
}/* (END) XPLANE_COMM_INIT*/



