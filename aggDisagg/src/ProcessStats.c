/*****************************************************************************
* DESCRIPTION 
* This file contains functions which, for a given siteDatatypeId, 
* process all values as specified in the command line, for computing
* statistics.
*
* Note that boundary date sets are processed more than one at a time.
* For instance, if daily values are being processed into monthly
* statistics for Jan, 1993 - Dec 1995, there will be a boundary date 
* range for:
*    Jan 1 - Jan 31, 1993
*    Jan 1 - Jan 31, 1994
*    Jan 1 - Jan 31, 1995
*
* All values for all Januaries must be processed together, in the 
* same source value list. So, for this *set* of boundary dates:
*
*   - load all source values from database (Jan 93, Jan 94, Jan 95)
*   - aggregate or disaggregate 
*   - convert resulting time series (convert units and ratios)
*   - output time series as requested (DB or file)
* 
***************************************
*
* ProcessStats
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

#define FUNC_NAME "ProcessStats"

int ProcessStats (RUN_CONTROL runControl, BOUNDARY_DATES *boundaryDates, 
		  int numBoundaries, ID sourceSiteDatatypeId, 
		  ID destSiteDatatypeId, TIME_INTERVALS *timeIntervals)

{
  int numSourceValues = 0,
      numDisAggValues = 0,
      boundary,
      result,
      maxBoundary,
      nextBoundary,
      increment;
  VALUE_RECORD *sourceValueList,
               *disAggValueList;
  FILE *filePtr;

  if (runControl.sourceIsRange || runControl.destIsRange)
    maxBoundary = 1;
  else
    switch (runControl.obsDest)
      {
        case BY_HOUR: 
	  maxBoundary = 24;
	  break;
	case BY_DAY:
	  maxBoundary = 365;
	  break;
	case BY_MONTH:
	  maxBoundary = 12;
	  break;
	default:
	  PrintError ("Dave... Don't you think you should reconsider this?\n\tAre you *sure* you want to create statistics by *%s*?\n\tExiting, Dave...\n", timeIntervals[runControl.obsDest].ingresName);
	  return (ERROR);
	}

  /* Increment to reach next corresponding boundary dates is same
     as maxBoundary. */
  increment = maxBoundary;

#ifdef DEBUG
  printf ("\n\n********* VALUES FOR SiteDatatypeID: %d ****************\n", 
	  sourceSiteDatatypeId);
#endif

  /* Open data file for ascii data sink. If a destination SDI exists, the
   file is named after this; if not*/
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

  for (boundary = 0; boundary < maxBoundary; boundary++)
    {
      numSourceValues = 0;

      /* Loop to get all values which belong in one statistic; eg
	 all March values for the entire POR. */
      for (nextBoundary = boundary; nextBoundary < numBoundaries;
	   nextBoundary += increment)
	{
	  /* Retrieve all source values to be used in calculating a 
	     statistical value; values from subsequent date boundaries
	     will be tacked onto end of sourceValueList. */
	  if ((result = RetrieveSourceValues 
	       (runControl, sourceSiteDatatypeId, 
		boundaryDates[nextBoundary].dateLow,
		boundaryDates[nextBoundary].dateHigh, &sourceValueList, 
		&numSourceValues)) != OK)
	    {
	      PrintError ("Error retrieving values for site_datatype_id %d between %s and %s.\n\tExiting.", sourceSiteDatatypeId, 
			  boundaryDates[nextBoundary].dateLow,
			  boundaryDates[nextBoundary].dateHigh);
	      return (ERROR);
	    }
	}

#ifdef DEBUG 
      PrintSourceValues (sourceValueList, numSourceValues, 
			 sourceSiteDatatypeId);
#endif

      /* Aggregate or disaggregate the data; no need to verify number 
	 of values retrieved. Note also that if you have stats as result data,
	 you *must* aggregate to get to it, as you can't start w/stats and, e.g., 
	 do a simple unit conversion. */
      if (runControl.opType == AGG)
	{
	  /* Note: use numSourceValues as divisor */
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
	  /* This should have been caught; not allowed. */
	  PrintError ("Error: disaggregation not allowed in conjunction with statistics.\n\tExiting.");
	  return (ERROR);
	}

#ifdef DEBUG       
      PrintDisAggValues (runControl.destIsRange, runControl.isStat,
			 disAggValueList, numDisAggValues,
			 sourceSiteDatatypeId);
#endif
      
      /* If unit conversion is necessary, call ConvertValue for every value. */
      if ((runControl.unitSource != runControl.unitDest) ||
	  runControl.toScale != 1)
	{
	  if ((result = ConvertEachStatValue (runControl, disAggValueList, 
					      numDisAggValues, 
					      boundaryDates[boundary])) != OK)
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


      /* Pass in TRUE to indicate that time series is complete and
	 values should always be written to database if sink is database. */
      if ((result = WriteValues (runControl, destSiteDatatypeId,
				 disAggValueList, numDisAggValues,
				 TRUE, filePtr)) 
	  != OK)
	{
	  PrintError ("Error inserting/updating values for site_datatype_id %d between %s and %s.\n\tExiting.", destSiteDatatypeId, 
		      boundaryDates[boundary].dateLow,
		      boundaryDates[boundary].dateHigh);
	  return (ERROR);
	}

      /* Free sourceValueList and disAggValueList */
      free (disAggValueList);
      free (sourceValueList);

    }/* boundary */

  if (!runControl.toDb)
    fclose (filePtr);

  return (OK);
}      
      
      
/*********************************************************************************/
#define FUNC_NAME "PrintSourceValues"

int PrintSourceValues (VALUE_RECORD *sourceValueList, int numSourceValues,
		       ID sourceSiteDatatypeId)

