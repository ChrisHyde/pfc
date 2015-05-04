
#include "autopilot.h"
#include <math.h>
#include <stdio.h>



/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : Starts the readXplane tasks and the output queue if SIMULATOR_ACTIVE = true
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/
void read_input_queues_task_func(void *arg)
{
	int	returnValue;

	/*init values*/
	returnValue 				= RETURN_OK;

	rt_task_set_periodic(NULL, TM_NOW, 1000*1000*10);

	  while(1)
	    {




	        rt_task_wait_period(NULL);
	    }


}/*(END) READ_INPUT__QUEUES_TASK_FUNC*/


int main(int argc,char *argv[])
{
	  int returnValue;


	  /*init values*/
	  returnValue 			    = RETURN_OK;


	 mlockall(MCL_CURRENT | MCL_FUTURE);

	 /*Create Tasks*////////////////////////////////////////////////
	  if (returnValue == RETURN_OK)
	  {
	  returnValue = rt_task_create(&read_input_queues_task,
								  "read_input_queues_task",
								  0,
								  4,
								  T_JOINABLE);
	  }

	  /*Create Output Queues*//////////////////////////////////////
	  if (returnValue == RETURN_OK)
	  {
	  returnValue = rt_queue_create(&acp220_outputQueue,
			  	  	  	  	  	    acp220_outputQueueName,
			  	  	  	  	        APC220_OUTPUT_BUFFER_SIZE,
								    Q_UNLIMITED,
								    Q_SHARED);
	  }
	  if (returnValue == RETURN_OK)
	  {
	  returnValue = rt_queue_create(&acp220_outputQueue,
								    acp220_outputQueueName,
								    APC220_OUTPUT_BUFFER_SIZE,
								    Q_UNLIMITED,
								    Q_SHARED);
	  }



	  /*Initialize Modules */////////////////////////////////////////
	  if (returnValue == RETURN_OK)
	  {
	    returnValue = apc220_comm_init();
	  }

	  if (returnValue == RETURN_OK)
	  {
		  returnValue = controller_comm_init();
	  }
	  if (returnValue == RETURN_OK)
	  {
	     returnValue = xplane_comm_init();
	  }

	  else
	  {
		  	  //start task apcqueue to Qt queue conversion and to xplane queue conversion

	  }





	while(1)
	  {
        /*do nothing*/
	  }

		fprintf(stderr,"retunrValue %d\n",returnValue);

	return (returnValue);
}
