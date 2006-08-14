/*****************************************************************************
* DESCRIPTION
*
* This application allows privileged HDB users to create new sets of data,
* inside or outside of HDB, by aggregating and disaggregating data stored
* in HDB.
* 
* AUTHOR 
* Carol Marra
* February, 1996
*****************************************************************************/
#define MAIN 1
#define VERSION "3.5 Datatype Upgrade"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aggDisagg.h"
#include "aggDisaggDefines.h"
#include "aggDisaggTypedefs.h"
#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

#define FUNC_NAME "main"

int main (int argc, char **argv)
{
  int result,
      numBoundaries,
      numSites,
      siteCount,
      sdiIsOk;
  RUN_CONTROL runControl;
  BOUNDARY_DATES *boundaryDates;
  ID siteList[MAX_NUM_SITES],
     sourceSiteDatatypeId,
     destSiteDatatypeId;
  char *dbName;
  char *appRole;

  if((argv[1] != NULL) && (!strcmp(argv[1], "-v")))
   {
      printf("\naggDisagg version number: %s\n\n", VERSION);
      exit (OK);
   }

  if (argc < 13)
    {
      PrintError ("Usage: aggDisagg <uid> <userPassword> <aggDisaggId> <destScale> <sourceRangeNon> <destRangeNon> <statNon> <Real | (<model><src_mdl_run_id>) <File|(<db>[<dst_mdl_run_id>])\n\t(<numPer> | <numRanges>) (<startDate> | <begRange> <endRange>) \n\t<siteList>");
      exit (ERROR);
    }

   /* set the name of the database */
   dbName = NULL;
   dbName = getenv("HDB_LOCAL");
   if (dbName == NULL)
   {
      PrintError("Environment variable HDB_LOCAL not set...\n");
      return(ERROR);
   }
   
   appRole = NULL;
   appRole = getenv ("APP_ROLE");

   if (appRole == NULL)
   {
      PrintError("Environment variable APP_ROLE not set...\n");
      exit(ERROR);
   }

   RegisterSignalHandler();

   if ((result = SqlConnectRole (dbName, argv[1], argv[2], appRole)) == ERROR)
   {
      exit (ERROR);

    } 

  if ((result = SqlSetDateFormat ("DD-MON-YYYY HH24:MI:SS")) != OK)
    {
      PrintError ("%s: Problem setting date format. Exiting.\n", FUNC_NAME);
      return (ERROR);
    }


  if ((result = ParseCommandLine (&runControl, argc - 3, &(argv[3]), 
				  siteList, &numSites)) != OK)
  {
    SqlDisconnect ();
    exit (ERROR);
  }

  if ((result = SetRunControl (&runControl)) != OK)
    {
      SqlDisconnect ();
      exit (ERROR);
    }


  /* - Verify validity of information on runControl; make sure
       user isn't making invalid specifications. */
   if ((result = VerifyInputs (runControl)) != OK)
   {
      SqlDisconnect ();
      exit (ERROR);
    }

  /* If range and agg, tableDest is range;
     if range and disagg, tableDest is not range */
   if ((result = GetTableNames (&runControl)) != OK)
    {
      SqlDisconnect ();
      exit (ERROR);
    }

  /* If result data is "real", get source ID for this run; 
     if result data is "model", get model_run_id.
     For either, insert record indicating derivation
     process associated with this source or model ID. If data is
     not going to database, don't do any bookkeeping. */
    if (runControl.toDb)
    {
      if ((result = Bookkeeping (&runControl, argv[0])) != OK)
	{
	  SqlDisconnect ();
	  exit (ERROR);
	}
    }

  /* Print for debug */
#ifdef DEBUG
    PrintRunControl (runControl); 
#endif

  /* Set the array of boundary dates. */
    if (runControl.sourceIsRange || runControl.destIsRange)
    {
      if ((result = SetBoundaryDatesRange (runControl, &boundaryDates, 
					   &numBoundaries)) != OK)
	{
	  exit (ERROR);
	}
    }
  else
    {
      if ((result = SetBoundaryDates (runControl, &boundaryDates, 
				      &numBoundaries)) 
	  != OK)
	{
	  exit (ERROR);
	}
    }

#ifdef DEBUG
    PrintBoundaryDates (boundaryDates, numBoundaries); 
#endif

  /* All boundary dates are set; process values for every siteDatatype,
     for every interval. */
    for (siteCount = 0; siteCount < numSites; siteCount++)
    {
      sdiIsOk = TRUE;
      
      /* Set source and destination siteDatatypes */
      if ((result = SqlGetSiteDatatypeIdWithId (siteList[siteCount], 
						runControl.datatypeSource,
						&sourceSiteDatatypeId)) != OK)
	{
	  PrintError ("Cannot process site: %d, source datatype %d.\n\t Moving on to next site.\n", siteList[siteCount], runControl.datatypeSource);
	  sdiIsOk = FALSE;
	}
      
      if (sdiIsOk && runControl.datatypeDest != NA)
	{
	  if ((result = SqlGetSiteDatatypeIdWithId (siteList[siteCount], 
						    runControl.datatypeDest,
						    &destSiteDatatypeId)) 
	      != OK)
	    {
	      PrintError ("Cannot process site: %d, dest datatype %d.\n\t Moving on to next site.\n", siteList[siteCount], runControl.datatypeDest);
	      sdiIsOk = FALSE;
	    }
	}
      else 
	destSiteDatatypeId = NA;

      if (sdiIsOk && !runControl.isStat)
	{
	  if ((result = ProcessNonStats (runControl, boundaryDates,
					 numBoundaries, sourceSiteDatatypeId,
					 destSiteDatatypeId)) 
	      != OK)
	    {
	      SqlDisconnect ();    
	      exit (ERROR);
	    }
	}
      else if (sdiIsOk)
	{
	  if ((result = ProcessStats (runControl, boundaryDates,
	                              numBoundaries, sourceSiteDatatypeId,
				      destSiteDatatypeId, timeIntervals)) 
	      != OK)
	    {
	      SqlDisconnect ();
	      exit (ERROR);
	    }
	}
    }

    /* Free boundary dates */
    free ((char*)boundaryDates); 

    
    return (OK);
}
