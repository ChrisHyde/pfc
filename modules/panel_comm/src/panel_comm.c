/*
 ============================================================================
 Name        : panel_comm.c
 Author      : christopher@cthyde.com
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include "panel_comm.h"



/*global variables*/

/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/


void yaw_left_task_func(void *arg)
{
	float 	xplaneInputfloatValues[8];
	float 	panelOutputfloatValues[4];
    int 	xplaneBytesRead;
    int 	returnValue;


    /*init values*/
    xplaneBytesRead = 0;
    returnValue		= RETURN_OK;
    fprintf(stderr,"holaa2\n");
    returnValue= rt_queue_bind (&read_from_xplane_queue,
    			  	  	  	  	  	  read_from_xplane_queue_Name,
    								  TM_NONBLOCK);
    if (returnValue == RETURN_OK)
    {
    	returnValue=rt_queue_flush(&read_from_xplane_queue);
    	returnValue=rt_queue_flush(&read_from_panel_queue);
    }



	   rt_task_set_periodic(NULL, TM_NOW, 2000*1000*100);
  while(1)
  {



	 if (returnValue==RETURN_OK)
	 {
		 xplaneBytesRead = rt_queue_read(&read_from_xplane_queue,
											 &xplaneInputfloatValues,
											 sizeof(xplaneInputfloatValues),
											 TM_NONBLOCK);
		 if (xplaneBytesRead<=0)
		 {
			 fprintf(stderr,"YAW_LEFT_TASK_FUNC_ERROR 1\n");
		 }
		 else
		 {
  /*fprintf(stderr,"%f,%f,%f,%f,%f,%f,%f,%f\n\n",xplaneInputfloatValues[0],
										  xplaneInputfloatValues[1],
										  xplaneInputfloatValues[2],
										  xplaneInputfloatValues[3],
										  xplaneInputfloatValues[4],
										  xplaneInputfloatValues[5],
										  xplaneInputfloatValues[6],
										  xplaneInputfloatValues[7]);*/
             if(xplaneInputfloatValues[1]<1  )
             {

            	    panelOutputfloatValues[0]=9.2 ;
					panelOutputfloatValues[1]=7.8 ;
					panelOutputfloatValues[2]=9.2 ;
					panelOutputfloatValues[3]=7.8 ;


						if(xplaneInputfloatValues[0]>=1)//IS pitch UP
						{
							panelOutputfloatValues[0]=7.5 ;
							panelOutputfloatValues[1]=7.5 ;
							panelOutputfloatValues[2]=9.5 ;
							panelOutputfloatValues[3]=9.5 ;
						}
						else if(xplaneInputfloatValues[0]<=-1)//IS pitch DOWN
						{
							panelOutputfloatValues[0]=9.5 ;
							panelOutputfloatValues[1]=9.5 ;
							panelOutputfloatValues[2]=7.5 ;
							panelOutputfloatValues[3]=7.5 ;
						}
             }


		 }
     }
	 else
	 {
		 fprintf(stderr,"YAW_LEFT_TASK_FUNC_ERROR 2\n");
	 }

	 returnValue = rt_queue_write(&read_from_panel_queue,
								 &panelOutputfloatValues,
								 sizeof(panelOutputfloatValues),
								 Q_URGENT);

	 rt_task_wait_period(NULL);
  }
}/*END_YAW_LEFT_TASK_FUNC*/


/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/


