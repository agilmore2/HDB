/*****************************************************************************
* DESCRIPTION 
* This file contains functions which, for a given siteDatatypeId, 
* process all values as specified in the command line.
*
* For every set of boundary dates:
*   - load source values from database
*   - aggregate or disaggregate
*   - convert resulting time series (convert units and ratios)
*   - output time series as requested (DB or file)
* 
***************************************
*
* ProcessNonStats
*
* Purpose: 
*
* Assumptions
*
* Input
*      RUN_CONTROL       --  structure holding info to drive application
*      BOUNDARY_DATES *  --  array of boundary dates
*      int               --  number of boundary date pairs
*      ID                --  sourceSiteDatatypeId to process
*      ID                --  destSiteDatatypeId to generated data for
*
* Output
* warnings and error messages
*
* Return 
* ERROR   - failure; processing not complete
* OK      - processing successfully completed
*
***************************************
*
* VerifyNumValues
*
* Purpose: 
*
* Assumptions
*
* Input
*      RUN_CONTROL       --  structure holding info to drive application
*      int               --  divisor 
*      int               --  number of source values
*
* Output
* warnings and error messages
*
* Return 
* MISSING_VALUE   - number of source values is incorrect
* OK              - number of source values is correct
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aggDisagg.h"
#include "aggDisaggDefines.h"
#include "aggDisaggTypedefs.h"
#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

extern TIME_INTERVALS timeIntervals[NUM_TIME_INTERVALS];

#define FUNC_NAME "ProcessNonStats"

int ProcessNonStats (RUN_CONTROL runControl, BOUNDARY_DATES *boundaryDates, 
		     int numBoundaries, ID sourceSiteDatatypeId, 
		     ID destSiteDatatypeId)

{

  int timeSeriesComplete,
      numSourceValues = 0,
      numDisAggValues = 0,
      boundary,
      result;
  VALUE_RECORD *sourceValueList,
               *disAggValueList;
  FILE *filePtr = NULL;

#ifdef DEBUG
  printf ("\n\n********* VALUES FOR SiteDatatypeID: %d ****************\n", 
	  sourceSiteDatatypeId);
#endif
  
  /* Open data file for ascii data sink. */
  if (!runControl.toDb)
    {
      if ((result = OpenInitDataFile (sourceSiteDatatypeId, 
				      destSiteDatatypeId, runControl,
				      &filePtr)) != OK)
	{
	  PrintError ("Problem opening data file for SDI: %d for output.\n\tExiting.", sourceSiteDatatypeId);
	  return (ERROR);
	}
    }

  for (boundary = 0; boundary < numBoundaries; boundary++)
    {
      timeSeriesComplete = TRUE;
      numSourceValues = 0;

      if ((result = RetrieveSourceValues 
	   (runControl, sourceSiteDatatypeId, boundaryDates[boundary].dateLow,
	    boundaryDates[boundary].dateHigh, &sourceValueList, 
	    &numSourceValues)) != OK)
	{
	  PrintError ("Error retrieving values for site_datatype_id %d between %s and %s.\n\tExiting.", sourceSiteDatatypeId, 
		      boundaryDates[boundary].dateLow,
		      boundaryDates[boundary].dateHigh);
	  return (ERROR);
	}

#ifdef DEBUG      
      PrintSourceValues (sourceValueList, numSourceValues, 
			 sourceSiteDatatypeId);
#endif

      /* Verify that there are correct number of values in source list. */
      if ((result = VerifyNumValues 
	   (runControl, boundaryDates[boundary].divisor,
	    numSourceValues)) == MISSING_VALUE)
	{
	  timeSeriesComplete = FALSE;
	}

      /* Aggregate or disaggregate the data */
      if (timeSeriesComplete)
	{
	  if (runControl.opType == AGG)
	    {
	      if ((result = AggValues (runControl, sourceValueList,
				       numSourceValues, 
				       boundaryDates[boundary],
				       &disAggValueList, &numDisAggValues,
				       sourceSiteDatatypeId)) != OK)
		{
		  PrintError ("Error aggregating values for site_datatype_id %d between %s and %s.\n\tExiting.", sourceSiteDatatypeId, 
			      boundaryDates[boundary].dateLow,
			      boundaryDates[boundary].dateHigh);
		  return (ERROR);
		}
	    }

	  else if (runControl.opType == DISAGG)
	    {
	      if ((result = DisaggValues (runControl, sourceValueList,
					  numSourceValues, 
					  boundaryDates[boundary],
					  &disAggValueList, 
					  &numDisAggValues,
					  sourceSiteDatatypeId)) != OK)
		{
		  PrintError ("Error disaggregating values for site_datatype_id %d between %s and %s.\n\tExiting.", sourceSiteDatatypeId, 
			      boundaryDates[boundary].dateLow,
			      boundaryDates[boundary].dateHigh);
		  return (ERROR);
		}
	    }
	  
	  else
	    {
	      disAggValueList = sourceValueList;
	      numDisAggValues = numSourceValues;
	    }

#ifdef DEBUG	  
	  PrintDisAggValues (runControl.destIsRange, runControl.isStat,
			     disAggValueList, numDisAggValues,
			     sourceSiteDatatypeId);
#endif
	  
	  /* If unit conversion is necessary, call ConvertValue for every 
	     value. */
	  if ((runControl.unitSource != runControl.unitDest) ||
	      runControl.toScale != 1)
	    {
	      if ((result = ConvertEachNonStatValue (runControl, 
						     disAggValueList, 
						     numDisAggValues, 
						     boundaryDates[boundary]))
		  != OK)
		{
		  PrintError ("Problem converting values for SDI %d,\n\tdates %s and %s.\n\tExiting.", sourceSiteDatatypeId, boundaryDates[boundary].dateLow,
			      boundaryDates[boundary].dateHigh);
		  return (ERROR);
		}
	    }

#ifdef DEBUG
	  PrintDisAggValues (runControl.destIsRange, runControl.isStat,
			     disAggValueList, numDisAggValues,
			     sourceSiteDatatypeId);
#endif
	}
      /* Time series is not complete. Set disAggValueList to NULL */
      else
	{
	  disAggValueList = NULL;

	  PrintError ("Incorrect number of source data values (%d) for siteDatatypeId %d, model_run_id %d, dates %s and %s.\n\tSkipping this date range for this siteDatatype.\n", numSourceValues, sourceSiteDatatypeId, 
		      runControl.model_run_id_src,
		      boundaryDates[boundary].dateLow,
		      boundaryDates[boundary].dateHigh);
	}
      
      /* Write the data to its sink; if time series is not complete,
	 database data will not be written, and NA will be written
	 to output files. First, set num values for incomplete time series,
	 in case NAs need to be written. */
      if (!timeSeriesComplete)
	{
	  switch (runControl.opType)
	    {
  	      case AGG: 
	          numDisAggValues = 1;
		  break;
  	      case DISAGG: 
		  numDisAggValues = boundaryDates[boundary].divisor;
		  break;
            }
	}
      
      if ((result = WriteValues (runControl, destSiteDatatypeId,
				 disAggValueList, numDisAggValues,
				 timeSeriesComplete, filePtr)) 
	  != OK)
	{
	  PrintError ("Error writing values for site_datatype_id %d between %s and %s.\n\tExiting.", destSiteDatatypeId, 
		      boundaryDates[boundary].dateLow,
		      boundaryDates[boundary].dateHigh);
	  return (ERROR);
	}
      
      /* Free sourceValueList and disAggValueList, only if they
       were used. */
      if (timeSeriesComplete)
	{
	  if (sourceValueList != disAggValueList)
	    free (disAggValueList);
	  free (sourceValueList);
	}

    } /* boundary */
  
  if (!runControl.toDb)
    fclose (filePtr);
  
  return (OK);
}      
            
      
/*************************************************************************/

