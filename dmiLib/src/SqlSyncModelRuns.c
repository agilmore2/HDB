
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned long magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[20];
};
static const struct sqlcxp sqlfpn =
{
    19,
    "SqlSyncModelRuns.pc"
};


static unsigned long sqlctx = 42757939;


static struct sqlexd {
   unsigned int   sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
            void  *sqhstv[13];
   unsigned int   sqhstl[13];
            int   sqhsts[13];
            void  *sqindv[13];
            int   sqinds[13];
   unsigned int   sqharm[13];
   unsigned int   *sqharc[13];
   unsigned short  sqadto[13];
   unsigned short  sqtdso[13];
} sqlstm = {10,13};

/* SQLLIB Prototypes */
extern void sqlcxt (void **, unsigned long *,
                    struct sqlexd *, const struct sqlcxp *);
extern void sqlcx2t(void **, unsigned long *,
                    struct sqlexd *, const struct sqlcxp *);
extern void sqlbuft(void **, char *);
extern void sqlgs2t(void **, char *);
extern void sqlorat(void **, unsigned long *, void *);

/* Forms Interface */
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern void sqliem(char *, int *);

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,0,0,1041,268,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
28,0,0,1,0,0,1069,286,0,0,0,0,0,0,128,1,97,0,0,
47,0,0,1,0,0,1037,304,0,0,4,0,0,0,128,2,97,0,0,2,3,0,0,2,3,0,0,2,3,0,0,1,97,0,
0,
82,0,0,1,0,0,1039,313,0,0,0,0,0,0,128,1,97,0,0,
101,0,0,1,0,0,1039,340,0,0,0,0,0,0,128,1,97,0,0,
120,0,0,2,0,0,1053,413,0,0,0,0,0,0,128,1,97,0,0,
139,0,0,3,0,0,1041,573,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
162,0,0,3,0,0,1069,591,0,0,0,0,0,0,128,1,97,0,0,
181,0,0,3,0,0,1037,603,0,0,12,0,0,0,128,2,3,0,0,2,97,0,0,2,3,0,0,2,97,0,0,2,97,
0,0,2,3,0,0,2,97,0,0,2,3,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,1,97,0,0,
248,0,0,3,0,0,1039,614,0,0,0,0,0,0,128,1,97,0,0,
267,0,0,3,0,0,1039,625,0,0,0,0,0,0,128,1,97,0,0,
286,0,0,4,114,0,1027,638,0,0,12,12,0,0,128,1,3,0,0,1,97,0,0,1,3,0,0,1,97,0,0,1,
97,0,0,1,3,0,0,1,97,0,0,1,3,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
353,0,0,5,0,0,1055,650,0,0,0,0,0,0,128,1,97,0,0,
372,0,0,3,0,0,1039,666,0,0,0,0,0,0,128,1,97,0,0,
391,0,0,3,0,0,1039,675,0,0,0,0,0,0,128,1,97,0,0,
410,0,0,3,0,0,1039,690,0,0,0,0,0,0,128,1,97,0,0,
429,0,0,6,62,0,1029,700,0,0,2,2,0,0,128,1,3,0,0,1,97,0,0,1,97,0,0,
456,0,0,7,0,0,1055,704,0,0,0,0,0,0,128,1,97,0,0,
};


/*  IDENTIFICATION SECTION

File Name: SqlSyncModelRuns.sc
 
Date Written: November, 1996/ June 1997
     Ported to Oracle: Rene Reitsma August 1998
  
Author(s): Yue Wu - Rene Reitsma, CADSWES
  
Module Description:
  Three routines to keep model run id in sync between databases
  for generic usage (input DMI and output DMI) and model run id 
  creation. 

****************************

int SqlSyncModelRuns()

Purpose: syncs model runids between sites. 

Relies on two environment variables:

     HDB_ROLE_PASSW: filename of the role password file

     HDB_LOCAL: name of the local database; e.g., slcdg1::uchdb

Returns OK on success, ERROR on failure.

Can be called directly from main.

****************************

int SqlUpdateModelRunIds(int site_no, int other_site_no,
                         int current_max_sync_id,
                         int new_max_sync_id): 

Purpose: 
Syncs site site_no with site other_site_no. Copies missing runid info 
from other_site_no into site_no and updates site_no's max_sync_id. 

Arguments:
int site_no: site: (session) to be updated
int other_site_no: site (session) where runinfo resides
int current_max_sync_id: site_no's current value for other_site_no's
                         max_sync_id.
int new_max_sync_id: other_site_no's mx_id.

Is called from SqlSyncModelRuns.

******************************************************************/

/* standard includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>

/* HDB includes */
#include <defines.h>
#include <dbutils.h>
#include <utils.h>

/* application-specific include */
#include "dmi_utils.h"

/* EXEC SQL INCLUDE sqlca;
 */ 
/*
 * $Header$ sqlca.h 
 */

/* Copyright (c) 1985,1986, 1998 by Oracle Corporation. */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   07/31/98 -  long to int
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ int     sqlabc;
         /* b4  */ int     sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ int     sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */


