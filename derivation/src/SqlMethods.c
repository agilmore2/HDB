
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
           char  filnam[14];
};
static const struct sqlcxp sqlfpn =
{
    13,
    "SqlMethods.pc"
};


static unsigned long sqlctx = 660819;


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
            void  *sqhstv[5];
   unsigned int   sqhstl[5];
            int   sqhsts[5];
            void  *sqindv[5];
            int   sqinds[5];
   unsigned int   sqharm[5];
   unsigned int   *sqharc[5];
   unsigned short  sqadto[5];
   unsigned short  sqtdso[5];
} sqlstm = {10,5};

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
5,0,0,1,0,0,17,174,0,0,1,1,0,1,0,1,5,0,0,
24,0,0,1,0,0,45,192,0,0,0,0,0,1,0,
39,0,0,1,0,0,13,208,0,0,4,0,0,1,0,2,3,0,0,2,5,0,0,2,5,0,0,2,4,0,0,
70,0,0,2,0,0,31,217,0,0,0,0,0,1,0,
85,0,0,1,0,0,15,224,0,0,0,0,0,1,0,
100,0,0,3,0,0,29,225,0,0,0,0,0,1,0,
115,0,0,4,0,0,17,250,0,0,1,1,0,1,0,1,5,0,0,
134,0,0,4,0,0,21,252,0,0,0,0,0,1,0,
149,0,0,5,0,0,31,261,0,0,0,0,0,1,0,
164,0,0,6,0,0,29,265,0,0,0,0,0,1,0,
179,0,0,7,215,0,4,304,0,0,5,4,0,1,0,2,3,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
214,0,0,8,0,0,31,318,0,0,0,0,0,1,0,
229,0,0,9,182,0,3,326,0,0,4,4,0,1,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
260,0,0,10,0,0,31,344,0,0,0,0,0,1,0,
275,0,0,11,0,0,29,349,0,0,0,0,0,1,0,
290,0,0,12,130,0,4,379,0,0,2,1,0,1,0,2,3,0,0,1,3,0,0,
313,0,0,13,0,0,31,390,0,0,0,0,0,1,0,
328,0,0,14,130,0,4,395,0,0,2,1,0,1,0,2,3,0,0,1,3,0,0,
351,0,0,15,0,0,31,406,0,0,0,0,0,1,0,
366,0,0,16,0,0,29,459,0,0,0,0,0,1,0,
381,0,0,17,0,0,17,508,0,0,1,1,0,1,0,1,5,0,0,
400,0,0,17,0,0,21,510,0,0,0,0,0,1,0,
415,0,0,18,0,0,31,516,0,0,0,0,0,1,0,
430,0,0,19,0,0,17,525,0,0,1,1,0,1,0,1,5,0,0,
449,0,0,19,0,0,21,527,0,0,0,0,0,1,0,
464,0,0,20,0,0,31,533,0,0,0,0,0,1,0,
479,0,0,21,0,0,29,538,0,0,0,0,0,1,0,
};


