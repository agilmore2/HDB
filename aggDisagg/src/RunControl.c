/*****************************************************************************
* DESCRIPTION
* 
***************************************
*
* ParseCommandLine
*
* Purpose: This function parses the command line sent to aggDisagg. All 
* parameters are required; there are no defaults. In addition to setting 
* numerous values on runControl, this function also initializes the siteList.
*
* Assumptions
*
* Input
*      RUN_CONTROL*-- pointer to structure holding info to drive application
*      int         -- argc
*      char **     -- argv; list of args to application
*      ID *        -- array of site IDs; to be set here
*      int *       -- num sites; to be set here
*
* Output
* warnings and error messages
*
* Return 
* ERROR   - failure; updates not complete
* OK      - updates successfully completed
*
***************************************
* SetRunControl
*
* Purpose: This function sets the remaining members of the runControl
*          structure that can be determined at this time.
*
* Assumptions
*
* Input
*      RUN_CONTROL* -- pointer to structure holding info to drive application
*
* Output
* warnings and error messages
*
* Return 
* ERROR   - failure; updates not complete
* OK      - updates successfully completed
*
***************************************
* VerifyInputs
*
* Purpose:   
*     - Verify that objecttypes are the same for each datatype. 
*       This is unnecessary if integrity is built into DB. 
*     - Verify obsSource jives w/ datatype; if obsSource = INST,
*       can't have an avg/tot/eop datatype.
*     - If (isStat), datatypeDest cannot end in eop, tot, etc. 
*     - If (sourceIsRange and agg)
*         - obsSource must = obsDest, 
*         - must be statistics
*         - destination must be range
*     - If (sourceIsRange and !isStat) must be disagg.
*     - If (sourceIsRange || destIsRange) verify dateBegRange < dateEndRange)
*       and methodClass cannot be EOP
*     - If (sourceIsRange || destIsRange), corresponding observation
*       interval must be monthly (i.e., only monthly ranges are handled)
*     - Do not allow stats and disagg in conjunction. 
*     - If DISAGG, destination cannot be range.
*     - Source interval must be valid DB interval.
*     - If result data goes to DB, destination interval must be valid DB 
*       interval.
*     - If you're not aggregating or disaggregating (eg you're doing unit
*       conversion) you can't write to the database.
*     - Statistics on model data not allowed.
*     - MethodClass of EOP must have destination datatype method of EOP
*     - MethodClass of NA must be associated w/ disaggregation.
* 
* Input
* 	RUN_CONTROL - structure holding run control info
*
* Output
*
* Return
* 	int - OK or ERROR
*
***************************************
* Bookkeeping
*
* Purpose:   
*     Depending on whether data goes to real or model table,
*     generate source_id or model_run_id with appropriate 
*     associated info.
*     
*     Insert a record into ref_data_derivation, mapping the 
*     source/model run ID onto the derivation process.
* 
* Input
* 	RUN_CONTROL * - structure holding run control info
*
* Output
*
* Return
* 	int - OK or ERROR
*
***************************************
* GetTableNames
*
* Purpose:   
*     Based on parameters on run control, determine the name
*     of the source and destination data tables.
* 
* Input
* 	RUN_CONTROL * - structure holding run control info
*
* Output
*
* Return
* 	int - OK or ERROR
* Assumptions
*
* AUTHOR 
* Carol Marra
* February, 1996
*****************************************************************************/

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

extern TIME_INTERVALS timeIntervals[NUM_TIME_INTERVALS];

#define FUNC_NAME "ParseCommandLine"

int ParseCommandLine (RUN_CONTROL *runControl, int argc, char **argv, 
		      ID *siteList, int *numSites)

