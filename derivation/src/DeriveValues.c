/*****************************************************************************
* DESCRIPTION
*
* This file contains the functions that control deriving identified values 
* upward to the identified destination interval 
*
***************************************
* DeriveList
*
* Purpose: Derives values to the destination interval based on an array of 
*          update records indicating a change in the source data.  Calls 
*          DeriveValue() for individual derivations from the array. 
*
* Input:   SQL_INTERVAL - Name of the destination interval for the derivation
*          UPDATE_RECORD** - Pointer to array of update records that 
*                            initiate the derivations
*          int - Number of records in the update array
*          REF_SOURCE* - Pointer to a ref_derivation_source specification
*          REF_DESTINATION* - Pointer to a ref_derivation_destination
*                             specification     
*
* Output:  None
*      
* Return:  int - OK or ERROR
*
***************************************
* DeriveValue
*
* Purpose: Derives a value to the destination interval.  
*
* Input:   SQL_INTERVAL - Name of the destination interval for the derivation
*          UPDATE_RECORD* - Pointer to an update record
*          INTERVAL_TIME* - Pointer to an interval time structure
*          REF_DESTINATION* - Pointer to a ref_derivation_destination
*                             specification
*          DEST_PARAM* - Pointer to the relevant destination parameters    
*
* Output:  None
*      
* Return:  int - OK or ERROR
*
***************************************
* GetDestParam
*
* Purpose: Records the parameters from the ref_destination data
*          structure relevant to a set of derivations 
*
* Input:   SQL_INTERVAL - Name of the destination interval for the derivation
*          REF_DESTINATION* - Pointer to a ref_derivation_destination
*                             specification    
*
* Output:  DEST_PARAM* - Pointer to the relevant destination specifications
*      
* Return:  int - OK or ERROR
*
***************************************
*
* Author
*
* Neil Wilson
* October, 2001
*****************************************************************************/
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

#define FUNC_NAME "DeriveList"

