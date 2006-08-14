/* This file contains defines for values commonly referenced in HDB 
   application code. */

#ifndef DEFINES_INC
#define DEFINES_INC 1

/* Database object string lengths */
#define DATABASE_NAME_LENGTH    50

#define DATATYPE_CODE_LENGTH    17
#define ERROR_MSG_LENGTH       512
#define EXPRESSION_LENGTH       65
#define OBJECTTYPE_NAME_LENGTH  33
#define OBJECTTYPE_TAG_LENGTH    6
#define QUALITY_FLAG_LENGTH      3
#define SITE_NAME_LENGTH        65

/* Changed from length of 26; HDB never uses more than 
   21 char date, and truncating the length simplifies
   string comparisons between dates. */
#define SQL_DATE_LENGTH         21
#define SQL_ERROR_MSG_LENGTH   256
#define SQL_INTERVAL_LENGTH     33
#define VALIDATION_FLAG_LENGTH   2
#define ROLE_PASSWORD_LENGTH    20
#define DATABASE_ALIAS_LENGTH    10

/* Database object type codes; update if codes in hdb_objecttype table 
   change!! */
#define BAS "bas"
#define CLI "cli"
#define CON "con"
#define DIV "div"
#define HYD "hyd"
#define RCH "rch"
#define RES "res"
#define SNO "sno"
#define STR "str"

/* Data table names and date column names for each time interval */
#define DATE_TIME "date_time"
#define REAL_INSTANT "r_instant"
#define REAL_HOUR "r_hour"
#define REAL_DAY "r_day"
#define REAL_MONTH "r_month"
#define REAL_YEAR "r_year"
#define REAL_WY "r_wy"
#define REAL_HOURSTAT "r_hourstat"
#define REAL_DAYSTAT "r_daystat"
#define REAL_MONTHSTAT "r_monthstat"
#define REAL_YEARSTAT "r_yearstat"
#define REAL_WYSTAT "r_wystat"

#define MODEL_INSTANT "m_instant"
#define MODEL_HOUR "m_hour"
#define MODEL_DAY "m_day"
#define MODEL_MONTH "m_month"
#define MODEL_YEAR "m_year"
#define MODEL_WY "m_wy"

#define DATE_INSTANT "date_time"
#define DATE_HOUR "date_hour"
#define DATE_DAY "date_day"
#define DATE_MONTH "date_month"
#define DATE_YEAR "date_year"
#define DATE_WY "date_wy"
#define DATE_HOURSTAT "hour"
#define DATE_DAYSTAT "day"
#define DATE_MONTHSTAT "month"
#define DATE_YEARSTAT "year"
#define DATE_WYSTAT "wy"

/* Table modifiers */
#define STAT_MOD "stat"
#define RANGE_MOD "range"
#define MOST_RECENT_MOD "mr"

/* Program constants */
#define TRUE 1
#define FALSE 0
#define NULL_DATE "01-JAN-1800 00:00:00"
#define NULL_VALUE 998877
#define NA -1
#define OVERWRITE_FLAG "O"
#define BOGUS_VALUE 1
#define EMPTY_STRING ""

/* Return values */
#define OK       0
#define ERROR    1
#define WARNING -2
#define CREATE_SDI -3
#define END_INPUT -4

/* Unit and dimension stuff */
#define SECONDS_IN_DAY 86400
#define MONTH_31_DAY 2678400
#define YEAR_365_DAY 31536000
#define NUM_CF_IN_AF 43560
#define CENTURY 1900

#define FLOW_ID 2
#define VOLUME_ID 1
#define POWER_ID 6
#define ENERGY_ID 7
#define TIME_ID 8

/* Date and time intervals recognized by SQL */
#define YEAR "year"
#define MONTH "month"
#define DAY "dd"
#define HOUR "hh24"
#define MINUTE "mi"
#define SECOND "ss"

/* Date and time intervals recognized by HDB */
#define HDB_YEAR "year"
#define HDB_MONTH "month"
#define HDB_DAY "day"
#define HDB_HOUR "hour"
#define HDB_MINUTE "N/A"
#define HDB_SECOND "N/A"
#define HDB_INSTANT "instant"
#define HDB_OTHER "other"
#define HDB_WY "wy"

/* Allowable intervals used in hdb_datatype */
#define INSTANT "instant"
#define NONINSTANT "non-instant"
#define EITHER "either"

/* Date range indicators; used in indicating which
   of two years (high or low) contains a Feb 28 value 
   (For leap year calculating. */
#define HIGH 0
#define LOW 1

#define IS_NULL -1
#define NULL_IND -1

#define MAX_SOURCE_IDS 100

/* strings used in r_base foreign key lookup tables */
#define SEE_LOADING_APP "See loading application"
#define UNKNOWN "unknown"
#define OVERWRITE "overwrite"

/* strings used in hdb_method_class to define method_class_type */
#define CALCULATION "calculation"
#define NA_METHOD_CLASS_TYPE "N/A"
#define TIME_AGGREGATION "time-aggregation"
#define TIME_DISAGGREGATION "time-disaggregation"

/* IDs for pertinent method classes */
#define EOP_METHOD_CLASS 6
#define BOP_METHOD_CLASS 3
#define SUM_METHOD_CLASS 14
#define COPY_METHOD_CLASS 5
#define DIV_METHOD_CLASS 16
#define AVE_METHOD_CLASS 2
#define MIN_METHOD_CLASS 9
#define MAX_METHOD_CLASS 8

/* for the lookup applications */

#define CONT "cont"
#define AREARES "areares"
#define MAXLUROWS 1000

/* expression evaluator constants and macros */
#define MAX_STACK_DEPTH 100
#define MAX_EXPRESSION_LENGTH 60
#define EXPRESSION_VAR_NAME_LENGTH 64

#define NEXT_TOKEN(cp,newcp,token) \
        while (isspace (*cp)) cp++; \
        newcp = strchr(cp,' '); \
        token = cp; \
        if (newcp != NULL) { \
            *newcp = '\0';\
            cp = newcp + 1;\
        }\
        else\
            cp = strchr(cp,'\0');

#define PUSH(s,t,v) (s[t] = v, t++)
#define POP(s,t)    (t--, s[t])

/*defines for reading the dmi meta control file */
#define MODEL 0
#define REAL  1

/* Macro to evaluate datatype_code endings. Assumes a 3-character ending. */
#define NON_STAT_DATATYPE_ENDING(ending) \
          ((!strcmp (ending, "eop") || !strcmp (ending, "tot")) ? 1 : 0)
          

/* Macro to verify that a TIME_INTERVAL_TYPE is an interval for which
   data is stored in the database. */
#define DB_TABLE_INTERVAL(interval) \
          ((interval == BY_INSTANT) || \
	   (interval == BY_HOUR) || \
	   (interval == BY_DAY) || \
	   (interval == BY_MONTH) || \
	   (interval == BY_YEAR) || \
	   (interval == BY_WY))

#endif


