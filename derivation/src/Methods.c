/*****************************************************************************
* DESCRIPTION
*
* This file contains nonSQL functions related to the methods that derive 
* data upward to the next interval 
*
***************************************
*
* MethodMax
*
* Purpose: Finds the maximum of the values in the source data and derives
*          this maximum value upward to the destination interval.
*
* Input:   SQL_INTERVAL - destination interval to write the result to
*          UPDATE_RECORD* - pointer to the record to be derived
*          INTERVAL_TIME* - pointer to the record's time structure
*          REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          DEST_PARAM* - pointer to the relevant destination parameters
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* MethodTwavelint
*
* Purpose: Finds the time-weighted average of the values in the source
*          interval using linear interpolation for the interval ends and
*          and derives it to the destination interval.  Also may
*          derive the time_weighted average of the two adjacent destination
*          intervals whose results may have been impacted by the change in
*          data in the original source interval.  This is true because data 
*          at the beginning and end of a destination interval may be
*          interpolated based on data points in the adjacent intervals.
*
* Input:   SQL_INTERVAL - destination interval to write the result to
*          UPDATE_RECORD* - pointer to the record to be derived
*          INTERVAL_TIME* - pointer to the record's time structure
*          REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          DEST_PARAM* - pointer to the relevant destination parameters
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* TWACalc
*
* Purpose: Finds the time_weighted average of the values for the given
*          interval and derives it to the destination table.  Calculating
*          the time-weighted average may involve interpolating a start and 
*          end value for the given interval.
*
* Input:   SQL_INTERVAL - destination interval to write the result to
*          UPDATE_RECORD* - pointer to the record to be derived
*          INTERVAL_TIME* - pointer to the record's time structure
*          DATA_RECORD* - pointer to the result record for the derivation
*          REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          DEST_PARAM* - pointer to the relevant destination parameters
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* MethodEop
*
* Purpose: Finds the last value ("end of period") in the source data and 
*          derives this value upward to the destination interval.
*
* Input:   SQL_INTERVAL - destination interval to write the result to
*          UPDATE_RECORD* - pointer to the record to be derived
*          INTERVAL_TIME* - pointer to the record's time structure
*          REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          DEST_PARAM* - pointer to the relevant destination parameters
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* MethodTot
*
* Purpose: Finds the total or sum over time for the data in the source window
*          and derives this value upward to the destination interval.
*
* Input:   SQL_INTERVAL - destination interval to write the result to
*          UPDATE_RECORD* - pointer to the record to be derived
*          INTERVAL_TIME* - pointer to the record's time structure
*          REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          DEST_PARAM* - pointer to the relevant destination parameters
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* MethodAve
*
* Purpose: Finds the arithmetic average of the data in the source window and 
*          derives this value upward to the destination interval.
*
* Input:   SQL_INTERVAL - destination interval to write the result to
*          UPDATE_RECORD* - pointer to the record to be derived
*          INTERVAL_TIME* - pointer to the record's time structure
*          REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          DEST_PARAM* - pointer to the relevant destination parameters
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* MethodMin
*
* Purpose: Finds the minimum of the values in the source data and derives
*          this minimum value upward to the destination interval.
*
* Input:   SQL_INTERVAL - destination interval to write the result to
*          UPDATE_RECORD* - pointer to the record to be derived
*          INTERVAL_TIME* - pointer to the record's time structure
*          REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          DEST_PARAM* - pointer to the relevant destination parameters
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* MethodBop
*
* Purpose: Finds the the first value ("beginning of period") in the source
*          data and derives this value upward to the destination interval.
*
* Input:   SQL_INTERVAL - destination interval to write the result to
*          UPDATE_RECORD* - pointer to the record to be derived
*          INTERVAL_TIME* - pointer to the record's time structure
*          REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          DEST_PARAM* - pointer to the relevant destination parameters
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* MethodTovol
*
* Purpose: Finds the volume from flow or volume data in the source interval
*          and derives this volume upward to the destination interval.
*
* Input:   SQL_INTERVAL - destination interval to write the result to
*          UPDATE_RECORD* - pointer to the record to be derived
*          INTERVAL_TIME* - pointer to the record's time structure
*          REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          DEST_PARAM* - pointer to the relevant destination parameters
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* MethodEoplint
*
* Purpose: Finds a linearly interpolated end of period value based on the 
*          last source data value in the interval and the first data value in
*          the next interval and derives the result to the destination 
*          interval.  Also rederives the interpolated end of period value
*          for the previous interval which may have been affected by the
*          change in data in the current source interval.
*
* Input:   SQL_INTERVAL - destination interval to write the result to
*          UPDATE_RECORD* - pointer to the record to be derived
*          INTERVAL_TIME* - pointer to the record's time structure
*          REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          DEST_PARAM* - pointer to the relevant destination parameters
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* PartialCalc
*
* Purpose: Determines if the derivation is partial, if the calc should
*          proceed based on the derivation specification, and assigns 
*          the derivation flag
*
* Input:   REF_DESTINATION* - pointer to the ref_derivation_destination
*                             specification
*          INTERVAL_TIME* - pointer to the source record's time structure
*
* Output:  DATA_RECORD* - pointer to what will be the resulting data record
*                         and its derivation flag member
*          int* - pointer to a valid parameter that is assigned 0 for false
*                 and 1 for true indicating if the calculation should 
*                 proceed or not
*
* Return:  int - OK or ERROR
*
***************************************
*
* InterpCalc
*
* Purpose: This function interpolates a value for a data record between two
*          given data records. All three data records must have date/times;
*          the first and last data records must have values.
*          This function will calculate an interpolated value and determine
*          a sourceId for the interpolated data record
*
* Input:   DATA_RECORD* - pointer to the first data record
*          DATA_RECORD* - pointer to the last data record
*
* Output:  DATA_RECORD* - pointer to what will be the interpolated data
*                         record
*
* Return:  int - OK or ERROR
*
***************************************
*
* NegNumSpec
*
* Purpose: This is called if one of the specs in ref_derivation_destination
*          for the number of values is negative.  This should only happen for
*          specs corresponding to month and has the meaning of the number of
*          missing values for the month.  This is needed because month is the 
*          only interval that has an unequal number of the previous intervals
*          (days) to be used as source values.  This function calculates the
*          positive number of source values applicable for that month.
*
* Input:   SQL_INTERVAL - the destination interval
*          INTERVAL_TIME* - pointer to time record for the derivation
*          DEST_PARAM* - pointer to the user parameters for the derivation
*
* Output:  int* - desired number of source values for the particular month
*          int* - required number of source values for the particular month
*
* Return:  int - OK or ERROR
*
***************************************
*
* Author
*
* Neil Wilson
* November, 2001
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "dbutils.h"
#include "defines.h"
#include "typedefs.h"
#include "derivation.h"
#include "derivationTypedefs.h"

extern SQL_DATE DATETIMELOADED;
extern FILE* LOGFILE;

#define FUNC_NAME "MethodMax"

int MethodMax(SQL_INTERVAL destInterval, 
                 UPDATE_RECORD* updateRecord,
                 INTERVAL_TIME* timeRecord,
                 REF_DESTINATION* refDestination,
                 DEST_PARAM* destParameters)
{
   /* This function writes the maximum value from the source data into 
      the destination table */

   DATA_RECORD resultRecord;
   DATA_RECORD* sourceList;
   int desired;
   int maxRecord = 0;
   int required;
   int result;
   int sourceCount;
   int sourceIndex;
   int valid;

   /* Check for partial calculation */
   if ((result = PartialCalc(refDestination, timeRecord, &resultRecord,
                                &valid )) != OK)
   {
      FilePrintError(LOGFILE, "Problem checking for partial calculation\n");
      return (ERROR);
   }

   if (valid == 0)
   {
      /* Calculation should not go forward based on partial calc check */
      return (OK);
   }

   /* Retrieve the source data */
   if ((result = SqlSelectData(updateRecord,
                               timeRecord->begSource,
                               timeRecord->endSource,
                               &sourceCount,
                               &sourceList )) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying source data for derivation\n");
      return (ERROR);
   }

   /* Check if any of the "number of source value" specs are negative */
   if ((destParameters->requiredNumSource != VALUE_NULL && 
        destParameters->requiredNumSource <= 0) ||
       (destParameters->desiredNumSource != VALUE_NULL &&
        destParameters->desiredNumSource <= 0))
   {
      if ((result = NegNumSpec(destInterval,
                               timeRecord,
                               destParameters,
                               &desired,
                               &required )) != OK)
      {
         FilePrintError(LOGFILE, "Problem evaluating a negative number of source values spec\n");
         return (ERROR);
      }      
   }
   else
   {
      desired = destParameters->desiredNumSource;
      required = destParameters->requiredNumSource;
   }

   /* Check if enough source data retrieved to do the calculation -
      Required number of source values only applies if this
      specification is given and the calc is not partial */
   if ((sourceCount == 0) ||
       (required != VALUE_NULL &&
         !strcmp(resultRecord.derivationFlag, EMPTY_STRING) &&
         sourceCount < required))
   {
      /* Log that not enough source values */
      fprintf(LOGFILE, " Not enough source values (%d) with SDI = %d and interval = %s to derive a maximum value to interval = %s, SDI = %d, start time = %s and end time = %s.\n", sourceCount, updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);

      /* Derivation can't be performed - delete anything in destination slot */
      if ((result = SqlDeleteDestination(destInterval,
                                         refDestination->destSDI,
                                         timeRecord-> begInterval,
                                         timeRecord-> endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
         return (ERROR);
      }

      /* Write record to r_interval_update to propagate derivation upward */
      if ((result = SqlIntervalUpdate(destInterval,
                                      refDestination->destSDI,
                                      timeRecord->begInterval,
                                      timeRecord->endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
         return (ERROR);
      }

      /* Free any memory allocated for sourceList */
      if (sourceList != NULL)
      {
         free(sourceList);
         sourceList = NULL;
      }

      return(OK);      
   }

   /* Check if the desired number of source values were retrieved -
      Only applies if this specification is given and the
      calc is not partial */
   if (desired != VALUE_NULL &&
        !strcmp(resultRecord.derivationFlag, EMPTY_STRING) && 
        sourceCount < desired)
   {
      /* Assign derivation flag for less than desired number of 
         source values */
      char s1[] = "n";
      strcat(resultRecord.derivationFlag, s1); 
   }

   /* Find the source data record with the maximum value */
   for (sourceIndex = 1; sourceIndex < sourceCount; sourceIndex++)
   {
      if (sourceList[sourceIndex].value > sourceList[maxRecord].value)
      {
         maxRecord = sourceIndex;
      }
   }

   /* Set values for all the columns in the result record */
   resultRecord.siteDatatypeId = refDestination->destSDI;
   strcpy(resultRecord.startDateTime, timeRecord->begInterval);
   strcpy(resultRecord.endDateTime, timeRecord->endInterval);
   resultRecord.value = sourceList[maxRecord].value;
   strcpy(resultRecord.dateTimeLoaded, DATETIMELOADED);
   resultRecord.methodId = 11;

   /* Check if unit conversion is necessary from source to destination */
   if (updateRecord->siteDatatypeId != refDestination->destSDI)
   {
      if ((result = SqlConvertUnit(destInterval,
                                   updateRecord,
                                   &resultRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem completing unit conversion for derivation\n");
         return (ERROR);
      }
      
   }

   /* Write results to the destination table */
   if ((result = SqlWriteResult(destInterval,
                                &resultRecord )) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing results of derivation to the destination table\n");
      return (ERROR);
   }

   /* Write record to r_interval_update to propagate derivation upward */
   if ((result = SqlIntervalUpdate(destInterval,
                                   resultRecord.siteDatatypeId,
                                   resultRecord.startDateTime,
                                   resultRecord.endDateTime)) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
      return (ERROR);
   }

   /* Free memory allocated for sourceList */
   if (sourceList != NULL)
   {
      free(sourceList);
      sourceList = NULL;
   }

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "MethodTwavelint"

int MethodTwavelint(SQL_INTERVAL destInterval, 
                      UPDATE_RECORD* updateRecord,
                      INTERVAL_TIME* timeRecord,
                      REF_DESTINATION* refDestination,
                      DEST_PARAM* destParameters)
{
   /* This function controls calculating time-weighted averages with
      linear interpolation from the source data into the destination table.
      Results are calculated for a triggering destination interval and 
      potentially the two adjacent destination intervals */

   char selectStatement[600];
   DATA_RECORD resultRecord;
   DATA_RECORD lastResultRecord;
   DATA_RECORD nextResultRecord;
   int destinationCount;
   int result;
   int valid;
   INTERVAL_TIME lastTimeRecord;
   INTERVAL_TIME nextTimeRecord;

   /* In all cases, the previous destination interval needs to be
      reevaluated because its result could have been impacted */

   /* Calculate time record for previous interval */
   if ((result = SqlTimeShift(timeRecord,
                              destInterval,
                              SUBTRACTION,
                              &lastTimeRecord)) != OK)
   {
      FilePrintError(LOGFILE, "Problem shifting to previous time record\n");
      return (ERROR);
   }

   /* Check to see if the previous interval has an overwrite value.
      If so, we do not want to replace it, so skip rest of previous
      interval code. */
   sprintf(selectStatement, "SELECT COUNT(*) FROM r_%s WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND overwrite_flag IN ('O', 'o')", destInterval, refDestination->destSDI, lastTimeRecord.begInterval, lastTimeRecord.endInterval);

   if ((result = SqlSelectCount(selectStatement, &destinationCount)) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying for overwrite value in previous destination slot\n");
      return (ERROR);
   }

   if (destinationCount < 1)
   {
      /* Means there is not an overwrite value in destination slot */

      /* Check for partial calculation for previous interval. The previous
         interval should never really be a partial calc, but this call will
         initialize the partial calc flag appropriately.*/
      if ((result = PartialCalc(refDestination,
                                &lastTimeRecord,
                                &lastResultRecord,
                                &valid )) != OK)
      {
         FilePrintError(LOGFILE, "Problem checking for partial calculation in previous time interval\n");
         return (ERROR);
      }
      /* If the previous interval is partial, means something is messed up and 
         perhaps future data is coming into the database, so terminate the
         Twavelint calc. */ 
      if (valid == 0 ||
          !strcmp(lastResultRecord.derivationFlag, "p"))
      {
         /* Calculation should not go forward */
         return (OK);
      }
      /* Calculate Twavelint for previous interval */
      if ((result = TwavelintCalc(destInterval,
                                  updateRecord,
                                  &lastTimeRecord,
                                  &lastResultRecord,
                                  refDestination,
                                  destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem calculating time-weighted average with linear interpolation for previous interval\n");
         return (ERROR);
      }
   }   

   /* This section evaluates the triggering destination interval */

   /* Check triggering interval for partial calculation */
   if ((result = PartialCalc(refDestination, timeRecord, &resultRecord,
                                &valid )) != OK)
   {
      FilePrintError(LOGFILE, "Problem checking for partial calculation in triggering interval\n");
      return (ERROR);
   }

   if (valid == 0)
   {
      /* In this case where the triggering interval is partial, but no partial
         calc is called for in the spec, neither the triggering or the next
         interval should get calculated */
      return (OK);
   }

   /* In all cases where the triggering interval check was valid, the 
      triggering interval should get calculated */
   if ((result = TwavelintCalc(destInterval,
                               updateRecord,
                               timeRecord,
                               &resultRecord,
                               refDestination,
                               destParameters)) != OK)
   {
      FilePrintError(LOGFILE, "Problem calculating time-weighted average with linear interpolation for triggering interval\n");
      return (ERROR);
   }

   /* This section evaluates the next destination interval after the
      triggering interval */

   /* The only case where the next interval should get calculated is if the
      triggering interval is not a partial calc */
   if (strchr(resultRecord.derivationFlag, 'p' ) == NULL)
   {
      /* Calculate time record for the next interval */
      if ((result = SqlTimeShift(timeRecord,
                                 destInterval,
                                 ADDITION,
                                 &nextTimeRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem shifting to next time record\n");
         return (ERROR);
      }

      /* Check to see if the next interval has an overwrite value.
         If so, we do not want to replace it, so skip rest of next
         interval code. */
      sprintf(selectStatement, "SELECT COUNT(*) FROM r_%s WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND overwrite_flag IN ('O', 'o')", destInterval, refDestination->destSDI, nextTimeRecord.begInterval, nextTimeRecord.endInterval);

      if ((result = SqlSelectCount(selectStatement, &destinationCount)) != OK)
      {
         FilePrintError(LOGFILE, "Problem querying for overwrite value in next destination slot\n");
         return (ERROR);
      }

      if (destinationCount < 1)
      {
         /* Means there is not an overwrite value in destination slot */

         /* Check for partial calculation for next interval */
         if ((result = PartialCalc(refDestination,
                                   &nextTimeRecord,
                                   &nextResultRecord,
                                   &valid )) != OK)
         {
            FilePrintError(LOGFILE, "Problem checking for partial calculation in next time interval\n");
            return (ERROR);
         }

         if (valid == 0)
         {
            /* Calculation for next interval should not go forward based on
               partial calc check */
            return (OK);
         }

         /* Calculate Twavelint for next interval */
         if ((result = TwavelintCalc(destInterval,
                                     updateRecord,
                                     &nextTimeRecord,
                                     &nextResultRecord,
                                     refDestination,
                                     destParameters)) != OK)
         {
            FilePrintError(LOGFILE, "Problem calculating time-weighted average with linear interpolation for next interval\n");
            return (ERROR);
         }
      }         
   }   

   return(OK);
}

/****************************************************************************/
#define FUNC_NAME "TwavelintCalc"

int TwavelintCalc(SQL_INTERVAL destInterval, 
                  UPDATE_RECORD* updateRecord,
                  INTERVAL_TIME* timeRecord,
                  DATA_RECORD* resultRecord,
                  REF_DESTINATION* refDestination,
                  DEST_PARAM* destParameters)
{
   /* This function calculates the time-weighted average using linear
      interpolation for an interval */

   DATA_RECORD* lastSourceList = NULL;
   DATA_RECORD* nextSourceList = NULL;
   DATA_RECORD* sourceList = NULL;
   DATA_RECORD* twaData;
   double area = 0;
   double timeDelta;
   double totalTimeDelta;
   int compare;
   int compare2;
   int desired;
   int endInterp = 0;
   int lastSourceCount;
   int nextSourceCount;
   int required;
   int result;
   int sourceCount;
   int sourceIndex;
   int startInterp = 0;
   int twaCount;
   int twaIndex = 0;
   INTERVAL_TIME lastTimeRecord;
   INTERVAL_TIME nextTimeRecord;

   /* Retrieve the source data for the interval */
   if ((result = SqlSelectData(updateRecord,
                               timeRecord->begSource,
                               timeRecord->endSource,
                               &sourceCount,
                               &sourceList )) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying source data for derivation\n");
      return (ERROR);
   }

   /* Check if any of the "number of source value" specs are negative */
   if ((destParameters->requiredNumSource != VALUE_NULL && 
        destParameters->requiredNumSource <= 0) ||
       (destParameters->desiredNumSource != VALUE_NULL &&
        destParameters->desiredNumSource <= 0))
   {
      if ((result = NegNumSpec(destInterval,
                               timeRecord,
                               destParameters,
                               &desired,
                               &required )) != OK)
      {
         FilePrintError(LOGFILE, "Problem evaluating a negative number of source values spec\n");
         return (ERROR);
      }      
   }
   else
   {
      desired = destParameters->desiredNumSource;
      required = destParameters->requiredNumSource;
   }


   /* Check if enough source data retrieved to do the calculation -
      required number of source values only applies if this specification
      is given and the calc is not partial*/
   if ((sourceCount == 0) ||
       (required != VALUE_NULL &&
         !strcmp(resultRecord->derivationFlag, EMPTY_STRING) &&
         sourceCount < required))
   {
      /* Log that not enough source values */
      fprintf(LOGFILE, " Not enough source values (%d) with SDI = %d and interval = %s to derive a time weighted average using linear interpolation to interval = %s, SDI = %d, start time = %s and end time = %s.\n", sourceCount, updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);

      /* Derivation can't be performed - delete anything in destination slot */
      if ((result = SqlDeleteDestination(destInterval,
                                         refDestination->destSDI,
                                         timeRecord-> begInterval,
                                         timeRecord-> endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
         return (ERROR);
      }

      /* Write record to r_interval_update to propagate derivation upward */
      if ((result = SqlIntervalUpdate(destInterval,
                                      refDestination->destSDI,
                                      timeRecord->begInterval,
                                      timeRecord->endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
         return (ERROR);
      }

      /* Free any memory allocated for sourceList */
      if (sourceList != NULL)
      {
         free(sourceList);
         sourceList = NULL;
      }

      return(OK);      
   }
   /* Check if the desired number of source values were retrieved -
      Only applies if this specification is given and the calc
      is not partial */
   if (desired != VALUE_NULL &&
        !strcmp(resultRecord->derivationFlag, EMPTY_STRING) && 
        sourceCount < desired)
   {
      /* Assign derivation flag for less than desired number of 
         source values */
      char s1[] = "n";
      strcat(resultRecord->derivationFlag, s1); 
   }

   /* Check for start interpolation */

   /* If start date of first source value is not equal to the start date of 
      the source window for the destination interval, then start date of
      source window for the destination interval must be interpolated
      utilizing the last data point in the previous destination 
      interval's source window */
   if ((result = SqlDateCompare(sourceList[0].startDateTime,
                                timeRecord->begSource,
                                &compare)) != OK)
   {
      FilePrintError(LOGFILE, "Problem comparing dates for start time of first source data value and start of destination source window.\n");
      return (ERROR);
   }
   if (compare == 1)
   {
      /* Start of first data point is after the start of destination source
         window, so need to interpolate */

      /* Need to get the time record for the previous destination interval */
      if ((result = SqlTimeShift(timeRecord,
                                 destInterval,
                                 SUBTRACTION,
                                 &lastTimeRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem shifting to previous time record\n");
         return (ERROR);
      }

      /* Retrieve source values in previous interval */
      if ((result = SqlSelectData(updateRecord,
                                  lastTimeRecord.begInterval,
                                  lastTimeRecord.endInterval,
                                  &lastSourceCount,
                                  &lastSourceList )) != OK)
      {
         FilePrintError(LOGFILE, "Problem querying source data for previous destination interval\n");
         return (ERROR);
      }

      if (lastSourceCount == 0)
      {
         /* Time-weighted average with linear interpolation can't be
            calculated because there is no data in the previous destination
            interval */

         /* Log that no value in previous destination interval */
         fprintf(LOGFILE, " No value with SDI = %d and interval = %s within the previous destination interval to allow deriving a time weighted average using linear interpolation for interval = %s, SDI = %d, start time = %s and end time = %s.\n", updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);


         /* Delete anything in destination slot */
         if ((result = SqlDeleteDestination(destInterval,
                                            refDestination->destSDI,
                                            timeRecord->begInterval,
                                            timeRecord->endInterval)) != OK)
         {
            FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
            return (ERROR);
         }

         /* Write record to r_interval_update to propagate derivation upward */
         if ((result = SqlIntervalUpdate(destInterval,
                                         refDestination->destSDI,
                                         timeRecord->begInterval,
                                         timeRecord->endInterval)) != OK)
         {
            FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
            return (ERROR);
         }

         /* Free any memory allocated for the sourceList */
         if (sourceList != NULL)
         {
            free(sourceList);
            sourceList = NULL;
         }

         return(OK);    
      }

      /* Start interpolation is required */
      startInterp = 1;
   }

   /* Check for end interpolation */

   /* If the result record is marked as partial at this time, meaning
      that the derivation run time is within the current destination interval,
      then an end interpolation will never be required */
   if (!strcmp(resultRecord->derivationFlag, EMPTY_STRING))
   {
      /* Result is not marked partial, so end interpolation may be
         required */

      /* If end date of last source value is not equal to the end date of 
         the source window for the destination interval, then end date of
         source window for the destination interval must be interpolated
         utilizing the first data point in the next destination 
         interval's source window */
      if ((result = SqlDateCompare(timeRecord->endSource,
                                   sourceList[sourceCount - 1].endDateTime,
                                   &compare)) != OK)
      {
         FilePrintError(LOGFILE, "Problem comparing dates for end of destination source window and end time of last source data value.\n");
         return (ERROR);
      }

      if (compare == 1)
      {
         /* End of destination source window is after the end of last source
            data point, so need to interpolate */

         /* Need to get the time record for the next destination interval */
         if ((result = SqlTimeShift(timeRecord,
                                    destInterval,
                                    ADDITION,
                                    &nextTimeRecord)) != OK)
         {
            FilePrintError(LOGFILE, "Problem shifting to next time record\n");
            return (ERROR);
         }

         /* Retrieve source values in next interval */
         if ((result = SqlSelectData(updateRecord,
                                     nextTimeRecord.begSource,
                                     nextTimeRecord.endSource,
                                     &nextSourceCount,
                                     &nextSourceList )) != OK)
         {
            FilePrintError(LOGFILE, "Problem querying source data for next destination interval\n");
            return (ERROR);
         }

         /* The only time an end interpolation can be done is if there is
            at least one data point in the next destination interval */
         if (nextSourceCount != 0)
         {
            endInterp = 1;
         }
         /* The next destination interval has no data */
         else
         {
            /* The only case where the TWA calc can go forward from here is 
               when a partial calc is allowed according to the spec and the
               current derivation run time has not exceeded the end of the
               next destination interval source window */
               
            /* Find status of current derivation run time */
            if ((result = SqlDateCompare(nextTimeRecord.endSource,
                                         DATETIMELOADED,
                                         &compare2)) != OK)
            {
               FilePrintError(LOGFILE, "Problem comparing dates for the end time of next destination source window and the derivation run time.\n");
               return (ERROR);
            }

            /* Check for the case where the calc can go forward */
            if ( (compare2 == 1) &&
                 (refDestination->partialCalc == 'y' || 
                  refDestination->partialCalc == 'Y')
               )
            {
               /* TWA calc can go forward as a partial calc, so set the
                  derivation flag to indicate this */
               sprintf(resultRecord->derivationFlag, "p");
            }
            /* Terminate the TWA calc */
            else
            {
               /* Log that no value in following destination interval */
               fprintf(LOGFILE, " No value with SDI = %d and interval = %s within the following destination interval to allow deriving a time weighted average using linear interpolation for interval = %s, SDI = %d, start time = %s and end time = %s.\n", updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);

               /* Delete anything in destination slot */
               if ((result = SqlDeleteDestination(destInterval,
                                                  refDestination->destSDI,
                                                  timeRecord->begInterval,
                                                  timeRecord->endInterval))
                                                  != OK)
               {
                  FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
                  return (ERROR);
               }

               /* Write record to r_interval_update to propagate derivation
                  upward */
               if ((result = SqlIntervalUpdate(destInterval,
                                               refDestination->destSDI,
                                               timeRecord->begInterval,
                                               timeRecord->endInterval)) != OK)
               {
                  FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
                  return (ERROR);
               }

               /* Free any memory allocated for the source lists */
               if (sourceList != NULL)
               {
                  free(sourceList);
                  sourceList = NULL;
               }
               if (lastSourceList != NULL)
               {
                  free(lastSourceList);
                  lastSourceList = NULL;
               }

               return(OK);    
            }
         }
      }
   }

   /* Allocate memory for the twaData array */
   twaCount = startInterp + (2 * sourceCount) + endInterp;
   if ((twaData = (DATA_RECORD *) malloc 
      (sizeof(DATA_RECORD) * twaCount))
       == NULL)
   {
      FilePrintError(LOGFILE, "Problem allocating memory for twaData array\n");
      return (ERROR);
   }

   /* If applicable, interpolate start value and add to twaData array */
   if (startInterp == 1)
   {
      /* Assign the time members in the interpolated data record - 
         interpolated value is instantaneous, so start and end
         times are the same */
      strcpy (twaData[twaIndex].startDateTime, timeRecord->begSource);
      strcpy (twaData[twaIndex].endDateTime, timeRecord->begSource);

      /* Interpolate a value for the interpolated record */
      if ((result = InterpCalc(&lastSourceList[lastSourceCount-1],
                               &sourceList[0],
                               &twaData[twaIndex])) != OK)
      {
         FilePrintError(LOGFILE, "Problem interpolating start value\n");
         return (ERROR);
      }

      /* Increment twaData array index */
      twaIndex++;
   }

   /* Add the source data for the current interval to the twaData array. 
      Both the startDateTime and endDateTime for a source data record
      will get an entry into the twaData array so that instantaneous and
      interval source data will be handled correctly in the subsequent
      time weighting calculations */
   for (sourceIndex = 0; sourceIndex < sourceCount; sourceIndex++)
   {
      /* Assign startDateTime values to twaData array record */
      strcpy (twaData[twaIndex].startDateTime,
              sourceList[sourceIndex].startDateTime);
      strcpy (twaData[twaIndex].endDateTime,
              sourceList[sourceIndex].startDateTime);
      twaData[twaIndex].value = sourceList[sourceIndex].value;

      /* Increment twaData array index */
      twaIndex++;

      /* Assign endDateTime values to twaData array record */
      strcpy (twaData[twaIndex].startDateTime,
              sourceList[sourceIndex].endDateTime);
      strcpy (twaData[twaIndex].endDateTime,
              sourceList[sourceIndex].endDateTime);
      twaData[twaIndex].value = sourceList[sourceIndex].value;

      /* Increment twaData array index */
      twaIndex++;
   }

   /* If applicable, interpolate end value and add to twaData array */
   if (endInterp == 1)
   {
      /* Assign the time members in the interpolated data record - 
         interpolated value is instantaneous, so start and end
         times are the same */
      strcpy (twaData[twaIndex].startDateTime, timeRecord->endSource);
      strcpy (twaData[twaIndex].endDateTime, timeRecord->endSource);

      /* Interpolate a value for the interpolated record */
      if ((result = InterpCalc(&sourceList[sourceCount-1],
                               &nextSourceList[0],
                               &twaData[twaIndex])) != OK)
      {
         FilePrintError(LOGFILE, "Problem interpolating end value\n");
         return (ERROR);
      }

      /* Increment twaData array index */
      twaIndex++;
   }

   /* Check twaData array indexing */
   if (twaCount != twaIndex)
   {
      FilePrintError(LOGFILE, "Problem with twaData array indexing\n");
      return (ERROR);
   }

   /* Time-weighted average is calculated as the area under the curve
      defined by the data points in the twaCalc array divided by the 
      time difference between the first and last points in this array */

   /* Find area under the curve */
   for ( twaIndex = 1; twaIndex < twaCount; twaIndex ++)
   {
      /* Get the time difference between the current and the previous
         array member */
      if ((result = SqlDateSubtract(twaData[twaIndex].startDateTime,
                                    twaData[twaIndex-1].startDateTime,
                                    &timeDelta)) != OK)
      {
         FilePrintError(LOGFILE, "Problem subtracting times for twaCalc array members\n");
         return (ERROR);
      }

      /* Add in the area between these adjacent array members */
      area = area + (timeDelta *
             ((twaData[twaIndex].value + twaData[twaIndex-1].value) / 2));
   }

   /* Get time difference between first and last points in twaCalc array */
   if ((result = SqlDateSubtract(twaData[twaCount-1].startDateTime,
                                 twaData[0].startDateTime,
                                 &totalTimeDelta)) != OK)
   {
      FilePrintError(LOGFILE, "Problem subtracting times for first and last twaCalc array members\n");
      return (ERROR);
   }

   /* Calculate the time_weighted average and assign to result record */
   resultRecord->value = area / totalTimeDelta;

   /* Set value for the remaining columns in result record */
   resultRecord->siteDatatypeId = refDestination->destSDI;
   strcpy(resultRecord->startDateTime, timeRecord->begInterval);
   strcpy(resultRecord->endDateTime, timeRecord->endInterval);
   strcpy(resultRecord->dateTimeLoaded, DATETIMELOADED);
   resultRecord->methodId = 3;

   /* Check if unit conversion is necessary from source to destination */
   if (updateRecord->siteDatatypeId != refDestination->destSDI)
   {
      if ((result = SqlConvertUnit(destInterval,
                                   updateRecord,
                                   resultRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem completing unit conversion for derivation\n");
         return (ERROR);
      }
      
   }

   /* Write results to the destination table */
   if ((result = SqlWriteResult(destInterval,
                                resultRecord )) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing results of derivation to the destination table\n");
      return (ERROR);
   }

   /* Write record to r_interval_update to propagate derivation upward */
   if ((result = SqlIntervalUpdate(destInterval,
                                   resultRecord->siteDatatypeId,
                                   resultRecord->startDateTime,
                                   resultRecord->endDateTime)) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
      return (ERROR);
   }

   /* Free memory allocated for source lists */
   if (sourceList != NULL)
   {
      free(sourceList);
      sourceList = NULL;
   }
   if (lastSourceList != NULL)
   {
      free(lastSourceList);
      lastSourceList = NULL;
   }
   if (nextSourceList != NULL)
   {
      free(nextSourceList);
      nextSourceList = NULL;
   }

   return(OK);
}
/****************************************************************************/
#define FUNC_NAME "MethodEop"

int MethodEop(SQL_INTERVAL destInterval, 
                 UPDATE_RECORD* updateRecord,
                 INTERVAL_TIME* timeRecord,
                 REF_DESTINATION* refDestination,
                 DEST_PARAM* destParameters)
{
   /* This function writes the last or end of period value from the source
      data into the destination table */

   DATA_RECORD resultRecord;
   DATA_RECORD* sourceList;
   int compare;
   int result;
   int sourceCount;
   int valid;
   SQL_DATE begWindow;
   SQL_DATE desiredWindow;

   /* Check for partial calculation */
   if ((result = PartialCalc(refDestination, timeRecord, &resultRecord,
                                &valid )) != OK)
   {
      FilePrintError(LOGFILE, "Problem checking for partial calculation\n");
      return (ERROR);
   }

   if (valid == 0)
   {
      /* Calculation should not go forward based on partial calc check */
      return (OK);
   }

   /* Check if there is a required eop window specification */
   if (destParameters->requiredEopWindow != VALUE_NULL)
   {
      /* Calculate beginning of the required time window for eop data
         selection */
      if ((result = SqlDateMath(SUBTRACTION,
                                timeRecord->endSource,
                                begWindow, 
                                destParameters->requiredEopWindow,
                                destParameters->windowUnit)) != OK)
      {
         FilePrintError(LOGFILE, "Problem subtracting times for required Eop window");
         return (ERROR);
      }
   }
   else
   {
      /* Beginning for data select is beginning of interval's source data
         window */
      strcpy (begWindow, timeRecord->begSource);
   }

   /* Retrieve the source data */
   if ((result = SqlSelectData(updateRecord,
                               begWindow,
                               timeRecord->endSource,
                               &sourceCount,
                               &sourceList )) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying source data for derivation\n");
      return (ERROR);
   }

   /* Check if source data retrieved to do the calculation */
   if (sourceCount == 0)
   {
      /* Log that no end of period value */
      fprintf(LOGFILE, " No value with SDI = %d and interval = %s to derive an end of period value to interval = %s, SDI = %d, start time = %s and end time = %s.\n", updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);

      /* Derivation can't be performed - delete anything in destination slot */
      if ((result = SqlDeleteDestination(destInterval,
                                         refDestination->destSDI,
                                         timeRecord-> begInterval,
                                         timeRecord-> endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
         return (ERROR);
      }

      /* Write record to r_interval_update to propagate derivation upward */
      if ((result = SqlIntervalUpdate(destInterval,
                                      refDestination->destSDI,
                                      timeRecord->begInterval,
                                      timeRecord->endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
         return (ERROR);
      }

      /* Free any memory allocated for sourceList */
      if (sourceList != NULL)
      {
         free(sourceList);
         sourceList = NULL;
      }

      return(OK);      
   }

   /* Check for desired eop window specification */
   if (destParameters->desiredEopWindow != VALUE_NULL)
   {
      /* Calculate beginning of the desired time window for eop data
         selection */
      if ((result = SqlDateMath(SUBTRACTION,
                                timeRecord->endSource,
                                desiredWindow, 
                                destParameters->desiredEopWindow,
                                destParameters->windowUnit)) != OK)
      {
         FilePrintError(LOGFILE, "Problem subtracting for desired Eop window");
         return (ERROR);
      }

      /* If the desired window time is later than the last source value's
         start date, then assign flag indicating result is
         outside of desired window */
      if ((result = SqlDateCompare(desiredWindow,
                                   sourceList[sourceCount-1].startDateTime,
                                   &compare)) != OK)
      {
         FilePrintError(LOGFILE, "Problem comparing date of beginning of desired eop window and the end of period value's start date\n");
         return (ERROR);
      }
      if (compare == 1)
      {
         /* Desired window is later than the eop value's start time, so
            assign the "w" flag for value outside desired window time */
         char s1[] = "w";
         strcat(resultRecord.derivationFlag, s1);      
      }
   }

   /* Set values for all the columns in the result record */
   resultRecord.siteDatatypeId = refDestination->destSDI;
   strcpy(resultRecord.startDateTime, timeRecord->begInterval);
   strcpy(resultRecord.endDateTime, timeRecord->endInterval);
   resultRecord.value = sourceList[sourceCount-1].value;
   strcpy(resultRecord.dateTimeLoaded, DATETIMELOADED);
   resultRecord.methodId = 7;

   /* Check if unit conversion is necessary from source to destination */
   if (updateRecord->siteDatatypeId != refDestination->destSDI)
   {
      if ((result = SqlConvertUnit(destInterval,
                                   updateRecord,
                                   &resultRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem completing unit conversion for derivation\n");
         return (ERROR);
      }
      
   }

   /* Write results to the destination table */
   if ((result = SqlWriteResult(destInterval,
                                &resultRecord )) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing results of derivation to the destination table\n");
      return (ERROR);
   }

   /* Write record to r_interval_update to propagate derivation upward */
   if ((result = SqlIntervalUpdate(destInterval,
                                   resultRecord.siteDatatypeId,
                                   resultRecord.startDateTime,
                                   resultRecord.endDateTime)) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
      return (ERROR);
   }

   /* Free memory allocated for sourceList */
   if (sourceList != NULL)
   {
      free(sourceList);
      sourceList = NULL;
   }


   return(OK);
}

/****************************************************************************/
#define FUNC_NAME "MethodTot"

int MethodTot(SQL_INTERVAL destInterval, 
                 UPDATE_RECORD* updateRecord,
                 INTERVAL_TIME* timeRecord,
                 REF_DESTINATION* refDestination,
                 DEST_PARAM* destParameters)
{
   /* This function writes the total of the source data values into 
      the destination table */

   DATA_RECORD resultRecord;
   DATA_RECORD* sourceList;
   double total = 0;
   int desired;
   int required;
   int result;
   int sourceCount;
   int sourceIndex;
   int valid;

   /* Check for partial calculation */
   if ((result = PartialCalc(refDestination, timeRecord, &resultRecord,
                                &valid )) != OK)
   {
      FilePrintError(LOGFILE, "Problem checking for partial calculation\n");
      return (ERROR);
   }

   if (valid == 0)
   {
      /* Calculation should not go forward based on partial calc check */
      return (OK);
   }

   /* Retrieve the source data */
   if ((result = SqlSelectData(updateRecord,
                               timeRecord->begSource,
                               timeRecord->endSource,
                               &sourceCount,
                               &sourceList )) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying source data for derivation\n");
      return (ERROR);
   }

   /* Check if any of the "number of source value" specs are negative */
   if ((destParameters->requiredNumSource != VALUE_NULL && 
        destParameters->requiredNumSource <= 0) ||
       (destParameters->desiredNumSource != VALUE_NULL &&
        destParameters->desiredNumSource <= 0))
   {
      if ((result = NegNumSpec(destInterval,
                               timeRecord,
                               destParameters,
                               &desired,
                               &required )) != OK)
      {
         FilePrintError(LOGFILE, "Problem evaluating a negative number of source values spec\n");
         return (ERROR);
      }      
   }
   else
   {
      desired = destParameters->desiredNumSource;
      required = destParameters->requiredNumSource;
   }


   /* Check if enough source data retrieved to do the calculation -
      Required number of source values only applies if this
      specification is given and the calc is not partial */
   if ((sourceCount == 0) ||
       (required != VALUE_NULL &&
         !strcmp(resultRecord.derivationFlag, EMPTY_STRING) &&
         sourceCount < required))
   {
      /* Log that not enough source values */
      fprintf(LOGFILE, " Not enough source values (%d) with SDI = %d and interval = %s to derive a sum over time value to interval = %s, SDI = %d, start time = %s and end time = %s.\n", sourceCount, updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);

      /* Derivation can't be performed - delete anything in destination slot */
      if ((result = SqlDeleteDestination(destInterval,
                                         refDestination->destSDI,
                                         timeRecord-> begInterval,
                                         timeRecord-> endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
         return (ERROR);
      }

      /* Write record to r_interval_update to propagate derivation upward */
      if ((result = SqlIntervalUpdate(destInterval,
                                      refDestination->destSDI,
                                      timeRecord->begInterval,
                                      timeRecord->endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
         return (ERROR);
      }

      /* Free any memory allocated for sourceList */
      if (sourceList != NULL)
      {
         free(sourceList);
         sourceList = NULL;
      }

      return(OK);      
   }

   /* Check if the desired number of source values were retrieved -
      Only applies if this specification is given and the
      calc is not partial */
   if (desired != VALUE_NULL &&
        !strcmp(resultRecord.derivationFlag, EMPTY_STRING) && 
        sourceCount < desired)
   {
      /* Assign derivation flag for less than desired number of 
         source values */
      char s1[] = "n";
      strcat(resultRecord.derivationFlag, s1); 
   }

   /* Total the source data values */
   for (sourceIndex = 0; sourceIndex < sourceCount; sourceIndex++)
   {
      total += sourceList[sourceIndex].value;
   }

   /* Set values for remaining columns in the result record */
   resultRecord.siteDatatypeId = refDestination->destSDI;
   strcpy(resultRecord.startDateTime, timeRecord->begInterval);
   strcpy(resultRecord.endDateTime, timeRecord->endInterval);
   resultRecord.value = total;
   strcpy(resultRecord.dateTimeLoaded, DATETIMELOADED);
   resultRecord.methodId = 17;

   /* Check if unit conversion is necessary from source to destination */
   if (updateRecord->siteDatatypeId != refDestination->destSDI)
   {
      if ((result = SqlConvertUnit(destInterval,
                                   updateRecord,
                                   &resultRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem completing unit conversion for derivation\n");
         return (ERROR);
      }
      
   }

   /* Write results to the destination table */
   if ((result = SqlWriteResult(destInterval,
                                &resultRecord )) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing results of derivation to the destination table\n");
      return (ERROR);
   }

   /* Write record to r_interval_update to propagate derivation upward */
   if ((result = SqlIntervalUpdate(destInterval,
                                   resultRecord.siteDatatypeId,
                                   resultRecord.startDateTime,
                                   resultRecord.endDateTime)) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
      return (ERROR);
   }

   /* Free memory allocated for sourceList */
   if (sourceList != NULL)
   {
      free(sourceList);
      sourceList = NULL;
   }

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "MethodAve"

int MethodAve(SQL_INTERVAL destInterval, 
                 UPDATE_RECORD* updateRecord,
                 INTERVAL_TIME* timeRecord,
                 REF_DESTINATION* refDestination,
                 DEST_PARAM* destParameters)
{
   /* This function writes the arithmetic average of the source data values 
      into the destination table */

   DATA_RECORD resultRecord;
   DATA_RECORD* sourceList;
   double total = 0;
   int desired;
   int required;
   int result;
   int sourceCount;
   int sourceIndex;
   int valid;

   /* Check for partial calculation */
   if ((result = PartialCalc(refDestination, timeRecord, &resultRecord,
                                &valid )) != OK)
   {
      FilePrintError(LOGFILE, "Problem checking for partial calculation\n");
      return (ERROR);
   }

   if (valid == 0)
   {
      /* Calculation should not go forward based on partial calc check */
      return (OK);
   }

   /* Retrieve the source data */
   if ((result = SqlSelectData(updateRecord,
                               timeRecord->begSource,
                               timeRecord->endSource,
                               &sourceCount,
                               &sourceList )) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying source data for derivation\n");
      return (ERROR);
   }

   /* Check if any of the "number of source value" specs are negative */
   if ((destParameters->requiredNumSource != VALUE_NULL && 
        destParameters->requiredNumSource <= 0) ||
       (destParameters->desiredNumSource != VALUE_NULL &&
        destParameters->desiredNumSource <= 0))
   {
      if ((result = NegNumSpec(destInterval,
                               timeRecord,
                               destParameters,
                               &desired,
                               &required )) != OK)
      {
         FilePrintError(LOGFILE, "Problem evaluating a negative number of source values spec\n");
         return (ERROR);
      }      
   }
   else
   {
      desired = destParameters->desiredNumSource;
      required = destParameters->requiredNumSource;
   }


   /* Check if enough source data retrieved to do the calculation -
      Required number of source values only applies if this
      specification is given and the calc is not partial */
   if ((sourceCount == 0) ||
       (required != VALUE_NULL &&
         !strcmp(resultRecord.derivationFlag, EMPTY_STRING) &&
         sourceCount < required))
   {
      /* Log that not enough source values */
      fprintf(LOGFILE, " Not enough source values (%d) with SDI = %d and interval = %s to derive an average value to interval = %s, SDI = %d, start time = %s and end time = %s.\n", sourceCount, updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);

      /* Derivation can't be performed - delete anything in destination slot */
      if ((result = SqlDeleteDestination(destInterval,
                                         refDestination->destSDI,
                                         timeRecord-> begInterval,
                                         timeRecord-> endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
         return (ERROR);
      }

      /* Write record to r_interval_update to propagate derivation upward */
      if ((result = SqlIntervalUpdate(destInterval,
                                      refDestination->destSDI,
                                      timeRecord->begInterval,
                                      timeRecord->endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
         return (ERROR);
      }

      /* Free any memory allocated for sourceList */
      if (sourceList != NULL)
      {
         free(sourceList);
         sourceList = NULL;
      }

      return(OK);      
   }

   /* Check if the desired number of source values were retrieved -
      Only applies if this specification is given and the
      calc is not partial */
   if (desired != VALUE_NULL &&
        !strcmp(resultRecord.derivationFlag, EMPTY_STRING) && 
        sourceCount < desired)
   {
      /* Assign derivation flag for less than desired number of 
         source values */
      char s1[] = "n";
      strcat(resultRecord.derivationFlag, s1); 
   }

   /* Total the source data values */
   for (sourceIndex = 0; sourceIndex < sourceCount; sourceIndex++)
   {
      total += sourceList[sourceIndex].value;
   }

   /* Calculate the average and assign to the result record */
   resultRecord.value = total / sourceCount;   

   /* Set values for remaining columns in the result record */
   resultRecord.siteDatatypeId = refDestination->destSDI;
   strcpy(resultRecord.startDateTime, timeRecord->begInterval);
   strcpy(resultRecord.endDateTime, timeRecord->endInterval);
   strcpy(resultRecord.dateTimeLoaded, DATETIMELOADED);
   resultRecord.methodId = 2;

   /* Check if unit conversion is necessary from source to destination */
   if (updateRecord->siteDatatypeId != refDestination->destSDI)
   {
      if ((result = SqlConvertUnit(destInterval,
                                   updateRecord,
                                   &resultRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem completing unit conversion for derivation\n");
         return (ERROR);
      }
      
   }

   /* Write results to the destination table */
   if ((result = SqlWriteResult(destInterval,
                                &resultRecord )) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing results of derivation to the destination table\n");
      return (ERROR);
   }

   /* Write record to r_interval_update to propagate derivation upward */
   if ((result = SqlIntervalUpdate(destInterval,
                                   resultRecord.siteDatatypeId,
                                   resultRecord.startDateTime,
                                   resultRecord.endDateTime)) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
      return (ERROR);
   }

   /* Free memory allocated for sourceList */
   if (sourceList != NULL)
   {
      free(sourceList);
      sourceList = NULL;
   }

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "MethodMin"

int MethodMin(SQL_INTERVAL destInterval, 
                 UPDATE_RECORD* updateRecord,
                 INTERVAL_TIME* timeRecord,
                 REF_DESTINATION* refDestination,
                 DEST_PARAM* destParameters)
{
   /* This function writes the minimum value from the source data into 
      the destination table */

   DATA_RECORD resultRecord;
   DATA_RECORD* sourceList;
   int desired;
   int minRecord = 0;
   int required;
   int result;
   int sourceCount;
   int sourceIndex;
   int valid;

   /* Check for partial calculation */
   if ((result = PartialCalc(refDestination, timeRecord, &resultRecord,
                                &valid )) != OK)
   {
      FilePrintError(LOGFILE, "Problem checking for partial calculation\n");
      return (ERROR);
   }

   if (valid == 0)
   {
      /* Calculation should not go forward based on partial calc check */
      return (OK);
   }

   /* Retrieve the source data */
   if ((result = SqlSelectData(updateRecord,
                               timeRecord->begSource,
                               timeRecord->endSource,
                               &sourceCount,
                               &sourceList )) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying source data for derivation\n");
      return (ERROR);
   }

   /* Check if any of the "number of source value" specs are negative */
   if ((destParameters->requiredNumSource != VALUE_NULL && 
        destParameters->requiredNumSource <= 0) ||
       (destParameters->desiredNumSource != VALUE_NULL &&
        destParameters->desiredNumSource <= 0))
   {
      if ((result = NegNumSpec(destInterval,
                               timeRecord,
                               destParameters,
                               &desired,
                               &required )) != OK)
      {
         FilePrintError(LOGFILE, "Problem evaluating a negative number of source values spec\n");
         return (ERROR);
      }      
   }
   else
   {
      desired = destParameters->desiredNumSource;
      required = destParameters->requiredNumSource;
   }

   /* Check if enough source data retrieved to do the calculation -
      Required number of source values only applies if this
      specification is given and the calc is not partial */
   if ((sourceCount == 0) ||
       (required != VALUE_NULL &&
         !strcmp(resultRecord.derivationFlag, EMPTY_STRING) &&
         sourceCount < required))
   {
      /* Log that not enough source values */
      fprintf(LOGFILE, " Not enough source values (%d) with SDI = %d and interval = %s to derive a minimum value to interval = %s, SDI = %d, start time = %s and end time = %s.\n", sourceCount, updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);

      /* Derivation can't be performed - delete anything in destination slot */
      if ((result = SqlDeleteDestination(destInterval,
                                         refDestination->destSDI,
                                         timeRecord-> begInterval,
                                         timeRecord-> endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
         return (ERROR);
      }

      /* Write record to r_interval_update to propagate derivation upward */
      if ((result = SqlIntervalUpdate(destInterval,
                                      refDestination->destSDI,
                                      timeRecord->begInterval,
                                      timeRecord->endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
         return (ERROR);
      }

      /* Free any memory allocated for sourceList */
      if (sourceList != NULL)
      {
         free(sourceList);
         sourceList = NULL;
      }

      return(OK);      
   }

   /* Check if the desired number of source values were retrieved -
      Only applies if this specification is given and the
      calc is not partial */
   if (desired != VALUE_NULL &&
        !strcmp(resultRecord.derivationFlag, EMPTY_STRING) && 
        sourceCount < desired)
   {
      /* Assign derivation flag for less than desired number of 
         source values */
      char s1[] = "n";
      strcat(resultRecord.derivationFlag, s1); 
   }

   /* Find the source data record with the minimum value */
   for (sourceIndex = 1; sourceIndex < sourceCount; sourceIndex++)
   {
      if (sourceList[sourceIndex].value < sourceList[minRecord].value)
      {
         minRecord = sourceIndex;
      }
   }

   /* Set values for all the columns in the result record */
   resultRecord.siteDatatypeId = refDestination->destSDI;
   strcpy(resultRecord.startDateTime, timeRecord->begInterval);
   strcpy(resultRecord.endDateTime, timeRecord->endInterval);
   resultRecord.value = sourceList[minRecord].value;
   strcpy(resultRecord.dateTimeLoaded, DATETIMELOADED);
   resultRecord.methodId = 12;

   /* Check if unit conversion is necessary from source to destination */
   if (updateRecord->siteDatatypeId != refDestination->destSDI)
   {
      if ((result = SqlConvertUnit(destInterval,
                                   updateRecord,
                                   &resultRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem completing unit conversion for derivation\n");
         return (ERROR);
      }
      
   }

   /* Write results to the destination table */
   if ((result = SqlWriteResult(destInterval,
                                &resultRecord )) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing results of derivation to the destination table\n");
      return (ERROR);
   }

   /* Write record to r_interval_update to propagate derivation upward */
   if ((result = SqlIntervalUpdate(destInterval,
                                   resultRecord.siteDatatypeId,
                                   resultRecord.startDateTime,
                                   resultRecord.endDateTime)) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
      return (ERROR);
   }

   /* Free memory allocated for sourceList */
   if (sourceList != NULL)
   {
      free(sourceList);
      sourceList = NULL;
   }

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "MethodBop"

int MethodBop(SQL_INTERVAL destInterval, 
                 UPDATE_RECORD* updateRecord,
                 INTERVAL_TIME* timeRecord,
                 REF_DESTINATION* refDestination,
                 DEST_PARAM* destParameters)
{
   /* This function writes the first or "beginning of period" value from the
      source data into the destination table */

   DATA_RECORD resultRecord;
   DATA_RECORD* sourceList;
   int compare;
   int result;
   int sourceCount;
   int valid;
   SQL_DATE endWindow;
   SQL_DATE desiredWindow;

   /* Check for partial calculation */
   if ((result = PartialCalc(refDestination, timeRecord, &resultRecord,
                                &valid )) != OK)
   {
      FilePrintError(LOGFILE, "Problem checking for partial calculation\n");
      return (ERROR);
   }

   if (valid == 0)
   {
      /* Calculation should not go forward based on partial calc check */
      return (OK);
   }

   /* Check if there is a required bop window specification */
   if (destParameters->requiredBopWindow != VALUE_NULL)
   {
      /* Calculate end of the required time window for bop data
         selection */
      if ((result = SqlDateMath(ADDITION,
                                timeRecord->begSource,
                                endWindow, 
                                destParameters->requiredBopWindow,
                                destParameters->windowUnit)) != OK)
      {
         FilePrintError(LOGFILE, "Problem adding times for required Bop window");
         return (ERROR);
      }
   }
   else
   {
      /* End time for data select is end of interval's source data
         window */
      strcpy (endWindow, timeRecord->endSource);
   }

   /* Retrieve the source data */
   if ((result = SqlSelectData(updateRecord,
                               timeRecord->begSource,
                               endWindow,
                               &sourceCount,
                               &sourceList )) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying source data for derivation\n");
      return (ERROR);
   }

   /* Check if source data retrieved to do the calculation */
   if (sourceCount == 0)
   {
      /* Log that no beginning of period value */
      fprintf(LOGFILE, " No value with SDI = %d and interval = %s to derive a beginning of period value to interval = %s, SDI = %d, start time = %s and end time = %s.\n", updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);

      /* Derivation can't be performed - delete anything in destination slot */
      if ((result = SqlDeleteDestination(destInterval,
                                         refDestination->destSDI,
                                         timeRecord-> begInterval,
                                         timeRecord-> endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
         return (ERROR);
      }

      /* Write record to r_interval_update to propagate derivation upward */
      if ((result = SqlIntervalUpdate(destInterval,
                                      refDestination->destSDI,
                                      timeRecord->begInterval,
                                      timeRecord->endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
         return (ERROR);
      }

      /* Free any memory allocated for sourceList */
      if (sourceList != NULL)
      {
         free(sourceList);
         sourceList = NULL;
      }

      return(OK);      
   }

   /* Check for desired bop window specification */
   if (destParameters->desiredBopWindow != VALUE_NULL)
   {
      /* Calculate end time of the desired time window for bop data
         selection */
      if ((result = SqlDateMath(ADDITION,
                                timeRecord->begSource,
                                desiredWindow, 
                                destParameters->desiredBopWindow,
                                destParameters->windowUnit)) != OK)
      {
         FilePrintError(LOGFILE, "Problem adding times for desired Bop window");
         return (ERROR);
      }

      /* If the first source values start date time is equal to or later than
         the desired window time, assign flag indicating result is
         outside of desired window */
      if ((result = SqlDateCompare(sourceList[0].startDateTime,
                                   desiredWindow,
                                   &compare)) != OK)
      {
         FilePrintError(LOGFILE, "Problem comparing beginning of period value's start date with the end of desired bop window\n");
         return (ERROR);
      }
      if (compare >= 0)
      {
         /* First source value's start time is not less than the 
            desired window time, so assign the "w" flag for value
            outside desired window time */
         char s1[] = "w";
         strcat(resultRecord.derivationFlag, s1);      
      }
   }

   /* Set values for all the columns in the result record */
   resultRecord.siteDatatypeId = refDestination->destSDI;
   strcpy(resultRecord.startDateTime, timeRecord->begInterval);
   strcpy(resultRecord.endDateTime, timeRecord->endInterval);
   resultRecord.value = sourceList[0].value;
   strcpy(resultRecord.dateTimeLoaded, DATETIMELOADED);
   resultRecord.methodId = 4;

   /* Check if unit conversion is necessary from source to destination */
   if (updateRecord->siteDatatypeId != refDestination->destSDI)
   {
      if ((result = SqlConvertUnit(destInterval,
                                   updateRecord,
                                   &resultRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem completing unit conversion for derivation\n");
         return (ERROR);
      }
      
   }

   /* Write results to the destination table */
   if ((result = SqlWriteResult(destInterval,
                                &resultRecord )) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing results of derivation to the destination table\n");
      return (ERROR);
   }

   /* Write record to r_interval_update to propagate derivation upward */
   if ((result = SqlIntervalUpdate(destInterval,
                                   resultRecord.siteDatatypeId,
                                   resultRecord.startDateTime,
                                   resultRecord.endDateTime)) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
      return (ERROR);
   }

   /* Free memory allocated for sourceList */
   if (sourceList != NULL)
   {
      free(sourceList);
      sourceList = NULL;
   }


   return(OK);
}

/****************************************************************************/
#define FUNC_NAME "MethodTovol"

int MethodTovol(SQL_INTERVAL destInterval, 
                 UPDATE_RECORD* updateRecord,
                 INTERVAL_TIME* timeRecord,
                 REF_DESTINATION* refDestination,
                 DEST_PARAM* destParameters)
{
   /* This function figures the volume from the source data and derives it 
      to the destination table */

   DATA_RECORD resultRecord;
   DATA_RECORD* sourceList;
   double total = 0;
   int desired;
   int required;
   int result;
   int sourceCount;
   int sourceIndex;
   int valid;

   /* Check for partial calculation */
   if ((result = PartialCalc(refDestination, timeRecord, &resultRecord,
                                &valid )) != OK)
   {
      FilePrintError(LOGFILE, "Problem checking for partial calculation\n");
      return (ERROR);
   }

   if (valid == 0)
   {
      /* Calculation should not go forward based on partial calc check */
      return (OK);
   }

   /* Retrieve the source data */
   if ((result = SqlSelectData(updateRecord,
                               timeRecord->begSource,
                               timeRecord->endSource,
                               &sourceCount,
                               &sourceList )) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying source data for derivation\n");
      return (ERROR);
   }

   /* Check if any of the "number of source value" specs are negative */
   if ((destParameters->requiredNumSource != VALUE_NULL && 
        destParameters->requiredNumSource <= 0) ||
       (destParameters->desiredNumSource != VALUE_NULL &&
        destParameters->desiredNumSource <= 0))
   {
      if ((result = NegNumSpec(destInterval,
                               timeRecord,
                               destParameters,
                               &desired,
                               &required )) != OK)
      {
         FilePrintError(LOGFILE, "Problem evaluating a negative number of source values spec\n");
         return (ERROR);
      }      
   }
   else
   {
      desired = destParameters->desiredNumSource;
      required = destParameters->requiredNumSource;
   }

   /* Check if enough source data retrieved to do the calculation -
      Required number of source values only applies if this
      specification is given and the calc is not partial */
   if ((sourceCount == 0) ||
       (required != VALUE_NULL &&
         !strcmp(resultRecord.derivationFlag, EMPTY_STRING) &&
         sourceCount < required))
   {
      /* Log that not enough source values */
      fprintf(LOGFILE, " Not enough source values (%d) with SDI = %d and interval = %s to derive a to volume value to interval = %s, SDI = %d, start time = %s and end time = %s.\n", sourceCount, updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);

      /* Derivation can't be performed - delete anything in destination slot */
      if ((result = SqlDeleteDestination(destInterval,
                                         refDestination->destSDI,
                                         timeRecord-> begInterval,
                                         timeRecord-> endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
         return (ERROR);
      }

      /* Write record to r_interval_update to propagate derivation upward */
      if ((result = SqlIntervalUpdate(destInterval,
                                      refDestination->destSDI,
                                      timeRecord->begInterval,
                                      timeRecord->endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
         return (ERROR);
      }

      /* Free any memory allocated for sourceList */
      if (sourceList != NULL)
      {
         free(sourceList);
         sourceList = NULL;
      }

      return(OK);      
   }

   /* Check if the desired number of source values were retrieved -
      Only applies if this specification is given and the
      calc is not partial */
   if (desired != VALUE_NULL &&
        !strcmp(resultRecord.derivationFlag, EMPTY_STRING) && 
        sourceCount < desired)
   {
      /* Assign derivation flag for less than desired number of 
         source values */
      char s1[] = "n";
      strcat(resultRecord.derivationFlag, s1); 
   }

   /* Total the source data values */
   for (sourceIndex = 0; sourceIndex < sourceCount; sourceIndex++)
   {
      total += sourceList[sourceIndex].value;
   }

   /* Set values for all the columns in the result record */
   resultRecord.siteDatatypeId = refDestination->destSDI;
   strcpy(resultRecord.startDateTime, timeRecord->begInterval);
   strcpy(resultRecord.endDateTime, timeRecord->endInterval);
   resultRecord.value = total;
   strcpy(resultRecord.dateTimeLoaded, DATETIMELOADED);
   resultRecord.methodId = 9;

   /* If the datatype changes from source to destination, then this is the
      first step where flow is converted to volume */
   if (updateRecord->siteDatatypeId != refDestination->destSDI)
   {
      /* For conversion, flow must be average flow over the source interval,
         so we need to divide the result value by the number of values */
      resultRecord.value = resultRecord.value / sourceCount;

      /* Then call the unit converter to do the flow to volume conversion */
      if ((result = SqlConvertUnit(destInterval,
                                   updateRecord,
                                   &resultRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem completing unit conversion for derivation\n");
         return (ERROR);
      }
   }
   /* The else case represents subsequent steps where we are just totaling
      the flows from the lower interval to the destination interval, so the
      result value as figured above is already correct */

   /* Write results to the destination table */
   if ((result = SqlWriteResult(destInterval,
                                &resultRecord )) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing results of derivation to the destination table\n");
      return (ERROR);
   }

   /* Write record to r_interval_update to propagate derivation upward */
   if ((result = SqlIntervalUpdate(destInterval,
                                   resultRecord.siteDatatypeId,
                                   resultRecord.startDateTime,
                                   resultRecord.endDateTime)) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
      return (ERROR);
   }

   /* Free memory allocated for sourceList */
   if (sourceList != NULL)
   {
      free(sourceList);
      sourceList = NULL;
   }

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "MethodEoplint"

int MethodEoplint(SQL_INTERVAL destInterval, 
                      UPDATE_RECORD* updateRecord,
                      INTERVAL_TIME* timeRecord,
                      REF_DESTINATION* refDestination,
                      DEST_PARAM* destParameters)
{
   /* This function controls calculating linearly interpolated end
      of period values from the source data into the destination table.
      Results are calculated for the triggering destination interval and 
      the previous destination interval */
 
   char selectStatement[600];
   DATA_RECORD resultRecord;
   DATA_RECORD lastResultRecord;
   int destinationCount;
   int result;
   int valid;
   INTERVAL_TIME lastTimeRecord;

   /* In all cases, the previous destination interval needs to be
      reevaluated because its result could have been impacted by a change
      in the current interval */

   /* Calculate time record for previous interval */
   if ((result = SqlTimeShift(timeRecord,
                              destInterval,
                              SUBTRACTION,
                              &lastTimeRecord)) != OK)
   {
      FilePrintError(LOGFILE, "Problem shifting to previous time record\n");
      return (ERROR);
   }

   /* Check to see if the previous interval has an overwrite value.
      If so, we do not want to replace it, so skip rest of previous
      interval code. */
   sprintf(selectStatement, "SELECT COUNT(*) FROM r_%s WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND overwrite_flag IN ('O', 'o')", destInterval, refDestination->destSDI, lastTimeRecord.begInterval, lastTimeRecord.endInterval);

   if ((result = SqlSelectCount(selectStatement, &destinationCount)) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying for overwrite value in previous destination slot\n");
      return (ERROR);
   }

   if (destinationCount < 1)
   {
      /* Means there is not an overwrite value in destination slot */

      /* Check for partial calculation for previous interval. The previous
         interval should never really be a partial calc, but this call will
         initialize the partial calc flag appropriately.*/
      if ((result = PartialCalc(refDestination,
                                &lastTimeRecord,
                                &lastResultRecord,
                                &valid )) != OK)
      {
         FilePrintError(LOGFILE, "Problem checking for partial calculation in previous time interval\n");
         return (ERROR);
      }
      /* If the previous interval is partial, means something is messed up and 
         perhaps future data is coming into the database, so terminate the
         Eoplint calc. */ 
      if (valid == 0 ||
          !strcmp(lastResultRecord.derivationFlag, "p"))
      {
         /* Calculation should not go forward */
         return (OK);
      }
      /* Calculate Eoplint  for previous interval */
      if ((result = EoplintCalc(destInterval,
                                  updateRecord,
                                  &lastTimeRecord,
                                  &lastResultRecord,
                                  refDestination,
                                  destParameters)) != OK)

      {
         FilePrintError(LOGFILE, "Problem calculating linearly interpolated end of period value for the previous interval\n");
         return (ERROR);
      }
   }   

   /* This section evaluates the triggering destination interval */

   /* Check triggering interval for partial calculation */
   if ((result = PartialCalc(refDestination,
                                timeRecord,
                                &resultRecord,
                                &valid )) != OK)
   {
      FilePrintError(LOGFILE, "Problem checking for partial calculation in triggering interval\n");
      return (ERROR);
   }
   /* If the triggering interval is partial (whether the spec calls for it
      or not), Eoplint can't be calculated because there would be no value
      in the following interval to interpolate to */
   if (valid == 0 ||
       !strcmp(resultRecord.derivationFlag, "p"))
   {
      return (OK);
   }

   /* If the triggering interval is not partial, proceed with calculation */ 
   if ((result = EoplintCalc(destInterval,
                               updateRecord,
                               timeRecord,
                               &resultRecord,
                               refDestination,
                               destParameters)) != OK)

   {
      FilePrintError(LOGFILE, "Problem calculating linearly interpolated end of period value for the triggering interval\n");
      return (ERROR);
   }

   return(OK);
}

/****************************************************************************/
#define FUNC_NAME "EoplintCalc"

int EoplintCalc(SQL_INTERVAL destInterval, 
                  UPDATE_RECORD* updateRecord,
                  INTERVAL_TIME* timeRecord,
                  DATA_RECORD* resultRecord,
                  REF_DESTINATION* refDestination,
                  DEST_PARAM* destParameters)
{
   /* This function calculates an interpolated end of period value
      for an interval using linear interpolation  */

   DATA_RECORD interpRecord;
   DATA_RECORD* nextSourceList = NULL;
   DATA_RECORD* sourceList;
   int compare;
   int nextSourceCount;
   int result;
   int sourceCount;
   INTERVAL_TIME nextTimeRecord;
   SQL_DATE begWindow;
   SQL_DATE desiredWindow;

   /* Check if there is a required eop window specification */
   if (destParameters->requiredEopWindow != VALUE_NULL)
   {
      /* Calculate beginning of the required time window for eop data
         selection */
      if ((result = SqlDateMath(SUBTRACTION,
                                timeRecord->endSource,
                                begWindow, 
                                destParameters->requiredEopWindow,
                                destParameters->windowUnit)) != OK)
      {
         FilePrintError(LOGFILE, "Problem subtracting times for required Eop window");
         return (ERROR);
      }
   }
   else
   {
      /* Beginning for data select is beginning of interval's source data
         window */
      strcpy (begWindow, timeRecord->begSource);
   }

   /* Retrieve the source data */
   if ((result = SqlSelectData(updateRecord,
                               begWindow,
                               timeRecord->endSource,
                               &sourceCount,
                               &sourceList )) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying source data for derivation\n");
      return (ERROR);
   }

   /* Check if source data retrieved to do the calculation */
   if (sourceCount == 0)
   {
      /* Log that no end of period value within destination interval*/
      fprintf(LOGFILE, " No value with SDI = %d and interval = %s within the destination interval to allow derivation of an end of period value using linear interpolation for the interval = %s, SDI = %d, start time = %s and end time = %s.\n", updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);


      /* Derivation can't be performed - delete anything in destination slot */
      if ((result = SqlDeleteDestination(destInterval,
                                         refDestination->destSDI,
                                         timeRecord-> begInterval,
                                         timeRecord-> endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
         return (ERROR);
      }

      /* Write record to r_interval_update to propagate derivation upward */
      if ((result = SqlIntervalUpdate(destInterval,
                                      refDestination->destSDI,
                                      timeRecord->begInterval,
                                      timeRecord->endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
         return (ERROR);
      }

      /* Free any memory allocated for sourceList */
      if (sourceList != NULL)
      {
         free(sourceList);
         sourceList = NULL;
      }

      return(OK);      
   }

   /* Check for desired eop window specification */
   if (destParameters->desiredEopWindow != VALUE_NULL)
   {
      /* Calculate beginning of the desired time window for eop data
         selection */
      if ((result = SqlDateMath(SUBTRACTION,
                                timeRecord->endSource,
                                desiredWindow, 
                                destParameters->desiredEopWindow,
                                destParameters->windowUnit)) != OK)
      {
         FilePrintError(LOGFILE, "Problem subtracting for desired Eop window");
         return (ERROR);
      }

      /* If the desired window time is later than the last source value's
         start date, then assign flag indicating result is
         outside of desired window */
      if ((result = SqlDateCompare(desiredWindow,
                                   sourceList[sourceCount-1].startDateTime,
                                   &compare)) != OK)
      {
         FilePrintError(LOGFILE, "Problem comparing date of beginning of desired eop window and the end of period value's start date\n");
         return (ERROR);
      }
      if (compare == 1)
      {
         /* Desired window is later than the eop value's start time, so
            assign the "w" flag for value outside desired window time */
         char s1[] = "w";
         strcat(resultRecord->derivationFlag, s1);      
      }
   }

   /* Need to get the time record for the next destination interval */
   if ((result = SqlTimeShift(timeRecord,
                              destInterval,
                              ADDITION,
                              &nextTimeRecord)) != OK)
   {
      FilePrintError(LOGFILE, "Problem shifting to next time record\n");
      return (ERROR);
   }

   /* Retrieve source values in next interval */
   if ((result = SqlSelectData(updateRecord,
                               nextTimeRecord.begSource,
                               nextTimeRecord.endSource,
                               &nextSourceCount,
                               &nextSourceList )) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying source data for next destination interval\n");
      return (ERROR);
   }

   /* The only time an end interpolation can be done is if there is
      at least one data point in the next destination interval */
   if (nextSourceCount == 0)
   {
      /* Log that no end of period value in next destination interval*/
      fprintf(LOGFILE, " No value with SDI = %d and interval = %s within the following destination interval to allow linear interpolation of an end of period value for interval = %s, SDI = %d, start time = %s and end time = %s.\n", updateRecord->siteDatatypeId, updateRecord->interval, destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);


      /* Delete anything in destination slot */
      if ((result = SqlDeleteDestination(destInterval,
                                         refDestination->destSDI,
                                         timeRecord->begInterval,
                                         timeRecord->endInterval))
                                         != OK)
      {
         FilePrintError(LOGFILE, "Problem deleting data from destination table\n");
         return (ERROR);
      }

      /* Write record to r_interval_update to propagate derivation
         upward */
      if ((result = SqlIntervalUpdate(destInterval,
                                      refDestination->destSDI,
                                      timeRecord->begInterval,
                                      timeRecord->endInterval)) != OK)
      {
         FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
         return (ERROR);
      }

      /* Free any memory allocated for the source lists */
      if (sourceList != NULL)
      {
         free(sourceList);
         sourceList = NULL;
      }
      if (nextSourceList != NULL)
      {
         free(nextSourceList);
         nextSourceList = NULL;
      }

      return(OK);    
   }

   /* Set the interpolation time as back one second from the end of
      the interval, because the end time is not in the interval but
      is actually the start of the next interval */
   if ((result = SqlDateMath(SUBTRACTION,
                                timeRecord->endInterval,
                                interpRecord.endDateTime, 
                                1,
                                SECOND)) != OK)
   {
      FilePrintError(LOGFILE, "Problem subtracting to get interpolated time\n");
      return (ERROR);
   }

   /* Assign the start time in the interpolated data record - 
      interpolated value is instantaneous, so start and end
      times are the same */
   strcpy (interpRecord.startDateTime, interpRecord.endDateTime);

   /* Interpolate the end of period value */
   if ((result = InterpCalc(&sourceList[sourceCount-1],
                            &nextSourceList[0],
                            &interpRecord)) != OK)
   {
      FilePrintError(LOGFILE, "Problem interpolating end of period value\n");
      return (ERROR);
   }
   
   /* Set values for the columns in the result record */
   resultRecord->value = interpRecord.value;
   resultRecord->siteDatatypeId = refDestination->destSDI;
   strcpy(resultRecord->startDateTime, timeRecord->begInterval);
   strcpy(resultRecord->endDateTime, timeRecord->endInterval);
   strcpy(resultRecord->dateTimeLoaded, DATETIMELOADED);
   resultRecord->methodId = 8;

   /* Check if unit conversion is necessary from source to destination */
   if (updateRecord->siteDatatypeId != refDestination->destSDI)
   {
      if ((result = SqlConvertUnit(destInterval,
                                   updateRecord,
                                   resultRecord)) != OK)
      {
         FilePrintError(LOGFILE, "Problem completing unit conversion for derivation\n");
         return (ERROR);
      }
      
   }

   /* Write results to the destination table */
   if ((result = SqlWriteResult(destInterval,
                                resultRecord )) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing results of derivation to the destination table\n");
      return (ERROR);
   }

   /* Write record to r_interval_update to propagate derivation upward */
   if ((result = SqlIntervalUpdate(destInterval,
                                   resultRecord->siteDatatypeId,
                                   resultRecord->startDateTime,
                                   resultRecord->endDateTime)) != OK)
   {
      FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
      return (ERROR);
   }

   /* Free memory allocated for source lists */
   if (sourceList != NULL)
   {
      free(sourceList);
      sourceList = NULL;
   }
   if (nextSourceList != NULL)
   {
      free(nextSourceList);
      nextSourceList = NULL;
   }

   return(OK);
}
/****************************************************************************/
#define FUNC_NAME "PartialCalc"

int PartialCalc(REF_DESTINATION* refDestination,
                   INTERVAL_TIME* timeRecord,
                   DATA_RECORD* resultRecord,
                   int* valid)
{
   /* This function determines if the derivation is partial and if the
      calc should proceed based on the derivation specification */

   int compare;
   int result;

   /* Check if derivation is partial */
   if ((result = SqlDateCompare(timeRecord->endSource,
                    DATETIMELOADED,
                    &compare)) != OK)
   {
      FilePrintError(LOGFILE, "Problem comparing dates for end of destination source window and time of derivation application run\n");
      return (ERROR);
   }
   if (compare == 1)
   {
      /* Source window extends past current derivation application run time,
         so derivation is partial */
      if (refDestination->partialCalc == 'y' || 
          refDestination->partialCalc == 'Y')
      {
         /* Partial calc is called for in the spec, so assign partial calc
            flag to what will be the resulting data record */
         sprintf(resultRecord->derivationFlag, "p");
         
         /* Calculation is valid */
         *valid = 1;
      }
      else
      {
         /* No partial calc is called for, so calculation is invalid*/
         *valid = 0;
      }
   }
   else
   {
      /* Derivation is not partial - flag is empty */
      strcpy (resultRecord->derivationFlag, EMPTY_STRING);

      /* Calculation is valid */
      *valid = 1;
   }

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "InterpCalc"

int InterpCalc(DATA_RECORD* firstRecord,
               DATA_RECORD* lastRecord,
               DATA_RECORD* interpRecord)
{
   /* This function interpolates a value for a data record between two given
      data records. All three data records must have date/times; the first and
      last data records must have values. This function will
      calculate an interpolated value for the result data record */

   double timeNumerator;
   double timeDenominator;
   int result;
   
   /* Find the difference in time between the startDateTime of the last
      record and the endDateTime of the first record */
   if ((result = SqlDateSubtract(lastRecord->startDateTime,
                                 firstRecord->endDateTime,
                                 &timeDenominator)) != OK)
   {
      FilePrintError(LOGFILE, "Problem finding the timeDenominator\n");
      return (ERROR);
   } 

   /* If denominator is zero, means first record and last record are
      adjacent intervals and the interpolated time is actually the last
      instant in the first interval. In this case, the interpolated value
      should be equal to the first record's value. */
   if (timeDenominator == 0)
   {
      interpRecord->value = firstRecord->value;
      return (OK);
   }

  /* Find the difference in time between the startDateTime of the
      interpolated record and the endDateTime of the first record */
   if ((result = SqlDateSubtract(interpRecord->startDateTime,
                                 firstRecord->endDateTime,
                                 &timeNumerator)) != OK)
   {
      FilePrintError(LOGFILE, "Problem finding the timeNumerator\n");
      return (ERROR);
   }

   /* Interpolate the value for the interpolated record */
   interpRecord->value = firstRecord->value +
                         ((lastRecord->value - firstRecord->value) *
                          (timeNumerator/timeDenominator)
                         );

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "NegNumSpec"

int NegNumSpec(SQL_INTERVAL destInterval,
               INTERVAL_TIME* timeRecord,
               DEST_PARAM* destParameters,
               int* desired,
               int* required)
{
   /* This function is called if one of the specs in ref_derivation_destination
      for the number of values is negative (or 0).  This should only happen for
      a spec corresponding to month and has the meaning of number of missing
      values for the month.  This is needed because month is the only interval 
      that has an unequal number of the previous intervals (days) to be used
      as source values. */

   int daysBetween = 0;
   int result;

   /* Negative "number of values" specs are only legitimate for derivations to 
      month */
   if (strcmp(destInterval, "month"))
   {
      FilePrintError(LOGFILE, "Required and desired number of source value specifications in ref_derivation_destination cannot be negative for the interval %s\n", destInterval);
      return (ERROR);      
   }
   
   /* If the required number of source values is negative, find the number of
      days in the month (1 less than the return from days between) and add
      the negative value in the spec to get the  return value of required
      number of day values to do the monthly derivation */
   if (destParameters->requiredNumSource != VALUE_NULL && 
       destParameters->requiredNumSource <= 0)
   {
      if ((result = SqlNumDaysBetweenDates(
                                 timeRecord->begInterval,
                                 timeRecord->endInterval,
                                 &daysBetween)) != OK)
      {
         FilePrintError(LOGFILE, "Problem finding number of days in destination month\n");
         return (ERROR);
      }

      *required = (daysBetween - 1) + destParameters->requiredNumSource;
   }
   /* If spec is not negative, return is the positive number of days in the 
      spec */
   else
   {
      *required = destParameters->requiredNumSource;
   }

   /* If the desired number of source values is negative, find the number of
      days in the month (if not done above) and add the negative value in the
      spec to get the return value of desired number of day values to do the
      monthly derivation */
   if (destParameters->desiredNumSource != VALUE_NULL && 
       destParameters->desiredNumSource <= 0)
   {
      if (daysBetween == 0)
      {
         if ((result = SqlNumDaysBetweenDates(
                                    timeRecord->begInterval,
                                    timeRecord->endInterval,
                                    &daysBetween)) != OK)
         {
            FilePrintError(LOGFILE, "Problem finding number of days in destination month\n");
            return (ERROR);
         }
      }

      *desired = (daysBetween - 1) + destParameters->desiredNumSource;
   }
   /* If spec is not negative, return is the positive number of days in the 
      spec */
   else
   {
      *desired = destParameters->desiredNumSource;
   }

   return (OK);
}
