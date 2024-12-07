/*****************************************************************************
* DESCRIPTION
* This file contains functions which calculate information about dates.
*
* AUTHOR 
* Carol Marra
* March 1995
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <defines.h>
#include <typedefs.h>
#include <dbutils.h>
#include <utils.h>

#define FUNC_NAME "GetWaterYear"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetWaterYear

<P> <STRONG>Function signature:</STRONG> int GetWaterYear (SQL_DATE, int *)

<P> <STRONG>Function description:</STRONG> Get the water year associated with the indicated date.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE date:  The date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> int *waterYear: The water year for the date, returned.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/


int GetWaterYear (SQL_DATE date, int *waterYear)
{
  int result,
      month;

  /* pull the month and year from the date */
  if ((result = SqlExtractDatePart (date, MONTH, &month)) == ERROR)
    {
      return (ERROR);
    }

  if ((result = SqlExtractDatePart (date, YEAR, waterYear)) == ERROR)
    {
      return (ERROR);
    }
   
  /* determine the water year which corresponds with date */
  if (month > 9)
    (*waterYear)++;

  return (OK);
}

/***************************************************************************/ 
#define FUNC_NAME "IsFirstDayWaterYear"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> IsFirstDayWaterYear

<P> <STRONG>Function signature:</STRONG> int IsFirstDayWaterYear (SQL_DATE, int)

<P> <STRONG>Function description:</STRONG> Determines if the date passed in is the first day of the indicated water year.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE date: The date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> int waterYear: The 4-digit water year.
</DL>

<P> <STRONG>Function return value:</STRONG> TRUE or FALSE (1 or 0)

</HTML>
*/

int IsFirstDayWaterYear (SQL_DATE date, int waterYear)
{
  char wyFirstDay[12];

  /* construct the first day of the water year */
  sprintf (wyFirstDay, "01-oct-%d", waterYear - 1);

#ifdef DEBUG
  fprintf (stdout, "FIRST DAY: %s\n", wyFirstDay);
#endif

  if (strncmp (date, wyFirstDay, 11))
    return (FALSE);
  else 
    return (TRUE);
}
 
/***************************************************************************/ 
#define FUNC_NAME "GetFirstDayWaterYear"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetFirstDayWaterYear

<P> <STRONG>Function signature:</STRONG> int GetFirstDayWaterYear (int, SQL_DATE)

<P> <STRONG>Function description:</STRONG> For the supplied water year, constructs the water year's first day.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> int waterYear: The 4-digit water year.
<BR><DD> SQL_DATE wyFirstDay: The water year's first day, constructed (dd-mon-yyyy hh24:mi:ss).
</DL>

<P> <STRONG>Function return value:</STRONG> OK

</HTML>
*/

int GetFirstDayWaterYear (int waterYear, SQL_DATE wyFirstDay)
{
  /* construct the first day of the water year */
  sprintf (wyFirstDay, "01-oct-%d 00:00:00", waterYear - 1);

  return (OK);
}

/*****************************************************************************/
#define FUNC_NAME  "GetDayOfYear"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetDayOfYear

<P> <STRONG>Function signature:</STRONG> int GetDayOfYear (SQL_DATE, int *)

<P> <STRONG>Function description:</STRONG> For the supplied date, gets the day of the year. For instance, for March 15, day of year is 74 in non-leap year.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE sDate: The date for which day of year is desired (dd-mon-yyyy hh24:mi:ss)
<BR><DD> int *dayOfYear: The day of year, calculated.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int GetDayOfYear (SQL_DATE sDate, int *dayOfYear)
{
  int result;
  SQL_DATE jan01;

  if ((result = SqlDateTrunc (sDate, YEAR, jan01)) != OK)
    {
      PrintError ("%s: Problem getting first day of year for %s.\n\tExiting.",
		  FUNC_NAME, sDate);
      return (ERROR);
    }

  if ((result = SqlNumDaysBetweenDates (jan01, sDate, dayOfYear)) != OK)
    {
      PrintError ("%s: Problem getting day of year for %s.\n\tExiting.",
		  FUNC_NAME, sDate);
      return (ERROR);
    }

  return(OK);
}

/***************************************************************************/ 
#define FUNC_NAME "IsLeapYear"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> IsLeapYear

<P> <STRONG>Function signature:</STRONG> int IsLeapYear (SQL_DATE)

<P> <STRONG>Function description:</STRONG> Determines if the given date is in a leap year.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE date: The date (dd-mon-yyyy hh24:mi:ss).
</DL>

<P> <STRONG>Function return value:</STRONG> TRUE or FALSE (1 or 0)

</HTML>
*/
int IsLeapYear (SQL_DATE date)
{
  int year,
      result;
  
  if ((result = SqlExtractDatePart (date, YEAR, &year)) != OK)
    return (ERROR);
  
  if (! (year % 4) && ((year % 100) || ! (year % 400)))
    return (TRUE);
  else
    return (FALSE);
  
}
 
/***************************************************************************/
#define FUNC_NAME "CalcNumLeapSeconds"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> CalcNumLeapSeconds

<P> <STRONG>Function signature:</STRONG> int CalcNumLeapSeconds (SQL_DATE, SQL_DATE, float *)

<P> <STRONG>Function description:</STRONG> Determines the number of seconds between the two dates which result from leap days. For instance, if there's one leap day between the two dates, nLeapSeconds is 86400.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE dateStart:  First date in range (dd-mon-yyyy hh24:mi:ss).
<BR><DD> SQL_DATE dateEnd:    Last date in range (dd-mon-yyyy hh24:mi:ss).
<BR><DD> float* nLeapSeconds: Number of leap seconds, calculated.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int CalcNumLeapSeconds (SQL_DATE dateStart, SQL_DATE dateEnd, 
			float *nLeapSeconds)

{
  int result,
      year,
      yearStart,
      yearEnd,
      count,
      totalLeapDays = 0; /* Do most of the calculation by days, then
			    multiply to get number of seconds. */
  

  /* Extract start and end years; loop for each one, doing leap
     year calculation. Don't call existing function because this
     is faster. */
  if ((result = SqlExtractDatePart (dateStart, YEAR, &yearStart)) != OK)
    return (ERROR);

  if ((result = SqlExtractDatePart (dateEnd, YEAR, &yearEnd)) != OK)
    return (ERROR);
  
  for (year = yearStart, count = 0; year <= yearEnd; year++, count++)
    if (! (year % 4) && ((year % 100) || ! (year % 400)))
      totalLeapDays++;

  *nLeapSeconds = ((float) totalLeapDays / (float) count) * SECONDS_IN_DAY;

  return (OK);
}





