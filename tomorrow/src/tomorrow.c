/* $Id$ */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include <defines.h>
#include <utils.h>

#define ONEDAY  24*60*60    /* One day in seconds */
#define VERSION "1.2"

int main(int argc, char *argv[])
{
  char date_string[11];
  int day,month,year;

  time_t the_time;
  struct tm my_tm, *outtime;

  if (argc != 2)
  {
    PrintError("Improper arguments to tomorrow...\n");
    PrintError("syntax: tomorrow <DD-MM-YYYY> || -v\n");
    exit(ERROR);
  }

  if (strcmp(argv[1],"-v") == 0)
  {
    printf("tomorrow version %s\n",VERSION);
    exit(OK);
  }
  strcpy(date_string,argv[1]);
  day = atoi(date_string);
  month = atoi(&(date_string[3]));
  year = atoi(&(date_string[6]));

  if (day == 0 || month == 0 || year == 0 || year < 1900)
  {
    PrintError("Error...improper date format; use <DD-MM-YYYY>\n");
    exit(ERROR);
  }

  my_tm.tm_mday = day;
  my_tm.tm_mon = month - 1;
  my_tm.tm_year = year - 1900;
  my_tm.tm_sec = 0;
  my_tm.tm_min = 0;
  my_tm.tm_hour = 11;  /* set to noon to prevent daylight savings problems */

  the_time = mktime(&my_tm);
  the_time = the_time + ONEDAY;
  outtime = localtime(&the_time);

  strftime(date_string,11,"%d-%m-%Y",outtime);
  printf("%s\n",date_string);

  exit(OK);
}

