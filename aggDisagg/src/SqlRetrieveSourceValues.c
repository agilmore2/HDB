
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
           char  filnam[27];
};
static const struct sqlcxp sqlfpn =
{
    26,
    "SqlRetrieveSourceValues.pc"
};


static unsigned long sqlctx = 1144992419;


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
            void  *sqhstv[3];
   unsigned int   sqhstl[3];
            int   sqhsts[3];
            void  *sqindv[3];
            int   sqinds[3];
   unsigned int   sqharm[3];
   unsigned int   *sqharc[3];
   unsigned short  sqadto[3];
   unsigned short  sqtdso[3];
} sqlstm = {10,3};

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
5,0,0,1,0,0,17,227,0,0,1,1,0,1,0,1,97,0,0,
24,0,0,1,0,0,45,242,0,0,0,0,0,1,0,
39,0,0,1,0,0,13,253,0,0,3,0,0,1,0,2,97,0,0,2,97,0,0,2,4,0,0,
66,0,0,2,0,0,31,258,0,0,0,0,0,1,0,
81,0,0,1,0,0,15,264,0,0,0,0,0,1,0,
96,0,0,3,0,0,29,265,0,0,0,0,0,1,0,
};


/*****************************************************************************
* DESCRIPTION
*
* This file contains the function which loads specified source data from HDB
* into the sourceValueList.
*
***************************************
*
* RetrieveSourceValues
*
* Purpose: Do preliminary work. Construct appropriate select statements
*          for range and non-range data. Find out how many values will
*          be retrieved by value-select query and allocate sourceValueList
*          accordingly. Call value-select query.
*
* Input
* 	RUN_CONTROL   - pointer to structure holding run control info; 
*       ID            - siteDatatypeId for which to retrieve data
*	SQL_DATE      - low date
*	SQL_DATE      - high date
*       VALUE_LIST ** - sourceValueList
*       int *         - numSourceValues
*
* Output
*
* Return
* 	int - OK or ERROR
*
***************************************
* SqlRetrieveSourceValues
*
* Purpose: load in source data for specified siteDatatypeId/date range.
*          Both range and non-range values are processed by this function.
*          Note that exact date/time stamps of range values are supplied
*          by dateHigh and dateLow, and hence don't need to be selected
*          and stored in the sourceValueList.
*
* Input
*       VALUE_LIST * - sourcValueList
*       char *       - select statement
*       int *        - numSourceValues
*
* Output
*
* Return
* 	int - OK or ERROR
*
***************************************
*
* Author
*
* Carol Marra
* March, 1996
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


#define FUNC_NAME "RetrieveSourceValues"

int RetrieveSourceValues (RUN_CONTROL runControl, ID siteDatatypeId,
			  SQL_DATE dateLow, SQL_DATE dateHigh, 
			  VALUE_RECORD **sourceValueList,
			  int *numSourceValues)

{
  int result,
      valueCount;
  char dateColName[32],
       selectValues[300],
       selectCount[100],
       whereStatement[300],
       selectStatement[600],
       countStatement[400];

  
  /* Set date column based on source interval. */
  if ((result = GetDateColumn (runControl.obsSource, FALSE, 
			       runControl.sourceIsRange, dateColName)) != OK)
    {
      PrintError ("Error in %s....\n", FUNC_NAME);
      return (ERROR);
    }

  /* Construct statements for selecting count of values, and values
     themselves. */
  sprintf (selectCount, "SELECT count (site_datatype_id) FROM %s",
	   runControl.tableSource);
  
  /* For model data: if range, there are no changes for hdb2, because 
     range dates and date columns are the same. If non-range, query
     is based on start_date_time, but this functionally is the same as
     the old date_hour, etc, so again, no change. Use date_low and
     date_high as before. */
  if (!runControl.sourceIsReal)
  {
     if (runControl.sourceIsRange)
     {	
	sprintf (selectValues, "SELECT to_char (start_%s, 'dd-mon-yyyy hh24:mi:ss'), to_char (end_%s, 'dd-mon-yyyy hh24:mi:ss'), value FROM %s",
		 dateColName, dateColName, runControl.tableSource);
	sprintf (whereStatement, "WHERE start_%s = to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') and end_%s = to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND site_datatype_id = %d AND model_run_id = %d", dateColName, dateLow, dateColName, dateHigh, siteDatatypeId, runControl.model_run_id_src);
     }
     else
     {
	sprintf (selectValues, "SELECT to_char (start_%s, 'dd-mon-yyyy hh24:mi:ss'), NULL, value FROM %s",
		 dateColName, runControl.tableSource);
	sprintf (whereStatement, "WHERE start_%s BETWEEN to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND site_datatype_id = %d AND model_run_id = %d", dateColName, dateLow, dateHigh, siteDatatypeId,
		 runControl.model_run_id_src);
     }
  }
  
  /* For real data, if range, there are no changes for hdb2, because 
     range dates and date columns are the same. If non-range, must 
     check to see if dimensionChangeInBaseArea or not... If not, query is 
     based on start_date_time in interval table. This functionally is the 
     same as the old date_hour, etc, so again, no change. Use date_low and
     date_high as before. If it is a dimension change, query is handled 
     totally differently.  */
  else
  {
     if (runControl.sourceIsRange)
     {
	sprintf (selectValues, "SELECT to_char (start_%s, 'dd-mon-yyyy hh24:mi:ss'), to_char (end_%s, 'dd-mon-yyyy hh24:mi:ss'), value FROM %s",
		 dateColName, dateColName, runControl.tableSource);
	sprintf (whereStatement, "WHERE start_%s = to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') and end_%s = to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND site_datatype_id = %d", dateColName, dateLow, dateColName, dateHigh, siteDatatypeId);
     }
     else if (!runControl.dimensionChangeInBaseArea)
     {
	sprintf (selectValues, "SELECT to_char (start_%s, 'dd-mon-yyyy hh24:mi:ss'), NULL, value FROM %s",
		 dateColName, runControl.tableSource);
	sprintf (whereStatement, "WHERE start_%s BETWEEN to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND site_datatype_id = %d", dateColName, dateLow, dateHigh, siteDatatypeId);
     }
     else
     {
	/* this is query for base area; this is the only place
	where end date is retrieved for non-range data. It should
	be used on the corresponding value inserted into r_base */
	sprintf (selectValues, "SELECT to_char (start_%s, 'dd-mon-yyyy hh24:mi:ss'), to_char (end_%s, 'dd-mon-yyyy hh24:mi:ss'), value FROM %s",
		 dateColName, dateColName, runControl.tableSource);
	sprintf (whereStatement, "WHERE start_%s BETWEEN to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND site_datatype_id = %d", dateColName, dateLow, dateHigh, siteDatatypeId);
     }
  }

  sprintf(selectStatement,"%s %s",selectValues,whereStatement);
  sprintf(countStatement,"%s %s",selectCount,whereStatement);
  
  /* Get count of values in sourceValueList; note that it can't 
     be known ahead of time, as an instantaneous source could supply
     any number of values. */
  if ((result = SqlSelectCount (countStatement, &valueCount)) != OK)
  {
     PrintError ("Problem determining number of source values\nfor SDI: %d, dates %s and %s...\n", siteDatatypeId, dateLow, dateHigh);
     return (ERROR);
  }
  
  /* Allocate sourceValueList. If there are already source values in the list, 
     use realloc, because this block of memory will need to be 
     *extended* to handle more values for statistics data. If valueCount
     is 0 for empty sourceValueLists, init the list to NULL. */
  if (!(*numSourceValues))
    {
      if (valueCount)
	{
	  if ((*sourceValueList = (VALUE_RECORD *) malloc 
	       (sizeof(VALUE_RECORD) * valueCount))
	      == NULL)
	    {
	      PrintError ("Problem allocating sourceValueList\n\tfor SDI: %s, dates %s and %s...\n", siteDatatypeId, dateLow, dateHigh);
	      return (ERROR);
	    }
	}
      else
	*sourceValueList = NULL;
    }
      
  else
    {
      if ((*sourceValueList = (VALUE_RECORD *) 
	   realloc (*sourceValueList, 
		    sizeof(VALUE_RECORD) * (valueCount + *numSourceValues)))
	  == NULL)
	{
	  PrintError ("Problem (re)allocating sourceValueList\n\tfor SDI: %s, dates %s and %s...\n", siteDatatypeId, dateLow, dateHigh);
	  return (ERROR);
	}
    }
  
  /* Now select actual values */
  if (valueCount)
    if ((result = SqlRetrieveSourceValues (*sourceValueList, selectStatement,
					   numSourceValues, valueCount)) != OK)
      {
	return (ERROR);
      }

  return (OK);
}


