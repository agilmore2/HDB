
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
           char  filnam[17];
};
static const struct sqlcxp sqlfpn =
{
    16,
    "SqlDerivation.pc"
};


static unsigned long sqlctx = 5250971;


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
            void  *sqhstv[41];
   unsigned int   sqhstl[41];
            int   sqhsts[41];
            void  *sqindv[41];
            int   sqinds[41];
   unsigned int   sqharm[41];
   unsigned int   *sqharc[41];
   unsigned short  sqadto[41];
   unsigned short  sqtdso[41];
} sqlstm = {10,41};

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

 static const char *sq0003 = 
"select site_datatype_id , interval  ,first_destination_interval ,min_value_e\
xpected ,min_value_cutoff ,max_value_expected ,max_value_cutoff ,time_offset_m\
inutes  from ref_derivation_source            ";

 static const char *sq0040 = 
"select site_datatype_id ,value  from r_base where ((((site_datatype_id=:b0 a\
nd  interval =:b1) and overwrite_flag is null ) and start_date_time>=TO_DATE(:\
b2,'DD-MON-YYYY HH24:MI:SS')) and end_date_time<=TO_DATE(:b3,'DD-MON-YYYY HH24\
:MI:SS')) order by date_time_loaded desc             ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,54,0,4,231,0,0,1,0,0,1,0,2,3,0,0,
24,0,0,2,0,0,31,239,0,0,0,0,0,1,0,
39,0,0,3,200,0,9,277,0,0,0,0,0,1,0,
54,0,0,3,0,0,13,289,0,0,8,0,0,1,0,2,3,0,0,2,5,0,0,2,5,0,0,2,4,0,0,2,4,0,0,2,4,
0,0,2,4,0,0,2,3,0,0,
101,0,0,4,0,0,31,294,0,0,0,0,0,1,0,
116,0,0,3,0,0,15,359,0,0,0,0,0,1,0,
131,0,0,5,0,0,29,360,0,0,0,0,0,1,0,
146,0,0,6,0,0,17,371,0,0,1,1,0,1,0,1,5,0,0,
165,0,0,6,0,0,21,372,0,0,0,0,0,1,0,
180,0,0,7,0,0,31,380,0,0,0,0,0,1,0,
195,0,0,8,0,0,17,416,0,0,1,1,0,1,0,1,5,0,0,
214,0,0,8,0,0,45,434,0,0,0,0,0,1,0,
229,0,0,8,0,0,13,450,0,0,4,0,0,1,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
260,0,0,9,0,0,31,459,0,0,0,0,0,1,0,
275,0,0,8,0,0,15,466,0,0,0,0,0,1,0,
290,0,0,10,0,0,29,467,0,0,0,0,0,1,0,
305,0,0,8,0,0,17,510,0,0,1,1,0,1,0,1,5,0,0,
324,0,0,8,0,0,45,528,0,0,0,0,0,1,0,
339,0,0,8,0,0,13,541,0,0,41,0,0,1,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,1,0,0,2,
3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,
2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,5,0,
0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,3,
0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,
518,0,0,11,0,0,31,547,0,0,0,0,0,1,0,
533,0,0,8,0,0,15,564,0,0,0,0,0,1,0,
548,0,0,12,0,0,29,565,0,0,0,0,0,1,0,
563,0,0,13,0,0,17,737,0,0,1,1,0,1,0,1,5,0,0,
582,0,0,13,0,0,45,756,0,0,0,0,0,1,0,
597,0,0,13,0,0,13,769,0,0,41,0,0,1,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,1,0,0,2,
3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,
2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,5,0,
0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,3,
0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,
776,0,0,14,0,0,31,775,0,0,0,0,0,1,0,
791,0,0,13,0,0,15,791,0,0,0,0,0,1,0,
806,0,0,15,0,0,29,792,0,0,0,0,0,1,0,
821,0,0,16,0,0,29,891,0,0,0,0,0,1,0,
836,0,0,17,244,0,5,900,0,0,4,4,0,1,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
867,0,0,18,0,0,31,912,0,0,0,0,0,1,0,
882,0,0,19,0,0,29,915,0,0,0,0,0,1,0,
897,0,0,20,132,0,2,919,0,0,2,2,0,1,0,1,3,0,0,1,5,0,0,
920,0,0,21,0,0,31,930,0,0,0,0,0,1,0,
935,0,0,22,0,0,29,933,0,0,0,0,0,1,0,
950,0,0,23,0,0,17,989,0,0,1,1,0,1,0,1,5,0,0,
969,0,0,24,0,0,31,993,0,0,0,0,0,1,0,
984,0,0,25,0,0,17,1000,0,0,1,1,0,1,0,1,5,0,0,
1003,0,0,26,0,0,31,1004,0,0,0,0,0,1,0,
1018,0,0,27,0,0,17,1011,0,0,1,1,0,1,0,1,5,0,0,
1037,0,0,28,0,0,31,1015,0,0,0,0,0,1,0,
1052,0,0,29,234,0,4,1041,0,0,5,4,0,1,0,2,3,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1087,0,0,30,0,0,31,1056,0,0,0,0,0,1,0,
1102,0,0,23,0,0,21,1065,0,0,2,2,0,1,0,1,5,0,0,1,5,0,0,
1125,0,0,31,0,0,31,1074,0,0,0,0,0,1,0,
1140,0,0,32,360,0,4,1084,0,0,8,4,0,1,0,2,3,0,0,2,5,0,0,2,5,0,0,2,4,0,0,1,3,0,0,
1,5,0,0,1,5,0,0,1,5,0,0,
1187,0,0,33,0,0,31,1110,0,0,0,0,0,1,0,
1202,0,0,25,0,0,21,1166,0,0,7,7,0,1,0,1,3,0,0,1,5,0,0,1,5,0,0,1,4,0,0,1,3,0,0,
1,5,0,0,1,5,0,0,
1245,0,0,34,0,0,31,1173,0,0,0,0,0,1,0,
1260,0,0,23,0,0,21,1187,0,0,2,2,0,1,0,1,5,0,0,1,5,0,0,
1283,0,0,35,0,0,31,1194,0,0,0,0,0,1,0,
1298,0,0,27,0,0,21,1201,0,0,7,7,0,1,0,1,4,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,3,0,0,
1,5,0,0,1,5,0,0,
1341,0,0,36,0,0,31,1208,0,0,0,0,0,1,0,
1356,0,0,37,236,0,4,1237,0,0,5,4,0,1,0,2,3,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1391,0,0,38,0,0,31,1252,0,0,0,0,0,1,0,
1406,0,0,23,0,0,21,1260,0,0,2,2,0,1,0,1,5,0,0,1,5,0,0,
1429,0,0,39,0,0,31,1269,0,0,0,0,0,1,0,
1444,0,0,40,285,0,9,1300,0,0,4,4,0,1,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1475,0,0,40,0,0,13,1310,0,0,2,0,0,1,0,2,3,0,0,2,4,0,0,
1498,0,0,41,0,0,31,1317,0,0,0,0,0,1,0,
1513,0,0,40,0,0,15,1322,0,0,0,0,0,1,0,
1528,0,0,42,0,0,29,1323,0,0,0,0,0,1,0,
1543,0,0,25,0,0,21,1384,0,0,7,7,0,1,0,1,3,0,0,1,5,0,0,1,5,0,0,1,4,0,0,1,3,0,0,
1,5,0,0,1,5,0,0,
1586,0,0,43,0,0,31,1391,0,0,0,0,0,1,0,
1601,0,0,23,0,0,21,1404,0,0,2,2,0,1,0,1,5,0,0,1,5,0,0,
1624,0,0,44,0,0,31,1411,0,0,0,0,0,1,0,
1639,0,0,27,0,0,21,1418,0,0,7,7,0,1,0,1,4,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,3,0,0,
1,5,0,0,1,5,0,0,
1682,0,0,45,0,0,31,1425,0,0,0,0,0,1,0,
1697,0,0,46,0,0,29,1434,0,0,0,0,0,1,0,
1712,0,0,47,78,0,4,1513,0,0,2,1,0,1,0,2,3,0,0,1,5,0,0,
1735,0,0,48,0,0,31,1522,0,0,0,0,0,1,0,
1750,0,0,49,98,0,4,1529,0,0,3,1,0,1,0,2,3,0,0,2,5,0,0,1,5,0,0,
1777,0,0,50,72,0,4,1587,0,0,2,1,0,1,0,2,5,0,0,1,5,0,0,
1800,0,0,51,78,0,4,1614,0,0,2,1,0,1,0,2,3,0,0,1,5,0,0,
1823,0,0,52,0,0,31,1623,0,0,0,0,0,1,0,
1838,0,0,53,98,0,4,1630,0,0,3,1,0,1,0,2,3,0,0,2,5,0,0,1,5,0,0,
1865,0,0,54,0,0,29,1749,0,0,0,0,0,1,0,
1880,0,0,55,72,0,4,1773,0,0,2,1,0,1,0,2,5,0,0,1,5,0,0,
1903,0,0,56,0,0,29,1815,0,0,0,0,0,1,0,
1918,0,0,57,241,0,44,1832,0,0,0,0,0,1,0,
1933,0,0,58,0,0,29,1848,0,0,0,0,0,1,0,
1948,0,0,59,28,0,1,1865,0,0,0,0,0,1,0,
1963,0,0,60,0,0,29,1873,0,0,0,0,0,1,0,
1978,0,0,8,0,0,17,2294,0,0,1,1,0,1,0,1,5,0,0,
1997,0,0,8,0,0,45,2312,0,0,0,0,0,1,0,
2012,0,0,8,0,0,13,2328,0,0,4,0,0,1,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
2043,0,0,61,0,0,31,2337,0,0,0,0,0,1,0,
2058,0,0,8,0,0,15,2344,0,0,0,0,0,1,0,
2073,0,0,62,0,0,29,2345,0,0,0,0,0,1,0,
2088,0,0,63,85,0,2,2381,0,0,1,1,0,1,0,1,5,0,0,
2107,0,0,64,0,0,29,2393,0,0,0,0,0,1,0,
};


