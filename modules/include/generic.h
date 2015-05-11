#ifndef GENERIC_H_
#define GENERIC_H_



#include <stdlib.h>
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <time.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <linux/joystick.h>
#include <math.h>


#include <linux/kernel.h>

#include "/usr/xenomai/include/xeno_config.h"
#include "/usr/xenomai/include/asm/xenomai/features.h"
#include "/usr/xenomai/include/asm/xenomai/atomic.h"
#include "/usr/xenomai/include/nucleus/system.h"
#include "/usr/xenomai/include/asm/xenomai/system.h"
#include "/usr/xenomai/include/nucleus/types.h"
#include "/usr/xenomai/include/nucleus/thread.h"
#include "/usr/xenomai/include/nucleus/sched.h"
#include "/usr/xenomai/include/native/task.h"
#include "/usr/xenomai/include/native/mutex.h"
#include "/usr/xenomai/include/native/queue.h"


/*Global constants*/
#define RETURN_OK 					  0
#define RETURN_ERROR 				 -1


#define XPLANE_UNSED_VALUE		   	   -999


typedef int bool;
#define TRUE  1
#define FALSE 0


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
}ST_Queue_Packet;




char*   acp220_inputQueueName  			= "apc220_inputQueue";
char*   acp220_outputQueueName  		= "apc220_outputQueue";
char*   write_to_xplane_queue_Name  	= "write_from_xplane_queue";
char*   read_from_xplane_queue_Name  	= "read_from_xplane_queue";
char*   controller_inputQueueName  		= "controller_inputQueue";
char*   read_from_panel_queue_Name  	= "read_from_panel_queue";


RT_QUEUE write_to_xplane_queue;  /*LIFO style queue*/
RT_QUEUE read_from_xplane_queue; /*LIFO style queue*/
RT_QUEUE controller_inputQueue;  /*LIFO style queue*/
RT_QUEUE acp220_inputQueue;		 /*LIFO style queue --> read from apc220*/
RT_QUEUE acp220_outputQueue;     /*LIFO style queue--> write to apc220*/
RT_QUEUE read_from_panel_queue;



#endif /* GENERIC_H_ */
