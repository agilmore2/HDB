/* This file contains function declarations for the database utilities in
   ../hdbLib/src. */

#ifndef DBUTILS_INC
#define DBUTILS_INC 1

#include <stdio.h>
#include "defines.h"
#include "typedefs.h"


/* Declare global var to hold name of current DB site to execute AT. */
#ifndef DBSITE
DATABASE_ALIAS current_dbsite; /* Note, will need to be global to DMIs if used
		    	          to check current session; for now, only 
				  dis/connect uses it. */
#define DBSITE
#endif

int SqlCommit(
#ifdef ANSIPROTO
#endif
);

int SqlDisconnectAt(
#ifdef ANSIPROTO
char *
#endif
);

int SqlGetDBName(
#ifdef ANSIPROTO
char *,
char *
#endif
);

int SqlDisconnectAll(
#ifdef ANSIPROTO
void
#endif
);

#ifdef OBJECT_C
extern "C" {
#endif
    
char * SqlCheckError(
#ifdef ANSIPROTO
char *
#endif
);

int SqlErrorHandler(
#ifdef ANSIPROTO
char *,
short,
char *,
...
#endif
);

int SqlFilePrintErrorHandler(
#ifdef ANSIPROTO
FILE *,
char *,
short,
char *,
...
#endif
);

int SqlFileErrorHandler(
#ifdef ANSIPROTO
FILE *,
char *,
short,
char *,
...
#endif
);

char * SqlCheckErrorNoRows(
#ifdef ANSIPROTO
char *
#endif
);

char * SqlCheckWarning(
#ifdef ANSIPROTO
char *
#endif
);

int SqlConnect(
#ifdef ANSIPROTO
char *,
char *,
char *
#endif
);

int SqlConnectAt(
#ifdef ANSIPROTO
char *,
char *,
char *,
char *
#endif
);

int SqlConnectRole(
#ifdef ANSIPROTO
char *,
char *,
char *,
char *
#endif
);

int SqlConnectRoleAt(
#ifdef ANSIPROTO
char *,
char *,
char *,
char *,
char *
#endif
);

int SqlCheckGrantedRole(
#ifdef ANSIPROTO
char *,
char *
#endif
);

int SqlGetRolePsswd(
#ifdef ANSIPROTO
char *,
char *
#endif
);

int SqlConnectRoleSession(
#ifdef ANSIPROTO
  char *,
  char *,
  char *,
  char *, 
  int
#endif
);

int SqlConnectSession(
#ifdef ANSIPROTO
  char *,
  int
#endif
);

int SqlDisconnect(
#ifdef ANSIPROTO
#endif
);

int AddConnection (
#ifdef ANSIPROTO
char *
#endif
);

int SwitchSession (
#ifdef ANSIPROTO
char *
#endif
);

int SqlGetObjecttypeId (
#ifdef ANSIPROTO
char *,
ID *
#endif
);

int SqlGetSiteDatatypeIdWithId(
#ifdef ANSIPROTO
ID,
ID,
ID *
#endif
);

int SqlGetUnitId (
#ifdef ANSIPROTO
ID,
ID *
#endif
);

int SqlGetDimensionId (
#ifdef ANSIPROTO
ID,
ID *
#endif
);

int SqlDatatypeIsInstant (
#ifdef ANSIPROTO
ID,
int *
#endif
);

int SqlDatatypeIsEither (
#ifdef ANSIPROTO
ID,
int *
#endif
);

int SqlGetSourceId(
#ifdef ANSIPROTO
ID,
SQL_DATE,
char *,
ID *
#endif
);

int SqlGetSourcetypeId(
#ifdef ANSIPROTO
char *,
ID *
#endif
);

int SqlGetAppSourceId(
#ifdef ANSIPROTO
char *,
ID *
#endif
);

int SqlGetModelId(
#ifdef ANSIPROTO
char *,
ID *
#endif
);

int SqlGetModelRunId (
#ifdef ANSIPROTO
ID,
char *,
SQL_DATE,
int,
char *,
ID,
SQL_DATE,
SQL_DATE,
char *,
char *,
ID *
#endif
);

int SqlGetUnitInfo (
#ifdef ANSIPROTO
ID,
UNIT_INFO *
#endif
);

int SqlMethodClassStatsOk (
#ifdef ANSIPROTO
METHOD_CLASS_INFO,
int *
#endif
);

int SqlGetValidationFlag(
#ifdef ANSIPROTO
ID,
char *
#endif
);

int SqlChkModelRunID(
#ifdef ANSIPROTO
int
#endif
);

int SqlFormatDate(
#ifdef ANSIPROTO
char *,
SQL_DATE
#endif
);

int SqlDateMath(
#ifdef ANSIPROTO
DATE_MATH_TYPES,
SQL_DATE,
SQL_DATE,
int,
char *
#endif
);

int SqlDateCompare(
#ifdef ANSIPROTO
SQL_DATE,
SQL_DATE,
int *
#endif
);

int SqlGetDate(
#ifdef ANSIPROTO
SQL_DATE
#endif
);

int SqlGetDateTime(
#ifdef ANSIPROTO
SQL_DATE
#endif
);

int SqlExtractDatePart(
#ifdef ANSIPROTO
SQL_DATE,
char *,
int *
#endif
);

int SqlDateTrunc(
#ifdef ANSIPROTO
SQL_DATE,
char *,
SQL_DATE
#endif
);

int SqlDateRangeIncludesFeb (
#ifdef ANSIPROTO
SQL_DATE,
SQL_DATE,
int *,
int *
#endif
);

int SqlSetDateFormatAt (
#ifdef ANSIPROTO
char *
#endif
);

int SqlSetDateFormat (
#ifdef ANSIPROTO
char *
#endif
);

int SqlNumDaysBetweenDates(
#ifdef ANSIPROTO
SQL_DATE,
SQL_DATE,
int *
#endif
);

int SqlDateSubtract(
#ifdef ANSIPROTO
SQL_DATE,
SQL_DATE,
double*
#endif
);

int SqlGetNumSecondsInMonth (
#ifdef ANSIPROTO 
SQL_DATE,
long *
#endif
);

int SqlNumIntervalsBetweenDates (
#ifdef ANSIPROTO
SQL_DATE,
SQL_DATE,
SQL_INTERVAL,
double *
#endif
);

int SqlIntervalCount(
#ifdef ANSIPROTO
int*
#endif
);

int SqlGetIntOrder(
#ifdef ANSIPROTO
SQL_INTERVAL,
int*
#endif
);

int SqlGetIntName(
#ifdef ANSIPROTO
int,
SQL_INTERVAL
#endif
);

int SqlGetPreviousIntName(
#ifdef ANSIPROTO
SQL_INTERVAL,
SQL_INTERVAL,
int*
#endif
);

int SqlElevValue(
#ifdef ANSIPROTO
ID,
char *,
float,
float *
#endif
);

int SqlSelectCount(
#ifdef ANSIPROTO
char *,
int *
#endif
);

int SqlGetAgencyId(
#ifdef ANSIPROTO
char*,
int*
#endif
);

int SqlGetLoadingId(
#ifdef ANSIPROTO
char*,
int*
#endif
);

int SqlGetCollectionId(
#ifdef ANSIPROTO
char*,
int*
#endif
);

int SqlGetMethodId(
#ifdef ANSIPROTO
char*,
int*
#endif
);

int SqlGetComputationId(
#ifdef ANSIPROTO
char*,
int*
#endif
);

int SqlGetOverwriteFlag(
#ifdef ANSIPROTO
char*,
char*
#endif
);

/* Prototype to avoid compiler warnings. */
char *strdup(const char *);

#ifdef OBJECT_C
}
#endif

#endif
 
