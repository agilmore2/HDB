#ifndef DER_INC
#define DER_INC

#include "derivationTypedefs.h"

int SqlDerivationControl(int*, char*);

int SqlCopyBase(
#ifdef ANSIPROTO
UPDATE_RECORD*,
int,
REF_SOURCE*
#endif
);

int SqlTimeInterval(
#ifdef ANSIPROTO
UPDATE_RECORD*,
SQL_INTERVAL,
INTERVAL_TIME*,
REF_SOURCE*
#endif
);

int SqlTimeShift(
#ifdef ANSIPROTO
INTERVAL_TIME*,
SQL_INTERVAL,
DATE_MATH_TYPES,
INTERVAL_TIME*
#endif
);

int SqlCreateTable();

int SqlDropTable();

int CheckCopy(
#ifdef ANSIPROTO
DATA_RECORD*,
REF_SOURCE*,
int*
#endif
);

int DeriveList(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
int,
REF_SOURCE*,
REF_DESTINATION*
#endif
);

int DeriveValue(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int GetDestParam(
#ifdef ANSIPROTO
SQL_INTERVAL,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int MethodMax(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int MethodTwavelint(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int TwavelintCalc(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
DATA_RECORD*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int MethodEop(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int MethodTot(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int MethodAve(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int MethodMin(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int MethodBop(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int MethodTovol(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int MethodEoplint(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int EoplintCalc(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
INTERVAL_TIME*,
DATA_RECORD*,
REF_DESTINATION*,
DEST_PARAM*
#endif
);

int PartialCalc(
#ifdef ANSIPROTO
REF_DESTINATION*,
INTERVAL_TIME*,
DATA_RECORD*,
int*
#endif
);

int InterpCalc(
#ifdef ANSIPROTO
DATA_RECORD*,
DATA_RECORD*,
DATA_RECORD*
#endif
);

int NegNumSpec(
#ifdef ANSIPROTO
SQL_INTERVAL,
INTERVAL_TIME*,
DEST_PARAM*,
int*,
int*
#endif
);

int SqlSelectData(
#ifdef ANSIPROTO
UPDATE_RECORD*,
SQL_DATE,
SQL_DATE,
int*,
DATA_RECORD**
#endif
);

int SqlDeleteDestination(
#ifdef ANSIPROTO
SQL_INTERVAL,
int,
SQL_DATE,
SQL_DATE
#endif
);

int SqlIntervalUpdate(
#ifdef ANSIPROTO
SQL_INTERVAL,
int,
SQL_DATE,
SQL_DATE
#endif
);

int SqlConvertUnit(
#ifdef ANSIPROTO
SQL_INTERVAL,
UPDATE_RECORD*,
DATA_RECORD*
#endif
);

int SqlWriteResult(
#ifdef ANSIPROTO
SQL_INTERVAL,
DATA_RECORD*
#endif
);

int SqlCopyOverwrite(int*, char*);

int SqlDeleteOverwrite(char*);

int SqlMethodTwo(
#ifdef ANSIPROTO
char*
#endif
);

int LoadDestination(
#ifdef ANSIPROTO
REF_DESTINATION*,
IND_DESTINATION*,
REF_DESTINATION*
#endif
);

int SqlUpdateSelect(
#ifdef ANSIPROTO
int,
SQL_INTERVAL,
int*,
UPDATE_RECORD**
#endif
);

int SqlRemoveOldUpdates();

int SqlVerifySDIList(
#ifdef ANSIPROTO
char *
#endif
);

#endif
