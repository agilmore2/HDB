#ifndef AGGDEFINES_INC
#define AGGDEFINES_INC

#define MAX_NUM_SITES 100

#define DB_OBS_LENGTH 17
#define OP_LENGTH 65
#define FUNC_LENGTH 33

/* Following line commented out because non-DBA users can
   invoke this program. */
/* #define AGG_DISAGG_USER "hdba" */
#define PASSWORD_FLAG "-P"

/* Model Run / Source indicators for data derivation */
#define REAL_IND "R"
#define MODEL_RUN_IND "M"

#define MISSING_VALUE -1

#define WIDTH 16
#define PRECISION 15

#define MODEL_NAME "aggDisagg"
#define LOADING_APPLICATION_NAME "AggDisagg"

#endif AGGDEFINES_INC
