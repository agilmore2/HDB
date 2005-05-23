/*****************************************************************************
* DESCRIPTION
*    Creates one-page report of Flood Control Criteria. Creates text
*    formatted file as output.
*
* ASSUMPTIONS
*    Site_datatype_ids are hard-coded (see #defines in lc_apps.h).
*
* INPUT
*    model_run_id is entered on the command line.
*
* OUTPUT
*     Text files '24moFld.out'.
*
* RETURN STATUS
*
* COMMENTS
*    The formatting and .sc routines used by this report are also compiled 
*    by the monthly_accum report. The development environment for this report
*    is therefore slightly more complicated than for other reports. 
*
* AUTHOR: Yue Wu Feb 1997
*         Darren Silver June 1997
*         Rene Reitsma Sept. 1997
*         Julien Chastang Nov. 1997
*         Oracle Verion:
*         Julien Chastang Sept. 1998
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"
#include <lc_apps.h>
#include <globals.h>

#define VERSION "1.1"

extern char *month_b[12];
extern char *week[7];

void usage(void)
{
  printf("\nIllegal command options...\n\n");
  printf("Usage: 24moFld [-v] [model_run_id] [-pP precision]\n\n");
  printf("Command line arguments:\n\n");
  printf("[-v]: prints version number. Must be first argument. Additional arguments will be ignored.\n\n");
  printf("[model_run_id]: model_run_id of the run (required unless -v option is specified).\n\n");
  printf("[-pP precision]: Specifies precision (0 < precision < 3) for\n");
  printf("                 the numbers in the report.\n\n");

  return;
}

void main(int argc, char **argv)
{
  time_t clock;
  struct tm *timestruct;
  
  char DateString[50];
  char *  dbName;
  char *  appRole;
  char *  appUser;
  char lcdatabase[10];
  char ucdatabase[10];

  int MonthOfYear;
  int Year;
  int precision;
  int model_run_id;
  
  int success;
  FILE * fp1;

  dbName = NULL;
  dbName = getenv("HDB_LOCAL");
  if (dbName == NULL)
  {
     PrintError("Environment variable HDB_LOCAL not set...\n");
     exit(ERROR);
  }

  appRole = NULL;
  appRole = getenv ("APP_ROLE");

  if (appRole == NULL)
  {
     PrintError("Environment variable APP_ROLE not set...\n");
     exit(ERROR);
  }

  appUser = NULL;
  appUser = getenv ("APP_USER");

  if (appUser == NULL)
  {
     PrintError("Environment variable APP_USER not set...\n");
     exit(ERROR);
  }


  RegisterSignalHandler();

/* Check command options 

  Valid configurations are:

   command -v ?? ?? (2 <= argc <= 4)
   command model_run_id (argc == 2, precision is set to 0)
   command model_run_id -p precision (argc == 4) */

  if (argc == 1 || argc > 4)
  {
    usage();
    exit(ERROR);
  }

/* If the first argument is -v, execute and ignore the rest */

  if(!strcmp(argv[1],"-v"))
  {
    printf("\n%s version %s\n\n",argv[0],VERSION);
    exit(OK);
  }

/* test for <command [model_run_id]> */

  if (argc == 2)
  {
    model_run_id = atoi(argv[1]);
    if (model_run_id == 0)
    {
      printf("Error: Invalid model run id\n");
      usage();
      exit(ERROR);
    }
    else
    {
      printf("Warning: No precision option...\n");
      printf("Generating report with zero precision.\n");
      precision = 0;
    }
  }

  if (argc == 3) /* have already tested for -v so must be wrong */
  {
    usage();
    exit(ERROR);
  }

