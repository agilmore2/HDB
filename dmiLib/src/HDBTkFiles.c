
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
           char  filnam[14];
};
static const struct sqlcxp sqlfpn =
{
    13,
    "HDBTkFiles.pc"
};


static unsigned int sqlctx = 582211;


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
   unsigned char  *sqhstv[13];
   unsigned long  sqhstl[13];
            int   sqhsts[13];
            short *sqindv[13];
            int   sqinds[13];
   unsigned long  sqharm[13];
   unsigned long  *sqharc[13];
   unsigned short  sqadto[13];
   unsigned short  sqtdso[13];
} sqlstm = {12,13};

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
5,0,0,1,0,0,1041,164,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
28,0,0,1,0,0,1039,176,0,0,0,0,0,0,128,1,97,0,0,
47,0,0,1,0,0,1069,180,0,0,0,0,0,0,128,1,97,0,0,
66,0,0,1,0,0,1039,184,0,0,0,0,0,0,128,1,97,0,0,
85,0,0,1,0,0,1037,192,0,0,12,0,0,0,128,2,3,0,0,2,9,0,0,2,3,0,0,2,97,0,0,2,97,0,
0,2,3,0,0,2,9,0,0,2,3,0,0,2,97,0,0,2,97,0,0,2,9,0,0,2,9,0,0,1,97,0,0,
152,0,0,1,0,0,1039,201,0,0,0,0,0,0,128,1,97,0,0,
171,0,0,1,0,0,1039,248,0,0,0,0,0,0,128,1,97,0,0,
190,0,0,2,0,0,1053,249,0,0,0,0,0,0,128,1,97,0,0,
};


/****************************************************************************
**
**	IDENTIFICATION SECTION
**	----------------------
**
**	Module Name:	HDBTk*
**
**	File Name:	HDB_TkFiles.c
**
**	Date Written:	August, 1997
**
**	Author(s): 	Nick Nielsen
**
**	Module Description:	
**           This file contains all of the C functions that get called from
**         the Tk interfaces of the input and output DMIs.
**		
**      Variables Passed In:  
**
*****************************************************************************/

#ifndef ORA_PROC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/param.h>
#include "tcl.h"
#include "tk.h"
#endif

#include <dbutils.h>
#include <utils.h>
#include <defines.h>
#include <typedefs.h>
#include "dmi_utils.h"
#include "typedefs.h"

#define BUFFERSIZE 500
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


#define FUNC_NAME "SqlSearchModelRunIdOutput"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlSearchModelRunIdOutput

<P> <STRONG>Function signature:</STRONG> int  SqlSearchModelRunIdOutput(model_run[MAPSIZE], model_run[MAPSIZE], int *)

<P> <STRONG>Function description:</STRONG> Retrieves from HDB all model_run_ids (and attributes) which match the specification passed in from the TCL interface.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> model_run search_results[MAPSIZE]: Model_run_ids and attributes which match the specification. Output.
<BR><DD> model_run spec[MAPSIZE]:           Specified model_run_id.
<BR><DD> int *n_of_records_found:           Number of matching model_run_id records. Output.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlSearchModelRunIdOutput(model_run search_results[MAPSIZE], 
			      model_run spec[MAPSIZE], int *n_of_records_found)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   /* varchar modelType[6], runName[100], comment[1000], 
      timeStep[129]; */ 
struct { unsigned short len; unsigned char arr[6]; } modelType;

struct { unsigned short len; unsigned char arr[100]; } runName;

struct { unsigned short len; unsigned char arr[1000]; } comment;

