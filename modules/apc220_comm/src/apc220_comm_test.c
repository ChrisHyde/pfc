/*
 ============================================================================
 Name        : serialComm.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include "apc220_comm.h"




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

	int main(int argc,char *argv[])
	{

	    int   returnValue;

	    returnValue= apc220_comm_init();

	    while(1)
	    {

	    }

	    return(returnValue);

	}
