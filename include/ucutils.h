/* declare functions; support ANSI or K and R C. */

int checkforModelrunid(
#ifdef ANSIPROTO
ID,
SQL_DATE,
int,
char *,
int *
#endif
);

int SqlLookupIds (
#ifdef ANSIPROTO
char* agen_name,
char* coll_sys_name,
char* load_app_name,
char* method_name,
char* computation_name
#endif
);

int insertModeldata(
#ifdef ANSIPROTO
int,
int,
char *,
char *,
float,
char
#endif
);

