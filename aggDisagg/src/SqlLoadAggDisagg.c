
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
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
    "SqlLoadAggDisagg.pc"
};


static unsigned int sqlctx = 42229667;


static struct sqlexd {
   unsigned long  sqlvsn;
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
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
            int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
   unsigned char  *sqhstv[10];
   unsigned long  sqhstl[10];
            int   sqhsts[10];
            short *sqindv[10];
            int   sqinds[10];
   unsigned long  sqharm[10];
   unsigned long  *sqharc[10];
   unsigned short  sqadto[10];
   unsigned short  sqtdso[10];
} sqlstm = {12,10};

/* SQLLIB Prototypes */
extern void sqlcxt (void **, unsigned int *,
                    struct sqlexd *, const struct sqlcxp *);
extern void sqlcx2t(void **, unsigned int *,
                    struct sqlexd *, const struct sqlcxp *);
extern void sqlbuft(void **, char *);
extern void sqlgs2t(void **, char *);
extern void sqlorat(void **, unsigned int *, void *);

/* Forms Interface */
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern void sqliem(char *, int *);

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4130,1,0,0,
5,0,0,1,372,0,4,68,0,0,10,1,0,1,0,2,3,0,0,2,5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,5,
0,0,2,3,0,0,2,3,0,0,2,9,0,0,1,3,0,0,
60,0,0,2,0,0,31,82,0,0,0,0,0,1,0,
};


/*****************************************************************************
* DESCRIPTION
*
* This file contains the function which loads driver data from HDB, 
* and the function which inserts a data-derivation record into
* the ref_data_derivation table.
*
***************************************
*
* SqlLoadAggDisagg
*
* Purpose: load in the row of driver data from HDB table ref_agg_disagg.
* 
* Input
* 	RUN_CONTROL* - pointer to structure holding run control info; 
*                      to be loaded
*	int *        - ID of driver row to be selected from database
*		       function
*
* Output
*
* Return
* 	int - OK or ERROR
*
*
* Author
*
* Carol Marra
* February, 1996
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aggDisagg.h"
#include "aggDisaggDefines.h"
#include "utils.h"
#include "dbutils.h"
#include "defines.h"
#include "typedefs.h"
#include "aggDisaggTypedefs.h"

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


#define FUNC_NAME "SqlLoadAggDisagg"

int SqlLoadAggDisagg (RUN_CONTROL *runControl, int aggDisaggId)

{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    int sqlAggDisaggId = aggDisaggId;
    ID datatypeSource;
    char obsSource[DB_OBS_LENGTH];
    char dtUnitInd[2];
    ID datatypeOrUnitDest;
    char obsDest[DB_OBS_LENGTH];
    char opOrFcn[2];
    ID method,
       methodClass;
    /* VARCHAR functionName[FUNC_LENGTH]; */ 
struct { unsigned short len; unsigned char arr[33]; } functionName;

    IND methodInd,
        methodClassInd,
        functionInd;
  /* EXEC SQL END DECLARE SECTION; */ 


  int result;
  
  /* Select driver row from database. */
  /* EXEC SQL SELECT 
    a.source_datatype_id, a.source_observation_interval, a.dest_datatype_unit_ind,
    a.dest_datatype_or_unit_id, a.dest_observation_interval, a.method_or_function,
    a.method_id, b.method_class_id, a.agg_disagg_function_name
    INTO :datatypeSource, :obsSource, :dtUnitInd, :datatypeOrUnitDest, 
      :obsDest, :opOrFcn, :method:methodInd, :methodClass:methodClassInd, 
      :functionName:functionInd
    FROM ref_agg_disagg a, hdb_method b
    WHERE a.agg_disagg_id = :sqlAggDisaggId
      AND a.method_id = b.method_id(+); */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 10;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select a.source_datatype_id ,a.source_observation_interval \