#define FUNC_NAME "VerifyNumValues"

int VerifyNumValues (RUN_CONTROL runControl, int divisor,
		     int numSourceValues)

{
  /* For aggregation: 
      - if source is range data, need numPer values
      - if source is non-range data, need divisor values 
      - is source data is instantaneous, need at least one. */
  
  if (runControl.obsSource == BY_INSTANT)
    {
     if (numSourceValues >0)
       {
          return (OK);
       }
     else
       {
          return (MISSING_VALUE);
       }
    }

  if (runControl.opType == AGG)
    {
      if (runControl.sourceIsRange)
	{
	  if (numSourceValues != runControl.numPer)
	    return (MISSING_VALUE);
	}
      else
	{
	  if (numSourceValues != divisor)
	    return (MISSING_VALUE);
	}
    }
  /* For disaggregation:
      - there must be one and only one source value in the list. */
  else if (runControl.opType == DISAGG)
    {
      if (numSourceValues != 1)
	return (MISSING_VALUE);
    }

  return (OK);
}
  
/****************************************************************************/

#define FUNC_NAME "ConvertEachNonStatValue"

int ConvertEachNonStatValue (RUN_CONTROL runControl, 
			     VALUE_RECORD * disAggValueList,
			     int numDisAggValues, BOUNDARY_DATES curBoundary)

