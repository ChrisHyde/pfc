
#ifndef CONTROLLER_COMM_H_
#define CONTROLLER_COMM_H_

#include "generic.h"

#define CONTROLLER_ADDRESS_PORT 		 "/dev/input/js0"
#define CONTROLLER_AXIS_MAX_VALUE		32767
#define CONTROLLER_AXIS_MIN_VALUE		-32767
#define CONTROLLER_BUFFER_SIZE          24
#define CONTROLLER_MOTOR_BUFFER_SIZE	3
#define HOVER_SPEED						60
#define BUFFERSIZE 				  		90 //Max length of buffer

#define MAX_CONNECTION_ATTEMPTS   		5

/*local functions*/
RT_TASK  controller_read_task;

/*local functions*/
int  xplane_controller_motors(int X,int Y,int Z,int R);
void controller_read_task_func(void *arg);
int  controller_comm_init();


#endif /* CONTROLLER_COMM_H_ */
