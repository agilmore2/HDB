/**************************************************************************
* DESCRIPTION
* 
* This file contains functions used in the aggregation of source values.
* 
***************************************
* AggValues
* 
* Purpose: Aggregate the list of values into a single value
*          and set the element of the new value array appropriately.
*
* Assumptions
*
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      VALUE_RECORD*  -- array of source values
*      int            -- number of source values
*      BOUNDARY_DATES -- current set of boundary dates
*      VALUE_RECORD*  -- array to hold aggregated value
*      int *          -- number of agg'd values; to be set
*
* Output
* warnings and error messages
*
* Return 
* ERROR   - failure; aggregation not complete
* OK      - aggregation completed
*
***************************************
* AggValuesWithOp
*
* Purpose: Perform aggregation with known methods such
*          as ave, sum, max, min
*
* Assumptions
*
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      VALUE_RECORD*  -- array of source values
*      int            -- number of source values
*      float *        -- resulting value
*
* Output
* warnings and error messages
*
* Return 
* ERROR   - failure; aggregation not complete
* OK      - aggregation completed
*
***************************************
* TotalValues
*
* Purpose:   Perform sum or averaging aggregation
* 
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      VALUE_RECORD*  -- array of source values
*      int            -- number of source values
*      float *        -- resulting value
*
* Output
*
* Return
* 	int - OK or ERROR
*
***************************************
* SortValues
*
* Purpose:   Perform max or min aggregation
* 
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      VALUE_RECORD*  -- array of source values
*      int            -- number of source values
*      float *        -- resulting value
*
* Output
*
* Return
* 	int - OK or ERROR
*
***************************************
* ValueCompare
*
* Purpose:   Compare the values of two VALUE_RECORD * parameters;
*            called by qsort
* 
* Input
* 	VALUE_RECORD *
* 	VALUE_RECORD *
*
* Output
*
* Return
* 	int - positive, negative or zero value
*
***************************************
* ComputeAggDate
*
* Purpose:   Set the start (and end) date (or indexes) for the
*            aggregated data value on the VALUE_RECORD.
* 
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      BOUNDARY_DATES -- boundary dates representing this value
*      VALUE_RECORD*  -- pointer to value/date record
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
#include "dmi_utils.h"
#include "defines.h"
#include "typedefs.h"
#include "userFunctions.h"

extern AGG_FUNCTION_TABLE aggFunctionTable [AGG_TABLE_SIZE];

int AggValues (RUN_CONTROL runControl, VALUE_RECORD *sourceValueList,
	       int numSourceValues, BOUNDARY_DATES curBoundaries,
	       VALUE_RECORD **disAggValueList,
	       int *numDisAggValues, ID siteDatatypeId)

{ 
  int result;
  double value;
  int (* aggFunction) (RUN_CONTROL, VALUE_RECORD *, int, ID,
		       SQL_DATE, SQL_DATE, double *);

  /* All aggregated data, including dimension changes, will result 
     in one value */

  if ((*disAggValueList = 
       (VALUE_RECORD *) malloc (sizeof (VALUE_RECORD))) == NULL)
  {
     PrintError ("Problem allocating disAggValueList.\n\tExiting.");
     return (ERROR);
  }

  if (!runControl.dimensionChangeInBaseArea)
  {
     if (!strcmp (runControl.opOrFcn, "M"))
     {
	if ((result = AggValuesWithOp (runControl, sourceValueList, 
				       numSourceValues, &value)) != OK)
	{
	   return (ERROR);
	}
     }
     
     else if (!strcmp (runControl.opOrFcn, "F"))
     {
	aggFunction = (*AggLookup) (runControl.functionName);
	if ((result = aggFunction (runControl, sourceValueList, 
				   numSourceValues, siteDatatypeId,
				   curBoundaries.dateLow, 
				   curBoundaries.dateHigh, &value)) != OK)
	{
	   return (ERROR);
	}
     }
     
     /* Something's wrong: need an operator or function. */
     else
     {
	PrintError ("You must specify a method or function to aggregate values.\n\tExiting.");
	return (ERROR);
     }
     
     /* Now set the date and value on disAggValueList, for non
      dimension changes */
     (*disAggValueList)[0].value = value;

     if ((result = ComputeAggDate (runControl, curBoundaries, 
				   &(*disAggValueList[0]))) != OK)
     {
	PrintError ("Error computing date for aggregated value.\n\tExiting.");
	return (ERROR);
     }
  }
  else
  {
     /* No real aggregations to do. Just set value and date on
	disAggValueList for every corresponding source value; unit
	conversion is done later. */
     (*disAggValueList)[0].value = sourceValueList[0].value;
     strcpy ((*disAggValueList)[0].dateStart, sourceValueList[0].dateStart);
     strcpy ((*disAggValueList)[0].dateEnd, sourceValueList[0].dateEnd);
  }

  *numDisAggValues = 1;

  return (OK);
}
  
/**************************************************************************/
#define FUNC_NAME "AggValuesWithOp"

int AggValuesWithOp (RUN_CONTROL runControl, VALUE_RECORD *sourceValueList,
		     int numSourceValues, double *value)
{
  /* Compute totals or averages */
  if ((runControl.methodClassId == AVE_METHOD_CLASS) ||
      (runControl.methodClassId == SUM_METHOD_CLASS))
    {
      TotalValues (runControl.methodClassId, sourceValueList, numSourceValues,
		   value);
    }

  /* Find max or min values. */
  else if ((runControl.methodClassId == MAX_METHOD_CLASS) ||
	   (runControl.methodClassId == MIN_METHOD_CLASS) ||
	   (runControl.methodClassId == EOP_METHOD_CLASS))
    {
      SortValues (runControl.methodClassId, sourceValueList, numSourceValues,
		  value);
    }

  else
    {
      PrintError ("%s: MethodClassId %d unknown.\n\tExiting.", FUNC_NAME,
                  runControl.methodClassId);
      return (ERROR);
    }


  return (OK);
}
  
