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
ST_Xplane_outputDecimalVectors *xplaneOutputDecimalValues;


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
void xplane_check_alive_task_func(void *arg)
{



	    rt_task_set_periodic(NULL, TM_NOW, 70000000);

	    while(1)
	    {


	        rt_task_wait_period(NULL);
	    }


}/*(END) XPLANE_CHECK_ALIVE_TASK*/


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
	int returnValue;
	int connectionAttempts;
	struct sockaddr_in xplaneReadAddress;
	socklen_t addr_size;

	    /*init values*/
	    connectionAttempts = 0;
	    returnValue        = RETURN_ERROR;

	    /*Create UDP socket*/
	      udpReadSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	      fprintf(stderr,"Read Socket:%d\n",udpReadSocket);
	     if(udpReadSocket>=0)
	     {

	    	 /*Configure settings in address struct*/
	    	 memset((char *) &xplaneReadAddress, 0, sizeof(xplaneReadAddress));
	    	 xplaneReadAddress.sin_family = AF_INET;
	    	 xplaneReadAddress.sin_port = htons(XPLANE_RECIEVE_PORT);
	    	 xplaneReadAddress.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);

	    	 /*Initialize size variable to be used later on*/
	    	 addr_size = sizeof xplaneReadAddress;
	    	 returnValue=bind(udpReadSocket , (struct sockaddr*)&xplaneReadAddress, sizeof(xplaneReadAddress));


	     }
	     else
	     {
	    	 sleep(2);
	    	 fprintf(stderr," Openning socket Failed: %d, %d \r",connectionAttempts,
	    			 returnValue);
	    	 returnValue=RETURN_ERROR;
	     }

	if(returnValue==RETURN_OK)
	{
		rt_task_set_periodic(NULL, TM_NOW, 7000000);

		  while(1)
		  {   fflush(stdout);
			  memset(buffer,'\0', BUFFERSIZE);
		    int i= recvfrom(udpReadSocket, buffer, BUFFERSIZE, 0, (struct sockaddr *) &xplaneReadAddress, &addr_size);
		    //fprintf(stderr," RECIEVING DATA: %s  -> %d \n",buffer,i);
int x;

		    for(x=0;x<i;x++)
		    {
		    sprintf(buffer, "%d", buffer[x]);
		    fprintf(stderr," %s",buffer);
		    //write in queue
		    /*returnValue = rt_queue_write(&xplane_inputQueue,
										 &inputVectors,
										 sizeof(inputVectors),
										 Q_URGENT);*/
		    }
		    fprintf(stderr,"\n");
		    fprintf(stderr,"\n");


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
	int counter=0;
	    /*init values*/
	    connectionAttempts = 0;
	    bytesRead		   = 0;
	    returnValue        = RETURN_ERROR;

	    /*Create UDP socket*/
	      udpWriteSocket = socket(PF_INET, SOCK_DGRAM, 0);
	      fprintf(stderr,"Xplane:Write Socket:%d\n",udpWriteSocket);
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
	    	 fprintf(stderr," Openning socket Failed: %d, %d \r",connectionAttempts,
	    			 returnValue);
	    	 returnValue=RETURN_ERROR;
	     }

	if(returnValue==RETURN_OK)
	{
		rt_task_set_periodic(NULL, TM_NOW, TM_INFINITE);
	  while(1)
	  {
		  if (returnValue == RETURN_OK)
		  { fprintf(stderr," start bind");
		  returnValue= rt_queue_bind (&acp220_inputQueue,
				  	  	  	          acp220_inputQueueName,
									  TM_INFINITE);fprintf(stderr," end bind");
		  }
		  if (returnValue == RETURN_OK)
		  {
			  xplaneOutputDecimalValues = malloc(sizeof(*xplaneOutputDecimalValues));

		  			bytesRead=rt_queue_read(&acp220_inputQueue,
		  									 xplaneOutputDecimalValues,
											sizeof(*xplaneOutputDecimalValues),
											TM_NONBLOCK);
		  }

		 fprintf(stderr,"Data recieved:%f<--->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%f<---->%d<---->%d\n",
		  											xplaneOutputDecimalValues->AcX,
		  											xplaneOutputDecimalValues->AcY,
		  											xplaneOutputDecimalValues->AcZ,
		  											xplaneOutputDecimalValues->GyX,
		  											xplaneOutputDecimalValues->GyY,
		  											xplaneOutputDecimalValues->GyZ,
		  											xplaneOutputDecimalValues->M1,
		  											xplaneOutputDecimalValues->M2,
		  											xplaneOutputDecimalValues->M3,
		  											xplaneOutputDecimalValues->M4,
		  											bytesRead,
		  											sizeof(*xplaneOutputDecimalValues));
		             union UN_Byte_float_transformation data;


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
		             data.floatValue = xplaneOutputDecimalValues->GyX;
		             buffer[UDP_DATA_START]=data.byteValue[0];
		             buffer[UDP_DATA_START+1]=data.byteValue[1];
		             buffer[UDP_DATA_START+2]=data.byteValue[2];
		             buffer[UDP_DATA_START+3]=data.byteValue[3];
		             data.floatValue = xplaneOutputDecimalValues->GyY;
		             buffer[UDP_DATA_START+UDP_DATA_SIZE]=data.byteValue[0];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+1]=data.byteValue[1];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+2]=data.byteValue[2];
					 buffer[UDP_DATA_START+UDP_DATA_SIZE+3]=data.byteValue[3];
					 data.floatValue = xplaneOutputDecimalValues->GyZ;
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



		             	// int i=0;
		             	 /*for(i=0;i<82;i++)
		             	 {
		             		fprintf(stderr,"%d ",buffer[i]);
		             	 }*/

			      //fprintf(stderr,"buffer %d\n",buffer[2]);
			      //fprintf(stderr,"bytes %d\n",data.byteValue[1]);
			      //fprintf(stderr,"bytes %d\n",data.byteValue[2]);
			      //fprintf(stderr,"bytes %d\n",data.byteValue[3]);


         if (bytesRead<0)
         {

         }
         counter++;
           fprintf(stderr,"\r%d",counter);
		  free(xplaneOutputDecimalValues);


		  //fprintf(stderr,"AccX %d---->bytes: %d\n",xplaneOutputDecimalValues->AcX,bytesRead);


		  sendto(udpWriteSocket,buffer,BUFFERSIZE,0,(struct sockaddr *)&xplaneWriteAddress,addr_size);
		  tcflush(udpWriteSocket,TCIOFLUSH);
		  memset(&buffer[0], 0, sizeof(buffer));
		  returnValue=rt_queue_unbind(&acp220_inputQueue) ;
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
	  returnValue 						  = 0;
	  fprintf(stderr," HOLAAAAAA %d\n", returnValue);

	 returnValue = rt_task_create(&xplane_read_task,
								   "xplane_read_task",
								   0,
								   99,
								   T_JOINABLE);
	 fprintf(stderr," HOLAAAAAA\n");

	 returnValue = rt_task_create(&xplane_write_task,
	 								   "xplane_write_task",
	 								   0,
	 								   99,
	 								   T_JOINABLE);
	 fprintf(stderr," HOLAAAAAA\n");
	 returnValue = rt_task_create(&xplane_check_alive_task,
								   "xplane_check_alive_task",
								   0,
								   99,
								   T_JOINABLE);
	 fprintf(stderr," HOLAAAAAA\n");


	 if (returnValue == RETURN_OK)
	   {
		 returnValue = rt_queue_create(&xplane_inputQueue,
									   xplane_inputQueueName,
									   BUFFERSIZE,
									   Q_UNLIMITED,
									   Q_SHARED);
	   }
	 fprintf(stderr," HOLAAAAAA\n");

     if (returnValue == RETURN_OK)
       {

     	  /*start tasks*/

     	 returnValue = rt_task_start(&xplane_write_task,
     	      		   	 	 	 	 &xplane_write_task_func,
     	      			 	 	 	 NULL);
     	  returnValue = rt_task_start(&xplane_read_task,
     	     		   	 	 	 	 	 	  &xplane_read_task_func,
     	     			 	 	 	 	 	  NULL);
     	 fprintf(stderr," HOLAAAAAA\n");
     	 /* returnValue = rt_task_start(&xplane_check_alive_task,
									  &xplane_check_alive_task_func,
									  NULL);*/
       }
       else
       {

     	  fprintf(stderr,"X-Plane connection error\n");
       }



	return (returnValue);
}/* (END) XPLANE_COMM_INIT*/





