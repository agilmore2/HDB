#ifndef DERTYPEDEFS_INC
#define DERTYPEDEFS_INC

#include "defines.h"
#include "typedefs.h"

#define VALUE_NULL -1111111
#define EMPTY_CHAR ' '

typedef char SQL_INTERVAL_UNIT[10];

typedef struct ref_source
{
   int siteDatatypeId;
   SQL_INTERVAL interval;
   SQL_INTERVAL firstDestInterval;
   double minValueExpected;
   double minValueCutoff;
   double maxValueExpected;
   double maxValueCutoff;
   int timeOffsetMinutes;
} REF_SOURCE;

typedef struct ind_source
{
   IND siteDatatypeId_ind;
   IND interval_ind;
   IND firstDestInterval_ind;
   IND minValueExpected_ind;
   IND minValueCutoff_ind;
   IND maxValueExpected_ind;
   IND maxValueCutoff_ind;
   IND timeOffset_ind;
} IND_SOURCE;

typedef struct ref_destination
{
   int baseSDI;
   int compoundingSourceSDI;
   SQL_INTERVAL compoundingSourceInterval;
   int destSDI;
   char partialCalc;
   int methodId;
   int hrDesiredEopWindow;
   int hrRequiredEopWindow;
   int hrDesiredBopWindow;
   int hrRequiredBopWindow;
   SQL_INTERVAL_UNIT hrWindowUnit;  
   int hrDesiredNumSource;
   int hrRequiredNumSource;
   int dayDesiredEopWindow;
   int dayRequiredEopWindow;
   int dayDesiredBopWindow;
   int dayRequiredBopWindow;
   SQL_INTERVAL_UNIT dayWindowUnit;  
   int dayDesiredNumSource;
   int dayRequiredNumSource;
   int monDesiredEopWindow;
   int monRequiredEopWindow;
   int monDesiredBopWindow;
   int monRequiredBopWindow;
   SQL_INTERVAL_UNIT monWindowUnit;  
   int monDesiredNumSource;
   int monRequiredNumSource;
   int yrDesiredEopWindow;
   int yrRequiredEopWindow;
   int yrDesiredBopWindow;
   int yrRequiredBopWindow;
   SQL_INTERVAL_UNIT yrWindowUnit;  
   int yrDesiredNumSource;
   int yrRequiredNumSource;
   int wyDesiredEopWindow;
   int wyRequiredEopWindow;
   int wyDesiredBopWindow;
   int wyRequiredBopWindow;
   SQL_INTERVAL_UNIT wyWindowUnit;  
   int wyDesiredNumSource;
   int wyRequiredNumSource;
} REF_DESTINATION;

typedef struct ind_destination
{
   IND baseSDI_ind;
   IND compoundingSourceSDI_ind;
   IND compoundingSourceInterval_ind;
   IND destSDI_ind;
   IND partialCalc_ind;
   IND methodId;
   IND hrDesiredEopWindow_ind;
   IND hrRequiredEopWindow_ind;
   IND hrDesiredBopWindow_ind;
   IND hrRequiredBopWindow_ind;
   IND hrWindowUnit_ind;
   IND hrDesiredNumSource_ind;
   IND hrRequiredNumSource_ind;
   IND dayDesiredEopWindow_ind;
   IND dayRequiredEopWindow_ind;
   IND dayDesiredBopWindow_ind;
   IND dayRequiredBopWindow_ind;
   IND dayWindowUnit_ind;
   IND dayDesiredNumSource_ind;
   IND dayRequiredNumSource_ind;
   IND monDesiredEopWindow_ind;
   IND monRequiredEopWindow_ind;
   IND monDesiredBopWindow_ind;
   IND monRequiredBopWindow_ind;
   IND monWindowUnit_ind;
   IND monDesiredNumSource_ind;
   IND monRequiredNumSource_ind;
   IND yrDesiredEopWindow_ind;
   IND yrRequiredEopWindow_ind;
   IND yrDesiredBopWindow_ind;
   IND yrRequiredBopWindow_ind;
   IND yrWindowUnit_ind;
   IND yrDesiredNumSource_ind;
   IND yrRequiredNumSource_ind;
   IND wyDesiredEopWindow_ind;
   IND wyRequiredEopWindow_ind;
   IND wyDesiredBopWindow_ind;
   IND wyRequiredBopWindow_ind;
   IND wyWindowUnit_ind;
   IND wyDesiredNumSource_ind;
   IND wyRequiredNumSource_ind;
} IND_DESTINATION;

typedef struct data_record
{
   int siteDatatypeId;
   SQL_DATE startDateTime;
   SQL_DATE endDateTime;
   double value;
   SQL_INTERVAL interval;
   char overwrite;
   SQL_DATE dateTimeLoaded;
   int methodId;
   char derivationFlag[20];
} DATA_RECORD;

typedef struct update_record
{
   int siteDatatypeId;
   SQL_DATE startDateTime;
   SQL_DATE endDateTime;
   SQL_INTERVAL interval;   
} UPDATE_RECORD;

typedef struct interval_time
{
   SQL_DATE begInterval;
   SQL_DATE endInterval;
   SQL_DATE begSource;
   SQL_DATE endSource;
} INTERVAL_TIME;

typedef struct dest_param
{
   int desiredEopWindow;
   int requiredEopWindow;
   int desiredBopWindow;
   int requiredBopWindow;
   SQL_INTERVAL_UNIT windowUnit;  
   int desiredNumSource;
   int requiredNumSource;
} DEST_PARAM;   

#endif
 
