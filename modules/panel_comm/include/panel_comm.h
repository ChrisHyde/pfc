
#ifndef PANEL_COMM_H_
#define PANEL_COMM_H_

#include "generic.h"


#define TCP_PORT 	  	   	 80
#define TCP_DATA_PORT 	  	 5000
#define TCP_COMM_ENABLE		 "tcp_comm_enable"
#define TCP_COMM_DATA_RADIO  "r_data"
#define TCP_COMM_XDATA_TRANSFER "tcp_comm_xdata_transfer"

/*local functions*/
RT_TASK  panel_tcp_test_incoming_task;
RT_TASK  send_data_to_panel_task;

void panel_tcp_test_incoming_task_func(void *arg);
void panel_shm_test_incoming_task_func(void *arg);
int  panel_comm_init();


#endif /* PANEL_COMM_H_ */
