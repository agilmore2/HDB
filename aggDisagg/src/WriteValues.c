/**************************************************************************
* DESCRIPTION
* 
* This file contains functions used to write data to a file and call those
* functions that write it to the database.
* 
***************************************
* WriteValues
* 
* Purpose: Call appropriate data-writing function.
*
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      ID             -- site datatype ID of data to be written
*      VALUE_RECORD*  -- array of values to be written
*      int            -- number of values
*
* Return 
* ERROR   - failure; writing not complete
* OK      - writing completed
*
***************************************
* OpenInitDataFile
* 
* Purpose: Open data file for current site/datatype and write keyword/value
*          pairs.
*
* Input
*      ID             -- site datatype ID of data to be written
*      RUN_CONTROL    -- structure holding info to drive application
*      FILE *         -- pointer to opened file
*
* Return 
* ERROR   - failure; open and writing not complete
* OK      - writing completed
*
***************************************
* WriteFileHeader
* 
* Purpose: Write start date, end date, unit, time step information to file.
*
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      FILE *         -- pointer to opened file
*
* Return 
* ERROR   - failure; writing not complete
* OK      - writing completed
*
***************************************
* WriteRiverWareDates
* 
* Purpose: Write start and end date. Note that statistics and range data
*          will result in start/end dates that RiverWare does not currently
*          handle.
*
* Input
*      RUN_CONTROL    -- structure holding info to drive application
*      FILE *         -- pointer to opened file
*
* Return 
* ERROR   - failure; writing not complete
* OK      - writing completed
*
***************************************
* GetRiverWareDates
* 
* Purpose: Based on range and statistics status of result data, convert
*          passed-in date(s) to RiverWare format (YYYY-MM-DD) and return.
*
* Input
*      int            -- indicates if data is statistics
*      int            -- indicates if data is range
*      SQL_DATE       -- first date to be converted; only date if data
*                        is not range
*      SQL_DATE       -- second date to be converted; end data if data
*                        is range
*      TIME_INTERVAL_TYPES -- indicates time step of result data
*      char *         -- first date in RiverWare format
*      char *         -- second date in RiverWare format
*
***************************************
* WriteAggDisaggFile
* 
* Purpose: Write value(s) associated with current set of boundary dates
*          to the open file for the current site/datatype.
*
* Input
*      RUN_CONTROL    -- run control
*      ID             -- current site datatype ID
*      VALUE_RECORD * -- list of values
*      int            -- number of values
*      int            -- indicates if time series was complete, or if
*                        NAs need to be written
*
* Return 
* ERROR   - failure; writing not complete
* OK      - writing completed
*
* AUTHOR 
* Carol Marra
* April, 1996
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
#include "dmiGlobals.h"
#include "defines.h"
#include "typedefs.h"

extern TIME_INTERVALS timeIntervals[NUM_TIME_INTERVALS];

#define FUNC_NAME "WriteValues"

int WriteValues (RUN_CONTROL runControl, ID siteDatatypeId,
		 VALUE_RECORD *disAggValueList, int numDisAggValues,
		 int timeSeriesComplete, FILE *filePtr)

{ 
  int result;

  if (runControl.toDb && timeSeriesComplete)
    {
      if (runControl.destIsReal)
	{
	  if ((result = SqlWriteRealAggDisagg (runControl, siteDatatypeId,
					       disAggValueList, 
					       numDisAggValues)) != OK)
	    {
	      PrintError ("%s: Exiting.\n", FUNC_NAME);
	      return (ERROR);
	    }
	}
      else 
	{
	  if ((result = SqlWriteModelAggDisagg (runControl, siteDatatypeId,
						disAggValueList, 
						numDisAggValues)) != OK)
	    {
	      PrintError ("%s: Exiting.\n", FUNC_NAME);
	      return (ERROR);
	    }
	}
    }

  /* Write to file. */
  else if (!runControl.toDb)
    {
      if ((result = WriteAggDisaggFile (filePtr, runControl, 
                                        disAggValueList, numDisAggValues, 
					timeSeriesComplete)) != OK)
	{
	  PrintError ("%s: Exiting.\n", FUNC_NAME);
	  return (ERROR);
	}
    }

  return (OK);
}

/***********************************************************************/
#define FUNC_NAME "OpenInitDataFile"

int OpenInitDataFile (ID sourceSiteDatatypeId, ID destSiteDatatypeId,
		      RUN_CONTROL runControl, FILE **filePtr)