/*****************************************************************************
* DESCRIPTION
*
* This file contains the functions that load changed r_base data from HDB
* and derive it to the interval tables based on user specifications in 
* ref_derivation_source and ref_derivation_destination.
*
***************************************
*
* SqlDerivationControl
*
* Purpose: Contains logic for copying and deriving new and changed r_base
*          data. Loads user specifications and finds the changed data in
*          r_base_update to be derived under each specification.  Calls the
*          SqlCopyBase() and DeriveList() to copy and derive the
*          data up through the interval tables.
*
* Input:   None
*
* Output:  int* - Pointer to number of r_base_update records processed
*
* Return:  int - OK or ERROR
*
***************************************
* SqlCopyBase
*
* Purpose: Copy data from the base area to the appropriate table
*
* Input:   UPDATE_RECORD** - Pointer to array of records from r_base_update
*                            to be copied  
*          int - Number of records in the array
*          REF_SOURCE* - Pointer to a ref_derivation_source specification
*      
* Output:  None
*
* Return:  int - OK or ERROR
*
***************************************
* SqlTimeInterval
*
* Purpose: Based on an update record, determines the beginning and end of the 
*          destination interval that contains the date/time of the record,
*          and the beginning and end of the source data window for this 
*          destination interval
*
* Input:   UPDATE_RECORD* - Pointer to an update record
*          SQL_INTERVAL - destination interval to calculate times for
*          REF_SOURCE* - Optional ref_derivation_source specification pointer
*                        - include only if there is a time offset in the 
*                        source specification that must be accounted for -
*                        if none required, pass in NULL
*
* Output:  INTERVAL_TIME* - Pointer to an interval time structure that the
*                           calculated times will be passed back through 
*
* Return:  int - OK or ERROR
*
***************************************
* SqlTimeShift
*
* Purpose: Takes a time record and shifts it forward or backward by one
*          interval
*
* Input:   INTERVAL_TIME* - Pointer to the existing time record
*          SQL_INTERVAL - interval by which the time should be shifted
*          DATE_MATH_TYPES - Operator (ADDITION or SUBTRACTION) indicating
*                            whether the time record is shifted forward or
*                            backward
*
* Output:  INTERVAL_TIME* - Pointer to the new time record that the shifted
*                           values will be passed back through 
*
* Return:  int - OK or ERROR
*
***************************************
* SqlCreateTable
*
* Purpose: Create the r_interval_update table for this run of the 
*          derivation application.  Acts as a semafore to prevent
*          multiple copies of the derivation app being started on
*          the same database  
*
* Input:   None
*
* Output:  None    
*
* Return:  int - OK or ERROR
*
***************************************
* SqlDropTable
*
* Purpose: Drops the r_interval_update table at the conclusion of this
*          run of the derivation app.  Frees the semafore to allow the
*          next run of the app to be started.   
*
* Input:   None
*
* Output:  None    
*
* Return:  int - OK or ERROR
*
***************************************
* LoadDestination
*
* Purpose: Moves a ref_destination spec into a structure - includes
*          checking the sql indicator variable for NULLs and loading
*          the structure appropriately based on the indicator.   
*
* Input:   REF_DESTINATION* - Pointer to ref_destination spec
*          IND_DESTINATION* - Pointer to indicator for the ref_destination spec
*
* Output:  REF_DESTINATION* - Pointer to output spec initialized appropriately
*
* Return:  int - OK or ERROR
*
***************************************
* SqlUpdateSelect
*
* Purpose: Selects update data of a specified site datatype id and interval
*          from r_interval_update 
*
* Input:   int - site datatype id to be selected
*          SQL_INTERVAL - interval to be selected
*
* Output:  int* - Pointer to an integer returning the number of records selected
*          UPDATE_RECORD** - Reference to a pointer to a list of the returned
*                            update records
*
* Return:  int - OK or ERROR
*
***************************************
* SqlRemoveOldUpdates
*
* Purpose: Removes entries from r_base_update that are over one year old. 
*          These could no longer function as the one latest residual value
*          to keep to make sure a partial calc at the longest interval (year) 
*          is made final. At this point they represent an SDI that no longer
*           has new data coming in
*
* Input:   None
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
* September, 2001
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


#define FUNC_NAME "SqlDerivationControl"

int SqlDerivationControl(int* baseUpdateTotal)
{
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      char sqlSelectStatement[1500];
      char sqlMarkDeleteStatement[600];
      IND_DESTINATION sqlIndDestination;
      IND_SOURCE sqlIndSource;
      int sqlSourceCount;
      REF_DESTINATION sqlRefDestination;
      REF_SOURCE sqlRefSource;
      UPDATE_RECORD* sqlUpdateRecord;
   /* EXEC SQL END DECLARE SECTION; */ 


   char selectStatement[600];
   int baseUpdateIndex = 0;
   int baseUpdateCount = 0;
   int compCount;
   int compIndex;
   int compOrd;
   int compSDI;
   int compUpdateCount;
   int dataOrd;
   int destCount;
   int destIndex;
   int firstDestOrd;
   int intervalLoop;
   int intUpdateCount;
   int result;
   int sourceIndex = 0;
   int TOTALNUMINT;
   int valid;
   REF_DESTINATION* compSpecList;
   REF_DESTINATION* destSpecList;
   REF_SOURCE* sourceSpecList;
   SQL_INTERVAL firstDestName;
   SQL_INTERVAL compIntName;
   SQL_INTERVAL compIntSource;
   SQL_INTERVAL updateIntName;
   SQL_INTERVAL updateIntSource;
   UPDATE_RECORD* compUpdateList;
   UPDATE_RECORD* intUpdateDataList;
   UPDATE_RECORD* updateDataList;


            if ((result = SqlGetPreviousIntName("day",
                          compIntSource,
                          &valid)) != OK)
            {
               FilePrintError(LOGFILE, "Problem finding previous interval name for interval %s\n", compIntName);
               return (ERROR);
            }

   /* Retrieve the number of intervals in HDB */
   if ((result = SqlIntervalCount(&TOTALNUMINT)) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying hdb_interval table");
      return (ERROR);
   }

   /* Load all of the ref_derivation_source entries */
   /* Find how many entries in ref_derivation_source */
   /* EXEC SQL
      SELECT COUNT(*) INTO :sqlSourceCount
      FROM ref_derivation_source; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 1;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select count(*)  into :b0  from ref_derivation_source ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )5;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&sqlSourceCount;
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
                 "Problem querying row count in ref_derivation_source\n"))
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

   /* Create array of structures to hold ref_derivation_source data */
   if (sqlSourceCount)
   {
      if ((sourceSpecList = (REF_SOURCE *) malloc 
         (sizeof(REF_SOURCE) * sqlSourceCount)) == NULL)
      {
         FilePrintError(LOGFILE, 
                        "Problem allocating memory for sourceSpecList\n");
         return (ERROR);
      }
   }
   else
   {
      sourceSpecList = NULL;
   }

   /* Declare source_cursor for selecting ref_derivation_source data */
   /* EXEC SQL DECLARE source_cursor CURSOR FOR
      SELECT site_datatype_id,
             interval,
             first_destination_interval,
             min_value_expected,
             min_value_cutoff,
             max_value_expected,
             max_value_cutoff,
             time_offset_minutes
      FROM ref_derivation_source; */ 

   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
      "Problem declaring source_cursor\n")) != OK)
   {
         return (ERROR);
   }

   /* Open source_cursor */
   /* EXEC SQL OPEN source_cursor; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 1;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = sq0003;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )39;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
      "Problem opening source_cursor\n")) != OK)
   {
      return (ERROR);
   }

   /* EXEC SQL WHENEVER NOT FOUND GOTO close_source_cursor; */ 


   /* Fetch ref_derivation_source data */
   for (sourceIndex = 0; sourceIndex < sqlSourceCount ; sourceIndex++)
   {
      /* EXEC SQL FETCH source_cursor INTO :sqlRefSource INDICATOR :sqlIndSource; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 8;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )54;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&sqlRefSource.siteDatatypeId;
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)&sqlIndSource.siteDatatypeId_ind;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)sqlRefSource.interval;
      sqlstm.sqhstl[1] = (unsigned int  )33;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)&sqlIndSource.interval_ind;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)sqlRefSource.firstDestInterval;
      sqlstm.sqhstl[2] = (unsigned int  )33;
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)&sqlIndSource.firstDestInterval_ind;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)&sqlRefSource.minValueExpected;
      sqlstm.sqhstl[3] = (unsigned int  )sizeof(double);
      sqlstm.sqhsts[3] = (         int  )0;
      sqlstm.sqindv[3] = (         void  *)&sqlIndSource.minValueExpected_ind;
      sqlstm.sqinds[3] = (         int  )0;
      sqlstm.sqharm[3] = (unsigned int  )0;
      sqlstm.sqadto[3] = (unsigned short )0;
      sqlstm.sqtdso[3] = (unsigned short )0;
      sqlstm.sqhstv[4] = (         void  *)&sqlRefSource.minValueCutoff;
      sqlstm.sqhstl[4] = (unsigned int  )sizeof(double);
      sqlstm.sqhsts[4] = (         int  )0;
      sqlstm.sqindv[4] = (         void  *)&sqlIndSource.minValueCutoff_ind;
      sqlstm.sqinds[4] = (         int  )0;
      sqlstm.sqharm[4] = (unsigned int  )0;
      sqlstm.sqadto[4] = (unsigned short )0;
      sqlstm.sqtdso[4] = (unsigned short )0;
      sqlstm.sqhstv[5] = (         void  *)&sqlRefSource.maxValueExpected;
      sqlstm.sqhstl[5] = (unsigned int  )sizeof(double);
      sqlstm.sqhsts[5] = (         int  )0;
      sqlstm.sqindv[5] = (         void  *)&sqlIndSource.maxValueExpected_ind;
      sqlstm.sqinds[5] = (         int  )0;
      sqlstm.sqharm[5] = (unsigned int  )0;
      sqlstm.sqadto[5] = (unsigned short )0;
      sqlstm.sqtdso[5] = (unsigned short )0;
      sqlstm.sqhstv[6] = (         void  *)&sqlRefSource.maxValueCutoff;
      sqlstm.sqhstl[6] = (unsigned int  )sizeof(double);
      sqlstm.sqhsts[6] = (         int  )0;
      sqlstm.sqindv[6] = (         void  *)&sqlIndSource.maxValueCutoff_ind;
      sqlstm.sqinds[6] = (         int  )0;
      sqlstm.sqharm[6] = (unsigned int  )0;
      sqlstm.sqadto[6] = (unsigned short )0;
      sqlstm.sqtdso[6] = (unsigned short )0;
      sqlstm.sqhstv[7] = (         void  *)&sqlRefSource.timeOffsetMinutes;
      sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[7] = (         int  )0;
      sqlstm.sqindv[7] = (         void  *)&sqlIndSource.timeOffset_ind;
      sqlstm.sqinds[7] = (         int  )0;
      sqlstm.sqharm[7] = (unsigned int  )0;
      sqlstm.sqadto[7] = (unsigned short )0;
      sqlstm.sqtdso[7] = (unsigned short )0;
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
      if (sqlca.sqlcode == 1403) goto close_source_cursor;
}



      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
         "Problem fetching source_cursor\n")) != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 8;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )101;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      }

      /* Load current element of sourceSpecList - check for NULLs */
      sourceSpecList[sourceIndex].siteDatatypeId = sqlRefSource.siteDatatypeId;
      strcpy (sourceSpecList[sourceIndex].interval, sqlRefSource.interval);
      if (sqlIndSource.firstDestInterval_ind == -1)
      {
         strcpy (sourceSpecList[sourceIndex].firstDestInterval, EMPTY_STRING);
      }
      else
      {
         strcpy (sourceSpecList[sourceIndex].firstDestInterval, 
                 sqlRefSource.firstDestInterval);
      }
      if (sqlIndSource.minValueExpected_ind == -1)
      {
         sourceSpecList[sourceIndex].minValueExpected = VALUE_NULL;
      }
      else
      {
         sourceSpecList[sourceIndex].minValueExpected = 
                                     sqlRefSource.minValueExpected;
      }
      if (sqlIndSource.minValueCutoff_ind == -1)
      {
         sourceSpecList[sourceIndex].minValueCutoff = VALUE_NULL;
      }
      else
      {
         sourceSpecList[sourceIndex].minValueCutoff = 
                                     sqlRefSource.minValueCutoff;
      }
      if (sqlIndSource.maxValueExpected_ind == -1)
      {
         sourceSpecList[sourceIndex].maxValueExpected = VALUE_NULL;
      }
      else
      {
         sourceSpecList[sourceIndex].maxValueExpected = 
                                     sqlRefSource.maxValueExpected;
      }
      if (sqlIndSource.maxValueCutoff_ind == -1)
      {
         sourceSpecList[sourceIndex].maxValueCutoff = VALUE_NULL;
      }
      else
      {
         sourceSpecList[sourceIndex].maxValueCutoff = 
                                     sqlRefSource.maxValueCutoff;
      }
      if (sqlIndSource.timeOffset_ind == -1)
      {
         sourceSpecList[sourceIndex].timeOffsetMinutes = VALUE_NULL;
      }
      else
      {
         sourceSpecList[sourceIndex].timeOffsetMinutes = 
                                     sqlRefSource.timeOffsetMinutes;
      }
   }

   /* Close source cursor */
   close_source_cursor:
   /* EXEC SQL CLOSE source_cursor; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 8;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )116;
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
   sqlstm.arrsiz = 8;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )131;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   /* EXEC SQL WHENEVER NOT FOUND CONTINUE; */ 


   /* Loop for each ref_derivation_source entry */
   for (sourceIndex = 0; sourceIndex < sqlSourceCount; sourceIndex++)
   {
      /* Mark the rows with this SDI in r_base_update to tag ones we are going
         to process - if one gets updated while we are processing, the tag 
         gets removed by the update trigger so we don't later delete that row.
         It then gets processed by the next run of the app */
      sprintf(sqlMarkDeleteStatement, "UPDATE r_base_update SET ready_for_delete = 'Y' WHERE site_datatype_id = %d AND interval = '%s' AND overwrite_flag IS NULL", sourceSpecList[sourceIndex].siteDatatypeId, sourceSpecList[sourceIndex].interval);
      /* EXEC SQL PREPARE mark FROM :sqlMarkDeleteStatement; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 8;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )146;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)sqlMarkDeleteStatement;
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
      sqlstm.arrsiz = 8;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )165;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



      /* TRUE used in second argument to SqlFilePrintErrorHandler because
         it is OK if no rows were processed above - means were no 
         records to mark for this SDI */
      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE, 
                    "Problem marking nonoverwrite records in r_base_update as ready for delete\n")) != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 8;
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

      /* Load rows from r_base_update related to ref_derivation_source spec */
      /* Find out how many applicable rows in r_base_update */
      sprintf(selectStatement, "SELECT COUNT(*) FROM r_base_update WHERE site_datatype_id = %d AND interval = '%s' AND overwrite_flag IS NULL", sourceSpecList[sourceIndex].siteDatatypeId, sourceSpecList[sourceIndex].interval);

      if ((result = SqlSelectCount(selectStatement, &baseUpdateCount)) != OK)
      {
         FilePrintError(LOGFILE,
                        "Problem querying row count in r_base_update\n");
         return (ERROR);
      }

      *baseUpdateTotal = *baseUpdateTotal + baseUpdateCount;

      /* Create array of structures to hold r_base_update data */
      if (baseUpdateCount)
      {
         if ((updateDataList = (UPDATE_RECORD *) malloc 
            (sizeof(UPDATE_RECORD) * (baseUpdateCount))) == NULL)
         {
             FilePrintError(LOGFILE,
                            "Problem allocating memory for updateDataList\n");
             return (ERROR);
         }
      }
      else
      {
         updateDataList = NULL;
      }

      /* Prepare the select statement for r_base_update data*/
      sprintf (sqlSelectStatement, "SELECT site_datatype_id, TO_CHAR (start_date_time, 'DD-MON-YYYY HH24:MI:SS'), TO_CHAR (end_date_time, 'DD-MON-YYYY HH24:MI:SS'), interval FROM r_base_update WHERE site_datatype_id = %d AND interval = '%s' AND overwrite_flag IS NULL ORDER BY start_date_time", sourceSpecList[sourceIndex].siteDatatypeId, sourceSpecList[sourceIndex].interval);

      /* EXEC SQL PREPARE sel FROM :sqlSelectStatement; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 8;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )195;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)sqlSelectStatement;
      sqlstm.sqhstl[0] = (unsigned int  )1500;
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

      /* Declare base_update_cursor for selecting r_base_update data */
      /* EXEC SQL DECLARE base_update_cursor CURSOR FOR sel; */ 

      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                    "Problem declaring base_update_cursor for: %s...\n",
                    sqlSelectStatement)) != OK)
      {
         return (ERROR);
      }

      /* Open base_update_cursor */
      /* EXEC SQL OPEN base_update_cursor; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 8;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )214;
      sqlstm.selerr = (unsigned short)1;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                    "Problem opening base_update_cursor for\n: %s...\n",
                    sqlSelectStatement)) != OK)
      {
         return (ERROR);
      }

      /* EXEC SQL WHENEVER NOT FOUND GOTO close_base_update_cursor; */ 


      /* Fetch applicable r_base_update data */ 
      for (baseUpdateIndex = 0; baseUpdateIndex < baseUpdateCount ;
           baseUpdateIndex++)
      {
         sqlUpdateRecord = &updateDataList[baseUpdateIndex];

         /* EXEC SQL FETCH base_update_cursor INTO
                                :sqlUpdateRecord->siteDatatypeId, 
                                :sqlUpdateRecord->startDateTime,
                                :sqlUpdateRecord->endDateTime,
                                :sqlUpdateRecord->interval; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 8;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )229;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)&(sqlUpdateRecord->siteDatatypeId);
         sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)(sqlUpdateRecord->startDateTime);
         sqlstm.sqhstl[1] = (unsigned int  )21;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)(sqlUpdateRecord->endDateTime);
         sqlstm.sqhstl[2] = (unsigned int  )21;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         void  *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned int  )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (         void  *)(sqlUpdateRecord->interval);
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
         if (sqlca.sqlcode == 1403) goto close_base_update_cursor;
}


         if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
            "Problem fetching base_update_cursor for\n: %s...\n",
                    sqlSelectStatement)) != OK)
         {
            /* EXEC SQL ROLLBACK; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 8;
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

      /* Close base_update_cursor */
      close_base_update_cursor:
      /* EXEC SQL CLOSE base_update_cursor; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 8;
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


      /* EXEC SQL COMMIT; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 8;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )290;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      /* EXEC SQL WHENEVER NOT FOUND CONTINUE; */ 


      /* Call to SqlCopyBase to copy base values to corresponding tables */
      if ((result = SqlCopyBase(updateDataList, baseUpdateCount,
                    &sourceSpecList[sourceIndex])) != OK)
      {
         FilePrintError(LOGFILE,
                        "Problem copying r_base values to corresponding tables\n");
         return (ERROR);
      }

      /* Load all noncompounded ref_derivation_destination entries that 
         are specified for the current ref_derivation_source */
      /* Find how many relevant entries are in ref_derivation_destination */
      sprintf(selectStatement, "SELECT COUNT(*) FROM ref_derivation_destination WHERE base_site_datatype_id = %d AND compounding_source_sdi IS NULL", sourceSpecList[sourceIndex].siteDatatypeId);

      if ((result = SqlSelectCount(selectStatement, &destCount)) != OK)
      {
         FilePrintError(LOGFILE,
                        "Problem querying row count in ref_derivation_destination\n");
         return (ERROR);
      }

      /* Create array of structures to hold ref_derivation_destination data */
      if (destCount)
      {
         if ((destSpecList = (REF_DESTINATION *) malloc 
            (sizeof(REF_DESTINATION) * destCount)) == NULL)
         {
            FilePrintError(LOGFILE,
                           "Problem allocating memory for destSpecList\n");
            return (ERROR);
         }
      }
      else
      {
         destSpecList = NULL;
      }

      /* Prepare the select statement for ref_derivation_destination data*/
      sprintf (sqlSelectStatement, "SELECT r.base_site_datatype_id, r.compounding_source_sdi, r.compounding_source_interval, r.dest_site_datatype_id, r.partial_calc, r.method_id, r.hr_desired_eop_window, r.hr_required_eop_window, r.hr_desired_bop_window, r.hr_required_bop_window, r.hr_window_unit, r.hr_desired_number_source, r.hr_required_number_source, r.day_desired_eop_window, r.day_required_eop_window, r.day_desired_bop_window, r.day_required_bop_window, r.day_window_unit, r.day_desired_number_source, r.day_required_number_source, r.mon_desired_eop_window, r.mon_required_eop_window, r.mon_desired_bop_window, r.mon_required_bop_window, r.mon_window_unit, r.mon_desired_number_source, r.mon_required_number_source, r.yr_desired_eop_window, r.yr_required_eop_window, r.yr_desired_bop_window, r.yr_required_bop_window, r.yr_window_unit, r.yr_desired_number_source, r.yr_required_number_source, r.wy_desired_eop_window, r.wy_required_eop_window, r.wy_desired_bop_window, r.wy_required_bop_window, r.wy_window_unit, r.wy_desired_number_source, r.wy_required_number_source FROM ref_derivation_destination r WHERE r.base_site_datatype_id = %d AND r.compounding_source_sdi IS NULL ", sourceSpecList[sourceIndex].siteDatatypeId);

      /* EXEC SQL PREPARE sel FROM :sqlSelectStatement; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 8;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )305;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)sqlSelectStatement;
      sqlstm.sqhstl[0] = (unsigned int  )1500;
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

      /* Declare dest_cursor for selecting ref_derivation_destination data */
      /* EXEC SQL DECLARE dest_cursor CURSOR FOR sel; */ 

      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                    "Problem declaring dest_cursor for: %s...\n",
                    sqlSelectStatement)) != OK)
      {
         return (ERROR);
      }

      /* Open dest_cursor */
      /* EXEC SQL OPEN dest_cursor; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 8;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )324;
      sqlstm.selerr = (unsigned short)1;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                    "Problem opening dest_cursor for\n: %s...\n",
                    sqlSelectStatement)) != OK)
      {
         return (ERROR);
      }

      /* EXEC SQL WHENEVER NOT FOUND GOTO close_dest_cursor; */ 


      /* Fetch applicable ref_derivation_destination data */ 
      for (destIndex = 0; destIndex < destCount ; destIndex++)
      {
         /* EXEC SQL FETCH dest_cursor INTO :sqlRefDestination INDICATOR :sqlIndDestination; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )339;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)&sqlRefDestination.baseSDI;
         sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)&sqlIndDestination.baseSDI_ind;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)&sqlRefDestination.compoundingSourceSDI;
         sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)&sqlIndDestination.compoundingSourceSDI_ind;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)sqlRefDestination.compoundingSourceInterval;
         sqlstm.sqhstl[2] = (unsigned int  )33;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         void  *)&sqlIndDestination.compoundingSourceInterval_ind;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned int  )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (         void  *)&sqlRefDestination.destSDI;
         sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[3] = (         int  )0;
         sqlstm.sqindv[3] = (         void  *)&sqlIndDestination.destSDI_ind;
         sqlstm.sqinds[3] = (         int  )0;
         sqlstm.sqharm[3] = (unsigned int  )0;
         sqlstm.sqadto[3] = (unsigned short )0;
         sqlstm.sqtdso[3] = (unsigned short )0;
         sqlstm.sqhstv[4] = (         void  *)&sqlRefDestination.partialCalc;
         sqlstm.sqhstl[4] = (unsigned int  )1;
         sqlstm.sqhsts[4] = (         int  )0;
         sqlstm.sqindv[4] = (         void  *)&sqlIndDestination.partialCalc_ind;
         sqlstm.sqinds[4] = (         int  )0;
         sqlstm.sqharm[4] = (unsigned int  )0;
         sqlstm.sqadto[4] = (unsigned short )0;
         sqlstm.sqtdso[4] = (unsigned short )0;
         sqlstm.sqhstv[5] = (         void  *)&sqlRefDestination.methodId;
         sqlstm.sqhstl[5] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[5] = (         int  )0;
         sqlstm.sqindv[5] = (         void  *)&sqlIndDestination.methodId;
         sqlstm.sqinds[5] = (         int  )0;
         sqlstm.sqharm[5] = (unsigned int  )0;
         sqlstm.sqadto[5] = (unsigned short )0;
         sqlstm.sqtdso[5] = (unsigned short )0;
         sqlstm.sqhstv[6] = (         void  *)&sqlRefDestination.hrDesiredEopWindow;
         sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[6] = (         int  )0;
         sqlstm.sqindv[6] = (         void  *)&sqlIndDestination.hrDesiredEopWindow_ind;
         sqlstm.sqinds[6] = (         int  )0;
         sqlstm.sqharm[6] = (unsigned int  )0;
         sqlstm.sqadto[6] = (unsigned short )0;
         sqlstm.sqtdso[6] = (unsigned short )0;
         sqlstm.sqhstv[7] = (         void  *)&sqlRefDestination.hrRequiredEopWindow;
         sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[7] = (         int  )0;
         sqlstm.sqindv[7] = (         void  *)&sqlIndDestination.hrRequiredEopWindow_ind;
         sqlstm.sqinds[7] = (         int  )0;
         sqlstm.sqharm[7] = (unsigned int  )0;
         sqlstm.sqadto[7] = (unsigned short )0;
         sqlstm.sqtdso[7] = (unsigned short )0;
         sqlstm.sqhstv[8] = (         void  *)&sqlRefDestination.hrDesiredBopWindow;
         sqlstm.sqhstl[8] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[8] = (         int  )0;
         sqlstm.sqindv[8] = (         void  *)&sqlIndDestination.hrDesiredBopWindow_ind;
         sqlstm.sqinds[8] = (         int  )0;
         sqlstm.sqharm[8] = (unsigned int  )0;
         sqlstm.sqadto[8] = (unsigned short )0;
         sqlstm.sqtdso[8] = (unsigned short )0;
         sqlstm.sqhstv[9] = (         void  *)&sqlRefDestination.hrRequiredBopWindow;
         sqlstm.sqhstl[9] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[9] = (         int  )0;
         sqlstm.sqindv[9] = (         void  *)&sqlIndDestination.hrRequiredBopWindow_ind;
         sqlstm.sqinds[9] = (         int  )0;
         sqlstm.sqharm[9] = (unsigned int  )0;
         sqlstm.sqadto[9] = (unsigned short )0;
         sqlstm.sqtdso[9] = (unsigned short )0;
         sqlstm.sqhstv[10] = (         void  *)sqlRefDestination.hrWindowUnit;
         sqlstm.sqhstl[10] = (unsigned int  )10;
         sqlstm.sqhsts[10] = (         int  )0;
         sqlstm.sqindv[10] = (         void  *)&sqlIndDestination.hrWindowUnit_ind;
         sqlstm.sqinds[10] = (         int  )0;
         sqlstm.sqharm[10] = (unsigned int  )0;
         sqlstm.sqadto[10] = (unsigned short )0;
         sqlstm.sqtdso[10] = (unsigned short )0;
         sqlstm.sqhstv[11] = (         void  *)&sqlRefDestination.hrDesiredNumSource;
         sqlstm.sqhstl[11] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[11] = (         int  )0;
         sqlstm.sqindv[11] = (         void  *)&sqlIndDestination.hrDesiredNumSource_ind;
         sqlstm.sqinds[11] = (         int  )0;
         sqlstm.sqharm[11] = (unsigned int  )0;
         sqlstm.sqadto[11] = (unsigned short )0;
         sqlstm.sqtdso[11] = (unsigned short )0;
         sqlstm.sqhstv[12] = (         void  *)&sqlRefDestination.hrRequiredNumSource;
         sqlstm.sqhstl[12] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[12] = (         int  )0;
         sqlstm.sqindv[12] = (         void  *)&sqlIndDestination.hrRequiredNumSource_ind;
         sqlstm.sqinds[12] = (         int  )0;
         sqlstm.sqharm[12] = (unsigned int  )0;
         sqlstm.sqadto[12] = (unsigned short )0;
         sqlstm.sqtdso[12] = (unsigned short )0;
         sqlstm.sqhstv[13] = (         void  *)&sqlRefDestination.dayDesiredEopWindow;
         sqlstm.sqhstl[13] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[13] = (         int  )0;
         sqlstm.sqindv[13] = (         void  *)&sqlIndDestination.dayDesiredEopWindow_ind;
         sqlstm.sqinds[13] = (         int  )0;
         sqlstm.sqharm[13] = (unsigned int  )0;
         sqlstm.sqadto[13] = (unsigned short )0;
         sqlstm.sqtdso[13] = (unsigned short )0;
         sqlstm.sqhstv[14] = (         void  *)&sqlRefDestination.dayRequiredEopWindow;
         sqlstm.sqhstl[14] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[14] = (         int  )0;
         sqlstm.sqindv[14] = (         void  *)&sqlIndDestination.dayRequiredEopWindow_ind;
         sqlstm.sqinds[14] = (         int  )0;
         sqlstm.sqharm[14] = (unsigned int  )0;
         sqlstm.sqadto[14] = (unsigned short )0;
         sqlstm.sqtdso[14] = (unsigned short )0;
         sqlstm.sqhstv[15] = (         void  *)&sqlRefDestination.dayDesiredBopWindow;
         sqlstm.sqhstl[15] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[15] = (         int  )0;
         sqlstm.sqindv[15] = (         void  *)&sqlIndDestination.dayDesiredBopWindow_ind;
         sqlstm.sqinds[15] = (         int  )0;
         sqlstm.sqharm[15] = (unsigned int  )0;
         sqlstm.sqadto[15] = (unsigned short )0;
         sqlstm.sqtdso[15] = (unsigned short )0;
         sqlstm.sqhstv[16] = (         void  *)&sqlRefDestination.dayRequiredBopWindow;
         sqlstm.sqhstl[16] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[16] = (         int  )0;
         sqlstm.sqindv[16] = (         void  *)&sqlIndDestination.dayRequiredBopWindow_ind;
         sqlstm.sqinds[16] = (         int  )0;
         sqlstm.sqharm[16] = (unsigned int  )0;
         sqlstm.sqadto[16] = (unsigned short )0;
         sqlstm.sqtdso[16] = (unsigned short )0;
         sqlstm.sqhstv[17] = (         void  *)sqlRefDestination.dayWindowUnit;
         sqlstm.sqhstl[17] = (unsigned int  )10;
         sqlstm.sqhsts[17] = (         int  )0;
         sqlstm.sqindv[17] = (         void  *)&sqlIndDestination.dayWindowUnit_ind;
         sqlstm.sqinds[17] = (         int  )0;
         sqlstm.sqharm[17] = (unsigned int  )0;
         sqlstm.sqadto[17] = (unsigned short )0;
         sqlstm.sqtdso[17] = (unsigned short )0;
         sqlstm.sqhstv[18] = (         void  *)&sqlRefDestination.dayDesiredNumSource;
         sqlstm.sqhstl[18] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[18] = (         int  )0;
         sqlstm.sqindv[18] = (         void  *)&sqlIndDestination.dayDesiredNumSource_ind;
         sqlstm.sqinds[18] = (         int  )0;
         sqlstm.sqharm[18] = (unsigned int  )0;
         sqlstm.sqadto[18] = (unsigned short )0;
         sqlstm.sqtdso[18] = (unsigned short )0;
         sqlstm.sqhstv[19] = (         void  *)&sqlRefDestination.dayRequiredNumSource;
         sqlstm.sqhstl[19] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[19] = (         int  )0;
         sqlstm.sqindv[19] = (         void  *)&sqlIndDestination.dayRequiredNumSource_ind;
         sqlstm.sqinds[19] = (         int  )0;
         sqlstm.sqharm[19] = (unsigned int  )0;
         sqlstm.sqadto[19] = (unsigned short )0;
         sqlstm.sqtdso[19] = (unsigned short )0;
         sqlstm.sqhstv[20] = (         void  *)&sqlRefDestination.monDesiredEopWindow;
         sqlstm.sqhstl[20] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[20] = (         int  )0;
         sqlstm.sqindv[20] = (         void  *)&sqlIndDestination.monDesiredEopWindow_ind;
         sqlstm.sqinds[20] = (         int  )0;
         sqlstm.sqharm[20] = (unsigned int  )0;
         sqlstm.sqadto[20] = (unsigned short )0;
         sqlstm.sqtdso[20] = (unsigned short )0;
         sqlstm.sqhstv[21] = (         void  *)&sqlRefDestination.monRequiredEopWindow;
         sqlstm.sqhstl[21] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[21] = (         int  )0;
         sqlstm.sqindv[21] = (         void  *)&sqlIndDestination.monRequiredEopWindow_ind;
         sqlstm.sqinds[21] = (         int  )0;
         sqlstm.sqharm[21] = (unsigned int  )0;
         sqlstm.sqadto[21] = (unsigned short )0;
         sqlstm.sqtdso[21] = (unsigned short )0;
         sqlstm.sqhstv[22] = (         void  *)&sqlRefDestination.monDesiredBopWindow;
         sqlstm.sqhstl[22] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[22] = (         int  )0;
         sqlstm.sqindv[22] = (         void  *)&sqlIndDestination.monDesiredBopWindow_ind;
         sqlstm.sqinds[22] = (         int  )0;
         sqlstm.sqharm[22] = (unsigned int  )0;
         sqlstm.sqadto[22] = (unsigned short )0;
         sqlstm.sqtdso[22] = (unsigned short )0;
         sqlstm.sqhstv[23] = (         void  *)&sqlRefDestination.monRequiredBopWindow;
         sqlstm.sqhstl[23] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[23] = (         int  )0;
         sqlstm.sqindv[23] = (         void  *)&sqlIndDestination.monRequiredBopWindow_ind;
         sqlstm.sqinds[23] = (         int  )0;
         sqlstm.sqharm[23] = (unsigned int  )0;
         sqlstm.sqadto[23] = (unsigned short )0;
         sqlstm.sqtdso[23] = (unsigned short )0;
         sqlstm.sqhstv[24] = (         void  *)sqlRefDestination.monWindowUnit;
         sqlstm.sqhstl[24] = (unsigned int  )10;
         sqlstm.sqhsts[24] = (         int  )0;
         sqlstm.sqindv[24] = (         void  *)&sqlIndDestination.monWindowUnit_ind;
         sqlstm.sqinds[24] = (         int  )0;
         sqlstm.sqharm[24] = (unsigned int  )0;
         sqlstm.sqadto[24] = (unsigned short )0;
         sqlstm.sqtdso[24] = (unsigned short )0;
         sqlstm.sqhstv[25] = (         void  *)&sqlRefDestination.monDesiredNumSource;
         sqlstm.sqhstl[25] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[25] = (         int  )0;
         sqlstm.sqindv[25] = (         void  *)&sqlIndDestination.monDesiredNumSource_ind;
         sqlstm.sqinds[25] = (         int  )0;
         sqlstm.sqharm[25] = (unsigned int  )0;
         sqlstm.sqadto[25] = (unsigned short )0;
         sqlstm.sqtdso[25] = (unsigned short )0;
         sqlstm.sqhstv[26] = (         void  *)&sqlRefDestination.monRequiredNumSource;
         sqlstm.sqhstl[26] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[26] = (         int  )0;
         sqlstm.sqindv[26] = (         void  *)&sqlIndDestination.monRequiredNumSource_ind;
         sqlstm.sqinds[26] = (         int  )0;
         sqlstm.sqharm[26] = (unsigned int  )0;
         sqlstm.sqadto[26] = (unsigned short )0;
         sqlstm.sqtdso[26] = (unsigned short )0;
         sqlstm.sqhstv[27] = (         void  *)&sqlRefDestination.yrDesiredEopWindow;
         sqlstm.sqhstl[27] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[27] = (         int  )0;
         sqlstm.sqindv[27] = (         void  *)&sqlIndDestination.yrDesiredEopWindow_ind;
         sqlstm.sqinds[27] = (         int  )0;
         sqlstm.sqharm[27] = (unsigned int  )0;
         sqlstm.sqadto[27] = (unsigned short )0;
         sqlstm.sqtdso[27] = (unsigned short )0;
         sqlstm.sqhstv[28] = (         void  *)&sqlRefDestination.yrRequiredEopWindow;
         sqlstm.sqhstl[28] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[28] = (         int  )0;
         sqlstm.sqindv[28] = (         void  *)&sqlIndDestination.yrRequiredEopWindow_ind;
         sqlstm.sqinds[28] = (         int  )0;
         sqlstm.sqharm[28] = (unsigned int  )0;
         sqlstm.sqadto[28] = (unsigned short )0;
         sqlstm.sqtdso[28] = (unsigned short )0;
         sqlstm.sqhstv[29] = (         void  *)&sqlRefDestination.yrDesiredBopWindow;
         sqlstm.sqhstl[29] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[29] = (         int  )0;
         sqlstm.sqindv[29] = (         void  *)&sqlIndDestination.yrDesiredBopWindow_ind;
         sqlstm.sqinds[29] = (         int  )0;
         sqlstm.sqharm[29] = (unsigned int  )0;
         sqlstm.sqadto[29] = (unsigned short )0;
         sqlstm.sqtdso[29] = (unsigned short )0;
         sqlstm.sqhstv[30] = (         void  *)&sqlRefDestination.yrRequiredBopWindow;
         sqlstm.sqhstl[30] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[30] = (         int  )0;
         sqlstm.sqindv[30] = (         void  *)&sqlIndDestination.yrRequiredBopWindow_ind;
         sqlstm.sqinds[30] = (         int  )0;
         sqlstm.sqharm[30] = (unsigned int  )0;
         sqlstm.sqadto[30] = (unsigned short )0;
         sqlstm.sqtdso[30] = (unsigned short )0;
         sqlstm.sqhstv[31] = (         void  *)sqlRefDestination.yrWindowUnit;
         sqlstm.sqhstl[31] = (unsigned int  )10;
         sqlstm.sqhsts[31] = (         int  )0;
         sqlstm.sqindv[31] = (         void  *)&sqlIndDestination.yrWindowUnit_ind;
         sqlstm.sqinds[31] = (         int  )0;
         sqlstm.sqharm[31] = (unsigned int  )0;
         sqlstm.sqadto[31] = (unsigned short )0;
         sqlstm.sqtdso[31] = (unsigned short )0;
         sqlstm.sqhstv[32] = (         void  *)&sqlRefDestination.yrDesiredNumSource;
         sqlstm.sqhstl[32] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[32] = (         int  )0;
         sqlstm.sqindv[32] = (         void  *)&sqlIndDestination.yrDesiredNumSource_ind;
         sqlstm.sqinds[32] = (         int  )0;
         sqlstm.sqharm[32] = (unsigned int  )0;
         sqlstm.sqadto[32] = (unsigned short )0;
         sqlstm.sqtdso[32] = (unsigned short )0;
         sqlstm.sqhstv[33] = (         void  *)&sqlRefDestination.yrRequiredNumSource;
         sqlstm.sqhstl[33] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[33] = (         int  )0;
         sqlstm.sqindv[33] = (         void  *)&sqlIndDestination.yrRequiredNumSource_ind;
         sqlstm.sqinds[33] = (         int  )0;
         sqlstm.sqharm[33] = (unsigned int  )0;
         sqlstm.sqadto[33] = (unsigned short )0;
         sqlstm.sqtdso[33] = (unsigned short )0;
         sqlstm.sqhstv[34] = (         void  *)&sqlRefDestination.wyDesiredEopWindow;
         sqlstm.sqhstl[34] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[34] = (         int  )0;
         sqlstm.sqindv[34] = (         void  *)&sqlIndDestination.wyDesiredEopWindow_ind;
         sqlstm.sqinds[34] = (         int  )0;
         sqlstm.sqharm[34] = (unsigned int  )0;
         sqlstm.sqadto[34] = (unsigned short )0;
         sqlstm.sqtdso[34] = (unsigned short )0;
         sqlstm.sqhstv[35] = (         void  *)&sqlRefDestination.wyRequiredEopWindow;
         sqlstm.sqhstl[35] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[35] = (         int  )0;
         sqlstm.sqindv[35] = (         void  *)&sqlIndDestination.wyRequiredEopWindow_ind;
         sqlstm.sqinds[35] = (         int  )0;
         sqlstm.sqharm[35] = (unsigned int  )0;
         sqlstm.sqadto[35] = (unsigned short )0;
         sqlstm.sqtdso[35] = (unsigned short )0;
         sqlstm.sqhstv[36] = (         void  *)&sqlRefDestination.wyDesiredBopWindow;
         sqlstm.sqhstl[36] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[36] = (         int  )0;
         sqlstm.sqindv[36] = (         void  *)&sqlIndDestination.wyDesiredBopWindow_ind;
         sqlstm.sqinds[36] = (         int  )0;
         sqlstm.sqharm[36] = (unsigned int  )0;
         sqlstm.sqadto[36] = (unsigned short )0;
         sqlstm.sqtdso[36] = (unsigned short )0;
         sqlstm.sqhstv[37] = (         void  *)&sqlRefDestination.wyRequiredBopWindow;
         sqlstm.sqhstl[37] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[37] = (         int  )0;
         sqlstm.sqindv[37] = (         void  *)&sqlIndDestination.wyRequiredBopWindow_ind;
         sqlstm.sqinds[37] = (         int  )0;
         sqlstm.sqharm[37] = (unsigned int  )0;
         sqlstm.sqadto[37] = (unsigned short )0;
         sqlstm.sqtdso[37] = (unsigned short )0;
         sqlstm.sqhstv[38] = (         void  *)sqlRefDestination.wyWindowUnit;
         sqlstm.sqhstl[38] = (unsigned int  )10;
         sqlstm.sqhsts[38] = (         int  )0;
         sqlstm.sqindv[38] = (         void  *)&sqlIndDestination.wyWindowUnit_ind;
         sqlstm.sqinds[38] = (         int  )0;
         sqlstm.sqharm[38] = (unsigned int  )0;
         sqlstm.sqadto[38] = (unsigned short )0;
         sqlstm.sqtdso[38] = (unsigned short )0;
         sqlstm.sqhstv[39] = (         void  *)&sqlRefDestination.wyDesiredNumSource;
         sqlstm.sqhstl[39] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[39] = (         int  )0;
         sqlstm.sqindv[39] = (         void  *)&sqlIndDestination.wyDesiredNumSource_ind;
         sqlstm.sqinds[39] = (         int  )0;
         sqlstm.sqharm[39] = (unsigned int  )0;
         sqlstm.sqadto[39] = (unsigned short )0;
         sqlstm.sqtdso[39] = (unsigned short )0;
         sqlstm.sqhstv[40] = (         void  *)&sqlRefDestination.wyRequiredNumSource;
         sqlstm.sqhstl[40] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[40] = (         int  )0;
         sqlstm.sqindv[40] = (         void  *)&sqlIndDestination.wyRequiredNumSource_ind;
         sqlstm.sqinds[40] = (         int  )0;
         sqlstm.sqharm[40] = (unsigned int  )0;
         sqlstm.sqadto[40] = (unsigned short )0;
         sqlstm.sqtdso[40] = (unsigned short )0;
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
         if (sqlca.sqlcode == 1403) goto close_dest_cursor;
}

 

         if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
            "Problem fetching dest_cursor for\n: %s...\n",
                    sqlSelectStatement)) != OK)
         {
            /* EXEC SQL ROLLBACK; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )518;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


            return (ERROR);
         }

         if ((result = LoadDestination(
                       &sqlRefDestination,
                       &sqlIndDestination,
                       &destSpecList[destIndex])) != OK)
         {
            FilePrintError(LOGFILE,
                           "Problem loading destination specification.");
            return (ERROR);
         }
      }

      /* Close dest_cursor */
      close_dest_cursor:
      /* EXEC SQL CLOSE dest_cursor; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )533;
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
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )548;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      /* EXEC SQL WHENEVER NOT FOUND CONTINUE; */ 



            if ((result = SqlGetPreviousIntName("day",
                          compIntSource,
                          &valid)) != OK)
            {
               FilePrintError(LOGFILE, "Problem finding previous interval name for interval %s\n", compIntName);
               return (ERROR);
            }

      /* Determine order number for the interval of the data */
      if ((result = SqlGetIntOrder(
                    sourceSpecList[sourceIndex].interval,
                    &dataOrd)) != OK)
      {
         FilePrintError(LOGFILE, "Problem finding interval order number of %s\n", sourceSpecList[sourceIndex].interval);
         return (ERROR);
      }

      /* Determine the order number and name of the first destination interval
         - is the same for all noncompounded destination specs */
      if (dataOrd == 1)
      {
         /* First destination for nonbusiness interval data must be specified
            in the firstDestInterval parameter.  Nonbusiness intervals
            have an interval order of 1 - current examples are "instant"
            and "other". */
         if (!strcmp(sourceSpecList[sourceIndex].firstDestInterval, EMPTY_STRING))
         {
             FilePrintError(LOGFILE, "A ref_derivation_source specification for interval = %s must have the first destination interval specified (error at site_datatype_id = %d)\n", sourceSpecList[sourceIndex].interval, sourceSpecList[sourceIndex].siteDatatypeId);
            return (ERROR);
         }
         strcpy (firstDestName, 
                 sourceSpecList[sourceIndex].firstDestInterval);
         if ((result = SqlGetIntOrder(
                       sourceSpecList[sourceIndex].firstDestInterval,
                       &firstDestOrd)) != OK)
         {
            FilePrintError(LOGFILE, "Problem finding interval order number of %s\n", sourceSpecList[sourceIndex].firstDestInterval);
            return (ERROR);
         } 
      }
      else
      {
         /* First destination for business interval data is one higher than 
            the interval of the data itself */
         firstDestOrd = dataOrd + 1;
         if ((result = SqlGetIntName(firstDestOrd, firstDestName)) != OK)
         {
            FilePrintError(LOGFILE, "Problem finding interval name for order number %d\n", firstDestOrd);
            return (ERROR);
         }
 
      }

      /* Loop through each ref_derivation_destination entry */
      for (destIndex = 0; destIndex < destCount; destIndex++)
      {
         /* Derive r_base_update entries to the first interval based on the
            ref_derivation_destination spec */
         if ((result = DeriveList(
                          firstDestName,
                          updateDataList,
                          baseUpdateCount,
                          &sourceSpecList[sourceIndex],
                          &destSpecList[destIndex])) != OK)
         {
            FilePrintError(LOGFILE, "Problem deriving r_base data to first interval\n");
            return (ERROR);
         }
         
         /* Loop and derive values up through the remaining intervals */
         for (intervalLoop = firstDestOrd + 1; intervalLoop <= TOTALNUMINT;
              intervalLoop++) 
         {
            /* Find the name of the interval */
            if ((result = SqlGetIntName(intervalLoop, updateIntName)) != OK)
            {
               FilePrintError(LOGFILE, "Problem finding interval name for order number %d\n", intervalLoop);
               return (ERROR);
            }
            
            /* Find name of previous interval that will be the data source */
            if ((result = SqlGetPreviousIntName(updateIntName,
                          updateIntSource,
                          &valid)) != OK)
            {
               FilePrintError(LOGFILE, "Problem finding previous interval name for interval %s\n", updateIntName);
               return (ERROR);
            }
            /* Check if previous interval is valid - always should be here,
               but if not, return error */
            if (valid == 0)
            {
               FilePrintError(LOGFILE, "Previous interval name not valid for interval %s\n", updateIntName);
               return (ERROR);               
            }

            /* Select the desired data from r_interval_update */
            if ((result = SqlUpdateSelect(
                             destSpecList[destIndex].destSDI,
                             updateIntSource,
                             &intUpdateCount,
                             &intUpdateDataList)) != OK)
            {
               FilePrintError(LOGFILE, "Problem selecting data from r_interval_update for SDI %d and interval %s./n", destSpecList[destIndex].destSDI, updateIntSource);
               return (ERROR);
            }

            /* Derive r_interval_update data to the interval based
               on the ref_derivation_destination spec */
            if ((result = DeriveList(
                             updateIntName,
                             intUpdateDataList,
                             intUpdateCount,
                             &sourceSpecList[sourceIndex],
                             &destSpecList[destIndex])) != OK)
            {
               FilePrintError(LOGFILE, "Problem deriving interval values upward\n");
               return (ERROR);
            }

            /* Free the memory for the intUpdateDataList */
            if (intUpdateDataList != NULL)
            {
               free(intUpdateDataList);
               intUpdateDataList = NULL;
            }
         }
      }
      /* Free the memory for the destSpecList */
      if (destSpecList != NULL)
      {
         free(destSpecList);
         destSpecList = NULL;
      }

      /* CODE FOR HANDLING COMPOUNDED ROWS FROM ref_derivation_destination */

      /* Load all compounded ref_derivation_destination entries that 
         are specified for the current ref_derivation_source */
      /* Find how many relevant entries are in ref_derivation_destination */
      sprintf(selectStatement, "SELECT COUNT(*) FROM ref_derivation_destination WHERE base_site_datatype_id = %d AND compounding_source_sdi IS NOT NULL", sourceSpecList[sourceIndex].siteDatatypeId);

      if ((result = SqlSelectCount(selectStatement, &compCount)) != OK)
      {
         FilePrintError(LOGFILE, "Problem querying compound row count in ref_derivation_destination\n");
         return (ERROR);
      }

      /* Create array of structures to hold compound 
         ref_derivation_destination data */
      if (compCount)
      {
         if ((compSpecList = (REF_DESTINATION *) malloc 
            (sizeof(REF_DESTINATION) * compCount)) == NULL)
         {
            FilePrintError(LOGFILE, "Problem allocating memory for compSpecList\n");
            return (ERROR);
         }
      }
      else
      {
         compSpecList = NULL;
      }

      /* Prepare the select statement for compound 
         ref_derivation_destination data */
      sprintf (sqlSelectStatement, "SELECT r.base_site_datatype_id, r.compounding_source_sdi, r.compounding_source_interval, r.dest_site_datatype_id, r.partial_calc, r.method_id, r.hr_desired_eop_window, r.hr_required_eop_window, r.hr_desired_bop_window, r.hr_required_bop_window, r.hr_window_unit, r.hr_desired_number_source, r.hr_required_number_source, r.day_desired_eop_window, r.day_required_eop_window, r.day_desired_bop_window, r.day_required_bop_window, r.day_window_unit, r.day_desired_number_source, r.day_required_number_source, r.mon_desired_eop_window, r.mon_required_eop_window, r.mon_desired_bop_window, r.mon_required_bop_window, r.mon_window_unit, r.mon_desired_number_source, r.mon_required_number_source, r.yr_desired_eop_window, r.yr_required_eop_window, r.yr_desired_bop_window, r.yr_required_bop_window, r.yr_window_unit, r.yr_desired_number_source, r.yr_required_number_source, r.wy_desired_eop_window, r.wy_required_eop_window, r.wy_desired_bop_window, r.wy_required_bop_window, r.wy_window_unit, r.wy_desired_number_source, r.wy_required_number_source FROM ref_derivation_destination r WHERE r.base_site_datatype_id = %d AND r.compounding_source_sdi IS NOT NULL ", sourceSpecList[sourceIndex].siteDatatypeId);

      /* EXEC SQL PREPARE comp FROM :sqlSelectStatement; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )563;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)sqlSelectStatement;
      sqlstm.sqhstl[0] = (unsigned int  )1500;
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

      /* Declare comp_cursor for selecting compound 
         ref_derivation_destination data */
      /* EXEC SQL DECLARE comp_cursor CURSOR FOR comp; */ 

      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                    "Problem declaring comp_cursor for: %s...\n",
                    sqlSelectStatement)) != OK)
      {
         return (ERROR);
      }

      /* Open comp_cursor */
      /* EXEC SQL OPEN comp_cursor; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )582;
      sqlstm.selerr = (unsigned short)1;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                    "Problem opening comp_cursor for\n: %s...\n",
                    sqlSelectStatement)) != OK)
      {
         return (ERROR);
      }

      /* EXEC SQL WHENEVER NOT FOUND GOTO close_comp_cursor; */ 


      /* Fetch applicable compound ref_derivation_destination data */ 
      for (compIndex = 0; compIndex < compCount ; compIndex++)
      {
         /* EXEC SQL FETCH comp_cursor INTO :sqlRefDestination INDICATOR :sqlIndDestination; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )597;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)&sqlRefDestination.baseSDI;
         sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)&sqlIndDestination.baseSDI_ind;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)&sqlRefDestination.compoundingSourceSDI;
         sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)&sqlIndDestination.compoundingSourceSDI_ind;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)sqlRefDestination.compoundingSourceInterval;
         sqlstm.sqhstl[2] = (unsigned int  )33;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         void  *)&sqlIndDestination.compoundingSourceInterval_ind;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned int  )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (         void  *)&sqlRefDestination.destSDI;
         sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[3] = (         int  )0;
         sqlstm.sqindv[3] = (         void  *)&sqlIndDestination.destSDI_ind;
         sqlstm.sqinds[3] = (         int  )0;
         sqlstm.sqharm[3] = (unsigned int  )0;
         sqlstm.sqadto[3] = (unsigned short )0;
         sqlstm.sqtdso[3] = (unsigned short )0;
         sqlstm.sqhstv[4] = (         void  *)&sqlRefDestination.partialCalc;
         sqlstm.sqhstl[4] = (unsigned int  )1;
         sqlstm.sqhsts[4] = (         int  )0;
         sqlstm.sqindv[4] = (         void  *)&sqlIndDestination.partialCalc_ind;
         sqlstm.sqinds[4] = (         int  )0;
         sqlstm.sqharm[4] = (unsigned int  )0;
         sqlstm.sqadto[4] = (unsigned short )0;
         sqlstm.sqtdso[4] = (unsigned short )0;
         sqlstm.sqhstv[5] = (         void  *)&sqlRefDestination.methodId;
         sqlstm.sqhstl[5] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[5] = (         int  )0;
         sqlstm.sqindv[5] = (         void  *)&sqlIndDestination.methodId;
         sqlstm.sqinds[5] = (         int  )0;
         sqlstm.sqharm[5] = (unsigned int  )0;
         sqlstm.sqadto[5] = (unsigned short )0;
         sqlstm.sqtdso[5] = (unsigned short )0;
         sqlstm.sqhstv[6] = (         void  *)&sqlRefDestination.hrDesiredEopWindow;
         sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[6] = (         int  )0;
         sqlstm.sqindv[6] = (         void  *)&sqlIndDestination.hrDesiredEopWindow_ind;
         sqlstm.sqinds[6] = (         int  )0;
         sqlstm.sqharm[6] = (unsigned int  )0;
         sqlstm.sqadto[6] = (unsigned short )0;
         sqlstm.sqtdso[6] = (unsigned short )0;
         sqlstm.sqhstv[7] = (         void  *)&sqlRefDestination.hrRequiredEopWindow;
         sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[7] = (         int  )0;
         sqlstm.sqindv[7] = (         void  *)&sqlIndDestination.hrRequiredEopWindow_ind;
         sqlstm.sqinds[7] = (         int  )0;
         sqlstm.sqharm[7] = (unsigned int  )0;
         sqlstm.sqadto[7] = (unsigned short )0;
         sqlstm.sqtdso[7] = (unsigned short )0;
         sqlstm.sqhstv[8] = (         void  *)&sqlRefDestination.hrDesiredBopWindow;
         sqlstm.sqhstl[8] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[8] = (         int  )0;
         sqlstm.sqindv[8] = (         void  *)&sqlIndDestination.hrDesiredBopWindow_ind;
         sqlstm.sqinds[8] = (         int  )0;
         sqlstm.sqharm[8] = (unsigned int  )0;
         sqlstm.sqadto[8] = (unsigned short )0;
         sqlstm.sqtdso[8] = (unsigned short )0;
         sqlstm.sqhstv[9] = (         void  *)&sqlRefDestination.hrRequiredBopWindow;
         sqlstm.sqhstl[9] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[9] = (         int  )0;
         sqlstm.sqindv[9] = (         void  *)&sqlIndDestination.hrRequiredBopWindow_ind;
         sqlstm.sqinds[9] = (         int  )0;
         sqlstm.sqharm[9] = (unsigned int  )0;
         sqlstm.sqadto[9] = (unsigned short )0;
         sqlstm.sqtdso[9] = (unsigned short )0;
         sqlstm.sqhstv[10] = (         void  *)sqlRefDestination.hrWindowUnit;
         sqlstm.sqhstl[10] = (unsigned int  )10;
         sqlstm.sqhsts[10] = (         int  )0;
         sqlstm.sqindv[10] = (         void  *)&sqlIndDestination.hrWindowUnit_ind;
         sqlstm.sqinds[10] = (         int  )0;
         sqlstm.sqharm[10] = (unsigned int  )0;
         sqlstm.sqadto[10] = (unsigned short )0;
         sqlstm.sqtdso[10] = (unsigned short )0;
         sqlstm.sqhstv[11] = (         void  *)&sqlRefDestination.hrDesiredNumSource;
         sqlstm.sqhstl[11] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[11] = (         int  )0;
         sqlstm.sqindv[11] = (         void  *)&sqlIndDestination.hrDesiredNumSource_ind;
         sqlstm.sqinds[11] = (         int  )0;
         sqlstm.sqharm[11] = (unsigned int  )0;
         sqlstm.sqadto[11] = (unsigned short )0;
         sqlstm.sqtdso[11] = (unsigned short )0;
         sqlstm.sqhstv[12] = (         void  *)&sqlRefDestination.hrRequiredNumSource;
         sqlstm.sqhstl[12] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[12] = (         int  )0;
         sqlstm.sqindv[12] = (         void  *)&sqlIndDestination.hrRequiredNumSource_ind;
         sqlstm.sqinds[12] = (         int  )0;
         sqlstm.sqharm[12] = (unsigned int  )0;
         sqlstm.sqadto[12] = (unsigned short )0;
         sqlstm.sqtdso[12] = (unsigned short )0;
         sqlstm.sqhstv[13] = (         void  *)&sqlRefDestination.dayDesiredEopWindow;
         sqlstm.sqhstl[13] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[13] = (         int  )0;
         sqlstm.sqindv[13] = (         void  *)&sqlIndDestination.dayDesiredEopWindow_ind;
         sqlstm.sqinds[13] = (         int  )0;
         sqlstm.sqharm[13] = (unsigned int  )0;
         sqlstm.sqadto[13] = (unsigned short )0;
         sqlstm.sqtdso[13] = (unsigned short )0;
         sqlstm.sqhstv[14] = (         void  *)&sqlRefDestination.dayRequiredEopWindow;
         sqlstm.sqhstl[14] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[14] = (         int  )0;
         sqlstm.sqindv[14] = (         void  *)&sqlIndDestination.dayRequiredEopWindow_ind;
         sqlstm.sqinds[14] = (         int  )0;
         sqlstm.sqharm[14] = (unsigned int  )0;
         sqlstm.sqadto[14] = (unsigned short )0;
         sqlstm.sqtdso[14] = (unsigned short )0;
         sqlstm.sqhstv[15] = (         void  *)&sqlRefDestination.dayDesiredBopWindow;
         sqlstm.sqhstl[15] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[15] = (         int  )0;
         sqlstm.sqindv[15] = (         void  *)&sqlIndDestination.dayDesiredBopWindow_ind;
         sqlstm.sqinds[15] = (         int  )0;
         sqlstm.sqharm[15] = (unsigned int  )0;
         sqlstm.sqadto[15] = (unsigned short )0;
         sqlstm.sqtdso[15] = (unsigned short )0;
         sqlstm.sqhstv[16] = (         void  *)&sqlRefDestination.dayRequiredBopWindow;
         sqlstm.sqhstl[16] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[16] = (         int  )0;
         sqlstm.sqindv[16] = (         void  *)&sqlIndDestination.dayRequiredBopWindow_ind;
         sqlstm.sqinds[16] = (         int  )0;
         sqlstm.sqharm[16] = (unsigned int  )0;
         sqlstm.sqadto[16] = (unsigned short )0;
         sqlstm.sqtdso[16] = (unsigned short )0;
         sqlstm.sqhstv[17] = (         void  *)sqlRefDestination.dayWindowUnit;
         sqlstm.sqhstl[17] = (unsigned int  )10;
         sqlstm.sqhsts[17] = (         int  )0;
         sqlstm.sqindv[17] = (         void  *)&sqlIndDestination.dayWindowUnit_ind;
         sqlstm.sqinds[17] = (         int  )0;
         sqlstm.sqharm[17] = (unsigned int  )0;
         sqlstm.sqadto[17] = (unsigned short )0;
         sqlstm.sqtdso[17] = (unsigned short )0;
         sqlstm.sqhstv[18] = (         void  *)&sqlRefDestination.dayDesiredNumSource;
         sqlstm.sqhstl[18] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[18] = (         int  )0;
         sqlstm.sqindv[18] = (         void  *)&sqlIndDestination.dayDesiredNumSource_ind;
         sqlstm.sqinds[18] = (         int  )0;
         sqlstm.sqharm[18] = (unsigned int  )0;
         sqlstm.sqadto[18] = (unsigned short )0;
         sqlstm.sqtdso[18] = (unsigned short )0;
         sqlstm.sqhstv[19] = (         void  *)&sqlRefDestination.dayRequiredNumSource;
         sqlstm.sqhstl[19] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[19] = (         int  )0;
         sqlstm.sqindv[19] = (         void  *)&sqlIndDestination.dayRequiredNumSource_ind;
         sqlstm.sqinds[19] = (         int  )0;
         sqlstm.sqharm[19] = (unsigned int  )0;
         sqlstm.sqadto[19] = (unsigned short )0;
         sqlstm.sqtdso[19] = (unsigned short )0;
         sqlstm.sqhstv[20] = (         void  *)&sqlRefDestination.monDesiredEopWindow;
         sqlstm.sqhstl[20] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[20] = (         int  )0;
         sqlstm.sqindv[20] = (         void  *)&sqlIndDestination.monDesiredEopWindow_ind;
         sqlstm.sqinds[20] = (         int  )0;
         sqlstm.sqharm[20] = (unsigned int  )0;
         sqlstm.sqadto[20] = (unsigned short )0;
         sqlstm.sqtdso[20] = (unsigned short )0;
         sqlstm.sqhstv[21] = (         void  *)&sqlRefDestination.monRequiredEopWindow;
         sqlstm.sqhstl[21] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[21] = (         int  )0;
         sqlstm.sqindv[21] = (         void  *)&sqlIndDestination.monRequiredEopWindow_ind;
         sqlstm.sqinds[21] = (         int  )0;
         sqlstm.sqharm[21] = (unsigned int  )0;
         sqlstm.sqadto[21] = (unsigned short )0;
         sqlstm.sqtdso[21] = (unsigned short )0;
         sqlstm.sqhstv[22] = (         void  *)&sqlRefDestination.monDesiredBopWindow;
         sqlstm.sqhstl[22] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[22] = (         int  )0;
         sqlstm.sqindv[22] = (         void  *)&sqlIndDestination.monDesiredBopWindow_ind;
         sqlstm.sqinds[22] = (         int  )0;
         sqlstm.sqharm[22] = (unsigned int  )0;
         sqlstm.sqadto[22] = (unsigned short )0;
         sqlstm.sqtdso[22] = (unsigned short )0;
         sqlstm.sqhstv[23] = (         void  *)&sqlRefDestination.monRequiredBopWindow;
         sqlstm.sqhstl[23] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[23] = (         int  )0;
         sqlstm.sqindv[23] = (         void  *)&sqlIndDestination.monRequiredBopWindow_ind;
         sqlstm.sqinds[23] = (         int  )0;
         sqlstm.sqharm[23] = (unsigned int  )0;
         sqlstm.sqadto[23] = (unsigned short )0;
         sqlstm.sqtdso[23] = (unsigned short )0;
         sqlstm.sqhstv[24] = (         void  *)sqlRefDestination.monWindowUnit;
         sqlstm.sqhstl[24] = (unsigned int  )10;
         sqlstm.sqhsts[24] = (         int  )0;
         sqlstm.sqindv[24] = (         void  *)&sqlIndDestination.monWindowUnit_ind;
         sqlstm.sqinds[24] = (         int  )0;
         sqlstm.sqharm[24] = (unsigned int  )0;
         sqlstm.sqadto[24] = (unsigned short )0;
         sqlstm.sqtdso[24] = (unsigned short )0;
         sqlstm.sqhstv[25] = (         void  *)&sqlRefDestination.monDesiredNumSource;
         sqlstm.sqhstl[25] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[25] = (         int  )0;
         sqlstm.sqindv[25] = (         void  *)&sqlIndDestination.monDesiredNumSource_ind;
         sqlstm.sqinds[25] = (         int  )0;
         sqlstm.sqharm[25] = (unsigned int  )0;
         sqlstm.sqadto[25] = (unsigned short )0;
         sqlstm.sqtdso[25] = (unsigned short )0;
         sqlstm.sqhstv[26] = (         void  *)&sqlRefDestination.monRequiredNumSource;
         sqlstm.sqhstl[26] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[26] = (         int  )0;
         sqlstm.sqindv[26] = (         void  *)&sqlIndDestination.monRequiredNumSource_ind;
         sqlstm.sqinds[26] = (         int  )0;
         sqlstm.sqharm[26] = (unsigned int  )0;
         sqlstm.sqadto[26] = (unsigned short )0;
         sqlstm.sqtdso[26] = (unsigned short )0;
         sqlstm.sqhstv[27] = (         void  *)&sqlRefDestination.yrDesiredEopWindow;
         sqlstm.sqhstl[27] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[27] = (         int  )0;
         sqlstm.sqindv[27] = (         void  *)&sqlIndDestination.yrDesiredEopWindow_ind;
         sqlstm.sqinds[27] = (         int  )0;
         sqlstm.sqharm[27] = (unsigned int  )0;
         sqlstm.sqadto[27] = (unsigned short )0;
         sqlstm.sqtdso[27] = (unsigned short )0;
         sqlstm.sqhstv[28] = (         void  *)&sqlRefDestination.yrRequiredEopWindow;
         sqlstm.sqhstl[28] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[28] = (         int  )0;
         sqlstm.sqindv[28] = (         void  *)&sqlIndDestination.yrRequiredEopWindow_ind;
         sqlstm.sqinds[28] = (         int  )0;
         sqlstm.sqharm[28] = (unsigned int  )0;
         sqlstm.sqadto[28] = (unsigned short )0;
         sqlstm.sqtdso[28] = (unsigned short )0;
         sqlstm.sqhstv[29] = (         void  *)&sqlRefDestination.yrDesiredBopWindow;
         sqlstm.sqhstl[29] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[29] = (         int  )0;
         sqlstm.sqindv[29] = (         void  *)&sqlIndDestination.yrDesiredBopWindow_ind;
         sqlstm.sqinds[29] = (         int  )0;
         sqlstm.sqharm[29] = (unsigned int  )0;
         sqlstm.sqadto[29] = (unsigned short )0;
         sqlstm.sqtdso[29] = (unsigned short )0;
         sqlstm.sqhstv[30] = (         void  *)&sqlRefDestination.yrRequiredBopWindow;
         sqlstm.sqhstl[30] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[30] = (         int  )0;
         sqlstm.sqindv[30] = (         void  *)&sqlIndDestination.yrRequiredBopWindow_ind;
         sqlstm.sqinds[30] = (         int  )0;
         sqlstm.sqharm[30] = (unsigned int  )0;
         sqlstm.sqadto[30] = (unsigned short )0;
         sqlstm.sqtdso[30] = (unsigned short )0;
         sqlstm.sqhstv[31] = (         void  *)sqlRefDestination.yrWindowUnit;
         sqlstm.sqhstl[31] = (unsigned int  )10;
         sqlstm.sqhsts[31] = (         int  )0;
         sqlstm.sqindv[31] = (         void  *)&sqlIndDestination.yrWindowUnit_ind;
         sqlstm.sqinds[31] = (         int  )0;
         sqlstm.sqharm[31] = (unsigned int  )0;
         sqlstm.sqadto[31] = (unsigned short )0;
         sqlstm.sqtdso[31] = (unsigned short )0;
         sqlstm.sqhstv[32] = (         void  *)&sqlRefDestination.yrDesiredNumSource;
         sqlstm.sqhstl[32] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[32] = (         int  )0;
         sqlstm.sqindv[32] = (         void  *)&sqlIndDestination.yrDesiredNumSource_ind;
         sqlstm.sqinds[32] = (         int  )0;
         sqlstm.sqharm[32] = (unsigned int  )0;
         sqlstm.sqadto[32] = (unsigned short )0;
         sqlstm.sqtdso[32] = (unsigned short )0;
         sqlstm.sqhstv[33] = (         void  *)&sqlRefDestination.yrRequiredNumSource;
         sqlstm.sqhstl[33] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[33] = (         int  )0;
         sqlstm.sqindv[33] = (         void  *)&sqlIndDestination.yrRequiredNumSource_ind;
         sqlstm.sqinds[33] = (         int  )0;
         sqlstm.sqharm[33] = (unsigned int  )0;
         sqlstm.sqadto[33] = (unsigned short )0;
         sqlstm.sqtdso[33] = (unsigned short )0;
         sqlstm.sqhstv[34] = (         void  *)&sqlRefDestination.wyDesiredEopWindow;
         sqlstm.sqhstl[34] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[34] = (         int  )0;
         sqlstm.sqindv[34] = (         void  *)&sqlIndDestination.wyDesiredEopWindow_ind;
         sqlstm.sqinds[34] = (         int  )0;
         sqlstm.sqharm[34] = (unsigned int  )0;
         sqlstm.sqadto[34] = (unsigned short )0;
         sqlstm.sqtdso[34] = (unsigned short )0;
         sqlstm.sqhstv[35] = (         void  *)&sqlRefDestination.wyRequiredEopWindow;
         sqlstm.sqhstl[35] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[35] = (         int  )0;
         sqlstm.sqindv[35] = (         void  *)&sqlIndDestination.wyRequiredEopWindow_ind;
         sqlstm.sqinds[35] = (         int  )0;
         sqlstm.sqharm[35] = (unsigned int  )0;
         sqlstm.sqadto[35] = (unsigned short )0;
         sqlstm.sqtdso[35] = (unsigned short )0;
         sqlstm.sqhstv[36] = (         void  *)&sqlRefDestination.wyDesiredBopWindow;
         sqlstm.sqhstl[36] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[36] = (         int  )0;
         sqlstm.sqindv[36] = (         void  *)&sqlIndDestination.wyDesiredBopWindow_ind;
         sqlstm.sqinds[36] = (         int  )0;
         sqlstm.sqharm[36] = (unsigned int  )0;
         sqlstm.sqadto[36] = (unsigned short )0;
         sqlstm.sqtdso[36] = (unsigned short )0;
         sqlstm.sqhstv[37] = (         void  *)&sqlRefDestination.wyRequiredBopWindow;
         sqlstm.sqhstl[37] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[37] = (         int  )0;
         sqlstm.sqindv[37] = (         void  *)&sqlIndDestination.wyRequiredBopWindow_ind;
         sqlstm.sqinds[37] = (         int  )0;
         sqlstm.sqharm[37] = (unsigned int  )0;
         sqlstm.sqadto[37] = (unsigned short )0;
         sqlstm.sqtdso[37] = (unsigned short )0;
         sqlstm.sqhstv[38] = (         void  *)sqlRefDestination.wyWindowUnit;
         sqlstm.sqhstl[38] = (unsigned int  )10;
         sqlstm.sqhsts[38] = (         int  )0;
         sqlstm.sqindv[38] = (         void  *)&sqlIndDestination.wyWindowUnit_ind;
         sqlstm.sqinds[38] = (         int  )0;
         sqlstm.sqharm[38] = (unsigned int  )0;
         sqlstm.sqadto[38] = (unsigned short )0;
         sqlstm.sqtdso[38] = (unsigned short )0;
         sqlstm.sqhstv[39] = (         void  *)&sqlRefDestination.wyDesiredNumSource;
         sqlstm.sqhstl[39] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[39] = (         int  )0;
         sqlstm.sqindv[39] = (         void  *)&sqlIndDestination.wyDesiredNumSource_ind;
         sqlstm.sqinds[39] = (         int  )0;
         sqlstm.sqharm[39] = (unsigned int  )0;
         sqlstm.sqadto[39] = (unsigned short )0;
         sqlstm.sqtdso[39] = (unsigned short )0;
         sqlstm.sqhstv[40] = (         void  *)&sqlRefDestination.wyRequiredNumSource;
         sqlstm.sqhstl[40] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[40] = (         int  )0;
         sqlstm.sqindv[40] = (         void  *)&sqlIndDestination.wyRequiredNumSource_ind;
         sqlstm.sqinds[40] = (         int  )0;
         sqlstm.sqharm[40] = (unsigned int  )0;
         sqlstm.sqadto[40] = (unsigned short )0;
         sqlstm.sqtdso[40] = (unsigned short )0;
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
         if (sqlca.sqlcode == 1403) goto close_comp_cursor;
}

 

         if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
            "Problem fetching comp_cursor for\n: %s...\n",
                    sqlSelectStatement)) != OK)
         {
            /* EXEC SQL ROLLBACK; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )776;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


            return (ERROR);
         }

         if ((result = LoadDestination(
                       &sqlRefDestination,
                       &sqlIndDestination,
                       &compSpecList[compIndex])) != OK)
         {
            FilePrintError(LOGFILE, "Problem loading compound destination specification.");
            return (ERROR);
         }
      }

      /* Close comp_cursor */
      close_comp_cursor:
      /* EXEC SQL CLOSE comp_cursor; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )791;
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
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )806;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      /* EXEC SQL WHENEVER NOT FOUND CONTINUE; */ 


      /* Loop through each compound ref_derivation_destination entry */
      for (compIndex = 0; compIndex < compCount; compIndex++)
      {
         /* Determine order number for the compounding source interval */
         if ((result = SqlGetIntOrder(
                       compSpecList[compIndex].compoundingSourceInterval,
                       &compOrd)) != OK)
         {
            FilePrintError(LOGFILE, "Problem finding interval order number of %s\n", compSpecList[compIndex].compoundingSourceInterval);
            return (ERROR);
         }

         /* Loop and derive values up through the intervals */
         for (intervalLoop = compOrd + 1; intervalLoop <= TOTALNUMINT;
              intervalLoop++) 
         {
            /* Find the name of the interval */
            if ((result = SqlGetIntName(intervalLoop, compIntName)) != OK)
            {
               FilePrintError(LOGFILE, "Problem finding interval name for order number %d\n", intervalLoop);
               return (ERROR);
            }
            
            /* Find name of previous interval that will be the data source */
            if ((result = SqlGetPreviousIntName(compIntName,
                          compIntSource,
                          &valid)) != OK)
            {
               FilePrintError(LOGFILE, "Problem finding previous interval name for interval %s\n", compIntName);
               return (ERROR);
            }

            /* Check if previous interval is valid - always should be here,
               but if not, return error */
            if (valid == 0)
            {
               FilePrintError(LOGFILE, "Previous interval name not valid for interval %s\n", compIntName);
               return (ERROR);               
            }

            /* Set the SDI for the source data query */
            if (intervalLoop == compOrd+1)
            {
               /* This is the first derivation, so source SDI is specified in 
                  the compound ref_derivation_destination spec */
               compSDI = compSpecList[compIndex].compoundingSourceSDI;
            }
            else
            {
               /* SDI is the dest SDI that was derived during the previous
                  interval iteration */
               compSDI = compSpecList[compIndex].destSDI;
            }

            /* Select the desired data from r_interval_update */
            if ((result = SqlUpdateSelect(
                             compSDI,
                             compIntSource,
                             &compUpdateCount,
                             &compUpdateList)) != OK)
            {
               FilePrintError(LOGFILE, "Problem selecting compound data from r_interval_update for SDI %d and interval %s./n", compSpecList[compIndex].compoundingSourceSDI, compIntSource);
               return (ERROR);
            }

            /* Derive the compound r_interval_update data to the interval based
               on the compound ref_derivation_destination spec */
            if ((result = DeriveList(
                             compIntName,
                             compUpdateList,
                             compUpdateCount,
                             &sourceSpecList[sourceIndex],
                             &compSpecList[compIndex])) != OK)
            {
               FilePrintError(LOGFILE, "Problem deriving compound interval values upward\n");
               return (ERROR);
            }

            /* Free the memory for the compUpdateList */
            if (compUpdateList != NULL)
            {
               free(compUpdateList);
               compUpdateList = NULL;
            }
         }

      }

      /* Free the memory for the compSpecList */
      if (compSpecList != NULL)
      {
         free(compSpecList);
         compSpecList = NULL;
      }

      /* Make sure all changes are committed */
      /* EXEC SQL COMMIT; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )821;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

     

      /* Delete r_base_update records for the current SDI */
      if (baseUpdateCount)
      {
         /* Unmark as ready for delete the most recent loaded record for
            this SDI  - keeping this one will ensure completion of partial
            calcs next time the app runs */
         sqlUpdateRecord = &updateDataList[baseUpdateCount - 1];
         /* EXEC SQL UPDATE r_base_update
            SET ready_for_delete = NULL
            WHERE site_datatype_id = :sqlUpdateRecord->siteDatatypeId
              AND start_date_time = TO_DATE (:sqlUpdateRecord->startDateTime,
                                        'DD-MON-YYYY HH24:MI:SS')
              AND end_date_time = TO_DATE (:sqlUpdateRecord->endDateTime,
                                        'DD-MON-YYYY HH24:MI:SS')
              AND interval = :sqlUpdateRecord->interval
              AND overwrite_flag IS NULL; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "update r_base_update  set ready_for_delete=null  whe\
re ((((site_datatype_id=:b0 and start_date_time=TO_DATE(:b1,'DD-MON-YYYY HH24:\
MI:SS')) and end_date_time=TO_DATE(:b2,'DD-MON-YYYY HH24:MI:SS')) and  interva\
l =:b3) and overwrite_flag is null )";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )836;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)&(sqlUpdateRecord->siteDatatypeId);
         sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)(sqlUpdateRecord->startDateTime);
         sqlstm.sqhstl[1] = (unsigned int  )21;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)(sqlUpdateRecord->endDateTime);
         sqlstm.sqhstl[2] = (unsigned int  )21;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         void  *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned int  )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (         void  *)(sqlUpdateRecord->interval);
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
                       "Problem unmarking record as ready for delete in r_base_update\n")) != OK)
         {
            /* EXEC SQL ROLLBACK; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )867;
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
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )882;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

  

         /* Then delete all the other entries from the r_base_update table for
            this SDI that are marked as ready for delete */
         /* EXEC SQL DELETE FROM r_base_update
            WHERE site_datatype_id = :sqlUpdateRecord->siteDatatypeId
               AND interval = :sqlUpdateRecord->interval
               AND overwrite_flag IS NULL
               AND ready_for_delete = 'Y'; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "delete  from r_base_update  where (((site_datatype_i\
d=:b0 and  interval =:b1) and overwrite_flag is null ) and ready_for_delete='Y\
')";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )897;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)&(sqlUpdateRecord->siteDatatypeId);
         sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)(sqlUpdateRecord->interval);
         sqlstm.sqhstl[1] = (unsigned int  )33;
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


         /* TRUE used in second argument to SqlFilePrintErrorHandler because it
            is OK if no rows were processed above - with records getting
            unmarked could be none left to delete */
         if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE,
                       "Problem deleting records from r_base_update\n")) != OK)
         {
            /* EXEC SQL ROLLBACK; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )920;
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
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )935;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

           
      }

      /* Free the memory for the updateDataList */
      if (updateDataList != NULL)
      {
         free(updateDataList);
         updateDataList = NULL; 
      }
   }

   /* Free the memory for the sourceSpecList */
   if (sourceSpecList != NULL)
   {
      free(sourceSpecList);
      sourceSpecList = NULL; 
   }

   return (OK);

}

