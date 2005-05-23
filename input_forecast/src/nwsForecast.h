
/* constant definitions */
#define buffer_length 134
#define NUM_SITES 7
#define PROB 50
#define FILE_A "SLCESPSLR"

static char mon[][15] = {"oct","nov","dec","jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"};

/* to modify the sites and datatype processed edit the following three lines */

static char str_array[][30] = {"glda3 :lake powell   ","sapc2 :blue mesa     ","clsc2 :crystal unreg*","gbrw4 :fontenelle    ",
				 "grnu1 :flaming gorge ","vcrc2 :vallecito     ","nvrn5 :navajo regltd "};

static ID site_id[] = {919,913,915,916,917,933,920};
static ID datatype_id[] = {33,33,33,29,33,29,29};
static ID datatype_id_tot[] = {34,34,34,30,34,30,30};

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

int NumDaysMonth(
#ifdef ANSIPROTO
SQL_DATE,
int *
#endif
);

