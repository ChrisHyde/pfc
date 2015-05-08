/*
 ============================================================================
 Name        : panel_comm.c
 Author      : christopher@cthyde.com
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include "panel_comm.h"



/*global variables*/



 /*n =
			  	     if (n < 0) fprintf(stderr,"ERROR writing to socket");*/

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


void send_data_to_panel_task_func(void *arg)
{

    int    		returnValue;
	int    		tcpSocketFD;
	int    		tcpAcceptSocketFD;
	int 		xplaneBytesRead;
	int    		connectionAttempts;
	struct 		sockaddr_in serv_addr;
	struct 		sockaddr_in cli_addr;
	socklen_t 	clilen;
	char   		dataRecievedBuffer[256];
	int 		bytesRead;
	float       xplaneInputfloatValues[8];
	char	    bufferM1[6];
	char	    bufferM2[6];
	char	    bufferM3[6];
	char	    bufferM4[6];
	char	    bufferPitch[6];
	char	    bufferRoll[6];
	char	    bufferYaw[6];
	char	    bufferAlt[6];
	char	    bufferSendToPanel[256];


	fprintf(stderr,"start send data task...\n");
	  /*init values*/
	  returnValue				= RETURN_OK;
	  connectionAttempts 		= 0;
	  xplaneBytesRead			= 0;
	  dataRecievedBuffer[0]     = '\0';
	  bytesRead					= 0;
	  xplaneInputfloatValues[0] = '\0';
	  bufferM1[0] 				= '\0';
	  bufferM2[0]				= '\0';
	  bufferM3[0]				= '\0';
	  bufferM4[0]				= '\0';
	  bufferPitch[0]			= '\0';
	  bufferRoll[0]				= '\0';
      bufferYaw[0]				= '\0';
	  bufferAlt[0]				= '\0';
	  bufferSendToPanel[0]		= '\0';

	  tcpSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	     if (tcpSocketFD >= 0)
	     {
	    	 bzero((char *) &serv_addr, sizeof(serv_addr));
			 serv_addr.sin_family 		= AF_INET;
			 serv_addr.sin_addr.s_addr  = INADDR_ANY;
			 serv_addr.sin_port 		= htons (TCP_DATA_PORT);

			 returnValue = bind(tcpSocketFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
			 fprintf(stderr,"Data transfer To panel Socket: OK.\n");
	     }
	     else
	     {
	    	 sleep(2);
			 fprintf(stderr,"Data transfer To panel: ERROR %d, %d \r",
					 connectionAttempts,returnValue);
			 returnValue = RETURN_ERROR;
	     }

	     rt_task_set_periodic(NULL, TM_NOW, 1000*1000*10);

		  while(1)
		  {

			  bufferM1[0] 				= '\0';
			  bufferM2[0]				= '\0';
			  bufferM3[0]				= '\0';
			  bufferM4[0]				= '\0';
			  bufferPitch[0]			= '\0';
			  bufferRoll[0]				= '\0';
			  bufferYaw[0]				= '\0';
			  bufferAlt[0]				= '\0';
			  bufferSendToPanel[0]		= '\0';

			  if(returnValue == RETURN_OK )
			  {
				  listen(tcpSocketFD,5);
				  clilen = sizeof(cli_addr);

				  tcpAcceptSocketFD = accept(tcpSocketFD,(struct sockaddr *) &cli_addr,&clilen);
				  if (tcpSocketFD >= 0)
				  {
					  bzero(dataRecievedBuffer,256);
				  }

			  }

			      bytesRead = read(tcpAcceptSocketFD,dataRecievedBuffer,255);
				 if (bytesRead >= 0)
				 {
					 fprintf(stderr,"reading bytes %s\n",dataRecievedBuffer);
					 if(strcmp(dataRecievedBuffer,TCP_COMM_DATA_RADIO) )
					 {

					 }
					 if(strcmp(dataRecievedBuffer,TCP_COMM_XDATA_TRANSFER) )
					 {
						 fprintf(stderr,"bind to xplane queue\n");
						 returnValue= rt_queue_bind (&write_to_panel_queue,
													 write_to_panel_queue_Name,
													   TM_NONBLOCK);
						 if(returnValue==RETURN_OK)
						 {
							 fprintf(stderr,"read from xplane queue\n");
							 xplaneBytesRead=rt_queue_read(&write_to_panel_queue,
														  &xplaneInputfloatValues,
														  sizeof(xplaneInputfloatValues),
														  TM_NONBLOCK);
						 }
						 if(xplaneBytesRead>0)
						 {

							 sprintf(bufferPitch, "%f",xplaneInputfloatValues[0] );
							 sprintf(bufferRoll, "%f",xplaneInputfloatValues[1] );
							 sprintf(bufferYaw, "%f",xplaneInputfloatValues[2] );
							 sprintf(bufferAlt, "%f",xplaneInputfloatValues[3] );
							 sprintf(bufferM1, "%f",xplaneInputfloatValues[4] );
							 sprintf(bufferM2, "%f",xplaneInputfloatValues[5] );
							 sprintf(bufferM3, "%f",xplaneInputfloatValues[6] );
							 sprintf(bufferM4, "%f",xplaneInputfloatValues[7] );
							/* fprintf(stderr,"Pitch: %s\n",bufferPitch); fprintf(stderr,"Roll: %s\n",bufferRoll);
							 fprintf(stderr,"Yaw: %s\n",bufferYaw);fprintf(stderr,"Alt: %s\n",bufferAlt);
							 fprintf(stderr,"M1: %s\n",bufferM1);fprintf(stderr,"M2: %s\n",bufferM2);
							 fprintf(stderr,"M3: %s\n",bufferM3);fprintf(stderr,"M4: %s\n",bufferM4);*/
							 strcpy(bufferSendToPanel,bufferPitch);
							 strcat(bufferSendToPanel,bufferRoll);
							 strcat(bufferSendToPanel,bufferYaw);
							 strcat(bufferSendToPanel,bufferAlt);
							 strcat(bufferSendToPanel,bufferM1);
							 strcat(bufferSendToPanel,bufferM2);
							 strcat(bufferSendToPanel,bufferM3);
							 strcat(bufferSendToPanel,bufferM4);


						 w//rite(tcpAcceptSocketFD,bufferSendToPanel,sizeof(bufferSendToPanel));

						 }

					 }
					 else
					 {
						 fprintf(stderr,"de que cono vas!...");
					 }
				 }


			  rt_task_wait_period(NULL);
		  }


		     close(tcpAcceptSocketFD);
		     close(tcpSocketFD);



}/*(END) SEND_DATA_TO_PANEL_TASK_FUNC*/

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


void panel_tcp_test_incoming_task_func(void *arg)
{

    int    		returnValue;
	int    		tcpSocketFD;
	int    		tcpAcceptSocketFD;
	int    		connectionAttempts;
	struct 		sockaddr_in serv_addr;
	struct 		sockaddr_in cli_addr;
	socklen_t 	clilen;
	char   		dataRecievedBuffer[256];
	int 		bytesRead;
	bool 		isSendDataToPanelStarted;



	  /*init values*/
	  returnValue				= RETURN_OK;
	  connectionAttempts 		= 0;
	  dataRecievedBuffer[0]     = '\0';
	  bytesRead					= 0;
	  isSendDataToPanelStarted	= FALSE;

	  tcpSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	     if (tcpSocketFD >= 0)
	     {
	    	 bzero((char *) &serv_addr, sizeof(serv_addr));
			 serv_addr.sin_family 		= AF_INET;
			 serv_addr.sin_addr.s_addr  = INADDR_ANY;
			 serv_addr.sin_port 		= htons (TCP_PORT);

			 returnValue = bind(tcpSocketFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
			 fprintf(stderr,"Panel Socket: OK.\n");

	     }
	     else
	     {
	    	 sleep(2);
			 fprintf(stderr," Openning TCPsocket Failed: %d, %d \r",
					 connectionAttempts,returnValue);
			 returnValue = RETURN_ERROR;
	     }

	     rt_task_set_periodic(NULL, TM_NOW, 5000*1000*100);
		  while(1)
		  {
			  if(returnValue == RETURN_OK )
			  {
				  listen(tcpSocketFD,5);
				  clilen = sizeof(cli_addr);

				  tcpAcceptSocketFD = accept(tcpSocketFD,(struct sockaddr *) &cli_addr,&clilen);
				  if (tcpSocketFD >= 0)
				  {
					  bzero(dataRecievedBuffer,256);
				  }

			  }

			      bytesRead = read(tcpAcceptSocketFD,dataRecievedBuffer,255);
				 if (bytesRead >= 0)
				 {
					 if(strcmp(dataRecievedBuffer,TCP_COMM_ENABLE) && !isSendDataToPanelStarted)
					 {
						 close(tcpAcceptSocketFD);
						 close(tcpSocketFD);
						 isSendDataToPanelStarted = TRUE;
						 returnValue = rt_task_start(&send_data_to_panel_task,
						 	   					     &send_data_to_panel_task_func,
						 	   						  NULL);

					 }
				 }


			  rt_task_wait_period(NULL);
		  }






}/*(END) PANEL_TCP_TEST_INCOMING_TASK_FUNC*/

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
 int panel_comm_init()
{
	  int returnValue;

	  /*init values*/
	  returnValue 	= RETURN_OK;


	  returnValue = rt_task_create(&panel_tcp_test_incoming_task,
	 							   "panel_tcp_test_incoming_task",
	 							    0,
	 							   2,
	 							   T_JOINABLE);
	  returnValue = rt_task_create(&send_data_to_panel_task,
	 	 							   "send_data_to_panel_task",
	 	 							    0,
	 	 							   3,
	 	 							   T_JOINABLE);

   if (returnValue == RETURN_OK)
   {
	  /*start tasks*/
	   returnValue = rt_task_start(&panel_tcp_test_incoming_task,
	   							   &panel_tcp_test_incoming_task_func,
	   							   NULL);


   }
   else
   {
	  fprintf(stderr,"Panel module error\n");
   }

	return (returnValue);
}/* (END) XPLANE_COMM_INIT*/



