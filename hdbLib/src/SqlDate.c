
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
           char  filnam[11];
};
static const struct sqlcxp sqlfpn =
{
    10,
    "SqlDate.pc"
};


static unsigned int sqlctx = 81699;


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
   unsigned char  *sqhstv[5];
   unsigned long  sqhstl[5];
            int   sqhsts[5];
            short *sqindv[5];
            int   sqinds[5];
   unsigned long  sqharm[5];
   unsigned long  *sqharc[5];
   unsigned short  sqadto[5];
   unsigned short  sqtdso[5];
} sqlstm = {12,5};

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
5,0,0,1,110,0,1028,66,0,0,3,2,0,0,128,1,97,0,0,1,97,0,0,2,97,0,0,1,97,0,0,
36,0,0,2,0,0,1055,76,0,0,0,0,0,0,128,1,97,0,0,
55,0,0,3,58,0,1028,116,0,0,1,0,0,0,128,2,97,0,0,1,97,0,0,
78,0,0,4,0,0,1055,124,0,0,0,0,0,0,128,1,97,0,0,
97,0,0,5,69,0,1028,162,0,0,1,0,0,0,128,2,97,0,0,1,97,0,0,
120,0,0,6,0,0,1055,170,0,0,0,0,0,0,128,1,97,0,0,
139,0,0,7,98,0,1028,354,0,0,2,1,0,0,128,1,97,0,0,2,97,0,0,1,97,0,0,
166,0,0,8,0,0,1055,363,0,0,0,0,0,0,128,1,97,0,0,
185,0,0,9,98,0,1028,541,0,0,2,1,0,0,128,1,97,0,0,2,97,0,0,1,97,0,0,
212,0,0,10,0,0,1055,550,0,0,0,0,0,0,128,1,97,0,0,
231,0,0,11,76,0,1028,561,0,0,2,1,0,0,128,1,97,0,0,2,97,0,0,1,97,0,0,
258,0,0,12,0,0,1055,570,0,0,0,0,0,0,128,1,97,0,0,
277,0,0,13,134,0,1028,683,0,0,4,2,0,0,128,1,97,0,0,1,97,0,0,2,97,0,0,2,97,0,0,
1,97,0,0,
312,0,0,14,0,0,1055,694,0,0,0,0,0,0,128,1,97,0,0,
331,0,0,15,105,0,1028,869,0,0,3,2,0,0,128,1,97,0,0,1,4,0,0,2,97,0,0,1,97,0,0,
362,0,0,16,105,0,1028,876,0,0,3,2,0,0,128,1,97,0,0,1,4,0,0,2,97,0,0,1,97,0,0,
393,0,0,17,0,0,1055,888,0,0,0,0,0,0,128,1,97,0,0,
412,0,0,18,105,0,1028,929,0,0,3,2,0,0,128,1,97,0,0,1,97,0,0,2,4,0,0,1,97,0,0,
443,0,0,19,169,0,1028,1035,0,0,4,3,0,0,128,2,3,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1,97,0,0,
478,0,0,20,0,0,1055,1046,0,0,0,0,0,0,128,1,97,0,0,
497,0,0,21,0,0,1053,1050,0,0,0,0,0,0,128,1,97,0,0,
516,0,0,22,0,0,1048,1091,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
539,0,0,23,0,0,1055,1097,0,0,0,0,0,0,128,1,97,0,0,
558,0,0,24,0,0,1053,1101,0,0,0,0,0,0,128,1,97,0,0,
577,0,0,25,0,0,24,1141,0,0,1,1,0,1,0,1,97,0,0,
596,0,0,26,0,0,31,1147,0,0,0,0,0,1,0,
611,0,0,27,0,0,29,1151,0,0,0,0,0,1,0,
626,0,0,28,105,0,1028,1199,0,0,3,2,0,0,128,1,97,0,0,1,97,0,0,2,4,0,0,1,97,0,0,
657,0,0,29,0,0,31,1208,0,0,0,0,0,1,0,
672,0,0,30,0,0,1053,1215,0,0,0,0,0,0,128,1,97,0,0,
};


/*****************************************************************************
* DESCRIPTION
* This file contains functions which perform various Ingres date operations.
*
* AUTHOR - EDITORS
* Carol Marra
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <defines.h> 
#include <typedefs.h>
#include <dbutils.h>
#include <utils.h>

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
/* EXEC SQL INCLUDE sqlda;
 */ 
/*
 * $Header$ sqlda.h 
 */

/***************************************************************
*      The SQLDA descriptor definition                         *
*--------------------------------------------------------------*
*      VAX/3B Version                                          *
*                                                              *
* Copyright (c) 1987, 2002, Oracle Corporation.  All rights reserved.  *
***************************************************************/


/* NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************
*/

/*  MODIFIED
    apopat     06/07/02  - [2362423] MVS PE to make lines shorter than 79
    apopat     07/31/99 -  [707588] TAB to blanks for OCCS
    lvbcheng   10/27/98 -  change long to int for sqlda
    lvbcheng   08/15/97 -  Move sqlda protos to sqlcpr.h
    lvbcheng   06/25/97 -  Move sqlda protos to this file
    jbasu      01/29/95 -  correct typo
    jbasu      01/27/95 -  correct comment - ub2->sb2
    jbasu      12/12/94 - Bug 217878: note this is an SOSD file
    Morse      12/01/87 - undef L and S for v6 include files
    Richey     07/13/87 - change int defs to long 
    Clare      09/13/84 - Port: Ch types to match SQLLIB structs
    Clare      10/02/86 - Add ifndef SQLDA
*/

#ifndef SQLDA_
#define SQLDA_ 1
 
#ifdef T
# undef T
#endif
#ifdef F
# undef F
#endif

