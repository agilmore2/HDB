/* this file contains includes for dmi utilities. The typedefs are
   put in this file b/c the the ingres parser cannot handle typedefs
   with members of themselves.
*/

#ifndef DMI_INC
#define DMI_INC 1

#include "typedefs.h"

#ifndef ORA_PROC
#include "tcl.h"
#include "tk.h"
#endif

/* Defaults */
#define NUM_HIST_TIMESTEPS 1
#define REAL_DATA_TABLE -1 

#define USER_FLAG "-U"
#define SYSTEM_FLAG "-S"

#define USER_QUIT -1

/* Define all user command line argument keywords here. */
#define MODEL_RUN "model_source"

/* Extern all user and system command line argument variables here. */
extern ID model_run_id;

typedef struct {
    int session_number;
    char dbsite_db_name[DATABASE_NAME_LENGTH + 1];
    char dbsite_alias[DATABASE_ALIAS_LENGTH + 1];
    int connected;
    int maxid;
} dbsite_member;

extern int number_of_db_sites;
extern dbsite_member *dbsite_array;

typedef struct { 
   char is_nan;
   double value;
   SQL_DATE hdb_date;
   SQL_DATE end_date;
   char processed_flag;
} data_array;

typedef struct dmi_header {
   /* mandatory items */
   char *pr_datafile_name;
   char *object_name;
   char *slot_name;
   char pr_runcontrol_startdate[20];
   char hdb_startdate[25];
   char pr_runcontrol_enddate[20]; 
   char hdb_enddate[25];
   int hdb_site_datatype_id;

   /* units are mandatory for output DMI, but input DMI will write out a */
   /* unit specifier in the data file that corresponds to the units in HDB */
   char * pr_units;

   /* stuff for the input DMI */
   int number_of_timesteps;
   char hdb_hist_data_enddate[25];     /* used only by input DMI */
   char hdb_model_data_startdate[25];  /* used only by input DMI */
   int number_of_hist_timesteps;       /* used only by input DMI */
   int number_of_model_timesteps;      /* used only by input DMI */
   char data_location[3];              /* LC, UC, etc. */
   char * load_model_data;             /* 'Y' or 'N' -- input DMI only   */
   int hist_source;  /* indicates "r_" table or modelRunId; input DMI only */
   int model_source;                   /* modelRunId; input DMI only */
   int destination;  /* output data to r_ or m_tables */
   int model_destination; /* model_run_id ffor output of data */

   /* not used -- for future enhancements */
   int hdb_unit_type;
   char pr_scale_factor[11];
   char timestep[11]; 

   /* the data for the object/slot */
   data_array *data; 

   struct dmi_header *next;
} dmi_header_struct;

#define OBJECT_NAME_LEN 240
#define SLOT_NAME_LEN 200

typedef struct {
   char object_name[OBJECT_NAME_LEN + 1];   char slot_name[SLOT_NAME_LEN + 1];
   int site_datatype_id;
   char data_location[3]; /* either UC or LC at the moment */
} object_slot_lookup_member;

extern object_slot_lookup_member *object_slot_lookup_array;

#define UNIT_NAME_LEN 32

typedef struct {
    char riverware_unit_name[UNIT_NAME_LEN + 1];
    int hdb_unit_id;
    int scale;
} unit_lookup_member;

extern unit_lookup_member *unit_lookup_array;

#define TIMESTEP_NAME_LEN 10

typedef struct {
    char riverware_timestep_name[10];
    TIME_INTERVAL_TYPES hdb_timestep;
  } timestep_lookup;

extern timestep_lookup timestep_lookup_array[];

int riverwareToHdbDate(
#ifdef ANSIPROTO
  char *prysmDate,
  SQL_DATE hdbDate,
  char *timestep
#endif
);

int hdbToRiverWareDate(
#ifdef ANSIPROTO
  SQL_DATE hdbDate,
  char *prysmDate,
  char *timestep
#endif
);

int DmiCharToNumericDate(
#ifdef ANSIPROTO
  char *,
  char *
#endif
);

int SetListStartEndDates(
#ifdef ANSIPROTO
  char *,
  char *,
  char *,			
  dmi_header_struct *,
  int
#endif
);

int InitializeOutputMember (
#ifdef ANSIPROTO
  dmi_header_struct *
#endif
);

