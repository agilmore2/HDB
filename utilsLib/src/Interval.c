/*****************************************************************************
* DESCRIPTION
* This file contains functions which get information applying to HDB
* date intervals.
*
* AUTHOR 
* Carol Marra
* Dec 1996
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <defines.h>
#include <typedefs.h>
#include <dbutils.h>
#include <utils.h>


#define FUNC_NAME "GetNumSecondsInYear"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetNumSecondsInYear

<P> <STRONG>Function signature:</STRONG> int GetNumSecondsInYear (SQL_DATE, long *, TIME_INTERVAL_TYPES)

<P> <STRONG>Function description:</STRONG> Get number of seconds in year indicated by date, accounting for BY_YEAR vs. BY_WY interval difference.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE theDate:             The date (dd-mon-yyyy hh24:mi:ss)
<BR><DD> long *seconds:                Number of seconds in year, calculated.
<BR><DD> TIME_INTERVAL_TYPES interval: The appropriate interval (BY_YEAR or BY_WY)
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int GetNumSecondsInYear (SQL_DATE theDate, long *seconds, 
			 TIME_INTERVAL_TYPES interval)

{
  int month,
      result;
  SQL_DATE tempDate;

  /* Do easy case first; otherwise, figure out which year to look for
     the leap year in... */
  if (interval == BY_YEAR)
    {
      if (!IsLeapYear (theDate))
	*seconds = YEAR_365_DAY;
      else
	*seconds = YEAR_365_DAY + SECONDS_IN_DAY;
      
      return (OK);
    }
  
  if ((result = SqlExtractDatePart (theDate, MONTH, &month)) != OK)
    return (ERROR);

  /* Otherwise, this is a WY interval. Increase the date by one year if in 
     Oct, Nov, Dec, so leap year check will be in correct year. */
  if (month >= 10)
    {
      if (SqlDateMath (ADDITION, theDate, tempDate, 1, YEAR) != OK)
	{
	  PrintError ("Problem getting %s + 1 year in %s\n\tExiting.\n",
		      theDate, FUNC_NAME);
	  return (ERROR);
	}
    }
  else
    strcpy (tempDate, theDate);

  if (!IsLeapYear (tempDate))
    *seconds = YEAR_365_DAY;
  else
    *seconds = YEAR_365_DAY + SECONDS_IN_DAY;
  
  return(OK);
}


/*****************************************************************************/
#define FUNC_NAME "GetNumSecondsInInterval"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetNumSecondsInInterval

<P> <STRONG>Function signature:</STRONG> int GetNumSecondsInInterval (TIME_INTERVAL_TYPES, SQL_DATE, long *)

<P> <STRONG>Function description:</STRONG> Get the number of seconds in the indicated interval for the specified date.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> TIME_INTERVAL_TYPES interval: The interval (e.g., BY_DAY).
<BR><DD> SQL_DATE fromDate:            The date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> long *seconds:                Number of seconds in interval, calculated.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int GetNumSecondsInInterval (TIME_INTERVAL_TYPES interval, SQL_DATE fromDate,
			     long *seconds)

{
  int result;
  
  switch (interval)
    {
      case BY_SECOND:
        *seconds = 1;
        break;

      case BY_MINUTE:
        *seconds = 60;
        break;

      case BY_HOUR:
        *seconds = 3600;
        break;

      case BY_DAY:
        *seconds = 86400;
        break;

      case BY_MONTH:
        if ((result = SqlGetNumSecondsInMonth (fromDate, seconds)) != OK)
	  {
	    PrintError ("Error getting number of seconds in month for %s.\n\tExiting", fromDate);
	    return (ERROR);
	  }
	break;

      case BY_YEAR:
      case BY_WY:
	if ((result = GetNumSecondsInYear (fromDate, seconds, interval)) != OK)
	  {
	    PrintError ("Error getting number of seconds in year for %s.\n\tExiting", fromDate);
	    return (ERROR);
	  }
        break;

      default:
	PrintError ("Error getting seconds; %d is not a valid interval.\n\tExiting", interval);
	return (ERROR);
        break;
      }

  return (OK);
}