struct { unsigned short len; unsigned char arr[129]; } timeStep;

   char operateStmt[1000];
   SQL_DATE  sysDate, runDate, startDate, endDate;
   ID runId, modelId, prob, userId;
   short int mt_null, syd_null, rd_null, ts_null, sd_null, ed_null, md_null, 
	pr_null, rn_null, ct_null, ui_null;
   /* EXEC SQL END DECLARE SECTION; */ 
 
   
   int     result, flag, j;
   char    line[100];
   
   /*create the select statement 
    NOTE:  we put on a WHERE at the end; will NULL it out later if not 
    needed */
   sprintf(operateStmt, "SELECT model_run_id, model_run_name, model_id, to_char(sys_date, 'dd-mon-yyyy hh24:mi:ss'), to_char(run_date, 'dd-mon-yyyy'), probability, modeltype, user_id, to_char(start_date, 'dd-mon-yyyy'), to_char(end_date, 'dd-mon-yyyy'), time_step_descriptor, cmmnt FROM ref_model_run WHERE ");
   flag = 0; 
   

   if (spec[0].modelRunId != 0)
   {
      sprintf(line, "model_run_id = %d and ", spec[0].modelRunId);
      strcat(operateStmt, line);
      spec[0].modelRunId = NA;
      flag = 1;
   }
   
   if (spec[0].modelId != 0)
   {
      sprintf(line, "model_id = %d and ", spec[0].modelId);
      strcat(operateStmt, line);
      flag = 1;
   }
   
   if (strcmp(spec[0].sysDate, "\0"))
   {
      sprintf(line, "to_char (trunc (sys_date, 'dd'), 'dd-mon-yyyy') = lower ('%s') and ", 
	      spec[0].sysDate);
      strcat(operateStmt, line);
      flag = 1;
   }

   if (strcmp(spec[0].runDate, "\0"))
   {
      sprintf(line, "to_char (trunc (run_date, 'dd'), 'dd-mon-yyyy') = lower ('%s') and ", 
	      spec[0].runDate);
      strcat(operateStmt, line);
      flag = 1;
   }
   if (spec[0].prob != NA)
   {
      sprintf(line, "probability = %d and ", spec[0].prob);
      strcat(operateStmt, line);
      flag = 1;
   }
   if (strcmp(spec[0].modelType, "\0"))
   {
      sprintf(line, "modeltype = '%s' and ", spec[0].modelType);
      strcat(operateStmt, line);
      flag = 1;
   }
   if (spec[0].userId != NA)
   {
      sprintf(line, "user_id = %d and ", spec[0].userId);
      strcat(operateStmt, line);
      flag = 1;
   }
   if (strcmp(spec[0].startDate, "\0"))
   {
      sprintf(line, "to_char (trunc (start_date, 'dd'), 'dd-mon-yyyy') = lower ('%s') and ", 
	      spec[0].startDate);
      strcat(operateStmt, line);
      flag = 1;
   }
   if (strcmp(spec[0].endDate, "\0"))
   {
      sprintf(line, "to_char (trunc (end_date, 'dd'), 'dd-mon-yyyy') = lower ('%s') and ", 
	      spec[0].endDate);
      flag = 1;
      strcat(operateStmt, line);
   }
   
   /* Need to get rid of the last 'and' statement, cause there is an extra */
   if(flag == 1)
      operateStmt[strlen(operateStmt) - 4] = '\0';
   /* Need ot get rid of the WHERE at the end of the statement, because 
    nothing was specified for the search*/
   if(flag == 0)
      operateStmt[strlen(operateStmt) - 6] = '\0';
   sprintf (line, "ORDER BY model_run_id");
   strcat(operateStmt, line);

   /* printf("Select statement: %s\n", operateStmt); */

   /* Prepare the select statement; and open a cursor */
   if (flag) 
   {
      /* EXEC SQL AT :current_dbsite DECLARE sel STATEMENT; */ 

      /* EXEC SQL PREPARE sel FROM :operateStmt; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
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
      sqlstm.sqhstv[0] = (unsigned char  *)operateStmt;
      sqlstm.sqhstl[0] = (unsigned long )1000;
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
                               "Problem preparing sel.");
      if (result != OK)
	 return (ERROR);
      
      /* EXEC SQL WHENEVER NOT FOUND goto close_c1; */ 

      /* EXEC SQL DECLARE c2 CURSOR FOR sel; */ 

      
      if((result = SqlErrorHandler
	   (FUNC_NAME, FALSE, "Problem declaring cursor c2.")) != OK)
      {
	 /* EXEC SQL CLOSE c2; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 2;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )28;
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


	 return (ERROR);
      }
      
      /* EXEC SQL OPEN c2; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 2;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )47;
      sqlstm.selerr = (unsigned short)1;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqcmod = (unsigned int )0;
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


      if ((result = SqlErrorHandler
	   (FUNC_NAME, FALSE, "Problem opening cursor c2.")) != OK)
      {
	 /* EXEC SQL CLOSE c2; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 2;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )66;
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


	 return (ERROR);
      }
      runId = NA;
      j = 0; /* initialize j */
      while(TRUE) /* SQL will automatically go to close_c1 when it can't read
		  anymore model_runs  */
      {
	 /* EXEC SQL FETCH c2 INTO :runId,:runName:rn_null,:modelId:md_null,
	                  :sysDate:syd_null,:runDate:rd_null,:prob:pr_null,
                          :modelType:mt_null,
	                  :userId:ui_null,:startDate:sd_null,:endDate:ed_null,
	                  :timeStep:ts_null,:comment:ct_null; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 13;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )85;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (         int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (unsigned char  *)&runId;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)&runName;
  sqlstm.sqhstl[1] = (unsigned long )102;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)&rn_null;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&modelId;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)&md_null;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)sysDate;
  sqlstm.sqhstl[3] = (unsigned long )21;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)&syd_null;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)runDate;
  sqlstm.sqhstl[4] = (unsigned long )21;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)&rd_null;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (unsigned char  *)&prob;
  sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         short *)&pr_null;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned long )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (unsigned char  *)&modelType;
  sqlstm.sqhstl[6] = (unsigned long )8;
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         short *)&mt_null;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned long )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (unsigned char  *)&userId;
  sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         short *)&ui_null;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned long )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (unsigned char  *)startDate;
  sqlstm.sqhstl[8] = (unsigned long )21;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         short *)&sd_null;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned long )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (unsigned char  *)endDate;
  sqlstm.sqhstl[9] = (unsigned long )21;
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         short *)&ed_null;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned long )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
  sqlstm.sqhstv[10] = (unsigned char  *)&timeStep;
  sqlstm.sqhstl[10] = (unsigned long )131;
  sqlstm.sqhsts[10] = (         int  )0;
  sqlstm.sqindv[10] = (         short *)&ts_null;
  sqlstm.sqinds[10] = (         int  )0;
  sqlstm.sqharm[10] = (unsigned long )0;
  sqlstm.sqadto[10] = (unsigned short )0;
  sqlstm.sqtdso[10] = (unsigned short )0;
  sqlstm.sqhstv[11] = (unsigned char  *)&comment;
  sqlstm.sqhstl[11] = (unsigned long )1002;
  sqlstm.sqhsts[11] = (         int  )0;
  sqlstm.sqindv[11] = (         short *)&ct_null;
  sqlstm.sqinds[11] = (         int  )0;
  sqlstm.sqharm[11] = (unsigned long )0;
  sqlstm.sqadto[11] = (unsigned short )0;
  sqlstm.sqtdso[11] = (unsigned short )0;
  sqlstm.sqhstv[12] = (unsigned char  *)current_dbsite;
  sqlstm.sqhstl[12] = (unsigned long )10;
  sqlstm.sqhsts[12] = (         int  )0;
  sqlstm.sqindv[12] = (         short *)0;
  sqlstm.sqinds[12] = (         int  )0;
  sqlstm.sqharm[12] = (unsigned long )0;
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
  if (sqlca.sqlcode == 1403) goto close_c1;
}


	 if ((result = SqlErrorHandler	      
	      (FUNC_NAME, TRUE, 
	       "Problem fetching into Ref_Model_Run on row %d.\n", j)) != OK)
	 {
	    /* EXEC SQL CLOSE c2; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 12;
     sqlstm.arrsiz = 13;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )152;
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


	    return (ERROR);
	 }
	 strcpy(search_results[j].code, spec[0].code);
	 search_results[j].modelRunId = runId;
	 if(md_null == -1) search_results[j].modelId = NA;
	 else search_results[j].modelId = modelId;
	 if(syd_null == -1) strcpy(search_results[j].sysDate, "\0"); 
	 else strcpy(search_results[j].sysDate, sysDate);
	 if(rd_null == -1) strcpy(search_results[j].runDate, "\0"); 
	 else strcpy(search_results[j].runDate, runDate);
	 if(pr_null == -1) search_results[j].prob = NA;
	 else search_results[j].prob = prob;
	 if(mt_null == -1) strcpy(search_results[j].modelType, "\0");
	 else 
	 {
	    strncpy(search_results[j].modelType, modelType.arr,
		    modelType.len);
	    search_results[j].modelType[modelType.len] = '\0';
	 }
	 if(ui_null == -1) search_results[j].userId = NA;
	 else search_results[j].userId = userId;
	 if(sd_null == -1) strcpy(search_results[j].startDate, "\0");
	 else strcpy(search_results[j].startDate, startDate);
	 if(ed_null == -1) strcpy(search_results[j].endDate, "\0");
	 else strcpy(search_results[j].endDate, endDate);
	 if(ts_null == -1) strcpy(search_results[j].timeStep, "\0");
	 else 
	    {
	       strncpy(search_results[j].timeStep, timeStep.arr, timeStep.len);
	       search_results[j].timeStep[timeStep.len] = '\0';
	    }
	 if(rn_null == -1) strcpy(search_results[j].Name, "\0");
	 else 
	    {
	       strncpy(search_results[j].Name, runName.arr, runName.len);
	       search_results[j].Name[runName.len] = '\0';
	    }
	 if(ct_null == -1) strcpy(search_results[j].Comment, "\0");
	 else 
	    {
	       strncpy(search_results[j].Comment, comment.arr, comment.len);
	       search_results[j].Comment[comment.len] = '\0';
	    }
	 j++;
      }
   close_c1:
      /* EXEC SQL CLOSE c2; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 13;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )171;
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


      /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 13;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )190;
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


   }

   *n_of_records_found = j;
   return (OK);
}

#define FUNC_NAME "StartTkOutputInterface"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> StartTkOutputInterface

<P> <STRONG>Function signature:</STRONG> int StartTkOutputInterface(char *)

<P> <STRONG>Function description:</STRONG> Starts the Output DMI interface.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *pathname: The path to the TK files.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int StartTkOutputInterface(char *pathname)
{
   Tcl_Interp *interp; 
   int code;
   
   char path[MAXPATHLEN];
   char *env;
   char tmp[BUFFERSIZE];
   char *last;

   interp = Tcl_CreateInterp(); 
   if (Tcl_Init(interp) == TCL_ERROR) 
   { 
      fprintf(stderr, "Tcl_Init failed: %s\n", interp->result); 
      return (ERROR);
   } 
   if (Tk_Init(interp) == TCL_ERROR) 
   { 
      fprintf(stderr, "Tk_Init failed: %s\n", interp->result); 
      return (ERROR);
   } 
   Tcl_CreateCommand(interp, "HDBTk_readModelMap", HDBTk_readModelMap, 
		     NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_formatModelMapLine", 
		     HDBTk_formatModelMapLine, NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_condenseData", HDBTk_condenseData, 
		     NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_stringAppend", HDBTk_stringAppend, 
		     NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_StartSearchOutput", 
		     HDBTk_StartSearchOutput, NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_SaveModelRunIdOutput", 
		     HDBTk_SaveModelRunIdOutput, NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_StartCreate", HDBTk_StartCreate, 
		     NULL, NULL);

   strcpy(path, pathname);
   last = strrchr(path, '/');

   if(last != NULL) /* character was found */
   {
      *last = '\0';
   }
   else
   {
      env = getenv("OUTPUT_DMI_PATH");
      if(env == NULL)
      {
	    printf("Error -- OUTPUT_DMI_PATH is not defined in this environment.\n");
	    return (ERROR);
      }
      strcpy(path, env);
      /*      getcwd(path);      get current working directory */
   }
   sprintf(tmp, "%s%s", path, "/riverwareDMI.Out.GUI.tk");
   
   code = Tcl_EvalFile(interp, tmp); 
   if (code != OK)
   {
      /* printf("%s\n", interp->result); */
      printf("Error: could not evaluate Tcl file --\n   Make sure that riverwareDMI.Out and riverwareDMI.Out.GUI.tk are in the same directory.\n");
      return (ERROR);
   }
   Tk_MainLoop();

   Tcl_DeleteInterp(interp);
   if(model_run_id == -1)
   {
      printf("\n\nExiting\n");
      return (USER_QUIT);
   }
   
   printf("User specified Model Run Id: %d\n", model_run_id);
   return (OK);
}


#define FUNC_NAME "StartTkInputInterface"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> StartTkInputInterface

<P> <STRONG>Function signature:</STRONG> int StartTkInputInterface(char *)

<P> <STRONG>Function description:</STRONG> Starts the Input DMI interface.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *pathname: The path to the TK files.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int StartTkInputInterface(char *pathname)
{
   int i;
   Tcl_Interp *interp; 
   int code; 
   char *last;
   char *env;
   char path[BUFFERSIZE];
   char tmp[BUFFERSIZE];
   
   interp = Tcl_CreateInterp(); 
   if (Tcl_Init(interp) == TCL_ERROR) 
   { 
      fprintf(stderr, "Tcl_Init failed: %s\n", interp->result); 
      return (ERROR);
   } 
   if (Tk_Init(interp) == TCL_ERROR) 
   { 
      fprintf(stderr, "Tk_Init failed: %s\n", interp->result); 
      return (ERROR);
   } 
   Tcl_CreateCommand(interp, "HDBTk_readModelMap", HDBTk_readModelMap, 
		     NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_formatModelMapLine", 
		     HDBTk_formatModelMapLine, NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_condenseData", HDBTk_condenseData, 
		     NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_stringAppend", HDBTk_stringAppend, 
		     NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_StartSearchInput", HDBTk_StartSearchInput, 
		     NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_SaveModelRunIdInput", 
		     HDBTk_SaveModelRunIdInput, NULL, NULL);
   Tcl_CreateCommand(interp, "HDBTk_createModelmap", HDBTk_createModelmap,
		     NULL, NULL);

   /*   env = getenv("NEW_TK");
	if(env == NULL)
	{
	printf("Error -- NEW_TK is not defined in this environment.");
	return (ERROR);
	}*/
   /*   strcpy(path, "/projects/riverware/staff/nielsenn/HDB_applications");*/
   
   strcpy(path, pathname);
   last = strrchr(path, '/');

   if(last != NULL) /* character was found */
   {
      *last = '\0';
   }
   else
   {
      env = getenv("INPUT_DMI_PATH");
      if(env == NULL)
      {
	 printf("Error -- INPUT_DMI_PATH is not defined in this environment.\n");
	 return (ERROR);
      }
      strcpy(path, env);
      /*      getcwd(path);      get current working directory */
   }
   sprintf(tmp, "%s%s", path, "/riverwareDMI.In.GUI.tk");

   code = Tcl_EvalFile(interp, tmp); 
   if (code != OK)
   {
      /* printf("%s\n", interp->result); */
      printf("Error: could not evaluate Tcl file --\n    Make sure that riverwareDMI.In and riverwareDMI.In.GUI.tk are in the same directory.\n");
      return (ERROR);
   }
   Tk_MainLoop();

   Tcl_DeleteInterp(interp);
   if(model_run_id == -1)
   {
      printf("\n\nExiting...\n");
      return (ERROR);
   }
   
   for(i = 0; i < modelRunMapCount; i++)
      printf("User specified Model Run Id: %d\n", modelRunMap[i].modelRunId);
   return (OK);
}


#define FUNC_NAME "CondenseSearchResultsOutput"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> CondenseSearchResultsOutput

<P> <STRONG>Function signature:</STRONG> int CondenseSearchResultsOutput(Tcl_Interp *, model_run *, int)

<P> <STRONG>Function description:</STRONG> Creates a large string filled with the results from a search. This string is in a form that can be given back to Tcl to deal with. 

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> Tcl_Interp *interp: TCL string holding search results.
<BR><DD> model_run *spec:    Search results.
<BR><DD> int counter:        Number of records in spec.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int CondenseSearchResultsOutput(Tcl_Interp *interp, model_run *spec, 
				int counter)
{
   /*  This function gets a big string filled with the results from a search
       so that it is in a form that can be given back to Tcl and dealt with */
   char buffer[1000] = "\0";
   int i;
   char *varValue;

   for (i = 0; i < counter; i++)
   {
      sprintf(buffer, "%d", spec[i].modelRunId);
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].Name);      
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%d", spec[i].modelId);  
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].sysDate);   
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].runDate);   
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%d", spec[i].prob);     
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].modelType); 
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%d", spec[i].userId);  
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].startDate);  
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].endDate);    
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].timeStep);  
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].Comment);  
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      
      /*  printf("\nResult for spec[%d]:\n", i);
	      printf("%d\n", spec[i].modelRunId); 
	      printf("%s\n", spec[i].Name);       
	      printf("%d\n", spec[i].modelId); 
	      printf("%s\n", spec[i].sysDate);   
	      printf("%s\n", spec[i].runDate);   
	      printf("%d\n", spec[i].prob);    
	      printf("%s\n", spec[i].modelType); 
	      printf("%d\n", spec[i].userId);   
	      printf("%s\n", spec[i].startDate); 
	      printf("%s\n", spec[i].endDate);    
	      printf("%s\n", spec[i].timeStep); 
	      printf("%s\n", spec[i].Comment);  */
   }
   varValue = Tcl_SetVar(interp, "DataList", "ERR", TCL_LIST_ELEMENT | 
			 TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
   return (OK);
}
#endif