void yaw_right_task_func(void *arg)
{
	float 	xplaneInputfloatValues[8];
	float 	panelOutputfloatValues[4];
    int 	xplaneBytesRead;
    int 	returnValue;


    /*init values*/
    xplaneBytesRead = 0;
    returnValue		= RETURN_OK;
    returnValue= rt_queue_bind (&read_from_xplane_queue,
    			  	  	  	  	  	  read_from_xplane_queue_Name,
    								  TM_NONBLOCK);
    if (returnValue == RETURN_OK)
    {
    	returnValue=rt_queue_flush(&read_from_xplane_queue);
    	returnValue=rt_queue_flush(&read_from_panel_queue);
    }



	   rt_task_set_periodic(NULL, TM_NOW, 2000*1000*100);
  while(1)
  {



	 if (returnValue==RETURN_OK)
	 {
		 xplaneBytesRead = rt_queue_read(&read_from_xplane_queue,
											 &xplaneInputfloatValues,
											 sizeof(xplaneInputfloatValues),
											 TM_NONBLOCK);
		 if (xplaneBytesRead<=0)
		 {
			 fprintf(stderr,"YAW_RIGHT_TASK_FUNC_ERROR 1\n");
		 }
		 else
		 {
  /*fprintf(stderr,"%f,%f,%f,%f,%f,%f,%f,%f\n\n",xplaneInputfloatValues[0],
										  xplaneInputfloatValues[1],
										  xplaneInputfloatValues[2],
										  xplaneInputfloatValues[3],
										  xplaneInputfloatValues[4],
										  xplaneInputfloatValues[5],
										  xplaneInputfloatValues[6],
										  xplaneInputfloatValues[7]);*/
             if(xplaneInputfloatValues[1]<1  )
             {

            	    panelOutputfloatValues[0]=7.8 ;
					panelOutputfloatValues[1]=9.2 ;
					panelOutputfloatValues[2]=7.8 ;
					panelOutputfloatValues[3]=9.2 ;


						if(xplaneInputfloatValues[0]>=1)//IS pitch UP
						{
							panelOutputfloatValues[0]=7.5 ;
							panelOutputfloatValues[1]=7.5 ;
							panelOutputfloatValues[2]=9.5 ;
							panelOutputfloatValues[3]=9.5 ;
						}
						else if(xplaneInputfloatValues[0]<=-1)//IS pitch DOWN
						{
							panelOutputfloatValues[0]=9.5 ;
							panelOutputfloatValues[1]=9.5 ;
							panelOutputfloatValues[2]=7.5 ;
							panelOutputfloatValues[3]=7.5 ;
						}
             }


		 }
     }
	 else
	 {
		 fprintf(stderr,"YAW_RIGHT_TASK_FUNC_ERROR 2\n");
	 }

	 returnValue = rt_queue_write(&read_from_panel_queue,
								 &panelOutputfloatValues,
								 sizeof(panelOutputfloatValues),
								 Q_URGENT);

	 rt_task_wait_period(NULL);
  }
}/*END_YAW_RIGHT_TASK_FUNC*/




/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/


void descend_task_func(void *arg)
{
	float 	xplaneInputfloatValues[8];
	float 	panelOutputfloatValues[4];
    int 	xplaneBytesRead;
    int 	returnValue;


    /*init values*/
    xplaneBytesRead = 0;
    returnValue		= RETURN_OK;

    returnValue= rt_queue_bind (&read_from_xplane_queue,
    			  	  	  	  	  	  read_from_xplane_queue_Name,
    								  TM_NONBLOCK);
    if (returnValue == RETURN_OK)
    {
    	returnValue=rt_queue_flush(&read_from_xplane_queue);
    	returnValue=rt_queue_flush(&read_from_panel_queue);
    }



	   rt_task_set_periodic(NULL, TM_NOW, 2000*1000*100);
  while(1)
  {




	 if (returnValue==RETURN_OK)
	 {
		 xplaneBytesRead = rt_queue_read(&read_from_xplane_queue,
											 &xplaneInputfloatValues,
											 sizeof(xplaneInputfloatValues),
											 TM_NONBLOCK);
		 if (xplaneBytesRead<=0)
		 {
			 fprintf(stderr,"DESCEND_TASK_FUNC_ERROR 1\n");
		 }
		 else
		 {
  /*fprintf(stderr,"%f,%f,%f,%f,%f,%f,%f,%f\n\n",xplaneInputfloatValues[0],
										  xplaneInputfloatValues[1],
										  xplaneInputfloatValues[2],
										  xplaneInputfloatValues[3],
										  xplaneInputfloatValues[4],
										  xplaneInputfloatValues[5],
										  xplaneInputfloatValues[6],
										  xplaneInputfloatValues[7]);*/
             if(xplaneInputfloatValues[3]>15 )
             {

            	    panelOutputfloatValues[0]=6.5 ;
					panelOutputfloatValues[1]=6.5 ;
					panelOutputfloatValues[2]=6.5 ;
					panelOutputfloatValues[3]=6.5 ;
						if(xplaneInputfloatValues[0]>=1)//IS pitch UP
						{
							panelOutputfloatValues[0]=5.5 ;
							panelOutputfloatValues[1]=5.5 ;
							panelOutputfloatValues[2]=6.5 ;
							panelOutputfloatValues[3]=6.5 ;
						}
						else if(xplaneInputfloatValues[0]<=-1)//IS pitch DOWN
						{
							panelOutputfloatValues[0]=6.5 ;
							panelOutputfloatValues[1]=6.5 ;
							panelOutputfloatValues[2]=5.5 ;
							panelOutputfloatValues[3]=5.5 ;
						}
						if(xplaneInputfloatValues[1]>=1)//IS Roll Right
						{
							panelOutputfloatValues[0]=5.5 ;
							panelOutputfloatValues[1]=6.5 ;
							panelOutputfloatValues[2]=5.5 ;
							panelOutputfloatValues[3]=6.5 ;
						}
						else if(xplaneInputfloatValues[1]<=-1)//ISRoll Left
						{
							panelOutputfloatValues[0]=5.5 ;
							panelOutputfloatValues[1]=6.5 ;
							panelOutputfloatValues[2]=5.5 ;
							panelOutputfloatValues[3]=6.5 ;
						}
             }
              if(xplaneInputfloatValues[3]<15 )
             {


            	  panelOutputfloatValues[0]=7 ;
				  panelOutputfloatValues[1]=7 ;
				  panelOutputfloatValues[2]=7 ;
				  panelOutputfloatValues[3]=7 ;

			  }
		 }
     }
	 else
	 {
		 fprintf(stderr,"DESCEND_TASK_FUNC_ERROR 2\n");
	 }

	 returnValue = rt_queue_write(&read_from_panel_queue,
								 &panelOutputfloatValues,
								 sizeof(panelOutputfloatValues),
								 Q_URGENT);

	 rt_task_wait_period(NULL);
  }
}/*END_DESNCEND_TASK_FUNC*/




