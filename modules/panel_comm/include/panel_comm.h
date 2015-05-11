
#ifndef PANEL_COMM_H_
#define PANEL_COMM_H_

#include "generic.h"


#define TCP_PORT 	  	   	 80
#define TCP_DATA_PORT 	  	 5000
#define TCP_COMM_ENABLE		    		"tcp_comm_enable"

#define PANEL_OUTPUT_BUFFER_SIZE			 24

#define TCP_COMM_XHOVER_TRANSFER 		3
#define TCP_COMM_XCLIMB_TRANSFER 		4
#define TCP_COMM_XROLL_LEFT_TRANSFER 	"tcp_comm_xroll_left_transfer"
#define TCP_COMM_XROLL_RIGHT_TRANSFER 	"tcp_comm_xroll_right_transfer"
#define TCP_COMM_XYAW_LEFT_TRANSFER 	8
#define TCP_COMM_XYAW_RIGHT_TRANSFER 	9
#define TCP_COMM_XDESCENT_TRANSFER 		5
#define TCP_COMM_XSTOP_TRANSFER 		0
#define TCP_COMM_XTAKEOFF_TRANSFER 		1
#define TCP_COMM_XLAND_TRANSFER 		2


#define TCP_COMM_RHOVER_TRANSFER 		"tcp_comm_rhover_transfer"
#define TCP_COMM_RCLIMB_TRANSFER 		"tcp_comm_rclimb_transfer"
#define TCP_COMM_RROLL_LEFT_TRANSFER 	"tcp_comm_rroll_left_transfer"
#define TCP_COMM_RROLL_RIGHT_TRANSFER 	"tcp_comm_rroll_right_transfer"
#define TCP_COMM_RYAW_LEFT_TRANSFER 	"tcp_comm_ryaw_left_transfer"
#define TCP_COMM_RYAW_RIGHT_TRANSFER 	"tcp_comm_ryaw_right_transfer"
#define TCP_COMM_RDESCENT_TRANSFER 		"tcp_comm_rdescent_transfer"
#define TCP_COMM_RSTOP_TRANSFER 		"tcp_comm_rstop_transfer"
#define TCP_COMM_RTAKEOFF_TRANSFER 		"tcp_comm_rtakeoff_transfer"
#define TCP_COMM_RLAND_TRANSFER 		"tcp_comm_rland_transfer"





/*local functions*/
RT_TASK  panel_tcp_test_incoming_task;
RT_TASK  takeoff_task;
RT_TASK  land_task;
RT_TASK  hover_task;
RT_TASK  climb_task;
RT_TASK  descend_task;
RT_TASK  yaw_left_task;
RT_TASK  yaw_right_task;

void panel_tcp_test_incoming_task_func(void *arg);
void takeoff_task_func(void *arg);
void land_task_func(void *arg);
void hover_task_func(void *arg);
void climb_task_func(void *arg);
void descend_task_func(void *arg);
void yaw_left_task_func(void *arg);
void yaw_right_task_func(void *arg);

int  panel_comm_init();


#endif /* PANEL_COMM_H_ */