{ 
  int result, 
      siteCount, 
      firstSite,
      siteNum,
      width,
      precision;
  char format[2],
       *temp,
       onechar;
  
  /* Get init info from DB and have user choose. Note that argv[] starts
   with the aggDisaggId; it does not include executable name or password. */
  if ((result = SqlLoadAggDisagg (runControl, atoi (argv[0]))) != OK)
    {
      SqlDisconnect();
      return (ERROR);
    }

  runControl->toScale = atoi (argv[1]);
  runControl->sourceIsRange = (!strcmp (argv[2], "n") ? 0 : 1);
  runControl->destIsRange = (!strcmp (argv[3], "n") ? 0 : 1);
  runControl->isStat = (!strcmp (argv[4], "n") ? 0 : 1);
  runControl->sourceIsReal = (!strcmp (argv[5], "r") ? 1 : 0);

  /* Check to see if source model_run_id is specified */
  if (!runControl->sourceIsReal && (strlen (argv[5]) > 1))
    {
      sscanf (argv[5], "m%d", &runControl->model_run_id_src);
    }
  else if (!runControl->sourceIsReal && (strlen (argv[5]) == 1))
    {
      PrintError (" Source model run id specified incorrectly.\n\tExiting.");
      return (ERROR);
    }

  runControl->toDb = (!strncmp (argv[6], "d",1) ? 1 : 0);

  /* Set format, width and precision for file output. */
  if (runControl->toDb)
    {
      runControl->format = NA;
      runControl->width = NA;
      runControl->precision = NA;
      runControl->model_run_id_flag = FALSE;

      /*Checking to see if destination model run id is being specified and
        if so save it*/

      if (strlen (argv[6]) > 1)
      {
          onechar = argv[6][1];
          temp = NULL;
          temp = strstr (argv[6], &onechar);
          sscanf (temp, "%d", &runControl->model_run_id_dest);
          runControl->model_run_id_flag = TRUE;
      }
    }
  else
    {
       runControl->model_run_id_flag = FALSE;

      /* If there are any format specs, read them in, otherwise assign
	 defaults. */
      if (strlen (argv[6]) > 1)
	{
	  /* Read in specs; replace "." delimiter w/ white space */
	  temp = strstr (argv[6], ".");
	  if (temp)
	    temp[0] = ' ';
	  else
	    {
	      PrintError ("Width and precision specified incorrectly. Decimal (.) delimiter required.\n\tExiting.");
	      return (ERROR);
	    }

	  if (sscanf (&argv[6][1], "%1c%d %d", format, &width, 
		      &precision) != 3)
	    {
	      PrintError ("Problem reading in format, width and precision for file output.\n\tExiting.");
	      return (ERROR);
	    }

	  /* Assign specs to runControl */
	  runControl->format = (!strcmp (format, "f") || 
			       (!strcmp (format, "F"))) ? FLOATING : EXP;
	  runControl->width = width;
	  runControl->precision = precision;
	}

      else 
	{
	  runControl->format = EXP;
	  runControl->width = WIDTH;
	  runControl->precision = PRECISION;
	}
    }
		       
  runControl->numPer = atoi (argv[7]);

  if (runControl->sourceIsRange || runControl->destIsRange)
    {
       if (result = SqlFormatDate (argv[8], runControl->dateStart) == ERROR)
       {
	  PrintError ("%s: Problem formatting start date.\n", FUNC_NAME);
	  return (ERROR);
       }
       if (result = SqlFormatDate (argv[8], runControl->dateBegRange) == ERROR)
       {
	  PrintError ("%s: Problem formatting range begin date.\n", FUNC_NAME);
	  return (ERROR);
       }
       if (result = SqlFormatDate (argv[9], runControl->dateEndRange) == ERROR)
       {
	  PrintError ("%s: Problem formatting range end date.\n", FUNC_NAME);
	  return (ERROR);
       }
      firstSite = 10;
    }
  else
    {
       if (result = SqlFormatDate (argv[8], runControl->dateStart) == ERROR)
       {
	  PrintError ("%s: Problem formatting start date.\n", FUNC_NAME);
	  return (ERROR);
       }
      runControl->dateBegRange[0] = '\0';
      runControl->dateEndRange[0] = '\0';
      firstSite = 9;
    }

  for (siteCount = firstSite, siteNum = firstSite - siteCount; 
       siteCount < argc; siteCount++, siteNum++)
    {
      siteList[siteNum] = atoi (argv[siteCount]);
    }

  *numSites = siteCount - firstSite;

  return (OK);
}
  
