
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
           char  filnam[17];
};
static const struct sqlcxp sqlfpn =
{
    16,
    "SqlGetDmiData.pc"
};


static unsigned int sqlctx = 5260163;


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
   unsigned char  *sqhstv[3];
   unsigned long  sqhstl[3];
            int   sqhsts[3];
            short *sqindv[3];
            int   sqinds[3];
   unsigned long  sqharm[3];
   unsigned long  *sqharc[3];
   unsigned short  sqadto[3];
   unsigned short  sqtdso[3];
} sqlstm = {12,3};

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
5,0,0,1,0,0,1053,66,0,0,0,0,0,0,128,1,97,0,0,
24,0,0,2,0,0,1041,69,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
47,0,0,2,0,0,1069,85,0,0,0,0,0,0,128,1,97,0,0,
66,0,0,2,0,0,1037,111,0,0,2,0,0,0,128,2,97,0,0,2,4,0,0,1,97,0,0,
93,0,0,2,0,0,1039,187,0,0,0,0,0,0,128,1,97,0,0,
112,0,0,3,0,0,1053,188,0,0,0,0,0,0,128,1,97,0,0,
};


/************************************************************************
* SqlGetDmiData
*
* Purpose: Get the data for a member of the linked list of dmi_header_structs.
*
* Input
*       dmi_header_struc - current list item
*       TIME_INTERVAL_TYPES - time step for which data is to be retrieved.
*
* Output
*       loaded data arrays
*
* Return
*       ERROR - data not successfully retrieved
*       OK - data successfully retrieved
*
* Author
*   Carol Marra, Dec '95 -- code for building queries and cursor management.
*   Paddy McCarthy, Dec '95 -- reworked so gets both hist and model data.
*
**********************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "dailyDmiIn.h"
#include "dbutils.h"
#include "dmi_utils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

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


#define FUNC_NAME       "GetTheValues"

int GetTheValues(char * select_statement, /* the statement                 */
                 int num_recs,            /* number of time steps to fetch */
                 data_array * data,       /* pointer to the array for data */
                 int first_member,        /* position in array for first fetched
                                               piece of data (model data queries
                                               put data in same array as hist)*/
                 char * time_step,        /* the time step of the fetched data*/
                 char * first_time_stamp) /* date of first piece of data   */
{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char statement[1000];
    SQL_DATE time_stamp,
             prev_time_stamp,
             next_time_stamp,
             hdb_data_date;
    double value;
  /* EXEC SQL END DECLARE SECTION; */ 


    int count;
    int result;

    /*
     * Get the data from the database.
     */
    strcpy(statement, select_statement);

    /* Commit to close any open transactions */
    /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 1;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5;
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



    /* EXEC SQL AT :current_dbsite DECLARE SelStatement STATEMENT; */ 

    /* EXEC SQL PREPARE SelStatement FROM :statement; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 2;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )24;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)statement;
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


    if (SqlErrorHandler(FUNC_NAME, FALSE, "Problem preparing statement.")
                         != OK) {
        return (ERROR);
    }
  
    /* EXEC SQL WHENEVER NOT FOUND goto close_c1; */ 

    /* EXEC SQL DECLARE c1 CURSOR FOR SelStatement; */ 

    if (SqlErrorHandler (FUNC_NAME, FALSE, "Problem declaring cursor c1.")
                         != OK) {
        return (ERROR);
    }

    /* ORACLE -- cannot set autocommit off, or set timeout. Verify that
	app doesn't hang. */

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


    if (SqlErrorHandler(FUNC_NAME, FALSE, "Problem opening cursor c1.")
                        != OK) {
        return (ERROR);
    }

    /*
    ** Initialize the previous_time_stamp to be one step before the first.
    **   This makes SqlDateGap() work properly for the first fetched piece
    **   of data. SqlDateGap() works by incrementing the previous_time_stamp
    **   one step and seeing if the incremented date is the same as the fetched
    **   time stamp.
    */
    if (SqlDateMath (SUBTRACTION, first_time_stamp, prev_time_stamp, 1, time_step) != OK) {
        PrintError("DMI: %s: Problem adding to previous date: %s.\n",
                    FUNC_NAME, first_time_stamp);
        return (ERROR);
    }	
    /* REMOVED call to charToNumeric here (tmp_date, prev_time_stamp) */

    count = first_member;
    while(1) {
        /*
         * NOTE: Exit condition for this loop is NOTFOUND
         *   on this fetch (or errors).
         */
        /* EXEC SQL FETCH c1 INTO :time_stamp, :value; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 3;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )66;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)256;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqfoff = (         int )0;
        sqlstm.sqfmod = (unsigned int )2;
        sqlstm.sqhstv[0] = (unsigned char  *)time_stamp;
        sqlstm.sqhstl[0] = (unsigned long )21;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         short *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned long )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (unsigned char  *)&value;
        sqlstm.sqhstl[1] = (unsigned long )sizeof(double);
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


        if (SqlErrorHandler (FUNC_NAME, TRUE,
                             "Problem fetching into hmSiteDatatype on row %d.",
                              count) != OK) {
            return (ERROR);
        }

        /* found a value. make sure there's space for it in the array */
        if (count >= (num_recs + first_member)) {
            PrintError("DMI: Fetched value will not fit in data array.\n");
            return(ERROR);
        }
     
        /* compare previous date to current date; if there's a gap, create NaN
           record for the missing date(s) */

        while ((result = SqlDateGap (prev_time_stamp,
                                     time_stamp,
                                     time_step,
                                     next_time_stamp)) == TRUE) {

            /*
             * There is no match between the fetched data and the data array.
             *   Fill in the array member with an NaN value.
             */

            /*
            ** This check affirms that there was no error in SqlDateGap.
            **   Next_time_stamp is nulled out if error occurs in SqlDateGap.
            */
            if (next_time_stamp[0] == '\0') {
      	        PrintError("DMI: The next time stamp was set inappropriately.\n");
      	        return (ERROR);
            }

	    /* REMOVED call to charToNumeric here 
	       (next_time_stamp,hdb_data_date); subbed w/ strcpy for safety */
	    strcpy (hdb_data_date, next_time_stamp);
	    
      	    data[count].is_nan = 'Y';
      	    data[count].value = NULL_IND;
            strcpy (data[count].hdb_date, hdb_data_date);
            data[count].processed_flag = 'N';
  
            strcpy (prev_time_stamp, hdb_data_date);
            count++;
        } /* end while (SqlDateGap) */

        /*
         * Check for error return from SqlDateGap
         */
        if (result == ERROR) {
            PrintError("DMI: SqlDateGap returned ERROR.\n");
            return(ERROR);
        }

        /*
         * Now there is a match btwn the fetched data and the array position.
         *   So set initialize the values of the data array with the fetched
         *     data.
         */

        data[count].is_nan = 'N',
        data[count].value = value;
	/* REMOVED call to charToNumeric here 
	   (time_stamp,hdb_data_date); subbed w/ strcpy for safety */
	strcpy (hdb_data_date, time_stamp);
	    
        strcpy (data[count].hdb_date, hdb_data_date);
        data[count].processed_flag = 'N';

        strcpy (prev_time_stamp, hdb_data_date);
        count++;
    } /* end while(1) -- Done processing fetched data. */
  
close_c1:
    /* EXEC SQL CLOSE c1; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 3;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )93;
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



    /* ORACLE -- can't set autocommit ON */

    /* Fill in NaNs for any data missing from end of time series. */
    while (count < (num_recs + first_member)) {

        if (count == first_member) { /* no data in db. the first date is not processed */
            strcpy(next_time_stamp, first_time_stamp);
        }
        else {
            if ((SqlDateMath (ADDITION, prev_time_stamp, next_time_stamp, 1,
			      time_step)) != OK) {
	        PrintError("DMI: Problem adding to previous date: %s.\n",
		            prev_time_stamp);
	        return (ERROR);
            }
	}
      
	/* REMOVED call to charToNumeric here 
	   (next_time_stamp,hdb_data_date); subbed w/ strcpy for safety */
	strcpy (hdb_data_date, next_time_stamp);

        data[count].is_nan = 'Y';
        data[count].value = NULL_IND;
        strcpy (data[count].hdb_date, hdb_data_date);
        data[count].processed_flag = 'N';
      
        strcpy(prev_time_stamp, next_time_stamp);
        count++;
    }

    return(OK);
}

