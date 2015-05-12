
#ifndef APC220_COMM_H_
#define APC220_COMM_H_

#include "generic.h"

/*local constants*/
#define APC220_PANIC_CONNECTION_LOSS    10
#define APC220_OK_CONNECTION            50
#define MAX_CONNECTION_ATTEMPTS 	     5
#define PORT 						     "/dev/ttyUSB0"
#define ACK_INPUT_MSG_SIZE				 30



speed_t BAUDRATE 	    				= B9600;


/*Xenomai Tasks*/
RT_TASK  apc220_read_task;
RT_TASK  apc220_write_task;
RT_TASK  apc220_connect_task;


struct  termios  portSettings;

/*local functions*/
void apc220_read_task_func(void *arg);
void apc220_write_task_func(void *arg);
void apc220_connect_task_func(void *arg);
int  apc220_comm_init();



#endif /* APC220_COMM_H_ */
