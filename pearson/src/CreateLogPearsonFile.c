/* Application to create an input file for a program which generates 
   log-pearson distributions. Generates data for daily or monthly, real or
   modeled, depending on command line flags. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dbutils.h"
#include "typedefs.h"
#include "utils.h"
#include "defines.h"
#include "pearson.h"

#define VERSION "HDB2 1.2"

int main ( int argc, char** argv )
{	
   char  	 *dbName, 
                 *appRole;
   char          bdate[26],
                 edate[26],
                 bdate_aj[26],
                 edate_aj[26];
   char          site_name[65],
                 datatype_name[65],
                 unit_name[33],
                 model_run_name[65];
   int           site_id,
                 datatype_id,
                 unit_id,
                 site_datatype_id,
                 model_run_id;
   
   int           result;
   int	         beg_year,
                 num_years,
                 start_year;
   TIME_INTERVAL_TYPES timestep;
   
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

   if (argc != 9)
   {
      printf("\nusage: createLogPearsonFile <oracleUser> <psswd> d || m <site_id> <datatype_id> <model_run_id> || 0 <begin_year> <number_of years>\n");
      printf("for real data use 0 instead of model_run_id\n");
      exit(ERROR);
   }

   /* Copy command line arguments into variables. */
   if (!strcmp (argv[3], "d"))
       timestep = BY_DAY;
   else if (!strcmp (argv[3], "m"))
      timestep = BY_MONTH;
   else
      PrintError ("Timestep %s unknown. Exiting.\n", argv[3]);
   site_id = atoi(argv[4]);
   datatype_id = atoi(argv[5]);
   model_run_id = atoi(argv[6]);
   beg_year = atoi(argv[7]);
   num_years = atoi(argv[8]);
   
   if(beg_year < 1800)
   {
      printf("\nERROR!  Year must be input fully i.e. 1995 not 95\n\nExiting!\n");
      exit(ERROR);
   }

   RegisterSignalHandler();
   
   if ((result = SqlConnectRole (dbName, argv[1], argv[2], appRole)) == ERROR)
   {
      exit (ERROR);
   } 

   /* Set the date format */
   if ((result = SqlSetDateFormat ("DD-MON-YYYY HH24:MI:SS")) != OK)
   {
      SqlDisconnect ();
      exit (ERROR);
   }
      
   /* get the datatype description and units for it */
   if ((result = SqlSelectDatatypeInfo (datatype_id, datatype_name,
					&unit_id)) != OK)
   {
      SqlDisconnect ();
      exit (ERROR);
   }

   if ((result = SqlSelectUnitName (unit_id, unit_name)) != OK)
   {
      SqlDisconnect ();
      exit (ERROR);
   }

   /* get the site name */
   if ((result = SqlSelectSiteName (site_id, site_name)) != OK)
   {
      SqlDisconnect ();
      exit (ERROR);
   }
   
   if(model_run_id != 0)
   {
      /* get the model run name */
      if ((result = SqlSelectModelRunName (model_run_id, model_run_name)) 
	  != OK)
      {
	 SqlDisconnect ();
	 exit (ERROR);
      }
   }

   if ((result = SqlGetSiteDatatypeIdWithId (site_id, datatype_id, 
					     &site_datatype_id)) != OK)
   {
      SqlDisconnect ();
      exit (ERROR);
   }


   /* Calculate begin and end dates for daily or monthly timestep */
   start_year = beg_year -1;
   sprintf(bdate,"01-oct-%4d 00:00:00",start_year);

   if (timestep == BY_DAY)
   {
      sprintf(edate,"30-sep-%4d 00:00:00",beg_year);
      sprintf(bdate_aj,"01-apr-%4d 00:00:00",beg_year);
      sprintf(edate_aj,"31-jul-%4d 00:00:00",beg_year);
   }
   /* Monthly; calculate end date only */
   else
   {
      SqlDateMath(ADDITION,bdate,edate,num_years,YEAR);
      SqlDateMath(SUBTRACTION,edate,edate,1,MONTH);
   }

   if (timestep == BY_DAY)
   {
      if ((result = SqlProcessDailyValues (bdate, edate, bdate_aj, edate_aj,
					   site_datatype_id, model_run_id,
					   site_name, datatype_name, unit_name,
					   model_run_name, num_years,
					   beg_year)) != OK)
      {
	 SqlDisconnect ();
	 exit (ERROR);
      }
   }
   /* BY_MONTH */
   else 
   {
      if ((result = SqlProcessMonthlyValues (bdate, edate, site_datatype_id, 
					     model_run_id, site_name, 
					     datatype_name, unit_name,
					     model_run_name, num_years, 
					     beg_year)) != OK)
      {
	 SqlDisconnect ();
	 exit (ERROR);
      }
   }

   SqlDisconnect();

   exit (OK);
}

