/*****************************************************************************
* DESCRIPTION
* This file contains functions which return information about the
* HDB schema (eg column names, etc).
*
* AUTHOR - EDITORS
* Carol Marra
*****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <defines.h> 
#include <typedefs.h>
#include <dbutils.h>
#include <utils.h>

#define FUNC_NAME	"GetDateColumn"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetDateColumn

<P> <STRONG>Function signature:</STRONG> int GetDateColumn (TIME_INTERVAL_TYPES, int, int, OBJECTTYPE_NAME)


<P> <STRONG>Function description:</STRONG> Return name of date column for time series data, depending on interval of data, and if data is statistics or not. User must know that range data requires 2 date columns, and prepend "start" and "end" to the returned column name.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> TIME_INTERVAL_TYPES interval: The time interval of the table, e.g., BY_DAY, BY_MONTH, etc.
<BR><DD> int isStat:                   0 or 1 indicator of whether or not data is statistics.
<BR><DD> int isRange:                   0 or 1 indicator of whether or not data is range data.
<BR><DD> OBJECTTYPE_NAME dateColumn:   Name of date column for this data, returned.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int GetDateColumn (TIME_INTERVAL_TYPES interval, int isStat, int isRange,
 		   OBJECTTYPE_NAME dateColumn)

{
   /* If neither stat or range, date column does not depend on interval */
  if (!isStat && !isRange)
     strcpy (dateColumn, DATE_TIME);
  /* If not stat, but range, use intervals */
  else if (!isStat)
    switch (interval)
      {
        case BY_INSTANT: 
	  strcpy (dateColumn, DATE_INSTANT);
	  break;
	case BY_HOUR: 
	  strcpy (dateColumn, DATE_HOUR);
	  break;
	case BY_DAY: 
	  strcpy (dateColumn, DATE_DAY);
	  break;
	case BY_MONTH: 
	  strcpy (dateColumn, DATE_MONTH);
	  break;
	case BY_YEAR: 
	  strcpy (dateColumn, DATE_YEAR);
	  break;
	case BY_WY: 
	  strcpy (dateColumn, DATE_WY);
	  break;
	default:
	  PrintError ("You shouldn't be here. Your data interval is invalid.\n\tExiting.");
	  return (ERROR);
	}

  /* If stat, use indexes */
  else
    switch (interval)
      {
	case BY_HOUR: 
	  strcpy (dateColumn, DATE_HOURSTAT);
	  break;
	case BY_DAY: 
	  strcpy (dateColumn, DATE_DAYSTAT);
	  break;
	case BY_MONTH: 
	  strcpy (dateColumn, DATE_MONTHSTAT);
	  break;
	case BY_YEAR: 
	  strcpy (dateColumn, DATE_YEARSTAT);
	  break;
	case BY_WY: 
	  strcpy (dateColumn, DATE_WYSTAT);
	  break;
	default:
	  PrintError ("You shouldn't be here. Your data interval is invalid.\n\tExiting.");
	  return (ERROR);
	}

  return (OK);
}