#define FUNC_NAME "CondenseSearchResultsInput"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> CondenseSearchResultsInput

<P> <STRONG>Function signature:</STRONG> int CondenseSearchResultsInput(Tcl_Interp *, model_run *, int)

<P> <STRONG>Function description:</STRONG> Creates a large string filled with the results from a search. This string is in a form that can be given back to Tcl to deal with. 

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> Tcl_Interp *interp: TCL string holding search results.
<BR><DD> model_run *spec:    Search results.
<BR><DD> int counter:        Number of records in spec.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int CondenseSearchResultsInput(Tcl_Interp *interp, model_run *spec, 
			  int counter)
{
   /*  This function gets a big string filled with the results from a search
       so that it is in a form that can be given back to Tcl and dealt with */
   char buffer[1000] = "\0";
   int i;
   char *varValue;

   for (i = 0; i < counter; i++)
   {
      sprintf(buffer, "%s", spec[i].code);
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%d", spec[i].modelRunId);
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%d", spec[i].modelId);  
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].sysDate);   
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].runDate);   
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%d", spec[i].prob);     
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].modelType); 
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%d", spec[i].userId);  
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].startDate);  
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].endDate);    
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      sprintf(buffer, "%s", spec[i].timeStep);  
      varValue = Tcl_SetVar(interp, "DataList", buffer, TCL_LIST_ELEMENT | 
			    TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
      
      /*  printf("\nResult for spec[%d]:\n", i);
	      printf("%s\n", spec[i].code);
	      printf("%d\n", spec[i].modelRunId); 
	      printf("%d\n", spec[i].modelId); 
	      printf("%s\n", spec[i].sysDate);   
	      printf("%s\n", spec[i].runDate);   
	      printf("%d\n", spec[i].prob);    
	      printf("%s\n", spec[i].modelType); 
	      printf("%d\n", spec[i].userId);   
	      printf("%s\n", spec[i].startDate); 
	      printf("%s\n", spec[i].endDate);    
	      printf("%s\n", spec[i].timeStep); */
   }
   varValue = Tcl_SetVar(interp, "DataList", "ERR", TCL_LIST_ELEMENT | 
			 TCL_APPEND_VALUE |  TCL_LEAVE_ERR_MSG);
   return (OK);
}
#endif

