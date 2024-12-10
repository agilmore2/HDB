/*****************************************************************************
* DESCRIPTION
* This function takes the date string produced by SNOTEL log files, parses it,
* and sets the string passed in to a valid Oracle date.
*
* ASSUMPTIONS 
*
* INPUT
* char *      - date string
* char *      - ingres date
*
* OUTPUT
* warnings and error messages
*
* RETURN
* OK
*
*
* COMMENTS
*
* AUTHOR - EDITORS  
* Carol Marra
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "basins.h"

#define FUNC_NAME "parseDate"
#define NUM_COLUMNS 2
#define DATE_LEN 20
#define MON_LEN 25
#define DAY_LEN 11
#define YR_LEN 8

int parseDate (char *dateString, char *oracleDate)

{
  char *tempDate,
       tempDate1[DATE_LEN],
       tempDate2[DATE_LEN],
       day[DAY_LEN],
       month[MON_LEN],
       year[YR_LEN];
  short i;


  /* Get the month */
  tempDate = strstr(dateString, ":");
  if (tempDate == NULL) {
    fprintf (stdout, "Error finding : in date %s.\n", dateString);
    exit (ERROR);
  }
  strncpy (tempDate2, &(tempDate[2]), DATE_LEN);
  strncpy (month, tempDate2, MON_LEN);

  i = 0;
  while (month[i] != ' ')
    i++;
  month[i] = '\0';
  strncpy (tempDate1, &(tempDate2[i + 1]), DATE_LEN); 


  /* Get the day */
  strncpy (day, tempDate1, DAY_LEN);

  i = 0;
  while (day[i] != ' ')
    i++;
  day[i] = '\0';
  strncpy (tempDate2, &(tempDate1[i + 3]), DATE_LEN); 


  /* Get the year */
  strncpy (year, tempDate2, YR_LEN);

  i = 0;
  while (year[i] != '\n' && year[i] != ' ')
    i++;
  year[i] = '\0';

  /* construct the Oracle date */
  snprintf (oracleDate, SQL_DATE_LENGTH, "%s-%s-%s", day, month, year);

  return (OK);
}