#ifdef S
# undef S
#endif
#ifdef L
# undef L
#endif
 
struct SQLDA {
  /* ub4    */ int        N; /* Descriptor size in number of entries        */
  /* text** */ char     **V; /* Ptr to Arr of addresses of main variables   */
  /* ub4*   */ int       *L; /* Ptr to Arr of lengths of buffers            */
  /* sb2*   */ short     *T; /* Ptr to Arr of types of buffers              */
  /* sb2**  */ short    **I; /* Ptr to Arr of addresses of indicator vars   */
  /* sb4    */ int        F; /* Number of variables found by DESCRIBE       */
  /* text** */ char     **S; /* Ptr to Arr of variable name pointers        */
  /* ub2*   */ short     *M; /* Ptr to Arr of max lengths of var. names     */
  /* ub2*   */ short     *C; /* Ptr to Arr of current lengths of var. names */
  /* text** */ char     **X; /* Ptr to Arr of ind. var. name pointers       */
  /* ub2*   */ short     *Y; /* Ptr to Arr of max lengths of ind. var. names*/
  /* ub2*   */ short     *Z; /* Ptr to Arr of cur lengths of ind. var. names*/
  };
 
typedef struct SQLDA SQLDA;
 
#endif

/* ----------------- */
/* defines for sqlda */
/* ----------------- */

#define SQLSQLDAAlloc(arg1, arg2, arg3, arg4) sqlaldt(arg1, arg2, arg3, arg4) 

#define SQLSQLDAFree(arg1, arg2) sqlclut(arg1, arg2) 





/*****************************************************************************/
#define FUNC_NAME	"SqlDateTrunc"

/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlDateTrunc

<P> <STRONG>Function signature:</STRONG> int SqlDateTrunc (SQL_DATE , char *, SQL_DATE)

<P> <STRONG>Function description:</STRONG> Truncate a date to the specified interval, e.g., 15-JAN-1998 14:58:14 truncated to month is 01-JAN-1998 00:00:00.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE date:      The date to be truncated (dd-mon-yyyy hh24:mi:ss)
<BR><DD> char *datePart:     The interval to which to truncate; e.g., hh24, month.
<BR><DD> SQL_DATE dateTrunc: The truncated date (dd-mon-yyyy hh24:mi:ss).
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlDateTrunc (SQL_DATE date, char * datePart, SQL_DATE dateTrunc)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   SQL_DATE sqlDateTrunc;
   SQL_DATE sqlDate;
   SQL_DATE sqlDatePart;
   /* EXEC SQL END DECLARE SECTION; */ 

   
   int result;
   
   strcpy(sqlDatePart, datePart);
   strcpy(sqlDate, date);

   /* Can't truncate to seconds, so handle this case specially. */
   if (!strcmp (datePart, SECOND))
   {
      strcpy (dateTrunc, sqlDate);
      return (OK);
   }

   /* EXEC SQL AT :current_dbsite SELECT 
      TO_CHAR(TRUNC(TO_DATE(:sqlDate, 'dd-mon-yyyy hh24:mi:ss'), :sqlDatePart),
	'dd-mon-yyyy hh24:mi:ss')
      INTO :sqlDateTrunc
      FROM DUAL; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select TO_CHAR(TRUNC(TO_DATE(:b1,'dd-mon-yyyy hh24:mi:ss')\
,:b2),'dd-mon-yyyy hh24:mi:ss') into :b3  from DUAL ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)sqlDate;
   sqlstm.sqhstl[0] = (unsigned long )21;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)sqlDatePart;
   sqlstm.sqhstl[1] = (unsigned long )21;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)sqlDateTrunc;
   sqlstm.sqhstl[2] = (unsigned long )21;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)current_dbsite;
   sqlstm.sqhstl[3] = (unsigned long )10;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
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
	(FUNC_NAME, FALSE,
	 "Error truncating %s to %s.\n\tExiting.", date, datePart)) == ERROR)
   {		
      /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )36;
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
  
#ifdef DEBUG
  fprintf (stdout, "%s\n", select_statement);
#endif 
  
  strcpy (dateTrunc, sqlDateTrunc);
  
  return (OK);
}

#define FUNC_NAME	"SqlGetDate"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlGetDate 

<P> <STRONG>Function signature:</STRONG> int SqlGetDate (SQL_DATE)

<P> <STRONG>Function description:</STRONG> Get the current date.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE date: The current date, retrieved (dd-mon-yyyy hh24:mi:ss)
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlGetDate (SQL_DATE date)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   SQL_DATE sqlCurDate;
   /* EXEC SQL END DECLARE SECTION; */ 

   
   int result;
   
   /* EXEC SQL AT :current_dbsite SELECT TO_CHAR(SYSDATE, 'dd-mon-yyyy')
      INTO :sqlCurDate
      FROM DUAL; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select TO_CHAR(SYSDATE,'dd-mon-yyyy') into :b1  from DUAL ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )55;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)sqlCurDate;
   sqlstm.sqhstl[0] = (unsigned long )21;
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


   
  if ((result = SqlErrorHandler 
       (FUNC_NAME, FALSE,
	"Problem getting current date/time.\n\tExiting.")) == ERROR)
    {	
      /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )78;
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

  sqlCurDate[11] = NULL;
  sprintf (date, "%s %s", sqlCurDate, "00:00:00");

  return (OK);
}

/*****************************************************************************/
#define FUNC_NAME	"SqlGetDateTime"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlGetDateTime

<P> <STRONG>Function signature:</STRONG> int SqlGetDateTime (SQL_DATE)

