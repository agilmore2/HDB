/* inflow_24mo.h stores chacteristics and infomation that will determine the behavior
   of the program inflow_24mo  */

#define NUM_SITES 9 /* define the number of sites to be processed */

#define NWS_MODEL_ID 1  /* hard wired to National Weather Service - NRCS forecast */
#define AOP_MODEL_ID 2  /* hard wired to National Weather Service - NRCS forecast */

/* define charaters that will be used to populate the arrays that determine how to develop inflows 
   these will be used later when the variable driver (which is a structure) is populated */

#define F 0   /* F -> use forecasted monthly value */
#define H 1   /* H -> use statistical value for appropriate hydrology */
#define I 2   /* I -> interpolate */
#define A 3   /* A -> disaggregate from April through July volume */
#define N 4   /* N -> forecast not available - use a neighboring site for this value */
#define M 5   /* M -> most probable inflow */

/* also define strings to correspond to the above characters */

#define FORECAST       0   /* F -> use forecasted monthly value */
#define HISTORICAL     1   /* H -> use statistical value for appropriate hydrology */
#define INTERPOLATE    2   /* I -> interpolate */
#define APR_JUL_DISAGG 3   /* A -> disaggregate from April through July volume */
#define NEIGHBOR       4   /* N -> forecast not available - use a neighboring site for this value */
#define MOST_PROBABLE  5   /* M -> most probable inflow */

/* defines for probability */

#define MOST 1
#define MAX 2
#define MIN 3

/* defines for model type */

#define PRELIM 1
#define FINAL 2
#define MID 3

/* these defines relate to where stats are stored in the database
   most, max, and min data are stored in the table m_monthstat
   with model_run_ids of 22, 20 and 21 for min, most and max respectively */

#define MODEL_RUN_ID_STATS_MIN 22
#define MODEL_RUN_ID_STATS_MOST 20
#define MODEL_RUN_ID_STATS_MAX 21

/* define a structure HYDROLOGY that has three integers in it */

typedef struct hydrology {
  int most;
  int max;
  int min;
} HYDROLOGY;

/* define a structure MO_INPUT_SPEC which has 12 HYDROLOGY structures nested in it */

typedef struct mo_input_spec {
  HYDROLOGY jan_run[12];
  HYDROLOGY feb_run[12];
  HYDROLOGY mar_run[12];
  HYDROLOGY apr_run[12];
  HYDROLOGY may_run[12];
  HYDROLOGY jun_run[12];
  HYDROLOGY jul_run[12];
  HYDROLOGY aug_run[12];
  HYDROLOGY sep_run[12];
  HYDROLOGY oct_run[12];
  HYDROLOGY nov_run[12];
  HYDROLOGY dec_run[12];
} MO_INPUT_SPEC;

/* define another structure FULL_INPUT_SPEC - FULL_INPUT_SPEC which has 2 ints, a char, two more ints, and 
   then two MO_INPUT_SPEC - as you can see on variable of type FULL_INPUT_SPEC hold lots of information */

typedef struct full_input_spec {
  int site_id;
  int datatype_id;
  char use;
  int neighbor_site_id; /* the site_id of the neighbor to use if the forecast is not available */
  int neighbor_datatype_id; /* the datatype_id of the neighbor to use if the forecast is not available */
  MO_INPUT_SPEC defaulter;
  MO_INPUT_SPEC special;
} FULL_INPUT_SPEC;

/* define and populate static variable months, probc, typec, and method */