/*****************************************************************************
* DESCRIPTION
*
* This file contains the SQL functions associated with the methods  
* that derive the data upward to the next interval 
*
***************************************
*
* SqlSelectData
*
* Purpose: Retrieves source data for the derivation from one of the
*          interval tables.
*
* Input:   UPDATE_RECORD* - pointer to the record to be derived
*          DATE_TIME* - pointer to begin of source window
*          DATE_TIME* - pointer to end of source window
*
* Output:  int* - pointer to an integer that will contain the number of
*                 source records retrieved
*          DATA_RECORD** - reference to a pointer to a list that will contain 
*                          the source data records retrieved
*
* Return:  int - OK or ERROR
*
***************************************
*
* SqlDeleteDestination
*
* Purpose: Deletes any existing value from the destination slot in the
*          destination table
*
* Input:   SQL_INTERVAL - destination interval to delete record from
*          int - destination site_datatype_id
*          SQL_DATE - start_date_time for destination slot
*          SQL_DATE - end_date_time for destination slot
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* SqlIntervalUpdate
*
* Purpose: Writes an entry to r_interval_update for the destination slot 
*          to continue the propagation of the derivation upward
*
* Input:   SQL_INTERVAL - destination interval to write record for
*          int - destination site_datatype_id
*          SQL_DATE - start_date_time for destination slot
*          SQL_DATE - end_date_time for destination slot
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* SqlConvertUnit
*
* Purpose: Determines if a unit conversion is required in deriving
*          from the source to the destination and converts the units,
*          if required
*
* Input:   SQL_INTERVAL - destination interval for the derivation
*          UPDATE_RECORD* - pointer to the record to be derived
*          
* Input/Output:  DATA_RECORD* - pointer to the resulting data record
*                which contains the value to be converted
*
* Return:  int - OK or ERROR
*
***************************************
*
* SqlWriteResult
*
* Purpose: Writes the results of the derivation to the destination
*          table
*
* Input:   SQL_INTERVAL - destination interval for the derivation
*          DATA_RECORD* - pointer to the resulting data record
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* Author
*
* Neil Wilson
* November, 2001
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "dbutils.h"
#include "defines.h"
#include "typedefs.h"
#include "derivation.h"
#include "derivationTypedefs.h"

extern FILE* LOGFILE;

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



#define FUNC_NAME "SqlSelectData"

int SqlSelectData(UPDATE_RECORD* updateRecord,
                  SQL_DATE beginTime,
                  SQL_DATE endTime,
                  int* sourceCount,
                  DATA_RECORD** sourceList )
{
   /* This function retrieves source data from one of the interval tables */

   /* NOTE:  THE USER OF THIS METHOD IS RESPONSIBLE FOR FREEING THE MEMORY
             THAT IS ALLOCATED TO sourceList BY MALLOC IF SOURCE DATA IS
             RETRIEVED */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      char sqlSelectStatement[600];
      DATA_RECORD* sqlDataRecord;
   /* EXEC SQL END DECLARE SECTION; */ 


   char selectStatement[600];
   int result;
   int sourceIndex;

   /* Get the count of data in the source data window */
   /* NOTE:  This select statement must be structured generically so that
      it works with any of the interval tables (at this time - instant,
      other, hour, day, month, year, wy).  In all cases the start time
      for selected data must be greater than or equal to the beginTime of 
      the window.  The start time less than the endTime of the window 
      condition is necessary to screen out instantaneous values with a
      start and end time equal to the endTime of the window (this end of
      the window instant is not actually part of the interval but is 
      equal to the start time of the next interval).  The end time 
      condition of less than or equal to the endTime of the window
      screens out "other" interval values that can be irregular and run over
      the endTime cut off. Business interval values will already be
      normalized and have end dates equal to the end of the intervals */
   sprintf(selectStatement, "SELECT COUNT(*) FROM r_%s WHERE site_datatype_id = %d AND start_date_time >= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND start_date_time < TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time <= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS')", updateRecord->interval, updateRecord->siteDatatypeId, beginTime, endTime, endTime);

   if ((result = SqlSelectCount(selectStatement, sourceCount)) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying row count of source data in r_%s\n",
                  updateRecord->interval);
      return (ERROR);
   }

   /* Create array of data structures to hold the source data */
   if (*sourceCount)
   {
      if ((*sourceList = (DATA_RECORD *) malloc 
         (sizeof(DATA_RECORD) * (*sourceCount))) == NULL)
      {
         FilePrintError(LOGFILE, "Problem allocating memory for sourceList\n");
         return (ERROR);
      }
   }
   else
   {
      *sourceList = NULL;
      return (OK);
   }

   /* Prepare the select statement for the source data */
   sprintf (sqlSelectStatement, "SELECT site_datatype_id, TO_CHAR (start_date_time, 'DD-MON-YYYY HH24:MI:SS'), TO_CHAR (end_date_time, 'DD-MON-YYYY HH24:MI:SS'), value FROM r_%s WHERE site_datatype_id = %d AND start_date_time >= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND start_date_time < TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time <= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS')ORDER BY start_date_time", updateRecord->interval, updateRecord->siteDatatypeId, beginTime, endTime, endTime);

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
   sqlstm.sqhstl[0] = (unsigned int  )600;
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


   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                 "Problem preparing select statement: %s...\n",
                 sqlSelectStatement)) != OK)
   {
      return (ERROR);
   }

   /* Declare source_data_cursor to select source data */
   /* EXEC SQL DECLARE source_data_cursor CURSOR FOR sel; */ 

   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                 "Problem declaring source_data_cursor for: %s...\n",
                 sqlSelectStatement)) != OK)
   {
      return (ERROR);
   }

   /* Open source_data_cursor */
   /* EXEC SQL OPEN source_data_cursor; */ 

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


   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                 "Problem opening source_data_cursor for\n: %s...\n",
                 sqlSelectStatement)) != OK)
   {
      return (ERROR);
   }

   /* EXEC SQL WHENEVER NOT FOUND GOTO close_source_data_cursor; */ 


   /* Fetch applicable source data */ 
   for (sourceIndex = 0; sourceIndex < *sourceCount ;
        sourceIndex++)
   {
      sqlDataRecord = &((*sourceList)[sourceIndex]);

      /* EXEC SQL FETCH source_data_cursor INTO
                       :sqlDataRecord->siteDatatypeId, 
                       :sqlDataRecord->startDateTime,
                       :sqlDataRecord->endDateTime,
                       :sqlDataRecord->value; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )39;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&(sqlDataRecord->siteDatatypeId);
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)(sqlDataRecord->startDateTime);
      sqlstm.sqhstl[1] = (unsigned int  )21;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)(sqlDataRecord->endDateTime);
      sqlstm.sqhstl[2] = (unsigned int  )21;
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)&(sqlDataRecord->value);
      sqlstm.sqhstl[3] = (unsigned int  )sizeof(double);
      sqlstm.sqhsts[3] = (         int  )0;
      sqlstm.sqindv[3] = (         void  *)0;
      sqlstm.sqinds[3] = (         int  )0;
      sqlstm.sqharm[3] = (unsigned int  )0;
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
      if (sqlca.sqlcode == 1403) goto close_source_data_cursor;
}


      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
           "Problem fetching source_data_cursor for\n: %s...\n",
           sqlSelectStatement)) != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 4;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )70;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      }
   }

   /* Close source_data_cursor */
   close_source_data_cursor:
   /* EXEC SQL CLOSE source_data_cursor; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )85;
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
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )100;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   /* EXEC SQL WHENEVER NOT FOUND CONTINUE; */ 
   
   
   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlDeleteDestination"

int SqlDeleteDestination(SQL_INTERVAL destInterval,
                         int destSDI,
                         SQL_DATE startDateTime,
                         SQL_DATE endDateTime)
{
   /* This function deletes any existing value in the destination table */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      char sqlDeleteStatement[600];
   /* EXEC SQL END DECLARE SECTION; */ 


   int result;

   /* Prepare and execute the delete statement */
   sprintf (sqlDeleteStatement, "DELETE FROM r_%s WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS')", destInterval, destSDI, startDateTime, endDateTime);

   /* EXEC SQL PREPARE del FROM :sqlDeleteStatement; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )115;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)sqlDeleteStatement;
   sqlstm.sqhstl[0] = (unsigned int  )600;
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



   /* EXEC SQL EXECUTE del; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )134;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   /* TRUE used in second argument to SqlFilePrintErrorHandler because it
      is OK if no rows were processed above - means a value hadn't 
      been previously written to the destination table */
   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE, 
        "Problem deleting data record from r_%s\n", 
        destInterval)) != OK)
   {
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )149;
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
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )164;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlIntervalUpdate"