/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/
void climb_task_func(void *arg)
{
	float xplaneInputfloatValues[8];
	float panelOutputfloatValues[4];
    int xplaneBytesRead;
    int returnValue;

    /*init values*/
    xplaneBytesRead = 0;
    returnValue=RETURN_OK;


    returnValue= rt_queue_bind (&read_from_xplane_queue,
    			  	  	  	  	  	  read_from_xplane_queue_Name,
    								  TM_NONBLOCK);
    if (returnValue == RETURN_OK)
    {
    	returnValue=rt_queue_flush(&read_from_xplane_queue);
    	returnValue=rt_queue_flush(&read_from_panel_queue);
    }


	  returnValue= rt_queue_bind (&read_from_xplane_queue,
			  	  	  	  	  	  read_from_xplane_queue_Name,
								  TM_NONBLOCK);



	   rt_task_set_periodic(NULL, TM_NOW, 2000*1000*100);

  while(1)
  {



	 if (returnValue==RETURN_OK)
	 {
		 xplaneBytesRead = rt_queue_read(&read_from_xplane_queue,
											 &xplaneInputfloatValues,
											 sizeof(xplaneInputfloatValues),
											 TM_NONBLOCK);
		 if (xplaneBytesRead<=0)
		 {
			 fprintf(stderr,"CLIMB_TASK_FUNC_ERROR 1\n");
		 }
		 else
		 {
  /*fprintf(stderr,"%f,%f,%f,%f,%f,%f,%f,%f\n\n",xplaneInputfloatValues[0],
										  xplaneInputfloatValues[1],
										  xplaneInputfloatValues[2],
										  xplaneInputfloatValues[3],
										  xplaneInputfloatValues[4],
										  xplaneInputfloatValues[5],
										  xplaneInputfloatValues[6],
										  xplaneInputfloatValues[7]);*/
             if(xplaneInputfloatValues[3]>3 )
             {
            	    panelOutputfloatValues[0]=12 ;
					panelOutputfloatValues[1]=12 ;
					panelOutputfloatValues[2]=12 ;
					panelOutputfloatValues[3]=12 ;
					if(xplaneInputfloatValues[0]>=2)//IS pitch UP
								{
									panelOutputfloatValues[0]=11 ;
									panelOutputfloatValues[1]=11 ;
									panelOutputfloatValues[2]=12 ;
									panelOutputfloatValues[3]=12 ;
								}
								else if(xplaneInputfloatValues[0]<=-2)//IS pitch DOWN
								{
									panelOutputfloatValues[0]=12 ;
									panelOutputfloatValues[1]=12 ;
									panelOutputfloatValues[2]=11 ;
									panelOutputfloatValues[3]=11 ;
								}
             }
		 }
     }
	 else
	 {
		 fprintf(stderr,"CLIMB_TASK_FUNC_ERROR 2\n");
	 }
	 returnValue=rt_queue_flush(&read_from_xplane_queue);

	 returnValue = rt_queue_write(&read_from_panel_queue,
								 &panelOutputfloatValues,
								 sizeof(panelOutputfloatValues),
								 Q_URGENT);
	 rt_task_wait_period(NULL);
  }
}/*END_CLIMB_TASK_FUNC*/



/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/


