
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
           char  filnam[16];
};
static const struct sqlcxp sqlfpn =
{
    15,
    "SqlOverwrite.pc"
};


static unsigned long sqlctx = 2661923;


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

 static const char *sq0004 = 
"select site_datatype_id ,TO_CHAR(start_date_time,'DD-MON-YYYY HH24:MI:SS') ,\
TO_CHAR(end_date_time,'DD-MON-YYYY HH24:MI:SS') , interval   from r_base_updat\
e where overwrite_flag in ('O','o')           ";

 static const char *sq0015 = 
"select d.base_site_datatype_id ,s. interval  ,s.time_offset_minutes  from re\
f_derivation_destination d ,ref_derivation_source s where (d.dest_site_datatyp\
e_id=:b0 and d.base_site_datatype_id=s.site_datatype_id)           ";

 static const char *sq0025 = 
"select site_datatype_id ,value  from r_base where ((((site_datatype_id=:b0 a\
nd  interval =:b1) and overwrite_flag in ('O','o')) and start_date_time>=TO_DA\
TE(:b2,'DD-MON-YYYY HH24:MI:SS')) and end_date_time<=TO_DATE(:b3,'DD-MON-YYYY \
HH24:MI:SS')) order by date_time_loaded desc             ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,79,0,4,111,0,0,1,0,0,1,0,2,3,0,0,
24,0,0,2,0,0,31,120,0,0,0,0,0,1,0,
39,0,0,3,0,0,29,129,0,0,0,0,0,1,0,
54,0,0,4,200,0,9,156,0,0,0,0,0,1,0,
69,0,0,4,0,0,13,171,0,0,4,0,0,1,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
100,0,0,5,0,0,31,179,0,0,0,0,0,1,0,
115,0,0,4,0,0,15,186,0,0,0,0,0,1,0,
130,0,0,6,0,0,29,187,0,0,0,0,0,1,0,
145,0,0,7,240,0,4,231,0,0,5,4,0,1,0,2,3,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
180,0,0,8,0,0,31,246,0,0,0,0,0,1,0,
195,0,0,9,0,0,17,258,0,0,1,1,0,1,0,1,5,0,0,
214,0,0,9,0,0,21,259,0,0,0,0,0,1,0,
229,0,0,10,0,0,31,268,0,0,0,0,0,1,0,
244,0,0,11,0,0,17,290,0,0,1,1,0,1,0,1,5,0,0,
263,0,0,11,0,0,21,291,0,0,0,0,0,1,0,
278,0,0,12,0,0,31,300,0,0,0,0,0,1,0,
293,0,0,13,90,0,4,313,0,0,2,1,0,1,0,2,3,0,0,1,3,0,0,
316,0,0,14,0,0,31,322,0,0,0,0,0,1,0,
331,0,0,15,221,0,9,344,0,0,1,1,0,1,0,1,3,0,0,
350,0,0,15,0,0,13,352,0,0,3,0,0,1,0,2,3,0,0,2,5,0,0,2,3,0,0,
377,0,0,16,0,0,31,360,0,0,0,0,0,1,0,
392,0,0,15,0,0,15,365,0,0,0,0,0,1,0,
407,0,0,17,0,0,29,366,0,0,0,0,0,1,0,
422,0,0,18,0,0,17,460,0,0,1,1,0,1,0,1,5,0,0,
441,0,0,18,0,0,45,479,0,0,0,0,0,1,0,
456,0,0,18,0,0,13,487,0,0,2,0,0,1,0,2,5,0,0,2,5,0,0,
479,0,0,19,0,0,31,494,0,0,0,0,0,1,0,
494,0,0,18,0,0,15,499,0,0,0,0,0,1,0,
509,0,0,20,0,0,29,500,0,0,0,0,0,1,0,
524,0,0,21,211,0,4,505,0,0,5,4,0,1,0,2,3,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
559,0,0,22,0,0,31,519,0,0,0,0,0,1,0,
574,0,0,23,178,0,3,527,0,0,4,4,0,1,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
605,0,0,24,0,0,31,545,0,0,0,0,0,1,0,
620,0,0,25,289,0,9,597,0,0,4,4,0,1,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
651,0,0,25,0,0,13,607,0,0,2,0,0,1,0,2,3,0,0,2,4,0,0,
674,0,0,26,0,0,31,614,0,0,0,0,0,1,0,
689,0,0,25,0,0,15,619,0,0,0,0,0,1,0,
704,0,0,27,0,0,29,620,0,0,0,0,0,1,0,
719,0,0,28,0,0,17,640,0,0,1,1,0,1,0,1,5,0,0,
738,0,0,28,0,0,21,641,0,0,0,0,0,1,0,
753,0,0,29,0,0,31,647,0,0,0,0,0,1,0,
768,0,0,30,0,0,17,656,0,0,1,1,0,1,0,1,5,0,0,
787,0,0,30,0,0,21,657,0,0,0,0,0,1,0,
802,0,0,31,0,0,31,663,0,0,0,0,0,1,0,
817,0,0,32,0,0,17,697,0,0,1,1,0,1,0,1,5,0,0,
836,0,0,32,0,0,21,698,0,0,0,0,0,1,0,
851,0,0,33,0,0,31,707,0,0,0,0,0,1,0,
866,0,0,34,246,0,5,717,0,0,4,4,0,1,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
897,0,0,35,0,0,31,729,0,0,0,0,0,1,0,
912,0,0,36,0,0,29,735,0,0,0,0,0,1,0,
927,0,0,37,54,0,2,762,0,0,0,0,0,1,0,
942,0,0,38,0,0,31,772,0,0,0,0,0,1,0,
957,0,0,39,0,0,29,776,0,0,0,0,0,1,0,
972,0,0,32,0,0,17,791,0,0,1,1,0,1,0,1,5,0,0,
991,0,0,40,0,0,31,795,0,0,0,0,0,1,0,
1006,0,0,32,0,0,21,799,0,0,0,0,0,1,0,
1021,0,0,41,0,0,31,806,0,0,0,0,0,1,0,
1036,0,0,42,0,0,29,810,0,0,0,0,0,1,0,
};


