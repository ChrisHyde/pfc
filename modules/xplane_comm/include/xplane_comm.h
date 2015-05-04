
#ifndef XPLANE_COMM_H_
#define XPLANE_COMM_H_

#include "generic.h"



#define BUFFERSIZE 				  	90 //Max length of buffer
#define XPLANE_IP_ADDRESS 	      	"192.168.1.22"
#define LOCAL_IP_ADDRESS    	  	"192.168.1.17"
#define XPLANE_SEND_PORT 		  	49000   //The port on which to listen for incoming data
#define XPLANE_RECIEVE_PORT 	  	49001
#define MAX_CONNECTION_ATTEMPTS   	5


/*UDP definitions*/
/**********************************************************************************
 *  01234            5678             9012  3456  7890  1234  5678  9012  3456  7890
 * |XXXXX| 			|XXXX|			 |XXXX  XXXX  XXXX  XXXX  XXXX  XXXX  XXXX  XXXX|
 * header(5 bytes)  Index (4bytes)   Data (32 bytes, 8 groups of 4 bytes)
 *
 * Total 41 bytes per index
 *********************************************************************************/
#define UDP_HEADER_SIZE			  	4   /*last byte not used*/
#define UDP_INDEX_START			  	5
#define UDP_SECOND_INDEX_START		41
#define UDP_INDEX_SIZE			  	4
#define UDP_DATA_START				9
#define UDP_SECOND_DATA_START		45
#define UDP_DATA_SIZE				4
#define UDP_DATA_TOTAL_SIZE			32
#define UDP_DATA_GROUPS				8
#define UDP_HEADER_INSTRUCTION   	"DATA"
#define XPLANE_UNSED_VALUE		   	-999

/*Xplane index definitions*/
#define XPLANE_BASIC_INDEXES_USED      2
#define XPLANE_G_FORCES_INDEX	       4
#define XPLANE_ANGULAR_MOMENTS_INDEX   15
#define XPLANE_ANGULAR_VELOCITY_INDEX  16
#define XPLANE_PITCH_ROLL_INDEX    	   17
#define XPLANE_ENGINE_POWER_INDEX  	   26
#define XPLANE_UNSED_VALUE		   	   -999


typedef struct  {
	float xplaneInputDecimalValues[XPLANE_BASIC_INDEXES_USED][UDP_DATA_GROUPS+1];
}ST_Xplane_inputDecimalVectors;

typedef struct  {
	float     AcX;
	float     AcY;
	float     AcZ;
	float     Pitch;
	float     Roll;
	float     Yaw;
	float	  M1;
	float	  M2;
	float	  M3;
	float	  M4;
}ST_Xplane_outputDecimalVectors;


union UN_Byte_float_transformation
{
	 char  byteValue[4];        // --- first variable -  char or byte array
	 float floatValue;          // ---- second variable - float
	 char  header[4];
}  ;



/*local functions*/
RT_TASK  xplane_read_task;
RT_TASK  xplane_write_task;
RT_TASK  xplane_check_alive_task;

/*local functions*/
void xplane_read_task_func(void *arg);
void xplane_write_task_func(void *arg);
int  xplane_comm_init();


#endif /* XPLANE_COMM_H_ */