void hover_task_func(void *arg)
{
	float xplaneInputfloatValues[8];
	float panelOutputfloatValues[4];
    int xplaneBytesRead;
    float altitude;
    int returnValue;


    /*init values*/
    xplaneBytesRead = 0;
    altitude = 0.0;
    returnValue=RETURN_OK;


    returnValue= rt_queue_bind (&read_from_xplane_queue,
    			  	  	  	  	  	  read_from_xplane_queue_Name,
    								  TM_NONBLOCK);


    xplaneBytesRead = rt_queue_read(&read_from_xplane_queue,
								 &xplaneInputfloatValues,
								 sizeof(xplaneInputfloatValues),
								 TM_NONBLOCK);
    if(xplaneBytesRead>=0)
    {
    altitude = xplaneInputfloatValues[3];
    }
    else
    {
    	returnValue = RETURN_ERROR;
    }
    if (returnValue == RETURN_OK)
       {

    	returnValue=rt_queue_flush(&read_from_panel_queue);
       }




fprintf(stderr,"ATILTUDE: %f\n",altitude);

      returnValue= rt_queue_bind (&read_from_xplane_queue,
			  	  	  	  	  	  read_from_xplane_queue_Name,
								  TM_NONBLOCK);
	  fprintf(stderr,"Bind returnValue %d\n",returnValue);

	   rt_task_set_periodic(NULL, TM_NOW, 2000*1000*100);
  while(1)
  {




	 if (returnValue==RETURN_OK)
	 {
		 xplaneBytesRead = rt_queue_read(&read_from_xplane_queue,
											 &xplaneInputfloatValues,
											 sizeof(xplaneInputfloatValues),
											 TM_NONBLOCK);

		 if (xplaneBytesRead<=0)
		 {
            fprintf(stderr,"HOVER_TASK_FUNC_ERROR 1 \n");
		 }
		 else
		 {

 /* fprintf(stderr,"%f,%f,%f,%f,%f,%f,%f,%f\n\n",xplaneInputfloatValues[0],
										  xplaneInputfloatValues[1],
										  xplaneInputfloatValues[2],
										  xplaneInputfloatValues[3],
										  xplaneInputfloatValues[4],
										  xplaneInputfloatValues[5],
										  xplaneInputfloatValues[6],
										  xplaneInputfloatValues[7]);*/
             if(xplaneInputfloatValues[3]< altitude )
             {//15 feet takeoff level

            	    panelOutputfloatValues[0]=9 ;
					panelOutputfloatValues[1]=9 ;
					panelOutputfloatValues[2]=9 ;
					panelOutputfloatValues[3]=9 ;
					if(xplaneInputfloatValues[0]>=2)//IS pitch UP
						{
							panelOutputfloatValues[0]=7.5 ;
							panelOutputfloatValues[1]=7.5 ;
							panelOutputfloatValues[2]=8.5 ;
							panelOutputfloatValues[3]=8.5 ;
						}
						else if(xplaneInputfloatValues[0]<=-2)//IS pitch DOWN
						{
							panelOutputfloatValues[0]=8.5 ;
							panelOutputfloatValues[1]=8.5 ;
							panelOutputfloatValues[2]=7.5 ;
							panelOutputfloatValues[3]=7.5 ;
						}
             }
             if(xplaneInputfloatValues[3]> altitude)
			  {//15 feet takeoff level

					panelOutputfloatValues[0]=7 ;
					panelOutputfloatValues[1]=7 ;
					panelOutputfloatValues[2]=7 ;
					panelOutputfloatValues[3]=7 ;
					if(xplaneInputfloatValues[0]>=2)//IS pitch UP
							{
								panelOutputfloatValues[0]=7 ;
								panelOutputfloatValues[1]=7 ;
								panelOutputfloatValues[2]=8 ;
								panelOutputfloatValues[3]=8 ;
							}
							else if(xplaneInputfloatValues[0]<=-2)//IS pitch DOWN
							{
								panelOutputfloatValues[0]=8 ;
								panelOutputfloatValues[1]=8 ;
								panelOutputfloatValues[2]=7 ;
								panelOutputfloatValues[3]=7 ;
							}
			  }
		 }


     }
	 returnValue = rt_queue_write(&read_from_panel_queue,
								 &panelOutputfloatValues,
								 sizeof(panelOutputfloatValues),
								 Q_URGENT);

	 if(returnValue == RETURN_ERROR)
	 {
		 fprintf(stderr,"HOVER_TASK_FUNC_ERROR 2\n");
	 }
	 rt_task_wait_period(NULL);
  }
}/*END_HOVER_TASK_FUNC*/


/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/


