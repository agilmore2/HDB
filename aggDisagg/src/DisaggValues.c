/**************************************************************************
* DESCRIPTION
* 
* This file contains functions used in the disaggregation of source values.
* 
***************************************
* DisaggValues
* 
* Purpose: Disaggregate the value passed in to the appropriate number
*          of values, using either division, or a user-specified function.
*
* Assumptions
*
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      VALUE_RECORD*  -- array of source values
*      int            -- number of source values
*      BOUNDARY_DATES -- current set of boundary dates
*      VALUE_RECORD** -- array to hold disaggregated values
*      int *          -- number of disagg'd values; to be set
*
* Output
* warnings and error messages
*
* Return 
* ERROR   - failure; disaggregation not complete
* OK      - disaggregation completed
*
***************************************
* DisaggValuesWithOp
*
* Purpose: Perform disaggregation with known method, division.
*
* Assumptions
*
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      double         -- source value
*      VALUE_RECORD*  -- array of disagg'd values; to be filled in
*      int            -- divisor; indicates number of values to be generated
*
* Output
* warnings and error messages
*
* Return 
* ERROR   - failure; disaggregation not complete
* OK      - disaggregation completed
*
***************************************
* Divide Value
*
* Purpose:   Perform disaggregation using division
* 
* Input
*      double         -- source value
*      VALUE_RECORD*  -- array of disaggregated values; filled in
*      int            -- number of values to generate
*
* Output
*
* Return
* 	int - OK or ERROR
*
***************************************
* ComputeDisaggDates
*
* Purpose:   Set the start dates for each generated disaggregated value.
*            Assumes number of dates to generate is equal to the divisor
*            passed in to DisaggValues (on curBoundary)
* 
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      BOUNDARY_DATES -- boundary dates representing this series of values
*      VALUE_RECORD*  -- value/date record array
*
* Output
*
* Return
* 	int - OK or ERROR
*
* AUTHOR 
* Carol Marra
* March, 1996
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aggDisaggDefines.h"
#include "aggDisaggTypedefs.h"
#include "aggDisagg.h"
#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"
#include "userFunctions.h"

extern DISAGG_FUNCTION_TABLE disaggFunctionTable [DISAGG_TABLE_SIZE];

int DisaggValues (RUN_CONTROL runControl, VALUE_RECORD *sourceValueList,
		  int numSourceValues, BOUNDARY_DATES curBoundaries,
		  VALUE_RECORD **disAggValueList,
		  int *numDisAggValues, ID siteDatatypeId)

{ 
  int result;
  int (* disaggFunction) (RUN_CONTROL, double, ID, SQL_DATE, SQL_DATE,
			  VALUE_RECORD *, int);

  /* Aggregated data will result in *divisor* number of values. */
  if ((*disAggValueList = 
      (VALUE_RECORD *) malloc (sizeof (VALUE_RECORD) *
			       curBoundaries.divisor)) == NULL)
    {
      PrintError ("Problem allocating disAggValueList.\n\tExiting.");
      return (ERROR);
    }
						    
  if (!strcmp (runControl.opOrFcn, "M"))
    {
      if ((result = DisaggValuesWithOp (runControl, sourceValueList[0].value,
					*disAggValueList, 
					curBoundaries.divisor)) != OK)
	{
	  return (ERROR);
	}
    }
  
  else if (!strcmp (runControl.opOrFcn, "F"))
    {

/*      disaggFunction = (*DisaggLookup) (runControl.functionName); */

        disaggFunction = DisaggLookup(runControl.functionName);



      if ((result = disaggFunction (runControl, sourceValueList[0].value, 
                                    siteDatatypeId, curBoundaries.dateLow,
				    curBoundaries.dateHigh,
				    *disAggValueList, 
				    curBoundaries.divisor)) 
	  != OK)
	{
	  return (ERROR);
	}
    }

  /* Something's wrong: need an operator or function. */
  else
    {
      PrintError ("You must specify a method or function to disaggregate values.\n\tExiting.");
      return (ERROR);
    }
  
  /* Now set the dates on disAggValueList; assumes that all disagg 
     operations result in *divisor* number of values. */
  *numDisAggValues = curBoundaries.divisor;

  if ((result = ComputeDisaggDates (runControl, curBoundaries, 
				    *disAggValueList)) != OK)
    {
      PrintError ("Error computing dates for disaggregated value.\n\tExiting.");
      return (ERROR);
    }

  return (OK);
}
  
/**************************************************************************/
#define FUNC_NAME "DisaggValuesWithOp"

