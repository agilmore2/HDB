#ifndef FUNCTIONS_INC
#define FUNCTIONS_INC

/**************************************************************************/
/* AGGREGATION: Typedefs and function declaration */

typedef int (*AGG_FCN_PTR) (RUN_CONTROL, VALUE_RECORD *, int, ID,
			    SQL_DATE, SQL_DATE, double *);

typedef struct agg_function_table{
  char functionName[33];
  AGG_FCN_PTR fcn;
} AGG_FUNCTION_TABLE;

int InstToAve (
#ifdef ANSIPROTO
RUN_CONTROL,  
VALUE_RECORD *,
int,
ID,
SQL_DATE,
SQL_DATE,
double *
#endif
);

int lin (
#ifdef ANSIPROTO
RUN_CONTROL,
double,
ID,
SQL_DATE,
SQL_DATE,
VALUE_RECORD *,
int
#endif
);

int YourFunction (
#ifdef ANSIPROTO
RUN_CONTROL,
VALUE_RECORD *,
int,
ID,
SQL_DATE,
SQL_DATE,
double *
#endif
);

#define AGG_TABLE_SIZE 2

/**************************************************************************/
/* DISAGGREGATION: Typedefs and function declaration */

typedef int (*DISAGG_FCN_PTR) (RUN_CONTROL, double, ID, SQL_DATE,
			       SQL_DATE, VALUE_RECORD *, int);

typedef struct disagg_function_table{
  char functionName[33];
  DISAGG_FCN_PTR fcn;
} DISAGG_FUNCTION_TABLE;

int YourDisaggFunction (
#ifdef ANSIPROTO
RUN_CONTROL,
double,
ID,
SQL_DATE,
SQL_DATE,
VALUE_RECORD *,
int
#endif
);

#define DISAGG_TABLE_SIZE 2


/**************************************************************************/
/* FUNCTION LOOKUP TABLES */
#ifdef MAIN

AGG_FUNCTION_TABLE aggFunctionTable[] = {
  {"InstToAve", InstToAve},
  {"YourFunctionName", YourFunction}
};

DISAGG_FUNCTION_TABLE disaggFunctionTable[] = {
  {"lin", lin},
  {"YourDisaggFunctionName", YourDisaggFunction}
};

#endif;

#endif