int SqlIntervalUpdate(SQL_INTERVAL destInterval,
                      int destSDI,
                      SQL_DATE startDateTime,
                      SQL_DATE endDateTime)
{
   /* This function writes an entry to r_interval_update to continue the
      propagation of the derivation upward */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      int sqlCount;
      int sqlDestSDI;
      SQL_DATE sqlStartDateTime;
      SQL_DATE sqlEndDateTime;
      SQL_INTERVAL sqlDestInterval;
   /* EXEC SQL END DECLARE SECTION; */ 


   int result;

   /* If the entry is to be for year or wy, the entry is not needed for further
      propagation */
   if ( (!strcmp(destInterval, "year")) || (!strcmp(destInterval, "wy")))
   {
      return (OK);
   }

   /* Initialize the sql variables */
   strcpy (sqlDestInterval, destInterval);
   strcpy (sqlStartDateTime, startDateTime);
   strcpy (sqlEndDateTime, endDateTime);
   sqlDestSDI = destSDI;

   /* Do a count to see if the record already exists */
   /* EXEC SQL
      SELECT COUNT(*) INTO :sqlCount
      FROM r_interval_update 
      WHERE site_datatype_id = :sqlDestSDI
         AND start_date_time = TO_DATE (:sqlStartDateTime,
                                  'DD-MON-YYYY HH24:MI:SS')
         AND end_date_time = TO_DATE (:sqlEndDateTime,
                                  'DD-MON-YYYY HH24:MI:SS')
         AND interval = :sqlDestInterval; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select count(*)  into :b0  from r_interval_update where ((\
(site_datatype_id=:b1 and start_date_time=TO_DATE(:b2,'DD-MON-YYYY HH24:MI:SS'\
)) and end_date_time=TO_DATE(:b3,'DD-MON-YYYY HH24:MI:SS')) and  interval =:b4\
)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )179;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&sqlCount;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)&sqlDestSDI;
   sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)sqlStartDateTime;
   sqlstm.sqhstl[2] = (unsigned int  )21;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         void  *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned int  )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (         void  *)sqlEndDateTime;
   sqlstm.sqhstl[3] = (unsigned int  )21;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         void  *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned int  )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (         void  *)sqlDestInterval;
   sqlstm.sqhstl[4] = (unsigned int  )33;
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
}



   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                 "Problem querying for count of data in r_interval_update\n"))
                 != OK)
   {
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )214;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
   }

   /* If count is nonzero, record exists and does not need to be written */
   /* If count is zero, the record needs to be inserted */
   if (sqlCount == 0)
   {
      /* EXEC SQL
         INSERT INTO r_interval_update( 
            site_datatype_id,
            start_date_time,
            end_date_time,
            interval)
         VALUES (
            :sqlDestSDI,
            TO_DATE (:sqlStartDateTime,
                     'DD-MON-YYYY HH24:MI:SS'),
            TO_DATE (:sqlEndDateTime,
                     'DD-MON-YYYY HH24:MI:SS'),
            :sqlDestInterval); */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "insert into r_interval_update (site_datatype_id,start_d\
ate_time,end_date_time, interval ) values (:b0,TO_DATE(:b1,'DD-MON-YYYY HH24:M\
I:SS'),TO_DATE(:b2,'DD-MON-YYYY HH24:MI:SS'),:b3)";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )229;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&sqlDestSDI;
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)sqlStartDateTime;
      sqlstm.sqhstl[1] = (unsigned int  )21;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)sqlEndDateTime;
      sqlstm.sqhstl[2] = (unsigned int  )21;
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)sqlDestInterval;
      sqlstm.sqhstl[3] = (unsigned int  )33;
      sqlstm.sqhsts[3] = (         int  )0;
      sqlstm.sqindv[3] = (         void  *)0;
      sqlstm.sqinds[3] = (         int  )0;
      sqlstm.sqharm[3] = (unsigned int  )0;
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



      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                    "Problem inserting row into r_interval_update\n")
                    )!= OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )260;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      } 
   }

   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )275;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlConvertUnit"

