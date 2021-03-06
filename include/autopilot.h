#ifndef AUTOPILOT_H_
#define AUTOPILOT_H_

#include "generic.h"
#include "apc220_comm.h"
#include "xplane_comm.h"
#include "controller_comm.h"
#include "panel_comm.h"



#define AccX	1
#define AccY	2
#define AccZ	3
#define GyrX	4
#define GyrY	5
#define GyrZ	6
#define Motor1	7
#define Motor2	8
#define Motor3	9
#define Motor4	0

#define APC220_OUTPUT_BUFFER_SIZE			 50
#define XPLANE_OUTPUT_BUFFER_SIZE			 24
#define PANEL_OUTPUT_BUFFER_SIZE			 24
#define CONTROLLER_OUTPUT_BUFFER_SIZE		 24

#define XPLANE_BASIC_INDEXES_USED      		 3
#define UDP_DATA_GROUPS						 8


#define XPLANE_SHM "/xplane_shared_memory"


RT_TASK  read_input_queues_task;




typedef struct  {
	float xplaneInputQueueDecimalValues[XPLANE_BASIC_INDEXES_USED][UDP_DATA_GROUPS+1];
}ST_Xplane_inputQueueValues;



void read_input_queues_task_func(void *arg);

#endif /* GENERIC_H_ */