#define FUNC_NAME       "SqlGetDmiData"

int SqlGetDmiData (dmi_header_struct *current, 
		   TIME_INTERVAL_TYPES timeInterval)

{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    int cnt;
    char histStatement[1000];
    char modelStatement[1000];
  /* EXEC SQL END DECLARE SECTION; */ 


  int switched = FALSE;

  /*
   * Prepare the queries and set up some dates for the queries.
   *   Items initialized by this routine are:
   *                            histStatement
   *                            modelStatement
   */
  if (PrepareForQueries(current,
			histStatement,
			modelStatement,
			timeInterval) != OK) 
    {
      PrintError("DMI: Could Not create query for select from database.\n");
      return(ERROR);

    }

  /* If the data is not located at the default database, 
     set the session appropriately */
  if (strcmp(current->data_location, dbsite_array[0].dbsite_alias))
    {
      cnt = 0;

      /* Search for the needed session in the array of sessions */
      while (cnt < number_of_db_sites) 
	{
	  if (!strcmp(dbsite_array[cnt].dbsite_alias, current->data_location))
	    {
	      break;
	    }
	  cnt++;
        }
    
      /* The appropriate member was found */
      if (cnt < number_of_db_sites) 
	{ 
	  /* Return if not connected to that session -- no way to get 
	     the data */
	  if (!dbsite_array[cnt].connected) 
	    {
	      PrintError("Not Connected to session for %s. Cannot fetch data.\n",
			 dbsite_array[cnt].dbsite_alias);
	      
	      /* switch the session back to default. */
	      /* NOTE SESSION */
	      if (SwitchSession(dbsite_array[0].dbsite_alias) != OK)
		{
		  PrintError("ERROR occurred changing to session 1 for %s\n",
			     dbsite_array[0].dbsite_alias);
		  return(ERROR);
                }

	      return(ERROR);
            }

	  /* Otherwise, attempt to switch to the needed session */
	  else 
	    {
	       /* NOTE SESSION */
	      if (SwitchSession(dbsite_array[cnt].dbsite_alias) != OK) 
		{
		  PrintError("ERROR occurred changing to session %d for %s\n",
			     dbsite_array[cnt].session_number,
			     dbsite_array[cnt].dbsite_alias);
		  return(ERROR);
		}
	      /* Switch was successful. */
	      else
		{
		  switched = TRUE;
		}
	    }
	}
      /* The name is not in the array. Exit. */
      else 
	 /* NOTE SESSION */
	{
	  PrintError("Could not find database session ID for %s.\n",
		     current->data_location);
	  return(ERROR);
        }
    }

  /* get the historical values */
  if (GetTheValues(histStatement,
		   current->number_of_hist_timesteps,
		   current->data,
		   0,
		   current->timestep,
		   current->hdb_startdate) != OK) {
    PrintError("DMI: Unable to get historical values.\n");
    return(ERROR);
  }

  /* At this point, stay connected to same database, as both model and
     historical data are stored at the location of the responsible site. */
  if ( strcmp(current->load_model_data, "Y") == 0 ) 
    {
      /* get the model values */
      if (GetTheValues(modelStatement,
		       current->number_of_model_timesteps,
		       current->data,
		       current->number_of_hist_timesteps,
		       current->timestep,
		       current->hdb_model_data_startdate) != OK) 
	{
	  PrintError("DMI: Unable to get model values.\n");
	  return(ERROR);
	}
    }
  /* reset the data array and pointers to exclude model data */
  else 
    {
      current->number_of_timesteps = current->number_of_hist_timesteps;
      current->number_of_model_timesteps = 0;
      sprintf(current->hdb_enddate, current->hdb_hist_data_enddate);
      
      current->data = realloc(current->data,
			      (sizeof(data_array) * current->number_of_hist_timesteps) );
      if (!current->data) 
	{
	  PrintError("DMI: Could not realloc data array for slot where model data is not loaded.\n");
	  return(ERROR);
	}
    }

  /* Set session back to default, if switch was made. */
  if (switched)
    {
       /* NOTE SESSION */
      if (SwitchSession(dbsite_array[0].dbsite_alias) != OK) 
	{
	  PrintError("ERROR occurred changing to default session 1 for %s\n",
		     dbsite_array[0].dbsite_alias);
	  return(ERROR);
	}
    }
  return (OK);
}

