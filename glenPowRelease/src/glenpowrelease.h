#define NUMDATATYPES 4
#define HOURS 24
ID agen_id,collection_system_id,loading_application_id,method_id,computation_id;

/* declare functions; support ANSI or K and R C. */

int insertAVM(
#ifdef ANSIPROTO
SQL_DATE *,
int,
float *,
char *
#endif
);

int SqlGetAVMData(
#ifdef ANSIPROTO
        char*,
        char*,
        char*
#endif
);