/**************************************************************************/
#define FUNC_NAME "SetRunControl"

int SetRunControl (RUN_CONTROL *runControl)
{
  int result;
  
  /* Set the unit and dimension information on runControl-> 
     Must do this before checking for dimensionChangeInBaseArea */
  if ((result = SqlGetUnitId (runControl->datatypeSource, 
			      &(runControl->unitSource))) != OK)
    {
      return (ERROR);
    }

  if ((result = SqlGetDimensionId (runControl->unitSource, 
			      &(runControl->dimensionSource))) != OK)
    {
      return (ERROR);
    }

  if (runControl->unitDest == NA)
    {
      if ((result = SqlGetUnitId (runControl->datatypeDest,
				  &(runControl->unitDest))) != OK)
      {
	 return (ERROR);
      }
      if ((result = SqlGetDimensionId (runControl->unitDest, 
				       &(runControl->dimensionDest))) != OK)
      {
	 return (ERROR);
      }
    }

  /* Determine if operation is aggregation or disaggregation, or
     neither. */
  /* No operation or function => no agg or disagg. (Unit/datatype conversion
     only. */
  if (!strcmp (runControl->opOrFcn, "N"))
    runControl->opType = NA;

  /* Standard AGG, or creating stats, range or range statistics data. */
  else 
  {
     /* Determine if this is the special case of converting flow->vol
	or power->energy in the base area. */
     if ((result = DimensionChangeInBaseArea (runControl)) != OK)
     {
	return (ERROR);
     }

     if ((runControl->obsSource < runControl->obsDest) ||
	 (runControl->isStat) || 
	 ((runControl->obsSource == runControl->obsDest) &&
	  (runControl->destIsRange || runControl->dimensionChangeInBaseArea)))
	runControl->opType = AGG;

     /* Disagg */
     else if (runControl->obsSource > runControl->obsDest)
	runControl->opType = DISAGG;
     else
     {
	PrintError ("Cannot determine if this is aggregation or disaggregation.\n\tExiting.");
	return (ERROR);
     }
  }

  /* Set value of destIsReal:
     - disaggregated and aggregated data go to the same kind of 
       table it came from;
     - no-op data doesn't go to the database
  */
    runControl->destIsReal = runControl->sourceIsReal;

  /* Get the observation period which drives calculations. It is always
     the greater of the 2 observation periods. E.g., if we're going from 
     daily to monthly values, the data must be processed by month. 
     If we're going from yearly to monthly values, the data must be 
     processed by year. */
  if (runControl->obsSource > runControl->obsDest)
    {
      runControl->obsDriver = runControl->obsSource;
      runControl->obsSub = runControl->obsDest;
    }
  else
    {
      runControl->obsDriver = runControl->obsDest;
      runControl->obsSub = runControl->obsSource;
    }

  /* Set corresponding Ingres and HDB observation intervals. */
  strcpy (runControl->sqlObsDriver, 
	  timeIntervals[(int)runControl->obsDriver].ingresName);
  strcpy (runControl->sqlObsSub,
	  timeIntervals[(int)runControl->obsSub].ingresName);
  runControl->hdbObsDriver = timeIntervals[(int)runControl->obsDriver].unitId;
  runControl->hdbObsSub = timeIntervals[(int)runControl->obsSub].unitId;

  /* Set dateEnd; calculation depends on if any data (source or destination)
     is range. */
  if (runControl->sourceIsRange || runControl->destIsRange)
    {
      /* This is really dateEnd of the first range (e.g., 31-jul-96). */
      if ((result = SqlDateMath (ADDITION, runControl->dateEndRange, 
				 runControl->dateEnd, 1, 
				 runControl->sqlObsDriver)) != OK)
	{
	  return (ERROR);
	}
    }
  else
    {
      if ((result = SqlDateMath (ADDITION, runControl->dateStart, 
				 runControl->dateEnd, runControl->numPer, 
				 runControl->sqlObsDriver)) != OK)
	{
	  return (ERROR);
	}
    }
  
  /* To properly set dateEnd to the very end of the interval being represented,
     subtract 1 second from the value as it is now. */
  if ((result = SqlDateMath (SUBTRACTION, runControl->dateEnd, 
			     runControl->dateEnd, 1, 
			     timeIntervals[(int)BY_SECOND].ingresName)) != OK)
    {
      return (ERROR);
    }
  
  /* Ensure that dateStart is beginning of interval date */
  if ((result = SqlDateTrunc (runControl->dateStart, runControl->sqlObsDriver,
                              runControl->dateStart)) != OK)
    {
      PrintError ("Problem setting dateStart to beginning-of-interval status.\n\tExiting.");
      return (ERROR);
    }

  /* Set identifying values */
  if ((result = SqlGetAgencyId (SEE_LOADING_APP, &(runControl->agencyId))) != OK)
    {
      PrintError ("Problem getting agency ID.\n\tExiting.");
      return (ERROR);
    }
  
  /* If this is disagg of real data, set overwrite flag. Otherwise,
     set it to NULL */
  if (runControl->opType == DISAGG && runControl->sourceIsReal)
    {
      if ((result = SqlGetOverwriteFlag (OVERWRITE,
					 runControl->overwriteFlag)) != OK)
	{
	  PrintError ("Problem getting overwrite flag.\n\tExiting.");
	  return (ERROR);
	}
    }
  else
    runControl->overwriteFlag[0] = '\0';

  if ((result = SqlGetCollectionId (SEE_LOADING_APP,
				    &(runControl->collectionSystemId))) != OK)
    {
      PrintError ("Problem getting collection system ID.\n\tExiting.");
      return (ERROR);
    }

  if ((result = SqlGetLoadingId (LOADING_APPLICATION_NAME, 
				 &(runControl->loadingApplicationId))) != OK)
    {
      PrintError ("Problem getting loading application ID.\n\tExiting.");
      return (ERROR);
    }

  /* Set methodId to UNKNOWN only if it's NA, meaning user defined function;
   otherwise, use method indicated in ref_agg_disagg table */
  if (runControl->methodId == NA)
    {
      if ((result = SqlGetMethodId (UNKNOWN, &(runControl->methodId))) != OK)
	{
	  PrintError ("Problem getting method ID.\n\tExiting.");
	  return (ERROR);
	}
    }

  if ((result = SqlGetComputationId (UNKNOWN, &(runControl->computationId))) != OK)
    {
      PrintError ("Problem getting computation ID.\n\tExiting.");
      return (ERROR);
    }

  return (OK);
}
  
