/* Prototypes for commonly-used straight C (not embedded SQL) functions 
   which will be used in HDB applications.  These functions sit in 
   ../utilsLib/src.

   This file should be included in all HDB application source code files.
*/

#ifndef UTILS_INC
#define UTILS_INC 1

#include <stdio.h>
#include <stdarg.h>
#include <defines.h> 
#include "typedefs.h"

/* Prototypes for generic coding (C) utility functions */

#ifdef OBJECT_C
extern "C" {
#endif

int SetErrorFd (
#ifdef ANSIPROTO
char *
#endif
);

int RegisterSignalHandler(
#ifdef ANSIPROTO
void
#endif
);

void SignalHandler(
#ifdef ANSIPROTO
int
#endif
);

int CloseErrorFd (
#ifdef ANSIPROTO
#endif
);

int PrintError (
#ifdef ANSIPROTO
char *,
...
#endif
);

int PrintDbError (
#ifdef ANSIPROTO
char *,
va_list
#endif
);

int FilePrintError (
#ifdef ANSIPROTO
FILE *,
char *,
...
#endif
);

int FileError (
#ifdef ANSIPROTO
FILE *,
char *,
...
#endif
);

expression_var_list * GetVariableList(
#ifdef ANSIPROTO
  char * 
#endif
);

void FreeVariableList(
#ifdef ANSIPROTO
  expression_var_list * 
#endif
);

int CalcExpression(
#ifdef ANSIPROTO
  double *, 
  char *,
  expression_var_list *
#endif
);

int GetNumDays (
#ifdef ANSIPROTO
SQL_DATE,
TIME_INTERVAL_TYPES,
int *
#endif
);

int GetNumSeconds (
#ifdef ANSIPROTO
SQL_DATE,
TIME_INTERVAL_TYPES,
int *
#endif
);

int GetWaterYear(
#ifdef ANSIPROTO
SQL_DATE,
int *
#endif
);

int IsFirstDayWaterYear(
#ifdef ANSIPROTO
SQL_DATE,
int
#endif
);

int IsLeapYear(
#ifdef ANSIPROTO
SQL_DATE
#endif
);

int CalcNumLeapSeconds(
#ifdef ANSIPROTO
SQL_DATE,
SQL_DATE,
float *
#endif
);

int GetFirstDayWaterYear(
#ifdef ANSIPROTO
int,
SQL_DATE
#endif
);

int GetDayOfYear(
#ifdef ANSIPROTO
SQL_DATE,
int *
#endif
);

float RoundValue(
#ifdef ANSIPROTO
float,
int
#endif
);

int TableLookup(
#ifdef ANSIPROTO
float *,
float *,
float,
float *,
int
#endif
);

int GetDateColumn (
#ifdef ANSIPROTO
TIME_INTERVAL_TYPES,
int,
int,
OBJECTTYPE_NAME
#endif
);
 
/* HERE */
int ConvertValue (
#ifdef ANSIPROTO
VALUE,
int,
ID,
SQL_DATE,
VALUE *,
int,
ID,
TIME_INTERVAL_TYPES,
long
#endif
);

int SimpleConvert (
#ifdef ANSIPROTO
VALUE,
int,
UNIT_INFO,
SQL_DATE,
VALUE *,
int,
UNIT_INFO,
long
#endif
);

int ConvertFlowVolume (
#ifdef ANSIPROTO
UNIT_INFO,
UNIT_INFO,
VALUE,
int,
SQL_DATE,
VALUE *,
int,
TIME_INTERVAL_TYPES,
long
#endif
);

int ConvertPowerEnergy (
#ifdef ANSIPROTO
VALUE,
int,
UNIT_INFO,
SQL_DATE,
VALUE *,
int,
UNIT_INFO,
TIME_INTERVAL_TYPES,
long
#endif
);

int ConvertExpressionValue (
#ifdef ANSIPROTO
VALUE,
int,
UNIT_INFO,
SQL_DATE,
VALUE *,
int,
UNIT_INFO,
long,
TIME_INTERVAL_TYPES
#endif
);

int ConvertExpression (
#ifdef ANSIPROTO
VALUE,
int,
SQL_DATE,
char *,
VALUE *,
int,
long,
TIME_INTERVAL_TYPES
#endif
);

int GetSeconds (
#ifdef ANSIPROTO
UNIT_INFO,
UNIT_INFO,
SQL_DATE, 
long *
#endif
);

int GetNumSecondsInInterval(
#ifdef ANSIPROTO
TIME_INTERVAL_TYPES,
SQL_DATE, 
long *
#endif
);

int GetNumSecondsInYear (
#ifdef ANSIPROTO
SQL_DATE,
long *,
TIME_INTERVAL_TYPES
#endif
);


#ifdef OBJECT_C
}
#endif

#endif