/****************************************************************************/
#define FUNC_NAME "SqlCopyBase"

int SqlCopyBase(UPDATE_RECORD* updateList, int updateCount,
                REF_SOURCE* refSource)
{
   /* Copy data from the base area to the appropriate table */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      char sqlDeleteStatement[600];
      char sqlInsertStatement[600];
      char sqlUpdateStatement[600];
      DATA_RECORD sqlDataRecord;
      IND sqlDerFlag_ind = 0;
      INTERVAL_TIME sqlIntervalTime;
      int sqlCountBase;
      int sqlCountSource;
      UPDATE_RECORD* sqlUpdateRecord;
   /* EXEC SQL END DECLARE SECTION; */ 


   char intervalTable[12];
   char selectStatement[600];
   int countDestination;
   int order;
   int result;
   int updateIndex;
   int valid;

   /* Create the destination interval table name */
   sprintf (intervalTable, "r_%s", refSource->interval);

   /* Create and prepare delete statement for the destination table for 
      use in data processing loops below (Dynamic SQL Method 2) */
   sprintf(sqlDeleteStatement, "DELETE FROM %s WHERE site_datatype_id = %d AND start_date_time = TO_DATE (:a, 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE (:b, 'DD-MON-YYYY HH24:MI:SS')", intervalTable, refSource->siteDatatypeId);
   /* EXEC SQL PREPARE del FROM :sqlDeleteStatement; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )950;
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


   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                "Problem preparing statement %s\n", sqlDeleteStatement)) != OK)
   {
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )969;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
   }      

   /* Create and prepare insert statement for the destination table for 
      use in data processing loops below (Dynamic SQL Method 2) */
   sprintf(sqlInsertStatement, "INSERT INTO %s (site_datatype_id, start_date_time, end_date_time, value, overwrite_flag, method_id, derivation_flags, date_time_loaded, validation) VALUES (:a, TO_DATE (:b, 'DD-MON-YYYY HH24:MI:SS'), TO_DATE (:c, 'DD-MON-YYYY HH24:MI:SS'), :d, NULL, :g, :h, TO_DATE (:j, 'DD-MON-YYYY HH24:MI:SS'), NULL)", intervalTable);
   /* EXEC SQL PREPARE ins FROM :sqlInsertStatement; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )984;
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


   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                "Problem preparing statement %s\n", sqlInsertStatement)) != OK)
   {
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )1003;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
   }      

   /* Create and prepare update statement for the destination table for
      use in data processing loops below (Dynamic SQL Method 2) */
   sprintf(sqlUpdateStatement, "UPDATE %s SET value = :c, overwrite_flag = NULL, method_id = :f, derivation_flags = :g, date_time_loaded = TO_DATE (:i, 'DD-MON-YYYY HH24:MI:SS') WHERE site_datatype_id = :j AND start_date_time = TO_DATE (:k, 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE (:l, 'DD-MON-YYYY HH24:MI:SS')", intervalTable);
   /* EXEC SQL PREPARE upd FROM :sqlUpdateStatement; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1018;
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


   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                "Problem preparing statement %s\n", sqlUpdateStatement)) != OK)
   {
      /* EXEC SQL ROLLBACK; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )1037;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


      return (ERROR);
   }      

   /* Get the order of the interval we are looking at */
   if ((result = SqlGetIntOrder(refSource->interval, &order))
                 != OK)
   {
      FilePrintError(LOGFILE, "Problem getting interval order number for %s\n",
                  refSource->interval);
      return (ERROR);
   }

   /* Check if values are not one of the regular business intervals (this
      can be determined by checking if the interval order is 1. Current
      examples of this are the intervals "instant" and "other".) Copy
      process is different for these nonbusiness intervals */   
   if (order == 1)
   {
      /* Step through data list and insert, update, or delete in destination
         table */
      for (updateIndex = 0; updateIndex < updateCount ; updateIndex++)
      {
         sqlUpdateRecord = &updateList[updateIndex];

         /* Look for the value in r_base */
         /* EXEC SQL
            SELECT COUNT(*) INTO :sqlCountBase
            FROM r_base 
            WHERE site_datatype_id = :sqlUpdateRecord->siteDatatypeId
               AND start_date_time = TO_DATE (:sqlUpdateRecord->startDateTime,
                                        'DD-MON-YYYY HH24:MI:SS')
               AND end_date_time = TO_DATE (:sqlUpdateRecord->endDateTime,
                                        'DD-MON-YYYY HH24:MI:SS')
               AND interval = :sqlUpdateRecord->interval
               AND overwrite_flag IS NULL; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "select count(*)  into :b0  from r_base where ((((sit\
e_datatype_id=:b1 and start_date_time=TO_DATE(:b2,'DD-MON-YYYY HH24:MI:SS')) a\
nd end_date_time=TO_DATE(:b3,'DD-MON-YYYY HH24:MI:SS')) and  interval =:b4) an\
d overwrite_flag is null )";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1052;
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
         sqlstm.sqhstv[1] = (         void  *)&(sqlUpdateRecord->siteDatatypeId);
         sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)(sqlUpdateRecord->startDateTime);
         sqlstm.sqhstl[2] = (unsigned int  )21;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         void  *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned int  )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (         void  *)(sqlUpdateRecord->endDateTime);
         sqlstm.sqhstl[3] = (unsigned int  )21;
         sqlstm.sqhsts[3] = (         int  )0;
         sqlstm.sqindv[3] = (         void  *)0;
         sqlstm.sqinds[3] = (         int  )0;
         sqlstm.sqharm[3] = (unsigned int  )0;
         sqlstm.sqadto[3] = (unsigned short )0;
         sqlstm.sqtdso[3] = (unsigned short )0;
         sqlstm.sqhstv[4] = (         void  *)(sqlUpdateRecord->interval);
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
                       "Problem querying for count of data in r_base\n"))
                        != OK)
         {
            /* EXEC SQL ROLLBACK; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )1087;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


            return (ERROR);
         }

         /* If the value does not exist in r_base, entry represents a delete
            from the base area, so also delete the value from the destination
            table if it exists */
         if (sqlCountBase == 0)
         {
            /* EXEC SQL EXECUTE del USING :sqlUpdateRecord->startDateTime, :sqlUpdateRecord->endDateTime; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )1102;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)(sqlUpdateRecord->startDateTime);
            sqlstm.sqhstl[0] = (unsigned int  )21;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)(sqlUpdateRecord->endDateTime);
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



            /* TRUE used in second argument to SqlFilePrintErrorHandler because
               it is OK if no rows were processed above - means a value hadn't 
               been previously copied to the destination table */
            if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE, 
                          "Problem deleting data record from %s\n", 
                          intervalTable)) != OK)
            {
               /* EXEC SQL ROLLBACK; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 41;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )1125;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


               return (ERROR);
            }        
         }

         /* Value exists in r_base so represents an update or insert */
         else
         {
            /* Select information from r_base to fill in a data record
               corresponding to the entry */
            /* EXEC SQL
               SELECT site_datatype_id,
                      TO_CHAR (start_date_time, 
                               'DD-MON-YYYY HH24:MI:SS'), 
                      TO_CHAR (end_date_time,
                               'DD-MON-YYYY HH24:MI:SS'),
                      value
               INTO :sqlDataRecord.siteDatatypeId,
                    :sqlDataRecord.startDateTime,
                    :sqlDataRecord.endDateTime,
                    :sqlDataRecord.value
               FROM r_base
               WHERE site_datatype_id = :sqlUpdateRecord->siteDatatypeId
                  AND start_date_time = TO_DATE (
                                           :sqlUpdateRecord->startDateTime,
                                           'DD-MON-YYYY HH24:MI:SS')
                  AND end_date_time = TO_DATE (
                                           :sqlUpdateRecord->endDateTime,
                                           'DD-MON-YYYY HH24:MI:SS')
                  AND interval = :sqlUpdateRecord->interval
                  AND overwrite_flag IS NULL; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "select site_datatype_id ,TO_CHAR(start_date_time,\
'DD-MON-YYYY HH24:MI:SS') ,TO_CHAR(end_date_time,'DD-MON-YYYY HH24:MI:SS') ,va\
lue into :b0,:b1,:b2,:b3  from r_base where ((((site_datatype_id=:b4 and start\
_date_time=TO_DATE(:b5,'DD-MON-YYYY HH24:MI:SS')) and end_date_time=TO_DATE(:b\
6,'DD-MON-YYYY HH24:MI:SS')) and  interval =:b7) and overwrite_flag is null )";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )1140;
            sqlstm.selerr = (unsigned short)1;
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
            sqlstm.sqhstv[1] = (         void  *)(sqlDataRecord.startDateTime);
            sqlstm.sqhstl[1] = (unsigned int  )21;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         void  *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned int  )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (         void  *)(sqlDataRecord.endDateTime);
            sqlstm.sqhstl[2] = (unsigned int  )21;
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         void  *)0;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned int  )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (         void  *)&(sqlDataRecord.value);
            sqlstm.sqhstl[3] = (unsigned int  )sizeof(double);
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         void  *)0;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned int  )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
            sqlstm.sqhstv[4] = (         void  *)&(sqlUpdateRecord->siteDatatypeId);
            sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
            sqlstm.sqhsts[4] = (         int  )0;
            sqlstm.sqindv[4] = (         void  *)0;
            sqlstm.sqinds[4] = (         int  )0;
            sqlstm.sqharm[4] = (unsigned int  )0;
            sqlstm.sqadto[4] = (unsigned short )0;
            sqlstm.sqtdso[4] = (unsigned short )0;
            sqlstm.sqhstv[5] = (         void  *)(sqlUpdateRecord->startDateTime);
            sqlstm.sqhstl[5] = (unsigned int  )21;
            sqlstm.sqhsts[5] = (         int  )0;
            sqlstm.sqindv[5] = (         void  *)0;
            sqlstm.sqinds[5] = (         int  )0;
            sqlstm.sqharm[5] = (unsigned int  )0;
            sqlstm.sqadto[5] = (unsigned short )0;
            sqlstm.sqtdso[5] = (unsigned short )0;
            sqlstm.sqhstv[6] = (         void  *)(sqlUpdateRecord->endDateTime);
            sqlstm.sqhstl[6] = (unsigned int  )21;
            sqlstm.sqhsts[6] = (         int  )0;
            sqlstm.sqindv[6] = (         void  *)0;
            sqlstm.sqinds[6] = (         int  )0;
            sqlstm.sqharm[6] = (unsigned int  )0;
            sqlstm.sqadto[6] = (unsigned short )0;
            sqlstm.sqtdso[6] = (unsigned short )0;
            sqlstm.sqhstv[7] = (         void  *)(sqlUpdateRecord->interval);
            sqlstm.sqhstl[7] = (unsigned int  )33;
            sqlstm.sqhsts[7] = (         int  )0;
            sqlstm.sqindv[7] = (         void  *)0;
            sqlstm.sqinds[7] = (         int  )0;
            sqlstm.sqharm[7] = (unsigned int  )0;
            sqlstm.sqadto[7] = (unsigned short )0;
            sqlstm.sqtdso[7] = (unsigned short )0;
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
                          "Problem querying data record from r_base\n"))
                           != OK)
            {
               /* EXEC SQL ROLLBACK; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 41;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )1187;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


               return (ERROR);
            }

            /* Initialize the derivation flag to an empty string */
            strcpy (sqlDataRecord.derivationFlag, EMPTY_STRING);

            /* Is record valid for updating or inserting */
            if ((result = CheckCopy(&sqlDataRecord, refSource, &valid))
                 != OK)
            {
               FilePrintError(LOGFILE, "Problem verifying data record for copy\n");
               return (ERROR);
            }

            /* If valid, fill in other members of the data record */
            if (valid != 0)
            {
               /* Assign the dateTimeLoaded member */
               strcpy (sqlDataRecord.dateTimeLoaded, DATETIMELOADED);

               /* Assign the method_id for copy to the method member */
               sqlDataRecord.methodId = 6;

               /* Assign equivalent of NULL to the overwrite member */
               sqlDataRecord.overwrite = EMPTY_CHAR;    

               /* Assign indicator variable for the derivation flag
                  for future writing to the database */
               if (!strcmp(sqlDataRecord.derivationFlag, EMPTY_STRING))
               {
                  sqlDerFlag_ind = -1;
               }
               else
               {
                  sqlDerFlag_ind = 0;
               }   

            }

            /* Check to see if there is a value in the destination slot */   
            sprintf(selectStatement, "SELECT COUNT(*) FROM %s WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND overwrite_flag IS NULL", intervalTable, sqlDataRecord.siteDatatypeId, sqlDataRecord.startDateTime, sqlDataRecord.endDateTime);

            if ((result = SqlSelectCount(selectStatement, &countDestination))                  != OK)
            {
               FilePrintError(LOGFILE, "Problem querying count for destination table\n");
               return (ERROR);
            }

            /* If value doesn't exist in destination, it is an insert */
            if (countDestination == 0)
            {
               /* If record is valid, insert it to destination table - invalid
                  records do not get inserted */
               if (valid != 0)
               {
                  /* EXEC SQL EXECUTE ins USING :sqlDataRecord.siteDatatypeId, :sqlDataRecord.startDateTime, :sqlDataRecord.endDateTime, :sqlDataRecord.value, :sqlDataRecord.methodId, :sqlDataRecord.derivationFlag :sqlDerFlag_ind, :sqlDataRecord.dateTimeLoaded; */ 

{
                  struct sqlexd sqlstm;
                  sqlstm.sqlvsn = 10;
                  sqlstm.arrsiz = 41;
                  sqlstm.sqladtp = &sqladt;
                  sqlstm.sqltdsp = &sqltds;
                  sqlstm.stmt = "";
                  sqlstm.iters = (unsigned int  )1;
                  sqlstm.offset = (unsigned int  )1202;
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
                  sqlstm.sqhstv[1] = (         void  *)(sqlDataRecord.startDateTime);
                  sqlstm.sqhstl[1] = (unsigned int  )21;
                  sqlstm.sqhsts[1] = (         int  )0;
                  sqlstm.sqindv[1] = (         void  *)0;
                  sqlstm.sqinds[1] = (         int  )0;
                  sqlstm.sqharm[1] = (unsigned int  )0;
                  sqlstm.sqadto[1] = (unsigned short )0;
                  sqlstm.sqtdso[1] = (unsigned short )0;
                  sqlstm.sqhstv[2] = (         void  *)(sqlDataRecord.endDateTime);
                  sqlstm.sqhstl[2] = (unsigned int  )21;
                  sqlstm.sqhsts[2] = (         int  )0;
                  sqlstm.sqindv[2] = (         void  *)0;
                  sqlstm.sqinds[2] = (         int  )0;
                  sqlstm.sqharm[2] = (unsigned int  )0;
                  sqlstm.sqadto[2] = (unsigned short )0;
                  sqlstm.sqtdso[2] = (unsigned short )0;
                  sqlstm.sqhstv[3] = (         void  *)&(sqlDataRecord.value);
                  sqlstm.sqhstl[3] = (unsigned int  )sizeof(double);
                  sqlstm.sqhsts[3] = (         int  )0;
                  sqlstm.sqindv[3] = (         void  *)0;
                  sqlstm.sqinds[3] = (         int  )0;
                  sqlstm.sqharm[3] = (unsigned int  )0;
                  sqlstm.sqadto[3] = (unsigned short )0;
                  sqlstm.sqtdso[3] = (unsigned short )0;
                  sqlstm.sqhstv[4] = (         void  *)&(sqlDataRecord.methodId);
                  sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
                  sqlstm.sqhsts[4] = (         int  )0;
                  sqlstm.sqindv[4] = (         void  *)0;
                  sqlstm.sqinds[4] = (         int  )0;
                  sqlstm.sqharm[4] = (unsigned int  )0;
                  sqlstm.sqadto[4] = (unsigned short )0;
                  sqlstm.sqtdso[4] = (unsigned short )0;
                  sqlstm.sqhstv[5] = (         void  *)(sqlDataRecord.derivationFlag);
                  sqlstm.sqhstl[5] = (unsigned int  )20;
                  sqlstm.sqhsts[5] = (         int  )0;
                  sqlstm.sqindv[5] = (         void  *)&sqlDerFlag_ind;
                  sqlstm.sqinds[5] = (         int  )0;
                  sqlstm.sqharm[5] = (unsigned int  )0;
                  sqlstm.sqadto[5] = (unsigned short )0;
                  sqlstm.sqtdso[5] = (unsigned short )0;
                  sqlstm.sqhstv[6] = (         void  *)(sqlDataRecord.dateTimeLoaded);
                  sqlstm.sqhstl[6] = (unsigned int  )21;
                  sqlstm.sqhsts[6] = (         int  )0;
                  sqlstm.sqindv[6] = (         void  *)0;
                  sqlstm.sqinds[6] = (         int  )0;
                  sqlstm.sqharm[6] = (unsigned int  )0;
                  sqlstm.sqadto[6] = (unsigned short )0;
                  sqlstm.sqtdso[6] = (unsigned short )0;
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



                  if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME,
                                FALSE,
                                "Problem inserting data record into %s\n", 
                                 intervalTable)) != OK)
                  {
                     /* EXEC SQL ROLLBACK; */ 

{
                     struct sqlexd sqlstm;
                     sqlstm.sqlvsn = 10;
                     sqlstm.arrsiz = 41;
                     sqlstm.sqladtp = &sqladt;
                     sqlstm.sqltdsp = &sqltds;
                     sqlstm.iters = (unsigned int  )1;
                     sqlstm.offset = (unsigned int  )1245;
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
            /* Value exists in destination so is an update */
            else
            {
               /* If new record to update the old is not valid, it is the 
                  equivalent of just deleting the old record */
               if (valid == 0)
               {
                  /* EXEC SQL EXECUTE del USING :sqlDataRecord.startDateTime, :sqlDataRecord.endDateTime; */ 

{
                  struct sqlexd sqlstm;
                  sqlstm.sqlvsn = 10;
                  sqlstm.arrsiz = 41;
                  sqlstm.sqladtp = &sqladt;
                  sqlstm.sqltdsp = &sqltds;
                  sqlstm.stmt = "";
                  sqlstm.iters = (unsigned int  )1;
                  sqlstm.offset = (unsigned int  )1260;
                  sqlstm.cud = sqlcud0;
                  sqlstm.sqlest = (unsigned char  *)&sqlca;
                  sqlstm.sqlety = (unsigned short)256;
                  sqlstm.occurs = (unsigned int  )0;
                  sqlstm.sqhstv[0] = (         void  *)(sqlDataRecord.startDateTime);
                  sqlstm.sqhstl[0] = (unsigned int  )21;
                  sqlstm.sqhsts[0] = (         int  )0;
                  sqlstm.sqindv[0] = (         void  *)0;
                  sqlstm.sqinds[0] = (         int  )0;
                  sqlstm.sqharm[0] = (unsigned int  )0;
                  sqlstm.sqadto[0] = (unsigned short )0;
                  sqlstm.sqtdso[0] = (unsigned short )0;
                  sqlstm.sqhstv[1] = (         void  *)(sqlDataRecord.endDateTime);
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



                  if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME,
                                FALSE, 
                                "Problem deleting data record from %s\n", 
                                intervalTable)) != OK)
                  {
                     /* EXEC SQL ROLLBACK; */ 

{
                     struct sqlexd sqlstm;
                     sqlstm.sqlvsn = 10;
                     sqlstm.arrsiz = 41;
                     sqlstm.sqladtp = &sqladt;
                     sqlstm.sqltdsp = &sqltds;
                     sqlstm.iters = (unsigned int  )1;
                     sqlstm.offset = (unsigned int  )1283;
                     sqlstm.cud = sqlcud0;
                     sqlstm.sqlest = (unsigned char  *)&sqlca;
                     sqlstm.sqlety = (unsigned short)256;
                     sqlstm.occurs = (unsigned int  )0;
                     sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


                     return (ERROR);
                  }
               }
               /* New record is valid and can be used to update the old one */
               else
               {
                  /* EXEC SQL EXECUTE upd USING :sqlDataRecord.value, :sqlDataRecord.methodId, :sqlDataRecord.derivationFlag :sqlDerFlag_ind, :sqlDataRecord.dateTimeLoaded,:sqlDataRecord.siteDatatypeId, :sqlDataRecord.startDateTime, :sqlDataRecord.endDateTime; */ 

{
                  struct sqlexd sqlstm;
                  sqlstm.sqlvsn = 10;
                  sqlstm.arrsiz = 41;
                  sqlstm.sqladtp = &sqladt;
                  sqlstm.sqltdsp = &sqltds;
                  sqlstm.stmt = "";
                  sqlstm.iters = (unsigned int  )1;
                  sqlstm.offset = (unsigned int  )1298;
                  sqlstm.cud = sqlcud0;
                  sqlstm.sqlest = (unsigned char  *)&sqlca;
                  sqlstm.sqlety = (unsigned short)256;
                  sqlstm.occurs = (unsigned int  )0;
                  sqlstm.sqhstv[0] = (         void  *)&(sqlDataRecord.value);
                  sqlstm.sqhstl[0] = (unsigned int  )sizeof(double);
                  sqlstm.sqhsts[0] = (         int  )0;
                  sqlstm.sqindv[0] = (         void  *)0;
                  sqlstm.sqinds[0] = (         int  )0;
                  sqlstm.sqharm[0] = (unsigned int  )0;
                  sqlstm.sqadto[0] = (unsigned short )0;
                  sqlstm.sqtdso[0] = (unsigned short )0;
                  sqlstm.sqhstv[1] = (         void  *)&(sqlDataRecord.methodId);
                  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
                  sqlstm.sqhsts[1] = (         int  )0;
                  sqlstm.sqindv[1] = (         void  *)0;
                  sqlstm.sqinds[1] = (         int  )0;
                  sqlstm.sqharm[1] = (unsigned int  )0;
                  sqlstm.sqadto[1] = (unsigned short )0;
                  sqlstm.sqtdso[1] = (unsigned short )0;
                  sqlstm.sqhstv[2] = (         void  *)(sqlDataRecord.derivationFlag);
                  sqlstm.sqhstl[2] = (unsigned int  )20;
                  sqlstm.sqhsts[2] = (         int  )0;
                  sqlstm.sqindv[2] = (         void  *)&sqlDerFlag_ind;
                  sqlstm.sqinds[2] = (         int  )0;
                  sqlstm.sqharm[2] = (unsigned int  )0;
                  sqlstm.sqadto[2] = (unsigned short )0;
                  sqlstm.sqtdso[2] = (unsigned short )0;
                  sqlstm.sqhstv[3] = (         void  *)(sqlDataRecord.dateTimeLoaded);
                  sqlstm.sqhstl[3] = (unsigned int  )21;
                  sqlstm.sqhsts[3] = (         int  )0;
                  sqlstm.sqindv[3] = (         void  *)0;
                  sqlstm.sqinds[3] = (         int  )0;
                  sqlstm.sqharm[3] = (unsigned int  )0;
                  sqlstm.sqadto[3] = (unsigned short )0;
                  sqlstm.sqtdso[3] = (unsigned short )0;
                  sqlstm.sqhstv[4] = (         void  *)&(sqlDataRecord.siteDatatypeId);
                  sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
                  sqlstm.sqhsts[4] = (         int  )0;
                  sqlstm.sqindv[4] = (         void  *)0;
                  sqlstm.sqinds[4] = (         int  )0;
                  sqlstm.sqharm[4] = (unsigned int  )0;
                  sqlstm.sqadto[4] = (unsigned short )0;
                  sqlstm.sqtdso[4] = (unsigned short )0;
                  sqlstm.sqhstv[5] = (         void  *)(sqlDataRecord.startDateTime);
                  sqlstm.sqhstl[5] = (unsigned int  )21;
                  sqlstm.sqhsts[5] = (         int  )0;
                  sqlstm.sqindv[5] = (         void  *)0;
                  sqlstm.sqinds[5] = (         int  )0;
                  sqlstm.sqharm[5] = (unsigned int  )0;
                  sqlstm.sqadto[5] = (unsigned short )0;
                  sqlstm.sqtdso[5] = (unsigned short )0;
                  sqlstm.sqhstv[6] = (         void  *)(sqlDataRecord.endDateTime);
                  sqlstm.sqhstl[6] = (unsigned int  )21;
                  sqlstm.sqhsts[6] = (         int  )0;
                  sqlstm.sqindv[6] = (         void  *)0;
                  sqlstm.sqinds[6] = (         int  )0;
                  sqlstm.sqharm[6] = (unsigned int  )0;
                  sqlstm.sqadto[6] = (unsigned short )0;
                  sqlstm.sqtdso[6] = (unsigned short )0;
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



                  if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME,
                                FALSE, 
                                "Problem updating data record in %s\n", 
                                intervalTable)) != OK)
                  {
                     /* EXEC SQL ROLLBACK; */ 

{
                     struct sqlexd sqlstm;
                     sqlstm.sqlvsn = 10;
                     sqlstm.arrsiz = 41;
                     sqlstm.sqladtp = &sqladt;
                     sqlstm.sqltdsp = &sqltds;
                     sqlstm.iters = (unsigned int  )1;
                     sqlstm.offset = (unsigned int  )1341;
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
         }
      }
   }
   /* Values are associated with a business interval */
   else
   {
      /* Step through data list and insert, update, or delete in the
         interval table */
      for (updateIndex = 0; updateIndex < updateCount ; updateIndex++)
      {
         sqlUpdateRecord = &updateList[updateIndex];

         /* Get the destination interval times and the source data window
            times for this record */
         if ((result = SqlTimeInterval(sqlUpdateRecord, 
                                    sqlUpdateRecord->interval,
                                    &sqlIntervalTime,
                                    refSource)) != OK)
         {
            FilePrintError(LOGFILE, "Problem calculating interval times for copying interval data from r_base\n");
            return (ERROR);
         }

         /* Get count of data in the source data window */
         /* EXEC SQL
            SELECT COUNT(*) INTO :sqlCountSource
            FROM r_base 
            WHERE site_datatype_id = :sqlUpdateRecord->siteDatatypeId
               AND interval = :sqlUpdateRecord->interval
               AND overwrite_flag IS NULL
               AND start_date_time >= TO_DATE (:sqlIntervalTime.begSource,
                                               'DD-MON-YYYY HH24:MI:SS')
               AND end_date_time <= TO_DATE (:sqlIntervalTime.endSource,
                                            'DD-MON-YYYY HH24:MI:SS'); */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "select count(*)  into :b0  from r_base where ((((sit\
e_datatype_id=:b1 and  interval =:b2) and overwrite_flag is null ) and start_d\
ate_time>=TO_DATE(:b3,'DD-MON-YYYY HH24:MI:SS')) and end_date_time<=TO_DATE(:b\
4,'DD-MON-YYYY HH24:MI:SS'))";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1356;
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
         sqlstm.sqhstv[1] = (         void  *)&(sqlUpdateRecord->siteDatatypeId);
         sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)(sqlUpdateRecord->interval);
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
                       "Problem querying for count of data in r_base\n")
                        )!= OK)
         {
            /* EXEC SQL ROLLBACK; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )1391;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


            return (ERROR);
         }

         /* If none are found, delete any value that may exist in the 
            destination interval */
         if (sqlCountSource == 0)
         {
            /* EXEC SQL EXECUTE del USING :sqlIntervalTime.begInterval, :sqlIntervalTime.endInterval; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )1406;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)(sqlIntervalTime.begInterval);
            sqlstm.sqhstl[0] = (unsigned int  )21;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)(sqlIntervalTime.endInterval);
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



            /* TRUE used in second argument to SqlFilePrintErrorHandler because
               it is OK if no rows were processed above - means a value hadn't 
               been previously copied to the destination table */
            if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE, 
                          "Problem deleting data record from %s\n", 
                          intervalTable)) != OK)
            {
               /* EXEC SQL ROLLBACK; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 41;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )1429;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


               return (ERROR);
            }        
         }
         /* Data exists in the source data window */
         else
         {
            /* Retrieve the source data value with the most recent 
               date_time_loaded */
            /* Declare sourcedata_cursor for selecting r_base source data 
               and order by descending date_time_loaded */
            /* EXEC SQL DECLARE sourceData_cursor CURSOR FOR
               SELECT site_datatype_id,
                      value
               FROM r_base
               WHERE site_datatype_id = :sqlUpdateRecord->siteDatatypeId
                  AND interval = :sqlUpdateRecord->interval
                  AND overwrite_flag IS NULL
                  AND start_date_time >= TO_DATE (:sqlIntervalTime.begSource,
                                      'DD-MON-YYYY HH24:MI:SS')
                  AND end_date_time <= TO_DATE (:sqlIntervalTime.endSource,
                                      'DD-MON-YYYY HH24:MI:SS')
               ORDER BY date_time_loaded DESC; */ 


            if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
               "Problem declaring sourceData_cursor\n")) != OK)
            {
               return (ERROR);
            }

            /* Open sourceData_cursor */
            /* EXEC SQL OPEN sourceData_cursor; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = sq0040;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )1444;
            sqlstm.selerr = (unsigned short)1;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (         void  *)&(sqlUpdateRecord->siteDatatypeId);
            sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         void  *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned int  )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (         void  *)(sqlUpdateRecord->interval);
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
               "Problem opening sourceData_cursor\n")) != OK)
            {
               return (ERROR);
            }

            /* Fetch first r_base entry with sourcedData_cursor - this is
               the one with the most recent date_time_loaded - don't
               need to load any others that may be in the active set */
            /* EXEC SQL FETCH sourceData_cursor
               INTO :sqlDataRecord.siteDatatypeId,
                    :sqlDataRecord.value; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )1475;
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
                "Problem fetching sourceData_cursor\n")) != OK)
            {
               /* EXEC SQL ROLLBACK; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 41;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.iters = (unsigned int  )1;
               sqlstm.offset = (unsigned int  )1498;
               sqlstm.cud = sqlcud0;
               sqlstm.sqlest = (unsigned char  *)&sqlca;
               sqlstm.sqlety = (unsigned short)256;
               sqlstm.occurs = (unsigned int  )0;
               sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


               return (ERROR);
            }

            /* Close sourceData_cursor */
            /* EXEC SQL CLOSE sourceData_cursor; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )1513;
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
            sqlstm.arrsiz = 41;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )1528;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



            /* Initialize the derivation flag to an empty string */
            strcpy (sqlDataRecord.derivationFlag, EMPTY_STRING);

            /* Is record valid for updating or inserting */
            if ((result = CheckCopy(&sqlDataRecord, refSource, &valid))
                 != OK)
            {
               FilePrintError(LOGFILE, "Problem verifying data record for copy\n");
               return (ERROR);
            }

            /* If valid, fill in other members of the data record */
            if (valid != 0)
            {
               /* Assign beginning of interval time to startDateTime member */
               strcpy (sqlDataRecord.startDateTime,
                       sqlIntervalTime.begInterval);

               /* Assign end of interval time to endDateTime member */
               strcpy (sqlDataRecord.endDateTime,
                       sqlIntervalTime.endInterval);

               /* Assign the dateTimeLoaded member */
               strcpy (sqlDataRecord.dateTimeLoaded, DATETIMELOADED);

               /* Assign the method_id for copy to the method member */
               sqlDataRecord.methodId = 6;

               /* Assign equivalent of NULL to the overwrite member */
               sqlDataRecord.overwrite = EMPTY_CHAR;    

               /* Assign indicator variable for the derivation flag
                  for future writing to the database */
               if (!strcmp(sqlDataRecord.derivationFlag, EMPTY_STRING))
               {
                  sqlDerFlag_ind = -1;
               }
               else
               {
                  sqlDerFlag_ind = 0;
               }
            }

            /* Check to see if there is a value in the destination slot */   
            sprintf(selectStatement, "SELECT COUNT(*) FROM %s WHERE site_datatype_id = %d AND start_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS') AND end_date_time = TO_DATE ('%s', 'DD-MON-YYYY HH24:MI:SS')", intervalTable, sqlDataRecord.siteDatatypeId, sqlDataRecord.startDateTime, sqlDataRecord.endDateTime);

            if ((result = SqlSelectCount(selectStatement, &countDestination))                  != OK)
            {
               FilePrintError(LOGFILE, "Problem querying count for destination slot\n");
               return (ERROR);
            }

            /* If value doesn't exist in destination table, it is an insert */
            if (countDestination == 0)
            {
               /* If record is valid, insert it to destination table - invalid
                  records do not get inserted */
               if (valid != 0)
               {
                  /* EXEC SQL EXECUTE ins USING :sqlDataRecord.siteDatatypeId, :sqlDataRecord.startDateTime, :sqlDataRecord.endDateTime, :sqlDataRecord.value, :sqlDataRecord.methodId, :sqlDataRecord.derivationFlag :sqlDerFlag_ind, :sqlDataRecord.dateTimeLoaded; */ 

{
                  struct sqlexd sqlstm;
                  sqlstm.sqlvsn = 10;
                  sqlstm.arrsiz = 41;
                  sqlstm.sqladtp = &sqladt;
                  sqlstm.sqltdsp = &sqltds;
                  sqlstm.stmt = "";
                  sqlstm.iters = (unsigned int  )1;
                  sqlstm.offset = (unsigned int  )1543;
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
                  sqlstm.sqhstv[1] = (         void  *)(sqlDataRecord.startDateTime);
                  sqlstm.sqhstl[1] = (unsigned int  )21;
                  sqlstm.sqhsts[1] = (         int  )0;
                  sqlstm.sqindv[1] = (         void  *)0;
                  sqlstm.sqinds[1] = (         int  )0;
                  sqlstm.sqharm[1] = (unsigned int  )0;
                  sqlstm.sqadto[1] = (unsigned short )0;
                  sqlstm.sqtdso[1] = (unsigned short )0;
                  sqlstm.sqhstv[2] = (         void  *)(sqlDataRecord.endDateTime);
                  sqlstm.sqhstl[2] = (unsigned int  )21;
                  sqlstm.sqhsts[2] = (         int  )0;
                  sqlstm.sqindv[2] = (         void  *)0;
                  sqlstm.sqinds[2] = (         int  )0;
                  sqlstm.sqharm[2] = (unsigned int  )0;
                  sqlstm.sqadto[2] = (unsigned short )0;
                  sqlstm.sqtdso[2] = (unsigned short )0;
                  sqlstm.sqhstv[3] = (         void  *)&(sqlDataRecord.value);
                  sqlstm.sqhstl[3] = (unsigned int  )sizeof(double);
                  sqlstm.sqhsts[3] = (         int  )0;
                  sqlstm.sqindv[3] = (         void  *)0;
                  sqlstm.sqinds[3] = (         int  )0;
                  sqlstm.sqharm[3] = (unsigned int  )0;
                  sqlstm.sqadto[3] = (unsigned short )0;
                  sqlstm.sqtdso[3] = (unsigned short )0;
                  sqlstm.sqhstv[4] = (         void  *)&(sqlDataRecord.methodId);
                  sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
                  sqlstm.sqhsts[4] = (         int  )0;
                  sqlstm.sqindv[4] = (         void  *)0;
                  sqlstm.sqinds[4] = (         int  )0;
                  sqlstm.sqharm[4] = (unsigned int  )0;
                  sqlstm.sqadto[4] = (unsigned short )0;
                  sqlstm.sqtdso[4] = (unsigned short )0;
                  sqlstm.sqhstv[5] = (         void  *)(sqlDataRecord.derivationFlag);
                  sqlstm.sqhstl[5] = (unsigned int  )20;
                  sqlstm.sqhsts[5] = (         int  )0;
                  sqlstm.sqindv[5] = (         void  *)&sqlDerFlag_ind;
                  sqlstm.sqinds[5] = (         int  )0;
                  sqlstm.sqharm[5] = (unsigned int  )0;
                  sqlstm.sqadto[5] = (unsigned short )0;
                  sqlstm.sqtdso[5] = (unsigned short )0;
                  sqlstm.sqhstv[6] = (         void  *)(sqlDataRecord.dateTimeLoaded);
                  sqlstm.sqhstl[6] = (unsigned int  )21;
                  sqlstm.sqhsts[6] = (         int  )0;
                  sqlstm.sqindv[6] = (         void  *)0;
                  sqlstm.sqinds[6] = (         int  )0;
                  sqlstm.sqharm[6] = (unsigned int  )0;
                  sqlstm.sqadto[6] = (unsigned short )0;
                  sqlstm.sqtdso[6] = (unsigned short )0;
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



                  if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME,
                                FALSE, 
                                "Problem inserting data record into %s\n", 
                                 intervalTable)) != OK)
                  {
                     /* EXEC SQL ROLLBACK; */ 

{
                     struct sqlexd sqlstm;
                     sqlstm.sqlvsn = 10;
                     sqlstm.arrsiz = 41;
                     sqlstm.sqladtp = &sqladt;
                     sqlstm.sqltdsp = &sqltds;
                     sqlstm.iters = (unsigned int  )1;
                     sqlstm.offset = (unsigned int  )1586;
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

            /* Value already exists in destination slot, so is an update */
            else
            {
               /* If new record to update the old is not valid, it is the 
                  equivalent of just deleting the old record */
               if (valid == 0)
               {
                  /* EXEC SQL EXECUTE del USING :sqlDataRecord.startDateTime, :sqlDataRecord.endDateTime; */ 

{
                  struct sqlexd sqlstm;
                  sqlstm.sqlvsn = 10;
                  sqlstm.arrsiz = 41;
                  sqlstm.sqladtp = &sqladt;
                  sqlstm.sqltdsp = &sqltds;
                  sqlstm.stmt = "";
                  sqlstm.iters = (unsigned int  )1;
                  sqlstm.offset = (unsigned int  )1601;
                  sqlstm.cud = sqlcud0;
                  sqlstm.sqlest = (unsigned char  *)&sqlca;
                  sqlstm.sqlety = (unsigned short)256;
                  sqlstm.occurs = (unsigned int  )0;
                  sqlstm.sqhstv[0] = (         void  *)(sqlDataRecord.startDateTime);
                  sqlstm.sqhstl[0] = (unsigned int  )21;
                  sqlstm.sqhsts[0] = (         int  )0;
                  sqlstm.sqindv[0] = (         void  *)0;
                  sqlstm.sqinds[0] = (         int  )0;
                  sqlstm.sqharm[0] = (unsigned int  )0;
                  sqlstm.sqadto[0] = (unsigned short )0;
                  sqlstm.sqtdso[0] = (unsigned short )0;
                  sqlstm.sqhstv[1] = (         void  *)(sqlDataRecord.endDateTime);
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



                  if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME,
                                FALSE, 
                                "Problem deleting data record from %s\n", 
                                intervalTable)) != OK)
                  {
                     /* EXEC SQL ROLLBACK; */ 

{
                     struct sqlexd sqlstm;
                     sqlstm.sqlvsn = 10;
                     sqlstm.arrsiz = 41;
                     sqlstm.sqladtp = &sqladt;
                     sqlstm.sqltdsp = &sqltds;
                     sqlstm.iters = (unsigned int  )1;
                     sqlstm.offset = (unsigned int  )1624;
                     sqlstm.cud = sqlcud0;
                     sqlstm.sqlest = (unsigned char  *)&sqlca;
                     sqlstm.sqlety = (unsigned short)256;
                     sqlstm.occurs = (unsigned int  )0;
                     sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


                     return (ERROR);
                  }
               }
               /* New record is valid and can be used to update the old one */
               else
               {
                  /* EXEC SQL EXECUTE upd USING :sqlDataRecord.value, :sqlDataRecord.methodId, :sqlDataRecord.derivationFlag :sqlDerFlag_ind, :sqlDataRecord.dateTimeLoaded,:sqlDataRecord.siteDatatypeId, :sqlDataRecord.startDateTime, :sqlDataRecord.endDateTime; */ 

{
                  struct sqlexd sqlstm;
                  sqlstm.sqlvsn = 10;
                  sqlstm.arrsiz = 41;
                  sqlstm.sqladtp = &sqladt;
                  sqlstm.sqltdsp = &sqltds;
                  sqlstm.stmt = "";
                  sqlstm.iters = (unsigned int  )1;
                  sqlstm.offset = (unsigned int  )1639;
                  sqlstm.cud = sqlcud0;
                  sqlstm.sqlest = (unsigned char  *)&sqlca;
                  sqlstm.sqlety = (unsigned short)256;
                  sqlstm.occurs = (unsigned int  )0;
                  sqlstm.sqhstv[0] = (         void  *)&(sqlDataRecord.value);
                  sqlstm.sqhstl[0] = (unsigned int  )sizeof(double);
                  sqlstm.sqhsts[0] = (         int  )0;
                  sqlstm.sqindv[0] = (         void  *)0;
                  sqlstm.sqinds[0] = (         int  )0;
                  sqlstm.sqharm[0] = (unsigned int  )0;
                  sqlstm.sqadto[0] = (unsigned short )0;
                  sqlstm.sqtdso[0] = (unsigned short )0;
                  sqlstm.sqhstv[1] = (         void  *)&(sqlDataRecord.methodId);
                  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
                  sqlstm.sqhsts[1] = (         int  )0;
                  sqlstm.sqindv[1] = (         void  *)0;
                  sqlstm.sqinds[1] = (         int  )0;
                  sqlstm.sqharm[1] = (unsigned int  )0;
                  sqlstm.sqadto[1] = (unsigned short )0;
                  sqlstm.sqtdso[1] = (unsigned short )0;
                  sqlstm.sqhstv[2] = (         void  *)(sqlDataRecord.derivationFlag);
                  sqlstm.sqhstl[2] = (unsigned int  )20;
                  sqlstm.sqhsts[2] = (         int  )0;
                  sqlstm.sqindv[2] = (         void  *)&sqlDerFlag_ind;
                  sqlstm.sqinds[2] = (         int  )0;
                  sqlstm.sqharm[2] = (unsigned int  )0;
                  sqlstm.sqadto[2] = (unsigned short )0;
                  sqlstm.sqtdso[2] = (unsigned short )0;
                  sqlstm.sqhstv[3] = (         void  *)(sqlDataRecord.dateTimeLoaded);
                  sqlstm.sqhstl[3] = (unsigned int  )21;
                  sqlstm.sqhsts[3] = (         int  )0;
                  sqlstm.sqindv[3] = (         void  *)0;
                  sqlstm.sqinds[3] = (         int  )0;
                  sqlstm.sqharm[3] = (unsigned int  )0;
                  sqlstm.sqadto[3] = (unsigned short )0;
                  sqlstm.sqtdso[3] = (unsigned short )0;
                  sqlstm.sqhstv[4] = (         void  *)&(sqlDataRecord.siteDatatypeId);
                  sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
                  sqlstm.sqhsts[4] = (         int  )0;
                  sqlstm.sqindv[4] = (         void  *)0;
                  sqlstm.sqinds[4] = (         int  )0;
                  sqlstm.sqharm[4] = (unsigned int  )0;
                  sqlstm.sqadto[4] = (unsigned short )0;
                  sqlstm.sqtdso[4] = (unsigned short )0;
                  sqlstm.sqhstv[5] = (         void  *)(sqlDataRecord.startDateTime);
                  sqlstm.sqhstl[5] = (unsigned int  )21;
                  sqlstm.sqhsts[5] = (         int  )0;
                  sqlstm.sqindv[5] = (         void  *)0;
                  sqlstm.sqinds[5] = (         int  )0;
                  sqlstm.sqharm[5] = (unsigned int  )0;
                  sqlstm.sqadto[5] = (unsigned short )0;
                  sqlstm.sqtdso[5] = (unsigned short )0;
                  sqlstm.sqhstv[6] = (         void  *)(sqlDataRecord.endDateTime);
                  sqlstm.sqhstl[6] = (unsigned int  )21;
                  sqlstm.sqhsts[6] = (         int  )0;
                  sqlstm.sqindv[6] = (         void  *)0;
                  sqlstm.sqinds[6] = (         int  )0;
                  sqlstm.sqharm[6] = (unsigned int  )0;
                  sqlstm.sqadto[6] = (unsigned short )0;
                  sqlstm.sqtdso[6] = (unsigned short )0;
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



                  if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME,
                                FALSE, 
                                "Problem updating data record in %s\n", 
                                intervalTable)) != OK)
                  {
                     /* EXEC SQL ROLLBACK; */ 

{
                     struct sqlexd sqlstm;
                     sqlstm.sqlvsn = 10;
                     sqlstm.arrsiz = 41;
                     sqlstm.sqladtp = &sqladt;
                     sqlstm.sqltdsp = &sqltds;
                     sqlstm.iters = (unsigned int  )1;
                     sqlstm.offset = (unsigned int  )1682;
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
         }
      }
   }

   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1697;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
} 