/*****************************************************************************
* DESCRIPTION
*
* This file contains the SQL functions that load new and changed overwrite
* data from r_base, move it to the appropriate interval, and mark it and
* adjacent lower interval data with correct overwrite flags.
*
***************************************
*
* SqlCopyOverwrite
*
* Purpose: Contains logic for copying new and changed overwrite data from
*          r_base.
*
* Input:   None
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* SqlDeleteOverwrite
*
* Purpose: Deletes overwrite records from r_base_update that are
*          marked as "ready for delete".
*
* Input:   None
*
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
*
* SqlMethodTwo
*
* Purpose: Executes a Dynamic SQL Method 2 type of statement  - used
*          in nonSql files.
*
* Input:   char* - statement to be executed
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
* June, 2002
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

extern SQL_DATE DATETIMELOADED;
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


#define FUNC_NAME "SqlCopyOverwrite"

int SqlCopyOverwrite(int* baseUpdateTotal)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      char sqlAddTruncateStatement[600];
      char sqlDeleteStatement[600];
      char sqlInsertStatement[600];
      char sqlIntervalTable[12];
      char sqlPreviousIntervalTable[12];
      char sqlRemoveTruncateStatement[600];
      char sqlSelectStatement[600];
      char sqlUpdateStatement[600];
      DATA_RECORD sqlDataRecord;
      IND sqlTimeOffset_ind;
      int sqlBaseSDI;
      int sqlCountOverwrite;
      int sqlCountSource;
      int sqlCountSpec;
      int sqlCountBase;
      int sqlTimeOffset;
      INTERVAL_TIME sqlIntervalTime;
      SQL_DATE sqlBeginDate;
      SQL_DATE sqlEndDate;
      SQL_INTERVAL sqlBaseInterval;
      UPDATE_RECORD* sqlOverwriteRecord;
   /* EXEC SQL END DECLARE SECTION; */ 


   char selectStatement[600];
   int baseOrd;
   int countDestination;
   int countSource;
   int overwriteIndex = 0;
   int order;
   int positiveValue;
   int previousOrder;
   int result;
   SQL_INTERVAL previousInterval;
   UPDATE_RECORD* overwriteDataList;

   /* Load rows from r_base_update marked as overwrites */
   /* Find out how many applicable rows in r_base_update */
   /* EXEC SQL
      SELECT COUNT(*) INTO :sqlCountOverwrite
      FROM r_base_update 
      WHERE overwrite_flag IN ('O', 'o'); */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 1;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select count(*)  into :b0  from r_base_update where overwr\