/*  This function is called by Tcl, and it will read the modelMap info */
#define FUNC_NAME "HDBTk_readModelMap"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_readModelMap

<P> <STRONG>Function signature:</STRONG> int HDBTk_readModelMap(ClientData, Tcl_Interp *, int, char **)

<P> <STRONG>Function description:</STRONG> Reads the modelMap information.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientDate clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int HDBTk_readModelMap(ClientData clientData, Tcl_Interp *interp, 
		 int argc, char **argv)
{
   char str[5000] = "\0";
   char buf[200] = "\0";
   model_map modelMap[MAPSIZE];
   int counter = 0;
   int i, result;
   
   str[0] = '\0';
   if ((result = SqlGetModelIdArray(modelMap, &counter)) != OK)
   {
      printf("Error getting the model Id array...");
      rmTextFile();
      return (ERROR);
   }
   
   for(i = 0; i < counter; i++)
   {
      sprintf(buf, "%d\n", modelMap[i].modelId);
      strcat(str, buf);
      sprintf(buf, "%s\n", modelMap[i].name);
      strcat(str, buf);
   }
   Tcl_SetResult(interp, str, TCL_STATIC);
   return (TCL_OK);
}
#endif

/*  This function is called by Tcl, and it will format the modelMap lines */
#define FUNC_NAME "HDBTk_formatModelMapLine"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_formatModelMapLine

