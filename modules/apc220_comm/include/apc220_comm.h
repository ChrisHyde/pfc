
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


/*Xenomai Tasks*/
RT_TASK  apc220_read_task;
RT_TASK  apc220_check_read_alive_task;


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
