
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
           char  filnam[23];
};
static const struct sqlcxp sqlfpn =
{
    22,
    "SqlCreateModelRunId.pc"
};


static unsigned int sqlctx = 336371275;


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
   unsigned char  *sqhstv[11];
   unsigned long  sqhstl[11];
            int   sqhsts[11];
            short *sqindv[11];
            int   sqinds[11];
   unsigned long  sqharm[11];
   unsigned long  *sqharc[11];
   unsigned short  sqadto[11];
   unsigned short  sqtdso[11];
} sqlstm = {12,11};

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
5,0,0,1,58,0,1028,72,0,0,1,0,0,0,128,2,3,0,0,1,97,0,0,
28,0,0,2,216,0,1027,159,0,0,10,10,0,0,128,1,97,0,0,1,3,0,0,1,97,0,0,1,3,0,0,1,
97,0,0,1,3,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
87,0,0,3,0,0,1053,171,0,0,0,0,0,0,128,1,97,0,0,
106,0,0,4,348,0,1029,297,0,0,10,10,0,0,128,1,97,0,0,1,3,0,0,1,97,0,0,1,3,0,0,1,
97,0,0,1,3,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
165,0,0,5,0,0,1053,317,0,0,0,0,0,0,128,1,97,0,0,
};



/* standard includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>

/* HDB includes */
#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

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


#define FUNC_NAME "SqlCreateModelRunId"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlCreateModelRunId

<P> <STRONG>Function signature:</STRONG> int SqlCreateModelRunId(model_run *, int *)

<P> <STRONG>Function description:</STRONG> Creates a new model_run_id in ref_model_run based on the spec.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> model_run *spec: Specification of model_run_id to be created.
<BR><DD> int *suc:        Indicator of success of model_run_id creation. Output.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlCreateModelRunId(model_run *spec, int *suc)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   short int probNull, modelTypeNull, userIdNull,
      startDateNull, endDateNull,
      timeStepNull, commentNull;
   int runId, modelId, prob, userId;
   char runName[65], runDate[25], modelType[25], startDate[25], 
      endDate[25], timeStep[129], comment[1001];
   /* EXEC SQL END DECLARE SECTION; */ 

   
   int result;
  
/* This routine inserts run information into the ref_model_run table.
   We insert the information at the local database only.  A rule on the 
   database will increment ref_db_site_list.maxid automatically.
   (Note that we are not connected using the model_role, hence the 
   trigger will fire).
   Remote sites will update their tables through a call to 
   SqlSyncModelRuns().  */