<P> <STRONG>Function signature:</STRONG> int HDBTk_formatModelMapLine(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)

<P> <STRONG>Function description:</STRONG> Formats each line of the ModelMap so that it is easily usable in Tk.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientData clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONg>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int HDBTk_formatModelMapLine(ClientData clientData, Tcl_Interp *interp, 
		       int argc, char **argv)
{
   char str[1000] = "\0";
   char *block = argv[1];
   int number = atoi(argv[2]);
   int cursor = 0;
   int counter = 0;
   int i = 0;

   while(counter != (number * 2))
   {
      if(block[cursor] == '\n')
	 counter++;
      cursor++;  /* step the cursor down one character */
      if(cursor >= strlen(block))
      {
	 Tcl_SetResult(interp, "-1", TCL_STATIC);
	 return (TCL_OK);
      }
   }
   while(block[cursor] != '\n')
   {
      str[i] = block[cursor];
      cursor++;
      i++;
   }
   str[i] = '\0'; /* end str with a NULL so that strcat will take it */
   i++;
   cursor++;
   strcat(str, " ---- ");
   i = i + 5;
   while(block[cursor] != '\n')
   {
      str[i] = block[cursor];
      cursor++;
      i++;
   }
   str[i] = '\0';
   Tcl_SetResult(interp, str, TCL_STATIC);
   return (TCL_OK);
}
#endif