ite_flag in ('O','o')";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&sqlCountOverwrite;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
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
           "Problem querying for count of overwrite data in r_base_update\n"))
           != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 1;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )24;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      }

   *baseUpdateTotal = *baseUpdateTotal + sqlCountOverwrite;

   if (sqlCountOverwrite == 0)
   {
      /* No overwrites to process */
      /* EXEC SQL COMMIT; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 1;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )39;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (OK);      
   }

   /* Create array of structures to hold overwrite data */
   if ((overwriteDataList = (UPDATE_RECORD *) malloc 
       (sizeof(UPDATE_RECORD) * sqlCountOverwrite)) == NULL)
   {
       FilePrintError(LOGFILE, "Problem allocating memory for overwriteDataList\n");
       return (ERROR);
   }

   /* Declare overwrite_cursor for selecting overwrite data */
      /* EXEC SQL DECLARE overwrite_cursor CURSOR FOR
         SELECT site_datatype_id,
                TO_CHAR (start_date_time, 'DD-MON-YYYY HH24:MI:SS'),
                TO_CHAR (end_date_time, 'DD-MON-YYYY HH24:MI:SS'),
                interval
         FROM r_base_update
         WHERE overwrite_flag IN ('O', 'o'); */ 

      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                    "Problem declaring overwrite_cursor\n")) != OK)
      {
         return (ERROR);
      }

   /* Open overwrite_cursor */
   /* EXEC SQL OPEN overwrite_cursor; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 1;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = sq0004;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )54;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                 "Problem opening overwrite_cursor\n")) != OK)
   {
      return (ERROR);
   }

   /* EXEC SQL WHENEVER NOT FOUND GOTO close_overwrite_cursor; */ 


   /* Fetch applicable overwrite data */ 
   for (overwriteIndex = 0; overwriteIndex < sqlCountOverwrite ;
        overwriteIndex++)
   {
      sqlOverwriteRecord = &overwriteDataList[overwriteIndex];

      /* EXEC SQL FETCH overwrite_cursor INTO
                             :sqlOverwriteRecord->siteDatatypeId, 
                             :sqlOverwriteRecord->startDateTime,
                             :sqlOverwriteRecord->endDateTime,
                             :sqlOverwriteRecord->interval; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )69;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&(sqlOverwriteRecord->siteDatatypeId);
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)(sqlOverwriteRecord->startDateTime);
      sqlstm.sqhstl[1] = (unsigned int  )21;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)(sqlOverwriteRecord->endDateTime);
      sqlstm.sqhstl[2] = (unsigned int  )21;
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)(sqlOverwriteRecord->interval);
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
      if (sqlca.sqlcode == 1403) goto close_overwrite_cursor;
}


      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
         "Problem fetching overwrite_cursor\n")) != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

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


         return (ERROR);
      }
   }

   /* Close overwrite_cursor */
      close_overwrite_cursor:
      /* EXEC SQL CLOSE overwrite_cursor; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )115;
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
      sqlstm.offset = (unsigned int  )130;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      /* EXEC SQL WHENEVER NOT FOUND CONTINUE; */ 


   /* Step through data list and insert, update, or delete the
      overwrite value, as appropriate, in its interval table */
   for (overwriteIndex = 0; overwriteIndex < sqlCountOverwrite ;
        overwriteIndex++)
   {
      sqlOverwriteRecord = &overwriteDataList[overwriteIndex];

      /* Check to make sure the interval of the overwrite value is 
         legitimate */
      if ((result = SqlGetIntOrder(sqlOverwriteRecord->interval, &order))
                 != OK)
      {
         FilePrintError(LOGFILE, "Problem getting interval order number for %s\n",
                     sqlOverwriteRecord->interval);
         return (ERROR);
      }
      /* If value is not one of the regular business intervals, it cannot
         be an overwrite value (this can be determined by checking if the
         interval order is 1).*/   
      if (order <= 1)
      {
         FilePrintError(LOGFILE, "Overwrite value cannot have an interval of %s\n",
                     sqlOverwriteRecord->interval);
         return (ERROR);
      }

      /* Create the destination interval table name */
      sprintf (sqlIntervalTable, "r_%s", sqlOverwriteRecord->interval);

      /* Get the destination interval times and the source data window
         times for this record */
      if ((result = SqlTimeInterval(sqlOverwriteRecord, 
                                 sqlOverwriteRecord->interval,
                                 &sqlIntervalTime,
                                 NULL)) != OK)
      {
         FilePrintError(LOGFILE, "Problem calculating interval times for overwrite record\n");
         return (ERROR);
      }

      /* Get count of overwrite data from r_base in the source data window */
      /* EXEC SQL
         SELECT COUNT(*) INTO :sqlCountSource
         FROM r_base 
         WHERE site_datatype_id = :sqlOverwriteRecord->siteDatatypeId
            AND interval = :sqlOverwriteRecord->interval
            AND overwrite_flag IN ('O', 'o')
            AND start_date_time >= TO_DATE (:sqlIntervalTime.begSource,
                                            'DD-MON-YYYY HH24:MI:SS')
            AND end_date_time <= TO_DATE (:sqlIntervalTime.endSource,
                                         'DD-MON-YYYY HH24:MI:SS'); */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "select count(*)  into :b0  from r_base where ((((site_d\
atatype_id=:b1 and  interval =:b2) and overwrite_flag in ('O','o')) and start_\
date_time>=TO_DATE(:b3,'DD-MON-YYYY HH24:MI:SS')) and end_date_time<=TO_DATE(:\
b4,'DD-MON-YYYY HH24:MI:SS'))";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )145;
      sqlstm.selerr = (unsigned short)1;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&sqlCountSource;
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)&(sqlOverwriteRecord->siteDatatypeId);
      sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)(sqlOverwriteRecord->interval);
      sqlstm.sqhstl[2] = (unsigned int  )33;
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)(sqlIntervalTime.begSource);
      sqlstm.sqhstl[3] = (unsigned int  )21;
      sqlstm.sqhsts[3] = (         int  )0;
      sqlstm.sqindv[3] = (         void  *)0;
      sqlstm.sqinds[3] = (         int  )0;
      sqlstm.sqharm[3] = (unsigned int  )0;
      sqlstm.sqadto[3] = (unsigned short )0;
      sqlstm.sqtdso[3] = (unsigned short )0;
      sqlstm.sqhstv[4] = (         void  *)(sqlIntervalTime.endSource);
      sqlstm.sqhstl[4] = (unsigned int  )21;
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
                    "Problem querying for count of overwrites in r_base\n")
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
         sqlstm.offset = (unsigned int  )180;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      }

      /* If no overwrite values exist in r_base in the source window,
         the entry represents a delete from the base area, so need to 
         delete the overwrite from the destination table if it exists,
         and reestablish normal derivation for this spot */
      if (sqlCountSource == 0)
      {
         /* Delete record from destination table */
         sprintf(sqlDeleteStatement, "DELETE FROM %s  WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND overwrite_flag IN ('O', 'o')",  sqlIntervalTable, sqlOverwriteRecord->siteDatatypeId, sqlIntervalTime.begInterval, sqlIntervalTime.endInterval);
         /* EXEC SQL PREPARE del FROM :sqlDeleteStatement; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )195;
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
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )214;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



         /* TRUE used in second argument to SqlFilePrintErrorHandler because it
            is OK if no rows were processed above - means the overwrite hadn't 
            been previously copied to the destination table */
         if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE, 
                       "Problem deleting overwrite record from %s\n", 
                       sqlIntervalTable)) != OK)
         {
            /* EXEC SQL ROLLBACK; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )229;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


            return (ERROR);
         }

         /* If the next lower interval is still a business interval, need to
            unmark as truncated any data (that are not overwrite values) 
            within the source window times with this SDI */
         previousOrder = order - 1;
         if (previousOrder > 1)
         {
            /* Get previous interval name */
            if ((result = SqlGetIntName(previousOrder, previousInterval)) != OK)
            {
               FilePrintError(LOGFILE, "Problem finding interval name for order number %d\n", previousOrder);
               return (ERROR);
            }

            /* Construct previous interval table name */
            sprintf (sqlPreviousIntervalTable, "r_%s", previousInterval);  

            /* Unmark records in the previous interval */
            sprintf(sqlRemoveTruncateStatement, "UPDATE %s SET overwrite_flag = NULL WHERE site_datatype_id = %d AND start_date_time >= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time <= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND overwrite_flag NOT IN ('O', 'o')", sqlPreviousIntervalTable, sqlOverwriteRecord->siteDatatypeId, sqlIntervalTime.begSource, sqlIntervalTime.endSource);
            /* EXEC SQL PREPARE unmark FROM :sqlRemoveTruncateStatement; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )244;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)sqlRemoveTruncateStatement;
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


            /* EXEC SQL EXECUTE unmark; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )263;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



            /* TRUE used in second argument to SqlFilePrintErrorHandler because
               it is OK if no rows were processed above - means were no 
               records marked in the previous interval */
            if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE, 
                          "Problem unmarking truncated records in %s\n", 
                          sqlPreviousIntervalTable)) != OK)
            {
               /* EXEC SQL ROLLBACK; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )278;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


               return (ERROR);
            }
         }

         /* Reestablish normal derivation through the now deleted overwrite
            by tracking back to the base site_datatype_id and interval, and
            writing one of these values to r_base_update to kick off
            rederivation of the tree the overwrite was in */
         /* Find out if there is a ref_derivation_destination spec 
            for the deleted value (can only be one due to primary key) -
            if no spec, we don't have to worry about reestablishing
            the derivation */
         /* EXEC SQL
            SELECT COUNT(*) INTO :sqlCountSpec
            FROM ref_derivation_destination 
            WHERE dest_site_datatype_id = :sqlOverwriteRecord->siteDatatypeId; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "select count(*)  into :b0  from ref_derivation_desti\
nation where dest_site_datatype_id=:b1";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )293;
         sqlstm.selerr = (unsigned short)1;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)&sqlCountSpec;
         sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)&(sqlOverwriteRecord->siteDatatypeId);
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
                       "Problem querying for count of spec in ref_derivation_destination\n")
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
            sqlstm.offset = (unsigned int  )316;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


            return (ERROR);
         }

         if (sqlCountSpec == 1)
         {
            /* EXEC SQL DECLARE spec_cursor CURSOR FOR
               SELECT d.base_site_datatype_id,
                      s.interval,
                      s.time_offset_minutes
               FROM ref_derivation_destination d,
                    ref_derivation_source s
               WHERE d.dest_site_datatype_id = :sqlOverwriteRecord->siteDatatypeId
                     AND d.base_site_datatype_id = s.site_datatype_id; */ 


            if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
               "Problem declaring spec_cursor\n")) != OK)
            {
               return (ERROR);
            }

            /* Open spec_cursor */
            /* EXEC SQL OPEN spec_cursor; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = sq0015;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )331;
            sqlstm.selerr = (unsigned short)1;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)&(sqlOverwriteRecord->siteDatatypeId);
            sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
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
               "Problem opening spec_cursor\n")) != OK)
            {
               return (ERROR);
            }

            /* Fetch the spec data - only one set due to primary keys */
            /* EXEC SQL FETCH spec_cursor
               INTO :sqlBaseSDI,
                    :sqlBaseInterval,
                    :sqlTimeOffset INDICATOR :sqlTimeOffset_ind; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )350;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)&sqlBaseSDI;
            sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)sqlBaseInterval;
            sqlstm.sqhstl[1] = (unsigned int  )33;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         void  *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned int  )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (         void  *)&sqlTimeOffset;
            sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         void  *)&sqlTimeOffset_ind;
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
}

 

            if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                "Problem fetching spec_cursor\n")) != OK)
            {
               /* EXEC SQL ROLLBACK; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )377;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


               return (ERROR);
            }

            /* Close spec_cursor */
            /* EXEC SQL CLOSE spec_cursor; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )392;
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
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )407;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



            /* Find the order number for the base interval */
            if ((result = SqlGetIntOrder(
                          (sqlBaseInterval),
                           &baseOrd)) != OK)
            {
               FilePrintError(LOGFILE, "Problem finding interval order number of %s\n", sqlBaseInterval);
               return (ERROR);
            }

            /* If the base interval is a nonbusiness interval and there is a 
               time offset specified in the spec, we need to adjust the times
               for this when tracking back to the base data */
            if (baseOrd == 1 && sqlTimeOffset_ind >= 0)
            {
               if (sqlTimeOffset < 0)
               {
                  /* Are adding a negative so need to call SUBTRACTION of the
                     positive value for SqlDateMath */
                  positiveValue = (-1) * sqlTimeOffset;
                  if ((result = SqlDateMath(SUBTRACTION, 
                                            sqlIntervalTime.begSource,
                                            sqlIntervalTime.begSource, 
                                            positiveValue,
                                            MINUTE)) != OK)
                  {
                     FilePrintError(LOGFILE, "Problem adding time offset to begSource");
                     return (ERROR);
                  }
                  if ((result = SqlDateMath(SUBTRACTION, 
                                            sqlIntervalTime.endSource,
                                            sqlIntervalTime.endSource, 
                                            positiveValue,
                                            MINUTE)) != OK)
                  {
                     FilePrintError(LOGFILE, "Problem adding time offset to endSource");
                     return (ERROR);
                  }
               }
               else
               {
                  /* Call ADDITION */
                  if ((result = SqlDateMath(ADDITION, 
                                            sqlIntervalTime.begSource,
                                            sqlIntervalTime.begSource, 
                                            sqlTimeOffset,
                                            MINUTE)) != OK)
                  {
                     FilePrintError(LOGFILE, "Problem adding time offset to begSource");
                     return (ERROR);
                  }
                  if ((result = SqlDateMath(ADDITION, 
                                            sqlIntervalTime.endSource,
                                            sqlIntervalTime.endSource, 
                                            sqlTimeOffset,
                                            MINUTE)) != OK)
                  {
                     FilePrintError(LOGFILE, "Problem adding time offset to endSource");
                     return (ERROR);
                  }
               }
            }

            /* Check to see if there are values at the base interval during the
               source times */
   /* NOTE:  This select statement must be structured generically so that
      it works with any of the interval tables (at this time - instant,
      other, hour, day, month, year, wy).  In all cases the start time
      for selected data must be greater than or equal to the begSource of 
      the window.  The start time less than the endSource of the window 
      condition is necessary to screen out instantaneous values with a
      start and end time equal to the endSource of the window (this end of
      the window instant is not actually part of the interval but is 
      equal to the start time of the next interval).  The end time 
      condition of less than or equal to the endSource of the window
      screens out "other" interval values that can be irregular and run over
      the endSource cut off. Business interval values will already be
      normalized and have end dates equal to the end of the intervals */
            sprintf(selectStatement, "SELECT COUNT(*) FROM r_%s WHERE site_datatype_id = %d AND start_date_time >= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND start_date_time < TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time <= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS')", sqlBaseInterval, sqlBaseSDI, sqlIntervalTime.begSource, sqlIntervalTime.endSource, sqlIntervalTime.endSource);

            if ((result = SqlSelectCount(selectStatement, &countSource)) != OK)
            {
               FilePrintError(LOGFILE, "Problem querying count for base data\n");
               return (ERROR);
            }            

            /* If there are values, select one and write an entry for it to
               r_base_update */
            if ( countSource >= 1)
            {
               /* Prepare the select statement for base interval data*/
               sprintf (sqlSelectStatement, "SELECT TO_CHAR (start_date_time, 'DD-MON-YYYY HH24:MI:SS'), TO_CHAR (end_date_time, 'DD-MON-YYYY HH24:MI:SS') FROM r_%s WHERE site_datatype_id = %d AND start_date_time >= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND start_date_time < TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time <= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS')", sqlBaseInterval, sqlBaseSDI, sqlIntervalTime.begSource, sqlIntervalTime.endSource, sqlIntervalTime.endSource);

               /* EXEC SQL PREPARE sel FROM :sqlSelectStatement; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.stmt = "";
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )422;
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

               /* Declare base_interval_cursor for selecting base interval
                  data */
               /* EXEC SQL DECLARE base_interval_cursor CURSOR FOR sel; */ 

               if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                            "Problem declaring base_interval_cursor for: %s\n",
                            sqlSelectStatement)) != OK)
               {
                  return (ERROR);
               }

               /* Open base_interval_cursor */
               /* EXEC SQL OPEN base_interval_cursor; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.stmt = "";
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )441;
               sqlstm.selerr = (unsigned short)1;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


               if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                  "Problem opening base_interval_cursor\n")) != OK)
               {
                  return (ERROR);
               }

               /* Fetch one of the base interval data */
               /* EXEC SQL FETCH base_interval_cursor
                  INTO :sqlBeginDate,
                       :sqlEndDate; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )456;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlstm.sqhstv[0] = (         void  *)sqlBeginDate;
               sqlstm.sqhstl[0] = (unsigned int  )21;
               sqlstm.sqhsts[0] = (         int  )0;
               sqlstm.sqindv[0] = (         void  *)0;
               sqlstm.sqinds[0] = (         int  )0;
               sqlstm.sqharm[0] = (unsigned int  )0;
               sqlstm.sqadto[0] = (unsigned short )0;
               sqlstm.sqtdso[0] = (unsigned short )0;
               sqlstm.sqhstv[1] = (         void  *)sqlEndDate;
               sqlstm.sqhstl[1] = (unsigned int  )21;
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
                   "Problem fetching base_interval_cursor\n")) != OK)
               {
                  /* EXEC SQL ROLLBACK; */ 

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


                  return (ERROR);
               }

               /* Close base_interval_cursor */
               /* EXEC SQL CLOSE base_interval_cursor; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )494;
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
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )509;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



               /* Write entry for value to r_base_update to kick off the root
                  of this derivation */
               /* Do a count to see if the record already exists */
               /* EXEC SQL
                  SELECT COUNT(*) INTO :sqlCountBase
                  FROM r_base_update 
                  WHERE site_datatype_id = :sqlBaseSDI
                     AND start_date_time = TO_DATE (:sqlBeginDate,
                                              'DD-MON-YYYY HH24:MI:SS')
                     AND end_date_time = TO_DATE (:sqlEndDate,
                                              'DD-MON-YYYY HH24:MI:SS')
                     AND interval = :sqlBaseInterval; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.stmt = "select count(*)  into :b0  from r_base_update \
where (((site_datatype_id=:b1 and start_date_time=TO_DATE(:b2,'DD-MON-YYYY HH2\
4:MI:SS')) and end_date_time=TO_DATE(:b3,'DD-MON-YYYY HH24:MI:SS')) and  inter\
val =:b4)";
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )524;
               sqlstm.selerr = (unsigned short)1;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlstm.sqhstv[0] = (         void  *)&sqlCountBase;
               sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
               sqlstm.sqhsts[0] = (         int  )0;
               sqlstm.sqindv[0] = (         void  *)0;
               sqlstm.sqinds[0] = (         int  )0;
               sqlstm.sqharm[0] = (unsigned int  )0;
               sqlstm.sqadto[0] = (unsigned short )0;
               sqlstm.sqtdso[0] = (unsigned short )0;
               sqlstm.sqhstv[1] = (         void  *)&sqlBaseSDI;
               sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
               sqlstm.sqhsts[1] = (         int  )0;
               sqlstm.sqindv[1] = (         void  *)0;
               sqlstm.sqinds[1] = (         int  )0;
               sqlstm.sqharm[1] = (unsigned int  )0;
               sqlstm.sqadto[1] = (unsigned short )0;
               sqlstm.sqtdso[1] = (unsigned short )0;
               sqlstm.sqhstv[2] = (         void  *)sqlBeginDate;
               sqlstm.sqhstl[2] = (unsigned int  )21;
               sqlstm.sqhsts[2] = (         int  )0;
               sqlstm.sqindv[2] = (         void  *)0;
               sqlstm.sqinds[2] = (         int  )0;
               sqlstm.sqharm[2] = (unsigned int  )0;
               sqlstm.sqadto[2] = (unsigned short )0;
               sqlstm.sqtdso[2] = (unsigned short )0;
               sqlstm.sqhstv[3] = (         void  *)sqlEndDate;
               sqlstm.sqhstl[3] = (unsigned int  )21;
               sqlstm.sqhsts[3] = (         int  )0;
               sqlstm.sqindv[3] = (         void  *)0;
               sqlstm.sqinds[3] = (         int  )0;
               sqlstm.sqharm[3] = (unsigned int  )0;
               sqlstm.sqadto[3] = (unsigned short )0;
               sqlstm.sqtdso[3] = (unsigned short )0;
               sqlstm.sqhstv[4] = (         void  *)sqlBaseInterval;
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
                  "Problem querying for count of data in r_base_update\n"))
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
                  sqlstm.offset = (unsigned int  )559;
                  sqlstm.cud = sqlcud0;
                  sqlstm.sqlest = (unsigned char  *)&sqlca;
                  sqlstm.sqlety = (unsigned short)256;
                  sqlstm.occurs = (unsigned int  )0;
                  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


                  return (ERROR);
               }
               /* If count is nonzero, record exists and does not need
                  to be written */
               /* If count is zero, the record needs to be inserted */
               if (sqlCountBase == 0)
               {
                  /* EXEC SQL
                     INSERT INTO r_base_update( 
                        site_datatype_id,
                        start_date_time,
                        end_date_time,
                        interval)
                     VALUES (
                        :sqlBaseSDI,
                        TO_DATE (:sqlBeginDate,
                                 'DD-MON-YYYY HH24:MI:SS'),
                        TO_DATE (:sqlEndDate,
                                 'DD-MON-YYYY HH24:MI:SS'),
                        :sqlBaseInterval); */ 

{
                  struct sqlexd sqlstm;
                  sqlstm.sqlvsn = 10;
                  sqlstm.arrsiz = 5;
                  sqlstm.sqladtp = &sqladt;
                  sqlstm.sqltdsp = &sqltds;
                  sqlstm.stmt = "insert into r_base_update (site_datatype_id\
,start_date_time,end_date_time, interval ) values (:b0,TO_DATE(:b1,'DD-MON-YYY\
Y HH24:MI:SS'),TO_DATE(:b2,'DD-MON-YYYY HH24:MI:SS'),:b3)";
                  sqlstm.iters = (unsigned int  )1;
                  sqlstm.offset = (unsigned int  )574;
                  sqlstm.cud = sqlcud0;
                  sqlstm.sqlest = (unsigned char  *)&sqlca;
                  sqlstm.sqlety = (unsigned short)256;
                  sqlstm.occurs = (unsigned int  )0;
                  sqlstm.sqhstv[0] = (         void  *)&sqlBaseSDI;
                  sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
                  sqlstm.sqhsts[0] = (         int  )0;
                  sqlstm.sqindv[0] = (         void  *)0;
                  sqlstm.sqinds[0] = (         int  )0;
                  sqlstm.sqharm[0] = (unsigned int  )0;
                  sqlstm.sqadto[0] = (unsigned short )0;
                  sqlstm.sqtdso[0] = (unsigned short )0;
                  sqlstm.sqhstv[1] = (         void  *)sqlBeginDate;
                  sqlstm.sqhstl[1] = (unsigned int  )21;
                  sqlstm.sqhsts[1] = (         int  )0;
                  sqlstm.sqindv[1] = (         void  *)0;
                  sqlstm.sqinds[1] = (         int  )0;
                  sqlstm.sqharm[1] = (unsigned int  )0;
                  sqlstm.sqadto[1] = (unsigned short )0;
                  sqlstm.sqtdso[1] = (unsigned short )0;
                  sqlstm.sqhstv[2] = (         void  *)sqlEndDate;
                  sqlstm.sqhstl[2] = (unsigned int  )21;
                  sqlstm.sqhsts[2] = (         int  )0;
                  sqlstm.sqindv[2] = (         void  *)0;
                  sqlstm.sqinds[2] = (         int  )0;
                  sqlstm.sqharm[2] = (unsigned int  )0;
                  sqlstm.sqadto[2] = (unsigned short )0;
                  sqlstm.sqtdso[2] = (unsigned short )0;
                  sqlstm.sqhstv[3] = (         void  *)sqlBaseInterval;
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
                                "Problem inserting row into r_base_update\n")
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
                     sqlstm.offset = (unsigned int  )605;
                     sqlstm.cud = sqlcud0;
                     sqlstm.sqlest = (unsigned char  *)&sqlca;
                     sqlstm.sqlety = (unsigned short)256;
                     sqlstm.occurs = (unsigned int  )0;
                     sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


                     return (ERROR);
                  } 
               }
            }

            /* In the case of there being no values in the base interval 
               within the source times, an entry needs to be written to
               r_interval_update for the overwrite slot, so that the 
               derivation gets propogated upward from the now deleted
               overwrite value.
               It was decided this should also be done for the case where
               there is base interval data in case there is an intermediate
               overwrite value between the base interval and the now deleted
               overwrite interval so that the delete change still gets 
               propagated upward */
            if ((result = SqlIntervalUpdate(sqlOverwriteRecord->interval,
                                      sqlOverwriteRecord->siteDatatypeId,
                                      sqlIntervalTime.begInterval,
                                      sqlIntervalTime.endInterval)) != OK)
            {
               FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
               return (ERROR);
            }
         }
      }

      /* Value(s) exists in r_base so represents an update or insert */
      else
      {
         /* Retrieve the overwrite value with the most recent
            date_time_loaded */
         /* EXEC SQL DECLARE overwriteData_cursor CURSOR FOR
         SELECT site_datatype_id,
                value
         FROM r_base
            WHERE site_datatype_id = :sqlOverwriteRecord->siteDatatypeId
               AND interval = :sqlOverwriteRecord->interval
               AND overwrite_flag IN ('O', 'o')
               AND start_date_time >= TO_DATE (:sqlIntervalTime.begSource,
                                   'DD-MON-YYYY HH24:MI:SS')
               AND end_date_time <= TO_DATE (:sqlIntervalTime.endSource,
                                   'DD-MON-YYYY HH24:MI:SS')
            ORDER BY date_time_loaded DESC; */ 


         if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
            "Problem declaring overwriteData_cursor\n")) != OK)
         {
            return (ERROR);
         }

         /* Open overwriteData_cursor */
         /* EXEC SQL OPEN overwriteData_cursor; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = sq0025;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )620;
         sqlstm.selerr = (unsigned short)1;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)&(sqlOverwriteRecord->siteDatatypeId);
         sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)(sqlOverwriteRecord->interval);
         sqlstm.sqhstl[1] = (unsigned int  )33;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)(sqlIntervalTime.begSource);
         sqlstm.sqhstl[2] = (unsigned int  )21;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         void  *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned int  )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (         void  *)(sqlIntervalTime.endSource);
         sqlstm.sqhstl[3] = (unsigned int  )21;
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
            "Problem opening overwriteData_cursor\n")) != OK)
         {
            return (ERROR);
         }

         /* Fetch first r_base entry with overwriteData_cursor - this is
            the one with the most recent date_time_loaded - don't
            need to load any others that may be in the active set */
         /* EXEC SQL FETCH overwriteData_cursor
            INTO :sqlDataRecord.siteDatatypeId,
                 :sqlDataRecord.value; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )651;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)&(sqlDataRecord.siteDatatypeId);
         sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)&(sqlDataRecord.value);
         sqlstm.sqhstl[1] = (unsigned int  )sizeof(double);
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
             "Problem fetching overwriteData_cursor\n")) != OK)
         {
            /* EXEC SQL ROLLBACK; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )674;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


            return (ERROR);
         }

         /* Close overwriteData_cursor */
         /* EXEC SQL CLOSE overwriteData_cursor; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )689;
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
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )704;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



         /* Assign the dateTimeLoaded member of the record to be copied
            as the run time of the derivation application */
         strcpy (sqlDataRecord.dateTimeLoaded, DATETIMELOADED);

         /* Check to see if there is a value in the destination slot */   
         sprintf(selectStatement, "SELECT COUNT(*) FROM %s WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS')", sqlIntervalTable, sqlDataRecord.siteDatatypeId, sqlIntervalTime.begInterval, sqlIntervalTime.endInterval);

         if ((result = SqlSelectCount(selectStatement, &countDestination))                  != OK)
         {
            FilePrintError(LOGFILE, "Problem querying count for destination slot\n");
            return (ERROR);
         }

         /* If a value doesn't exist in destination table, it is an insert */
         if (countDestination == 0)
         {
            /* Insert the overwrite record in the destination table */
            sprintf(sqlInsertStatement, "INSERT INTO %s (site_datatype_id, start_date_time, end_date_time, value, overwrite_flag, method_id, derivation_flags, date_time_loaded, validation) VALUES (%d, TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS'), TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS'), %f, 'O', 6, NULL, TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS'), NULL)", sqlIntervalTable, sqlDataRecord.siteDatatypeId, sqlIntervalTime.begInterval, sqlIntervalTime.endInterval, sqlDataRecord.value, sqlDataRecord.dateTimeLoaded);
            /* EXEC SQL PREPARE insert FROM :sqlInsertStatement; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )719;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)sqlInsertStatement;
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


            /* EXEC SQL EXECUTE insert; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )738;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



            if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                          "Problem inserting overwrite data record into %s\n", 
                           sqlIntervalTable)) != OK)
            {
               /* EXEC SQL ROLLBACK; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )753;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


               return (ERROR);
            }        
         }
         /* Value already exists in destination slot, so is an update */
         else
         {
            /* Update the overwrite record in the destination table */
            sprintf(sqlUpdateStatement, "UPDATE %s SET value = %f, overwrite_flag = 'O', method_id = 6, derivation_flags = NULL, date_time_loaded = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS')", sqlIntervalTable, sqlDataRecord.value, sqlDataRecord.dateTimeLoaded, sqlDataRecord.siteDatatypeId, sqlIntervalTime.begInterval, sqlIntervalTime.endInterval);
            /* EXEC SQL PREPARE upd FROM :sqlUpdateStatement; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )768;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)sqlUpdateStatement;
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
            sqlstm.offset = (unsigned int  )787;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



            if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                          "Problem updating overwrite data record in %s\n", 
                          sqlIntervalTable)) != OK)
            {
               /* EXEC SQL ROLLBACK; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )802;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


               return (ERROR);
            }                  
         }
        
         /* Write a record to r_interval_update for the inserted or
            updated record to propagate the change upward */
         if ((result = SqlIntervalUpdate(sqlOverwriteRecord->interval,
                                         sqlOverwriteRecord->siteDatatypeId,
                                         sqlIntervalTime.begInterval,
                                         sqlIntervalTime.endInterval)) != OK)
         {
            FilePrintError(LOGFILE, "Problem writing record to r_interval_update\n");
            return (ERROR);
         }

         /* If the next lower interval is still a business interval, need to
            mark as truncated any data within the source window times with
            this SDI (unless they are overwrite values)*/
         previousOrder = order - 1;
         if (previousOrder > 1)
         {
            /* Get previous interval name */
            if ((result = SqlGetIntName(previousOrder, previousInterval)) != OK)
            {
               FilePrintError(LOGFILE, "Problem finding interval name for order number %d\n", previousOrder);
               return (ERROR);
            }

            /* Construct previous interval table name */
            sprintf (sqlPreviousIntervalTable, "r_%s", previousInterval);  

            /* Mark records in previous interval */
            sprintf(sqlAddTruncateStatement, "UPDATE %s SET overwrite_flag = 'T' WHERE site_datatype_id = %d AND start_date_time >= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time <= TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND (overwrite_flag NOT IN ('O', 'o') OR overwrite_flag IS NULL)", sqlPreviousIntervalTable, sqlOverwriteRecord->siteDatatypeId, sqlIntervalTime.begSource, sqlIntervalTime.endSource );
            /* EXEC SQL PREPARE mark FROM :sqlAddTruncateStatement; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )817;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)sqlAddTruncateStatement;
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


            /* EXEC SQL EXECUTE mark; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 5;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )836;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



            /* TRUE used in second argument to SqlFilePrintErrorHandler because
               it is OK if no rows were processed above - means were no 
               records to mark in the previous interval */
            if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE, 
                          "Problem marking truncated records in %s\n", 
                          sqlPreviousIntervalTable)) != OK)
            {
               /* EXEC SQL ROLLBACK; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 5;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )851;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


               return (ERROR);
            }
         } 
      }

      /* Mark the entry in r_base_update as "ready for delete".  This entry 
         will actually get deleted at the end of the derivation run so that if 
         the program crashes in the middle, this entry can be reprocessed
         in the next run and none of the derivation procedure will get lost */
      /* EXEC SQL UPDATE r_base_update
         SET ready_for_delete = 'Y'
         WHERE site_datatype_id = :sqlOverwriteRecord->siteDatatypeId
            AND interval = :sqlOverwriteRecord->interval
            AND overwrite_flag IN ('O', 'o')
            AND start_date_time = TO_DATE (:sqlOverwriteRecord->startDateTime,
                                            'DD-MON-YYYY HH24:MI:SS')
            AND end_date_time = TO_DATE (:sqlOverwriteRecord->endDateTime,
                                            'DD-MON-YYYY HH24:MI:SS'); */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "update r_base_update  set ready_for_delete='Y' where ((\
((site_datatype_id=:b0 and  interval =:b1) and overwrite_flag in ('O','o')) an\
d start_date_time=TO_DATE(:b2,'DD-MON-YYYY HH24:MI:SS')) and end_date_time=TO_\
DATE(:b3,'DD-MON-YYYY HH24:MI:SS'))";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )866;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&(sqlOverwriteRecord->siteDatatypeId);
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)(sqlOverwriteRecord->interval);
      sqlstm.sqhstl[1] = (unsigned int  )33;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)(sqlOverwriteRecord->startDateTime);
      sqlstm.sqhstl[2] = (unsigned int  )21;
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)(sqlOverwriteRecord->endDateTime);
      sqlstm.sqhstl[3] = (unsigned int  )21;
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
                       "Problem marking overwrite as ready for delete in r_base_update\n")) != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 5;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )897;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      }         
   }

   /* Make sure all changes are committed */
   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )912;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

     

   /* Free the memory for the overwriteDataList */
   if (overwriteDataList != NULL)
   {
      free(overwriteDataList);
      overwriteDataList = NULL; 
   }

   return (OK);

}

