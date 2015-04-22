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

typedef int bool;
#define true  1
#define false 0

char*   acp220_inputQueueName  = "apc220_inputQueue";
char*   xplane_inputQueueName  = "xplane_inputQueue";


RT_QUEUE acp220_inputQueue; /*LIFO style queue*/
RT_QUEUE xplane_inputQueue; /*LIFO style queue*/




#endif /* GENERIC_H_ */