#define FUNC_NAME "HDBTk_condenseData"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_condenseData

<P> <STRONG>Function signature:</STRONG> int HDBTk_condenseData(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)

<P> <STRONG>Function description:</STRONG> This function can be called from Tcl to combine two pieces of data, so that it can be used later from within Tcl.  Note: it puts a newline on the end of the resulting string.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientDate clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int HDBTk_condenseData(ClientData clientData, Tcl_Interp *interp, int argc,
		 char **argv)
{
   char tmp[1000] = "\0";
   sprintf(tmp, "%s%s\n", argv[1], argv[2]);
   Tcl_SetResult(interp, tmp, TCL_STATIC);
   return (TCL_OK);
}
#endif

#define FUNC_NAME "HDBTk_stringAppend"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_stringAppend

<P> <STRONG>Function signature:</STRONG> int HDBTk_readModelMap(ClientData, Tcl_Interp *, int, char **)

<P> <STRONG>Function description:</STRONG> This function can be called from Tcl to combine two pieces of data, so that it can be used later from within Tcl.  Note:  it does NOT put a newline at the end of the resulting string.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientDate clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int HDBTk_stringAppend(ClientData clientData, Tcl_Interp *interp, int argc,
		 char **argv)
{
   char tmp[1000] = "\0";
   sprintf(tmp, "%s%s", argv[1], argv[2]);
   Tcl_SetResult(interp, tmp, TCL_STATIC);
   return (TCL_OK);
}
#endif

#define FUNC_NAME "HDBTk_StartSearchOutput"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_StartSearchOutput

<P> <STRONG>Function signature:</STRONG> int HDBTk_StartSearchOutput(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)

<P> <STRONG>Function description:</STRONG> This function can be called from Tcl to start a search for the output DMI.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientDate clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int HDBTk_StartSearchOutput(ClientData clientData, Tcl_Interp *interp, 
			    int argc, char **argv)
{
   model_run spec[MAPSIZE];
   model_run search_results[MAPSIZE];
   char *datablock = argv[1];
   int n_of_records_found = 0;
   int result;
   
   FillSpecDataOutput(datablock, spec);
   result = SqlSearchModelRunIdOutput(search_results, spec, 
				      &n_of_records_found);
   if(result != OK)
   {
      printf("Error in SqlSearchModelRunIdOutput...\n");
      return (ERROR);
   }
   result = CondenseSearchResultsOutput(interp, search_results, 
					n_of_records_found);
   if(result != OK )
   {
      printf("Error giving results back to Tcl...\n");
      return(ERROR);
   }
   return (TCL_OK);
}
#endif

#define FUNC_NAME "HDBTk_StartSearchInput"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_StartSearchInput

<P> <STRONG>Function signature:</STRONG> int HDBTk_StartSearchInput(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)

<P> <STRONG>Function description:</STRONG> This function can be called from Tcl to start a search for the input DMI.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientDate clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

#ifndef ORA_PROC
int HDBTk_StartSearchInput(ClientData clientData, Tcl_Interp *interp, int argc,
		 char **argv)
{
   model_run spec[MAPSIZE];
   model_run search_results[MAPSIZE];
   char *datablock = argv[1];
   int n_of_records_found = 0;
   int result;
   
   FillSpecDataInput(datablock, spec);
   result = SqlSearchModelRunIdInput(search_results, spec, 
				     &n_of_records_found);
   if(result != OK)
   {
      printf("Error in SqlSearchModelRunIdInput...\n");
      return (ERROR);
   }
   result = CondenseSearchResultsInput(interp, search_results, 
				       n_of_records_found);
   if(result != OK )
   {
      printf("Error giving results back to Tcl...\n");
      return(ERROR);
   }
   return (TCL_OK);
}
#endif

#define FUNC_NAME "HDBTk_StartCreate"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_StartCreate

<P> <STRONG>Function signature:</STRONG> int HDBTk_StartCreate(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)

<P> <STRONG>Function description:</STRONG> This function can be called from Tcl to create a new model run id for the output dmi.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientDate clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int HDBTk_StartCreate(ClientData clientData, Tcl_Interp *interp, int argc,
		 char **argv)
{
   model_run spec[MAPSIZE];
   int success;
   char *datablock = argv[1];
   char buffer[10] = "\0";
   int result;
   int cre_suc;  /* You must pass SqlCreateModelRunId this parameter,
		    But I am not using it for anything  */
   
   FillSpecDataOutput(datablock, spec);   
   result = SqlCreateModelRunId(spec, &cre_suc);
   if(result != OK)
   {
      printf("Error creating model_run_id...");
      success = 0;
   }
   success = model_run_id;
   if(cre_suc == 0)
      success = 0;
   sprintf(buffer, "%d", success);
   Tcl_SetResult(interp, buffer, TCL_STATIC);
   return (TCL_OK);
}
#endif