/***************************************************************************/

#define FUNC_NAME "SqlRetrieveSourceValues"

int SqlRetrieveSourceValues (VALUE_RECORD *sourceValueList,
			     char *selectStatement, int *numSourceValues,
			     int valueCount)

{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    VALUE_RECORD *sqlValueRecord;
    char *sqlSelectStatement = selectStatement;
    IND dateInd;
  /* EXEC SQL END DECLARE SECTION; */ 


  int count,
      result;

  /* EXEC SQL PREPARE sel FROM :sqlSelectStatement; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 1;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)sqlSelectStatement;
  sqlstm.sqhstl[0] = (unsigned int  )0;
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


  if ((result = SqlErrorHandler (FUNC_NAME, FALSE, "Problem preparing select statement: %s...\n", selectStatement)) != OK)

    {
      return (ERROR);
    }

  /* EXEC SQL WHENEVER NOT FOUND GOTO close_c1; */ 


  /* EXEC SQL DECLARE c1 CURSOR FOR sel; */ 

  if ((result = SqlErrorHandler (FUNC_NAME, FALSE, "Problem describing cursor for: %s...\n", selectStatement)) != OK)
    {
      return (ERROR);
    }
  
  /* EXEC SQL OPEN c1; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 1;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )24;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


  if ((result = SqlErrorHandler (FUNC_NAME, FALSE, "Problem opening cursor for\n: %s...\n", selectStatement)) != OK)
    {
      return (ERROR);
    }

  /* Select all source data */
  for (count = 0; ; count++)
    {
      sqlValueRecord = &sourceValueList[count + *numSourceValues];

      /* EXEC SQL FETCH c1 INTO :sqlValueRecord->dateStart, 
                             :sqlValueRecord->dateEnd:dateInd,
                             :sqlValueRecord->value; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 3;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )39;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)(sqlValueRecord->dateStart);
      sqlstm.sqhstl[0] = (unsigned int  )21;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)(sqlValueRecord->dateEnd);
      sqlstm.sqhstl[1] = (unsigned int  )21;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)&dateInd;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)&(sqlValueRecord->value);
      sqlstm.sqhstl[2] = (unsigned int  )sizeof(double);
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
      if (sqlca.sqlcode == 1403) goto close_c1;
}


      if ((result = SqlErrorHandler (FUNC_NAME, FALSE, "Problem executing: %s...\n", selectStatement)) != OK)
	{
	  /* EXEC SQL ROLLBACK; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 3;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )66;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	  return (ERROR);
	}
    }

  close_c1:
    /* EXEC SQL CLOSE c1; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 3;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )81;
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
    sqlstm.arrsiz = 3;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )96;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



  if (valueCount != count)
    {
      PrintError ("Number of values selected, %d,\n\tdoes not match number anticipated, %d...\n", count, *numSourceValues);
      return (ERROR);
    }

  *numSourceValues += count;

  return (OK);
}


/***********************************************************************/