/* Initialize the indicator variables.  -1 is NULL and 0 is not NULL */
   
   probNull = modelTypeNull = userIdNull = startDateNull = endDateNull =
      timeStepNull = commentNull = 0;
   
   result = SwitchSession(dbsite_array[0].dbsite_alias);
   if(result != OK)
   {
      PrintError("ERROR occurred changing to session 1\n");
      return(ERROR);
   }
   
   /* EXEC SQL AT :current_dbsite SELECT maxid INTO :runId 
            FROM ref_db_list 
            WHERE session_no = 1; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 2;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select maxid into :b1  from ref_db_list where session_no=1";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)&runId;
   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)current_dbsite;
   sqlstm.sqhstl[1] = (unsigned long )10;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
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



   result = SqlErrorHandler(FUNC_NAME, FALSE, 
                            "Could not retrieve the maxId.\n");
   if(result != OK)
   {
       PrintError("ERROR retrieving maxId from ref_dblist.\n");
       return (ERROR);
   }
   
   /* The following four tests are to see if the first four fields
      in a record for ref_model_run_id are indeed not NULL.  If one of them
      is NULL, return but do not close any database connections; connections
      are closed by the top level calling routine. */
   
   if(spec[0].Name == NULL)
   {
      PrintError("Error: model_run_name may not be NULL\n");
      return (ERROR);
   }
   strcpy(runName, spec[0].Name);

   if(spec[0].modelId == NA)
   {
      PrintError("Error: model_Id may not be NULL\n");
      return (ERROR);
   }
   modelId = spec[0].modelId;

   if(spec[0].runDate == NULL)
   {
      PrintError("Error: model_run_date may not be NULL\n");
      return (ERROR);
   }
   if ((result = SqlFormatDate (spec[0].runDate, runDate)) != OK)
	{
	   PrintError ("Error formatting run date %s. Exiting. \n", 
		       spec[0].runDate);
	   return (ERROR);
	}

   if(spec[0].modelType == NULL)
   {
      PrintError("Error: modelType may not be NULL\n");
      return (ERROR);
   }
   strcpy(modelType, spec[0].modelType);
   
   if(spec[0].prob == NA) probNull = -1;
   else prob = spec[0].prob;
   if(spec[0].userId == NA) userIdNull = -1;
   else userId = spec[0].userId;
   if(spec[0].startDate[0] == '\0') startDateNull = -1;
   else if ((result = SqlFormatDate (spec[0].startDate, startDate)) != OK)
   {
      PrintError ("Error formatting start date %s. Exiting. \n", spec[0].startDate);
      return (ERROR);
   }
   if(spec[0].endDate[0] == '\0') endDateNull = -1;
   else if ((result = SqlFormatDate (spec[0].endDate, endDate)) != OK)
   {
      PrintError ("Error formatting end date %s. Exiting. \n", spec[0].endDate);
      return (ERROR);
   }
   if(spec[0].timeStep[0] == '\0') timeStepNull = -1;
   else strcpy(timeStep, spec[0].timeStep);
   if(spec[0].Comment[0] == '\0') commentNull = -1;
   else strcpy(comment, spec[0].Comment);
      
   /* now connect to other sessions and insert the data into temp table */
   /* from here on, if there's failure, must rollback all sessions      */
   /* NOTE: if the process ends w/o commit, rollback is implicit        */
 
   result = SwitchSession(dbsite_array[0].dbsite_alias);
   if(result != OK)
   {
      PrintError("Could not connect to site database %s\n",
		 dbsite_array[0].dbsite_alias);
      return (ERROR);
   }

   model_run_id = runId;

   /* Insert model_run_id of 0, because procedure will update it 
      appropriately. */ 
   /* EXEC SQL AT :current_dbsite 
      INSERT INTO ref_model_run VALUES (0, :runName, :modelId, 
      sysdate, to_date (:runDate, 'dd-mon-yyyy hh24:mi:ss'), :prob:probNull, 
      :modelType:modelTypeNull, :userId:userIdNull, 
      to_date (:startDate:startDateNull, 'dd-mon-yyyy hh24:mi:ss'),
      to_date (:endDate:endDateNull, 'dd-mon-yyyy hh24:mi:ss'), 
      :timeStep:timeStepNull, :comment:commentNull); */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 11;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "insert into ref_model_run  values (0,:b1,:b2,sysdate,to_da\
te(:b3,'dd-mon-yyyy hh24:mi:ss'),:b4:b5,:b6:b7,:b8:b9,to_date(:b10:b11,'dd-mon\
-yyyy hh24:mi:ss'),to_date(:b12:b13,'dd-mon-yyyy hh24:mi:ss'),:b14:b15,:b16:b1\
7)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )28;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)runName;
   sqlstm.sqhstl[0] = (unsigned long )65;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&modelId;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)runDate;
   sqlstm.sqhstl[2] = (unsigned long )25;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&prob;
   sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)&probNull;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)modelType;
   sqlstm.sqhstl[4] = (unsigned long )25;
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)&modelTypeNull;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&userId;
   sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)&userIdNull;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqhstv[6] = (unsigned char  *)startDate;
   sqlstm.sqhstl[6] = (unsigned long )25;
   sqlstm.sqhsts[6] = (         int  )0;
   sqlstm.sqindv[6] = (         short *)&startDateNull;
   sqlstm.sqinds[6] = (         int  )0;
   sqlstm.sqharm[6] = (unsigned long )0;
   sqlstm.sqadto[6] = (unsigned short )0;
   sqlstm.sqtdso[6] = (unsigned short )0;
   sqlstm.sqhstv[7] = (unsigned char  *)endDate;
   sqlstm.sqhstl[7] = (unsigned long )25;
   sqlstm.sqhsts[7] = (         int  )0;
   sqlstm.sqindv[7] = (         short *)&endDateNull;
   sqlstm.sqinds[7] = (         int  )0;
   sqlstm.sqharm[7] = (unsigned long )0;
   sqlstm.sqadto[7] = (unsigned short )0;
   sqlstm.sqtdso[7] = (unsigned short )0;
   sqlstm.sqhstv[8] = (unsigned char  *)timeStep;
   sqlstm.sqhstl[8] = (unsigned long )129;
   sqlstm.sqhsts[8] = (         int  )0;
   sqlstm.sqindv[8] = (         short *)&timeStepNull;
   sqlstm.sqinds[8] = (         int  )0;
   sqlstm.sqharm[8] = (unsigned long )0;
   sqlstm.sqadto[8] = (unsigned short )0;
   sqlstm.sqtdso[8] = (unsigned short )0;
   sqlstm.sqhstv[9] = (unsigned char  *)comment;
   sqlstm.sqhstl[9] = (unsigned long )1001;
   sqlstm.sqhsts[9] = (         int  )0;
   sqlstm.sqindv[9] = (         short *)&commentNull;
   sqlstm.sqinds[9] = (         int  )0;
   sqlstm.sqharm[9] = (unsigned long )0;
   sqlstm.sqadto[9] = (unsigned short )0;
   sqlstm.sqtdso[9] = (unsigned short )0;
   sqlstm.sqhstv[10] = (unsigned char  *)current_dbsite;
   sqlstm.sqhstl[10] = (unsigned long )10;
   sqlstm.sqhsts[10] = (         int  )0;
   sqlstm.sqindv[10] = (         short *)0;
   sqlstm.sqinds[10] = (         int  )0;
   sqlstm.sqharm[10] = (unsigned long )0;
   sqlstm.sqadto[10] = (unsigned short )0;
   sqlstm.sqtdso[10] = (unsigned short )0;
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

  
   result = SqlErrorHandler(FUNC_NAME, FALSE, 
			       "Problem executing insert_new...\n");
   if(result != OK)
       return (ERROR);

   /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 11;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )87;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)current_dbsite;
   sqlstm.sqhstl[0] = (unsigned long )10;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
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
			     "Could not commit changes.\n");
   if(result != OK)
   {
      PrintError("ERROR in Commit\n");
      return (ERROR);
   }
   *suc = 1;
   return (OK);
}