/****************************************************************************/
#define FUNC_NAME "SqlTimeInterval"

int SqlTimeInterval(UPDATE_RECORD* updateRecord, SQL_INTERVAL destInterval,
                    INTERVAL_TIME* timeRecord, REF_SOURCE* refSource)
{
   /* Find the destination interval and source window for a given date/time */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      int sqlCurrentCount;
      int sqlOffset;
      SQL_INTERVAL sqlCurrentInterval;
      SQL_INTERVAL_UNIT sqlCurrentUnit;
      SQL_INTERVAL sqlDestInterval;
      SQL_INTERVAL_UNIT sqlDestUnit;
   /* EXEC SQL END DECLARE SECTION; */ 


   int positiveValue;
   int result;
   int valid;
   SQL_DATE begInterval;
   SQL_DATE endInterval;
   SQL_DATE begSource;
   SQL_DATE endSource;
   SQL_INTERVAL previousInterval;


   /* Set begInterval equal to given startDateTime */
   strcpy (begInterval, updateRecord->startDateTime);

   /* Set sqlDestInterval variable */
   strcpy (sqlDestInterval, destInterval);   

   /* Subtract the time offset, if applicable */
   if (refSource != NULL)
   {
      /* If time offset is specified */
      if (refSource->timeOffsetMinutes != VALUE_NULL)
      {
         if (refSource->timeOffsetMinutes < 0 )
         {
            /* Subtracting a negative so need ADDITION of the 
               positive value for SqlDateMath */
            positiveValue = (-1) * refSource->timeOffsetMinutes; 
            if ((result = SqlDateMath(ADDITION, begInterval, begInterval, 
                                      positiveValue,
                                      MINUTE)) != OK)
            {
               FilePrintError(LOGFILE, "Problem subtracting time offset");
               return (ERROR);
            }
         }
         else
         {
            /* Call SUBTRACTION */
            if ((result = SqlDateMath(SUBTRACTION, begInterval, begInterval, 
                                      refSource->timeOffsetMinutes,
                                      MINUTE)) != OK)
            {
               FilePrintError(LOGFILE, "Problem subtracting time offset");
               return (ERROR);
            }
         }
      }
   }

   /* Set currentInterval to be the destination interval */
   strcpy (sqlCurrentInterval, sqlDestInterval);

   /* Subtract interval redefinitions for the destination interval down to,
      but not including the interval of the given updateRecord */
   while (strcmp (sqlCurrentInterval, updateRecord->interval))
   {
      /* Check to see if there is an interval redefinition row for the 
         current interval (can only be one since this is the primary key) */
      /* EXEC SQL
         SELECT COUNT(*) INTO :sqlCurrentCount
         FROM ref_interval_redefinition 
         WHERE interval = :sqlCurrentInterval; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "select count(*)  into :b0  from ref_interval_redefiniti\
on where  interval =:b1";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )1712;
      sqlstm.selerr = (unsigned short)1;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&sqlCurrentCount;
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)sqlCurrentInterval;
      sqlstm.sqhstl[1] = (unsigned int  )33;
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
                    "Problem getting count from ref_interval_redefinition\n"))
                     != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1735;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      }

      /* If redefinition exists, retrieve and subtract it from begInterval */
      if (sqlCurrentCount != 0)
      {
         /* EXEC SQL SELECT time_offset, offset_units
            INTO :sqlOffset, :sqlCurrentUnit
            FROM ref_interval_redefinition
            WHERE interval = :sqlCurrentInterval; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "select time_offset ,offset_units into :b0,:b1  from \
ref_interval_redefinition where  interval =:b2";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1750;
         sqlstm.selerr = (unsigned short)1;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)&sqlOffset;
         sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)sqlCurrentUnit;
         sqlstm.sqhstl[1] = (unsigned int  )10;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)sqlCurrentInterval;
         sqlstm.sqhstl[2] = (unsigned int  )33;
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
}



         if ((result = SqlFilePrintErrorHandler (LOGFILE, FUNC_NAME, FALSE,
            "Problem selecting interval redefinition for the interval %s\n",
             sqlCurrentInterval)) != OK)
         {
            return (ERROR);
         }

         if (sqlOffset < 0)
         {
            /* Subtracting a negative so need ADDITION of the positive
               value in SqlDateMath */
            positiveValue = (-1) * sqlOffset;
            if ((result = SqlDateMath(ADDITION, begInterval, begInterval, 
                                      positiveValue, sqlCurrentUnit)) != OK)
            {
               FilePrintError(LOGFILE, "Problem subtracting interval redefinition for %s\n",
                           sqlCurrentInterval);
               return (ERROR);
            } 
         }
         else
         {
            /* Call SUBTRACTION */
            if ((result = SqlDateMath(SUBTRACTION, begInterval, begInterval, 
                                      sqlOffset, sqlCurrentUnit)) != OK)
            {
               FilePrintError(LOGFILE, "Problem subtracting interval redefinition for %s\n",
                           sqlCurrentInterval);
               return (ERROR);
            }
         }
      }

      /* Decrement to the previous interval */
      if ((result = SqlGetPreviousIntName(sqlCurrentInterval,
                    previousInterval,
                    &valid)) != OK)
      {
         FilePrintError(LOGFILE, "Problem finding previous interval name for interval %s\n", sqlCurrentInterval);
         return (ERROR);
      }

      /* If previous interval name is not valid, means you have reached the
         smallest business interval, so can break loop and continue */
      if (valid == 0)
      {
         break;
      }

      strcpy (sqlCurrentInterval, previousInterval);
   }

   /* Truncate begInterval to part corresponding to destination interval */
   /* EXEC SQL SELECT interval_unit
      INTO :sqlDestUnit
      FROM hdb_interval
      WHERE interval_name = :sqlDestInterval; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select interval_unit into :b0  from hdb_interval where int\
erval_name=:b1";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1777;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)sqlDestUnit;
   sqlstm.sqhstl[0] = (unsigned int  )10;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)sqlDestInterval;
   sqlstm.sqhstl[1] = (unsigned int  )33;
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



   if ((result = SqlFilePrintErrorHandler (LOGFILE, FUNC_NAME, FALSE,
      "Problem selecting sql_unit for the interval %s\n",
       sqlDestInterval)) != OK)
   {
      return (ERROR);
   }

   if ((result = SqlDateTrunc(begInterval, sqlDestUnit, begInterval)) != OK)
   {
      FilePrintError(LOGFILE, "Problem truncating date to %s\n", sqlDestInterval);
      return (ERROR);
   }      

   /* Set currentInterval to be the destination interval */
   strcpy (sqlCurrentInterval, sqlDestInterval);

   /* Add back in the interval redefinitions for the destination interval
      down to, but not including the interval of the given updateRecord */
   while (strcmp (sqlCurrentInterval, updateRecord->interval))
   {
      /* Check to see if there is an interval redefinition row for the 
         current interval (can only be one since this is the primary key) */
      /* EXEC SQL
         SELECT COUNT(*) INTO :sqlCurrentCount
         FROM ref_interval_redefinition 
         WHERE interval = :sqlCurrentInterval; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "select count(*)  into :b0  from ref_interval_redefiniti\
on where  interval =:b1";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )1800;
      sqlstm.selerr = (unsigned short)1;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&sqlCurrentCount;
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)sqlCurrentInterval;
      sqlstm.sqhstl[1] = (unsigned int  )33;
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
                    "Problem getting count from ref_interval_redefinition\n"))
                     != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1823;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      }

      /* If redefinition exists, retrieve and add it to begInterval */
      if (sqlCurrentCount != 0)
      {
         /* EXEC SQL SELECT time_offset, offset_units
            INTO :sqlOffset, :sqlCurrentUnit
            FROM ref_interval_redefinition
            WHERE interval = :sqlCurrentInterval; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "select time_offset ,offset_units into :b0,:b1  from \
ref_interval_redefinition where  interval =:b2";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1838;
         sqlstm.selerr = (unsigned short)1;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (         void  *)&sqlOffset;
         sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         void  *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned int  )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (         void  *)sqlCurrentUnit;
         sqlstm.sqhstl[1] = (unsigned int  )10;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         void  *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned int  )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (         void  *)sqlCurrentInterval;
         sqlstm.sqhstl[2] = (unsigned int  )33;
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
}



         if ((result = SqlFilePrintErrorHandler (LOGFILE, FUNC_NAME, FALSE,
            "Problem selecting interval redefinition for the interval %s\n",
             sqlCurrentInterval)) != OK)
         {
            return (ERROR);
         }

         if (sqlOffset < 0)
         {
            /* Are adding a negative so need to call SUBTRACTION of the
               positive value for SqlDateMath */
            positiveValue = (-1) * sqlOffset;
            if ((result = SqlDateMath(SUBTRACTION, begInterval, begInterval, 
                                      positiveValue, sqlCurrentUnit)) != OK)
            {
               FilePrintError(LOGFILE, "Problem adding interval redefinition for %s\n",
                           sqlCurrentInterval);
               return (ERROR);
            }            
         }
         else
         {
            /* Call ADDITION */
            if ((result = SqlDateMath(ADDITION, begInterval, begInterval, 
                                      sqlOffset, sqlCurrentUnit)) != OK)
            {
               FilePrintError(LOGFILE, "Problem adding interval redefinition for %s\n",
                           sqlCurrentInterval);
               return (ERROR);
            }
         }
      }

      /* Decrement to the previous interval */
      if ((result = SqlGetPreviousIntName(sqlCurrentInterval,
                    previousInterval,
                    &valid)) != OK)
      {
         FilePrintError(LOGFILE, "Problem finding previous interval name for interval %s\n", sqlCurrentInterval);
         return (ERROR);
      }

      /* If previous interval name is not valid, means you have reached the
         smallest business interval, so can break loop and continue */
      if (valid == 0)
      {
         break;
      }

      strcpy (sqlCurrentInterval, previousInterval);
   }

   /* Result here is the beginning of the destination interval that the 
      original given value falls into */
   strcpy (timeRecord->begInterval, begInterval);

   /* End of the destination interval is the beginning time plus the 
      destination interval period */
   if ((result = SqlDateMath(ADDITION, begInterval, endInterval, 
                             1, sqlDestUnit)) != OK)
   {
      FilePrintError(LOGFILE, "Problem adding destination interval length for %s\n",
         sqlDestInterval);
      return (ERROR);
   }
   strcpy (timeRecord->endInterval, endInterval);

   /* Beginning of the source data window is beginning of the destination
      interval plus the time offset, if one is applicable */
   strcpy (begSource, begInterval); 
   if (refSource != NULL)
   {
      /* If time offset is specified */
      if (refSource->timeOffsetMinutes != VALUE_NULL)
      {
         if (refSource->timeOffsetMinutes < 0)
         {
            /* Are adding a negative so need to call SUBTRACTION of the
               positive value for SqlDateMath */
            positiveValue = (-1) * refSource->timeOffsetMinutes;
            if ((result = SqlDateMath(SUBTRACTION, begSource, begSource, 
                                      positiveValue,
                                      MINUTE)) != OK)
            {
               FilePrintError(LOGFILE, "Problem adding time offset");
               return (ERROR);
            }
         }
         else
         {
            /* Call ADDITION */
            if ((result = SqlDateMath(ADDITION, begSource, begSource, 
                                      refSource->timeOffsetMinutes,
                                      MINUTE)) != OK)
            {
               FilePrintError(LOGFILE, "Problem adding time offset");
               return (ERROR);
            }
         }
      }
   }
   strcpy (timeRecord->begSource, begSource);

   /* End of the source data window is beginning source window time plus the
      destination interval period */
   if ((result = SqlDateMath(ADDITION, begSource, endSource, 
                             1, sqlDestUnit)) != OK)
   {
      FilePrintError(LOGFILE, "Problem adding destination interval length for %s\n",
         sqlDestInterval);
      return (ERROR);
   }
   strcpy (timeRecord->endSource, endSource);   

   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1865;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlTimeShift"

