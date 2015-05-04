
#include "autopilot.h"
#include <math.h>
#include <stdio.h>


int SIMULATOR_ACTIVE = 0;


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
void xplane_activation_task_func(void *arg)
{
	int	returnValue;
	int xplane_shared_memory_fd;
	int *mappedValue;
	bool taskInitiated;
	/*init values*/
	returnValue 				= RETURN_OK;
	xplane_shared_memory_fd     = 0;
	mappedValue					= NULL;
	taskInitiated				= FALSE;

	SIMULATOR_ACTIVE=1;
	rt_task_set_periodic(NULL, TM_NOW, 1000*1000*10);
	  while(1)
	    {
		  xplane_shared_memory_fd = shm_open(XPLANE_SHM, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		  returnValue= ftruncate(xplane_shared_memory_fd, sizeof(int));

		  	if(xplane_shared_memory_fd<0)
		  	{
		  		returnValue=RETURN_ERROR;
		  	}
		  	mappedValue = mmap(NULL, sizeof(int),PROT_READ,
		  					  MAP_SHARED, xplane_shared_memory_fd, 0);

		   if(mappedValue == MAP_FAILED)
			{
				returnValue=RETURN_ERROR;
			}

		   /*TODO: enable when other application access shared memory to activate simulator*/
		   //SIMULATOR_ACTIVE=mappedValue[0];

		   munmap(mappedValue, sizeof(int));

            if(returnValue == RETURN_OK)
            {
            	close( xplane_shared_memory_fd );

            }

            if(SIMULATOR_ACTIVE==1 && taskInitiated==FALSE)
            {
            	 SIMULATOR_ACTIVE=0;
            	 taskInitiated=TRUE;



            }
            else
            {
            	/*TODO: shut down tasks and queues*/
            }
	        rt_task_wait_period(NULL);
	    }


}/*(END) XPLANE_ACTIVATION_TASK_FUNC*/


int main(int argc,char *argv[])
{
	  int returnValue;


	  /*init values*/
	  returnValue 			    = RETURN_OK;


	 mlockall(MCL_CURRENT | MCL_FUTURE);

	 /*Create Tasks*/
	  if (returnValue == RETURN_OK)
	  {
	  returnValue = rt_task_create(&xplane_activation_task,
								  "xplane_activation_task",
								  0,
								  2,
								  T_JOINABLE);
	  }

	  //APC220 Radio comunication Available
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