{
  int count;

  printf ("\nSOURCE VALUES\n");

  for (count = 0; count < numSourceValues; count++)
    {
      printf ("\t%s\t%f\n", sourceValueList[count].dateStart, sourceValueList[count].value);
    }

  printf ("\n\n");

  return (OK);
}

/*********************************************************************/

#define FUNC_NAME "PrintDisAggValues"

int PrintDisAggValues (int isRange, int isStat, 
		       VALUE_RECORD *disAggValueList, 
		       int numDisAggValues, ID sourceSiteDatatypeId)

{
  int count;

  printf ("\nAGG/DISAGG VALUES\n");

  if (!isStat)
    {
      if (!isRange)
	for (count = 0; count < numDisAggValues; count++)
	  printf ("\t%s\t%f\n", disAggValueList[count].dateStart,
		  disAggValueList[count].value);
      else
	for (count = 0; count < numDisAggValues; count++)
	  printf ("\t%s\t%s\t%f\n", disAggValueList[count].dateStart,
		  disAggValueList[count].dateEnd,
		  disAggValueList[count].value);
    }
  else	
    {
      if (!isRange)
	for (count = 0; count < numDisAggValues; count++)
	  printf ("\t%d\t%f\n", disAggValueList[count].timeIndexStart,
		  disAggValueList[count].value);
      else
	for (count = 0; count < numDisAggValues; count++)
	  printf ("\t%d\t%d\t%f\n", disAggValueList[count].timeIndexStart,
		  disAggValueList[count].timeIndexEnd,
		  disAggValueList[count].value);
    }

  printf ("\n\n");

  return (OK);
}

/***************************************************************************/
#define FUNC_NAME "ConvertEachStatValue"

int ConvertEachStatValue (RUN_CONTROL runControl, 
			  VALUE_RECORD * disAggValueList,
			  int numDisAggValues, BOUNDARY_DATES curBoundary)

{
  int result,
      valueCount,
      nSeconds,
      includesFeb,
      yearInd;
  float nLeapSeconds;
  SQL_DATE endIntervalDate;

  /* For ranges, get nSeconds by getting number of seconds between upper
     and lower boundary dates; for non-ranges, use nSeconds function (nSeconds
     might be smaller than 1 for non-ranges. */
  if (runControl.sourceIsRange || runControl.destIsRange)
    {
      if ((result = CalcRangeNSeconds (runControl, curBoundary, &nSeconds))
	  != OK)
	{
	  return (ERROR);
	}
    }
  
  else
    {
      if ((result = GetNumSeconds (curBoundary.dateLow, runControl.obsDest,
				   &nSeconds)) != OK)
	{
	  PrintError ("%s: Could not get nSeconds (using get) for Feb stats processing.\n\tExiting.", FUNC_NAME);
	  return (ERROR);
	}
    }

  /* Check to see if February is included. Use endOfInterval for dateHigh. */
  if ((result = GetEndOfInterval (curBoundary.dateHigh, runControl.sqlObsSub,
				  endIntervalDate)) != OK)
    {
      PrintError ("%s: Exiting.", FUNC_NAME);
      return (ERROR);
    }
  
  if ((result = SqlDateRangeIncludesFeb (curBoundary.dateLow,
					 endIntervalDate,
					 &includesFeb, &yearInd)) != OK)
    {
      return (ERROR);
    }

  if (includesFeb)
    {
      /* Needs to check whichever year included February and see if it's a
	 leap year. */
      if (yearInd == LOW)
	{
	  if (IsLeapYear (curBoundary.dateLow))
	    nSeconds -= SECONDS_IN_DAY;
	}
      else
	{
	  if (IsLeapYear (curBoundary.dateHigh))
	    nSeconds  -= SECONDS_IN_DAY;
	}
      
      /* For range stats, dateEnd represents first year; it must represent
	 last year of last range, so add (numPer - 1) years before calling 
	 CalcNumLeapSeconds. */
      if (runControl.sourceIsRange || runControl.destIsRange)
	{
	  if ((result = SqlDateMath (ADDITION, runControl.dateEnd,
				     endIntervalDate, runControl.numPer - 1,
				     YEAR)) != OK)
	    {
	      PrintError ("%s: Problem adding %d years to dateEnd %s.\n\tExiting.",
			  FUNC_NAME, runControl.numPer, runControl.dateEnd);
	      return (ERROR);
	    }
	}
      else 
	strcpy (endIntervalDate, runControl.dateEnd);
	    
      if ((result = CalcNumLeapSeconds (runControl.dateStart, endIntervalDate,
					&nLeapSeconds)) != OK)
	{
	  return (ERROR);
	}
      
      nSeconds += nLeapSeconds;
    }

  /* Now convert values */
  PrintError ("************   CONVERTING VALUES   ************\n");
  PrintError ("Calling ConvertValue with: \n\tSUnit %d, DUnit %d, DataDate: %s, DestTime: %d, nSeconds: %d\n\n", runControl.unitSource, runControl.unitDest,
	      curBoundary.dateLow, runControl.obsDest, nSeconds);
	      
  /* If we're creating stats, use the low date of the current boundary
     as the date by which to do numSeconds calculations. Otherwise, just
     pass in the time stamp associated with the value. */
  for (valueCount = 0; valueCount < numDisAggValues; valueCount++)
    {
      if ((result = ConvertValue (disAggValueList[valueCount].value, 1,
				  runControl.unitSource, curBoundary.dateLow, 
				  &(disAggValueList[valueCount].value),
				  runControl.toScale, runControl.unitDest,
				  runControl.obsDest,  nSeconds)) != OK)
	{
	  return (ERROR);
	}
    }

  return (OK);
} 

