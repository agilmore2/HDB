/* Application to create a report of flow durations, for either observed or
   modeled daily data. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dbutils.h"
#include "typedefs.h"
#include "utils.h"
#include "defines.h"
#include "flowDuration.h"

#define VERSION "HDB2 1.0"
#define BIG 20000
#define NATSIZE 1200

int main ( int argc, char** argv )
{	
   char  	 *dbName, 
                 *appRole;
   int           siteId,
                 datatypeId,
                 modelRunId;
   SQL_DATE      begDate,
                 endDate;
   int           result,
                 getReal;
   
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
   
   /* If the first argument is -v, execute and ignore the rest */
   if (argc > 1)
      if(!strcmp(argv[1],"-v"))
      {
	 printf("\n%s version %s\n\n",argv[0],VERSION);
	 exit(ERROR);
      }

   if (argc != 8 && argc != 9)
   {
      PrintError ("\nusage: flowDuration <oracleUser> <psswd> <siteId> <datatypeId> <begDate> <endDate> r || m [<model_run_id>]\n");
      exit(ERROR);
   }

   /* Copy command line arguments into variables. */
   siteId = atoi (argv[3]);
   datatypeId = atoi (argv[4]);
   
   if (!strcmp (argv[7], "r") || !strcmp (argv[7], "R"))
      getReal = TRUE;
   else
      getReal = FALSE;

   if (!getReal)
   {
      if (argc != 9)
      {
	 PrintError ("Model_run_id required for model data.\n");
	 PrintError ("\nusage: flowDuration <oracleUser> <psswd> <siteId> <datatypeId> <begDate> <endDate> r || m [<model_run_id>]\n");
	 exit(ERROR);
      }
      else
	 modelRunId = atoi (argv[8]);
   }
   else
      	 modelRunId = NA;

   RegisterSignalHandler();
   
   if ((result = SqlConnectRole (dbName, argv[1], argv[2], appRole)) == ERROR)
   {
      exit (ERROR);
   } 


   if ((result = SqlFormatDate (argv[5], begDate)) != OK)
   {
      PrintError ("Problem formatting begDate: %s\n Exiting.", begDate);
      exit (ERROR);
   }

   if ((result = SqlSetDateFormat ("dd-mon-yyyy hh24:mi:ss")) != OK)
   {
      PrintError ("Problem setting date format.\n Exiting.");
      exit (ERROR);
   }

   if ((result = SqlFormatDate (argv[6], endDate)) != OK)
   {
      PrintError ("Problem formatting endDate: %s\n Exiting.", endDate);
      exit (ERROR);
   }

   if ((result = SqlGetFlowDuration (siteId, datatypeId, begDate, endDate,
				     getReal, modelRunId)) != OK)
   {
      PrintError ("Problem getting flow duration data.\n\t Exiting.");
      exit (ERROR);
   }

   SqlDisconnect ();
   exit (OK);
}