#define FUNC_NAME "HDBTk_StartUpdate"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_StartUpdate

<P> <STRONG>Function signature:</STRONG> int HDBTk_StartUpdate(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)

<P> <STRONG>Function description:</STRONG> This function can be called from Tcl to update an existing model run id for the output dmi.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientDate clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int HDBTk_StartUpdate(ClientData clientData, Tcl_Interp *interp, int argc,
		 char **argv)
{
   model_run spec[MAPSIZE];
   int success;
   char *datablock = argv[1];
   char buffer[10] = "\0";
   int result;
   int upd_suc;  /* You must pass SqlUpdateModelRunId this parameter,
		    But I am not using it for anything  */
   
   FillSpecDataOutput(datablock, spec);   
   result = SqlUpdateModelRunId(spec, &upd_suc);
   if(result != OK)
   {
      printf("Error updating model_run_id...");
      success = 0;
   }
   success = model_run_id;
   if(upd_suc == 0)
      success = 0;
   sprintf(buffer, "%d", success);
   Tcl_SetResult(interp, buffer, TCL_STATIC);
   return (TCL_OK);
}
#endif

#define FUNC_NAME "FillSpecDataOutput"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> FillSpecDataOutput

<P> <STRONG>Function signature:</STRONG> int FillSpecDataOutput(char *datablock, model_run spec[MAPSIZE])

<P> <STRONG>Function description:</STRONG> This function will fill the model_run spec with the chunk of data contained in the string datablock, that was received from Tcl.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *datablock:
<BR><DD> model_run spec[MAPSIZE]
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int FillSpecDataOutput(char *datablock, model_run spec[MAPSIZE])
{
  char s[MAX_STACK_DEPTH];
   
  parseDataLine(datablock, 0, s);
  spec[0].modelRunId = atoi(s);

  parseDataLine(datablock, 1, s);
  strcpy(spec[0].Name,s);

  parseDataLine(datablock, 2, s);
  spec[0].modelId = atoi(s);

  parseDataLine(datablock, 3, s);
  strcpy(spec[0].sysDate, s);

  parseDataLine(datablock, 4, s);
  strcpy(spec[0].runDate, s);

  parseDataLine(datablock, 5, s);
  spec[0].prob = atoi(s);
  if(spec[0].prob == 0) 
     spec[0].prob = NA;

  parseDataLine(datablock, 6, s);
  strcpy(spec[0].modelType, s);

  parseDataLine(datablock, 7, s);
  spec[0].userId = atoi(s);
  if(spec[0].userId == 0) 
     spec[0].userId = NA;
  
  parseDataLine(datablock, 8, s);
  strcpy(spec[0].startDate, s);

  parseDataLine(datablock, 9, s);
  strcpy(spec[0].endDate, s);
  
  parseDataLine(datablock, 10, s);
  strcpy(spec[0].timeStep, s);
  
  parseDataLine(datablock, 11, s);
  strcpy(spec[0].Comment, s);
   
  /*  printf("%d\n", spec[0].modelRunId);
      printf("%s\n", spec[0].Name);
      printf("%d\n",spec[0].modelId);
      printf("%s\n",spec[0].sysDate);
      printf("%s\n",spec[0].runDate);
      printf("%d\n",spec[0].prob);
      printf("%s\n",spec[0].modelType);
      printf("%d\n",spec[0].userId);
      printf("%s\n",spec[0].startDate);
      printf("%s\n",spec[0].endDate);
      printf("%s\n", spec[0].timeStep);
      printf("%s\n", spec[0].Comment);
      printf("%s\n", datablock); */
  
  return (OK);
  
}

#define FUNC_NAME "FillSpecDataInput"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> FillSpecDataInput

<P> <STRONG>Function signature:</STRONG> int FillSpecDataInput(char *datablock, model_run spec[MAPSIZE])

<P> <STRONG>Function description:</STRONG> This function will fill the model_run spec with the chunk of data contained in the string datablock, that was received from Tcl.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *datablock
<BR><DD> model_run spec[MAPSIZE]
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int FillSpecDataInput(char *datablock, model_run spec[MAPSIZE])
{
  char s[MAX_STACK_DEPTH];
  int num = 11;  /* number of items */
  int i;

  for(i = 0; i < modelRunMapCount; i++)
  {
     parseDataLine(datablock, (num * i) + 0, s);
     strcpy(spec[i].code, s);
     
     parseDataLine(datablock, (num * i) + 1, s);
     spec[i].modelRunId = atoi(s);
     
     parseDataLine(datablock, (num * i) + 2, s);
     spec[i].modelId = atoi(s);
     
     parseDataLine(datablock, (num * i) + 3, s);
     strcpy(spec[i].sysDate, s);

     parseDataLine(datablock, (num * i) + 4, s);
     strcpy(spec[i].runDate, s);
     
     parseDataLine(datablock, (num * i) + 5, s);
     spec[i].prob = atoi(s);
     if(spec[i].prob == 0) 
	spec[i].prob = NA;
     
     parseDataLine(datablock, (num * i) + 6, s);
     strcpy(spec[i].modelType, s);
     
     parseDataLine(datablock, (num * i) + 7, s);
     spec[i].userId = atoi(s);
     if(spec[i].userId == 0) 
	spec[i].userId = NA;
     
     parseDataLine(datablock, (num * i) + 8, s);
     strcpy(spec[i].startDate, s);
     
     parseDataLine(datablock, (num * i) + 9, s);
     strcpy(spec[i].endDate, s);
     
     parseDataLine(datablock, (num * i) + 10, s);
     strcpy(spec[i].timeStep, s);
     
  /*  printf("%d\n", spec[0].modelRunId);
	 printf("%d\n",spec[0].modelId);
	 printf("%s\n",spec[0].sysDate);
	 printf("%s\n",spec[0].runDate);
	 printf("%d\n",spec[0].prob);
	 printf("%s\n",spec[0].modelType);
	 printf("%d\n",spec[0].userId);
	 printf("%s\n",spec[0].startDate);
	 printf("%s\n",spec[0].endDate);
	 printf("%s\n", spec[0].timeStep);
	 printf("%s\n", datablock); */
  }
  return (OK);
}