/**************************************************************************/
#define FUNC_NAME "SqlDateGap"

int SqlDateGap (SQL_DATE prev_time_stamp, /* the last time stamp handled */
                SQL_DATE time_stamp,      /* the stamp of the fetched data */
		char *time_step,          /* the frequency of data */
                SQL_DATE next_time_stamp) /* pointer to returned next stamp */

{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    SQL_DATE tmp_date;
  /* EXEC SQL END DECLARE SECTION; */ 


  int result;

    /*
     * Add one timestep to the previous time stamp.
     */
  if ((result = SqlDateMath (ADDITION, prev_time_stamp, tmp_date, 1,
			     time_step)) != OK)
    {
      PrintError("DMI: %s: Problem adding to previous date: %s.\n", FUNC_NAME,
		  prev_time_stamp);
      return (ERROR);
    }

    /*
     * Compare the incremented date to the time stamp of data fetched from db.
     */
    if (SqlDateCompare(
        tmp_date,    /* the newly-calculated date */
        time_stamp,  /* the date of the next fetched time step */
        &result) != OK) {
            PrintError("DMI: Could not compare dates %s and %s.\n",
                        tmp_date,
                        time_stamp);
            next_time_stamp[0] = '\0';
            return(ERROR); /* This is trapped by checking if next_time_stamp */
                           /*   is returned as a null.                       */
    }

    /*
     * A negative result indicates that there is a gap in dates -- the
     *   incremented date is still before the piece of data fetched from the db.
     */
  if (result < 0) {
    strcpy(next_time_stamp, tmp_date);
    return (TRUE);
  }
  else if (result == 0) {
    next_time_stamp[0] = '\0';
    return (FALSE);
  }
  else {
    next_time_stamp[0] = '\0';
    PrintError("DMI: SqlDateGap found fetched data AHEAD of next valid time stamp.\n");
    return(ERROR); /* This is trapped by checking if next_time_stamp */
                   /*   is returned as a null.                       */
  }

}