void land_task_func(void *arg)
{
	float xplaneInputfloatValues[8];
	float panelOutputfloatValues[4];
    int xplaneBytesRead;
    int returnValue;


    /*init values*/
    xplaneBytesRead = 0;
    returnValue=RETURN_OK;
    returnValue= rt_queue_bind (&read_from_xplane_queue,
     			  	  	  	  	  	  read_from_xplane_queue_Name,
     								  TM_NONBLOCK);
     if (returnValue == RETURN_OK)
     {
    	 returnValue=rt_queue_flush(&read_from_xplane_queue);
    	 returnValue=rt_queue_flush(&read_from_panel_queue);
     }

     returnValue= rt_queue_bind (&read_from_xplane_queue,
   			  	  	  	  	  	  read_from_xplane_queue_Name,
   								  TM_NONBLOCK);
   	  fprintf(stderr,"LAND_TASK_FUNC_ERROR 0: %d\n",returnValue);

	   rt_task_set_periodic(NULL, TM_NOW, 2000*1000*100);
  while(1)
  {



	 if (returnValue==RETURN_OK)
	 {
		 xplaneBytesRead = rt_queue_read(&read_from_xplane_queue,
											 &xplaneInputfloatValues,
											 sizeof(xplaneInputfloatValues),
											 TM_NONBLOCK);
		 if (xplaneBytesRead<=0)
		 {
			 fprintf(stderr,"LAND_TASK_FUNC_ERROR 1\n");
		 }
		 else
		 {
             if(xplaneInputfloatValues[3]>=15 )
             {//15 feet takeoff level

            	    panelOutputfloatValues[0]=6 ;
					panelOutputfloatValues[1]=6 ;
					panelOutputfloatValues[2]=6 ;
					panelOutputfloatValues[3]=6 ;

					if(xplaneInputfloatValues[0]>=2)//IS pitch UP
					{
						panelOutputfloatValues[0]=5 ;
						panelOutputfloatValues[1]=5 ;
						panelOutputfloatValues[2]=6 ;
						panelOutputfloatValues[3]=6 ;
					}
					else if(xplaneInputfloatValues[0]<=-2)//IS pitch DOWN
					{
						panelOutputfloatValues[0]=6 ;
						panelOutputfloatValues[1]=6 ;
						panelOutputfloatValues[2]=5 ;
						panelOutputfloatValues[3]=5 ;
					}

             }
             if(xplaneInputfloatValues[3]< 15 && xplaneInputfloatValues[3]>5 )
			  {//15 feet takeoff level

					panelOutputfloatValues[0]=7 ;
					panelOutputfloatValues[1]=7 ;
					panelOutputfloatValues[2]=7 ;
					panelOutputfloatValues[3]=7 ;
			  }
             if(xplaneInputfloatValues[3]<=5 && xplaneInputfloatValues[3]>=3)
			  {//15 feet takeoff level

					panelOutputfloatValues[0]=7.5 ;
					panelOutputfloatValues[1]=7.5 ;
					panelOutputfloatValues[2]=7.5 ;
					panelOutputfloatValues[3]=7.5 ;
			  }

             if(xplaneInputfloatValues[3]<=3 )
			  {//15 feet takeoff level

					panelOutputfloatValues[0]=6.5 ;
					panelOutputfloatValues[1]=6.5 ;
					panelOutputfloatValues[2]=6.5 ;
					panelOutputfloatValues[3]=6.5 ;
			  }
             if(xplaneInputfloatValues[3]<=1 )
			  {//15 feet takeoff level

					panelOutputfloatValues[0]=0 ;
					panelOutputfloatValues[1]=0 ;
					panelOutputfloatValues[2]=0 ;
					panelOutputfloatValues[3]=0 ;
			  }


		 }


     }
	 else
	 {
		 fprintf(stderr,"LAND_TASK_FUNC_ERROR 2\n");
	 }
	 returnValue = rt_queue_write(&read_from_panel_queue,
								 &panelOutputfloatValues,
								 sizeof(panelOutputfloatValues),
								 Q_URGENT);
	 rt_task_wait_period(NULL);
  }
} /*END_LAND_TASK_FUNC*/


/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/