/**************************************************************************/
#define FUNC_NAME "PrintRunControl"

int PrintRunControl (RUN_CONTROL runControl)
{

  printf ("**************   RUN CONTROL  ****************\n\n");
  printf ("\tSourceIsRange: %d\n", runControl.sourceIsRange);
  printf ("\tDestIsRange: %d\n", runControl.destIsRange);
  printf ("\tIsStat: %d\n", runControl.isStat);
  printf ("\tSourceIsReal: %d\n", runControl.sourceIsReal);
  printf ("\tDestIsReal: %d\n", runControl.destIsReal);
  printf ("\tToDB: %d\n", runControl.toDb);

  printf ("\n");

  printf ("\tmodel_run_id_source: %d\n", runControl.model_run_id_src);
  printf ("\tmodel_run_id_dest: %d\n", runControl.model_run_id_dest);
  printf ("\tmodel_run_id_flag: %d\n", runControl.model_run_id_flag);

  printf ("\n");

  printf ("\tInterval: %s\n", runControl.interval);
  printf ("\tModelOrSourceId: %d\n", runControl.modelOrSourceId);
  printf ("\tAgency: %d\n", runControl.agencyId);
  printf ("\tOverwriteFlag: %s\n", runControl.overwriteFlag);
  printf ("\tCollectionSystemId: %d\n", runControl.collectionSystemId);
  printf ("\tLoadingApplicationId: %d\n", runControl.loadingApplicationId);
  printf ("\tMethodId: %d\n", runControl.methodId);
  printf ("\tComputationId: %d\n", runControl.computationId);

  printf ("\tDatatypeSource: %d\n", runControl.datatypeSource);
  printf ("\tUnitSource: %d\n", runControl.unitSource);

  printf ("\n");

  printf ("\tDatatypeDest: %d\n", runControl.datatypeDest);
  printf ("\tUnitDest: %d\n", runControl.unitDest);

  printf ("\n");

  printf ("\tObsSource: %d\n", runControl.obsSource);
  printf ("\tObsDest: %d\n", runControl.obsDest);
  printf ("\tObsDriver: %d\n", runControl.obsDriver);
  printf ("\tObsSub: %d\n", runControl.obsSub);
  printf ("\tSqlObsDriver: %s\n", runControl.sqlObsDriver);
  printf ("\tSqlObsSub: %s\n", runControl.sqlObsSub);
  printf ("\tHdbObsDriver: %d\n", runControl.hdbObsDriver);
  printf ("\tHdbObsSub: %d\n", runControl.hdbObsSub);

  printf ("\n");

  printf ("\tDateStart: %s\n", runControl.dateStart);
  printf ("\tDateEnd: %s\n", runControl.dateEnd);
  printf ("\tDateBegRange: %s\n", runControl.dateBegRange);
  printf ("\tDateEndRange: %s\n", runControl.dateEndRange);
  printf ("\tNumPer: %d\n", runControl.numPer);

  printf ("\n");

  printf ("\tTableSource: %s\n", runControl.tableSource);
  printf ("\tTableDest: %s\n", runControl.tableDest);

  printf ("\n");

  printf ("\tOpType: %s\n", (runControl.opType == AGG ? "AGG" : 
			     (runControl.opType == DISAGG ? "DISAGG" : "NA")));
  printf ("\tOperatorOrFunction: %s\n", runControl.opOrFcn);
  printf ("\tMethodId: %d\n", runControl.methodId);
  printf ("\tMethodClassId: %d\n", runControl.methodClassId);
  printf ("\tFunctionName: %s\n", runControl.functionName);

  printf ("\n");  
  printf ("\n");

  return (OK);
}