int DisaggValuesWithOp (RUN_CONTROL runControl, double sourceValue, 
			VALUE_RECORD *disAggValueList, int n_of_intervals)
{
  /* Currently, only division and 'none' are handled. */
   if (runControl.methodClassId == DIV_METHOD_CLASS)
    {
      DivideValue (sourceValue, disAggValueList, n_of_intervals, n_of_intervals);
    }

  else if (runControl.methodClassId == COPY_METHOD_CLASS)
    {
      DivideValue (sourceValue, disAggValueList, 1, n_of_intervals);
    }

  else 
    {
      PrintError ("Disaggregation method %d is unknown.\n\tExiting.",
		   runControl.methodId);
      return (ERROR);
    }

  return (OK);
}
  
/************************************************************************/
#define FUNC_NAME "DivideValue"

int DivideValue (double sourceValue, VALUE_RECORD *disAggValueList,
		 int divide_value, int n_of_intervals)
{
  int count;
  double disaggValue;

  disaggValue = (double) sourceValue / (double) divide_value;

  for (count = 0; count < n_of_intervals; count++)
    {
      disAggValueList[count].value = disaggValue;
    }

  return (OK);
}


/*************************************************************************/
#define FUNC_NAME "ComputeDisaggDates"

int ComputeDisaggDates (RUN_CONTROL runControl, BOUNDARY_DATES boundaries,
			VALUE_RECORD *disAggValueList)

{
  int result,
      count;
  SQL_DATE prevStartDate;

  /* Ensure that first date is represented to appropriate time granularity. */
  if ((result = SqlDateMath (ADDITION, boundaries.dateLow, 
			     disAggValueList[0].dateStart, 0, 
			     runControl.sqlObsSub)) != OK)
    {
      return (ERROR);
    }

  /* Calculate end date based on start date */
  if ((result = SqlDateMath (ADDITION, disAggValueList[0].dateStart, 
			     disAggValueList[0].dateEnd, 1, 
			     runControl.sqlObsSub)) != OK)
    {
      return (ERROR);
    }

  strcpy (prevStartDate, disAggValueList[0].dateStart);

  for (count = 1; count < boundaries.divisor; count++)
    {
      if ((result = SqlDateMath (ADDITION, prevStartDate,
				 disAggValueList[count].dateStart,
				 1, runControl.sqlObsSub)) != OK)
	{
	  return (ERROR);
	}

      if ((result = SqlDateMath (ADDITION, disAggValueList[count].dateStart,
				 disAggValueList[count].dateEnd,
				 1, runControl.sqlObsSub)) != OK)
	{
	  return (ERROR);
	}

      strcpy (prevStartDate, disAggValueList[count].dateStart);
    }

  return (OK);
}


/*************************************************************************/

#define FUNC_NAME "DisaggLookup"

DISAGG_FCN_PTR DisaggLookup (char *functionName) 

{
  int i,
      length;
  
  for (i = 0; i < DISAGG_TABLE_SIZE; i++)
    {
      length = strlen (functionName);
      if (!strncmp (functionName, disaggFunctionTable[i].functionName, length))
	{
	  return (disaggFunctionTable[i].fcn);
	}
    }

  return (NULL);
}
			      
/*************************************************************************/

#define FUNC_NAME "lin"

int lin (RUN_CONTROL runControl, double sourceValue,
                        ID siteDatatypeId, SQL_DATE dateLow,
                        SQL_DATE dateHigh,
                        VALUE_RECORD *disAggValueList, int divisor)

{

  double previous_timestep_value;
  double increment_value;
  int    result;
  int count;

  result = 
    SqlFetchPreviousTimeStepValue(runControl.tableSource, runControl.obsSource,
				  siteDatatypeId, dateLow, &previous_timestep_value, 
				  runControl.sourceIsRange, runControl.sourceIsReal,
				  runControl.model_run_id_src);

  if (result != OK)
  {
      PrintError("Could not retrieve value for previous time step.\n Exiting...");
      exit(ERROR);
  } 

  increment_value = (sourceValue - previous_timestep_value)/divisor;

  for (count = 1; count <= divisor; count++)
  {
      disAggValueList[count-1].value = previous_timestep_value + (increment_value*count);
  }

  return (OK);
}

/*************************************************************************/

#define FUNC_NAME "YourDisaggFunction"

int YourDisaggFunction (RUN_CONTROL runControl, double sourceValue, 
			ID siteDatatypeId, SQL_DATE dateLow,
			SQL_DATE dateHigh,
			VALUE_RECORD *disAggValueList, int divisor)

{
  int count;

  for (count = 0; count < divisor; count++)
    {
      disAggValueList[count].value = (double) NA;
    }

  return (OK);
}