static char months[][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static char probc[][5] = {"Nada","Most","Max ","Min "};
static char typec[][12] = {"Nada       ","Preliminary","Final      ","Mid Month  "};
static char method[][20] = {"Forecasted         ","Historical Stats   ","Interpolated       ","Disagg from Apr-Jul","Neighbor           ","Most Probable      "};

/* define and populate ID variables for model ids and model_run_ids */

ID nws_model_id = NWS_MODEL_ID;
ID aop_model_id = AOP_MODEL_ID;

char model_run_name_most[] = "Most Probable 24 Month Study";
char model_run_name_max[] = "Maximum Probable 24 Month Study";
char model_run_name_min[] = "Minimum Probable 24 Month Study";

/* now the big part - we define and populate the variable driver, which is a FULL_INPUT_SPEC structure */

/* Driver is populated  in this way.  Site_id is the site_id.  I.E. 916 for Fontenelle.  datatype_id is datatype_id.
   Here we need to use 29 for inflow and 33 for unregulated inflow.  Fontenelle, Taylor Park and Navajo get 29 while
   the rest get a 33.  Then comes use.  This is a single character which needs to be either a 'd' for default, or a
   's' for special.  If it's a 'd' the program will use the defaulter portion of the stucture when it runs if it is
   a 's', the program will use the special portion.  In this way you can have two setups for each reservoir.  To toggle
   you must change this variable.  Next we must populate all the HYDROLOGY structures nested within the FULL_INPUT_SPEC
   structure.  The HYDROLOGY structure has three integers which correspond to most, max and min.  These are populated
   with constants.  Currently threre are 5 possibilities: F, H, I, A, and N.  For descritions see the previous section
   of this .h file.  The MO_INPUT_SPEC structure within the FULL_INPUT_SPEC structure has jan_run, feb_run ... dec_run.
   For each of these the user must populate the HYDROLOGY structure for the behavior.  I.E {F,H,H} means that for the
   most probable model a forecasted value is used, but for the min and maximum probable historical values will be used.
   The program will look in the database for a forecasted value for the most probable case.  The program will retrieve
   historical stats from the database for the max and min case.  Understood? */

FULL_INPUT_SPEC driver[] = 
{
  {916,29,'d',998877,998877, /* 916 is for Fontenelle */
     {  
       /*                                            Default Values     */
     
       /*                     Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   
     
/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Feb 24 Month Study */  { {H,M,M},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Mar 24 Month Study */  { {H,M,M},{H,M,M},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Apr 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* May 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Jun 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,M,M},{H,M,M},{H,M,M} },
/* Jul 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,I,I},{F,I,I},{I,M,M},{I,M,M},{H,M,M} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			     },
   {

                                                 /*      Special  Values     */

  	              /*      Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   

/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Feb 24 Month Study */  { {H,H,H},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Mar 24 Month Study */  { {H,H,H},{H,H,H},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Apr 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* May 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Jun 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,H,H},{H,H,H},{H,H,H} },
/* Jul 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,I,I},{F,I,I},{I,H,H},{I,H,H},{H,H,H} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			}
 },
  {917,33,'d',998877,998877, /* 917 is for Flaming Gorge*/
     {  
       /*                                            Default Values     */
     
       /*                     Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   
     
/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Feb 24 Month Study */  { {H,M,M},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Mar 24 Month Study */  { {H,M,M},{H,M,M},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Apr 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* May 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Jun 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,M,M},{H,M,M},{H,M,M} },
/* Jul 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,I,I},{F,I,I},{I,M,M},{I,M,M},{H,M,M} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			     },
   {

                                                 /*      Special  Values     */

  	              /*      Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   

/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Feb 24 Month Study */  { {H,H,H},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Mar 24 Month Study */  { {H,H,H},{H,H,H},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Apr 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* May 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Jun 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,H,H},{H,H,H},{H,H,H} },
/* Jul 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,I,I},{F,I,I},{I,H,H},{I,H,H},{H,H,H} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			}
 },
  {912,29,'d',913,33, /* 912 is for Taylor Park (note uses Blue Mesa, 913 for inflow for all those marked N */
     {  
       /*                                            Default Values     */
     
       /*                     Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   
     
/* Jan 24 Month Study */  { {N,N,N},{N,N,N},{N,N,N},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Feb 24 Month Study */  { {H,M,M},{N,N,N},{N,N,N},{N,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Mar 24 Month Study */  { {H,M,M},{H,M,M},{N,N,N},{N,A,A},{N,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Apr 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{N,A,A},{N,A,A},{N,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* May 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{N,A,A},{N,A,A},{N,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Jun 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{N,A,A},{N,A,A},{N,I,I},{I,I,I},{I,M,M},{H,M,M},{H,M,M} },
/* Jul 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{N,A,A},{N,I,I},{N,I,I},{I,M,M},{I,M,M},{H,M,M} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H} },
/* Nov 24 Month Study */  { {N,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H} },
/* Dec 24 Month Study */  { {N,H,H},{N,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H} }
			     },
   {

                                                 /*      Special  Values     */

  	              /*      Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   

/* Jan 24 Month Study */  { {N,N,N},{N,N,N},{N,N,N},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Feb 24 Month Study */  { {H,H,H},{N,N,N},{N,N,N},{N,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Mar 24 Month Study */  { {H,H,H},{H,H,H},{N,N,N},{N,A,A},{N,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Apr 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{N,A,A},{N,A,A},{N,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* May 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,A,A},{N,A,A},{N,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Jun 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,A,A},{N,A,A},{N,I,I},{I,I,I},{I,H,H},{H,H,H},{H,H,H} },
/* Jul 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,A,A},{N,I,I},{N,I,I},{I,H,H},{I,H,H},{H,H,H} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H} },
/* Nov 24 Month Study */  { {N,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H} },
/* Dec 24 Month Study */  { {N,H,H},{N,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H} }
			}
 },
  {913,33,'d',998877,998877, /* 913 is for Blue Mesa */
     {  
       /*                                            Default Values     */
     
       /*                     Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   
     
/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Feb 24 Month Study */  { {H,M,M},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Mar 24 Month Study */  { {H,M,M},{H,M,M},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Apr 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* May 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Jun 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,M,M},{H,M,M},{H,M,M} },
/* Jul 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,I,I},{F,I,I},{I,M,M},{I,M,M},{H,M,M} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			     },
   {

                                                 /*      Special  Values     */

  	              /*      Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   

/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Feb 24 Month Study */  { {H,H,H},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Mar 24 Month Study */  { {H,H,H},{H,H,H},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Apr 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* May 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Jun 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,H,H},{H,H,H},{H,H,H} },
/* Jul 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,I,I},{F,I,I},{I,H,H},{I,H,H},{H,H,H} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			}
 },
  {914,33,'d',913,33, /* 914 is for Morrow Point (note uses Blue Mesa, 913 for inflow for all those marked N */
     {  
       /*                                            Default Values     */
     
       /*                     Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   
     
/* Jan 24 Month Study */  { {N,N,N},{N,N,N},{N,N,N},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Feb 24 Month Study */  { {H,M,M},{N,N,N},{N,N,N},{N,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Mar 24 Month Study */  { {H,M,M},{H,M,M},{N,N,N},{N,A,A},{N,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Apr 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{N,A,A},{N,A,A},{N,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* May 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{N,A,A},{N,A,A},{N,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Jun 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{N,A,A},{N,A,A},{N,I,I},{I,I,I},{I,M,M},{H,M,M},{H,M,M} },
/* Jul 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{N,A,A},{N,I,I},{N,I,I},{I,M,M},{I,M,M},{H,M,M} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H} },
/* Nov 24 Month Study */  { {N,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H} },
/* Dec 24 Month Study */  { {N,H,H},{N,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H} }
			     },
   {

                                                 /*      Special  Values     */

  	              /*      Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   

/* Jan 24 Month Study */  { {N,N,N},{N,N,N},{N,N,N},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Feb 24 Month Study */  { {H,H,H},{N,N,N},{N,N,N},{N,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Mar 24 Month Study */  { {H,H,H},{H,H,H},{N,N,N},{N,A,A},{N,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Apr 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{N,A,A},{N,A,A},{N,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* May 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,A,A},{N,A,A},{N,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Jun 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,A,A},{N,A,A},{N,I,I},{I,I,I},{I,H,H},{H,H,H},{H,H,H} },
/* Jul 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,A,A},{N,I,I},{N,I,I},{I,H,H},{I,H,H},{H,H,H} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H},{N,H,H} },
/* Nov 24 Month Study */  { {N,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H},{N,H,H} },
/* Dec 24 Month Study */  { {N,H,H},{N,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{N,H,H} }
			}
 },
  {915,33,'d',998877,998877, /* 915 is for Crystal */
     {  
       /*                                            Default Values     */
     
       /*                     Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   
     
/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Feb 24 Month Study */  { {H,M,M},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Mar 24 Month Study */  { {H,M,M},{H,M,M},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Apr 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* May 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Jun 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,M,M},{H,M,M},{H,M,M} },
/* Jul 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,I,I},{F,I,I},{I,M,M},{I,M,M},{H,M,M} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			     },
   {

                                                 /*      Special  Values     */

  	              /*      Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   

/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Feb 24 Month Study */  { {H,H,H},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Mar 24 Month Study */  { {H,H,H},{H,H,H},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Apr 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* May 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Jun 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,H,H},{H,H,H},{H,H,H} },
/* Jul 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,I,I},{F,I,I},{I,H,H},{I,H,H},{H,H,H} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			}
 },
  {920,123,'d',998877,998877, /* 920 is for Navajo */
     {  
       /*                                            Default Values     */
     
       /*                     Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   
     
/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Feb 24 Month Study */  { {H,M,M},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Mar 24 Month Study */  { {H,M,M},{H,M,M},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Apr 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* May 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Jun 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,M,M},{H,M,M},{H,M,M} },
/* Jul 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,I,I},{F,I,I},{I,M,M},{I,M,M},{H,M,M} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			     },
   {

                                                 /*      Special  Values     */

  	              /*      Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   

/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Feb 24 Month Study */  { {H,H,H},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Mar 24 Month Study */  { {H,H,H},{H,H,H},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Apr 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* May 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Jun 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,H,H},{H,H,H},{H,H,H} },
/* Jul 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,I,I},{F,I,I},{I,H,H},{I,H,H},{H,H,H} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			}
 },
  {919,33,'d',998877,998877, /* 919 is for Glen Canyon */
     {  
       /*                                            Default Values     */
     
       /*                     Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   
     
/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Feb 24 Month Study */  { {H,M,M},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Mar 24 Month Study */  { {H,M,M},{H,M,M},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Apr 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* May 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Jun 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,M,M},{H,M,M},{H,M,M} },
/* Jul 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,I,I},{F,I,I},{I,M,M},{I,M,M},{H,M,M} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			     },
   {

                                                 /*      Special  Values     */

  	              /*      Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   

/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Feb 24 Month Study */  { {H,H,H},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Mar 24 Month Study */  { {H,H,H},{H,H,H},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Apr 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* May 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Jun 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,H,H},{H,H,H},{H,H,H} },
/* Jul 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,I,I},{F,I,I},{I,H,H},{I,H,H},{H,H,H} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			}
  },
  {933,29,'d',998877,998877, /* 933 is for Vallecito */
     {  
       /*                                            Default Values     */
     
       /*                     Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   
     
/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Feb 24 Month Study */  { {H,M,M},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Mar 24 Month Study */  { {H,M,M},{H,M,M},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Apr 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* May 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,M,M},{H,M,M},{H,M,M} },
/* Jun 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,M,M},{H,M,M},{H,M,M} },
/* Jul 24 Month Study */  { {H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{H,M,M},{F,A,A},{F,I,I},{F,I,I},{I,M,M},{I,M,M},{H,M,M} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
			     },
   {

                                                 /*      Special  Values     */

  	              /*      Jan     Feb     Mar     Apr     May     Jun     Jul     Aug     Sep     Oct     Nov     Dec */   

/* Jan 24 Month Study */  { {F,F,F},{F,F,F},{F,F,F},{A,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Feb 24 Month Study */  { {H,H,H},{F,F,F},{F,F,F},{F,A,A},{A,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Mar 24 Month Study */  { {H,H,H},{H,H,H},{F,F,F},{F,A,A},{F,A,A},{A,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Apr 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{A,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* May 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,A,A},{I,I,I},{I,I,I},{H,H,H},{H,H,H},{H,H,H} },
/* Jun 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,A,A},{F,I,I},{I,I,I},{I,H,H},{H,H,H},{H,H,H} },
/* Jul 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,A,A},{F,I,I},{F,I,I},{I,H,H},{I,H,H},{H,H,H} },
/* Aug 24 Month Study */  { {H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H},{I,H,H} },
/* Sep 24 Month Study */  { {I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H},{I,H,H} },
/* Oct 24 Month Study */  { {I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H},{F,H,H} },
/* Nov 24 Month Study */  { {F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H},{F,H,H} },
/* Dec 24 Month Study */  { {F,H,H},{F,H,H},{I,H,H},{I,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{H,H,H},{F,H,H} }
   }
  }
};


int get_monthly_model_data(
#ifdef ANSIPROTO
ID,
ID,
SQL_DATE,
SQL_DATE,
char *,
int,
VALUE *,
ID
#endif
);

int get_monthly_data(
#ifdef ANSIPROTO
ID,
ID,
SQL_DATE,
VALUE *
#endif
);

int get_monthly_model_range_data(
#ifdef ANSIPROTO
ID,
ID,
SQL_DATE,
SQL_DATE,
SQL_DATE,
char *,
int,
VALUE *,
ID
#endif
);

int get_monthly_stats_data(
#ifdef ANSIPROTO
ID,
ID,
ID,
VALUE *
#endif
);

int load_monthly_model_data(
#ifdef ANSIPROTO
ID,
ID,
ID,
SQL_DATE,
VALUE
#endif
);

int do_apr_jul(
#ifdef ANSIPROTO
VALUE *,
float *,
float,
int,
int
#endif
);

int interpo_late(
#ifdef ANSIPROTO
VALUE *,
int,
float *,
int,
int
#endif
);

int checkforModelrunid(
#ifdef ANSIPROTO
ID,
SQL_DATE,
int,
char *,
int *
#endif
);

void modify_to_lower(
#ifdef ANSIPROTO
char *
#endif
);

void modify_to_upper(
#ifdef ANSIPROTO
char *
#endif
);