#define STATEMENT_LEN  10000

#define FUNC_NAME "SqlSyncModelRuns"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlSyncModelRuns

<P> <STRONG>Function signature:</STRONG> int SqlSyncModelRuns(void)

<P> <STRONG>Function description:</STRONG> Synchronizes existing model_run_ids between all indicated HDB locations.

<P><DL> <STRONG>Function arguments:</STRONG>
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlSyncModelRuns(void)
{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

  char siteCode[DATABASE_ALIAS_LENGTH+1],
       operateStmt[STATEMENT_LEN+1];
  int  maxId, maxSyncId, siteCodeSize;
  short int  maxIdNull, maxSyncIdNull;
  /* EXEC SQL END DECLARE SECTION; */ 


  int site_no, other_site_no;
  int *maxids, **max_sync_ids;
  int i, j, result, num_rec, did_commits;

  char *dbName = NULL;
  char *appRole = NULL;
  char *appUser = NULL;
  char *modelRole = NULL;

  printf("Synchronizing model runs...\n\n");

/* get all the necessary environmental variables */

  dbName = getenv("HDB_LOCAL");
  if (dbName == NULL)
  {
     PrintError("Environment variable HDB_LOCAL not set...\n");
     return(ERROR);
  }

  appRole = getenv ("APP_ROLE");
  if (appRole == NULL)
  {
     PrintError("Environment variable APP_ROLE not set...\n");
     return(ERROR);
  }

  appUser = getenv ("APP_USER");
  if (appUser == NULL)
  {
     PrintError("Environment variable APP_USER not set...\n");
     return(ERROR);
  }

  modelRole = getenv ("MODEL_ROLE");
  if (modelRole == NULL)
  {
     PrintError("Environment variable MODEL_ROLE not set...\n");
     return(ERROR);
  }

/* Register the signal handler */

    RegisterSignalHandler();

/* Connect to the local database to retrieve DB site info. Then close
   the session and open connections to all sites, including the
   local one.

   IMPORTANT!!! At this time, we MUST use the MODEL_ROLE (model_role)
   to connect. Reason is that there exists a rule/procedure in the database
   which will automatically increment maxid in ref_model_run when
   a record is inserted, unless we are connected as role 
   model_role !!!. And since during syncing we do NOT want to 
   increment maxid (we're only copying information of other site's 
   runs) we MUST connect using model_role. */

  result = SqlConnectRole (dbName, appUser, dbName, modelRole);
  if (result != OK)
  {
    PrintError("Could not connect to local database as %s...\n", modelRole);
    return(ERROR);
  }

 /* Load the site info */

  result = SqlLoadDbSiteInfo();
  if (result != OK)
  {
    PrintError("Problem loading site info from local database...\n");
    SqlDisconnectAll();
    return(ERROR);
  }

  /* Now disconnect and then re-connect to all named sessions. */
  result = SqlDisconnect ();
  if (result != OK)
  {
    PrintError("Problem disconnecting from local database...\n");
    SqlDisconnectAll();
    return(ERROR);
  }
  
/* Now open sessions to all other databases. Use the session numbers 
   retrieved with the SqlLoadDbSiteInfo call in the order in which the sites 
   are stored in the dbsite_array (note: the lowest session number that 
   can be used is 1.)) */

  for (site_no = 0; site_no < number_of_db_sites; site_no++)
  {
    result = SqlConnectRoleAt(dbsite_array[site_no].dbsite_alias,
                              dbsite_array[site_no].dbsite_db_name,
                              appUser,  
                              dbsite_array[site_no].dbsite_db_name,
                              modelRole);
    if (result != OK)
    {
      PrintError("Could not connect to database %s...\n",
                 dbsite_array[site_no].dbsite_db_name);
      SqlDisconnectAll(); /* disconnect all open sessions, if any */
      return(ERROR);
    }
    dbsite_array[i].connected = TRUE;
  }

/* Now all sessions are open, figure out the synchronization task.

   1. Get the maxid and max_sync_ids for all sites.

   2. Build a synchronization map (who needs what from where).

   3. Copy run info between sites according to the sync map.

   4. If at any time an error, rollback all updates and quit
      the application.

   5. If all's well, commit all the updates and close all sessions. */ 

/* First create memory to hold the maxid and max_sync_id information.
   Since each site has only one maxid, we need a simple array of maxids.
   However, since each site has as many max_sync_ids as there are other
   sites (number_of_db_sites - 1), we need a 2D matrix of sync_ids. 
   We will, however, create arrays of number_of_db_sites long, as we
   want to process them all in the order of the sites stored
   in the db_site array. */

  if ((maxids = (int *) malloc(number_of_db_sites 
                              * sizeof(int))) == NULL)
  {
    PrintError("Error Mallocing maxids...\n");
    SqlDisconnectAll();
    return(ERROR);
  }

  if ((max_sync_ids = (int **) malloc(number_of_db_sites * 
                                      sizeof(int*))) == NULL)
  {
    PrintError("Error Mallocing max_sync_ids (int **)...\n");
    SqlDisconnectAll();
    return(ERROR);
  }
  for (i = 0; i < number_of_db_sites; i++)
    if ((max_sync_ids[i] = (int *) malloc(number_of_db_sites
                                          * sizeof(int))) == NULL)
    {
      PrintError("Error Mallocing max_sync_ids[i]...\n");
      SqlDisconnectAll();
      return(ERROR);
    }

/* Now cycle through the sessions (sites) and fill the maxid and
   max_sync_id arrays */

  for (site_no = 0; site_no < number_of_db_sites; site_no++)
  {
    result = SwitchSession(dbsite_array[site_no].dbsite_alias);
    if (result != OK)
    {
      PrintError("Error switching to database session %s...\n",
		 dbsite_array[site_no].dbsite_alias);
      SqlDisconnectAll();
      return(ERROR);
    }

    sprintf(operateStmt, "%s", 
      "SELECT db_site_code, length (db_site_code), maxid, max_sync_id FROM ref_db_list");

/* Prepare the select statement and open a cursor */

    /* EXEC SQL WHENEVER NOT FOUND goto close_c2; */ 

    /* EXEC SQL AT :current_dbsite DECLARE selc2 STATEMENT; */ 

    /* EXEC SQL PREPARE selc2 FROM :operateStmt; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 2;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)operateStmt;
    sqlstm.sqhstl[0] = (unsigned int  )10001;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)current_dbsite;
    sqlstm.sqhstl[1] = (unsigned int  )10;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         void  *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned int  )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


    result = SqlErrorHandler (FUNC_NAME, FALSE,
                              "Problem preparing selc2...\n");
    if (result != OK)
    {
      SqlDisconnectAll();
      return (ERROR);
    }

    /* EXEC SQL DECLARE c2 CURSOR for selc2; */ 

    result = SqlErrorHandler (FUNC_NAME, FALSE,
                              "Problem declaring cursor c2...\n");
    if (result != OK)
    {
      SqlDisconnectAll();
      return(ERROR);
    }

    /* EXEC SQL OPEN c2; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 2;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )28;
    sqlstm.selerr = (unsigned short)1;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)current_dbsite;
    sqlstm.sqhstl[0] = (unsigned int  )10;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


    result = SqlErrorHandler (FUNC_NAME, FALSE, 
                              "Problem opening cursor c2...\n");
    if (result != OK)
    {
      SqlDisconnectAll();
      return(ERROR);
    }

/* For this site, set everything to 0. */

    maxids[site_no] = 0;
    for (j = 0; j < number_of_db_sites; j++)
      max_sync_ids[site_no][j] = 0; 
 
    num_rec = 0;
    while(TRUE)
    {
      /* EXEC SQL FETCH c2 INTO :siteCode, 
                             :siteCodeSize,
                             :maxId:maxIdNull, 
                             :maxSyncId:maxSyncIdNull; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )47;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)siteCode;
      sqlstm.sqhstl[0] = (unsigned int  )11;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)&siteCodeSize;
      sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)&maxId;
      sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)&maxIdNull;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)&maxSyncId;
      sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[3] = (         int  )0;
      sqlstm.sqindv[3] = (         void  *)&maxSyncIdNull;
      sqlstm.sqinds[3] = (         int  )0;
      sqlstm.sqharm[3] = (unsigned int  )0;
      sqlstm.sqadto[3] = (unsigned short )0;
      sqlstm.sqtdso[3] = (unsigned short )0;
      sqlstm.sqhstv[4] = (         void  *)current_dbsite;
      sqlstm.sqhstl[4] = (unsigned int  )10;
      sqlstm.sqhsts[4] = (         int  )0;
      sqlstm.sqindv[4] = (         void  *)0;
      sqlstm.sqinds[4] = (         int  )0;
      sqlstm.sqharm[4] = (unsigned int  )0;
      sqlstm.sqadto[4] = (unsigned short )0;
      sqlstm.sqtdso[4] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
      if (sqlca.sqlcode == 1403) goto close_c2;
}



      result = SqlErrorHandler (FUNC_NAME, TRUE,
               "Problem fetching from ref_db_list on num_rec %d.\n", num_rec);
      if (result != OK)
      {
        /* EXEC SQL CLOSE c2; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 5;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )82;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)256;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)current_dbsite;
        sqlstm.sqhstl[0] = (unsigned int  )10;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqphss = sqlstm.sqhsts;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqpins = sqlstm.sqinds;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlstm.sqpadto = sqlstm.sqadto;
        sqlstm.sqptdso = sqlstm.sqtdso;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


        SqlDisconnectAll();
        return (ERROR);
      }

      siteCode[siteCodeSize] = '\0';
      if (num_rec == 0) /* first fetch */
        if (maxId > 0)
          maxids[site_no] = maxId;

/* Now, as we want to make sure that each element in each max_sync_ids[i] 
   array corresponds with the same database, we use the order of the
   sites in the dbsite_array as the "master order." This, however,
   implies that for each record fetched, we must find the element in
   the dbsite_array that it corresponds to and then set the
   corresponding element in the max_sync_ids[site_no] array. */

      for (i = 0; i < number_of_db_sites; i++)
        if (!(strcmp(siteCode,dbsite_array[i].dbsite_alias))) /* found! */
        {
          max_sync_ids[site_no][i] = maxSyncId;
          num_rec++;
          break;
        } 
    } /* end while(TRUE) */

   close_c2:
     /* EXEC SQL CLOSE c2; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 10;
     sqlstm.arrsiz = 5;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )101;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)256;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (         void  *)current_dbsite;
     sqlstm.sqhstl[0] = (unsigned int  )10;
     sqlstm.sqhsts[0] = (         int  )0;
     sqlstm.sqindv[0] = (         void  *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned int  )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqphsv = sqlstm.sqhstv;
     sqlstm.sqphsl = sqlstm.sqhstl;
     sqlstm.sqphss = sqlstm.sqhsts;
     sqlstm.sqpind = sqlstm.sqindv;
     sqlstm.sqpins = sqlstm.sqinds;
     sqlstm.sqparm = sqlstm.sqharm;
     sqlstm.sqparc = sqlstm.sqharc;
     sqlstm.sqpadto = sqlstm.sqadto;
     sqlstm.sqptdso = sqlstm.sqtdso;
     sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



  } /* end of for (site_no = 0; site_no < number_of_db_sites; site_no++) */

/* At this point, the max_ids and max_sync_ids are filled and 
   we "know" who needs what, so we can again cycle through
   the sites (sessions) and do the update.

   For each site:
     Go through its max_sync_ids[site_no] array
     For each max_sync_ids array[site_no] value:
       Compare the value with the maxids array value of the 
       corresponding site. Update when needed. */

  for (site_no = 0; site_no < number_of_db_sites; site_no++)
  {
    for (other_site_no = 0; 
         other_site_no < number_of_db_sites; other_site_no++) 
    {
      if (site_no == other_site_no)
        continue; /* site does not have to sync with itself */

      if (max_sync_ids[site_no][other_site_no] < 
                                    maxids[other_site_no] - 1)
      {

/* Do the sync */

/*      printf("About to do insert/update...\nother_site_no: %s, site_no: %s\n",
               dbsite_array[other_site_no].dbsite_alias,
               dbsite_array[site_no].dbsite_alias);
        printf("site_no max_sync_id: %d\n,other_site_no max_id: %d\n",
               max_sync_ids[site_no][other_site_no],
               maxids[other_site_no]); */

        printf("Updating: %s From: %s\n",
               dbsite_array[site_no].dbsite_alias,
               dbsite_array[other_site_no].dbsite_alias);
        
        result = SqlUpdateModelRunIds(site_no,other_site_no,
                                      max_sync_ids[site_no][other_site_no],
                                      maxids[other_site_no]);
        if (result != OK)
        {
          PrintError("Error in SqlUpdateModelRunIds...\n");
          SqlDisconnectAll();
          return(ERROR);
        }
      }
    }
  } 

/* All updates are done. Cycle one last time through the sessions 
   to commit all the inserts and updates, then disconnect from the databases 
   and get out of here. If the cycling stops because of an unsuccesful
   session switch, notify the user and get out */ 

  did_commits = FALSE;
  for (site_no = 0; site_no < number_of_db_sites; site_no++)
  {
   if (SwitchSession(dbsite_array[site_no].dbsite_alias) != OK)
   {
     PrintError("WATCH OUT!!!!! during the commit phase of the syncing\n");
     PrintError("a reconnect to database %s did not succeed.\n",
                dbsite_array[site_no].dbsite_alias);
     if (did_commits)
       PrintError("commits elsewhere have occurred, however...\n");
     else
       PrintError("no commits have occurred anywhere, however...\n");
     PrintError("Please rerun the synchronization when all databases can be reached...\n");
     SqlDisconnectAll();
     return(ERROR);
   }
   /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )120;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)current_dbsite;
   sqlstm.sqhstl[0] = (unsigned int  )10;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 /* commit the updates */
   did_commits = TRUE;
  }

  SqlDisconnectAll();

