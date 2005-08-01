/*****************************************************************************
* DESCRIPTION
*
* This application allows privileged HDB users to automatically derive
* new or updated records coming into the r_base table into the r_interval
* tables.
* 
* AUTHOR 
* Neil Wilson
* September, 2001
*****************************************************************************/
#define VERSION "1.2"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"
#include "derivation.h"
#include "derivationTypedefs.h"

#define FUNC_NAME "main"

FILE* LOGFILE;
SQL_DATE DATETIMELOADED;

int main (int argc, char **argv)
{
   int baseUpdateTotal = 0;
   int result;
   char* derivationRole;
   char* dbName;
   char* derivationLogPath;
   char* hdbEnv;
   char* sdilist = NULL;
   char logName[300];
   double elapsedTime;
   SQL_DATE endTime;

   /* Check arguments */
   if ((argv[1] != NULL) && (!strcmp(argv[1], "-v")))
   {
      printf("\nDerivation version number: %s\n\n", VERSION);
      exit (OK);
   }

   if (argc < 3 || argc > 4)
   {
      PrintError("Usage: derivation <-v> | <user name> <password> [sdilist]\n");
      exit (ERROR);
   }

   /* Set up log file */
   derivationLogPath = NULL;
   derivationLogPath = getenv("DERIVATION_LOG_PATH");
   if (derivationLogPath == NULL)
   {
      hdbEnv = NULL;
      hdbEnv = getenv("HDB_ENV");
      if (hdbEnv == NULL)
      {
         PrintError("At least one of the environment variables HDB_ENV or DERIVATION_LOG_PATH must be set...\n");
         exit (ERROR);
      }
      else
      {
         sprintf(logName, "%s%s", hdbEnv, "/log/derivation.log");
      }
   }
   else
   {
      sprintf(logName, "%s%s", derivationLogPath, "/derivation.log");
   }

   LOGFILE = fopen(logName,"a");
   if (LOGFILE == NULL)
   {
      PrintError("Can't open log file %s.\n", logName);
      exit (ERROR);
   }


   /* Set the name of the database */
   dbName = NULL;
   dbName = getenv("HDB_LOCAL");

   if (dbName == NULL)
   {
      FilePrintError(LOGFILE, "Environment variable HDB_LOCAL not set...\n");
      exit (ERROR);
   }

   /* Set the database role */   
   derivationRole = NULL;
   derivationRole = getenv ("DERIVATION_ROLE");

   if (derivationRole == NULL)
   {
      FilePrintError(LOGFILE, "Environment variable DERIVATION_ROLE not set...\n");
      exit(ERROR);
   }

   /* Set error handling */
   RegisterSignalHandler();

   /* Connect to database */   
   if ((result = SqlConnectRole(dbName, argv[1], argv[2],
                                derivationRole)) == ERROR)
   {
      exit (ERROR);

   }

   /* Handle the sdi list if it exists */
   if (argc == 4)
   {
      if ((result = SqlVerifySDIList(argv[3])) == ERROR)
      {
         SqlDisconnect();
         exit (ERROR);
      }
      sdilist = argv[3];
   }

   /* Create the r_interval_update table which acts as "traffic cop" to
      ensure that only one copy of the derivation application is running on a
      database */
   if ((result = SqlCreateTable()) != OK)
   {
      FilePrintError(LOGFILE, "Derivation application is either already running\n"
		     "the %s database or crashed and did not properly drop\n"
		     "the r_interval_update table.  If the program crashed, the\n"
		     "r_interval_update table under the app_user's schema must be\n"
		     "dropped before the derivation application will rerun.\n",
                  argv[2]);
      SqlDisconnect();
      exit (ERROR);            
   }

   /* Define date_time_loaded for this run of the derivation app -
      will be assigned to all values written to the r_interval tables */
   if ((result = SqlGetDateTime (DATETIMELOADED))
                 != OK)
   {
      FilePrintError(LOGFILE, "Problem setting dateTimeLoaded to current time\n");
      SqlDropTable();
      SqlDisconnect();
      exit (ERROR);            
   }

   /* Write app startup info to log file */
   fprintf(LOGFILE, "\n\nDERIVATION APPLICATION RUN STARTED ON %s FOR TIME %s\n",
           argv[2],
           DATETIMELOADED);

   /* Call to process any changed overwrite values */
   if ((result = SqlCopyOverwrite(&baseUpdateTotal, sdilist)) != OK)
   {
      SqlDropTable();
      SqlDisconnect();
      exit (ERROR);
   }     

   /* Call to SqlDerivationControl to identify and perform non overwrite 
      derivations */
   if ((result = SqlDerivationControl(&baseUpdateTotal, sdilist)) != OK)
   {
      SqlDropTable();
      SqlDisconnect();
      exit (ERROR);
   }

   /* Since processing is finished, delete the previously tagged overwrite
      values from r_base_update */
   if ((result = SqlDeleteOverwrite(sdilist)) != OK)
   {
      SqlDropTable();
      SqlDisconnect();
      exit (ERROR);
   }

   /* Delete any residual values in r_base_update that are over one year
      old - this cleans up for any saved SDIs that no longer have data
      coming in */
   if ((result = SqlRemoveOldUpdates()) != OK)
   {
      SqlDropTable();
      SqlDisconnect();
      exit (ERROR);
   }   

   /* Drop the r_interval_update table to permit starting of the next run 
      of the derivation application (traffic cop) */
   if ((result = SqlDropTable()) != OK)
   {
      SqlDisconnect();
      exit (ERROR);
   }

   /* Find elapsed time of run */
   if ((result = SqlGetDateTime (endTime))
                 != OK)
   {
      FilePrintError(LOGFILE, "Problem setting endTime to current time\n");
      SqlDisconnect();
      exit (ERROR);            
   }
   if ((result = SqlDateSubtract (endTime, DATETIMELOADED, &elapsedTime))
                 != OK)
   {
      FilePrintError(LOGFILE, "Problem calculating elapsed time\n");
      SqlDisconnect();
      exit (ERROR);            
   }

   SqlDisconnect();

   /* Write successful completion message to log file */
   fprintf(LOGFILE, "DERIVATION APPLICATION RUN ON %s FOR TIME %s IS SUCCESSFULLY COMPLETED. %d RECORDS PROCESSED FROM R_BASE_UPDATE IN  ELAPSED TIME OF %f MINUTES\n", argv[2], DATETIMELOADED, baseUpdateTotal, (elapsedTime * 1440));
    
   return (OK);
}
