/*****************************************************************************
* DESCRIPTION
* This file contains the function which creates queries for the input dmi.
*
******************************
* PrepareForQueries
*
* Purpose: This routine takes several parameters and builds two queries -- one
*            to retrieve data from the appropriate r_ table and one from the
*            appropriate m_ table.
*
* Input
*      dmi_header_struct * current -- the member of list for which query is built
*      char *              histStatement -- pointer to returned hist query
*      char *              modelStatement -- pointer to returned model query
*      TIME_INTERVAL_TYPES timeStep -- the time step of data being fetched
*      char *              sqlTimeInterval -- pointer to returned time interval
*
* Output
*      char *              histStatement -- pointer to returned hist query
*      char *              modelStatement -- pointer to returned model query
*      char *              sqlTimeInterval -- pointer to returned time interval
*
* Return
* OK or ERROR
*
* Author
*   Paddy McCarthy, Dec '95
*
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "dailyDmiIn.h"
#include "dbutils.h"
#include "dmi_utils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

#define FUNC_NAME       "PrepareForQueries"

int PrepareForQueries(dmi_header_struct * current,
		      char * histStatement,
		      char * modelStatement,
		      TIME_INTERVAL_TYPES timeInterval)
{
  int result;
  char histTableName[35];
  char modelTableName[35];
  
  switch (timeInterval) 
    {
    case BY_HOUR:
      strcpy (histTableName, REAL_HOUR);
      strcpy (modelTableName, MODEL_HOUR);
      break;
    case BY_DAY:
      strcpy (histTableName, REAL_DAY);
      strcpy (modelTableName, MODEL_DAY);
      break;
    case BY_MONTH:
      strcpy (histTableName, REAL_MONTH);
      strcpy (modelTableName, MODEL_MONTH);
      break;
    default:
      PrintError ("DMI: Time step %d not known", timeInterval);
      return (ERROR);
      break;
    }
  
  /*
   *   The model run start date delimits which table data is fetched from.
   *     -- Data is fetched from m_ tables for days falling on run
   *          control start date and later.
   *     -- Data is fetched from r_ tables for all days before run
   *          control start date.
   *
   *   The dates that delimit the hist and model intervals are initialized
   *    in SetMemberStartDateAndNumTimesteps() called in the main routine.
   */
  
  /* Create the queries sent to the database */
  if (current->hist_source == REAL_DATA_TABLE)
    { 
      sprintf (histStatement, "SELECT to_char (start_date_time, 'dd-mon-yyyy hh24:mi:ss'), value FROM %s WHERE site_datatype_id = %d AND start_date_time BETWEEN to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') ORDER by start_date_time",
			histTableName,
			current->hdb_site_datatype_id,
			current->hdb_startdate,
			current->hdb_hist_data_enddate);
    } 
  /* Otherwise, historical data is coming from a model. */
  else 
    {
      result = sprintf (histStatement, "SELECT to_char (start_date_time, 'dd-mon-yyyy hh24:mi:ss'), value FROM %s WHERE model_run_id = %d AND site_datatype_id = %d AND start_date_time BETWEEN to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') ORDER by start_date_time",
			modelTableName,
			current->hist_source,
			current->hdb_site_datatype_id,
			current->hdb_startdate,
			current->hdb_hist_data_enddate);
    }
  
  result = sprintf (modelStatement, "SELECT to_char (start_date_time, 'dd-mon-yyyy hh24:mi:ss'), value FROM %s WHERE model_run_id = %d AND site_datatype_id = %d AND start_date_time BETWEEN to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') ORDER by start_date_time",
		    modelTableName,
		    current->model_source,
		    current->hdb_site_datatype_id,
		    current->hdb_model_data_startdate,
		    current->hdb_enddate);
  
  /* Make the space to hold the values */
  current->data = malloc(sizeof(data_array) * current->number_of_timesteps);
  if (!current->data) {
    PrintError("DMI: Could not malloc data array.\n");
    return ERROR;
  }
  
  return OK;
}

