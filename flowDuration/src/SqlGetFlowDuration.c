
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
           char  filnam[22];
};
static const struct sqlcxp sqlfpn =
{
    21,
    "SqlGetFlowDuration.pc"
};


static unsigned long sqlctx = 168426395;


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
            void  *sqhstv[2];
   unsigned int   sqhstl[2];
            int   sqhsts[2];
            void  *sqindv[2];
            int   sqinds[2];
   unsigned int   sqharm[2];
   unsigned int   *sqharc[2];
   unsigned short  sqadto[2];
   unsigned short  sqtdso[2];
} sqlstm = {10,2};

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
5,0,0,1,58,0,4,42,0,0,2,1,0,1,0,2,9,0,0,1,3,0,0,
28,0,0,2,70,0,4,55,0,0,2,1,0,1,0,2,9,0,0,1,3,0,0,
51,0,0,3,0,0,17,91,0,0,1,1,0,1,0,1,97,0,0,
70,0,0,3,0,0,45,115,0,0,0,0,0,1,0,
85,0,0,3,0,0,13,130,0,0,1,0,0,1,0,2,4,0,0,
104,0,0,3,0,0,15,138,0,0,0,0,0,1,0,
119,0,0,4,0,0,29,139,0,0,0,0,0,1,0,
};


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "dbutils.h"
#include "typedefs.h"
#include "utils.h"
#include "defines.h"
#include "flowDuration.h"

#define VERSION "1.1"

/* EXEC SQL INCLUDE SQLCA;
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


INC_HEADER header;

#define FUNC_NAME "SqlGetFlowDuration"
int SqlGetFlowDuration (ID siteId, ID datatypeId, SQL_DATE begDate, 
	SQL_DATE endDate, int getReal, ID modelRunId)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

     ID sqSiteId,
	sqDatatypeId;
     /* varchar siteName[65],
	     datatypeName[65]; */ 
struct { unsigned short len; unsigned char arr[65]; } siteName;

struct { unsigned short len; unsigned char arr[65]; } datatypeName;

     char    selectStatement[2000];
     float   values[30000];
   /* EXEC SQL END DECLARE SECTION; */ 


   int numRows,
       rowCount,
       result;
   char whereClause[200];
   char temp[100];

   INCREMENT *current;

   /* Initialize host variables */
   sqSiteId = siteId;
   sqDatatypeId = datatypeId;
   
   /* EXEC SQL SELECT site_name 
      INTO :siteName
	  FROM hdb_site 
      WHERE site_id = :sqSiteId; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 2;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select site_name into :b0  from hdb_site where site_id=:b1";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&siteName;
   sqlstm.sqhstl[0] = (unsigned int  )67;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)&sqSiteId;
   sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
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


   
   if ((result = SqlErrorHandler 
	(FUNC_NAME, FALSE, "Problem selecting site name name.\n")) 
       != OK)
   {
      SqlDisconnect();
      return (ERROR);
   }

   /* EXEC SQL SELECT datatype_name
      INTO :datatypeName
      FROM hdb_datatype 
      WHERE datatype_id = :sqDatatypeId; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 2;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select datatype_name into :b0  from hdb_datatype where dat\
atype_id=:b1";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )28;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&datatypeName;
   sqlstm.sqhstl[0] = (unsigned int  )67;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)&sqDatatypeId;
   sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
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



   if ((result = SqlErrorHandler 
	(FUNC_NAME, FALSE, "Problem selecting datatype name.\n")) 
       != OK)
   {
      SqlDisconnect();
      return (ERROR);
   }

   strncpy (temp, siteName.arr, siteName.len);
   temp[siteName.len] = '\0';
   printf ("     Station - %s.\n", temp);
   strncpy (temp, datatypeName.arr, datatypeName.len);
   temp[datatypeName.len] = '\0';
   printf("               Flow duration for %s daily %s\n\n",
	  getReal ? "observed" : "modeled", temp);

  /* create the select statement */

