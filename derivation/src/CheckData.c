/*****************************************************************************
* DESCRIPTION
*
* This file contains the functions that check input data to the derivation
* application against user parameters that are defined in the derivation 
* application driver tables
*
***************************************
*
* CheckCopy
*
* Purpose: Checks data entered into r_base against the specifications
*          entered for that site_datatype_id in the ref_derivation_source
*          driver table.  The specifications checked are for minimum value
*          expected and maximum value expected, and for minimum value cutoff
*          and maximum value cutoff.  Function assigns derivation flag 
*          to the data record and assigns 0 or 1 to the valid 
*          parameter based on the checks performed
*
* Input:   REF_SOURCE* - pointer to the ref_derivation_source specification
*
* Input/Output:   DATA_RECORD* - pointer to the data record to be checked
*
* Output:  int* - pointer to a valid parameter that is assigned 0 for false
*                 and 1 for true
*
* Return:  int - OK or ERROR
*
***************************************
*
* Author
*
* Neil Wilson
* September, 2001
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

#define FUNC_NAME "CheckCopy"

extern FILE* LOGFILE;

int CheckCopy(DATA_RECORD* dataRecord, REF_SOURCE* refSource, int* valid )
{
   /* Checks magnitude of r_base data against user specifications */

   /* Assume value is valid */
   *valid = 1;

   /* Check if less than min value expected */
   if (refSource->minValueExpected != VALUE_NULL)
   {
      if (dataRecord->value < refSource->minValueExpected)
      {
         /* Assign derivation flag of "l" for low range exceedance */
         sprintf(dataRecord->derivationFlag, "l");
      }
   }

   /* Check if less than min value cutoff */
   if (refSource->minValueCutoff != VALUE_NULL)
   {
      if (dataRecord->value < refSource->minValueCutoff)
      {
         /* Value is invalid */
         *valid = 0;

         /* Log invalid value */
         fprintf(LOGFILE, " Value of %f in base area for SDI = %d is less than the minimum value cutoff of %f. This record for start time = %s and end time = %s not copied out of base area.\n", dataRecord->value, dataRecord->siteDatatypeId, refSource->minValueCutoff, dataRecord->startDateTime, dataRecord->endDateTime);
      }
   }

   /* Check if more than max value expected */
   if (refSource->maxValueExpected != VALUE_NULL)
   {
      if (dataRecord->value > refSource->maxValueExpected)
      {
         /* Assign derivation flag of "h" for high range exceedance */
         sprintf(dataRecord->derivationFlag, "h");
      }
   }

   /* Check if more than max value cutoff */
   if (refSource->maxValueCutoff != VALUE_NULL)
   {
      if (dataRecord->value > refSource->maxValueCutoff)
      {
         /* Value is invalid */
         *valid = 0;

         /* Log invalid value */
         fprintf(LOGFILE, " Value of %f in base area for SDI = %d is greater than the maximum value cutoff of %f. This record for start time = %s and end time = %s not copied out of base area.\n", dataRecord->value, dataRecord->siteDatatypeId, refSource->maxValueCutoff, dataRecord->startDateTime, dataRecord->endDateTime);
      }
   }

   return (OK);   
}