int SqlTimeShift(INTERVAL_TIME* timeRecord, SQL_INTERVAL destInterval,
                 DATE_MATH_TYPES op, INTERVAL_TIME* newRecord)
{
   /* This function shifts a time record forward or backward by one
      interval */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      SQL_INTERVAL sqlDestInterval;
      SQL_INTERVAL_UNIT sqlDestUnit;
   /* EXEC SQL END DECLARE SECTION; */ 


   int result;

   /* Initialize the sql variable */
   strcpy(sqlDestInterval, destInterval);

   /* Retrieve the sql unit for the interval */
   /* EXEC SQL SELECT interval_unit
      INTO :sqlDestUnit
      FROM hdb_interval
      WHERE interval_name = :sqlDestInterval; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select interval_unit into :b0  from hdb_interval where int\
erval_name=:b1";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1880;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)sqlDestUnit;
   sqlstm.sqhstl[0] = (unsigned int  )10;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)sqlDestInterval;
   sqlstm.sqhstl[1] = (unsigned int  )33;
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



   if ((result = SqlFilePrintErrorHandler (LOGFILE, FUNC_NAME, FALSE,
      "Problem selecting interval_unit for the interval %s\n",
       sqlDestInterval)) != OK)
   {
      return (ERROR);
   }

   /* Each member of the time structure needs to be adjusted */
   if ((result = SqlDateMath(op, timeRecord->begInterval,
                             newRecord->begInterval, 
                             1, sqlDestUnit)) != OK)
   {
      FilePrintError(LOGFILE, "Problem shifting begInterval time in time record\n");
      return (ERROR);
   }
   if ((result = SqlDateMath(op, timeRecord->endInterval,
                             newRecord->endInterval, 
                             1, sqlDestUnit)) != OK)
   {
      FilePrintError(LOGFILE, "Problem shifting endInterval time in time record\n");
      return (ERROR);
   }
   if ((result = SqlDateMath(op, timeRecord->begSource,
                             newRecord->begSource, 
                             1, sqlDestUnit)) != OK)
   {
      FilePrintError(LOGFILE, "Problem shifting begSource time in time record\n");
      return (ERROR);
   }
   if ((result = SqlDateMath(op, timeRecord->endSource,
                             newRecord->endSource, 
                             1, sqlDestUnit)) != OK)
   {
      FilePrintError(LOGFILE, "Problem shifting endSource time in time record\n");
      return (ERROR);
   }

   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1903;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlCreateTable"

int SqlCreateTable()
{
   /* Creating the r_interval_update table for the internal use of
      the derivation application also acts as a semafore to assure that
      only one copy of the application is running at a time on a 
      database */

   int result;

   /* Create r_interval_update table */
   /* EXEC SQL CREATE TABLE r_interval_update
      (site_datatype_id             NUMBER,
       interval                     VARCHAR2(16),
       start_date_time              DATE,
       end_date_time                DATE,
       CONSTRAINT  r_interval_update_pk
          PRIMARY KEY (site_datatype_id, interval,
                    start_date_time, end_date_time)
      ); */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "create TABLE r_interval_update ( site_datatype_id NUMBER ,\
 interval VARCHAR2 ( 16 ) , start_date_time DATE , end_date_time DATE , CONSTR\
AINT r_interval_update_pk PRIMARY KEY ( site_datatype_id , interval , start_da\
te_time , end_date_time ) )";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1918;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
      "Problem creating r_interval_update table\n")) != OK)
   {
         return (ERROR);
   }

   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1933;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlDropTable"

