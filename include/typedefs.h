/* This file contains typedefs which can be recognized by the Embedded SQL
   precompiler (e.g., variables of these types can be declared in SQL
   DECLARE sections). When the precompiler examines this file, it creates a
   straight C include file (.h extension).  This .h file can then be
   included by any C routine which requires its typedefs. This eliminates
   the need for 2 include files with identical typedefs, one for Emb SQL
   and one for straight C. 
   Note that .sc source and include files should include this file with the
   embedded SQL include statement.
*/
#ifndef TYPEDEFS_INC
#define TYPEDEFS_INC 1
#include "defines.h"
  typedef char DATABASE_NAME[DATABASE_NAME_LENGTH];
  typedef char DATABASE_ALIAS[DATABASE_ALIAS_LENGTH];
  typedef char DATATYPE_CODE[DATATYPE_CODE_LENGTH];
  typedef char ERROR_MSG[ERROR_MSG_LENGTH];
  typedef char EXPRESSION[EXPRESSION_LENGTH];
  typedef char OBJECTTYPE_NAME[OBJECTTYPE_NAME_LENGTH];
  typedef char OBJECTTYPE_TAG[OBJECTTYPE_TAG_LENGTH];
  typedef char QUALITY_FLAG[QUALITY_FLAG_LENGTH];
  typedef char SITE_NAME[SITE_NAME_LENGTH];
  typedef char SQL_DATE[SQL_DATE_LENGTH];
  typedef char SQL_ERROR_MSG[SQL_ERROR_MSG_LENGTH];
  typedef char SQL_INTERVAL[SQL_INTERVAL_LENGTH];
  typedef char VALIDATION_FLAG[VALIDATION_FLAG_LENGTH];
  typedef int ID;
  typedef short IND;
  typedef double VALUE;
  typedef char oradecimal[80]; 


  enum expression_var_types {
    SOURCE_VALUE, N_DAYS, ILLEGAL
  };
  typedef struct expression_var_list_struct {
    char var_name[EXPRESSION_VAR_NAME_LENGTH];
    int var_type;
    double value;
    struct expression_var_list_struct *nextItem;
  } expression_var_list;
  typedef struct app_source_id {
    ID sourceId;
    ID sourcetypeId;
    SQL_DATE updateDateTime;
    char qualityFlag[2];
  } APP_SOURCE_ID;
  typedef enum date_math_types {
    ADDITION, SUBTRACTION
  }DATE_MATH_TYPES;
  typedef enum time_interval_types {
    BY_INSTANT =0, BY_SECOND =1, BY_MINUTE =2, BY_HOUR =3, BY_DAY =4, BY_MONTH 
    =5, BY_YEAR =6, BY_WY =7
  }TIME_INTERVAL_TYPES;
  typedef struct time_intervals {
    ID unitId;
    SQL_INTERVAL ingresName;
  } TIME_INTERVALS;
  typedef struct unit_info {
    ID unitId;
    char name[33];
    ID dimension;
    ID storedUnitId;
    int isFactor;
    long double multFactor;
    char fromStdExpression[65];
    char toStdExpression[65];
    int monthly;
    TIME_INTERVAL_TYPES yearly;
    int overTime;
  } UNIT_INFO;
  typedef struct method_class_info {
    ID methodClassId;
    char name[33];
    char methodClassType[17];
  } METHOD_CLASS_INFO;
#define NUM_TIME_INTERVALS 8
#ifdef MAIN
/* Note: this array of structures maps TIME_INTERVAL_TYPES onto 
   the HDB unit_ids corresponding to those types, and the Ingres character
   string names for those intervals, where appropriate.  Access to the 
   array is by the TIME_INTERVAL_TYPE; that is, with:
   timeIntervals[BY_HOUR]
   the unit_id for the HDB unit *hour*, and the Ingres string for that
   interval ("hours") can be referenced. 
   Note that the unitId *must* correspond with the unit_id in the hdb_unit
   table. 
   NOTE: if you use this array as a global variable in your program code
	 (that is, it's included in main, and you declare it "extern"
	  everywhere else you use it), declare it THIS way:
   	      extern TIME_INTERVALS timeIntervals[NUM_TIME_INTERVALS];
	  NOT:
  	      extern TIME_INTERVALS *timeIntervals;
	  For some reason, you'll get segmentation violations
	  when trying to reference it. Nobody really knows why...
*/
/* Treat instantaneous as seconds; any date arithmetic to be done
   should use the smallest time interval available. */
/* IF YOU CHANGE THE SIZE OF THIS ARRAY, be sure to change NUM_TIME_INTERVALS,
   defined above. */
TIME_INTERVALS timeIntervals[] =
                  {{NA, SECOND},
		   {36, SECOND},
		   {37, MINUTE},
		   {17, HOUR},
		   {39, DAY},
		   {68, MONTH},
		   {69, YEAR},
		   {70, YEAR}
		  };


#endif 
#endif