void takeoff_task_func(void *arg)
{
	float xplaneInputfloatValues[8];
	float panelOutputfloatValues[4];
    int xplaneBytesRead;
    int returnValue;


    /*init values*/
    xplaneBytesRead = 0;
    returnValue=RETURN_OK;
    returnValue= rt_queue_bind (&read_from_xplane_queue,
     			  	  	  	  	  	  read_from_xplane_queue_Name,
     								  TM_NONBLOCK);
     if (returnValue == RETURN_OK)
     {
    	 returnValue=rt_queue_flush(&read_from_xplane_queue);
    	 returnValue=rt_queue_flush(&read_from_panel_queue);
     }

     returnValue= rt_queue_bind (&read_from_xplane_queue,
   			  	  	  	  	  	  read_from_xplane_queue_Name,
   								  TM_NONBLOCK);


	   rt_task_set_periodic(NULL, TM_NOW, 2000*1000*100);
  while(1)
  {


	  returnValue= rt_queue_bind (&read_from_xplane_queue,
			  	  	  	  	  	  read_from_xplane_queue_Name,
								  TM_NONBLOCK);

	 if (returnValue==RETURN_OK)
	 {
		 xplaneBytesRead = rt_queue_read(&read_from_xplane_queue,
											 &xplaneInputfloatValues,
											 sizeof(xplaneInputfloatValues),
											 TM_NONBLOCK);
		 if (xplaneBytesRead<=0)
		 {
			 fprintf(stderr,"TAKEOFF_TASK_FUNC_ERROR 1\n");
		 }
		 else
		 {
 /* fprintf(stderr,"%f,%f,%f,%f,%f,%f,%f,%f\n\n",xplaneInputfloatValues[0],
										  xplaneInputfloatValues[1],
										  xplaneInputfloatValues[2],
										  xplaneInputfloatValues[3],
										  xplaneInputfloatValues[4],
										  xplaneInputfloatValues[5],
										  xplaneInputfloatValues[6],
										  xplaneInputfloatValues[7]);*/
             if(xplaneInputfloatValues[3]<=1 )
             {

            	    panelOutputfloatValues[0]=10 ;
					panelOutputfloatValues[1]=10 ;
					panelOutputfloatValues[2]=10 ;
					panelOutputfloatValues[3]=10 ;
             }
             if(xplaneInputfloatValues[3]< 15 && xplaneInputfloatValues[3]>2 )
			  {//15 feet takeoff level

					panelOutputfloatValues[0]=9 ;
					panelOutputfloatValues[1]=9 ;
					panelOutputfloatValues[2]=9 ;
					panelOutputfloatValues[3]=9 ;
					if(xplaneInputfloatValues[0]>=3)//IS pitch UP
							{
								panelOutputfloatValues[0]=8 ;
								panelOutputfloatValues[1]=8 ;
								panelOutputfloatValues[2]=9 ;
								panelOutputfloatValues[3]=9 ;
							}
							else if(xplaneInputfloatValues[0]<=-3)//IS pitch DOWN
							{
								panelOutputfloatValues[0]=9 ;
								panelOutputfloatValues[1]=9 ;
								panelOutputfloatValues[2]=8 ;
								panelOutputfloatValues[3]=8 ;
							}
			  }
             if(xplaneInputfloatValues[3]>= 15 )
			  {

					panelOutputfloatValues[0]=8 ;
					panelOutputfloatValues[1]=8 ;
					panelOutputfloatValues[2]=8 ;
					panelOutputfloatValues[3]=8 ;
			  }
		 }
     }
	 else
	 {
		 fprintf(stderr,"TAKEOFF_TASK_FUNC_ERROR 2\n");
	 }
	 returnValue = rt_queue_write(&read_from_panel_queue,
								 &panelOutputfloatValues,
								 sizeof(panelOutputfloatValues),
								 Q_URGENT);
	 rt_task_wait_period(NULL);
  }
}/*END_TAKEOFF_TASK_FUNC*/


/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/