/**************************************************************************/
#define FUNC_NAME "VerifyInputs"

int VerifyInputs (RUN_CONTROL runControl)
{

  int result,
      isInstant,
      isEither,
      compareResult;
  
  /* Disallow aggregation of real data that is not stat, range,
     or a dimension change */
  if (runControl.opType == AGG &&
      runControl.sourceIsReal &&
      !runControl.isStat &&
      !runControl.sourceIsRange &&
      !runControl.destIsRange &&
      !runControl.dimensionChangeInBaseArea)
    {
       PrintError ("You cannot aggregate (to the DB or a file) real data that is not statistics, range, or a dimension change in the base area.\nThis looks like an operation for the Derivation Application.\n\tExiting.");
       return (ERROR);
    }

  /* If obsSource is instantaneous, make sure datatypeSource is 
     instantaneous */
  if (runControl.obsSource == BY_INSTANT)
    {
      if ((result = SqlDatatypeIsInstant (runControl.datatypeSource, 
					  &isInstant)) != OK)
	{
	  return (ERROR);
	}
      
      if (!isInstant)

      {
	  PrintError ("Source datatype %d is not instantaneous;\n\tsource observation interval IS instantaneous.\n\tExiting.", runControl.datatypeSource);
	  return (ERROR);
	}
    }

  /* If destination data is instantaneous, must be a no-op. (Can't 
     disaggregate or aggregate to instantaneous; only unit/datatype
     conversion. */
  if ((runControl.obsDest == BY_INSTANT) && (runControl.opType != NA))
    {
      PrintError ("Destination is instantaneous; you cannot aggregate or disaggregate.\n\tUnit or datatype conversion only is allowed.\n\tExiting.");
      return (ERROR);
    }
    
  /* If statistics data is being generated, destination datatype cannot be an
     end-of-period, beginning-of-period or total. Best way to check for
     this in new datatype design is to look for allowable_intervals of "either".
     Excluding "either" datatypes will exclude a few that might be OK for 
     status, but this probably will not be an issue... */
  if (runControl.isStat)
    {
      if (runControl.datatypeDest != NA)
	{
	  if ((result = SqlDatatypeIsEither (runControl.datatypeDest,
					     &isEither)) != OK)
	    {
	      return (ERROR);
	    }
      
	  if (isEither)
	    {
	      PrintError ("Allowable interval of EITHER for datatype %d indicates invalid datatype for statistics.\nContact support if you believe this to be in error.\n\tExiting.", runControl.datatypeDest);
	      return (ERROR);
	    }
	}
    }

  /* For source or destination ranges, corresponding observation interval
     must be monthly. */
  if (runControl.sourceIsRange && !(runControl.obsSource == BY_MONTH))
    {
      PrintError ("Source data is range, but not monthly. Only monthly range data is handled.\n\tExiting.");
      return (ERROR);
    }

  if (runControl.destIsRange && !(runControl.obsDest == BY_MONTH))
    {
      PrintError ("Destination data is range, but not monthly. Only monthly range data is handled.\n\tExiting.");
      return (ERROR);
    }

  /* If source data is range and op_type is agg, 
      - result data must be statistics;
      - source and destination observation intervals must be same. Cannot
        aggregate across ranges.
      - destination must be range. */
  if (runControl.sourceIsRange && (runControl.opType == AGG))
    {
      if (!runControl.isStat)
	{
	  PrintError ("Your source data is RANGE, and you are aggregating.\n\tResulting data must be statistics.\n\tExiting.");
	  return (ERROR);
	}

      if (runControl.obsSource != runControl.obsDest)
	{
	  PrintError ("Your source data is RANGE, and you are aggregating.\n\tSource and destination intervals must be the same.\n\tE.g., Cannot aggregate from daily range to monthly range.\n\tExiting.");
	  return (ERROR);
	}

      if (!runControl.destIsRange)
	{
	  PrintError ("Your source data is RANGE, and you are aggregating.\n\tYour destination data must be range.\n\tExiting.");
	  return (ERROR);
	}
    }

  /* If source data is range and you're not doing statistics, operation must be
     NA or DISAGG. You can't aggregate. Converse of above. */
  if (runControl.sourceIsRange && !runControl.isStat)
    {
      if (runControl.opType == AGG)
	{
	  PrintError ("Your source data is RANGE, and you're not doing statistics.\n\tYou must disaggregate or do datatype/unit conversion (not aggregate).\n\tExiting.");
	  return (ERROR);
	}
    }

  /* Verify begin and end range dates. Do not allow  EOP aggregation
   in conjunction with any range data. */
  if (runControl.sourceIsRange || runControl.destIsRange)
    {
      if (runControl.methodClassId == EOP_METHOD_CLASS)
	{
	  PrintError ("Range data not allowed in conjunction with EOP aggregation.\n\tExiting.");
	  return (ERROR);
	}

      if ((result = SqlDateCompare (runControl.dateBegRange, runControl.dateEndRange,
				    &compareResult)) != OK)
	{
	  return (ERROR);
	}

      if (compareResult >= 0)
	{
	  PrintError ("Begin range date: %s must be less than end range date: %s.\n\tExiting.", runControl.dateBegRange, runControl.dateEndRange);
	  return (ERROR);
	}
    }	  

  /* Do not allow statistics and disaggregation in conjunction. */
  if (runControl.isStat && (runControl.opType == DISAGG))
    {
      PrintError ("Statistics and disaggregation not allowed in conjunction.\n\tExiting.");
      return (ERROR);
    }

  /* If disaggregating, destination data cannot be range. */
  if ((runControl.opType == DISAGG) && runControl.destIsRange)
    {
      PrintError ("You are disaggregating; destination data cannot be range.\n\tExiting.");
      return (ERROR);
    }
    
  /* If result data goes to DB, destination interval must be valid 
     DB interval and scale must be 1. */
  if (runControl.toDb)
    {
      if (!DB_TABLE_INTERVAL (runControl.obsDest))
	{
	  PrintError ("Data tables do not exist for destination observation interval '%s'.\n\tExiting.", timeIntervals[runControl.obsDest].ingresName);
	  return (ERROR);
	}

      if (runControl.toScale != 1)
	{
	  PrintError ("Scale must be 1 when writing data back to database.\n\tExiting.");
	  return (ERROR);
	}
    }
  
  /* If there is no destination datatype, you cannot write to the database.
     (Because you can't get a site_datatype_id.) */
  if ((runControl.datatypeDest == NA) && runControl.toDb)
    {
      PrintError ("You cannot write to the database if you are doing straight unit conversion.\n\tExiting.");
      return (ERROR);
    }

  /* Statistics cannot be generated on model data. */
  if (!runControl.sourceIsReal && runControl.isStat)
    {
      PrintError ("You cannot generate statistics on model data.\n\tExiting.");
      return (ERROR);
    }

  /* Can no longer verify if run control method_class is a match for the 
     datatype... */


  /* If methodClass is NA, must be disaggregating. */
  if ((runControl.methodClassId == COPY_METHOD_CLASS) && 
      (runControl.opType == AGG))
  {
     PrintError ("MethodClass of N/A requires DISAGGREGATION.\n\tExiting.");
     return (ERROR);
  }

  /* Is the destination model_run_id valid? */
  if ((runControl.model_run_id_flag) && ((result =  SqlChkModelRunID(runControl.model_run_id_dest)) != OK))
  {
      PrintError ("You have chosen a destination model_run_id that does not exist.\n\tExiting.");
      return (ERROR);
  }

  return (OK);
}

