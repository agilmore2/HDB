/*****************************************************************************
* DESCRIPTION
* This file contains function which finds an HDB timestep (TIME_INTERVAL_TYPE)
* given a string with the RiverWare timestep.
*
*********************************
* LookupTimestep
*
* Purpose: Find HDB timestep for RiverWare timestep string
*
* Input
*       char *              - RiverWare timestep string
*       TIME_INTERVAL_TYPES * - matching HDB timestep; set by function
*
*
* Return
*       ERROR - timestep not successfully retrieved
*       OK - timestep successfully retrieved
*
*
*********************************
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "dbutils.h"
#include "dmi_utils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

#define FUNC_NAME "LookupTimestep"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> LookupTimestep


<P> <STRONG>Function signature:</STRONG> int LookupTimestep(char *, TIME_INTERVAL_TYPES )

<P> <STRONG>Function description:</STRONG> Given a Riverware timestep string, lookup the corresponding HDB timestep.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *riverwareTimestep:           Riverware timestep string.
<BR><DD> TIME_INTERVAL_TYPES *timestep: Corresponding HDB timestep. Looked up.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int LookupTimestep(char * riverwareTimestep, TIME_INTERVAL_TYPES *timestep)
{
    int i;

    i = 0;
    while (timestep_lookup_array[i].riverware_timestep_name[0] != '\0') 
    {
      if (!strcmp(riverwareTimestep, 
	      timestep_lookup_array[i].riverware_timestep_name))
	    break;
      i++;
    }

    if (timestep_lookup_array[i].riverware_timestep_name[0] != '\0') 
    {
      *timestep = timestep_lookup_array[i].hdb_timestep;
      return(OK);
    }
    else 
    {
      *timestep = NA;
      return(ERROR);
    }
}
    

