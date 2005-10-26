#define NUMDATATYPES 3
#define HOURS 24
ID agen_id,collection_system_id,loading_application_id,method_id,computation_id;

/* declare functions; support ANSI or K and R C. */

int insertAVM(
#ifdef ANSIPROTO
char **,
int,
double *,
char *
#endif
);

int SqlGetTotData(
#ifdef ANSIPROTO
        char*
#endif
);