/************************************************************************/
#define FUNC_NAME "TotalValues"

int TotalValues (ID methodClassId, VALUE_RECORD *sourceValueList,
		 int numSourceValues, double *value)
{
  int count;
  double sum = 0;

  for (count = 0; count < numSourceValues; count++)
    {
      sum += sourceValueList[count].value;
    }

  if (methodClassId == SUM_METHOD_CLASS)
    *value = sum;
  else if (methodClassId == AVE_METHOD_CLASS)
    *value = (double)sum / (double)numSourceValues;

  return (OK);
}


/************************************************************************/

#define FUNC_NAME "SortValues"

int SortValues (ID methodClassId, VALUE_RECORD *sourceValueList,
		 int numSourceValues, double *value)
{
  if (methodClassId == EOP_METHOD_CLASS)
    {
      qsort (sourceValueList, numSourceValues, sizeof (VALUE_RECORD),
	     (int (*) (const void*,const void *)) DateCompare);
      *value = sourceValueList[numSourceValues -1].value;
    }
     
  else /* min or max */
    {
      qsort (sourceValueList, numSourceValues, sizeof (VALUE_RECORD),
	     (int (*) (const void*,const void *)) ValueCompare);

      if (methodClassId == MAX_METHOD_CLASS)
	*value = sourceValueList[numSourceValues -1].value;
      else if (methodClassId == MIN_METHOD_CLASS)
	*value = sourceValueList[0].value;
    }

  return (OK);
}

/************************************************************************/

#define FUNC_NAME "ValueCompare"

int ValueCompare (VALUE_RECORD *rec1, VALUE_RECORD *rec2)
{
  
  return (rec1->value - rec2->value);
}

/************************************************************************/

#define FUNC_NAME "DateCompare"

int DateCompare (VALUE_RECORD *rec1, VALUE_RECORD *rec2)
{
  int compareResult,
      result;

  if ((result = SqlDateCompare (rec1->dateStart, rec2->dateStart,
				&compareResult)) != OK)
    {
      PrintError ("Problem comparing dates for EOP aggregation.\n\t Exiting somewhat ungracefully from qsort.");
      exit (ERROR);
    }

  return (compareResult);
}

/*************************************************************************/
#define FUNC_NAME "ComputeAggDate"

int ComputeAggDate (RUN_CONTROL runControl, BOUNDARY_DATES boundaries,
		    VALUE_RECORD * valueRecord)

{
  int result;

  if (!runControl.isStat)
    {
      if ((result = SqlDateTrunc (boundaries.dateLow, 
				  runControl.sqlObsDriver,
				  valueRecord->dateStart)) != OK)
	{
	  return (ERROR);
	}

      /* Set end date for range value */
      if (runControl.destIsRange)
	{
	   if ((result = SqlDateTrunc (boundaries.dateHigh,
				       runControl.sqlObsDriver,
				       valueRecord->dateEnd)) != OK)
	   {
	      return (ERROR);
	   }
	}
      /* Set end date for interval (non-base area) data */
      else
        {
	   if ((result = SqlDateMath (ADDITION, valueRecord->dateStart,
				      valueRecord->dateEnd, 1,
				      runControl.sqlObsDriver)) != OK)
	   {
	      return (ERROR);
	   }
	}
    }
  else
    {
      /* Date part will retrieve correct index for hourly (hour of day)
	 and monthly (month of year) stats only. Must do day (of year)
	 stat indexes separately. */
      if (runControl.obsDest != BY_DAY)
	{
	  if ((result = SqlExtractDatePart (boundaries.dateLow, 
					    runControl.sqlObsDriver,
					    &(valueRecord->timeIndexStart))) 
	      != OK)
	    {
	      return (ERROR);
	    }
	  
	  /* Set end index for range value if necessary */
	  if (runControl.destIsRange)
	    {
	      if ((result = SqlExtractDatePart (boundaries.dateHigh,
						runControl.sqlObsDriver,
						&(valueRecord->timeIndexEnd))) 
		  != OK)
		{
		  return (ERROR);
		}
	    }
	}
      /* Get indexes for day-of-year stats. Note that day-of-year, range
       stats cannot be created. */
      else
	{
	  if ((result = GetDayOfYear (boundaries.dateLow, 
				      &(valueRecord->timeIndexStart))) != OK)
	    {
	      return (ERROR);
	    }
	}
    }

  return (OK);
}


/*************************************************************************/

#define FUNC_NAME "AggLookup"

AGG_FCN_PTR AggLookup (char *functionName) 

{
  int i,
      length;
  
  for (i = 0; i < AGG_TABLE_SIZE; i++)
    { 
      length = strlen (functionName);
      if (!strncmp (functionName, aggFunctionTable[i].functionName, length))
	{
	  return (aggFunctionTable[i].fcn);
	}
    }

  return (NULL);
}
			      
/*************************************************************************/

#define FUNC_NAME "InstToAve"

int InstToAve (RUN_CONTROL runControl, VALUE_RECORD *sourceValueList,
	       int numSourceValues, ID siteDatatypeId, SQL_DATE dateLow,
	       SQL_DATE dateHigh, double *value)

{
  *value = (double) NA;
  return (OK);
}

/*************************************************************************/

#define FUNC_NAME "YourFunction"

int YourFunction (RUN_CONTROL runControl, VALUE_RECORD *sourceValueList,
		  int numSourceValues, ID siteDatatypeId, SQL_DATE dateLow,
		  SQL_DATE dateHigh, double *value)

{
  *value = (double) NA;
  return (OK);
}