{
  int result;

  char objectName[OBJECT_NAME_LEN + 1],
       slotName[SLOT_NAME_LEN + 1],
       fileName[OBJECT_NAME_LEN + SLOT_NAME_LEN + 2];

    /* Get all mappings from the database, including units, object.slot ->
       SDIs, and DB site list. */
    if ((result = DmiLoadMappings ()) != OK)
      {
        PrintError ("Problem loading mappings into global arrays for use in naming output files.\n\tExiting.");
        exit (ERROR);
      }
  
  /* Base name on dest SDI if there is one. If not, or if the mapping fails,
     use source SDI. */
  if (destSiteDatatypeId != NA)
    {
      LookupObjectSlot (destSiteDatatypeId, objectName, slotName);
      if (objectName[0] == '\0')
	LookupObjectSlot (sourceSiteDatatypeId, objectName, slotName);
    }
  else
    LookupObjectSlot (sourceSiteDatatypeId, objectName, slotName);

  /* Create file name: if there's no SDI mapping to object/slot name, 
     use appropriate SDI */
  if (objectName[0] == '\0')
    sprintf (fileName, "SDI_%d", (destSiteDatatypeId != NA) ? destSiteDatatypeId
	     : sourceSiteDatatypeId);
  else
    sprintf (fileName, "%s.%s", objectName, slotName);
      
  if ((*filePtr = fopen (fileName, "w")) == NULL)
    {
      PrintError ("Problem opening %s for writing.\n\tExiting.",
		  fileName);
      return (ERROR);
    }

  /* Now write file header information -- KEYWORD, VALUE pairs. */
  if ((result = WriteFileHeader (runControl, *filePtr)) != OK)
    {
      PrintError ("Problem writing header info to %s.\n\tExiting.",
		  fileName);
      return (ERROR);
    }

  return (OK);
}

/***********************************************************************/
#define FUNC_NAME "WriteFileHeader"

int WriteFileHeader (RUN_CONTROL runControl, FILE *filePtr)

{
  int result,
      scale;
  char timestep[15],
       unit[50];

  /* Write start and end dates, converted to RiverWare format; note that
     range data and stats data will be written in a format that RiverWare
     currently does not support. */
  if ((result = WriteRiverWareDates (runControl, filePtr)) != OK)
    {
      return (ERROR);
    }

  /* Write time step. */
  switch (runControl.obsDest)
    {
      case BY_INSTANT: 
        sprintf (timestep, "INSTANT");
	break;
      case BY_SECOND: 
        sprintf (timestep, "1SECOND");
	break;
      case BY_MINUTE: 
        sprintf (timestep, "1MINUTE");
	break;
      case BY_HOUR: 
        sprintf (timestep, "1 HOUR");
	break;
      case BY_DAY: 
        sprintf (timestep, "1 DAY");
	break;
      case BY_MONTH: 
        sprintf (timestep, "1MONTH");
	break;
      case BY_YEAR: 
        sprintf (timestep, "1YEAR");
	break;
      case BY_WY: 
        sprintf (timestep, "1WY");
	break;
    }

  fprintf (filePtr, "timestep: %s\n", timestep);

  /* Write units. (And scale, if != 1 ) */
  DmiLookupRiverWareUnitScale (runControl.unitDest, unit, &scale);
  fprintf (filePtr, "units: %s\n", unit);

  if ((scale != NA) && (scale != 1))
    fprintf (filePtr, "scale: %d\n", scale);

  return (OK);
}

/*********************************************************************/
#define FUNC_NAME "WriteRiverWareDates"

int WriteRiverWareDates (RUN_CONTROL runControl, FILE *filePtr)
{
  int result;
  char riverwareDateStart[50],
       riverwareDateStart2[50],
       riverwareDateEnd[50],
       riverwareDateEnd2[50];
  SQL_DATE temp1,
           temp2;

  /* Get and write start date(s). */
  if ((result = GetRiverWareDates (runControl.isStat, runControl.destIsRange,
			       runControl.dateStart, 
			       runControl.dateEndRange, 
			       runControl.obsDest,
			       riverwareDateStart, riverwareDateStart2)) != OK)
    {
      PrintError ("Problem getting corresponding RiverWare dates.\n\tExiting.\n\t");
      return (ERROR);
    }
  fprintf (filePtr, "start_date: %s %s\n", riverwareDateStart, riverwareDateStart2);

  /* Get and write end date(s). If non-range data, pass in dateEnd
     truncated to obsDest; if range data, pass in beg and end range dates
     w/ nRanges years added. */
  if (!runControl.destIsRange)
    {
      if ((result = SqlDateTrunc (runControl.dateEnd, 
				  timeIntervals[runControl.obsDest].ingresName,
				  temp1)) != OK)
	{
	  PrintError ("%s: Exiting.\n", FUNC_NAME);
	  return (ERROR);
	}
    }
  else
    {
      if ((result = SqlDateMath (ADDITION, runControl.dateBegRange, 
				 temp1, runControl.numPer - 1, YEAR)) != OK)
	{
	  PrintError ("%s: Exiting.\n", FUNC_NAME);
	  return (ERROR);
	}

      if ((result = SqlDateMath (ADDITION, runControl.dateEndRange, 
				 temp2, runControl.numPer - 1, YEAR)) != OK)
	{
	  PrintError ("%s: Exiting.\n", FUNC_NAME);
	  return (ERROR);
	}
    }

  /* Statistics on range data does not require an end date, because
     only one value is generated. */
  if (!(runControl.isStat && runControl.destIsRange))
    {
      if ((result = GetRiverWareDates (runControl.isStat, runControl.destIsRange,
				   temp1, temp2, runControl.obsDest,
				   riverwareDateEnd, riverwareDateEnd2)) != OK)
	{
	  PrintError ("Problem getting corresponding RiverWare dates.\n\tExiting.\n\t");
	  return (ERROR);
	}
      fprintf (filePtr, "end_date: %s %s\n", riverwareDateEnd, riverwareDateEnd2);
    }

  return (OK);
}
      