#define FUNC_NAME "SqlUpdateModelRunId"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlUpdateModelRunId

<P> <STRONG>Function signature:</STRONG> int SqlUpdateModelRunId(model_run *, int *)

<P> <STRONG>Function description:</STRONG> Updates an existing model_run_id in ref_model_run based on the spec.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> model_run *spec: Specification of model_run_id to be updated.
<BR><DD> int *suc:        Indicator of success of model_run_id update. Output.

</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlUpdateModelRunId(model_run *spec, int *suc)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   short int probNull, modelTypeNull, userIdNull,
      startDateNull, endDateNull,
      timeStepNull, commentNull;
   int runId, modelId, prob, userId;
   char runName[65], runDate[25], modelType[25], startDate[25], 
      endDate[25], timeStep[129], comment[1001];
   /* EXEC SQL END DECLARE SECTION; */ 

   
   int result;
  
/* This routine updates run information in the ref_model_run table.
   We update the information at the local database only.  
   Remote sites will update their tables through a call to 
   SqlSyncModelRuns().  */

/* Initialize the indicator variables.  -1 is NULL and 0 is not NULL */
   
   probNull = modelTypeNull = userIdNull = startDateNull = endDateNull =
      timeStepNull = commentNull = 0;
   
   result = SwitchSession(dbsite_array[0].dbsite_alias);
   if(result != OK)
   {
      PrintError("ERROR occurred changing to session 1\n");
      return(ERROR);
   }
   
   /* The following four tests are to see if the first four fields
      in a record for ref_model_run_id are indeed not NULL.  If one of them
      is NULL, return but do not close any database connections; connections
      are closed by the top level calling routine. */
   
   if(spec[0].Name == NULL)
   {
      PrintError("Error: model_run_name may not be NULL\n");
      return (ERROR);
   }
   strcpy(runName, spec[0].Name);

   if(spec[0].modelId == NA)
   {
      PrintError("Error: model_Id may not be NULL\n");
      return (ERROR);
   }
   modelId = spec[0].modelId;

   if(spec[0].runDate == NULL)
   {
      PrintError("Error: model_run_date may not be NULL\n");
      return (ERROR);
   }
   if ((result = SqlFormatDate (spec[0].runDate, runDate)) != OK)
	{
	   PrintError ("Error formatting run date %s. Exiting. \n", 
		       spec[0].runDate);
	   return (ERROR);
	}

   if(spec[0].modelType == NULL)
   {
      PrintError("Error: modelType may not be NULL\n");
      return (ERROR);
   }
   strcpy(modelType, spec[0].modelType);
   
   if(spec[0].prob == NA) probNull = -1;
   else prob = spec[0].prob;
   if(spec[0].userId == NA) userIdNull = -1;
   else userId = spec[0].userId;
   if(spec[0].startDate[0] == '\0') startDateNull = -1;
   else if ((result = SqlFormatDate (spec[0].startDate, startDate)) != OK)
   {
      PrintError ("Error formatting start date %s. Exiting. \n", spec[0].startDate);
      return (ERROR);
   }
   if(spec[0].endDate[0] == '\0') endDateNull = -1;
   else if ((result = SqlFormatDate (spec[0].endDate, endDate)) != OK)
   {
      PrintError ("Error formatting end date %s. Exiting. \n", spec[0].endDate);
      return (ERROR);
   }
   if(spec[0].timeStep[0] == '\0') timeStepNull = -1;
   else strcpy(timeStep, spec[0].timeStep);
   if(spec[0].Comment[0] == '\0') commentNull = -1;
   else strcpy(comment, spec[0].Comment);

   /* Set the model_run_id to update */
   runId = spec[0].modelRunId;

   /* Insert model_run_id of 0, because procedure will update it 
      appropriately. */ 
   /* EXEC SQL AT :current_dbsite 
      UPDATE ref_model_run 
	SET model_run_name = :runName, 
	    model_id = :modelId, 
	    sys_date = sysdate,
	    run_date = to_date (:runDate, 'dd-mon-yyyy hh24:mi:ss'), 
	    probability = :prob:probNull, 
	    modeltype = :modelType:modelTypeNull, 
	    user_id = :userId:userIdNull, 
            start_date = to_date (:startDate:startDateNull, 'dd-mon-yyyy hh24:mi:ss'),
	    end_date = to_date (:endDate:endDateNull, 'dd-mon-yyyy hh24:mi:ss'), 
	    time_step_descriptor = :timeStep:timeStepNull, 
	    cmmnt = :comment:commentNull
	WHERE model_run_id = runId; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 11;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "update ref_model_run  set model_run_name=:b1,model_id=:b2,\
sys_date=sysdate,run_date=to_date(:b3,'dd-mon-yyyy hh24:mi:ss'),probability=:b\
4:b5,modeltype=:b6:b7,user_id=:b8:b9,start_date=to_date(:b10:b11,'dd-mon-yyyy \
hh24:mi:ss'),end_date=to_date(:b12:b13,'dd-mon-yyyy hh24:mi:ss'),time_step_des\
criptor=:b14:b15,cmmnt=:b16:b17 where model_run_id=runId";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )106;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)runName;
   sqlstm.sqhstl[0] = (unsigned long )65;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&modelId;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)runDate;
   sqlstm.sqhstl[2] = (unsigned long )25;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)&prob;
   sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)&probNull;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)modelType;
   sqlstm.sqhstl[4] = (unsigned long )25;
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)&modelTypeNull;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (unsigned char  *)&userId;
   sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         short *)&userIdNull;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned long )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqhstv[6] = (unsigned char  *)startDate;
   sqlstm.sqhstl[6] = (unsigned long )25;
   sqlstm.sqhsts[6] = (         int  )0;
   sqlstm.sqindv[6] = (         short *)&startDateNull;
   sqlstm.sqinds[6] = (         int  )0;
   sqlstm.sqharm[6] = (unsigned long )0;
   sqlstm.sqadto[6] = (unsigned short )0;
   sqlstm.sqtdso[6] = (unsigned short )0;
   sqlstm.sqhstv[7] = (unsigned char  *)endDate;
   sqlstm.sqhstl[7] = (unsigned long )25;
   sqlstm.sqhsts[7] = (         int  )0;
   sqlstm.sqindv[7] = (         short *)&endDateNull;
   sqlstm.sqinds[7] = (         int  )0;
   sqlstm.sqharm[7] = (unsigned long )0;
   sqlstm.sqadto[7] = (unsigned short )0;
   sqlstm.sqtdso[7] = (unsigned short )0;
   sqlstm.sqhstv[8] = (unsigned char  *)timeStep;
   sqlstm.sqhstl[8] = (unsigned long )129;
   sqlstm.sqhsts[8] = (         int  )0;
   sqlstm.sqindv[8] = (         short *)&timeStepNull;
   sqlstm.sqinds[8] = (         int  )0;
   sqlstm.sqharm[8] = (unsigned long )0;
   sqlstm.sqadto[8] = (unsigned short )0;
   sqlstm.sqtdso[8] = (unsigned short )0;
   sqlstm.sqhstv[9] = (unsigned char  *)comment;
   sqlstm.sqhstl[9] = (unsigned long )1001;
   sqlstm.sqhsts[9] = (         int  )0;
   sqlstm.sqindv[9] = (         short *)&commentNull;
   sqlstm.sqinds[9] = (         int  )0;
   sqlstm.sqharm[9] = (unsigned long )0;
   sqlstm.sqadto[9] = (unsigned short )0;
   sqlstm.sqtdso[9] = (unsigned short )0;
   sqlstm.sqhstv[10] = (unsigned char  *)current_dbsite;
   sqlstm.sqhstl[10] = (unsigned long )10;
   sqlstm.sqhsts[10] = (         int  )0;
   sqlstm.sqindv[10] = (         short *)0;
   sqlstm.sqinds[10] = (         int  )0;
   sqlstm.sqharm[10] = (unsigned long )0;
   sqlstm.sqadto[10] = (unsigned short )0;
   sqlstm.sqtdso[10] = (unsigned short )0;
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



   result = SqlErrorHandler(FUNC_NAME, FALSE, 
			       "Problem executing update of ref_model_run...\n");
   if(result != OK)
       return (ERROR);

   /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 11;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )165;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)current_dbsite;
   sqlstm.sqhstl[0] = (unsigned long )10;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
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
			     "Could not commit changes.\n");
   if(result != OK)
   {
      PrintError("ERROR in Commit\n");
      return (ERROR);
   }
   *suc = 1;
   return (OK);
}