void panel_tcp_test_incoming_task_func(void *arg)
{

    int    		returnValue;
	int    		tcpSocketFD;
	int    		tcpAcceptSocketFD;
	int    		connectionAttempts;
	struct 		sockaddr_in serv_addr;
	struct 		sockaddr_in cli_addr;
	socklen_t 	clilen;
	char   		dataRecievedBuffer[256];
	int 		bytesRead;
	int 	    taskExist;
	int			tcpInput;
	float 		panelOutputfloatValues[4];



	  /*init values*/
	  returnValue				= RETURN_OK;
	  connectionAttempts 		= 0;
	  dataRecievedBuffer[0]     = '\0';
	  bytesRead					= 0;
	  taskExist 				= 1;

	  tcpSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	     if (tcpSocketFD >= 0)
	     {
	    	 bzero((char *) &serv_addr, sizeof(serv_addr));
			 serv_addr.sin_family 		= AF_INET;
			 serv_addr.sin_addr.s_addr  = INADDR_ANY;
			 serv_addr.sin_port 		= htons (TCP_PORT);
			 returnValue = bind(tcpSocketFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
			 fprintf(stderr,"Panel Socket: OK.\n");

	     }
	     else
	     {
	    	 sleep(2);
			 fprintf(stderr," Openning TCPsocket Failed: %d, %d \r",
					 connectionAttempts,returnValue);
			 returnValue = RETURN_ERROR;
	     }

	     rt_task_set_periodic(NULL, TM_NOW, 1000*1000*100);
		  while(1)
		  {
			  if(returnValue == RETURN_OK )
			  {
				  listen(tcpSocketFD,5);
				  clilen = sizeof(cli_addr);

				  tcpAcceptSocketFD = accept(tcpSocketFD,(struct sockaddr *) &cli_addr,&clilen);
				  if (tcpSocketFD >= 0)
				  {
					  bzero(dataRecievedBuffer,256);
				  }
			  }
			      bytesRead = read(tcpAcceptSocketFD,dataRecievedBuffer,255);

				 if (bytesRead >= 0)

				     write(tcpAcceptSocketFD,"I got your message",18);
				     tcpInput=atoi(dataRecievedBuffer);
				 {
					 //TAKEOFF//////////////////////////////////////////////////////////////////////
					 if(tcpInput==TCP_COMM_XTAKEOFF_TRANSFER)
					 { fprintf(stderr,"holaaa\n");
						 taskExist= rt_task_inquire(&takeoff_task,NULL);
						 if(taskExist!=0)
						 {
							 //delete al other tasks
							 rt_task_delete(&land_task);
							 rt_task_delete(&hover_task);
							 rt_task_delete(&climb_task);
							 rt_task_delete(&takeoff_task);
							 rt_task_delete(&descend_task);
							 rt_task_delete(&yaw_left_task);
							 rt_task_delete(&yaw_right_task);

						  returnValue = rt_task_create(&takeoff_task,
														"takeoff_task",
														0,
														3,
														T_JOINABLE);
                        returnValue = rt_task_start(&takeoff_task,
                       	   							&takeoff_task_func,
                       	   							NULL);
						 }
						 else
						 {
							 //task already running
						 }
					 }
					 //STOP//////////////////////////////////////////////////////////////////////
					 if(tcpInput==TCP_COMM_XSTOP_TRANSFER)
					 {

						 rt_task_delete(&takeoff_task);
						 rt_task_delete(&hover_task);
						 rt_task_delete(&climb_task);
						 rt_task_delete(&descend_task);
						 rt_task_delete(&land_task);
						 rt_task_delete(&yaw_left_task);
						 rt_task_delete(&yaw_right_task);
						 panelOutputfloatValues[0]=0 ;
						 panelOutputfloatValues[1]=0 ;
						 panelOutputfloatValues[2]=0 ;
						 panelOutputfloatValues[3]=0 ;
						 returnValue = rt_queue_write(&read_from_panel_queue,
						 								 &panelOutputfloatValues,
						 								 sizeof(panelOutputfloatValues),
						 								 Q_URGENT);
					 }

					 //LAND//////////////////////////////////////////////////////////////////////
					 if(tcpInput==TCP_COMM_XLAND_TRANSFER)
					 {

						 taskExist= rt_task_inquire(&land_task,NULL);
						 if(taskExist!=0)
						 {
							 //delete al other tasks
							 rt_task_delete(&takeoff_task);
							 rt_task_delete(&hover_task);
							 rt_task_delete(&climb_task);
							 rt_task_delete(&descend_task);
							 rt_task_delete(&land_task);
							 rt_task_delete(&yaw_left_task);
							 rt_task_delete(&yaw_right_task);

						  returnValue = rt_task_create(&land_task,
														"land_task",
														0,
														3,
														T_JOINABLE);
						returnValue = rt_task_start(&land_task,
													&land_task_func,
													NULL);
						 }
						 else
						 {
							 //task already running
						 }
					 }
					 //HOVER//////////////////////////////////////////////////////////////////////
					 if(tcpInput==TCP_COMM_XHOVER_TRANSFER)
					 {

						 taskExist= rt_task_inquire(&hover_task,NULL);
						 if(taskExist!=0)
						 {
							 //delete al other tasks
							 rt_task_delete(&takeoff_task);
							 rt_task_delete(&land_task);
							 rt_task_delete(&climb_task);
							 rt_task_delete(&descend_task);
							 rt_task_delete(&hover_task);
							 rt_task_delete(&yaw_left_task);
							 rt_task_delete(&yaw_right_task);

						  returnValue = rt_task_create(&hover_task,
														"hover_task",
														0,
														3,
														T_JOINABLE);
						returnValue = rt_task_start(&hover_task,
													&hover_task_func,
													NULL);
						 }
						 else
						 {
							 //task already running
						 }
					 }

					 //CLIMB//////////////////////////////////////////////////////////////////////
					 if(tcpInput==TCP_COMM_XCLIMB_TRANSFER)
					 {

						 taskExist= rt_task_inquire(&climb_task,NULL);
						 if(taskExist!=0)
						 {
							 //delete al other tasks
							 rt_task_delete(&takeoff_task);
							 rt_task_delete(&land_task);
							 rt_task_delete(&hover_task);
							 rt_task_delete(&descend_task);
							 rt_task_delete(&climb_task);
							 rt_task_delete(&yaw_left_task);
							 rt_task_delete(&yaw_right_task);
						  returnValue = rt_task_create(&climb_task,
														"climb_task",
														0,
														1,
														T_JOINABLE);
						returnValue = rt_task_start(&climb_task,
													&climb_task_func,
													NULL);
						 }
						 else
						 {
							 //task already running
						 }
					 }
					 //DESCEND//////////////////////////////////////////////////////////////////////
					 if(tcpInput==TCP_COMM_XDESCENT_TRANSFER)
					 {

						 taskExist= rt_task_inquire(&descend_task,NULL);
						 if(taskExist!=0)
						 {
							 //delete al other tasks
							 rt_task_delete(&takeoff_task);
							 rt_task_delete(&land_task);
							 rt_task_delete(&hover_task);
							 rt_task_delete(&climb_task);
							 rt_task_delete(&descend_task);
							 rt_task_delete(&yaw_left_task);
							 rt_task_delete(&yaw_right_task);

						  returnValue = rt_task_create(&descend_task,
														"descend_task",
														0,
														1,
														T_JOINABLE);
						returnValue = rt_task_start(&descend_task,
													&descend_task_func,
													NULL);
						 }
						 else
						 {
							 //task already running
						 }
					 }
					 //YAW LEFT//////////////////////////////////////////////////////////////////////
					 if(tcpInput==TCP_COMM_XYAW_LEFT_TRANSFER)
					 {

						 taskExist= rt_task_inquire(&yaw_left_task,NULL);
						 if(taskExist!=0)
						 { fprintf(stderr,"holaa\n");
							 //delete al other tasks
							 rt_task_delete(&takeoff_task);
							 rt_task_delete(&land_task);
							 rt_task_delete(&hover_task);
							 rt_task_delete(&climb_task);
							 rt_task_delete(&descend_task);
							 rt_task_delete(&yaw_left_task);
							 rt_task_delete(&yaw_right_task);
						  returnValue = rt_task_create(&yaw_left_task,
														"yaw_left_task",
														0,
														1,
														T_JOINABLE);
						returnValue = rt_task_start(&yaw_left_task,
													&yaw_left_task_func,
													NULL);
						 }
						 else
						 {
							 //task already running
						 }
					 }


				 }
				 //YAW RIGHT//////////////////////////////////////////////////////////////////////
					 if(tcpInput==TCP_COMM_XYAW_RIGHT_TRANSFER)
					 {

						 taskExist= rt_task_inquire(&yaw_right_task,NULL);
						 if(taskExist!=0)
						 {
							 //delete al other tasks
							 rt_task_delete(&takeoff_task);
							 rt_task_delete(&land_task);
							 rt_task_delete(&hover_task);
							 rt_task_delete(&climb_task);
							 rt_task_delete(&descend_task);
							 rt_task_delete(&yaw_left_task);
							 rt_task_delete(&yaw_right_task);
						  returnValue = rt_task_create(&yaw_right_task,
														"yaw_right_task",
														0,
														1,
														T_JOINABLE);
						returnValue = rt_task_start(&yaw_right_task,
													&yaw_right_task_func,
													NULL);
						 }

						 else
						 {
							 //task already running
						 }
					 }






			  rt_task_wait_period(NULL);
		  }
		  close(tcpAcceptSocketFD);
		  close(tcpSocketFD);

}/*(END) PANEL_TCP_TEST_INCOMING_TASK_FUNC*/

/*******************************************************************************
 * Type         : ...
 * Name         : ...
 * Description  : ...
 * Globals      : ...
 * Input params : ...
 * Output params: ...
 * Return value : ...
 * Features     : ...
 *
 ******************************************************************************/
 int panel_comm_init()
{
	  int returnValue;

	  /*init values*/
	  returnValue 	= RETURN_OK;

	  returnValue = rt_task_create(&panel_tcp_test_incoming_task,
	 							   "panel_tcp_test_incoming_task",
	 							    0,
	 							    3,
	 							    T_JOINABLE);
   if (returnValue == RETURN_OK)
   {
   returnValue = rt_queue_create(&read_from_panel_queue,
								read_from_panel_queue_Name,
								PANEL_OUTPUT_BUFFER_SIZE,
								Q_UNLIMITED,
								Q_SHARED);
   }
   if (returnValue == RETURN_OK)
   {
	  /*start tasks*/
	   returnValue = rt_task_start(&panel_tcp_test_incoming_task,
	   							   &panel_tcp_test_incoming_task_func,
	   							   NULL);
   }
   else
   {
	  fprintf(stderr,"Panel module error\n");
   }

	return (returnValue);
}/* (END) XPLANE_COMM_INIT*/