int SqlConvertUnit(SQL_INTERVAL destInterval,
                   UPDATE_RECORD* updateRecord,
                   DATA_RECORD* resultRecord)
{
   /* This function determines if a unit conversion is required in deriving
      from the source to the destination and converts the units, if required */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      ID sqlDestUnitId;
      ID sqlSourceUnitId;
      int sqlDestSDI;
      int sqlSourceSDI;
   /* EXEC SQL END DECLARE SECTION; */ 


   double resultValue;
   int result;
   TIME_INTERVAL_TYPES intervalType;

   /* Initialize sql variables */
   sqlSourceSDI = updateRecord->siteDatatypeId;
   sqlDestSDI = resultRecord->siteDatatypeId;

   /* Retrieve the unit id for the source SDI */
   /* EXEC SQL
      SELECT d.unit_id
      INTO :sqlSourceUnitId
      FROM hdb_site_datatype s, hdb_datatype d
      WHERE s.datatype_id = d.datatype_id
         AND s.site_datatype_id = :sqlSourceSDI; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select d.unit_id into :b0  from hdb_site_datatype s ,hdb_d\
atatype d where (s.datatype_id=d.datatype_id and s.site_datatype_id=:b1)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )290;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&sqlSourceUnitId;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)&sqlSourceSDI;
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



   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                 "Problem retrieving unit_id for source SDI %d\n", 
                 sqlSourceSDI))!= OK)
   {
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )313;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
   }

   /* Retrieve the unit id for the destination SDI */
   /* EXEC SQL
      SELECT d.unit_id
      INTO :sqlDestUnitId
      FROM hdb_site_datatype s, hdb_datatype d
      WHERE s.datatype_id = d.datatype_id
         AND s.site_datatype_id = :sqlDestSDI; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select d.unit_id into :b0  from hdb_site_datatype s ,hdb_d\
atatype d where (s.datatype_id=d.datatype_id and s.site_datatype_id=:b1)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )328;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&sqlDestUnitId;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)&sqlDestSDI;
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



   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                 "Problem retrieving unit_id for destination SDI %d\n", 
                  sqlDestSDI))!= OK)
   {
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )351;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
   }

   /* Compare the unit ids and convert if required */
   if (sqlSourceUnitId != sqlDestUnitId)
   {
      /* First the TIME_INTERVAL_TYPES must be determined */
      if (!strncmp(destInterval, "ho", 2))
      {
         intervalType = BY_HOUR;
      }
      else if (!strncmp(destInterval, "da", 2))
      {
         intervalType = BY_DAY;
      } 
      else if (!strncmp(destInterval, "mo", 2))
      {
         intervalType = BY_MONTH;
      } 
      else if (!strncmp(destInterval, "yr", 2))
      {
         intervalType = BY_YEAR;
      } 
      else if (!strncmp(destInterval, "wy", 2))
      {
         intervalType = BY_WY;
      }
      else
      {
         FilePrintError(LOGFILE, "Code to determine the TIME_INTERVAL_TYPES for the interval named %s needs to be written in the function SqlConvertUnit\n", destInterval);
         return (ERROR);      
      }

      /* Call the ConvertValue function */
      if ((result = ConvertValue(resultRecord->value,
                    1,
                    sqlSourceUnitId,
                    resultRecord->startDateTime,
                    &resultValue,
                    1,
                    sqlDestUnitId,
                    intervalType,
                    NA)) != OK)
      {
         FilePrintError(LOGFILE, "Problem figuring unit conversion for the site_datatypes %d to %d\n", sqlSourceSDI, sqlDestSDI);
         return (ERROR);
      }

      /* Assign the converted value */
      resultRecord->value = resultValue;
   }

   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )366;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlWriteResult"