/* test for <command [model_run_id] [-pP precision]> */

  if (argc == 4)
  {
    model_run_id = atoi(argv[1]);
    if (model_run_id == 0)
    {
      printf("Error: Invalid model run id\n");
      usage();
      exit(ERROR);
    }

    if (strcmp(argv[2], "-P") && strcmp(argv[2], "-p"))
    {
      usage();
      exit(ERROR);
    }

    precision = atoi(argv[3]);
    if (precision == 0)
	{
	  printf("Warning: zero or non-numeric precision.\n");
	  printf("   Generating report with zero precision.\n");
	}

	if (precision > 3 || precision < 0)
	{
	  printf("Error: precision must be larger than 0 and smaller than 3.\n");
          usage();
	  exit(ERROR);
	}
  }
  
  /* Get the time from the operating system. */
  clock = time(NULL);
  timestruct = localtime(&clock);
  
  MonthOfYear = timestruct->tm_mon;
  Year = timestruct->tm_year+CENTURY;
  
  sprintf(DateString, "%s %s %d %d:%d:%d %d\n", week[timestruct->tm_wday],
	  month_b[timestruct->tm_mon],
	  timestruct->tm_mday,
	  timestruct->tm_hour,
	  timestruct->tm_min,
	  timestruct->tm_sec,
	  timestruct->tm_year+CENTURY);
  
  /*
   * Connect to UC and LC as two different sessions 
   * Note: this could be reworkd so the db names and 
   * session ids are defines 
   * Also Note: Must try to connect to UC FIRST,
   * b/c if it fails, would kill open LC session 
   *
   * The global variable 'ConnectedToUC' is initialized to zero. 
   */

   if ((success = SqlConnectRole (dbName, appUser, dbName, appRole)) == ERROR)
   {
      PrintError("Failed to connect to local database... exiting\n");
      exit (ERROR);
   }

   success = SqlGetDBName("LC",lcdatabase);
   if (success != OK)
   {
       PrintError("Failed to get name of LC database... exiting\n");
       SqlDisconnect();
       exit (ERROR);
   }

   success = SqlGetDBName("UC",ucdatabase);
   if (success != OK)
   {
       PrintError("Failed to get name of UC database... exiting\n");
       SqlDisconnect();
       exit (ERROR);
   }

   SqlDisconnect();

  if ((success = SqlConnectRoleAt ("LC",lcdatabase, appUser, lcdatabase, appRole)) == ERROR)
  {
     PrintError("Failed to connect to LC database... exiting\n");
     exit (ERROR);
  }

  PrintError("Connected to LC database\n");

  if ((success = SqlConnectRoleAt ("UC",ucdatabase, appUser, ucdatabase, appRole)) == ERROR)
  {
     PrintError("Failed to connect to UC database.\n");
     ConnectedToUC = ERROR;
  }

  ConnectedToUC = OK;
  PrintError("Connected to UC database\n");

  /* Validate user entered model_run_id*/
  if ((success = SqlValidModelRunId(model_run_id)) != OK)
  {
     PrintError("\tConsult the ref_model_run table for valid model_run_id.\n");
     SqlDisconnectAll();
     exit(ERROR);
  }
  
  if ((success = SqlFetchDateByModelRunId(model_run_id, 
					  &MonthOfYear, &Year)) != OK)
  {
     PrintError("Error in fetching the run date of 24 month study!\n");
     SqlDisconnectAll();
     exit(ERROR);
  } 
  
  /* Open output file */
  fp1 = fopen("24moFld.out", "w");
  if (!fp1)
  {
     printf("Error opening output file 24moFld.out\n");
     SqlDisconnectAll();
     exit(ERROR);
  }
  
  /* 
   * write the control characters for the HP Laser Jet 4 printer
   * first reset to defaults to get Courier
   * then change to landscape
   * then change pitch size
   * and follow with a newline 
   
   * fprintf(fp1,"\x1B");
   * fprintf(fp1,"e");
   * fprintf(fp1,"\x1B");
   * fprintf(fp1,"&a90p");
   * fprintf(fp1,"\x1B");
   * fprintf(fp1,"(s16.7H");
   * fprintf(fp1,"\n");
   */
  
  /*
   *This is the function call that creates the report.
   *It is located in: 
   *HDB_applications/appCode/reports/24moFld/src/FormatOutput.c
   */
  if( Create24moFldReport(fp1, DateString, MonthOfYear, Year, model_run_id, precision) != OK) 
  {
       printf("Error Creating 24-month Flood Control Report\n");
  }
  else 
     printf("Created 24-month Flood Control Report successfully (24moFld.out)\n");
  
  /*
   * reset the printer by adding these control characters to the output file 
   *fprintf(fp1,"\x1B");
   *fprintf(fp1,"E");
   */
  
  /* close the text files */
  fclose(fp1);
  
  success = SqlCommit();

  SqlDisconnectAll();
  
  exit (OK);
}