int InitializeInputMember(
#ifdef ANSIPROTO
  dmi_header_struct *
#endif
);

int GetDataStartDate(
#ifdef ANSIPROTO
  ID,
  int,
  char *,
  SQL_DATE,
  SQL_DATE
#endif
);


int LookupSiteDatatype(
#ifdef ANSIPROTO
  char *,
  char *,
  int *
#endif
);

int LookupLocation(
#ifdef ANSIPROTO
  int,
  char *
#endif
);

int LookupObjectSlot(
#ifdef ANSIPROTO
  int,
  char *,
  char *
#endif
);

int LookupTimestep(
#ifdef ANSIPROTO
  char *,
  TIME_INTERVAL_TYPES *		   
#endif
);

int DmiVerifyUnits(
#ifdef ANSIPROTO
  char * riverware_unit_name,
  int hdb_site_datatype_id
#endif
);

int readInputControlFile(
#ifdef ANSIPROTO
  char *controlFile,
  dmi_header_struct ** list,
  int *TkInterfaceNeeded
#endif
);

int readOutputControlFile(
#ifdef ANSIPROTO
  char *controlFile,
  dmi_header_struct ** list
#endif
);

int SetPointersDeleteItem(
#ifdef ANSIPROTO
  dmi_header_struct **,
  dmi_header_struct **,
  dmi_header_struct **
#endif
);

int FreeDmiHeaderMember(
#ifdef ANSIPROTO
  dmi_header_struct *
#endif
);

int DmiLookupRiverWareUnitsForSDI(
#ifdef ANSIPROTO
  int,
  char **
#endif
);

int DmiLookupRiverWareUnitScale (
#ifdef ANSIPROTO
  int,
  char *,
  int *		 
#endif
);

int DmiLoadMappings(
#ifdef ANSIPROTO
#endif
);

int SqlLoadDbSiteInfo(
#ifdef ANSIPROTO
#endif
);

int SqlLoadObjectSlotSDIMappings(
#ifdef ANSIPROTO
#endif
);

int SqlLoadRiverWareHdbUnitMappings(
#ifdef ANSIPROTO
#endif
);

int GetOptionalArgs(
#ifdef ANSIPROTO
char **,
int
#endif
);

int GetSystemArgs(
#ifdef ANSIPROTO
char **,
int,
int *
#endif
);

int GetUserArgs(
#ifdef ANSIPROTO
char **,
int
#endif
);

int FindUserArg(
#ifdef ANSIPROTO
char **,
int,
char *,
char *
#endif
);

/* The following defines, typedefs, and procedure pre-declarations are
   made for createModelmap.sc. 
   Augest, 1996
*/

#define READCODE "/tmp/readCode.hdb"
#define READMODEL "/tmp/readModel.hdb"
#define WRITESPEC "/tmp/writeSpec.hdb"
#define SAVECHOICE "/tmp/savechoice.hdb"
#define ERROR1 "/tmp/error1.hdb"
#define ERROR2 "/tmp/error2.hdb"

#define MAPSIZE 1000

#define QUIT       0
#define NOSPECDATA 1
#define SPECDATA   2
#define CREATE     3
#define SEARCH     4

#define CREATESUC  "CS"
#define CREATEFAI  "CF"
#define SEARCHSUC  "SS"
#define SEARCHFAI  "SF"

#define UNUSED    65535

typedef struct {
    char code[7];
    int modelRunId;
  } model_run_map;

typedef struct {
    char name[100];
    int modelId;
  } model_map;

typedef struct {
    char code[7];
    int modelRunId;
    int modelId;
    char sysDate[26];
    char runDate[26];
    int prob;
    char modelType[6];
    int userId;
    char startDate[26];
    char endDate[26];
    char timeStep[20];
    char Name[100];
    char Comment[1000];
  } model_run;

/*int SqlGetModelIdArray(
#ifdef ANSIPROTO
  model_map *,
  int *
#endif
);
*/
int WriteToFiles(
#ifdef ANSIPROTO
  model_run *,
  int count,
  model_run_map *,
  int counter
#endif
);

int ReadTkFile(
#ifdef ANSIPROTO
  model_run_map *,
  int count
#endif
);

int GetSpecData(
#ifdef ANSIPROTO
  model_run *,
  int *,
  int *
#endif
);

int SqlSearchModelRunIdInput(
#ifdef ANSIPROTO
  model_run *,
  model_run *,
  int *
#endif
);