/****************************************************************************/
#define FUNC_NAME "SqlDeleteOverwrite"

int SqlDeleteOverwrite()
{
   /* This function deletes the overwrite values from r_base_update that
      have been marked as "ready for delete" by the SqpCopyOverwrite
      function. This is run at the end of the derivation application to
      clean out overwrite values that have now been completely processed */

   int result;

   /* Delete the "ready for delete" overwrite values */

   /* EXEC SQL DELETE FROM r_base_update
      WHERE ready_for_delete = 'Y'; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "delete  from r_base_update  where ready_for_delete='Y'";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )927;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   /* TRUE used in second argument to SqlFilePrintErrorHandler because it
      is OK if no rows were processed above - means no overwrite values
      were marked for deletion */
   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE,
                 "Problem deleting overwrite records from r_base_update\n"))
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
      sqlstm.offset = (unsigned int  )942;
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
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )957;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlMethodTwo"

int SqlMethodTwo(char* statement)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      char* sqlStatement = statement;
   /* EXEC SQL END DECLARE SECTION; */ 

  
   int result;

   /* EXEC SQL PREPARE mark FROM :sqlStatement; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )972;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)sqlStatement;
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


   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                "Problem preparing statement %s\n", sqlStatement)) != OK)
   {
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )991;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
   }      

   /* EXEC SQL EXECUTE mark; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1006;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   /* TRUE used in second argument to SqlFilePrintErrorHandler because it
      is OK if no rows were processed above */
   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE,
                 "Problem executing statement %s\n", sqlStatement))
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
      sqlstm.offset = (unsigned int  )1021;
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
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1036;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   return (OK);
     
}
