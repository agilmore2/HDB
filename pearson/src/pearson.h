/* Function prototypes */

int SqlProcessDailyValues (
#ifdef ANSIPROTO
SQL_DATE,
SQL_DATE,
SQL_DATE,
SQL_DATE,
ID,
ID,
char *,
char *,
char *,
char *,
int,
int
#endif
);

int SqlProcessMonthlyValues (
#ifdef ANSIPROTO
SQL_DATE,
SQL_DATE,
ID,
ID,
char *,
char *,
char *,
char *,
int,
int
#endif
);

int SqlSelectDatatypeInfo (
#ifdef ANSIPROTO
ID,
char *,
ID *
#endif
);

int SqlSelectUnitName (
#ifdef ANSIPROTO
ID,
char *
#endif
);

int SqlSelectSiteName (
#ifdef ANSIPROTO
ID,
char *
#endif
);

int SqlSelectModelRunName (
#ifdef ANSIPROTO
ID,
char *
#endif
);


/* Prototype to avoid compiler warnings. */
char *strdup(const char *);

