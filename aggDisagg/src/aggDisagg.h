#ifndef AGG_INC
#define AGG_INC

#include "aggDisaggTypedefs.h"
#include "userFunctions.h"

int ParseCommandLine (
#ifdef ANSIPROTO
RUN_CONTROL *,
int,
char **,
ID *,
int *
#endif
);

int SqlLoadAggDisagg (
#ifdef ANSIPROTO
RUN_CONTROL *,
int
#endif
);

int SetRunControl (
#ifdef ANSIPROTO
RUN_CONTROL *
#endif
);

int PrintRunControl (
#ifdef ANSIPROTO
RUN_CONTROL
#endif
);

int VerifyInputs (
#ifdef ANSIPROTO
RUN_CONTROL
#endif
);

int GetTableNames (
#ifdef ANSIPROTO
RUN_CONTROL *
#endif
);

int Bookkeeping (
#ifdef ANSIPROTO
RUN_CONTROL *,
char *
#endif
);

int DimensionChangeInBaseArea (
#ifdef ANSIPROTO
RUN_CONTROL *
#endif
);

int SetBoundaryDates (
#ifdef ANSIPROTO
RUN_CONTROL,
BOUNDARY_DATES **,
int *
#endif
);

int CreateBoundaryDatePairs (
#ifdef ANSIPROTO
RUN_CONTROL,
BOUNDARY_DATES *,
int,
double,
UNIT_INFO,
UNIT_INFO
#endif
);

int SetBoundaryDatesRange (
#ifdef ANSIPROTO
RUN_CONTROL,
BOUNDARY_DATES **,
int *
#endif
);

int CreateBoundaryDateRanges (
#ifdef ANSIPROTO
RUN_CONTROL,
BOUNDARY_DATES *,
int,
double,
UNIT_INFO,
UNIT_INFO
#endif
);
 
int GetEndOfInterval (
#ifdef ANSIPROTO
SQL_DATE,
SQL_INTERVAL,
SQL_DATE
#endif
);

int PrintBoundaryDates (
#ifdef ANSIPROTO
BOUNDARY_DATES *,
int
#endif
);

int ProcessNonStats (
#ifdef ANSIPROTO
RUN_CONTROL,		    
BOUNDARY_DATES *,
int,
ID,
ID
#endif
);

int ProcessStats (
#ifdef ANSIPROTO
RUN_CONTROL,		    
BOUNDARY_DATES *,
int,
ID,
ID,
TIME_INTERVALS *
#endif
);

int RetrieveSourceValues(
#ifdef ANSIPROTO
RUN_CONTROL,	
ID,
SQL_DATE,	    
SQL_DATE,	    
VALUE_RECORD **,
int *
#endif
);

int SqlRetrieveSourceValues(
#ifdef ANSIPROTO
VALUE_RECORD *,
char *,
int *,
int
#endif
);

int SqlFetchPreviousTimeStepValue(
#ifdef ANSIPROTO
OBJECTTYPE_NAME ,
TIME_INTERVAL_TYPES,
ID,
SQL_DATE,
double *,
int,
int,
int
#endif
);

int PrintSourceValues (
#ifdef ANSIPROTO
VALUE_RECORD *,
int,
ID
#endif
);

int PrintDisAggValues (
#ifdef ANSIPROTO
int,
int,
VALUE_RECORD *,
int,
ID
#endif
);

int VerifyNumValues (
#ifdef ANSIPROTO
RUN_CONTROL,
int,
int
#endif
);

int AggValues (
#ifdef ANSIPROTO
RUN_CONTROL,
VALUE_RECORD *,
int,
BOUNDARY_DATES,
VALUE_RECORD **,
int *,
ID
#endif
);

int AggValuesWithOp (
#ifdef ANSIPROTO
RUN_CONTROL,
VALUE_RECORD *,
int,
double *
#endif
);

int TotalValues (
#ifdef ANSIPROTO
ID,
VALUE_RECORD *,
int,
double *
#endif
);

int SortValues (
#ifdef ANSIPROTO
ID,
VALUE_RECORD *,
int,
double *
#endif
);

int ValueCompare (
#ifdef ANSIPROTO
VALUE_RECORD *,
VALUE_RECORD *
#endif
);

int DateCompare (
#ifdef ANSIPROTO
VALUE_RECORD *,
VALUE_RECORD *
#endif
);

int ComputeAggDate (
#ifdef ANSIPROTO
RUN_CONTROL,
BOUNDARY_DATES,
VALUE_RECORD *
#endif
);

AGG_FCN_PTR AggLookup (
#ifdef ANSIPROTO
char *
#endif
);

int DisaggValues (
#ifdef ANSIPROTO
RUN_CONTROL,
VALUE_RECORD *,
int,
BOUNDARY_DATES,
VALUE_RECORD **,
int *,
ID 
#endif
);

int DisaggValuesWithOp (
#ifdef ANSIPROTO
RUN_CONTROL,
double,
VALUE_RECORD *,
int
#endif
);

int DivideValue (
#ifdef ANSIPROTO
double,
VALUE_RECORD *,
int,
int
#endif
);

int ComputeDisaggDates (
#ifdef ANSIPROTO
RUN_CONTROL,
BOUNDARY_DATES,
VALUE_RECORD *
#endif
);

DISAGG_FCN_PTR DisaggLookup (
#ifdef ANSIPROTO
char *
#endif
);

int ConvertEachStatValue (
#ifdef ANSIPROTO
RUN_CONTROL,
VALUE_RECORD *,
int,
BOUNDARY_DATES 
#endif
);

int ConvertEachNonStatValue (
#ifdef ANSIPROTO
RUN_CONTROL,
VALUE_RECORD *,
int,
BOUNDARY_DATES 
#endif
);

int CalcRangeNSeconds(
#ifdef ANSIPROTO
RUN_CONTROL,
BOUNDARY_DATES,
int *
#endif
);

int WriteValues (
#ifdef ANSIPROTO
RUN_CONTROL,
ID,
VALUE_RECORD *,
int,
int,
FILE *
#endif
);

int OpenInitDataFile(
#ifdef ANSIPROTO
ID,
ID,
RUN_CONTROL,
FILE **
#endif
);
 
int WriteFileHeader(
#ifdef ANSIPROTO
RUN_CONTROL,
FILE *
#endif
);

int WriteRiverWareDates(
#ifdef ANSIPROTO
RUN_CONTROL,
FILE *
#endif
);

int GetRiverWareDates(
#ifdef ANSIPROTO
int,
int,
SQL_DATE,
SQL_DATE,
TIME_INTERVAL_TYPES,
char *,
char *
#endif
);

int WriteAggDisaggFile(
#ifdef ANSIPROTO
FILE *,
RUN_CONTROL,
VALUE_RECORD *,
int,
int
#endif
);
 

int SqlWriteRealAggDisagg(
#ifdef ANSIPROTO
RUN_CONTROL,
ID,
VALUE_RECORD *,
int
#endif
);

int SqlWriteModelAggDisagg(
#ifdef ANSIPROTO
RUN_CONTROL,
ID,
VALUE_RECORD *,
int
#endif
);

#endif
