
/* constant definitions */
#define buffer_length 141
#define NUM_SITES 9
#define PROB 50
#define FILE_A "SLCESPSLR"

static char mon[][12] = {"jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"};

/* to modify the sites and datatype processed edit the following three lines */
/* What about navajo?? Need which forecast? */
static char str_array[][30] = {"GLDA3","BMDC2","MPSC2","CLSC2","GBRW4",
				 "GRNU1","VCRC2","NVRN5","TPIC2"};

static ID site_id[] = {919,913,914,915,916,917,933,920,912};
static ID datatype_id[] = {33,33,33,33,29,33,29,123,29};
static ID datatype_id_tot[] = {34,34,34,34,30,34,30,124,30};

/* declare functions; support ANSI or K and R C. */

int NumDaysMonth(
#ifdef ANSIPROTO
SQL_DATE,
int *
#endif
);