/* (rc, 4/18/03) Modified for HDB2 schema.
 *   sprintf (selectStatement, "select value from %s a, hdb_site_datatype b where a.site_datatype_id = b.site_datatype_id and b.site_id = %d and b.datatype_id = %d and a.date_day between '%s' and '%s'", getReal ? "r_day" : "m_day", sqSiteId, sqDatatypeId, begDate, endDate);
 */

   sprintf (selectStatement, "select value from %s a, hdb_site_datatype b where a.site_datatype_id = b.site_datatype_id and b.site_id = %d and b.datatype_id = %d and a.start_date_time between to_date('%s','dd-mon-yyyy hh24:mi:ss') and to_date('%s','dd-mon-yyyy hh24:mi:ss')", getReal ? "r_day" : "m_day", sqSiteId, sqDatatypeId, begDate, endDate);

   /* Add model_run_id restriction for modeled data queries */
   if (!getReal)
   {
      sprintf (whereClause, " and a.model_run_id = %d", modelRunId);
      strcat (selectStatement, whereClause);
   }

   /* EXEC SQL PREPARE sel FROM :selectStatement; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 2;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )51;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)selectStatement;
   sqlstm.sqhstl[0] = (unsigned int  )2000;
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


   if ((result = SqlErrorHandler 
	(FUNC_NAME, FALSE, "Problem preparing select statement:\n\t%s\n",
	 selectStatement)) 
       != OK)
   {
      SqlDisconnect();
      return (ERROR);
   }

	/* debugging:
	 * printf("select statement:\n%s\n",selectStatement);
	 */

   /* EXEC SQL DECLARE aa CURSOR FOR sel; */ 

   if ((result = SqlErrorHandler 
	(FUNC_NAME, FALSE, "Problem declaring cursor aa.\n",
	 selectStatement)) 
       != OK)
   {
      SqlDisconnect();
      return (ERROR);
   }

  /* EXEC SQL OPEN aa; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 2;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )70;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   if ((result = SqlErrorHandler 
	(FUNC_NAME, FALSE, "Problem opening cursor aa.\n",
	 selectStatement)) != OK)
   {
      SqlDisconnect();
      return (ERROR);
   }

   /* EXEC SQL WHENEVER NOT FOUND GOTO close_aa; */ 


   rowCount = 0;

   while (1)
    {
      /* EXEC SQL FETCH aa
	INTO :values[rowCount]; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 2;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )85;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&values[rowCount];
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(float);
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
      if (sqlca.sqlcode == 1403) goto close_aa;
}



      numRows = rowCount;
      rowCount++;
    }
  
close_aa:
   /* EXEC SQL CLOSE aa; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 2;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )104;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 2;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )119;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   fprintf(stderr,"The number of rows is %d\n", numRows);

   if ((result = SortValues (values, numRows)) != OK)
   {
      PrintError ("Error sorting values array\n\tExiting");
      return (ERROR);
   }

   if ((result = CalcIncrements (values, numRows))
       != OK)
   {
      PrintError ("Problem determining increments on values\n\tExiting");
      return (ERROR);
   }


   current = header.list;
   while (current)
    {
      printf("     Percent of values over %d cfs is %5.2f percent\n",
	     current->maxVal, current->pctVals * 100);
      current = current->next;
    }

  return (OK);
}

#define FUNC_NAME "SortValues"

int SortValues (float *values, int size)

{

    qsort(values, size, sizeof(float), 
	  (int (*) (int *,int*)) CompareValues);

    return (OK);
}


#define FUNC_NAME "CompareValues"

/* A function to sort in reverse order; descending */
int CompareValues (int *arg1, int *arg2)
{
  /* compare values */
  if (*arg1 < *arg2)
     return(-1);
  else if (*arg1 > *arg2)
     return (1);
  else
     return (0);
}


#define FUNC_NAME "CalcIncrements"

int CalcIncrements (float *values, int numRows)

{
   float max,
         min,
         range,
         increment;
   long  bottomOfRange;

   /* Start figuring appropriate ranges */
   min = values[0];
   max = values[numRows-1];
   range = max - min;

   if (range < 1)
   {
      increment = .01;
      bottomOfRange = floor (min);
   }
   else if (range < 10)
   {
      increment = .1;
      bottomOfRange = floor (min);
   }
   else if (range < 100)
   {
      increment = 1; 
      bottomOfRange = floor (min);
   }			
   else if (range < 1000)
   {
      increment = 10;
      bottomOfRange = floor(min/10) * 10;
   }
   else if (range < 5000)
   {
      increment = 100;
      bottomOfRange = floor(min/100) * 100;
   }
   else 
   {
      increment = 1000;
      bottomOfRange = floor(min/1000) * 1000;
   }  

   /* Now decrement the bottom of range by 1 increment, so that output
      will reflect range in which lowest value occurs */
   bottomOfRange -= increment;

   /* Initial call of routine which creates increments at appropriate 
      percentage levels */
   header.numElements = 0;
   header.list = NULL;
   MakeIncrements (bottomOfRange, 0, numRows, numRows, values, increment, 
		   header.list);
   
   return (OK);
}

#define FUNC_NAME "MakeIncrements"
int MakeIncrements (long bottomOfRange, int minIndex, int maxIndex, 
		    int numVals, float *values, float increment, 
		    INCREMENT *current)
{
   int index,
       prevIndex,
       tempMinIndex;
   int valsInRange = 0;
   float topOfRange,
         delta;
   long tempBottomOfRange;
   INCREMENT *new, *next, *temp, *prev, *firstEl;

   firstEl = current;
   /* for all recursive calls, start w/ valsInRange set to number
      for last completely processed range */
   if (current)
      valsInRange = current->numVals;

   /* Initialize first range */
   topOfRange = bottomOfRange + increment;

   for (index = minIndex, prevIndex = minIndex; index <= maxIndex; )
   {
      /* When index = maxIndex, else will be taken and last range
	 will be completed. */
      if ((values[index] <= topOfRange) && (index < maxIndex))
      {
	 valsInRange++;
	 index++;
      }
      else
      {
	 /* Done w/ this range; now set the range and % of values */

	 /* First of all allocate a new list member and put it in place */
	 if ((new = (INCREMENT *) malloc (sizeof (INCREMENT))) == NULL)
	 {
	    PrintError ("Error malloc'ing new list member.\n\tExiting");
	 }
	 
	 if (current)
	 {
	    if (current->next)
	    {
	       next = current->next;
	       new->next = next;
	       next->prev = new;
	    }
	    else
	    {
	       new->next = NULL;
	    }
	    
	    new->prev = current;
	    current->next = new;
	 }
	 else
	 {
	    new->prev = NULL;
	    new->next = NULL;
	    header.list = new;
	 }

	 new->minIndex = prevIndex;
	 new->minVal = bottomOfRange;
	 new->maxVal = topOfRange;
	 new->pctVals = 1 - (float)valsInRange/(float)numVals;
	 new->numVals = valsInRange;
	 new->increment = increment;

	 prevIndex = index;
	 header.numElements++;
	 bottomOfRange = topOfRange;
	 topOfRange += increment;
	 current = new;

	 /* check for last value condition; increment index to drop out of 
	    loop */
	 if (index == maxIndex)
	    index++;
      }
   }

   /* Check the delta between each pctValue in list, to see if there are
      any gaps that are too large. If so, scale the increment in the
      appropriate range*/

   /* if first time through, set to first list element; otherwise,
      set to first *new* list element. */
   if (minIndex !=0)
      current = firstEl;
   else
      current = header.list;
   next = current->next;
   while (next && next->minIndex != maxIndex)
   {
      delta = current->pctVals - next->pctVals;
      if (delta > .05)
      {
	 /* need to subdivide 'next' increment */
	 temp = next;
	 next = temp->next;
	 current->next = next;
	 next->prev = current;

	 /* Temp points to space to be freed; next now points to the 
	    element after next.... */
	 tempMinIndex = temp->minIndex;
	 tempBottomOfRange = temp->minVal;

	 free (temp);
   
	 MakeIncrements (tempBottomOfRange, tempMinIndex, next->minIndex, 
			 numVals, values, increment/10, current);

	 /* Set pointer back one element, so a comparison is made between
	    last range in just-subdivided increment, and the next increment */
	 next = next->prev;
      }
      
      current = next;
      next = current->next;
   }
   
   return (OK);
}

#define FUNC_NAME "RemoveIncrement"
int RemoveIncrement (INCREMENT *current)
{
   INCREMENT *prev,
      *next,
      *temp;
   
   /* Assign pointers */
   prev = current->prev;
   next = current->next;
   
   next->prev = prev;
   prev->next = next;

   /* Temp points to space to be freed; current now points to the 
      previous element in the list */
   temp = current;
   current = prev;

   free (temp);
   
   return (OK);
}
   
   