,a.dest_datatype_unit_ind ,a.dest_datatype_or_unit_id ,a.dest_observation_inte\
rval ,a.method_or_function ,a.method_id ,b.method_class_id ,a.agg_disagg_funct\
ion_name into :b0,:b1,:b2,:b3,:b4,:b5,:b6:b7,:b8:b9,:b10:b11  from ref_agg_dis\
agg a ,hdb_method b where (a.agg_disagg_id=:b12 and a.method_id=b.method_id(+)\
)";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&datatypeSource;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)obsSource;
  sqlstm.sqhstl[1] = (unsigned long )17;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)dtUnitInd;
  sqlstm.sqhstl[2] = (unsigned long )2;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)&datatypeOrUnitDest;
  sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)obsDest;
  sqlstm.sqhstl[4] = (unsigned long )17;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)opOrFcn;
  sqlstm.sqhstl[5] = (unsigned long )2;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&method;
  sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)&methodInd;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&methodClass;
  sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)&methodClassInd;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (unsigned char  *)&functionName;
  sqlstm.sqhstl[8] = (unsigned long )35;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         short *)&functionInd;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned long )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (unsigned char  *)&sqlAggDisaggId;
  sqlstm.sqhstl[9] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         short *)0;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned long )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
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



  if ((result = SqlErrorHandler (FUNC_NAME, FALSE, "Problem selecting driver data from ref_agg_disagg\n\tfor agg_disagg_id = %d.\n\tExiting.", 
				 sqlAggDisaggId)) != OK)
    {
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 10;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )60;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
    }

  /* Set values on runControl */
  runControl->datatypeSource = datatypeSource;
  strcpy (runControl->interval, obsDest);

  if (!strncmp (obsSource, HDB_INSTANT, 3))
    runControl->obsSource = BY_INSTANT;
  else if (!strncmp (obsSource, HDB_HOUR, 3))
    runControl->obsSource = BY_HOUR;
  else if (!strncmp (obsSource, HDB_DAY, 3))
    runControl->obsSource = BY_DAY;
  else if (!strncmp (obsSource, HDB_MONTH, 3))
    runControl->obsSource = BY_MONTH;
  else if (!strncmp (obsSource, HDB_YEAR, 3))
    runControl->obsSource = BY_YEAR;
  else if (!strncmp (obsSource, HDB_WY, 2))
    runControl->obsSource = BY_WY;
  else
    {
      PrintError ("Value for source_observation_prd in ref_agg_disagg, '%s', is not recognized\n\tor is not a valid HDB table interval.\n\tExiting.", obsSource);
      return (ERROR);
    }

  if (!strncmp (dtUnitInd, "D", 1))
    {
      runControl->datatypeDest = datatypeOrUnitDest;
      runControl->unitDest = NA;
    }
  else if (!strncmp (dtUnitInd, "U", 1))
    {
      runControl->datatypeDest = NA;
      runControl->unitDest = datatypeOrUnitDest;
    }
  else
    {
      PrintError ("Value for dest_datatype_unit_ind in ref_agg_disagg, '%s', is not recognized.\n\tExiting.", dtUnitInd);
      return (ERROR);
    }

  /* Set destination observation interval. Note that instantaneous is 
     never valid. */
  if (!strncmp (obsDest, HDB_INSTANT, 3))
    runControl->obsDest = BY_INSTANT;
  else if (!strncmp (obsDest, HDB_HOUR, 3))
    runControl->obsDest = BY_HOUR;
  else if (!strncmp (obsDest, HDB_DAY, 3))
    runControl->obsDest = BY_DAY;
  else if (!strncmp (obsDest, HDB_MONTH, 3))
    runControl->obsDest = BY_MONTH;
  else if (!strncmp (obsDest, HDB_YEAR, 3))
    runControl->obsDest = BY_YEAR;
  else if (!strncmp (obsDest, HDB_WY, 2))
    runControl->obsDest = BY_WY;
  else
    {
      PrintError ("Value for dest_observation_prd in ref_agg_disagg, '%s', is not recognized.\n\tExiting.", obsDest);
      return (ERROR);
    }
  
  strcpy (runControl->opOrFcn, opOrFcn);
  if (!strcmp (opOrFcn, "M"))
    {
      runControl->methodId =  method;
      runControl->methodClassId = methodClass; 
      runControl->functionName[0] = '\0';
    }
  else if (!strcmp (opOrFcn, "F"))
    {
      runControl->methodId = NA;
      runControl->methodClassId = NA;
      strncpy (runControl->functionName, functionName.arr, functionName.len);
      runControl->functionName[functionName.len] = '\0';
    }
  else
    {
      runControl->methodId = NA;
      runControl->methodClassId = NA;
      runControl->functionName[0] = '\0';
    }
  

  return (OK);
}