int mapList(
#ifdef ANSIPROTO
  dmi_header_struct **
#endif
);

int GetSpecData1(
#ifdef ANSIPROTO
  model_run *,
  int *,
  int *
#endif
);

int HDBTk_createModelmap(
#ifdef ANSIPROTO
  ClientData, 
  Tcl_Interp *, 
  int, 
  char **
#endif
);

int HDBTk_readModelMap(
#ifdef ANSIPROTO
  ClientData, 
  Tcl_Interp *, 
  int, 
  char **
#endif
);

int HDBTk_formatModelMapLine(
#ifdef ANSIPROTO
  ClientData, 
  Tcl_Interp *, 
  int, 
  char **
#endif
);

int HDBTk_condenseData(
#ifdef ANSIPROTO
  ClientData,
  Tcl_Interp *,
  int,
  char **
#endif
);

int HDBTk_stringAppend(
#ifdef ANSIPROTO
  ClientData,
  Tcl_Interp *,
  int,
  char **
#endif
);

int HDBTk_StartSearchOutput(
#ifdef ANSIPROTO
  ClientData,
  Tcl_Interp *,
  int,
  char **
#endif
);

int HDBTk_StartSearchInput(
#ifdef ANSIPROTO
  ClientData,
  Tcl_Interp *,
  int,
  char **
#endif
);

int HDBTk_StartCreate(
#ifdef ANSIPROTO
  ClientData,
  Tcl_Interp *,
  int,
  char **
#endif
);

int FillSpecDataOutput(
#ifdef ANSIPROTO
  char *,
  model_run *
#endif
);

int parseDataLine(
#ifdef ANSIPROTO
  char *,
  int, 
  char *
#endif
);

int FillSpecDataInput(
#ifdef ANSIPROTO
  char *,
  model_run *
#endif
);

int HDBTk_SaveModelRunIdOutput(
#ifdef ANSIPROTO
  ClientData,
  Tcl_Interp *, 
  int,
  char **
#endif
);

int HDBTk_SaveModelRunIdInput(
#ifdef ANSIPROTO
  ClientData,
  Tcl_Interp *, 
  int,
  char **
#endif
);

int CondenseSearchResultsOutput(
#ifdef ANSIPROTO
  Tcl_Interp *,
  model_run *,
  int
#endif
);

int CondenseSearchResultsInput(
#ifdef ANSIPROTO
  Tcl_Interp *,
  model_run *,
  int
#endif
);

int ReadTkFile1(
#ifdef ANSIPROTO
#endif
);

void rmTextFile(
#ifdef ANSIPROTO
#endif
);

int SqlSearchModelRunIdOutput(
#ifdef ANSIPROTO
  model_run *,
  model_run *,
  int *
#endif
);

int SyncModelRunId(
#ifdef ANSIPROTO
#endif
);

int SqlUpdateModelRunIds(
#ifdef ANSIPROTO
int, int, int, int
#endif
);

int StartTkOutputInterface(
#ifdef ANSIPROTO
  char *
#endif
);

int StartTkInputInterface(
#ifdef ANSIPROTO
  char *
#endif
);

/*  Synchronization function for Model Run Id */
/*  SqlSyncModelrun()                         */
typedef struct{
     char dbsite_code[DATABASE_ALIAS_LENGTH+1];
     int maxModelRun;
}dbsite_modelrun;

typedef struct{
     char dbsite_code[DATABASE_ALIAS_LENGTH+1];
     dbsite_modelrun record[50];
}dbsite_modelrun_record;

#define ID_RANGE  100

int SqlSyncModelRuns(
#ifdef ANSIPROTO
#endif
);

int SqlCreateModelRunId(
#ifdef ANSIPROTO
 model_run *,
 int *
#endif
);

int SqlSetAutocommitOn(
#ifdef ANSIPROTO
#endif
);

int SqlSetAutocommitOff(
#ifdef ANSIPROTO
#endif
);

int SqlUpdateSys_date(
#ifdef ANSIPROTO
 int
#endif
);

model_run_map modelRunMap[MAPSIZE];
int modelRunMapCount;
dmi_header_struct **modelRunMapList;
/* I changed these variables to global variables so that C functions that
   are called from TCL can read them. These used to be only declared in the
   readControlFile.c file. */
#endif
