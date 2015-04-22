
#ifndef APC220_COMM_H_
#define APC220_COMM_H_

#include "generic.h"

/*local constants*/
#define APC220_PANIC_CONNECTION_LOSS    10
#define APC220_OK_CONNECTION            50
#define MAX_CONNECTION_ATTEMPTS 	     5
#define PORT 						     "/dev/ttyUSB0"
#define ACK_INPUT_MSG_SIZE				 8

#define READ_POSITION_AcX					 1
#define READ_POSITION_AcY					 2
#define READ_POSITION_AcZ					 3
#define READ_POSITION_GyX					 4
#define READ_POSITION_GyY					 5
#define READ_POSITION_GyZ					 6
#define READ_POSITION_M1					 7
#define READ_POSITION_M2					 8
#define READ_POSITION_M3					 9
#define READ_POSITION_M4					 10

speed_t BAUDRATE 	    				= B9600;



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
#define XPLANE_ENGINE_POWER_INDEX  	   25
#define XPLANE_UNSED_VALUE		   	   -999



/*Xenomai Tasks*/
RT_TASK  apc220_read_task;
RT_TASK  apc220_check_read_alive_task;


union UN_Byte_float_transformation
{
	 char  byteValue[4];        // --- first variable -  char or byte array
	 float floatValue;          // ---- second variable - float
	 char  header[4];
}  ;



typedef struct  {
bool    isReadTaskAlive;
}ST_alivetasks;


typedef struct  {
	float     AcX;
	float     AcY;
	float     AcZ;
	float     GyX;
	float     GyY;
	float     GyZ;
	float	  M1;
	float	  M2;
	float	  M3;
	float	  M4;
}ST_apc220_inputVectors;

struct  termios  portSettings;

/*local functions*/
void apc220_check_read_alive_task_func(void *arg);
void apc220_read_task_func(void *arg);
int  open_port(void);
int  apc220_comm_init();



#endif /* APC220_COMM_H_ */