/* the following frees only happen when the routine completes
   successfully; i.e., if the routine runs into error conditions
   before this point, the memory is not freed. However, the calling
   process will (most likely) exit on error anyway */

  free((char*)maxids);

  for (i = 0; i < number_of_db_sites; i++)
    free((char *)max_sync_ids[i]);
  free((char *)max_sync_ids);


  printf("Synchronization completed...\n\n");
  return(OK);
}    

/*******************************************************************/
#define FUNC_NAME "SqlUpdateModelRunIds"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlSyncModelRuns

<P> <STRONG>Function signature:</STRONG> int SqlUpdateModelRunIds(int, int, int, int)  

<P> <STRONG>Function description:</STRONG> Does the work of copying needed model_run_ids from one site to another.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> int site_no:             Site (session) to be updated.
<BR><DD> int other_site_no:       Site (session) where run info resides.
<BR><DD> int current_max_sync_id: Site_no's current value for other_site's max_sync_id.
<BR><DD> int new_max_sync_id:     Other_site's max_id.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlUpdateModelRunIds(int site_no, int other_site_no,
                         int current_max_sync_id, 
                         int new_max_sync_id)  
{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

  int       prob, newmaxsync, runs_to_add;
  char      operateStmt[STATEMENT_LEN+1],
            modelType[2], runName[65],
            comment[1001], timeStep[129],
            dbsitecode[4];
  SQL_DATE  sysDate, runDate, startDate, endDate;
  ID        runId, modelId, userId;
  short int probNull, modelTypeNull, userIdNull,
            startDateNull, endDateNull,
            timeStepNull, commentNull;
  /* EXEC SQL END DECLARE SECTION; */ 


  int i, j, same_block, result, count;
  int *model_run_ids;

  char line[10000];

/* First compute how many run ids are actually involved. Remember
   that as sites get allocated alternating blocks of ID_RANGE
   ids, a sync will occasionaly span blocks. */

  same_block = FALSE; /* assume that a block must be spanned */
  if ((new_max_sync_id - current_max_sync_id) < ID_RANGE) 
  {
    same_block = TRUE; /* same block */
    runs_to_add = new_max_sync_id - 1 - current_max_sync_id;
  }
  else /* ids span one or more blocks */
  {
    runs_to_add = 0;
    count = current_max_sync_id + 1;
    while (count < new_max_sync_id)
    {
      if (count % ID_RANGE == 0) /* last one in the block */ 
        count = count + (number_of_db_sites - 1) * ID_RANGE + 1; 
      else
        count++;
      runs_to_add++;
    }
  }

/* Allocate an array of runs_to_add ints to hold the model_
   run_ids to be updated and fill the array (the above code segment is
   repeated here; perhaps a bit expensive, but the task seems too
   small to do something smart. */
 
  if ((model_run_ids = (int *) malloc(runs_to_add * 
                                   sizeof(int))) == NULL)
  {
    PrintError ("Problem mallocing model_run_ids in SqlUpdateModelRunIds...\n");
    return(ERROR);
  }

  i = 0; 
  if (same_block == TRUE)
    for (j = current_max_sync_id + 1; j < new_max_sync_id; j++)
      model_run_ids[i++] = j;
  else
  {
    count = current_max_sync_id + 1;
    while (count < new_max_sync_id)
    {
      if (count % ID_RANGE == 0) /* last one in the block */
        count = count + ID_RANGE + 1;
      else
        count++;
      model_run_ids[i++] = count;
    }
  }

/* Now copy the runs in model_run_ids from other_site_no to site_no.
   Since (apparently) a FETCH cannot read from a cursor
   opened on another session, we must switch the sessions back
   and forth between FETCH (from other_site_no) and INSERT
   (to site_no) actions */

  result = SwitchSession(dbsite_array[other_site_no].dbsite_alias);
  if (result != OK)
  {
    PrintError("Error switching database sessions...\n");
    return(ERROR);
  }

/* printf("session to go get runs switched to: %s\n",
          dbsite_array[other_site_no].dbsite_alias); */

/* Build the select statement. Since we know exactly which runs we
   need we can stick them all in the WHERE clause */

  sprintf(operateStmt, "%s",
    "SELECT model_run_id, model_run_name, model_id, sys_date, run_date, probability, modeltype, user_id, start_date, end_date, time_step_descriptor, cmmnt ");
  strcat(operateStmt,"FROM ref_model_run ");
  sprintf(line, "WHERE model_run_id in (%d ", model_run_ids[0]);
  strcat(operateStmt,line);

  for (i = 1; i < runs_to_add; i++)
  {
    sprintf(line,",%d ",model_run_ids[i]);
    strcat(operateStmt,line);
  }
  strcat(operateStmt,")");

  printf("select for the run_ids to be moved from %s to %s:\n: %s\n",
         dbsite_array[other_site_no].dbsite_alias,
         dbsite_array[site_no].dbsite_alias,
         operateStmt);

/* Prepare the SELECT statement and open a cursor */

  /* EXEC SQL WHENEVER NOT FOUND goto close_c3; */ 

  /* EXEC SQL AT :current_dbsite DECLARE selc3 STATEMENT; */ 

  /* EXEC SQL PREPARE selc3 FROM :operateStmt; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )139;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)operateStmt;
  sqlstm.sqhstl[0] = (unsigned int  )10001;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)current_dbsite;
  sqlstm.sqhstl[1] = (unsigned int  )10;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


  result = SqlErrorHandler (FUNC_NAME, FALSE,
                             "Problem preparing selc3...\n");
  if (result != OK)
  {
    PrintError("Error preparing statement selc3...\n");
    return (ERROR);
  }

  /* EXEC SQL DECLARE c3 CURSOR FOR selc3; */ 

  result = SqlErrorHandler (FUNC_NAME, FALSE,
                            "Problem declaring cursor c3...\n");
  if (result != OK)
  {
    PrintError("Error declaring cursor c3...\n");
    return (ERROR);
  }

  /* EXEC SQL OPEN c3; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )162;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)current_dbsite;
  sqlstm.sqhstl[0] = (unsigned int  )10;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
  result = SqlErrorHandler (FUNC_NAME, FALSE,
                            "Problem opening cursor c3...\n");
  if (result != OK)
  {
    PrintError("Error opening cursor c3...\n");
    return(ERROR);
  }

  count = 0;
  while(TRUE)
  {
    /* EXEC SQL FETCH c3 INTO :runId, :runName, :modelId, :sysDate, :runDate, 
                           :prob:probNull, :modelType:modelTypeNull, 
                           :userId:userIdNull, :startDate:startDateNull, 
                           :endDate:endDateNull, :timeStep:timeStepNull, 
                           :comment:commentNull; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 13;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )181;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)&runId;
    sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)runName;
    sqlstm.sqhstl[1] = (unsigned int  )65;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         void  *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned int  )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (         void  *)&modelId;
    sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         void  *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned int  )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (         void  *)sysDate;
    sqlstm.sqhstl[3] = (unsigned int  )21;
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         void  *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned int  )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (         void  *)runDate;
    sqlstm.sqhstl[4] = (unsigned int  )21;
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         void  *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned int  )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (         void  *)&prob;
    sqlstm.sqhstl[5] = (unsigned int  )sizeof(int);
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         void  *)&probNull;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned int  )0;
    sqlstm.sqadto[5] = (unsigned short )0;
    sqlstm.sqtdso[5] = (unsigned short )0;
    sqlstm.sqhstv[6] = (         void  *)modelType;
    sqlstm.sqhstl[6] = (unsigned int  )2;
    sqlstm.sqhsts[6] = (         int  )0;
    sqlstm.sqindv[6] = (         void  *)&modelTypeNull;
    sqlstm.sqinds[6] = (         int  )0;
    sqlstm.sqharm[6] = (unsigned int  )0;
    sqlstm.sqadto[6] = (unsigned short )0;
    sqlstm.sqtdso[6] = (unsigned short )0;
    sqlstm.sqhstv[7] = (         void  *)&userId;
    sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
    sqlstm.sqhsts[7] = (         int  )0;
    sqlstm.sqindv[7] = (         void  *)&userIdNull;
    sqlstm.sqinds[7] = (         int  )0;
    sqlstm.sqharm[7] = (unsigned int  )0;
    sqlstm.sqadto[7] = (unsigned short )0;
    sqlstm.sqtdso[7] = (unsigned short )0;
    sqlstm.sqhstv[8] = (         void  *)startDate;
    sqlstm.sqhstl[8] = (unsigned int  )21;
    sqlstm.sqhsts[8] = (         int  )0;
    sqlstm.sqindv[8] = (         void  *)&startDateNull;
    sqlstm.sqinds[8] = (         int  )0;
    sqlstm.sqharm[8] = (unsigned int  )0;
    sqlstm.sqadto[8] = (unsigned short )0;
    sqlstm.sqtdso[8] = (unsigned short )0;
    sqlstm.sqhstv[9] = (         void  *)endDate;
    sqlstm.sqhstl[9] = (unsigned int  )21;
    sqlstm.sqhsts[9] = (         int  )0;
    sqlstm.sqindv[9] = (         void  *)&endDateNull;
    sqlstm.sqinds[9] = (         int  )0;
    sqlstm.sqharm[9] = (unsigned int  )0;
    sqlstm.sqadto[9] = (unsigned short )0;
    sqlstm.sqtdso[9] = (unsigned short )0;
    sqlstm.sqhstv[10] = (         void  *)timeStep;
    sqlstm.sqhstl[10] = (unsigned int  )129;
    sqlstm.sqhsts[10] = (         int  )0;
    sqlstm.sqindv[10] = (         void  *)&timeStepNull;
    sqlstm.sqinds[10] = (         int  )0;
    sqlstm.sqharm[10] = (unsigned int  )0;
    sqlstm.sqadto[10] = (unsigned short )0;
    sqlstm.sqtdso[10] = (unsigned short )0;
    sqlstm.sqhstv[11] = (         void  *)comment;
    sqlstm.sqhstl[11] = (unsigned int  )1001;
    sqlstm.sqhsts[11] = (         int  )0;
    sqlstm.sqindv[11] = (         void  *)&commentNull;
    sqlstm.sqinds[11] = (         int  )0;
    sqlstm.sqharm[11] = (unsigned int  )0;
    sqlstm.sqadto[11] = (unsigned short )0;
    sqlstm.sqtdso[11] = (unsigned short )0;
    sqlstm.sqhstv[12] = (         void  *)current_dbsite;
    sqlstm.sqhstl[12] = (unsigned int  )10;
    sqlstm.sqhsts[12] = (         int  )0;
    sqlstm.sqindv[12] = (         void  *)0;
    sqlstm.sqinds[12] = (         int  )0;
    sqlstm.sqharm[12] = (unsigned int  )0;
    sqlstm.sqadto[12] = (unsigned short )0;
    sqlstm.sqtdso[12] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode == 1403) goto close_c3;
}


    result = SqlErrorHandler (FUNC_NAME, TRUE,
             "Problem fetching from ref_model_run table on record %d.\n",
                                    count);
    if (result != OK)
    {
      PrintError("Error in Fetch on c3...\n");
      /* EXEC SQL CLOSE c3; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 13;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )248;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)current_dbsite;
      sqlstm.sqhstl[0] = (unsigned int  )10;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
    }
    count++;

/* Now switch to site_no for INSERT */

    result = SwitchSession(dbsite_array[site_no].dbsite_alias);
    if (result != OK)
    {
      PrintError("Problem switching sessions...\n");
      /* EXEC SQL CLOSE c3; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 13;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )267;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)current_dbsite;
      sqlstm.sqhstl[0] = (unsigned int  )10;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
    }

/*  printf("Switched to %s for insert of run %d...\n",
            dbsite_array[site_no].dbsite_alias,runId); */

/* Since a record in ref_model_run can have NULL values in some of
   its fields, we use the full var:ind_var combination as part of the
   insert. */

    printf("Copying run %d\n",runId);

    /* EXEC SQL AT :current_dbsite INSERT INTO ref_model_run VALUES 
      (:runId, :runName, :modelId, :sysDate, :runDate, 
       :prob:probNull, :modelType:modelTypeNull, 
       :userId:userIdNull, :startDate:startDateNull, 
       :endDate:endDateNull, :timeStep:timeStepNull,
       :comment:commentNull); */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 13;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "insert into ref_model_run  values (:b1,:b2,:b3,:b4,:b5,:b\
6:b7,:b8:b9,:b10:b11,:b12:b13,:b14:b15,:b16:b17,:b18:b19)";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )286;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)&runId;
    sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)runName;
    sqlstm.sqhstl[1] = (unsigned int  )65;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         void  *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned int  )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (         void  *)&modelId;
    sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         void  *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned int  )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (         void  *)sysDate;
    sqlstm.sqhstl[3] = (unsigned int  )21;
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         void  *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned int  )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (         void  *)runDate;
    sqlstm.sqhstl[4] = (unsigned int  )21;
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         void  *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned int  )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (         void  *)&prob;
    sqlstm.sqhstl[5] = (unsigned int  )sizeof(int);
    sqlstm.sqhsts[5] = (         int  )0;
    sqlstm.sqindv[5] = (         void  *)&probNull;
    sqlstm.sqinds[5] = (         int  )0;
    sqlstm.sqharm[5] = (unsigned int  )0;
    sqlstm.sqadto[5] = (unsigned short )0;
    sqlstm.sqtdso[5] = (unsigned short )0;
    sqlstm.sqhstv[6] = (         void  *)modelType;
    sqlstm.sqhstl[6] = (unsigned int  )2;
    sqlstm.sqhsts[6] = (         int  )0;
    sqlstm.sqindv[6] = (         void  *)&modelTypeNull;
    sqlstm.sqinds[6] = (         int  )0;
    sqlstm.sqharm[6] = (unsigned int  )0;
    sqlstm.sqadto[6] = (unsigned short )0;
    sqlstm.sqtdso[6] = (unsigned short )0;
    sqlstm.sqhstv[7] = (         void  *)&userId;
    sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
    sqlstm.sqhsts[7] = (         int  )0;
    sqlstm.sqindv[7] = (         void  *)&userIdNull;
    sqlstm.sqinds[7] = (         int  )0;
    sqlstm.sqharm[7] = (unsigned int  )0;
    sqlstm.sqadto[7] = (unsigned short )0;
    sqlstm.sqtdso[7] = (unsigned short )0;
    sqlstm.sqhstv[8] = (         void  *)startDate;
    sqlstm.sqhstl[8] = (unsigned int  )21;
    sqlstm.sqhsts[8] = (         int  )0;
    sqlstm.sqindv[8] = (         void  *)&startDateNull;
    sqlstm.sqinds[8] = (         int  )0;
    sqlstm.sqharm[8] = (unsigned int  )0;
    sqlstm.sqadto[8] = (unsigned short )0;
    sqlstm.sqtdso[8] = (unsigned short )0;
    sqlstm.sqhstv[9] = (         void  *)endDate;
    sqlstm.sqhstl[9] = (unsigned int  )21;
    sqlstm.sqhsts[9] = (         int  )0;
    sqlstm.sqindv[9] = (         void  *)&endDateNull;
    sqlstm.sqinds[9] = (         int  )0;
    sqlstm.sqharm[9] = (unsigned int  )0;
    sqlstm.sqadto[9] = (unsigned short )0;
    sqlstm.sqtdso[9] = (unsigned short )0;
    sqlstm.sqhstv[10] = (         void  *)timeStep;
    sqlstm.sqhstl[10] = (unsigned int  )129;
    sqlstm.sqhsts[10] = (         int  )0;
    sqlstm.sqindv[10] = (         void  *)&timeStepNull;
    sqlstm.sqinds[10] = (         int  )0;
    sqlstm.sqharm[10] = (unsigned int  )0;
    sqlstm.sqadto[10] = (unsigned short )0;
    sqlstm.sqtdso[10] = (unsigned short )0;
    sqlstm.sqhstv[11] = (         void  *)comment;
    sqlstm.sqhstl[11] = (unsigned int  )1001;
    sqlstm.sqhsts[11] = (         int  )0;
    sqlstm.sqindv[11] = (         void  *)&commentNull;
    sqlstm.sqinds[11] = (         int  )0;
    sqlstm.sqharm[11] = (unsigned int  )0;
    sqlstm.sqadto[11] = (unsigned short )0;
    sqlstm.sqtdso[11] = (unsigned short )0;
    sqlstm.sqhstv[12] = (         void  *)current_dbsite;
    sqlstm.sqhstl[12] = (unsigned int  )10;
    sqlstm.sqhsts[12] = (         int  )0;
    sqlstm.sqindv[12] = (         void  *)0;
    sqlstm.sqinds[12] = (         int  )0;
    sqlstm.sqharm[12] = (unsigned int  )0;
    sqlstm.sqadto[12] = (unsigned short )0;
    sqlstm.sqtdso[12] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode == 1403) goto close_c3;
}



    result = SqlErrorHandler (FUNC_NAME, FALSE,
                   "Problem executing insert into ref_model_run.\n");
    if (result != OK)
    {
      PrintError("Error inserting into ref_model_run...\n");
      /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 13;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )353;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)current_dbsite;
      sqlstm.sqhstl[0] = (unsigned int  )10;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      result = SqlErrorHandler (FUNC_NAME, TRUE,
                    "Could not rollback changes.\n");
      if (result != OK)
        PrintError("Error during ROLLBACK...\n");

      return(ERROR);
    }

