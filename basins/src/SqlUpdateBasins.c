
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
           char  filnam[19];
};
static const struct sqlcxp sqlfpn =
{
    18,
    "SqlUpdateBasins.pc"
};


static unsigned long sqlctx = 21293939;


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
            void  *sqhstv[10];
   unsigned int   sqhstl[10];
            int   sqhsts[10];
            void  *sqindv[10];
            int   sqinds[10];
   unsigned int   sqharm[10];
   unsigned int   *sqharc[10];
   unsigned short  sqadto[10];
   unsigned short  sqtdso[10];
} sqlstm = {10,10};

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
5,0,0,1,106,0,4,76,0,0,2,1,0,1,0,1,97,0,0,2,97,0,0,
28,0,0,2,103,0,4,77,0,0,2,1,0,1,0,1,97,0,0,2,97,0,0,
51,0,0,3,153,0,5,128,0,0,4,4,0,1,0,1,4,0,0,1,97,0,0,1,3,0,0,1,97,0,0,
82,0,0,4,295,0,3,144,0,0,10,10,0,1,0,1,3,0,0,1,97,0,0,1,97,0,0,1,4,0,0,1,3,0,0,
1,97,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
137,0,0,5,0,0,31,174,0,0,0,0,0,1,0,
152,0,0,6,0,0,31,187,0,0,0,0,0,1,0,
167,0,0,7,0,0,29,192,0,0,0,0,0,1,0,
182,0,0,8,106,0,4,233,0,0,2,1,0,1,0,1,97,0,0,2,97,0,0,
205,0,0,9,103,0,4,234,0,0,2,1,0,1,0,1,97,0,0,2,97,0,0,
228,0,0,10,153,0,5,286,0,0,4,4,0,1,0,1,4,0,0,1,97,0,0,1,3,0,0,1,97,0,0,
259,0,0,11,295,0,3,302,0,0,10,10,0,1,0,1,3,0,0,1,97,0,0,1,97,0,0,1,4,0,0,1,3,0,
0,1,97,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
314,0,0,12,0,0,31,332,0,0,0,0,0,1,0,
329,0,0,13,0,0,31,345,0,0,0,0,0,1,0,
344,0,0,14,0,0,29,350,0,0,0,0,0,1,0,
};


/*****************************************************************************
* DESCRIPTION
* These functions update the value(s) in the r_instant table with the 
* values in the array basinArray, for either percent snow 
* (SqlUpdateBasPctSnow) or percent precip (SqlUpdateBasPctPrecip).
*
* If a record does not exist for the current site/datatype/date combination,
* then a new record is inserted.
*
* Currently, basins with an ID of 0 are NOT inserted; this is an
* invalid basin ID.
*
* ASSUMPTIONS 
*
* INPUT
* BASIN*        - basin_data
* int           - number of elements in basin_array
* char *        - date of values
* ID            - source ID for this run of the application
* int *         - total number of values inserted
*
* OUTPUT
* warnings and error messages
*
* RETURN
* RET_ERROR - values not successfully updated
* short - number of sites successfully updated
*
*
* COMMENTS
*
* AUTHOR - EDITORS
* Carol Marra
* Rick Casey, May 2003. Modified for HDB2.
*****************************************************************************/

#include <stdlib.h>
#include <math.h>
#include "basins.h"
#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"
#include "basinTypedefs.h"

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


#define FUNC_NAME "SqlUpdateBasPctSnow"

int SqlUpdateBasPctSnow (BASIN *basinArray, int numSites, SQL_DATE date, 
			 ID sourceId, int *totalNumValues)