int SqlWriteResult(SQL_INTERVAL destInterval,
                   DATA_RECORD* resultRecord)
{
   /* This function writes the results of the derivation to the destination
      table */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      char sqlStatement[600];
   /* EXEC SQL END DECLARE SECTION; */ 


   char tempFlag[20];
   char selectStatement[600];
   int destCount;
   int result;

   /* Find out if derivation flag value should be NULL and assign it to 
      the variable */
   if (!strcmp(resultRecord->derivationFlag, EMPTY_STRING))
   {
      sprintf(resultRecord->derivationFlag, "NULL");
   }
   else
   {
      /* add single quotes around flag value for the sql statements */
      strcpy(tempFlag, resultRecord->derivationFlag);
      sprintf(resultRecord->derivationFlag, "'%s'", tempFlag);
   }

   /* Find out if this row already exists in the destination table */
   sprintf(selectStatement, "SELECT COUNT(*) FROM r_%s WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS')", destInterval, resultRecord->siteDatatypeId, resultRecord->startDateTime, resultRecord->endDateTime);

   if ((result = SqlSelectCount(selectStatement, &destCount)) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying row count in r_%s\n", destInterval);
      return (ERROR);
   }

   /* If record doesn't exist, insert it */
   if (destCount == 0)
   {
      sprintf(sqlStatement, "INSERT INTO r_%s (site_datatype_id, start_date_time, end_date_time, value, overwrite_flag, method_id, derivation_flags, date_time_loaded, validation) VALUES (%d, TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS'), TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS'), %f, NULL, %d, %s, TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS'), NULL)", destInterval, resultRecord->siteDatatypeId, resultRecord->startDateTime, resultRecord->endDateTime, resultRecord->value, resultRecord->methodId, resultRecord->derivationFlag, resultRecord->dateTimeLoaded);

      /* EXEC SQL PREPARE ins FROM :sqlStatement; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )381;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)sqlStatement;
      sqlstm.sqhstl[0] = (unsigned int  )600;
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



      /* EXEC SQL EXECUTE ins; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )400;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                    "Problem inserting data record into r_%s\n", 
                    destInterval)) != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )415;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      }       
   }
   /* Record exists, so is an update */
   else
   {
      sprintf(sqlStatement, "UPDATE r_%s SET value = %f, method_id = %d, derivation_flags = %s, date_time_loaded = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS')", destInterval, resultRecord->value, resultRecord->methodId, resultRecord->derivationFlag, resultRecord->dateTimeLoaded, resultRecord->siteDatatypeId, resultRecord->startDateTime, resultRecord->endDateTime);

      /* EXEC SQL PREPARE upd FROM :sqlStatement; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )430;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)sqlStatement;
      sqlstm.sqhstl[0] = (unsigned int  )600;
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



      /* EXEC SQL EXECUTE upd; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )449;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                    "Problem updating data record in r_%s\n", 
                    destInterval)) != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )464;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      }       
   }

   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )479;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
}

