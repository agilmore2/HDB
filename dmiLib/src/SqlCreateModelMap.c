
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
           char  filnam[21];
};
static const struct sqlcxp sqlfpn =
{
    20,
    "SqlCreateModelMap.pc"
};


static unsigned int sqlctx = 84092235;


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
5,0,0,1,0,0,1041,78,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
28,0,0,1,0,0,1039,91,0,0,0,0,0,0,128,1,97,0,0,
47,0,0,1,0,0,1069,95,0,0,0,0,0,0,128,1,97,0,0,
66,0,0,1,0,0,1039,99,0,0,0,0,0,0,128,1,97,0,0,
85,0,0,1,0,0,1037,105,0,0,2,0,0,0,128,2,3,0,0,2,9,0,0,1,97,0,0,
112,0,0,1,0,0,1039,110,0,0,0,0,0,0,128,1,97,0,0,
131,0,0,1,0,0,1039,119,0,0,0,0,0,0,128,1,97,0,0,
150,0,0,2,0,0,1053,120,0,0,0,0,0,0,128,1,97,0,0,
169,0,0,3,0,0,1041,538,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
192,0,0,3,0,0,1039,551,0,0,0,0,0,0,128,1,97,0,0,
211,0,0,3,0,0,1069,555,0,0,0,0,0,0,128,1,97,0,0,
230,0,0,3,0,0,1039,559,0,0,0,0,0,0,128,1,97,0,0,
249,0,0,3,0,0,1037,567,0,0,10,0,0,0,128,2,3,0,0,2,3,0,0,2,97,0,0,2,97,0,0,2,3,
0,0,2,9,0,0,2,3,0,0,2,97,0,0,2,97,0,0,2,9,0,0,1,97,0,0,
308,0,0,3,0,0,1039,575,0,0,0,0,0,0,128,1,97,0,0,
327,0,0,3,0,0,1039,627,0,0,0,0,0,0,128,1,97,0,0,
346,0,0,4,0,0,1053,628,0,0,0,0,0,0,128,1,97,0,0,
};


/****************************************************************************
**
**	IDENTIFICATION SECTION
**	----------------------
**
**	Module Name:	int createModelmap(model_run_map *map, 
**                          int count) 
**
**	File Name:	createModelmap.sc
**
**	Date Written:	Augest, 1996
**
**	Author(s): 	Yue Wu
**
**	Module Description:	
**	routine to find an appropriate model_run_id for each wildcard users
**	  specified in their metaControl file. At this point, the wildcards
**        are stored in structure array "map".
**		
**      Variables Passed In:  
**         model_run_map *map = structure array containing the wildcards and 
**              correponding model_run_id found later.  
**         int count = the number of wildcards 
**
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*  hdb apps include file to define OK and ERROR  */
#include <dbutils.h>
#include <utils.h>
#include <defines.h>
#include <typedefs.h>

/*  hdb dmi stuff */
#include <dmi_utils.h>

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


#define FUNC_NAME "SqlGetModelIdArray"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlGetModelIdArray

<P> <STRONG>Function signature:</STRONG> int SqlGetModelIdArray(model_map[MAPSIZE], int *)