/* switch-back to other_site_no */

    result = SwitchSession(dbsite_array[other_site_no].dbsite_alias);
    if (result != OK)
    {
      PrintError("Problem switching to session %s before fetch from c3...\n",
		 dbsite_array[other_site_no].dbsite_alias);
      /* EXEC SQL CLOSE c3; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 13;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )372;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)current_dbsite;
      sqlstm.sqhstl[0] = (unsigned int  )10;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
    }

/*   printf("switched back to %s for new run fetch\n",
            dbsite_array[other_site_no].dbsite_alias); */
  }

  close_c3:
    /* EXEC SQL CLOSE c3; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 13;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )391;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)current_dbsite;
    sqlstm.sqhstl[0] = (unsigned int  )10;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



/* If anything was fetched (count > 0), update the associated
   max_sync_id using host language variables */

  if (count > 0)
  {

/* First switch back to the site_no site */

    result = SwitchSession(dbsite_array[site_no].dbsite_alias);
    if (result != OK)
    {
      PrintError("Problem switching to session %s before UPDATE...\n",
		 dbsite_array[site_no].dbsite_alias);
      /* EXEC SQL CLOSE c3; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 13;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )410;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)current_dbsite;
      sqlstm.sqhstl[0] = (unsigned int  )10;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
    }

/*      printf("switched to %s for UPDATE of max_sync_id\n",
             dbsite_array[site_no].dbsite_alias); */

    newmaxsync = new_max_sync_id - 1;
    strcpy(dbsitecode,dbsite_array[other_site_no].dbsite_alias); 

    /* EXEC SQL AT :current_dbsite UPDATE ref_db_list SET max_sync_id = :newmaxsync WHERE db_site_code = :dbsitecode; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 13;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update ref_db_list  set max_sync_id=:b1 where db_site_cod\
e=:b2";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )429;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)&newmaxsync;
    sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)dbsitecode;
    sqlstm.sqhstl[1] = (unsigned int  )4;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         void  *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned int  )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (         void  *)current_dbsite;
    sqlstm.sqhstl[2] = (unsigned int  )10;
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         void  *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned int  )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode == 1403) goto close_c3;
}


    if ((result = SqlErrorHandler (FUNC_NAME, FALSE,
                       "Error in update of max_sync_id...\n")) != OK)
    {
      /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 13;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )456;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)current_dbsite;
      sqlstm.sqhstl[0] = (unsigned int  )10;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      result = SqlErrorHandler (FUNC_NAME, TRUE,
                     "Could not rollback changes.\n");
      if (result != OK)
        PrintError("Error during ROLLBACK...\n");

      return (ERROR);
    }
  }

/* the following free only happen when the routine completes
   successfully; i.e. if the routine runs into error conditions
   before this point, the memory is not freed. However, the calling
   process will (most likely) exit on error anyway */

  free((char *)model_run_ids);

  return(OK);
}
