#ifndef AGGTYPEDEFS_INC
#define AGGTYPEDEFS_INC

#include "aggDisaggDefines.h"
#include "defines.h"
#include "typedefs.h"


typedef enum op_type {
  AGG = 0,
  DISAGG = 1
} OP_TYPE;

typedef enum format_type {
  EXP,
  FLOATING
} FORMAT_TYPE;

typedef struct run_control{
  int sourceIsRange;
  int destIsRange;
  int isStat;
  int sourceIsReal;
  int destIsReal;
  int toDb;

   /*New variable to accomodate reading from a specific model_run_id*/
  int model_run_id_src;

   /*New variables to accomodate writing to a specific model_run_id*/
  int model_run_id_dest;
  int model_run_id_flag;

  FORMAT_TYPE format;
  int width;
  int precision;

  /* Identifying information, written along with the value to 
     the database */
  SQL_INTERVAL interval;
  ID modelOrSourceId;
  ID agencyId;  
  char overwriteFlag[2];
  ID collectionSystemId;
  ID loadingApplicationId;
  ID methodId;
  ID methodClassId;
  ID computationId;

  ID datatypeSource;
  ID unitSource;
  ID dimensionSource;

  ID datatypeDest;
  ID unitDest;
  ID dimensionDest;
  int toScale;

  TIME_INTERVAL_TYPES obsSource;
  TIME_INTERVAL_TYPES obsDest;
  TIME_INTERVAL_TYPES obsDriver;
  TIME_INTERVAL_TYPES obsSub;
  SQL_INTERVAL sqlObsDriver;
  SQL_INTERVAL sqlObsSub;
  ID hdbObsDriver;
  ID hdbObsSub;

  SQL_DATE dateStart;
  SQL_DATE dateEnd;
  SQL_DATE dateBegRange;
  SQL_DATE dateEndRange;
  int numPer;

  OBJECTTYPE_NAME tableSource;
  OBJECTTYPE_NAME tableDest;

  OP_TYPE opType;
  char opOrFcn[2];
  char functionName[FUNC_LENGTH];

  int dimensionChangeInBaseArea;
} RUN_CONTROL;

typedef struct value_record {
  SQL_DATE dateStart;
  SQL_DATE dateEnd;
  int timeIndexStart;
  int timeIndexEnd;
   /* ID methodId;*/
  double value;
} VALUE_RECORD;

typedef struct boundary_dates {
  SQL_DATE dateLow;
  SQL_DATE dateHigh;
  int   divisor;
} BOUNDARY_DATES;


#endif
 