#define FUNC_NAME "parseDataLine"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> parseDataLine

<P> <STRONG>Function signature:</STRONG> int parseDataLine(char block[], int number, char parsed[])

<P> <STRONG>Function description:</STRONG> This function will separate a large
character array into chunks separated by newline characters.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char block[]:
<BR><DD> int number:
<BR><DD> char parsed[]:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int parseDataLine(char block[], int number, char parsed[])
{
   int count = 0;
   int cursor = 0;
   while(count < number)
   {
      if(block[cursor] == '\n')
      {
	 count++;
      }
      cursor++;
      if(cursor >= strlen(block))
	 return (ERROR);
   }
   count = 0;
   while(block[cursor] != '\n')
   {
      parsed[count] = block[cursor];
      count++;
      cursor++;
      if(count >= MAX_STACK_DEPTH)
	 return (ERROR);
   }
   parsed[count] = '\0';
   return (OK);
}

#define FUNC_NAME "HDBTk_SaveModelRunIdOutput"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_SaveModelRunIdOutput

<P> <STRONG>Function signature:</STRONG> HDBTk_SaveModelRunIdOutput(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)

<P> <STRONG>Function description:</STRONG> This will take the value selected in the Tcl code, and save it as the model_run_id.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientData clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int HDBTk_SaveModelRunIdOutput(ClientData clientData, Tcl_Interp *interp, 
			       int argc, char **argv)
{
   model_run_id = atoi(argv[1]);
   return (TCL_OK);
}
#endif

#define FUNC_NAME "HDBTk_SaveModelRunIdInput"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_SaveModelRunIdInput

<P> <STRONG>Function signature:</STRONG> HDBTk_SaveModelRunIdIntput(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)

<P> <STRONG>Function description:</STRONG> This will take the value(s) selected in the Tcl code, and save it as the model_run_id.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientData clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int HDBTk_SaveModelRunIdInput(ClientData clientData, Tcl_Interp *interp, 
			      int argc, char **argv)
{
   int run_id, index;

   run_id = atoi(argv[1]);

   /* This is a signal that the user chose to quit. */
   if(run_id == -1)
   {
      model_run_id = -1;
      return (TCL_OK);
   }

   index = atoi(argv[2]);

   modelRunMap[index].modelRunId = run_id;
   return (TCL_OK);
}
#endif

#define FUNC_NAME "HDBTk_createModelmap"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> HDBTk_createModelmap

<P> <STRONG>Function signature:</STRONG> int HDBTk_createModelmap(ClientData
clientData, Tcl_Interp *interp, int argc, char **argv)

<P> <STRONG>Function description:</STRONG> This function will create the list of references such as DEF, etc.  These are used in referencing certain parts of data in the input and output DMIs, and this list is known as the Modelmap.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ClientData clientData:
<BR><DD> Tcl_Interp *interp:
<BR><DD> int argc:
<BR><DD> char **argv:
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
#ifndef ORA_PROC
int HDBTk_createModelmap(ClientData clientData, Tcl_Interp *interp, int argc,
		char **argv)
{
   model_map modelMap[MAPSIZE];

   int result, i, counter;
   char tmp[BUFFERSIZE];
   
   result = SqlGetModelIdArray(modelMap, &counter);
   if (result != OK)
      return (TCL_ERROR);
   
   for(i = 0; i < modelRunMapCount; i++)
   {
      sprintf(tmp, "%s", modelRunMap[i].code);   
      Tcl_SetVar(interp, "codes", tmp, TCL_LIST_ELEMENT
			  | TCL_APPEND_VALUE | TCL_LEAVE_ERR_MSG);
   }   
   
   for(i=0; i<counter; i++)
   {
      sprintf(tmp, "%d", modelMap[i].modelId);
      Tcl_SetVar(interp, "readmodel_modelId", tmp, TCL_LIST_ELEMENT
			  | TCL_APPEND_VALUE | TCL_LEAVE_ERR_MSG);
      sprintf(tmp, "%s", modelMap[i].name);
      Tcl_SetVar(interp, "readmodel_name", tmp, TCL_LIST_ELEMENT
			  | TCL_APPEND_VALUE | TCL_LEAVE_ERR_MSG);
   }
   return (TCL_OK);
}
#endif
