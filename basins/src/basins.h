#ifndef BASINS_INC
#define BASINS_INC 1

#include "basinTypedefs.h"
#include "typedefs.h"
#include "defines.h"

int validValue(
#ifdef ANSIPROTO
char *
#endif
);

int snotelSiteExists(
#ifdef ANSIPROTO
char *
#endif
);

NAME_ID  *validBasin(
#ifdef ANSIPROTO
char *
#endif
);

void storeId(
#ifdef ANSIPROTO
NAME_ID *,
char *
#endif
);

int SqlFetchSiteNamesPlusAltId (
#ifdef ANSIPROTO
ID,
NAME_ID *,
int *,
ID
#endif
);

int SqlFetchSiteNames (
#ifdef ANSIPROTO
ID,
NAME_ID *,
int *
#endif
);

int parseDate (
#ifdef ANSIPROTO
char *,
char *
#endif
);

int UpdateSnotel (
#ifdef ANSIPROTO
SNOTEL *,
int,
SQL_DATE,
ID
#endif
);

int UpdateBasins (
#ifdef ANSIPROTO
BASIN *,
int,
SQL_DATE,
ID
#endif
);

int SqlUpdateCurSnow (
#ifdef ANSIPROTO
SNOTEL *,
int,
SQL_DATE,
ID,
int *		  
#endif
);

int SqlUpdateCurPrecip (
#ifdef ANSIPROTO
SNOTEL *,
int,
SQL_DATE,
ID,
int *		  
#endif
);

int SqlUpdatePctSnow (
#ifdef ANSIPROTO
SNOTEL *,
int,
SQL_DATE,
ID,
int *		  
#endif
);

int SqlUpdatePctPrecip (
#ifdef ANSIPROTO
SNOTEL *,
int,
SQL_DATE,
ID,
int *		  
#endif
);

int SqlUpdateBasPctSnow (
#ifdef ANSIPROTO
BASIN *,
int,
SQL_DATE,
ID,
int *		  
#endif
);

int SqlUpdateBasPctPrecip (
#ifdef ANSIPROTO
BASIN *,
int,
SQL_DATE,
ID,
int *		  
#endif
);

int SqlConnectRoleNew (
#ifdef ANSIPROTO
char*,
char*,
char*,
char*
#endif
);

int SqlFetchRbasekeys (
#ifdef ANSIPROTO
int *,
int *,
int *,
int *
#endif
);

int SqlGetSiteDataTypeIdWithName (
#ifdef ANSIPROTO
int,
char *,
int *
#endif
);


#endif /* BASINS_INC */