<P> <STRONG>Function description:</STRONG> Selects all model IDs and names from hdb_model and loads them into an array.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> model_map map[MAPSIZE]: Array of model IDs and names; output.
<BR><DD> int *counter:           Number of elements in array; output.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlGetModelIdArray(model_map map[MAPSIZE], int *counter)
{
 /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char      operateStmt[500];
   /* varchar   name[100]; */ 
struct { unsigned short len; unsigned char arr[100]; } name;

   ID       Id;
 /* EXEC SQL END DECLARE SECTION; */ 
 

 int     result;

  sprintf(operateStmt, "SELECT model_id, model_name FROM hdb_model ORDER BY model_id ASC");

  /* Prepare the select statement; and open a cursor */
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
  sqlstm.sqhstl[0] = (unsigned long )500;
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
       (FUNC_NAME, FALSE, "Problem preparing sel.")) != OK)
    {
      return (ERROR);
    }

  /* EXEC SQL WHENEVER NOT FOUND goto close_c1; */ 

  /* EXEC SQL DECLARE c1 CURSOR FOR sel; */ 


  if ((result = SqlErrorHandler
       (FUNC_NAME, FALSE, "Problem declaring cursor c1.")) != OK)
    {
      /* EXEC SQL CLOSE c1; */ 

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

  /* EXEC SQL OPEN c1; */ 

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
       (FUNC_NAME, FALSE, "Problem opening cursor c1.")) != OK)
    {
      /* EXEC SQL CLOSE c1; */ 

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

  for (*counter = 0; ;(*counter)++)
    {
      /* EXEC SQL FETCH c1 INTO :Id,:name; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 3;
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
      sqlstm.sqhstv[0] = (unsigned char  *)&Id;
      sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         short *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned long )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (unsigned char  *)&name;
      sqlstm.sqhstl[1] = (unsigned long )102;
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
      if (sqlca.sqlcode == 1403) goto close_c1;
}


      if ((result = SqlErrorHandler
           (FUNC_NAME, TRUE, 
            "Problem fetching into ref_model on row %d.", *counter)) != OK)
        {
          /* EXEC SQL CLOSE c1; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 12;
          sqlstm.arrsiz = 3;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )112;
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
      strncpy(map[*counter].name, name.arr, name.len);   
      map[*counter].name[name.len] = '\0';
      map[*counter].modelId = Id;
    }

  close_c1:
    /* EXEC SQL CLOSE c1; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 3;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )131;
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
    sqlstm.arrsiz = 3;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )150;
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

#define FUNC_NAME "GetSpecData"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetSpecData

<P> <STRONG>Function signature:</STRONG> int GetSpecData(model_run[MAPSIZE], int *, int *)

<P> <STRONG>Function description:</STRONG> Read specification of model_runs from a file created by TCL.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> model_run spec[MAPSIZE]: Array of model_run specifications. Output.
<BR><DD> int *count:              Number of elements in array; output.
<BR><DD> int *status:             Status of array loading; used by calling function. Output.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int GetSpecData(model_run spec[MAPSIZE], int *count, int *status)
{
   FILE *fptr1;
   int i;
   char tmp[3];
   char s[MAX_STACK_DEPTH];
   
   if((fptr1 = fopen(WRITESPEC, "r")) == NULL)
   {
      printf("DMI: Failure to open WRITESPEC file %s\n", WRITESPEC);
      return (ERROR);
   }
   /*printf("test\n");*/
   if(fgets(tmp, 3, fptr1) != NULL) 
   {
      switch(tmp[0]) 
      {
          case 'Q':
	     *status = QUIT;
	     break;
          case 'N':
	     *status = NOSPECDATA;
	     break;
          case 'Y':
	     *status = SPECDATA;
	     break;
      }
   }
   if (*status == QUIT)
   {
      fclose(fptr1);
      return(ERROR);
   }   
   i = 0;
   while(fgets(s,100,fptr1) != NULL)
   {
      s[strlen(s)-1] = '\0';
      strcpy(spec[i].code,s);
      fgets(s, 100, fptr1);
      s[strlen(s)-1] = '\0';
      spec[i].modelRunId = atoi(s);
      fgets(s, 100, fptr1);
      s[strlen(s)-1] = '\0';
      spec[i].modelId = atoi(s);
      fgets(spec[i].sysDate, 100, fptr1);
      spec[i].sysDate[strlen(spec[i].sysDate)-1] = '\0';
      fgets(spec[i].runDate, 100, fptr1);
      spec[i].runDate[strlen(spec[i].runDate)-1] = '\0';
      fgets(s, 100, fptr1);
      s[strlen(s)-1] = '\0';
      spec[i].prob = atoi(s);
      fgets(spec[i].modelType, 100, fptr1);
      spec[i].modelType[strlen(spec[i].modelType)-1] = '\0';
      fgets(s, 100, fptr1);
      s[strlen(s)-1] = '\0';
      spec[i].userId = atoi(s);
      fgets(spec[i].startDate, 100, fptr1);
      spec[i].startDate[strlen(spec[i].startDate)-1] = '\0';
      fgets(spec[i].endDate, 100, fptr1);
      spec[i].endDate[strlen(spec[i].endDate)-1] = '\0';
      fgets(spec[i].timeStep, 100, fptr1);
      spec[i].timeStep[strlen(spec[i].timeStep)-1] = '\0';
      /*printf("%s\n", spec[i].code);
	printf("%d\n",spec[i].modelId);
	printf("%s\n",spec[i].sysDate);
	printf("%s\n",spec[i].runDate);
	printf("%d\n",spec[i].prob);
	printf("%s\n",spec[i].modelType);
	printf("%d\n",spec[i].userId);
	printf("%s\n",spec[i].startDate);
	printf("%s\n",spec[i].endDate);
	printf("%d, %s\n", i,s);*/
      i++;
   }
   fclose(fptr1);
   /*printf("TEST3\n");*/
   *count = i;
   return (OK);
} 

#define FUNC_NAME "WriteToFiles"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> WriteToFiles

<P> <STRONG>Function signature:</STRONG> int WriteToFiles(model_run[MAPSIZE],int,model_run_map[MAPSIZE],int)

<P> <STRONG>Function description:</STRONG> Write model_run specifications to a file.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> model_run spec[MAPSIZE]:    Array of model_run specifications.
<BR><DD> int *counter:               Number of elements in spec array.
<BR><DD> model_run_map map[MAPSIZE]: Array of model_run_maps (user code mapped onto modelRunId).
<BR><DD> int *count:                 Number of elements in map array.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int WriteToFiles(model_run spec[MAPSIZE],int counter,model_run_map map[MAPSIZE],int count)
{
   int i,j,c,flag,flag1;
   int position[MAX_STACK_DEPTH];
   FILE *fptr1, *fptr2;
   
   /*printf("WriteToFiles: %d\n", counter);*/
   if((fptr1 = fopen(ERROR1,"w")) == NULL)
   {
      printf("DMI: Failure to open ERROR file %s\n",ERROR1);
      return(ERROR);
   }
   
   if((fptr2 = fopen(ERROR2,"w")) == NULL)
   {
      printf("DMI: Failure to open ERROR file %s\n",ERROR2);
      return(ERROR);
   }
   
   flag = 0;
   flag1 =0;
   for (i=0; i < count; i++)
   {
      c = 0;
      for (j=0; j < counter; j++)
      {
	 /*printf("%s, %d\n", spec[j].code, spec[j].modelRunId);*/
	 if (!strcmp(map[i].code, spec[j].code))
	 {
	    position[c] = j;
	    c++;
	 }
      }
      if (c == 1)
      {
	 if (spec[position[c-1]].modelRunId != NA)
	 {
	    map[i].modelRunId = spec[position[c-1]].modelRunId;
	 }
	 else
	 {
	    if (flag1 == 0)
	    {
	       fprintf(fptr2, "%s", map[i].code);
	       flag1 = 1;
	    }
	    else {
               fprintf(fptr2, "\n%s", map[i].code);
	    }
	 }
      }
      else
      {
	 for (j=0; j < c; j++)
	 {
	    if (flag == 0) 
	    {
	       fprintf(fptr1, "%s", spec[position[j]].code);
	       flag = 1;
	    }
	    else
	    {
	       fprintf(fptr1, "\n%s", spec[position[j]].code);
	    }
	    fprintf(fptr1, "\n%d", spec[position[j]].modelRunId);
	    if(spec[position[j]].modelId == NA)
	    {
	       fprintf(fptr1, "\n  ");
	    }
	    else
	    {
	       fprintf(fptr1, "\n%d", spec[position[j]].modelId);
	    }
	    fprintf(fptr1, "\n%s", spec[position[j]].sysDate);
	    fprintf(fptr1, "\n%s", spec[position[j]].runDate);
	    if(spec[position[j]].prob == NA)
	    {
	       fprintf(fptr1, "\n  ");
	    }
	    else
	    {
	       fprintf(fptr1, "\n%d", spec[position[j]].prob);
	    }
	    fprintf(fptr1, "\n%s", spec[position[j]].modelType);
	    if(spec[position[j]].userId == NA)
	    {
	       fprintf(fptr1, "\n  ");
	    }
	    else
	    {
	       fprintf(fptr1, "\n%d", spec[position[j]].userId);
	    }
	    fprintf(fptr1, "\n%s", spec[position[j]].startDate);
	    fprintf(fptr1, "\n%s", spec[position[j]].endDate);
	    fprintf(fptr1, "\n%s", spec[position[j]].timeStep);
	 }
      }
   }
   fclose(fptr1);
   fclose(fptr2);
   return (OK);
}

#define FUNC_NAME "ReadTkFile"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> ReadTkFile

<P> <STRONG>Function signature:</STRONG> int ReadTkFile(model_run_map, int)

<P> <STRONG>Function description:</STRONG> Reads model_run codes and IDs from TK file and loads array.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> model_run_map map[MAPSIZE]: Array of model_run_maps (user code mapped onto modelRunId). Output.
<BR><DD> int count:                  Number of map elements in file.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int ReadTkFile(model_run_map map[MAPSIZE], int count)
{
  FILE *fptr1;
  char s[MAX_STACK_DEPTH];
  int i;

  if((fptr1 = fopen(SAVECHOICE,"r")) == NULL)
    {
      return(OK);
    }
  
  while(fgets(s, 100, fptr1) != NULL)
    {
      s[strlen(s)-1] = '\0';
      for (i=0; i < count; i++)
        {
/*printf("%s, %s\n", map[i].code, s);*/
          if (!strcmp(map[i].code, s))
            {
              fgets(s, 100, fptr1);
              map[i].modelRunId = atoi(s);
/*printf("%s, %s, %d\n", map[i].code, s, atoi(s));*/
              strcpy(s,"\0");
            }
        }
    }
  fclose(fptr1);
  return (OK);
}

#define FUNC_NAME "SqlSearchModelRunIdInput"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlSearchModelRunIdOutput

<P> <STRONG>Function signature:</STRONG> int SqlSearchModelRunIdInput(model_run[MAPSIZE], model_run[MAPSIZE], int *)

<P> <STRONG>Function description:</STRONG> Returns all information about the model_run_ids which match the indicated spec, by querying HDB and loading the results into the array search_results.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> model_run search_results[MAPSIZE]: Array of model_run_ids which match the spec, and their attributes. Output.
<BR><DD> model_run spec[MAPSIZE]: Specification for desired model_run_id(s).
<BR><DD> int *n_of_records_found: Number of model_run_ids matching the spec. Output.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlSearchModelRunIdInput(model_run search_results[MAPSIZE], 
			     model_run spec[MAPSIZE], int *n_of_records_found)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char      operateStmt[1000];
   /* varchar   modelType[6],
             timeStep[20]; */ 
struct { unsigned short len; unsigned char arr[6]; } modelType;

struct { unsigned short len; unsigned char arr[20]; } timeStep;

   SQL_DATE  sysDate,
      runDate,
      startDate,
      endDate;
   ID        runId,
      modelId,
      prob,
      userId;
   short int mt_null,
      syd_null,
      rd_null,
      ts_null,
      sd_null,
      ed_null,
      md_null,
      pr_null,
      ui_null;
   /* EXEC SQL END DECLARE SECTION; */ 
 
   
   int     result;
   int     i, j, flag, limit;
   char    line[50];
   
   /*   j = *n_of_records_found;*/
   j = 0;
   limit = modelRunMapCount;

   for(i = 0; i < limit; i++)
   {
      /*create the select statement
       NOTE:  we put on a WHERE at the end; will NULL it out later if not
       needed */
      sprintf(operateStmt, "SELECT model_run_id, model_id, to_char(sys_date, 'dd-mon-yyyy hh24:mi:ss'), to_char(run_date, 'dd-mon-yyyy'), probability, modeltype, user_id, to_char(start_date, 'dd-mon-yyyy'), to_char(end_date, 'dd-mon-yyyy'), time_step_descriptor FROM ref_model_run WHERE ");
      flag = 0;

      /*If specification of model_id*/
      if (spec[i].modelRunId != 0)
      {
	 sprintf(line, "model_run_id = %d and ", spec[i].modelRunId);
	 strcat(operateStmt, line);
	 spec[i].modelRunId = NA;
	 flag = 1;
      }      
      if (spec[i].modelId != 0) 
      {
	 sprintf(line, "model_id = %d and ", spec[i].modelId);
	 strcat(operateStmt, line);
	 flag = 1;
      }      
      /* ??? */
      if (strcmp(spec[i].sysDate, "\0"))
      {
	 sprintf(line, "to_char (sys_date, 'dd-mon-yyyy') = '%s' and ", 
		 spec[i].sysDate);
	 strcat(operateStmt, line);
	 flag = 1;
      }
      if (strcmp(spec[i].runDate, "\0"))
      {
	 sprintf(line, "to_char (run_date, 'dd-mon-yyyy') = '%s' and ", 
		 spec[i].runDate);
	 strcat(operateStmt, line);
	 flag = 1;
      }
      if (spec[i].prob != NA)
      {
	 sprintf(line, "probability = %d and ", spec[i].prob);
	 strcat(operateStmt, line);
	 flag = 1;
      }
      if (strcmp(spec[i].modelType, "\0"))
      {
	 sprintf(line, "modeltype = '%s' and ", spec[i].modelType);
	 strcat(operateStmt, line);
	 flag = 1;
      }
      if (spec[i].userId != NA)
      {
	 sprintf(line, "user_id = %d and ", spec[i].userId);
	 strcat(operateStmt, line);
	 flag = 1;
      }
      /* ??? */
      if (strcmp(spec[i].startDate, "\0"))
      {
	 sprintf(line, "to_char (start_date, 'dd-mon-yyyy') = '%s' and ", 
		 spec[i].startDate);
	 strcat(operateStmt, line);
	 flag = 1;
      }
      /* ??? */
      if (strcmp(spec[i].endDate, "\0"))
      {
	 sprintf(line, "to_char (end_date, 'dd-mon-yyyy') = '%s' and ", 
		 spec[i].endDate);
	 strcat(operateStmt, line);
	 flag = 1;
      }
      
      /*Need to get rid of the last 'and' statement, because there is an 
	extra */
      if(flag == 1)
	 operateStmt[strlen(operateStmt) - 4] = '\0';
      /* Need to get rid of the WHERE at the end of the statement, because 
	 nothing was specified for the search*/
      if(flag == 0)
	 operateStmt[strlen(operateStmt) - 6] = '\0';
      sprintf (line, "ORDER BY model_run_id ASC");
      strcat(operateStmt, line);
      
      /* Prepare the select statement; and open a cursor */
      if (flag) {
	 /* EXEC SQL AT :current_dbsite DECLARE sel2 STATEMENT; */ 

	 /* EXEC SQL PREPARE sel2 FROM :operateStmt; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 3;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )169;
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


	 result = SqlErrorHandler(FUNC_NAME, FALSE, "Problem preparing sel2.");

	 if (result != OK)
	    return (ERROR);
	 
	 /* EXEC SQL WHENEVER NOT FOUND goto close_c1; */ 

	 /* EXEC SQL DECLARE c2 CURSOR FOR sel2; */ 

	 
	 result = SqlErrorHandler(FUNC_NAME, FALSE, 
				  "Problem declaring cursor c2.");
	 if(result != OK)
	 {
            /* EXEC SQL CLOSE c2; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 12;
            sqlstm.arrsiz = 3;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )192;
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
  sqlstm.arrsiz = 3;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )211;
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
            sqlstm.arrsiz = 3;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )230;
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
	 flag = 0;
	 runId = NA;
	 while(TRUE) /*SQL will automatically go to close_c1 when it can't read
		       anymore model_runs  */
	 {
            /* EXEC SQL FETCH c2 INTO :runId,:modelId:md_null,:sysDate:syd_null,
               :runDate:rd_null,
	       :prob:pr_null,:modelType:mt_null,:userId:ui_null,
	       :startDate:sd_null,:endDate:ed_null,:timeStep:ts_null; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 12;
            sqlstm.arrsiz = 11;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )249;
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
            sqlstm.sqhstv[1] = (unsigned char  *)&modelId;
            sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         short *)&md_null;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned long )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (unsigned char  *)sysDate;
            sqlstm.sqhstl[2] = (unsigned long )21;
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         short *)&syd_null;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned long )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (unsigned char  *)runDate;
            sqlstm.sqhstl[3] = (unsigned long )21;
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         short *)&rd_null;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned long )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
            sqlstm.sqhstv[4] = (unsigned char  *)&prob;
            sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
            sqlstm.sqhsts[4] = (         int  )0;
            sqlstm.sqindv[4] = (         short *)&pr_null;
            sqlstm.sqinds[4] = (         int  )0;
            sqlstm.sqharm[4] = (unsigned long )0;
            sqlstm.sqadto[4] = (unsigned short )0;
            sqlstm.sqtdso[4] = (unsigned short )0;
            sqlstm.sqhstv[5] = (unsigned char  *)&modelType;
            sqlstm.sqhstl[5] = (unsigned long )8;
            sqlstm.sqhsts[5] = (         int  )0;
            sqlstm.sqindv[5] = (         short *)&mt_null;
            sqlstm.sqinds[5] = (         int  )0;
            sqlstm.sqharm[5] = (unsigned long )0;
            sqlstm.sqadto[5] = (unsigned short )0;
            sqlstm.sqtdso[5] = (unsigned short )0;
            sqlstm.sqhstv[6] = (unsigned char  *)&userId;
            sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
            sqlstm.sqhsts[6] = (         int  )0;
            sqlstm.sqindv[6] = (         short *)&ui_null;
            sqlstm.sqinds[6] = (         int  )0;
            sqlstm.sqharm[6] = (unsigned long )0;
            sqlstm.sqadto[6] = (unsigned short )0;
            sqlstm.sqtdso[6] = (unsigned short )0;
            sqlstm.sqhstv[7] = (unsigned char  *)startDate;
            sqlstm.sqhstl[7] = (unsigned long )21;
            sqlstm.sqhsts[7] = (         int  )0;
            sqlstm.sqindv[7] = (         short *)&sd_null;
            sqlstm.sqinds[7] = (         int  )0;
            sqlstm.sqharm[7] = (unsigned long )0;
            sqlstm.sqadto[7] = (unsigned short )0;
            sqlstm.sqtdso[7] = (unsigned short )0;
            sqlstm.sqhstv[8] = (unsigned char  *)endDate;
            sqlstm.sqhstl[8] = (unsigned long )21;
            sqlstm.sqhsts[8] = (         int  )0;
            sqlstm.sqindv[8] = (         short *)&ed_null;
            sqlstm.sqinds[8] = (         int  )0;
            sqlstm.sqharm[8] = (unsigned long )0;
            sqlstm.sqadto[8] = (unsigned short )0;
            sqlstm.sqtdso[8] = (unsigned short )0;
            sqlstm.sqhstv[9] = (unsigned char  *)&timeStep;
            sqlstm.sqhstl[9] = (unsigned long )22;
            sqlstm.sqhsts[9] = (         int  )0;
            sqlstm.sqindv[9] = (         short *)&ts_null;
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
        sqlstm.arrsiz = 11;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )308;
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
            strcpy(search_results[j].code, spec[i].code);
            search_results[j].modelRunId = runId;
            if(md_null == -1)	
	       search_results[j].modelId = NA;
            else 
	       search_results[j].modelId = modelId;
            if(syd_null == -1) 
	       strcpy(search_results[j].sysDate, "\0"); 
            else 
	       strcpy(search_results[j].sysDate, sysDate);
            if(rd_null == -1) 
	       strcpy(search_results[j].runDate, "\0"); 
            else 
	       strcpy(search_results[j].runDate, runDate);
            if(pr_null == -1) 
	       search_results[j].prob = NA;
            else 
	       search_results[j].prob = prob;
            if(mt_null == -1) 
	       strcpy(search_results[j].modelType, "\0");
            else 
	    {
	       strncpy(search_results[j].modelType, modelType.arr, 
		       modelType.len);
	       search_results[j].modelType[modelType.len] = '\0';
	    }
            if(ui_null == -1) 
	       search_results[j].userId = NA;
            else 
	       search_results[j].userId = userId;
            if(sd_null == -1) 
	       strcpy(search_results[j].startDate, "\0");
            else 
	       strcpy(search_results[j].startDate, startDate);
            if(ed_null == -1) 
	       strcpy(search_results[j].endDate, "\0");
            else 
	       strcpy(search_results[j].endDate, endDate);
            if(ts_null == -1) 
	       strcpy(search_results[j].timeStep, "\0");
            else 
	       {
		  strncpy(search_results[j].timeStep, timeStep.arr, 
			  timeStep.len);
		  search_results[j].timeStep[timeStep.len] = '\0';
	       }
	    j++;
	 }
      close_c1:
	 /* EXEC SQL CLOSE c2; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 11;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )327;
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
  sqlstm.arrsiz = 11;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )346;
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
      /*      if (search_results[i].modelRunId == 0) 
	      search_results[i].modelRunId = NA;
	      */
   }
   *n_of_records_found = j;
   return (OK);
}

#define FUNC_NAME "rmTextFile"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> rmTextFile

<P> <STRONG>Function signature:</STRONG> void rmTextFile (void)

<P> <STRONG>Function description:</STRONG> Removes .hdb files generated by TK from working directory.

<P><DL> <STRONG>Function arguments:</STRONG>
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
void rmTextFile(void)
{
   printf("Removing the .hdb files from working directory.\n\n");
   if(!remove(READMODEL))
      printf("Cannot remove  %s\n", READMODEL);
   printf(".");
   if(!remove(READCODE))
      printf("Cannot remove  %s\n", READCODE);
   printf(".");
   if(!remove(WRITESPEC))
      printf("Cannot remove  %s\n", WRITESPEC);
   printf(".");
   if(!remove(SAVECHOICE))
      printf("Cannot remove  %s\n", SAVECHOICE);
   printf(".");
   if(!remove(ERROR1))
      printf("Cannot remove  %s\n", ERROR1);
   printf(".");
   if(!remove(ERROR2))
      printf("Cannot remove  %s\n", ERROR2);
   printf(".");
   printf("\n");
}

#define FUNC_NAME "mapList"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> mapList

<P> <STRONG>Function signature:</STRONG> int mapList(dmi_header_struct **)

<P> <STRONG>Function description:</STRONG> Sets model and historical data sources on all elements of the list.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> dmi_header_struct **list: List of object/slots to be processed by DMI.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int mapList(dmi_header_struct **list)
{
   dmi_header_struct *tmp;
   
   tmp = (*list);
   while (tmp != NULL)
   {
      if (tmp->model_source <= 0)
      {
	 if (tmp->model_source == 0) tmp->model_source = modelRunMap[0].modelRunId;
	 else 
	 {
	    tmp->model_source = modelRunMap[0-tmp->model_source].modelRunId;
	 }
      }
      printf("ModelSource: %s, %s, %d\n",tmp->object_name,tmp->slot_name, tmp->model_source);
      
      if (tmp->hist_source < 0)
      {
	 tmp->hist_source = modelRunMap[0-tmp->hist_source].modelRunId;
      }
      if (tmp->hist_source == 0)
      {
	 tmp->hist_source = REAL_DATA_TABLE;
      } 
      printf("HistSource: %s, %s, %d\n",tmp->object_name,tmp->slot_name, tmp->hist_source);
      
      tmp = tmp->next;
   }
   return (OK);
}