{
 /* EXEC SQL BEGIN DECLARE SECTION; */ 

   ID            siteDatatypeId,
                 sqlSourceId = sourceId;
   float         pctSnow;
   char          validationFlag[2],
				 method_name[64];
   SQL_DATE      sqlDate, start_date_time, end_date_time;
	int 		 loading_application_id,
				 agen_id,
				 collection_system_id,
				 method_id,
				 computation_id;
 /* EXEC SQL END DECLARE SECTION; */ 


 int         rowCount,
             result;
 ID          siteId;

 sprintf (sqlDate, date);

	/* Create the date variables. They are the same for all updates or inserts. */
	 sprintf(sqlDate, date);
	 /* EXEC SQL SELECT TO_CHAR(TRUNC(TO_DATE(:sqlDate,'dd-mon-yyyy hh24:mi:ss')),'dd-mon-yyyy hh24:mi:ss') INTO :start_date_time FROM dual; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 2;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select TO_CHAR(TRUNC(TO_DATE(:b0,'dd-mon-yyyy hh24:mi:ss'))\
,'dd-mon-yyyy hh24:mi:ss') into :b1  from dual ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)sqlDate;
  sqlstm.sqhstl[0] = (unsigned int  )21;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)start_date_time;
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


 	 /* EXEC SQL SELECT TO_CHAR(TO_DATE(:start_date_time,'dd-mon-yyyy hh24:mi:ss')+1,'dd-mon-yyyy hh24:mi:ss') INTO :end_date_time FROM dual; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 2;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select TO_CHAR((TO_DATE(:b0,'dd-mon-yyyy hh24:mi:ss')+1),'\
dd-mon-yyyy hh24:mi:ss') into :b1  from dual ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )28;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)start_date_time;
   sqlstm.sqhstl[0] = (unsigned int  )21;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)end_date_time;
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


 (*totalNumValues) = 0;

	 /* Fetch the foreign keys required for performing any inserts to r_base.  */
	 if ( (result = SqlFetchRbasekeys(&loading_application_id, &agen_id, 
			 &collection_system_id, &computation_id) ) == ERROR)
	 {
		return (ERROR);
	 }
	 /* Get the method id for the method for Basin Percent Snow. */
	 strcpy(method_name,BAS_PCT_SNOW_METHOD_NAME);
	 if ( (result = SqlGetMethodId(&method_name, &method_id) ) == ERROR)
	 {
		return (ERROR);
	 }

 /* loop through the basinArray, updating all of the values for 
    each site in basinArray */
 for (rowCount = 0; rowCount < numSites; rowCount++)
   {
     siteId = basinArray[rowCount].siteId;

     /* proceed only if siteId is valid */
     if (siteId)
       {
	 if ((result = SqlGetSiteDataTypeIdWithName (siteId, BAS_PCT_SNOW, 
						     &siteDatatypeId)) 
	     != CREATE_SDI)
	   {
	     if (result != OK)
	       return (ERROR);
	 
	     if ((result = SqlGetValidationFlag (siteDatatypeId, 
						 validationFlag)) == ERROR)
	       return (ERROR);
	 
	     /* Now update value for this site_datatype, date; if none exists,
		insert a new value */
	     pctSnow = basinArray[rowCount].percentSnow;
	     
	     if (pctSnow != BAD_DATA)
	       {
		   /*
		 	* EXEC SQL UPDATE r_instant
		    *	SET value = :pctSnow,
		    * 	source_id = :sqlSourceId,
		    *	validation = :validationFlag
		    *	WHERE site_datatype_id = :siteDatatypeId
		    *	 AND date_time = to_date (:sqlDate, 'dd-mon-yyyy hh24:mi:ss');
			*/
		 
					/* EXEC SQL UPDATE r_base
							SET value = :pctSnow,
							validation = :validationFlag
							WHERE site_datatype_id = :siteDatatypeId
							AND interval = 'day'
							AND start_date_time = to_date (:start_date_time, 'dd-mon-yyyy hh24:mi:ss'); */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 10;
     sqlstm.arrsiz = 4;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.stmt = "update r_base  set value=:b0,validation=:b1 where ((site\
_datatype_id=:b2 and  interval ='day') and start_date_time=to_date(:b3,'dd-mon\
-yyyy hh24:mi:ss'))";
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )51;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)256;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (         void  *)&pctSnow;
     sqlstm.sqhstl[0] = (unsigned int  )sizeof(float);
     sqlstm.sqhsts[0] = (         int  )0;
     sqlstm.sqindv[0] = (         void  *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned int  )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqhstv[1] = (         void  *)validationFlag;
     sqlstm.sqhstl[1] = (unsigned int  )2;
     sqlstm.sqhsts[1] = (         int  )0;
     sqlstm.sqindv[1] = (         void  *)0;
     sqlstm.sqinds[1] = (         int  )0;
     sqlstm.sqharm[1] = (unsigned int  )0;
     sqlstm.sqadto[1] = (unsigned short )0;
     sqlstm.sqtdso[1] = (unsigned short )0;
     sqlstm.sqhstv[2] = (         void  *)&siteDatatypeId;
     sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[2] = (         int  )0;
     sqlstm.sqindv[2] = (         void  *)0;
     sqlstm.sqinds[2] = (         int  )0;
     sqlstm.sqharm[2] = (unsigned int  )0;
     sqlstm.sqadto[2] = (unsigned short )0;
     sqlstm.sqtdso[2] = (unsigned short )0;
     sqlstm.sqhstv[3] = (         void  *)start_date_time;
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


		 /* No rows processed; insert value */
		 if (sqlca.sqlcode > 0)
		   {
			/*
		     * EXEC SQL INSERT INTO r_instant 
		     *   VALUES (:siteDatatypeId, 
             *               to_date (:sqlDate, 'dd-mon-yyyy hh24:mi:ss'),
             *              :pctSnow, :sqlSourceId, :validationFlag);
			 */
		     
					/* EXEC SQL INSERT INTO r_base
						(site_datatype_id,
						interval, 
						start_date_time, 
						end_date_time,
						value,
						agen_id,
						validation,
						collection_system_id,
						loading_application_id,
						method_id,
						computation_id)
					VALUES
						(:siteDatatypeId,
						'day',
						to_date (:start_date_time, 'dd-mon-yyyy hh24:mi:ss'),
						to_date (:end_date_time, 'dd-mon-yyyy hh24:mi:ss'),
						:pctSnow,
						:agen_id,
						:validationFlag,
						:collection_system_id,
						:loading_application_id,
						:method_id,
						:computation_id); */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 10;
     sqlstm.arrsiz = 10;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.stmt = "insert into r_base (site_datatype_id, interval ,start_da\
te_time,end_date_time,value,agen_id,validation,collection_system_id,loading_ap\
plication_id,method_id,computation_id) values (:b0,'day',to_date(:b1,'dd-mon-y\
yyy hh24:mi:ss'),to_date(:b2,'dd-mon-yyyy hh24:mi:ss'),:b3,:b4,:b5,:b6,:b7,:b8\
,:b9)";
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )82;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)256;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (         void  *)&siteDatatypeId;
     sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[0] = (         int  )0;
     sqlstm.sqindv[0] = (         void  *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned int  )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqhstv[1] = (         void  *)start_date_time;
     sqlstm.sqhstl[1] = (unsigned int  )21;
     sqlstm.sqhsts[1] = (         int  )0;
     sqlstm.sqindv[1] = (         void  *)0;
     sqlstm.sqinds[1] = (         int  )0;
     sqlstm.sqharm[1] = (unsigned int  )0;
     sqlstm.sqadto[1] = (unsigned short )0;
     sqlstm.sqtdso[1] = (unsigned short )0;
     sqlstm.sqhstv[2] = (         void  *)end_date_time;
     sqlstm.sqhstl[2] = (unsigned int  )21;
     sqlstm.sqhsts[2] = (         int  )0;
     sqlstm.sqindv[2] = (         void  *)0;
     sqlstm.sqinds[2] = (         int  )0;
     sqlstm.sqharm[2] = (unsigned int  )0;
     sqlstm.sqadto[2] = (unsigned short )0;
     sqlstm.sqtdso[2] = (unsigned short )0;
     sqlstm.sqhstv[3] = (         void  *)&pctSnow;
     sqlstm.sqhstl[3] = (unsigned int  )sizeof(float);
     sqlstm.sqhsts[3] = (         int  )0;
     sqlstm.sqindv[3] = (         void  *)0;
     sqlstm.sqinds[3] = (         int  )0;
     sqlstm.sqharm[3] = (unsigned int  )0;
     sqlstm.sqadto[3] = (unsigned short )0;
     sqlstm.sqtdso[3] = (unsigned short )0;
     sqlstm.sqhstv[4] = (         void  *)&agen_id;
     sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[4] = (         int  )0;
     sqlstm.sqindv[4] = (         void  *)0;
     sqlstm.sqinds[4] = (         int  )0;
     sqlstm.sqharm[4] = (unsigned int  )0;
     sqlstm.sqadto[4] = (unsigned short )0;
     sqlstm.sqtdso[4] = (unsigned short )0;
     sqlstm.sqhstv[5] = (         void  *)validationFlag;
     sqlstm.sqhstl[5] = (unsigned int  )2;
     sqlstm.sqhsts[5] = (         int  )0;
     sqlstm.sqindv[5] = (         void  *)0;
     sqlstm.sqinds[5] = (         int  )0;
     sqlstm.sqharm[5] = (unsigned int  )0;
     sqlstm.sqadto[5] = (unsigned short )0;
     sqlstm.sqtdso[5] = (unsigned short )0;
     sqlstm.sqhstv[6] = (         void  *)&collection_system_id;
     sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[6] = (         int  )0;
     sqlstm.sqindv[6] = (         void  *)0;
     sqlstm.sqinds[6] = (         int  )0;
     sqlstm.sqharm[6] = (unsigned int  )0;
     sqlstm.sqadto[6] = (unsigned short )0;
     sqlstm.sqtdso[6] = (unsigned short )0;
     sqlstm.sqhstv[7] = (         void  *)&loading_application_id;
     sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[7] = (         int  )0;
     sqlstm.sqindv[7] = (         void  *)0;
     sqlstm.sqinds[7] = (         int  )0;
     sqlstm.sqharm[7] = (unsigned int  )0;
     sqlstm.sqadto[7] = (unsigned short )0;
     sqlstm.sqtdso[7] = (unsigned short )0;
     sqlstm.sqhstv[8] = (         void  *)&method_id;
     sqlstm.sqhstl[8] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[8] = (         int  )0;
     sqlstm.sqindv[8] = (         void  *)0;
     sqlstm.sqinds[8] = (         int  )0;
     sqlstm.sqharm[8] = (unsigned int  )0;
     sqlstm.sqadto[8] = (unsigned short )0;
     sqlstm.sqtdso[8] = (unsigned short )0;
     sqlstm.sqhstv[9] = (         void  *)&computation_id;
     sqlstm.sqhstl[9] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[9] = (         int  )0;
     sqlstm.sqindv[9] = (         void  *)0;
     sqlstm.sqinds[9] = (         int  )0;
     sqlstm.sqharm[9] = (unsigned int  )0;
     sqlstm.sqadto[9] = (unsigned short )0;
     sqlstm.sqtdso[9] = (unsigned short )0;
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
			   "Problem inserting %s row for basin site: %d.",
			   BAS_PCT_SNOW, siteId)) != OK)
		       {
			 /* EXEC SQL ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 10;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )137;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


			 return (ERROR);
		       }
		   }
		 
		 /* Check to make sure no errors on update */
		 else
		   {
		     if ((result = SqlErrorHandler 
			  (FUNC_NAME, FALSE,
			   "Problem updating %s row for basin site: %d.",
			   BAS_PCT_SNOW, siteId)) != OK)
		       {
			 /* EXEC SQL ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 10;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )152;
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
   sqlstm.arrsiz = 10;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )167;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		 (*totalNumValues)++;
	       }
	   }

	 /* If site_datatype is not known, continue */
	 else
	   {
	     PrintError ("***Continuing to process known site_datatypes.\n");
	   }
       }
   }
 return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlUpdateBasPctPrecip"

int SqlUpdateBasPctPrecip (BASIN *basinArray, int numSites, SQL_DATE date, 
			   ID sourceId, int *totalNumValues)

{
 /* EXEC SQL BEGIN DECLARE SECTION; */ 

   ID            siteDatatypeId,
                 sqlSourceId = sourceId;
   float         pctPrecip;
   char          validationFlag[2], method_name[64];
   SQL_DATE      sqlDate, start_date_time, end_date_time;
	int 		 loading_application_id,
				 agen_id,
				 collection_system_id,
				 method_id,
				 computation_id;
 /* EXEC SQL END DECLARE SECTION; */ 


 int         rowCount,
             result;
 ID          siteId;

	/* Create the date variables. They are the same for all updates or inserts. */
	 sprintf(sqlDate, date);
	 /* EXEC SQL SELECT TO_CHAR(TRUNC(TO_DATE(:sqlDate,'dd-mon-yyyy hh24:mi:ss')),'dd-mon-yyyy hh24:mi:ss') INTO :start_date_time FROM dual; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 10;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select TO_CHAR(TRUNC(TO_DATE(:b0,'dd-mon-yyyy hh24:mi:ss'))\
,'dd-mon-yyyy hh24:mi:ss') into :b1  from dual ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )182;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)sqlDate;
  sqlstm.sqhstl[0] = (unsigned int  )21;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)start_date_time;
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


 	 /* EXEC SQL SELECT TO_CHAR(TO_DATE(:start_date_time,'dd-mon-yyyy hh24:mi:ss')+1,'dd-mon-yyyy hh24:mi:ss') INTO :end_date_time FROM dual; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 10;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select TO_CHAR((TO_DATE(:b0,'dd-mon-yyyy hh24:mi:ss')+1),'\
dd-mon-yyyy hh24:mi:ss') into :b1  from dual ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )205;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)start_date_time;
   sqlstm.sqhstl[0] = (unsigned int  )21;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)end_date_time;
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


 (*totalNumValues) = 0;

	 /* Fetch the foreign keys required for performing any inserts to r_base.  */
	 if ( (result = SqlFetchRbasekeys(&loading_application_id, &agen_id, 
			 &collection_system_id, &computation_id) ) == ERROR)
	 {
		return (ERROR);
	 }
	 /* Get the method id for the method for Basin Percent Precip. */
	 strcpy(method_name,BAS_PCT_PCP_METHOD_NAME);
	 if ( (result = SqlGetMethodId(&method_name, &method_id) ) == ERROR)
	 {
		return (ERROR);
	 }

 /* loop through the basinArray, updating all of the values for 
    each site in basinArray. Note that this is precip data, so the alternate 
    (climate site) ID is used. */
 for (rowCount = 0; rowCount < numSites; rowCount++)
   {
     siteId = basinArray[rowCount].siteId;

     /* proceed only if siteId is valid */
     if (siteId)
       {
	 if ((result = SqlGetSiteDataTypeIdWithName (siteId, BAS_PCT_PCP, 
						     &siteDatatypeId)) 
	     != CREATE_SDI)
	   {
	     if (result != OK)
	       return (ERROR);
	 
	     if ((result = SqlGetValidationFlag (siteDatatypeId, 
						 validationFlag)) == ERROR)
	       return (ERROR);
	 
	     /* Now update value for this site_datatype, date; if none exists,
		insert a new value */
	     pctPrecip = basinArray[rowCount].percentPrecip;
	     
	     if (pctPrecip != BAD_DATA)
	       {
		   /*
			* EXEC SQL UPDATE r_instant
			*   SET value = :pctPrecip,
			*   source_id = :sqlSourceId,
			*   validation = :validationFlag
			*   WHERE site_datatype_id = :siteDatatypeId
			*     AND date_time = to_date (:sqlDate, 'dd-mon-yyyy hh24:mi:ss');
		    */
		 
					/* EXEC SQL UPDATE r_base
							SET value = :pctPrecip,
							validation = :validationFlag
							WHERE site_datatype_id = :siteDatatypeId
							AND interval = 'day'
							AND start_date_time = to_date (:start_date_time, 'dd-mon-yyyy hh24:mi:ss'); */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 10;
     sqlstm.arrsiz = 10;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.stmt = "update r_base  set value=:b0,validation=:b1 where ((site\
_datatype_id=:b2 and  interval ='day') and start_date_time=to_date(:b3,'dd-mon\
-yyyy hh24:mi:ss'))";
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )228;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)256;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (         void  *)&pctPrecip;
     sqlstm.sqhstl[0] = (unsigned int  )sizeof(float);
     sqlstm.sqhsts[0] = (         int  )0;
     sqlstm.sqindv[0] = (         void  *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned int  )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqhstv[1] = (         void  *)validationFlag;
     sqlstm.sqhstl[1] = (unsigned int  )2;
     sqlstm.sqhsts[1] = (         int  )0;
     sqlstm.sqindv[1] = (         void  *)0;
     sqlstm.sqinds[1] = (         int  )0;
     sqlstm.sqharm[1] = (unsigned int  )0;
     sqlstm.sqadto[1] = (unsigned short )0;
     sqlstm.sqtdso[1] = (unsigned short )0;
     sqlstm.sqhstv[2] = (         void  *)&siteDatatypeId;
     sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[2] = (         int  )0;
     sqlstm.sqindv[2] = (         void  *)0;
     sqlstm.sqinds[2] = (         int  )0;
     sqlstm.sqharm[2] = (unsigned int  )0;
     sqlstm.sqadto[2] = (unsigned short )0;
     sqlstm.sqtdso[2] = (unsigned short )0;
     sqlstm.sqhstv[3] = (         void  *)start_date_time;
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



		 /* No rows processed; insert value */
		 if (sqlca.sqlcode > 0)
		   {
		   /*
		    * EXEC SQL INSERT INTO r_instant
		    *   VALUES (:siteDatatypeId, 
            *               to_date (:sqlDate, 'dd-mon-yyyy hh24:mi:ss'), 
            *               :pctPrecip, :sqlSourceId, :validationFlag);
			*/
					/* EXEC SQL INSERT INTO r_base
						(site_datatype_id,
						interval, 
						start_date_time, 
						end_date_time,
						value,
						agen_id,
						validation,
						collection_system_id,
						loading_application_id,
						method_id,
						computation_id)
					VALUES
						(:siteDatatypeId,
						'day',
						to_date (:start_date_time, 'dd-mon-yyyy hh24:mi:ss'),
						to_date (:end_date_time, 'dd-mon-yyyy hh24:mi:ss'),
						:pctPrecip,
						:agen_id,
						:validationFlag,
						:collection_system_id,
						:loading_application_id,
						:method_id,
						:computation_id); */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 10;
     sqlstm.arrsiz = 10;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.stmt = "insert into r_base (site_datatype_id, interval ,start_da\
te_time,end_date_time,value,agen_id,validation,collection_system_id,loading_ap\
plication_id,method_id,computation_id) values (:b0,'day',to_date(:b1,'dd-mon-y\
yyy hh24:mi:ss'),to_date(:b2,'dd-mon-yyyy hh24:mi:ss'),:b3,:b4,:b5,:b6,:b7,:b8\
,:b9)";
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )259;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)256;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (         void  *)&siteDatatypeId;
     sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[0] = (         int  )0;
     sqlstm.sqindv[0] = (         void  *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned int  )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqhstv[1] = (         void  *)start_date_time;
     sqlstm.sqhstl[1] = (unsigned int  )21;
     sqlstm.sqhsts[1] = (         int  )0;
     sqlstm.sqindv[1] = (         void  *)0;
     sqlstm.sqinds[1] = (         int  )0;
     sqlstm.sqharm[1] = (unsigned int  )0;
     sqlstm.sqadto[1] = (unsigned short )0;
     sqlstm.sqtdso[1] = (unsigned short )0;
     sqlstm.sqhstv[2] = (         void  *)end_date_time;
     sqlstm.sqhstl[2] = (unsigned int  )21;
     sqlstm.sqhsts[2] = (         int  )0;
     sqlstm.sqindv[2] = (         void  *)0;
     sqlstm.sqinds[2] = (         int  )0;
     sqlstm.sqharm[2] = (unsigned int  )0;
     sqlstm.sqadto[2] = (unsigned short )0;
     sqlstm.sqtdso[2] = (unsigned short )0;
     sqlstm.sqhstv[3] = (         void  *)&pctPrecip;
     sqlstm.sqhstl[3] = (unsigned int  )sizeof(float);
     sqlstm.sqhsts[3] = (         int  )0;
     sqlstm.sqindv[3] = (         void  *)0;
     sqlstm.sqinds[3] = (         int  )0;
     sqlstm.sqharm[3] = (unsigned int  )0;
     sqlstm.sqadto[3] = (unsigned short )0;
     sqlstm.sqtdso[3] = (unsigned short )0;
     sqlstm.sqhstv[4] = (         void  *)&agen_id;
     sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[4] = (         int  )0;
     sqlstm.sqindv[4] = (         void  *)0;
     sqlstm.sqinds[4] = (         int  )0;
     sqlstm.sqharm[4] = (unsigned int  )0;
     sqlstm.sqadto[4] = (unsigned short )0;
     sqlstm.sqtdso[4] = (unsigned short )0;
     sqlstm.sqhstv[5] = (         void  *)validationFlag;
     sqlstm.sqhstl[5] = (unsigned int  )2;
     sqlstm.sqhsts[5] = (         int  )0;
     sqlstm.sqindv[5] = (         void  *)0;
     sqlstm.sqinds[5] = (         int  )0;
     sqlstm.sqharm[5] = (unsigned int  )0;
     sqlstm.sqadto[5] = (unsigned short )0;
     sqlstm.sqtdso[5] = (unsigned short )0;
     sqlstm.sqhstv[6] = (         void  *)&collection_system_id;
     sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[6] = (         int  )0;
     sqlstm.sqindv[6] = (         void  *)0;
     sqlstm.sqinds[6] = (         int  )0;
     sqlstm.sqharm[6] = (unsigned int  )0;
     sqlstm.sqadto[6] = (unsigned short )0;
     sqlstm.sqtdso[6] = (unsigned short )0;
     sqlstm.sqhstv[7] = (         void  *)&loading_application_id;
     sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[7] = (         int  )0;
     sqlstm.sqindv[7] = (         void  *)0;
     sqlstm.sqinds[7] = (         int  )0;
     sqlstm.sqharm[7] = (unsigned int  )0;
     sqlstm.sqadto[7] = (unsigned short )0;
     sqlstm.sqtdso[7] = (unsigned short )0;
     sqlstm.sqhstv[8] = (         void  *)&method_id;
     sqlstm.sqhstl[8] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[8] = (         int  )0;
     sqlstm.sqindv[8] = (         void  *)0;
     sqlstm.sqinds[8] = (         int  )0;
     sqlstm.sqharm[8] = (unsigned int  )0;
     sqlstm.sqadto[8] = (unsigned short )0;
     sqlstm.sqtdso[8] = (unsigned short )0;
     sqlstm.sqhstv[9] = (         void  *)&computation_id;
     sqlstm.sqhstl[9] = (unsigned int  )sizeof(int);
     sqlstm.sqhsts[9] = (         int  )0;
     sqlstm.sqindv[9] = (         void  *)0;
     sqlstm.sqinds[9] = (         int  )0;
     sqlstm.sqharm[9] = (unsigned int  )0;
     sqlstm.sqadto[9] = (unsigned short )0;
     sqlstm.sqtdso[9] = (unsigned short )0;
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
			   "Problem inserting %s row for basin site: %d.",
			   BAS_PCT_PCP, siteId)) != OK)
		       {
			 /* EXEC SQL ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 10;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )314;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


			 return (ERROR);
		       }
		   }
		 
		 /* Check to make sure no errors on update */
		 else
		   {
		     if ((result = SqlErrorHandler 
			  (FUNC_NAME, FALSE,
			   "Problem updating %s row for basin site: %d.",
			   BAS_PCT_PCP, siteId)) != OK)
		       {
			 /* EXEC SQL ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 10;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )329;
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
   sqlstm.arrsiz = 10;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )344;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		 (*totalNumValues)++;
	       }
	   }

	 /* If site_datatype is not known, continue */
	 else
	   {
	     PrintError ("***Continuing to process known site_datatypes.\n");
	   }
       }
   }
 return (OK);
}