int DeriveList(SQL_INTERVAL destInterval,
               UPDATE_RECORD* updateList,
               int updateCount,  
               REF_SOURCE* refSource,
               REF_DESTINATION* refDestination)
{
   /* Derive values from an array of data to a destination interval */

   DEST_PARAM destParameters;
   int compare;
   int dataOrd;
   int result;
   int updateIndex = 0;
   INTERVAL_TIME timeRecord;

   /* Get the destination parameters that will be used in deriving all
      values in the list */
   if ((result = GetDestParam(destInterval,
                              refDestination,
                              &destParameters)) != OK)
   {
      FilePrintError(LOGFILE, "Problem getting the destination parameters for the derivation\n");
      return (ERROR);
   }

   /* Get the interval order number for data in the list (all the same) */
   if (updateCount != 0)
   {
      if ((result = SqlGetIntOrder(updateList[0].interval, &dataOrd))
                    != OK)
      {
         FilePrintError(LOGFILE, "Problem getting interval order number for %s\n",
                     updateList[0].interval);
         return (ERROR);
      }
   }

   while (updateIndex < updateCount)
   {
      /* Get destination times for the record */
      if (dataOrd == 1)
      {
         /* If value is for a nonbusiness interval, pass ref_source structure
            to correct for any specified time offset */  
         result = SqlTimeInterval(&updateList[updateIndex], 
                                  destInterval,
                                  &timeRecord,
                                  refSource);
      }
      else
      {
         /* Pass NULL for ref_source pointer because no time offset correction
            needed (already has been corrected if data is in a business 
            interval)*/
         result = SqlTimeInterval(&updateList[updateIndex], 
                                  destInterval,
                                  &timeRecord,
                                  NULL);
      }
      if (result != OK)
      {
         FilePrintError(LOGFILE, "Problem calculating interval times for deriving update data\n");
         return (ERROR);
      }

      /* Derive the value to the destination interval */
      if ((result = DeriveValue(destInterval,
                                &updateList[updateIndex], 
                                &timeRecord,
                                refDestination,
                                &destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving value to destination interval\n");
         return (ERROR);
      }

      /* Index array and see if next member falls into the same interval that
         just got calculated and repeat until find one that did not - avoids 
         function calls to perform redundent calculations */
      do
      {
         updateIndex++;

         /* Compare end time of last destination source window to begin time 
            of update record */
         if (updateIndex < updateCount)
         {
            if ((result = SqlDateCompare(timeRecord.endSource,
                             updateList[updateIndex].startDateTime,
                             &compare)) != OK)
            {
               FilePrintError(LOGFILE, "Problem comparing dates for end of destination source window and start of update record\n");
               return (ERROR);
            }            
         }
      } while (updateIndex < updateCount && compare == 1);
   }

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "DeriveValue"

int DeriveValue(SQL_INTERVAL destInterval, UPDATE_RECORD* updateRecord,
                INTERVAL_TIME* timeRecord,
                REF_DESTINATION* refDestination,
                DEST_PARAM* destParameters)
{
   /* Derive a value to the destination interval */

   char addTruncateStatement[600];
   char selectStatement[600];
   int destinationCount;
   int order;
   int result;


   /* If the destination slot contains an overwrite value, then
      the derivation should not continue */
   sprintf(selectStatement, "SELECT COUNT(*) FROM r_%s WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND overwrite_flag IN ('O', 'o')", destInterval, refDestination->destSDI, timeRecord->begInterval, timeRecord->endInterval);

   if ((result = SqlSelectCount(selectStatement, &destinationCount)) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying for overwrite value in destination slot\n");
      return (ERROR);
   }

   if (destinationCount >= 1)
   {
      /* Means there is an overwrite value in destination slot, so
         end derivation and bail. */

      /* First verify that if the source interval is a business interval
         that the records here are marked as truncated. (There is a case
         if we have derived values in under on overwrite value where no
         values previously existed, then the new records would not be marked
         as truncated unless we do it here) */
      if ((result = SqlGetIntOrder(updateRecord->interval, &order))
                 != OK)
      {
         FilePrintError(LOGFILE, "Problem getting interval order number for %s\n",
                     updateRecord->interval);
         return (ERROR);
      }

      if (order > 1)
      {
         /* Interval is a business interval. */
         sprintf(addTruncateStatement, "UPDATE r_%s SET overwrite_flag = 'T' WHERE site_datatype_id = %d AND start_date_time >= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time <= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND (overwrite_flag NOT IN ('O', 'o') OR overwrite_flag IS NULL)", updateRecord->interval, updateRecord->siteDatatypeId, timeRecord->begSource, timeRecord->endSource );

         if ((result = SqlMethodTwo(addTruncateStatement)) != OK)
         {
            FilePrintError(LOGFILE, "Problem executing statement %s\n", 
                       addTruncateStatement);
            return (ERROR);
         }
      }

      return (OK);
   }


   /* Call the appropriate function based on the methodId */
   if (refDestination->methodId == 11)
   {
      if ((result = MethodMax(destInterval,
                                 updateRecord, 
                                 timeRecord,
                                 refDestination,
                                 destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving maximum value to destination interval\n");
         return (ERROR);
      }
   }
   else if (refDestination->methodId == 3)
   {
      if ((result = MethodTwavelint(destInterval,
                                    updateRecord, 
                                    timeRecord,
                                    refDestination,
                                    destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving time-weighted average value using linear interpolation to destination interval\n");
         return (ERROR);
      }
   } 
   else if (refDestination->methodId == 7)
   {
      if ((result = MethodEop(destInterval,
                                 updateRecord, 
                                 timeRecord,
                                 refDestination,
                                 destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving end of period value to destination interval\n");
         return (ERROR);
      }
   } 
   else if (refDestination->methodId == 17)
   {
      if ((result = MethodTot(destInterval,
                                 updateRecord, 
                                 timeRecord,
                                 refDestination,
                                 destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving sum-over-time value to destination interval\n");
         return (ERROR);
      }
   } 
   else if (refDestination->methodId == 2)
   {
      if ((result = MethodAve(destInterval,
                                 updateRecord, 
                                 timeRecord,
                                 refDestination,
                                 destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving average value to destination interval\n");
         return (ERROR);
      }
   } 
   else if (refDestination->methodId == 12)
   {
      if ((result = MethodMin(destInterval,
                                 updateRecord, 
                                 timeRecord,
                                 refDestination,
                                 destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving minimum value to destination interval\n");
         return (ERROR);
      }
   } 
   else if (refDestination->methodId == 4)
   {
      if ((result = MethodBop(destInterval,
                                 updateRecord, 
                                 timeRecord,
                                 refDestination,
                                 destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving beginning of period value to destination interval\n");
         return (ERROR);
      }
   } 
   else if (refDestination->methodId == 9)
   {
      if ((result = MethodTovol(destInterval,
                                 updateRecord, 
                                 timeRecord,
                                 refDestination,
                                 destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving flow to volume for the destination interval\n");
         return (ERROR);
      }
   } 
   else if (refDestination->methodId == 19)
   {
      if ((result = MethodTovol(destInterval,
                                 updateRecord, 
                                 timeRecord,
                                 refDestination,
                                 destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving volume to volume for the destination interval\n");
         return (ERROR);
      }
   } 
   else if (refDestination->methodId == 8)
   {
      if ((result = MethodEoplint(destInterval,
                                  updateRecord, 
                                  timeRecord,
                                  refDestination,
                                  destParameters)) != OK)
      {
         FilePrintError(LOGFILE, "Problem deriving linearly interpolated end of period value  for the destination interval\n");
         return (ERROR);
      }
   } 
   else
   {
      FilePrintError(LOGFILE, "No method code written in derivation app to calculate method_id = %d\n", 
                  refDestination->methodId);
      return (ERROR);      
   }
             

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "GetDestParam"

int GetDestParam(SQL_INTERVAL destInterval,
                 REF_DESTINATION* refDestination,
                 DEST_PARAM* destParameters)
{
   /* This function records the parameters from the ref_destination data
      structure relevant to a set of derivations */

   /* Get the parameters based on the destination interval */
   if (!strncmp(destInterval, "ho", 2))
   {
      destParameters->desiredEopWindow = refDestination->hrDesiredEopWindow;
      destParameters->requiredEopWindow = refDestination->hrRequiredEopWindow;
      destParameters->desiredBopWindow = refDestination->hrDesiredBopWindow;
      destParameters->requiredBopWindow = refDestination->hrRequiredBopWindow;
      strcpy(destParameters->windowUnit, refDestination->hrWindowUnit);
      destParameters->desiredNumSource = refDestination->hrDesiredNumSource;
      destParameters->requiredNumSource = refDestination->hrRequiredNumSource;
   }
   else if (!strncmp(destInterval, "da", 2))
   {
      destParameters->desiredEopWindow = refDestination->dayDesiredEopWindow;
      destParameters->requiredEopWindow = refDestination->dayRequiredEopWindow;
      destParameters->desiredBopWindow = refDestination->dayDesiredBopWindow;
      destParameters->requiredBopWindow = refDestination->dayRequiredBopWindow;
      strcpy(destParameters->windowUnit, refDestination->dayWindowUnit);
      destParameters->desiredNumSource = refDestination->dayDesiredNumSource;
      destParameters->requiredNumSource = refDestination->dayRequiredNumSource;
   } 
   else if (!strncmp(destInterval, "mo", 2))
   {
      destParameters->desiredEopWindow = refDestination->monDesiredEopWindow;
      destParameters->requiredEopWindow = refDestination->monRequiredEopWindow;
      destParameters->desiredBopWindow = refDestination->monDesiredBopWindow;
      destParameters->requiredBopWindow = refDestination->monRequiredBopWindow;
      strcpy(destParameters->windowUnit, refDestination->monWindowUnit);
      destParameters->desiredNumSource = refDestination->monDesiredNumSource;
      destParameters->requiredNumSource = refDestination->monRequiredNumSource;
   } 
   else if (!strncmp(destInterval, "ye", 2))
   {
      destParameters->desiredEopWindow = refDestination->yrDesiredEopWindow;
      destParameters->requiredEopWindow = refDestination->yrRequiredEopWindow;
      destParameters->desiredBopWindow = refDestination->yrDesiredBopWindow;
      destParameters->requiredBopWindow = refDestination->yrRequiredBopWindow;
      strcpy(destParameters->windowUnit, refDestination->yrWindowUnit);
      destParameters->desiredNumSource = refDestination->yrDesiredNumSource;
      destParameters->requiredNumSource = refDestination->yrRequiredNumSource;
   } 
   else if (!strncmp(destInterval, "wy", 2))
   {
      destParameters->desiredEopWindow = refDestination->wyDesiredEopWindow;
      destParameters->requiredEopWindow = refDestination->wyRequiredEopWindow;
      destParameters->desiredBopWindow = refDestination->wyDesiredBopWindow;
      destParameters->requiredBopWindow = refDestination->wyRequiredBopWindow;
      strcpy(destParameters->windowUnit, refDestination->wyWindowUnit);
      destParameters->desiredNumSource = refDestination->wyDesiredNumSource;
      destParameters->requiredNumSource = refDestination->wyRequiredNumSource;
   }
   else
   {
      FilePrintError(LOGFILE, "Code for the interval named %s needs to be written in\n"
                  "the function GetDestParam\n", 
                  destInterval);
      return (ERROR);      
   }

   return (OK);
}