/**************************************************************************/
#define FUNC_NAME "Bookkeeping"

int Bookkeeping (RUN_CONTROL *runControl, char *executableName)

{
  int result;
  SQL_DATE now;
  ID modelId,
     modelRunId,
     sourceId;
  char modelRunName[64],
       timeStep[64];

  if ((result = SqlGetDateTime (now)) != OK)
    {
      return (ERROR);
    }

  /* For range and stat data, get a source_id; for dimension changes
     going into r_base, get a loading_application_id. For either, store
     the result in runControl->modelOrSourceId */
  if (runControl->destIsReal)
    {
       if (runControl->dimensionChangeInBaseArea ||
	   (runControl->opType == DISAGG && runControl->sourceIsReal))
       {
	  if ((result = SqlGetLoadingId (LOADING_APPLICATION_NAME, &sourceId)) != OK)
	  {
	     return (ERROR);
	  }
       }
       else
       {
	  if ((result = SqlGetAppSourceId (executableName, &sourceId)) != OK)
	  {
	     return (ERROR);
	  }
       }

       runControl->modelOrSourceId = sourceId;
    }

  /* Data goes to model table. */
  else
    {
      sprintf (modelRunName, "aggDisagg %s", now);
      sprintf (timeStep, "%s to %s", 
	       timeIntervals[runControl->obsSource].ingresName,
	       timeIntervals[runControl->obsDest].ingresName);

      if ((result = SqlGetModelId (MODEL_NAME, &modelId)) != OK)
	{
	  return (ERROR);
	}

      if (!runControl->model_run_id_flag)
      {
          if ((result = SqlGetModelRunId (modelId, modelRunName, now, NA, NULL,
                                      NA, NULL, NULL, timeStep,
                                      NULL, &modelRunId)) != OK)
            {
              return (ERROR);
            }
      }
      else
          modelRunId = runControl->model_run_id_dest;
      runControl->modelOrSourceId = modelRunId;
    }

  return (OK);
}