<P> <STRONG>Function description:</STRONG> Get the current date and time.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE date: The current date and time, retrieved (dd-mon-yyyy hh24:mi:ss).
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlGetDateTime (SQL_DATE date)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   SQL_DATE sqlCurDate;
   /* EXEC SQL END DECLARE SECTION; */ 

   
   int result;
   
   /* EXEC SQL AT :current_dbsite SELECT TO_CHAR(SYSDATE, 'dd-mon-yyyy hh24:mi:ss')
      INTO :sqlCurDate
      FROM DUAL; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select TO_CHAR(SYSDATE,'dd-mon-yyyy hh24:mi:ss') into :b1 \
 from DUAL ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )97;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)sqlCurDate;
   sqlstm.sqhstl[0] = (unsigned long )21;
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


   
   if ((result = SqlErrorHandler 
	(FUNC_NAME, FALSE,
	 "Problem getting current date/time.\n\tExiting.")) == ERROR)
   {	
      /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )120;
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

  strcpy(date, sqlCurDate);
  
  return (OK); 
}

/*************************************************************************/

#define FUNC_NAME "SqlFormatDate"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlFormatDate

<P> <STRONG>Function signature:</STRONG> int SqlFormatDate (char [300], SQL_DATE)

<P> <STRONG>Function description:</STRONG> Format the supplied data into the format used by all other date functions (dd-mon-yyyy hh24:mi:ss). 

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char date[300]:         The date to be formatted, in day-month-year OR month/day/year format.
<BR><DD> SQL_DATE formattedDate: The formatted version of the date DD-MON-YYYY HH24:MI:SS.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlFormatDate(char date[300], SQL_DATE formattedDate)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   SQL_DATE sqlCharDate;
   SQL_DATE sqlDate;
   /* EXEC SQL END DECLARE SECTION; */ 


   int i;
   int result;
   int restLen;
   int monthLen;   
   char zeros[5];
   
   char *day;
   char tempDay[25];
   char *month;
   char tempMonth[25];
   char *year;
   char tempYear[25];
   char *hour;
   char tempHour[25];
   char *minute;
   char tempMinute[25];
   char *second;
   char tempSecond[25];
   char *rest;
   char *lastRest;

   strcpy(zeros, "00");
 	  
   strcpy(sqlDate, date);
   sqlDate[20] = NULL;

   /* Parse date string based on delimiter: day-month-year OR month/day/year */
   if (!strstr (sqlDate, "-"))
   {
      month = strtok(sqlDate, "/");
      day = strtok (NULL, "/");
      rest   = strtok(NULL, "/");
   }
   else
   {
      day = strtok(sqlDate, "-");
      month  = strtok(NULL, "-");
      rest   = strtok(NULL, "-");
   }
   restLen = strlen(rest);
   if(restLen < 5)
   {
      if (!strstr (rest, "-"))
	 year = strtok (rest, "/");
      else
	 year   = strtok(rest, "-");
      hour   = zeros;
      minute = zeros;
      second = zeros;
   }
   else
   {
      year   = strtok(rest, " ");
      hour   = strtok(NULL, ":");
      lastRest = strtok(NULL, " ");
      if(strlen(lastRest) > 2)
      {
	 minute = strtok(lastRest, ":");
	 second = strtok(NULL, ":");
      }
      else
      {
	 minute = strtok(lastRest, ":");
	 second = zeros;
      }
   }
   
   if(strlen(day) < 2)
   {
      sprintf(tempDay, "0%s", day);
      day = tempDay;
   }
   else
   {
      strcpy(tempDay, day);
      tempDay[3] = '\0';
      day = tempDay;  
   }
   
   monthLen = strlen(month); 
   if(monthLen < 2)
   {
      sprintf(tempMonth, "0%s", month);
      month = tempMonth;
      for(i = 0; month[i] != NULL; i++)
	 month[i] = toupper(month[i]);
   } 
   else
   {
      strcpy(tempMonth, month);
      tempMonth[3] = '\0';
      month = tempMonth;
   }
   
   if(strlen(year) < 4)
   {
      PrintError ("Year must be 4 digits!\n\tExiting.");
      return (ERROR);
   }
   
   else
   {
      strcpy(tempYear, year);
      tempYear[4] = '\0';
      year = tempYear;
   }
   
   if(strlen(hour) < 2)
   {
      sprintf(tempHour, "0%s", hour);
      hour = tempHour;
   }
   else
   {
      strcpy(tempHour, hour);
      tempHour[3] = '\0';
      hour = tempHour;
   }
   if(strlen(minute) < 2)
   {
      sprintf(tempMinute, "0%s", minute);
      minute = tempMinute;
   } 
   else
   {
      strcpy(tempMinute, minute);
      tempMinute[4] = '\0';
      minute = tempMinute;
      }
   
   if(strlen(second) < 2)
   {
      sprintf(tempSecond, "0%s", second);
      second = tempSecond;
   }
   else
   {
      strcpy(tempSecond, second);
      tempSecond[4] = '\0';
      second = tempSecond;
   }
   
   if(monthLen < 3)
   {
      sprintf(sqlDate,"%s-%s-%s %s:%s:%s", 
	      day,month,year,hour,minute,second);
      /* EXEC SQL AT :current_dbsite 
	SELECT TO_CHAR(TO_DATE(:sqlDate, 'DD-MM-YYYY HH24:MI:SS'),
			      'DD-MON-YYYY HH24:MI:SS')
	 INTO :sqlCharDate
	 FROM DUAL; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "select TO_CHAR(TO_DATE(:b1,'DD-MM-YYYY HH24:MI:SS'),'DD\
-MON-YYYY HH24:MI:SS') into :b2  from DUAL ";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )139;
      sqlstm.selerr = (unsigned short)1;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (unsigned char  *)sqlDate;
      sqlstm.sqhstl[0] = (unsigned long )21;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         short *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned long )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (unsigned char  *)sqlCharDate;
      sqlstm.sqhstl[1] = (unsigned long )21;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         short *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned long )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (unsigned char  *)current_dbsite;
      sqlstm.sqhstl[2] = (unsigned long )10;
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         short *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned long )0;
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
}

 
      if ((result = SqlErrorHandler 
	   (FUNC_NAME, FALSE,
	    "Problem converting date/time format.\n.")) == ERROR)
      {	
	 /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )166;
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
   }
   else
   {
     sprintf(sqlDate,"%s-%s-%s %s:%s:%s", 
	      day,month,year,hour,minute,second); 
     strcpy(sqlCharDate, sqlDate);
   }

   strcpy(formattedDate, sqlCharDate);
   return(OK);
}

/*****************************************************************************/

#define FUNC_NAME	"SqlExtractDatePart"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlExtractDatePart

<P> <STRONG>Function signature:</STRONG> int SqlExtractDatePart (SQL_DATE, char *, int *);

<P> <STRONG>Function description:</STRONG> Extract and return the integer representation of the specified part (e.g., day, month, year) of the date passed in.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE date:  Date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> char *datePart: The part (e.g., day, hh24) to be extracted.
<BR><DD> int intPart:    Integer representation of the date part, retrieved.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlExtractDatePart (SQL_DATE date, char *datePart, int *intPart)  
{ 
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   SQL_DATE sqlCharDate,
            sqlDate;
   /* EXEC SQL END DECLARE SECTION; */ 

  
   int result;
   int tempInt;
   int restLen;
   int monthLen;
   char zeros[5];

   char *day;
   char tempDay[25];
   char *month;
   char tempMonth[25];
   char *year;
   char tempYear[25];
   char *hour;
   char tempHour[25];
   char *minute;
   char tempMinute[25];
   char *second;
   char tempSecond[25];
   char *rest;

   strcpy(zeros, "00\0");
 	  
   strcpy(sqlDate, date);
   sqlDate[20] = '\0';
#ifdef DEBUG
   fprintf (stdout, "%s\n", select_statement);
#endif 

   day = strtok(sqlDate, "-");
   month  = strtok(NULL, "-");
   rest   = strtok(NULL, "-");
   restLen = strlen(rest);
   if(restLen < 5)
   {
      year   = strtok(rest, "-");
      hour   = NULL;
      minute = NULL;
      second = NULL;
   }
   else
   {
      year   = strtok(rest, " ");
      hour   = strtok(NULL, ":");
      rest   = strtok(NULL, ":");
      if(strlen(rest) > 2)
      {
	 minute = strtok(rest, ":");
	 second = strtok(NULL, ":");
      }
      else
      {
	 minute = strtok(rest, ":");
	 second = zeros;
      }
   }
   
   if(strlen(day) < 2)
   {
      sprintf(tempDay, "0%s", day);
      day = tempDay;
   }
   else
   {
      strcpy(tempDay, day);
      tempDay[3] = '\0';
      day = tempDay;  
   }
   
   monthLen = strlen(month); 
   if(monthLen < 2)
   {
      sprintf(tempMonth, "0%s", month);
      month = tempMonth;
   } 
   else
   {
      strcpy(tempMonth, month);
      tempMonth[4] = '\0';
      month = tempMonth;
   }
   
   if(strlen(year) < 4)
   {
      PrintError ("%s: Year must be 4 digits.\n\tExiting.\n", FUNC_NAME);
      return (ERROR);
   }
   
   else
   {
      strcpy(tempYear, year);
      tempYear[4] = '\0';
      year = tempYear;
   }
   if(restLen > 5)
   {
      if(strlen(hour) < 2)
      {
	 sprintf(tempHour, "0%s", hour);
	 hour = tempHour;
      }
      else
      {
	 strcpy(tempHour, hour);
	 tempHour[3] = '\0';
	 hour = tempHour;
      }
      if(strlen(minute) < 2)
      {
	 sprintf(tempMinute, "0%s", minute);
	 minute = tempMinute;
      } 
      else
      {
	 strcpy(tempMinute, minute);
	 tempMinute[4] = '\0';
	 minute = tempMinute;
      }
      
      if(strlen(second) < 2)
      {
	 sprintf(tempSecond, "0%s", second);
	 second = tempSecond;
      }
      else
      {
	 strcpy(tempSecond, second);
	 tempSecond[4] = '\0';
	 second = tempSecond;
      }

      if(monthLen > 2)
      {
	 sprintf(sqlDate,"%s-%s-%s %s:%s:%s", 
		 day,month,year,hour,minute,second);
	 /* EXEC SQL AT :current_dbsite 
	    SELECT TO_CHAR(TO_DATE(:sqlDate, 'DD-MON-YYYY HH24:MI:SS'),
			   'DD-MM-YYYY HH24:MI:SS')
	    INTO :sqlCharDate
	    FROM DUAL; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select TO_CHAR(TO_DATE(:b1,'DD-MON-YYYY HH24:MI:SS'),'DD-MM\
-YYYY HH24:MI:SS') into :b2  from DUAL ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )185;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)sqlDate;
  sqlstm.sqhstl[0] = (unsigned long )21;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)sqlCharDate;
  sqlstm.sqhstl[1] = (unsigned long )21;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)current_dbsite;
  sqlstm.sqhstl[2] = (unsigned long )10;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
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
}

 
	 if ((result = SqlErrorHandler 
	      (FUNC_NAME, FALSE,
	       "Problem converting date/time format.\n.")) == ERROR)
	 {	
	    /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 12;
     sqlstm.arrsiz = 4;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )212;
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
      }
   }
   
   else
   {
      if(monthLen > 2)
      {
	 sprintf(sqlDate,"%s-%s-%s", day,month,year);
	 /* EXEC SQL AT :current_dbsite 
	    SELECT TO_CHAR(TO_DATE(:sqlDate, 'DD-MON-YYYY'),
			   'DD-MM-YYYY')
	    INTO :sqlCharDate
	    FROM DUAL; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select TO_CHAR(TO_DATE(:b1,'DD-MON-YYYY'),'DD-MM-YYYY') int\
o :b2  from DUAL ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )231;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)sqlDate;
  sqlstm.sqhstl[0] = (unsigned long )21;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)sqlCharDate;
  sqlstm.sqhstl[1] = (unsigned long )21;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)current_dbsite;
  sqlstm.sqhstl[2] = (unsigned long )10;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
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
}

 
	 if ((result = SqlErrorHandler 
	      (FUNC_NAME, FALSE,
	       "Problem converting date/time format.\n.")) == ERROR)
	 {	
	    /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 12;
     sqlstm.arrsiz = 4;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )258;
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
      }
   }

 sqlDate[20] = NULL;
 
   if(monthLen > 2)
   {
      day = strtok(sqlCharDate, "-");
      month = strtok(NULL, "-");
   }
   
   if(strncmp(datePart, MONTH, strlen(MONTH))==0)
   {
      tempInt = atoi(month);
   }

   else if(strncmp(datePart, DAY, strlen(DAY))==0)
   {
      tempInt = atoi(day);
   }
   
   else if(strncmp(datePart, YEAR, strlen (YEAR))==0)
   {
      tempInt = atoi(year);
   }
  
   else if(strncmp(datePart, HOUR, strlen (HOUR))==0)
   {
      if(hour == NULL)
	 printf("This date/time: %s does not include the hours field\n",
		date);
      else
      {
	 tempInt = atoi(hour);
      }
   }
   
   else if(strncmp(datePart, MINUTE, strlen (MINUTE))==0)
   {
      if(minute == NULL)
	 printf("This date/time: %s does not include the minute field\n",
		date);
      else
      {
	 tempInt = atoi(minute);
      }
   }
   
   else if(strncmp(datePart, SECOND, strlen (SECOND))==0)
   {
      if(second == NULL)
	 printf("This date/time: %s does not include the second field\n",
		date);
      else
      {
	 tempInt = atoi(second);
      }
   }
   *intPart = tempInt;
   return (OK);
}

/****************************************************************************/
#define FUNC_NAME	"SqlNumDaysBetweenDates"
#define NCOLS		1

/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlNumDaysBetweenDates

<P> <STRONG>Function signature:</STRONG> int SqlNumDaysBetweenDates(SQL_DATE, SQL_DATE, int *)

<P> <STRONG>Function description:</STRONG> Get the number of days between 2 dates.

<P><DL> <STRONG>Function arguments:</STRONG> 
<BR><DD> SQL_DATE sDate: Start date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> SQL_DATE eDate: End date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> int *numdays:   Number of days between dates.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlNumDaysBetweenDates(SQL_DATE sDate, SQL_DATE eDate, int *numDays)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char        sqlStartTime[100];
   char        sqlEndTime[100];
   SQL_DATE        sqlStartDate,
                   sqlEndDate;
   /* EXEC SQL END DECLARE SECTION; */ 

   
   int result;
  
   result = SqlFormatDate(sDate, sqlStartDate); 
   if (result == ERROR)
      {
	 PrintError ("Start date could not be formatted.\n");
	 return (ERROR);
      }

   result = SqlFormatDate(eDate, sqlEndDate);
   if (result == ERROR)
      {
	 PrintError ("End date could not be formatted.\n");
	 return (ERROR);
      }

   /* EXEC SQL AT :current_dbsite SELECT 
      TO_CHAR(TO_DATE(:sqlStartDate, 'dd-mon-yyyy HH24:MI:SS'), 'J'), 
      TO_CHAR(TO_DATE(:sqlEndDate, 'dd-mon-yyyy HH24:MI:SS'), 'J')
      INTO :sqlStartTime, :sqlEndTime
      FROM DUAL; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select TO_CHAR(TO_DATE(:b1,'dd-mon-yyyy HH24:MI:SS'),'J') \
,TO_CHAR(TO_DATE(:b2,'dd-mon-yyyy HH24:MI:SS'),'J') into :b3,:b4  from DUAL ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )277;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)sqlStartDate;
   sqlstm.sqhstl[0] = (unsigned long )21;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)sqlEndDate;
   sqlstm.sqhstl[1] = (unsigned long )21;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)sqlStartTime;
   sqlstm.sqhstl[2] = (unsigned long )100;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)sqlEndTime;
   sqlstm.sqhstl[3] = (unsigned long )100;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (unsigned char  *)current_dbsite;
   sqlstm.sqhstl[4] = (unsigned long )10;
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         short *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned long )0;
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
}


   
   if ((result = SqlErrorHandler 
	(FUNC_NAME, FALSE,
	 "Error retrieving number of days between %s and %s.\n\tExiting.",
	 sDate, eDate)) == ERROR)
   {		
      /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )312;
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
   result = atoi(sqlEndTime) - atoi(sqlStartTime) + 1;
   *numDays = result;
   return(OK);
   
}
/****************************************************************************/

#define FUNC_NAME	"SqlDateMath"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlDateMath

<P> <STRONG>Function signature:</STRONG> int SqlDateMath(DATE_MATH_TYPES, SQL_DATE, SQL_DATE, int, char *)

<P> <STRONG>Function description:</STRONG> Add or subtract the specified number of intervals to/from the given date.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> DATE_MATH_TYPES op: Operator (ADDITION or SUBRTRACTION)
<BR><DD> SQL_DATE startDate: Initial date to operate on (dd-mon-yyyy hh24:mi:ss)
<BR><DD> SQL_DATE endDate:   Resulting date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> int numIntervals:   Number of intervals to add/subtract.
<BR><DD> char *interval:     The interval to add (dd, month, hh24, etc).
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlDateMath(DATE_MATH_TYPES op, SQL_DATE startDate, SQL_DATE endDate,
		int numIntervals, char * interval)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

     SQL_DATE      sqlEndDate,
                   sqlStartDate, 
                   sqlTempDate;
     double        sqlTime;
   /* EXEC SQL END DECLARE SECTION; */ 

   
   int result,
       month,
       year;
   char dd[4];
   char ddmon[8];
   char hms[10];

   int days_in_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
   int the_day;

   sqlEndDate[20] = NULL;
  
   strcpy(sqlStartDate, startDate);
   sqlStartDate[20] = NULL; 
  
   if(strcmp(interval, YEAR)==0)
   { 
      /* Save the hour/min/sec portion of date, as add_months blows
	 it away completely. */
      strncpy (ddmon, startDate, 7);
      ddmon[7] = '\0';
      if (SqlExtractDatePart (startDate, YEAR, &year) != OK)
      {
	 PrintError ("Exiting\n");
	 return (ERROR);
      }
	 
      strcpy (hms, strstr (startDate, " "));

      /* Add or subtract appropriate number of years to year, then 
	 reconstruct date. */
      switch (op)
      {
	  case ADDITION: 
	     year += numIntervals;
	     break;
	  case SUBTRACTION:
	     year -= numIntervals;
	     break;
      }

      sprintf (endDate, "%s%d%s", ddmon, year, hms);

      return (OK);
   }

   else if(strcmp(interval, MONTH)==0)
   {
      /* Save all portions of date, as will need to reconstruct
	 it later. */
      strncpy (dd, startDate, 3);
      dd[3] = '\0';
      if (SqlExtractDatePart (startDate, MONTH, &month) != OK)
      {
	 PrintError ("Exiting\n");
	 return (ERROR);
      }

      if (SqlExtractDatePart (startDate, YEAR, &year) != OK)
      {
	 PrintError ("Exiting\n");
	 return (ERROR);
      }
	 
      strcpy (hms, strstr (startDate, " "));

      /* Add or subtract appropriate number of months to month, add years if 
	 necessary, then reconstruct date. */
      switch (op)
      {
	  case ADDITION: 
	     year += (month + numIntervals - 1) / 12;
	     month = (month + numIntervals) % 12;
	     if (month == 0)
		month = 12;
	     break;
	  case SUBTRACTION:
	     if (numIntervals >= month)
   	       year -= ((numIntervals - month) / 12) + 1;

	     month = month - numIntervals;
	     if (month > 0)
		month = month % 12;
	     else if (month < 0)
		month = 12 - (abs(month) % 12);
	     else /* month == 0 */
		month = 12;
	     break;
      }
      /* now adjust the day so that it's not greater than the number
         of days in the month */

      the_day = atoi(dd);
      if (days_in_month[month - 1] < the_day)
      {
        the_day = days_in_month[month - 1];
        /* check for febr. */
        if (month == 2)
        {
          if ( (((year%4) == 0) && ((year%100)!= 0)) || 
             ((year%400)== 0)); /* leapyear */
          the_day = 29;
        }
        sprintf(dd,"%2d-",the_day);
      }

      sprintf (sqlTempDate, "%s%d-%d%s", dd, month, year, hms);
      
      if (SqlFormatDate (sqlTempDate, endDate) != OK)
      {
	 PrintError ("Exiting\n");
	 return (ERROR);
      }

      return (OK);
   }
   else if(strcmp(interval, DAY)==0)
      sqlTime = (int)numIntervals;
   else if(strcmp(interval, HOUR)==0)
      sqlTime = (double) numIntervals / 24;
   else if(strcmp(interval, MINUTE)==0)
      sqlTime = (double) numIntervals / 1440;
   else if(strcmp(interval, SECOND)==0)
      sqlTime = (double) numIntervals / SECONDS_IN_DAY;
   else
   {
      PrintError ("Invalid interval: %s. Valid intervals are YEAR, MONTH, DAY, HOUR, MINUTE, SECOND.\n\tExiting.\n", interval);
      return (ERROR);
   }
     
   switch (op)
   {
       case ADDITION: 
	  /* EXEC SQL AT :current_dbsite SELECT 
	     TO_CHAR(TO_DATE(:sqlStartDate, 'dd-mon-yyyy HH24:MI:SS') 
		     + :sqlTime,'dd-mon-yyyy HH24:MI:SS')
	     INTO :sqlEndDate
	     FROM DUAL; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select TO_CHAR((TO_DATE(:b1,'dd-mon-yyyy HH24:MI:SS')+:b2)\
,'dd-mon-yyyy HH24:MI:SS') into :b3  from DUAL ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )331;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)sqlStartDate;
   sqlstm.sqhstl[0] = (unsigned long )21;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&sqlTime;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(double);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)sqlEndDate;
   sqlstm.sqhstl[2] = (unsigned long )21;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)current_dbsite;
   sqlstm.sqhstl[3] = (unsigned long )10;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
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


	   break;
       case SUBTRACTION:
	  /* EXEC SQL AT :current_dbsite SELECT 
	     TO_CHAR(TO_DATE(:sqlStartDate, 'dd-mon-yyyy HH24:MI:SS') 
		     - :sqlTime,'dd-mon-yyyy HH24:MI:SS')
	     INTO :sqlEndDate
	     FROM DUAL; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select TO_CHAR((TO_DATE(:b1,'dd-mon-yyyy HH24:MI:SS')-:b2)\
,'dd-mon-yyyy HH24:MI:SS') into :b3  from DUAL ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )362;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)sqlStartDate;
   sqlstm.sqhstl[0] = (unsigned long )21;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)&sqlTime;
   sqlstm.sqhstl[1] = (unsigned long )sizeof(double);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)sqlEndDate;
   sqlstm.sqhstl[2] = (unsigned long )21;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)current_dbsite;
   sqlstm.sqhstl[3] = (unsigned long )10;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
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


	  break;
   }
   
   if ((result = SqlErrorHandler (FUNC_NAME, FALSE, 
				  "Error doing date math on date: %s.",
				  startDate)) != OK)
   {
      /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )393;
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
   strcpy(endDate, sqlEndDate);
   
   return (OK);
}

/*****************************************************************************/
#define FUNC_NAME	"SqlDateCompare"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlDateCompare

<P> <STRONG>Function signature:</STRONG> int SqlDateCompare(SQL_DATE, SQL_DATE, int *)

<P> <STRONG>Function description:</STRONG> Takes 2 dates and compares them, to the second if necessary.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE firstDate:  One of the two dates to be compared (dd-mon-yyyy hh24:mi:ss).
<BR><DD> SQL_DATE secondDate:  The other date to be compared (dd-mon-yyyy hh24:mi:ss).
<BR><DD> int * compareResult: The result of the comparison: 1 if firstDate is greater, -1 if secondDate is greater, 0 if equal.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlDateCompare(SQL_DATE firstDate, SQL_DATE secondDate, int *compareResult)

{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    double return_val;
    SQL_DATE date1,
             date2;
  /* EXEC SQL END DECLARE SECTION; */ 


  sprintf (date1, firstDate);
  sprintf (date2, secondDate);

  /* EXEC SQL AT :current_dbsite SELECT to_date(:date1, 'dd-mon-yyyy hh24:mi:ss') 
     - to_date(:date2, 'dd-mon-yyyy hh24:mi:ss') INTO :return_val
     FROM dual; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select (to_date(:b1,'dd-mon-yyyy hh24:mi:ss')-to_date(:b2,'\
dd-mon-yyyy hh24:mi:ss')) into :b3  from dual ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )412;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)date1;
  sqlstm.sqhstl[0] = (unsigned long )21;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)date2;
  sqlstm.sqhstl[1] = (unsigned long )21;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)&return_val;
  sqlstm.sqhstl[2] = (unsigned long )sizeof(double);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)current_dbsite;
  sqlstm.sqhstl[3] = (unsigned long )10;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
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



  if (SqlErrorHandler (FUNC_NAME, FALSE, "Problem selecting date difference.")
                       != OK) {
      return(ERROR);
  }

  if (return_val < 0) {
    *compareResult = -1;
  }
  else if (return_val == 0) {
     *compareResult = 0;
  }
  else if (return_val > 0) {
     *compareResult = 1;
  }
  else {
     PrintError("Compare Dates Utterly Failed.\n");
     exit(1);
  }
  
  return (OK);
}


/****************************************************************************/
#define FUNC_NAME	"SqlExtractDatePartWithoutCommit"

/* int SqlExtractDatePartWithoutCommit (SQL_DATE date, char * datePart,
			             int *intPart) */

/* Use SqlExtractDatePart in place of this function. */

/*****************************************************************************/
#define FUNC_NAME	"SqlDateCompareWithoutCommit"

    /* int SqlDateCompareWithoutCommit(SQL_DATE firstDate, SQL_DATE secondDate, int *compareResult) */

    /*  Use SqlDateCompare in place of this function. */

/*****************************************************************************/
#define FUNC_NAME	"SqlDateRangeIncludesFeb"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlDateRangeIncludesFeb

<P> <STRONG>Function signature:</STRONG> int SqlDateRangeIncludesFeb (SQL_DATE, SQL_DATE, int *, int*)

<P> <STRONG>Function description:</STRONG> Determines if the date range includes the entire month of February; for leap year testing.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE dateLow:  Beginning-of-range date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> SQL_DATE dateHigh: End-of-range date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> int *includesFeb:  Indicator (TRUE, FALSE) if range includes February
<BR><DD> int *yearInd:      Indicator of which year includes February (high: 0,low: 1)
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlDateRangeIncludesFeb (SQL_DATE dateLow, SQL_DATE dateHigh,
			     int *includesFeb, int* yearInd)

{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    SQL_DATE sqlDateLow,
             sqlDateHigh,
             feb28;
    int sqlIncludesFeb = 0;
  /* EXEC SQL END DECLARE SECTION; */ 


  int result,
      year,
      month;
  
  sprintf (sqlDateLow, dateLow);
  sprintf (sqlDateHigh, dateHigh);
  /*******************************************************
   Determine which Feb 28 to use (which year). If month of the low
   date is later than February, only chance that Feb is included is
   if high date is in next year and includes Feb... 
	*******************************************************/

  if ((result = SqlExtractDatePart (dateLow, MONTH, &month)) != OK)
    return (ERROR);

  if (month > 2)
    {
      if ((result = SqlExtractDatePart (dateHigh, YEAR, &year)) != OK)
	return (ERROR);
      *yearInd = HIGH;
    }
  else
    {
      if ((result = SqlExtractDatePart (dateLow, YEAR, &year)) != OK)
	return (ERROR);
      *yearInd = LOW;
    }
  
  sprintf (feb28, "28-feb-%d 00:00:00", year);
  
  /* EXEC SQL AT :current_dbsite SELECT DISTINCT 1 
    INTO :sqlIncludesFeb 
    FROM dual
    WHERE TO_DATE (:feb28, 'dd-mon-yyyy hh24:mi:ss') BETWEEN 
	TO_DATE (:sqlDateLow, 'dd-mon-yyyy hh24:mi:ss') AND 
	TO_DATE (:sqlDateHigh, 'dd-mon-yyyy hh24:mi:ss'); */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select distinct 1 into :b1  from dual where TO_DATE(:b2,'dd\
-mon-yyyy hh24:mi:ss') between TO_DATE(:b3,'dd-mon-yyyy hh24:mi:ss') and TO_DA\
TE(:b4,'dd-mon-yyyy hh24:mi:ss')";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )443;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)&sqlIncludesFeb;
  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         short *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned long )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (unsigned char  *)feb28;
  sqlstm.sqhstl[1] = (unsigned long )21;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         short *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned long )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (unsigned char  *)sqlDateLow;
  sqlstm.sqhstl[2] = (unsigned long )21;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         short *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned long )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (unsigned char  *)sqlDateHigh;
  sqlstm.sqhstl[3] = (unsigned long )21;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         short *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned long )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (unsigned char  *)current_dbsite;
  sqlstm.sqhstl[4] = (unsigned long )10;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         short *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned long )0;
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
}


  if ((result = SqlErrorHandler 
       (FUNC_NAME, TRUE,
	"Error checking for February 28 between %s and %s.", 
	sqlDateLow, sqlDateHigh)) == ERROR)
    {		
      /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )478;
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

  /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )497;
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



  *includesFeb = sqlIncludesFeb;

  return (OK);
}


