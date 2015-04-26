/*
 ============================================================================
 Name        : xplane_comm_test.c
 Author      : christopher@cthyde.com
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include "xplane_comm.h"


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



   while (connectionAttempts!=5 &&
		  returnValue == RETURN_ERROR)
   {
   connectionAttempts++;
   apcFd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK | O_FSYNC);
     if(apcFd>=0)
     {

    	 bzero(&portSettings, sizeof(portSettings));
    	 portSettings.c_cflag &= ~PARENB;
		 portSettings.c_cflag &= ~CSTOPB;
		 portSettings.c_cflag &= ~CSIZE;
		 portSettings.c_cflag |= CS8 | CLOCAL;
		 //portSettings.c_lflag |= (ICANON);
		 portSettings.c_lflag &= ~ICANON;
		 portSettings.c_oflag &= ~OPOST;
		 portSettings.c_cc[VMIN] = 0;
		 portSettings.c_cc[VTIME] = 5;
    	 tcflush(apcFd, TCIFLUSH);
    	 cfsetospeed(&portSettings, BAUDRATE);
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

	int main(int argc,char *argv[])
	{

		RT_TASK  serial_write;
	    int   returnValue;
	    returnValue = open_port();
	    if(returnValue == RETURN_OK)
	    {
		 returnValue = rt_task_create(&serial_write,
									   "serial_write",
									   0,
									   20,
									   T_JOINABLE);
	    }
	    if(returnValue == RETURN_OK)
	    	    {
		 returnValue = rt_task_start(&serial_write,
		      	      		   	 	 	 	 &serial_write_task_func,
		      	      			 	 	 	 NULL);
	    	    }

	   // returnValue= xplane_comm_init();

	    while(1)
	    {

	    }

	    return(returnValue);

	}