/**************************************************************************/
#define FUNC_NAME "GetTableNames"

int GetTableNames (RUN_CONTROL *runControl)

{
  /* Build source data table name */
  if (runControl->sourceIsReal)
    strcpy (runControl->tableSource, "r_");
  else
    strcpy (runControl->tableSource, "m_");

  /* If this is a dimension change, the source data is in r_base;
     otherwise, it's in one of the interval tables. */
  if (runControl->dimensionChangeInBaseArea)
     strcat (runControl->tableSource, "base");
  else 
     switch (runControl->obsSource)
     {
	 case BY_INSTANT: 
	    strcat (runControl->tableSource, "instant");
	    break;
	 case BY_HOUR: 
	    strcat (runControl->tableSource, "hour");
	    break;
	 case BY_DAY: 
	    strcat (runControl->tableSource, "day");
	    break;
	 case BY_MONTH: 
	    strcat (runControl->tableSource, "month");
	    break;
	 case BY_YEAR: 
	    strcat (runControl->tableSource, "year");
	    break;
	 case BY_WY: 
	    strcat (runControl->tableSource, "wy");
	    break;
	 default:
	    PrintError ("This is an invalid source observation period: %s.\n\tExiting.", timeIntervals[runControl->obsSource].ingresName);
	    return (ERROR);
     }

  /* Note that statistics data cannot be used as a source. */
  if (runControl->sourceIsRange)
    {
      strcat (runControl->tableSource, RANGE_MOD);
    }

  /* Build destination data table name */
  if (runControl->toDb)
    {
      if (runControl->destIsReal)
	strcpy (runControl->tableDest, "r_");
      else
	strcpy (runControl->tableDest, "m_");
      
      /* If this is a dimension change or a disagg of real data, the 
	 result is written to r_base; otherwise, it goes to one of the 
	 interval tables. */
      if (runControl->dimensionChangeInBaseArea ||
	  (runControl->opType == DISAGG && runControl->sourceIsReal))
	 strcat (runControl->tableDest, "base");
      else 
	 switch (runControl->obsDest)
	 {
	     case BY_INSTANT: 
		strcat (runControl->tableDest, "instant");
		break;
	     case BY_HOUR: 
		strcat (runControl->tableDest, "hour");
		break;
	     case BY_DAY: 
		strcat (runControl->tableDest, "day");
		break;
	     case BY_MONTH: 
		strcat (runControl->tableDest, "month");
		break;
	     case BY_YEAR: 
		strcat (runControl->tableDest, "year");
		break;
	     case BY_WY: 
		strcat (runControl->tableDest, "wy");
		break;
	     default:
		PrintError ("This is an invalid destination observation period: %s.\n\tExiting.", timeIntervals[runControl->obsDest].ingresName);
		return (ERROR);
	 }
      
      if (runControl->isStat)
	{
	  strcat (runControl->tableDest, STAT_MOD);
	}
      
      if (runControl->destIsRange)
	{
	  strcat (runControl->tableDest, RANGE_MOD);
	}
    }
  else
    runControl->tableDest[0] = '\0';
      
  return (OK);
}
  
  
/**************************************************************************/
#define FUNC_NAME "DimensionChangeInBaseArea"

int DimensionChangeInBaseArea (RUN_CONTROL *runControl)

{
  /* True if:
     source and destination intervals are equal;
     data source is real data;
     not generating statistics or range data;
     datatypes indicate flow->vol, vol->flow, or power->energy conversion
  */
     
  if (runControl->obsSource == runControl->obsDest &&
      runControl->sourceIsReal &&
      !runControl->isStat &&
      !runControl->sourceIsRange &&
      !runControl->destIsRange &&
      ((runControl->dimensionSource == FLOW_ID &&
	runControl->dimensionDest == VOLUME_ID) ||
       (runControl->dimensionSource == VOLUME_ID &&
	runControl->dimensionDest == FLOW_ID) ||
       (runControl->dimensionSource == POWER_ID &&
	runControl->dimensionDest == ENERGY_ID)))
     runControl->dimensionChangeInBaseArea = TRUE;
  else
     runControl->dimensionChangeInBaseArea = FALSE;
      
  return (OK);
}
  
  
  
  