/*****************************************************************************/
#define FUNC_NAME	"SqlSetDateFormatAt"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlSetDateFormatAt

<P> <STRONG>Function signature:</STRONG> int SqlSetDateFormatAt (char *)

<P> <STRONG>Function description:</STRONG> Sets the long date format to be used
throughout the current named session.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *dateFormat:  String indicating the desired format.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlSetDateFormatAt (char *dateFormat)

{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char setStatement[200];
  /* EXEC SQL END DECLARE SECTION; */ 


  int result;

  sprintf (setStatement, "ALTER SESSION SET nls_date_format = '%s'",
	   dateFormat);

  /* EXEC SQL AT :current_dbsite EXECUTE IMMEDIATE :setStatement; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )516;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)setStatement;
  sqlstm.sqhstl[0] = (unsigned long )200;
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



  if ((result = SqlErrorHandler 
       (FUNC_NAME, TRUE,
	"Error setting date format to %s\n", dateFormat)) == ERROR)
    {		
      /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )539;
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

  /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )558;
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



  return (OK);
}

/*****************************************************************************/
#define FUNC_NAME	"SqlSetDateFormat"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlSetDateFormat

<P> <STRONG>Function signature:</STRONG> int SqlSetDateFormat (char *)

<P> <STRONG>Function description:</STRONG> Sets the long date format to be used
throughout the current named session. This is the function that most 
applications will need to call to simplify date handling in their own,
non-library, code.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *dateFormat:  String indicating the desired format.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlSetDateFormat (char *dateFormat)

{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char setStatement[200];
  /* EXEC SQL END DECLARE SECTION; */ 


  int result;

  sprintf (setStatement, "ALTER SESSION SET nls_date_format = '%s'",
	   dateFormat);

  /* EXEC SQL EXECUTE IMMEDIATE :setStatement; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )577;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (unsigned char  *)setStatement;
  sqlstm.sqhstl[0] = (unsigned long )200;
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
       (FUNC_NAME, TRUE,
	"Error setting date format to %s\n", dateFormat)) == ERROR)
    {		
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )596;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
    }

  /* EXEC SQL COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 5;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )611;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



  return (OK);
}

/*****************************************************************************/
#define FUNC_NAME	"SqlDateSubtract"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlDateSubtract

<P> <STRONG>Function signature:</STRONG> int SqlDateSubtract (SQL_DATE,
SQL_DATE, double *)

<P> <STRONG>Function description:</STRONG> Subtracts the second date from the first date and gives a result in days, including decimal portion 

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE firstDate:  Initial date for the subtraction.
<BR><DD> SQL_DATE secondDate:  Second date to be subtacted from the first.
<BR><DD> double * resultDays:  Result in days including decimal portion.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlDateSubtract(SQL_DATE firstDate,
                    SQL_DATE secondDate,
                    double* resultDays)
{
   /* This function subtracts the second date from the first date 
      and gives a result in days, including decimal portion */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      SQL_DATE sqlFirstDate;
      SQL_DATE sqlSecondDate;
      double sqlResult;
   /* EXEC SQL END DECLARE SECTION; */ 


   int result;

   /* Initialize sql variables */
   strcpy (sqlFirstDate, firstDate);
   strcpy (sqlSecondDate, secondDate);

   /* Call to database to do subtraction */
   /* EXEC SQL AT :current_dbsite
      SELECT TO_DATE(:sqlFirstDate, 'DD-MON-YYYY HH24:MI:SS')
         - TO_DATE(:sqlSecondDate, 'DD-MON-YYYY HH24:MI:SS')
      INTO :sqlResult 
      FROM DUAL; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select (TO_DATE(:b1,'DD-MON-YYYY HH24:MI:SS')-TO_DATE(:b2,\
'DD-MON-YYYY HH24:MI:SS')) into :b3  from DUAL ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )626;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)sqlFirstDate;
   sqlstm.sqhstl[0] = (unsigned long )21;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         short *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned long )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (unsigned char  *)sqlSecondDate;
   sqlstm.sqhstl[1] = (unsigned long )21;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         short *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned long )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (unsigned char  *)&sqlResult;
   sqlstm.sqhstl[2] = (unsigned long )sizeof(double);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         short *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned long )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (unsigned char  *)current_dbsite;
   sqlstm.sqhstl[3] = (unsigned long )10;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         short *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned long )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
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



   if ((result = SqlErrorHandler(FUNC_NAME, FALSE,
                 "Problem subtracting dates\n")) != OK)
   {
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )657;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
   }         

   /* Assign return value */
   *resultDays = sqlResult;

   /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )672;
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


  
   return (OK);
}