{
  int result,
      valueCount,
      nSeconds;

  /* If this is range data, nSeconds cannot be calculated by ConvertValue
     because more than one interval is represented. So, do calculation
     by getting number of seconds between start and end of range. 
     If you've disaggregated, you know you're dealing with non-range 
     destination data and ConvertValue can get nSeconds, so this step 
     is unneeded. */
  if (runControl.opType != DISAGG && 
      (runControl.sourceIsRange || runControl.destIsRange))
    {
      if ((result = CalcRangeNSeconds (runControl, curBoundary, &nSeconds))
	  != OK)
	{
	  return (ERROR);
	}
    }
  else
    nSeconds = NA;

#ifdef DEBUG
  PrintError ("************   CONVERTING VALUES   ************\n");
  PrintError ("Calling ConvertValue with: \n\tSUnit %d, DUnit %d, DataDate: %s, DestTime: %d, nSeconds: %d\n\n", runControl.unitSource, runControl.unitDest,
	      "valueTimeStamp", runControl.obsDest, nSeconds);
#endif
  
  /* Use time stamp associated with the value to compute nSeconds in
     ConvertValue. */
  for (valueCount = 0; valueCount < numDisAggValues; valueCount++)
    {
      if ((result = ConvertValue (disAggValueList[valueCount].value, 1,
				  runControl.unitSource, 
				  disAggValueList[valueCount].dateStart,
				  &(disAggValueList[valueCount].value),
				  runControl.toScale, runControl.unitDest,
				  runControl.obsDest, nSeconds)) != OK)
	{
	  return (ERROR);
	}
    }

  return (OK);
} 

/****************************************************************************/

#define FUNC_NAME "CalcRangeNSeconds"

int CalcRangeNSeconds (RUN_CONTROL runControl, BOUNDARY_DATES curBoundary,
		       int *nSeconds)

{
  int result,
      nDays;
  SQL_DATE endIntervalDate;


  /* DateHigh always needs to be set here to the end of the interval it
     represents, otherwise numSeconds will be incorrect. The interval it
     represents is obsSub. */
  if ((result = GetEndOfInterval (curBoundary.dateHigh, runControl.sqlObsSub,
				  endIntervalDate)) != OK)
    {
      PrintError ("%s: Exiting.", FUNC_NAME);
      return (ERROR);
    }
				  
  if ((result = SqlNumDaysBetweenDates (curBoundary.dateLow, 
					endIntervalDate,
					&nDays)) != OK)
    {
      PrintError ("%s: Could not calculate nSeconds for range value processing.\n\tExiting.", FUNC_NAME);
      return (ERROR);
    }

  *nSeconds = nDays * SECONDS_IN_DAY;
  
  return (OK);
}
