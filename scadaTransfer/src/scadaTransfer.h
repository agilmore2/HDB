#define NUM_SITES 6
#define NUM_DATATYPES 7
#define NUM_HOURS 24
#define BUFFER_LENGTH 132
static char *reservoirName[NUM_SITES] = {"GLEN",
					   "MORROW",
					   "BLUE",
					   "CRYSTAL",
					   "FLAMING",
					   "FONTENELLE"};
static char *datatypeName[NUM_DATATYPES] = {"water_surface_elev",
					      "head",
					      "generation",
					      "pwr_releases",
					      "combined_releases",
					      "bypass",
					      "spill"};

ID agen_id,collection_system_id,loading_application_id,method_id,computation_id;

/* declare functions; support ANSI or K and R C. */

int insertScada(
#ifdef ANSIPROTO
SQL_DATE *,
int,
double *,
ID,
char *
#endif
);
/*
int SqlSetDateFormat(
#ifdef ANSIPROTO
		     int
#endif
);
*/
