#include "defines.h"

/* Typedefs */
typedef struct increment {
   int minIndex;
   long minVal;
   long maxVal;
   double pctVals;
   int numVals;
   double increment;
   struct increment *prev;
   struct increment *next;
} INCREMENT;

typedef struct inc_header {
   int numElements;
   INCREMENT *list;
} INC_HEADER;

/* Function prototypes */

int SqlGetFlowDuration (
#ifdef ANSIPROTO
ID,
ID,
SQL_DATE,
SQL_DATE,
int,
ID
#endif
);

int SortValues (
#ifdef ANSIPROTO
float *,
int
#endif
);

int CompareValues (
#ifdef ANSIPROTO
int*,
int*
#endif
);

int CalcIncrements (
#ifdef ANSIPROTO
float *,
int
#endif
);

int MakeIncrements (
#ifdef ANSIPROTO
long,
int,
int,
int,
float *,
float,
INCREMENT *
#endif
);

int RemoveIncrements (
#ifdef ANSIPROTO
INCREMENT *
#endif
);