int SqlDropTable()
{
   /* This function drops the r_interval_update table that was used
      during the run of the derivation application.  This also releases
      the semafore effect of the table so that another copy of the derivation 
      application can now be run. */

   int result;

   /* Drop r_interval_update table */
   /* EXEC SQL DROP TABLE r_interval_update; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "drop TABLE r_interval_update";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1948;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
      "Problem dropping the r_interval_update table\n")) != OK)
   {
         return (ERROR);
   }

   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1963;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "LoadDestination"

int LoadDestination(REF_DESTINATION* sourceSpec,
                    IND_DESTINATION* indicatorSource,
                    REF_DESTINATION* outputSpec)
{
   /* This function takes a specification that has been loaded with sql 
      from the ref_derivation_destination table and does NULL checking
      based on the associated indicator variable while loading the spec
      to another REF_DESTINATION structure */
  
         outputSpec->baseSDI = sourceSpec->baseSDI;
         if (indicatorSource->compoundingSourceSDI_ind == -1)
         {
            outputSpec->compoundingSourceSDI = VALUE_NULL;
         }
         else
         {
            outputSpec->compoundingSourceSDI = 
                                     sourceSpec->compoundingSourceSDI;
         }
         if (indicatorSource->compoundingSourceInterval_ind == -1)
         {
            strcpy (outputSpec->compoundingSourceInterval,
                    EMPTY_STRING);
         }
         else
         {
            strcpy (outputSpec->compoundingSourceInterval, 
                    sourceSpec->compoundingSourceInterval);
         }
         outputSpec->destSDI = sourceSpec->destSDI;
         if (indicatorSource->partialCalc_ind == -1)
         {
            outputSpec->partialCalc = EMPTY_CHAR;
         }
         else
         {
            outputSpec->partialCalc = 
                                     sourceSpec->partialCalc;
         }
         outputSpec->methodId = sourceSpec->methodId;
         if (indicatorSource->hrDesiredEopWindow_ind == -1)
         {
            outputSpec->hrDesiredEopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->hrDesiredEopWindow = 
                                    sourceSpec->hrDesiredEopWindow;
         }
         if (indicatorSource->hrRequiredEopWindow_ind == -1)
         {
            outputSpec->hrRequiredEopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->hrRequiredEopWindow = 
                                    sourceSpec->hrRequiredEopWindow;
         }
         if (indicatorSource->hrDesiredBopWindow_ind == -1)
         {
            outputSpec->hrDesiredBopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->hrDesiredBopWindow = 
                                    sourceSpec->hrDesiredBopWindow;
         }
         if (indicatorSource->hrRequiredBopWindow_ind == -1)
         {
            outputSpec->hrRequiredBopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->hrRequiredBopWindow = 
                                    sourceSpec->hrRequiredBopWindow;
         }
         if (indicatorSource->hrWindowUnit_ind == -1)
         {
            strcpy (outputSpec->hrWindowUnit, EMPTY_STRING);
         }
         else
         {
            strcpy (outputSpec->hrWindowUnit, 
                    sourceSpec->hrWindowUnit);
         }
         if (indicatorSource->hrDesiredNumSource_ind == -1)
         {
            outputSpec->hrDesiredNumSource = VALUE_NULL;
         }
         else
         {
            outputSpec->hrDesiredNumSource = 
                                    sourceSpec->hrDesiredNumSource;
         }
         if (indicatorSource->hrRequiredNumSource_ind == -1)
         {
            outputSpec->hrRequiredNumSource = VALUE_NULL;
         }
         else
         {
            outputSpec->hrRequiredNumSource = 
                                    sourceSpec->hrRequiredNumSource;
         }

         if (indicatorSource->dayDesiredEopWindow_ind == -1)
         {
            outputSpec->dayDesiredEopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->dayDesiredEopWindow = 
                                    sourceSpec->dayDesiredEopWindow;
         }
         if (indicatorSource->dayRequiredEopWindow_ind == -1)
         {
            outputSpec->dayRequiredEopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->dayRequiredEopWindow = 
                                    sourceSpec->dayRequiredEopWindow;
         }
         if (indicatorSource->dayDesiredBopWindow_ind == -1)
         {
            outputSpec->dayDesiredBopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->dayDesiredBopWindow = 
                                    sourceSpec->dayDesiredBopWindow;
         }
         if (indicatorSource->dayRequiredBopWindow_ind == -1)
         {
            outputSpec->dayRequiredBopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->dayRequiredBopWindow = 
                                    sourceSpec->dayRequiredBopWindow;
         }
         if (indicatorSource->dayWindowUnit_ind == -1)
         {
            strcpy (outputSpec->dayWindowUnit, EMPTY_STRING);
         }
         else
         {
            strcpy (outputSpec->dayWindowUnit, 
                    sourceSpec->dayWindowUnit);
         }
         if (indicatorSource->dayDesiredNumSource_ind == -1)
         {
            outputSpec->dayDesiredNumSource = VALUE_NULL;
         }
         else
         {
            outputSpec->dayDesiredNumSource = 
                                    sourceSpec->dayDesiredNumSource;
         }
         if (indicatorSource->dayRequiredNumSource_ind == -1)
         {
            outputSpec->dayRequiredNumSource = VALUE_NULL;
         }
         else
         {
            outputSpec->dayRequiredNumSource = 
                                    sourceSpec->dayRequiredNumSource;
         }
         if (indicatorSource->monDesiredEopWindow_ind == -1)
         {
            outputSpec->monDesiredEopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->monDesiredEopWindow = 
                                    sourceSpec->monDesiredEopWindow;
         }
         if (indicatorSource->monRequiredEopWindow_ind == -1)
         {
            outputSpec->monRequiredEopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->monRequiredEopWindow = 
                                    sourceSpec->monRequiredEopWindow;
         }
         if (indicatorSource->monDesiredBopWindow_ind == -1)
         {
            outputSpec->monDesiredBopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->monDesiredBopWindow = 
                                    sourceSpec->monDesiredBopWindow;
         }
         if (indicatorSource->monRequiredBopWindow_ind == -1)
         {
            outputSpec->monRequiredBopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->monRequiredBopWindow = 
                                    sourceSpec->monRequiredBopWindow;
         }
         if (indicatorSource->monWindowUnit_ind == -1)
         {
            strcpy (outputSpec->monWindowUnit, EMPTY_STRING);
         }
         else
         {
            strcpy (outputSpec->monWindowUnit, 
                    sourceSpec->monWindowUnit);
         }
         if (indicatorSource->monDesiredNumSource_ind == -1)
         {
            outputSpec->monDesiredNumSource = VALUE_NULL;
         }
         else
         {
            outputSpec->monDesiredNumSource = 
                                    sourceSpec->monDesiredNumSource;
         }
         if (indicatorSource->monRequiredNumSource_ind == -1)
         {
            outputSpec->monRequiredNumSource = VALUE_NULL;
         }
         else
         {
            outputSpec->monRequiredNumSource = 
                                    sourceSpec->monRequiredNumSource;
         }
         if (indicatorSource->yrDesiredEopWindow_ind == -1)
         {
            outputSpec->yrDesiredEopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->yrDesiredEopWindow = 
                                    sourceSpec->yrDesiredEopWindow;
         }
         if (indicatorSource->yrRequiredEopWindow_ind == -1)
         {
            outputSpec->yrRequiredEopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->yrRequiredEopWindow = 
                                    sourceSpec->yrRequiredEopWindow;
         }
         if (indicatorSource->yrDesiredBopWindow_ind == -1)
         {
            outputSpec->yrDesiredBopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->yrDesiredBopWindow = 
                                    sourceSpec->yrDesiredBopWindow;
         }
         if (indicatorSource->yrRequiredBopWindow_ind == -1)
         {
            outputSpec->yrRequiredBopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->yrRequiredBopWindow = 
                                    sourceSpec->yrRequiredBopWindow;
         }
         if (indicatorSource->yrWindowUnit_ind == -1)
         {
            strcpy (outputSpec->yrWindowUnit, EMPTY_STRING);
         }
         else
         {
            strcpy (outputSpec->yrWindowUnit, 
                    sourceSpec->yrWindowUnit);
         }
         if (indicatorSource->yrDesiredNumSource_ind == -1)
         {
            outputSpec->yrDesiredNumSource = VALUE_NULL;
         }
         else
         {
            outputSpec->yrDesiredNumSource = 
                                    sourceSpec->yrDesiredNumSource;
         }
         if (indicatorSource->yrRequiredNumSource_ind == -1)
         {
            outputSpec->yrRequiredNumSource = VALUE_NULL;
         }
         else
         {
            outputSpec->yrRequiredNumSource = 
                                    sourceSpec->yrRequiredNumSource;
         }
         if (indicatorSource->wyDesiredEopWindow_ind == -1)
         {
            outputSpec->wyDesiredEopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->wyDesiredEopWindow = 
                                    sourceSpec->wyDesiredEopWindow;
         }
         if (indicatorSource->wyRequiredEopWindow_ind == -1)
         {
            outputSpec->wyRequiredEopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->wyRequiredEopWindow = 
                                    sourceSpec->wyRequiredEopWindow;
         }
         if (indicatorSource->wyDesiredBopWindow_ind == -1)
         {
            outputSpec->wyDesiredBopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->wyDesiredBopWindow = 
                                    sourceSpec->wyDesiredBopWindow;
         }
         if (indicatorSource->wyRequiredBopWindow_ind == -1)
         {
            outputSpec->wyRequiredBopWindow = VALUE_NULL;
         }
         else
         {
            outputSpec->wyRequiredBopWindow = 
                                    sourceSpec->wyRequiredBopWindow;
         }
         if (indicatorSource->wyWindowUnit_ind == -1)
         {
            strcpy (outputSpec->wyWindowUnit, EMPTY_STRING);
         }
         else
         {
            strcpy (outputSpec->wyWindowUnit, 
                    sourceSpec->wyWindowUnit);
         }
         if (indicatorSource->wyDesiredNumSource_ind == -1)
         {
            outputSpec->wyDesiredNumSource = VALUE_NULL;
         }
         else
         {
            outputSpec->wyDesiredNumSource = 
                                    sourceSpec->wyDesiredNumSource;
         }
         if (indicatorSource->wyRequiredNumSource_ind == -1)
         {
            outputSpec->wyRequiredNumSource = VALUE_NULL;
         }
         else
         {
            outputSpec->wyRequiredNumSource = 
                                    sourceSpec->wyRequiredNumSource;
         }

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlUpdateSelect"

int SqlUpdateSelect(int siteDatatypeId,
                    SQL_INTERVAL sourceInterval,
                    int* updateCount,
                    UPDATE_RECORD** updateList )
{
   /* This function retrieves update data from the r_interval_update table */

   /* NOTE:  THE USER OF THIS METHOD IS RESPONSIBLE FOR FREEING THE MEMORY
             THAT IS ALLOCATED TO updateList BY MALLOC IF UPDATE DATA IS
             RETRIEVED */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      char sqlSelectStatement[600];
      UPDATE_RECORD* sqlUpdateRecord;
   /* EXEC SQL END DECLARE SECTION; */ 


   char selectStatement[600];
   int result;
   int updateIndex;


   /* Load rows from r_interval_update that belong to the
      site datatype and interval  */
   /* Find out how many applicable rows in r_interval_update */
   sprintf(selectStatement, "SELECT COUNT(*) FROM r_interval_update WHERE site_datatype_id = %d AND interval = '%s'", siteDatatypeId, sourceInterval);

   if ((result = SqlSelectCount(selectStatement, updateCount)) != OK)
   {
      FilePrintError(LOGFILE, "Problem querying row count in r_interval_update\n");
      return (ERROR);
   }

   /* Create array of structures to hold r_interval_update data */
   if (*updateCount)
   {
      if ((*updateList = (UPDATE_RECORD *) malloc 
         (sizeof(UPDATE_RECORD) * (*updateCount))) == NULL)
      {
          FilePrintError(LOGFILE, "Problem allocating memory for intUpdateDataList\n");
          return (ERROR);
      }
   }
   else
   {
      *updateList = NULL;
      return (OK);
   }

   /* Prepare the select statement for r_interval_update data*/
   sprintf (sqlSelectStatement, "SELECT site_datatype_id, TO_CHAR (start_date_time, 'DD-MON-YYYY HH24:MI:SS'), TO_CHAR (end_date_time, 'DD-MON-YYYY HH24:MI:SS'), interval FROM r_interval_update WHERE site_datatype_id = %d AND interval = '%s' ORDER BY start_date_time", siteDatatypeId, sourceInterval);

   /* EXEC SQL PREPARE sel FROM :sqlSelectStatement; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1978;
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

   /* Declare int_update_cursor to select r_interval_update data */
   /* EXEC SQL DECLARE int_update_cursor CURSOR FOR sel; */ 

   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE, 
                 "Problem declaring int_update_cursor for: %s...\n",
                 sqlSelectStatement)) != OK)
   {
      return (ERROR);
   }

   /* Open int_update_cursor */
   /* EXEC SQL OPEN int_update_cursor; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1997;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
                 "Problem opening int_update_cursor for\n: %s...\n",
                 sqlSelectStatement)) != OK)
   {
      return (ERROR);
   }

   /* EXEC SQL WHENEVER NOT FOUND GOTO close_int_update_cursor; */ 


   /* Fetch applicable r_interval_update data */ 
   for (updateIndex = 0; updateIndex < *updateCount ;
        updateIndex++)
   {
      sqlUpdateRecord = &((*updateList)[updateIndex]);

      /* EXEC SQL FETCH int_update_cursor INTO
                       :sqlUpdateRecord->siteDatatypeId, 
                       :sqlUpdateRecord->startDateTime,
                       :sqlUpdateRecord->endDateTime,
                       :sqlUpdateRecord->interval; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 41;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )2012;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&(sqlUpdateRecord->siteDatatypeId);
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)(sqlUpdateRecord->startDateTime);
      sqlstm.sqhstl[1] = (unsigned int  )21;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)(sqlUpdateRecord->endDateTime);
      sqlstm.sqhstl[2] = (unsigned int  )21;
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)(sqlUpdateRecord->interval);
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
      if (sqlca.sqlcode == 1403) goto close_int_update_cursor;
}


      if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, FALSE,
           "Problem fetching int_update_cursor for\n: %s...\n",
           sqlSelectStatement)) != OK)
      {
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 10;
         sqlstm.arrsiz = 41;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )2043;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)256;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


         return (ERROR);
      }
   }

   /* Close int_update_cursor */
   close_int_update_cursor:
   /* EXEC SQL CLOSE int_update_cursor; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2058;
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
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2073;
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
#define FUNC_NAME "SqlRemoveOldUpdates"

int SqlRemoveOldUpdates()
{
   /* This function removes entries from r_base_update that are over
      one year old. These could no longer function as the one latest residual
      value to keep to make sure a partial calc at the longest interval (year) 
      eventually is made final. At this point they would represent an SDI that
      no longer has new data coming in. */

   /* EXEC SQL BEGIN DECLARE SECTION; */ 

      SQL_DATE sqlLastYear;
   /* EXEC SQL END DECLARE SECTION; */ 


   int result;


   /* Sutract a year from the current run time of the app */
   if ((result = SqlDateMath(SUBTRACTION,
                             DATETIMELOADED,
                             sqlLastYear, 
                             1,
                             YEAR)) != OK)
   {
      FilePrintError(LOGFILE, "Problem finding last year's time");
      return (ERROR);
   }

   /* Delete old values from r_base_update */
   /* EXEC SQL DELETE FROM r_base_update
      WHERE end_date_time < TO_DATE (:sqlLastYear, 'DD-MON-YYYY HH24:MI:SS'); */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "delete  from r_base_update  where end_date_time<TO_DATE(:b\
0,'DD-MON-YYYY HH24:MI:SS')";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2088;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)sqlLastYear;
   sqlstm.sqhstl[0] = (unsigned int  )21;
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



   /* TRUE used in second argument to SqlFilePrintErrorHandler because it
      is OK if no rows were processed above - means there are no 
      old records to delete */
   if ((result = SqlFilePrintErrorHandler(LOGFILE, FUNC_NAME, TRUE,
      "Problem deleting old records from r_base_update \n")) != OK)
   {
         return (ERROR);
   }

   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 41;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )2107;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
   return (OK);
}
