/*****************************************************************************
* writeDataFile
*
* Purpose: Writes the data fetched from db to file for RiverWare.
*
* Input
*       dmi_header_struct * -- pointer to member of list of data items
*
* Output
*       data file named in metaControl file
*
* Return
*       ERROR - file not written.
*       OK - file written.
*
* Note:
*	This won't work for hourly timestep. Needs work.
*
* Author:
* 	Paddy McCarthy, Dec '95
*
*****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>

#include "dailyDmiIn.h"
#include "dbutils.h"
#include "dmi_utils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

#define FUNC_NAME       "writeDataFiles"

int writeDataFile(dmi_header_struct *current)
{
    int i;
    char riverwareStartDate[50];
    char riverwareEndDate[50];
    FILE * fp;

    if ( (current->number_of_timesteps) && (!current->data) ) {
        PrintError("DMI: No data passed to writeDataFile.\n");
        return(ERROR);
    }

    if (!current->pr_datafile_name) {
        PrintError("DMI: No filename passed to writeDataFile.\n");
        return(ERROR);
    }

    fp = fopen(current->pr_datafile_name, "w");
    if (!fp) {
        PrintError("DMI: Could not open file %s for writing.\n",
                    current->pr_datafile_name);
        return(ERROR);
    }

    /*
     * Convert the hdb dates to RiverWare format for the data file
     */
    if (hdbToRiverWareDate(current->hdb_startdate, riverwareStartDate,
		       current->timestep) != OK) {
        PrintError("DMI: Could not convert data start date to RiverWare format.\n", 
                    current->hdb_startdate);
        fclose(fp);
        unlink(current->pr_datafile_name);
        return (ERROR);
    }

    if (hdbToRiverWareDate(current->hdb_enddate, riverwareEndDate, 
		       current->timestep) != OK) {
        PrintError("DMI: Could not convert data end date to RiverWare format.\n", 
                    current->hdb_enddate);
        fclose(fp);
        unlink(current->pr_datafile_name);
        return (ERROR);
    }

    /* CAROL */
    /*
     * Start and end dates are now flexible; work w/ any timestep.
     */
    fprintf(fp, "start_date: %s\n", riverwareStartDate);
    fprintf(fp, "end_date: %s\n", riverwareEndDate);

    /*
     * Put the RiverWare units string into the file.
     */
    fprintf(fp, "units: %s\n", current->pr_units);

    for (i = 0; i < current->number_of_timesteps; i++) {
        if (current->data[i].is_nan == 'Y') {
            fprintf(fp, "NaN\n");
        }
        else {
            fprintf(fp, "%10.5f\n", current->data[i].value);
        }
    }
   
    fclose(fp);

    return(OK);
}