/************************************************************************/
#define FUNC_NAME "GetRiverWareDates"

int GetRiverWareDates (int isStat, int destIsRange, SQL_DATE dateFirst,
		   SQL_DATE dateSecond, TIME_INTERVAL_TYPES obsDest,
		   char *riverwareDateFirst, char *riverwareDateSecond)

{
  int hour,
      result;
  SQL_DATE temp1,
           temp2;
  SQL_INTERVAL timestep;

  strcpy (timestep, timeIntervals[obsDest].ingresName);

  /* Convert SQL dates (e.g., 10-NOV-92) to numeric format (e.g., 11-10-92) */
  if ((result = DmiCharToNumericDate (dateFirst, temp1)) != OK)
    {
      PrintError ("%s: Problem with date %s. Exiting.\n", FUNC_NAME, dateFirst);
      return (ERROR);
    }

  /* Do this conversion only if needed */
  if (destIsRange)
    {
      if ((result = DmiCharToNumericDate (dateSecond, temp2)) != OK)
	{
	  PrintError ("%s: Problem with date %s. Exiting.\n", FUNC_NAME, dateSecond);
	  return (ERROR);
	}
    }

  /* Initialize strings */
  riverwareDateFirst[0] = '\0';
  riverwareDateSecond[0] = '\0';

  if (!isStat)
    {
      /* Possible that hourly date doens't have explicit hour string; E.g.
	 14-jan-1996 is really 14-jan-1996 00:00. Explicitly attach hour
	 string where needed (if it's not there already, it must be
	 a 00:00 hour). */
       /*      if (obsDest == BY_HOUR && !(strstr (temp1, ":"))) */
       if (obsDest == BY_HOUR && !(strstr (dateFirst, ":")))
	{
	   /*	  strcat (temp1, " 00:00"); */
	  strcat (dateFirst, " 00:00");
	}

      if ((result = hdbToRiverWareDate (dateFirst, riverwareDateFirst, timestep))
	  != OK)
	{
	  return (ERROR);
	}

      if (destIsRange)
	{
	  if ((result = hdbToRiverWareDate (dateSecond,
					riverwareDateSecond, timestep)) != OK)
	    {
	      return (ERROR);
	    }
	}
    }
  else
    {
      if (obsDest == BY_HOUR)
	{
	  if ((result = SqlExtractDatePart (dateFirst,
					    HOUR, &hour)) != OK)
	    {
	      return (ERROR);
	    }
	  sprintf (riverwareDateFirst, "%s%d:00", (hour < 10 ? "0" : ""), hour);
	}
      else
	{
	  if ((result = hdbToRiverWareDate (dateFirst,
					riverwareDateFirst, timestep)) != OK)
	    {
	      return (ERROR);
	    }
	  strcpy (riverwareDateFirst, &(riverwareDateFirst[5]));
	}

      /* Range values cannot be by hour; by month only. */
      if (destIsRange)
	{
	  if ((result = hdbToRiverWareDate (dateSecond,
					riverwareDateSecond, timestep)) != OK)
	    {
	      return (ERROR);
	    }
	  strcpy (riverwareDateSecond, &(riverwareDateSecond[5]));
	}
    }
    
  return (OK);
}

/***********************************************************************/
#define FUNC_NAME "WriteAggDisaggFile"

int WriteAggDisaggFile (FILE *filePtr, RUN_CONTROL runControl, 
			VALUE_RECORD *disAggValueList, int numDisAggValues,
			int timeSeriesComplete)

{
  int count;

  /* Write data, NAs if real data wasn't generated. */
  if (timeSeriesComplete)
    {
      if (runControl.format == EXP)
	{
	  for (count = 0; count < numDisAggValues; count++)
	    fprintf (filePtr, "%*.*E\n", runControl.width, 
		     runControl.precision, 
		     disAggValueList[count].value);
	}
      else
	{
	  for (count = 0; count < numDisAggValues; count++)
	    fprintf (filePtr, "%*.*f\n", runControl.width, 
		     runControl.precision, 
		     disAggValueList[count].value);
	}
    }
  else
    {
      for (count = 0; count < numDisAggValues; count++)
	fprintf (filePtr, "NaN\n");
    }

  return (OK);
}


