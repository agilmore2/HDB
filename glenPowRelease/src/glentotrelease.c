/*************************************************************************************************************
 *  Function:  retrieve data from HDB2 and create a continuous GC power release
 *   and calculate GC total release
 *
 *  Author:    Andrew Gilmore
 *
 *
 *  Purpose:   1. retrieve data from HDB2
 *             2. check continuity of AVM data
 *             3. fill in holes, or NOT?
 *             4. write GC power release
 *             5. compute GC total release
 *             6. write GC total release
 *
 *
 **************************************************************************************************************/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stropts.h>
#include <poll.h>
#include <errno.h>
#include <sys/termios.h>
#include <ctype.h>
#include "typedefs.h"
#include "utils.h"
#include "defines.h"
#include "glenpowrelease.h"

double pow_rel[HOURS], spill[HOURS], bypass[HOURS];

int main(int argc, char **argv)
{
   char   date[30];
   char   hour[6];
   int    i,j, missing_data=0;
   char   val_flag[] = "Z";
   
   double  tot_rel[HOURS]; 

   char *formattedDates[HOURS];

   char  newDateFormat[] = "RRMONDD HH24\0";
   char   *dbName, *appRole;
   int    result;

   if (argc != 4)
    {
      printf("\nusage: glenTotRelease <user name> <password> YYMONDD\n\n");
      exit(2);
    }

   printf ("%s ",argv[3]);
   
/* set the name of the database */

   dbName = NULL; 
   dbName = getenv("HDB_LOCAL");
  
   if (dbName == NULL)
   {
      PrintError("Environment variable HDB_LOCAL not set...\n");
      return(ERROR);
   }
   /* get the appRole information */

  appRole = NULL;
  appRole = getenv ("APP_ROLE");

  if (appRole == NULL)
    {
      PrintError("Environment variable APP_ROLE not set...\n");
      return(ERROR);
    }

  /* run  the signal handler function which will disconnect from the
     database and free up tables if things go wierd */

  RegisterSignalHandler();
  
 
   if  ((result = SqlConnectRole (dbName, argv[1], argv[2], appRole)) == ERROR)
      exit (ERROR);
   
   printf("Connected to ORACLE.\n");

   if ((result = SqlSetDateFormat (newDateFormat)) != OK)
   {
      PrintError ("Problem setting default date format.\n\tExiting.");
      SqlDisconnect();
      exit(1);
   }
   
   for (i=0; i<HOURS; i++)
   {
      date[0] = '\0';
      strncat(date,argv[3],11);
      snprintf(hour,6," %d",i);
      strncat(date,hour,6);

      formattedDates[i] = strndup(date,18);
   }

   if (argc != 4) 
   {
     printf("Usage: <programname> app_user app_passwd YYMONDD\n");
     exit(1);
   }

   
   SqlGetTotData(argv[3]);
   /*
     Now, we must determine where the holes in the data are, and
     whether or not to fill in the AVM data with the SCADA data

     for now, assume avm data is fine
   */

   /* calculate Total Release*/
   for (j=0; j<HOURS; j++)
   {
      tot_rel[j] = spill[j] + bypass[j] + pow_rel[j];
   }

   /* Now, write data to database. Perhaps we should allow the new
      power release data to run through the derivation application
      before calculating total releases based on it?
   */
   /*
   for (j=0; j<HOURS; j++)
   {
      printf("%s %d: %s %.0f %s %.0f %s %.0f %s %.0f %s %.0f\n", date, j,
             "curve", curve_rel[j],"AVM",avm_rel[j], "total", tot_rel[j],
             "spill", spill[j], "bypass", bypass[j]);
   }
   */
   
   insertAVM(formattedDates, 1872,tot_rel,val_flag);   

   SqlDisconnect();
   return(0);
} /* end of main */

