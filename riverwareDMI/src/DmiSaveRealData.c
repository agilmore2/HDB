
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
           char  filnam[19];
};
static const struct sqlcxp sqlfpn =
{
    18,
    "DmiSaveRealData.pc"
};


static unsigned int sqlctx = 18891523;


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
5,0,0,1,0,0,1053,160,0,0,0,0,0,0,128,1,5,0,0,
24,0,0,2,114,0,1030,197,0,0,6,6,0,0,128,3,3,0,0,3,5,0,0,3,5,0,0,3,5,0,0,3,3,0,
0,3,3,0,0,1,5,0,0,
67,0,0,3,0,0,1055,209,0,0,0,0,0,0,128,1,5,0,0,
86,0,0,4,192,0,1030,219,0,0,10,10,0,0,128,3,3,0,0,3,5,0,0,3,5,0,0,3,5,0,0,3,4,
0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0,1,5,0,0,
145,0,0,5,0,0,1055,233,0,0,0,0,0,0,128,1,5,0,0,
164,0,0,6,0,0,1053,241,0,0,0,0,0,0,128,1,5,0,0,
};


/****************************************************************************
**
**      IDENTIFICATION SECTION
**      ----------------------
**
**      Module Name:    int DmiSaveRealData(dmi_header_struct * header)
**
**      File Name:      DmiSaveRealData.pc
**
**      Date Written:   Feb, 1999
**
**      Author(s):      Rene  Reitsma
**			Modified 2/04 to use data-loading stored procedures
**
**      Module Description:
**         Writes data for a single object/slot to the DB in appropriate 
**         real (r_) table. Code is the adjusted version of
**         DmiSaveModelData.pc
**
**      Variables Passed In:
**         dmi_header_struct *list = pointer to struct containing obj/slot
**              info and data.
**
**      Comments:
**        This routine assumes that the following information has been
**          initialized in the passed-in header:
**          hdb_site_datatype_id -- the proper sdi for the obj/slot
**          data[] -- all elements
**          
**        The routine uses a circular array to manage the passed-in data.
**          I designed this assuming that the routine could compare two
**          ordered lists -- the data fetched from the DB is compared to
**          data passed in with the header. The DB list of data could
**          contain missing values, whereas the passed-in list will have
**          no gaps. With ordered lists, when a piece of data is fetched 
**          from the DB, there is no reason to search the passed-in list
**          BEFORE the date of the last successful comparison to find
**          the matching piece of data. So a circular array would be
**          very efficient and robust for two ordered lists.
**
**        Unfortunately, Ingres does not allow one to open a cursor FOR UPDATE
**          that is ordered. So the ordered nature of one of the lists is not 
**          guaranteed. I kept the circular array b/c the DB records are mostly
**          ordered due to the nature in which they are inserted. This cuts 
**          down on the n-squared nature of the algorithm.
**          
**
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <utils.h>
#include <dbutils.h>
#include <defines.h>
#include "dmiOut.h"
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


#define FUNC_NAME "DmiSaveRealData"
int DmiSaveRealData(dmi_header_struct * header)
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 

       SQL_DATE      end_date;
       SQL_DATE      start_date;
       SQL_DATE      run_control_start_date;
       SQL_INTERVAL  dataInterval;
       double        value;

       int	     siteDatatypeId;
       int           agencyId;
       int           collectionId;
       int           computationId;
       int           i;
       int           loadingId;
       int           methodId;
       int           result;
    /* EXEC SQL END DECLARE SECTION; */ 


    int the_result;

    /* Lookup foreign keys for insertions to r_base */
    if ((result = SqlGetMethodId("unknown", &methodId)) != OK)
    {
       PrintError("Problem looking up method id for method named unknown.");
       return (ERROR);
    }
    if ((result = SqlGetComputationId("unknown", &computationId)) != OK)
    {
       PrintError("Problem looking up computation id for computation named unknown.");
       return (ERROR);
    }
    if ((result = SqlGetCollectionId("RiverWare", &collectionId)) != OK)
    {
       PrintError("Problem looking up collection system id for RiverWare.");
       return (ERROR);
    }
    if ((result = SqlGetLoadingId("RiverWare DMI", &loadingId)) != OK)
    {
       PrintError("Problem looking up loading application id for RiverWare DMI.");
       return (ERROR);
    }
    if ((result = SqlGetAgencyId("Bureau of Reclamation", &agencyId)) != OK)
    {
       PrintError("Problem looking up agency id for Bureau of Reclamation.");
       return (ERROR);
    }

    /* Determine interval based on timestep */

    if (!strcmp (header->timestep, HOUR))
    {
          strcpy(dataInterval, "hour");
    }
    else if (!strcmp (header->timestep, DAY))
         {
                strcpy(dataInterval, "day");
         }
         else if (!strcmp (header->timestep, MONTH))
              {
                    strcpy(dataInterval, "month");
              }
              else
              {
	            PrintError ("Error in %s...timestep %s unknown...\n.", 
                            FUNC_NAME,header->timestep);
	            return (ERROR);
              }
    
    /* Get the start date from the run control. No data from before this 
       date will be saved to the DB or cause deletions from the DB.  */

    result = riverwareToHdbDate(header->pr_runcontrol_startdate, 
                            run_control_start_date,
	                        header->timestep);
    if (result != OK) 
    {
      PrintError("DMI: Unable to convert start date (%s) to HDB format when saving data.\n", 
                 header->pr_runcontrol_startdate);
      return (ERROR);
    }

  /* If the data do not belong to the default database, exit with an 
     error; cannot write real data back to non-default database */

  if (strcmp(header->data_location, dbsite_array[0].dbsite_alias))
  {
    PrintError ("You cannot write real/historical data back to the non-local database.\n");
    PrintError ("Object = %s, database = %s\n", header->object_name, 
	header->data_location);
    return (ERROR);
  }

  /* ORACLE -- can't set Autocommit Off. Need to do other DB operations 
     while cursor is open. Commit to set new transaction. */

  /* needed??? */
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


  result = SqlErrorHandler (FUNC_NAME, TRUE,
                              "Problem committing at %s.\n",
                              current_dbsite);
  if (result != OK)
    {
       PrintError("Error...committing at :current_dbsite\n");
       return(ERROR);
    }

    for (i = 0; i < header->number_of_timesteps; i++) 
    { 
      /* Set host variables */
      siteDatatypeId = header->hdb_site_datatype_id;
      strcpy (start_date, header->data[i].hdb_date);
      strcpy (end_date, header->data[i].end_date);
      value = header->data[i].value;

      /* If header->data[i].hdb_date equal to or greater than 
	run_control_start_date, process the value; otherwise, do nothing */
      the_result = SqlDateCompare(header->data[i].hdb_date,
				  run_control_start_date,
				  &result);
      if (the_result != OK) 
	{
	  PrintError("Could not compare dates %s and %s.\n",
		     run_control_start_date,
		     header->data[i].hdb_date);
	  return(ERROR);
	}
      
      /* If this item is not before start date of model run, process it */
      if (result >= 0) 
	{
	  /* Delete old value if new is NaN */
	  if (header->data[i].is_nan == 'Y')
	    {
	      /* EXEC SQL AT :current_dbsite EXECUTE
		BEGIN
		delete_r_base (:siteDatatypeId,
			       :dataInterval, :start_date, :end_date,
			       :agencyId, :loadingId);
	      END;
	      END-EXEC; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 7;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "begin delete_r_base ( :siteDatatypeId , :dataInterval \
, :start_date , :end_date , :agencyId , :loadingId ) ; END ;";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )24;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)&siteDatatypeId;
       sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (unsigned char  *)dataInterval;
       sqlstm.sqhstl[1] = (unsigned long )33;
       sqlstm.sqhsts[1] = (         int  )0;
       sqlstm.sqindv[1] = (         short *)0;
       sqlstm.sqinds[1] = (         int  )0;
       sqlstm.sqharm[1] = (unsigned long )0;
       sqlstm.sqadto[1] = (unsigned short )0;
       sqlstm.sqtdso[1] = (unsigned short )0;
       sqlstm.sqhstv[2] = (unsigned char  *)start_date;
       sqlstm.sqhstl[2] = (unsigned long )21;
       sqlstm.sqhsts[2] = (         int  )0;
       sqlstm.sqindv[2] = (         short *)0;
       sqlstm.sqinds[2] = (         int  )0;
       sqlstm.sqharm[2] = (unsigned long )0;
       sqlstm.sqadto[2] = (unsigned short )0;
       sqlstm.sqtdso[2] = (unsigned short )0;
       sqlstm.sqhstv[3] = (unsigned char  *)end_date;
       sqlstm.sqhstl[3] = (unsigned long )21;
       sqlstm.sqhsts[3] = (         int  )0;
       sqlstm.sqindv[3] = (         short *)0;
       sqlstm.sqinds[3] = (         int  )0;
       sqlstm.sqharm[3] = (unsigned long )0;
       sqlstm.sqadto[3] = (unsigned short )0;
       sqlstm.sqtdso[3] = (unsigned short )0;
       sqlstm.sqhstv[4] = (unsigned char  *)&agencyId;
       sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[4] = (         int  )0;
       sqlstm.sqindv[4] = (         short *)0;
       sqlstm.sqinds[4] = (         int  )0;
       sqlstm.sqharm[4] = (unsigned long )0;
       sqlstm.sqadto[4] = (unsigned short )0;
       sqlstm.sqtdso[4] = (unsigned short )0;
       sqlstm.sqhstv[5] = (unsigned char  *)&loadingId;
       sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[5] = (         int  )0;
       sqlstm.sqindv[5] = (         short *)0;
       sqlstm.sqinds[5] = (         int  )0;
       sqlstm.sqharm[5] = (unsigned long )0;
       sqlstm.sqadto[5] = (unsigned short )0;
       sqlstm.sqtdso[5] = (unsigned short )0;
       sqlstm.sqhstv[6] = (unsigned char  *)current_dbsite;
       sqlstm.sqhstl[6] = (unsigned long )10;
       sqlstm.sqhsts[6] = (         int  )0;
       sqlstm.sqindv[6] = (         short *)0;
       sqlstm.sqinds[6] = (         int  )0;
       sqlstm.sqharm[6] = (unsigned long )0;
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


	      
	      if ((result = SqlErrorHandler 
		   (FUNC_NAME, TRUE,
		    "Problem in delete_r_base.")) != OK)
		{
		  /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 7;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )67;
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
	  /* update or insert the value */
	  else 
	    {
	      /* PrintError("Updating or inserting value.\n"); */

	      /* OverwriteFlag and ValidationFlag are both null */
	      /* EXEC SQL AT :current_dbsite EXECUTE
		BEGIN
		modify_r_base_raw (:siteDatatypeId,
				   :dataInterval, :start_date,
				   :end_date, :value,
				   :agencyId, NULL, NULL, :collectionId, 
				   :loadingId, :methodId, :computationId, 'Y');
	      END;
	      END-EXEC; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 11;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "begin modify_r_base_raw ( :siteDatatypeId , :dataInter\
val , :start_date , :end_date , :value , :agencyId , NULL , NULL , :collection\
Id , :loadingId , :methodId , :computationId , 'Y' ) ; END ;";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )86;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)&siteDatatypeId;
       sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (unsigned char  *)dataInterval;
       sqlstm.sqhstl[1] = (unsigned long )33;
       sqlstm.sqhsts[1] = (         int  )0;
       sqlstm.sqindv[1] = (         short *)0;
       sqlstm.sqinds[1] = (         int  )0;
       sqlstm.sqharm[1] = (unsigned long )0;
       sqlstm.sqadto[1] = (unsigned short )0;
       sqlstm.sqtdso[1] = (unsigned short )0;
       sqlstm.sqhstv[2] = (unsigned char  *)start_date;
       sqlstm.sqhstl[2] = (unsigned long )21;
       sqlstm.sqhsts[2] = (         int  )0;
       sqlstm.sqindv[2] = (         short *)0;
       sqlstm.sqinds[2] = (         int  )0;
       sqlstm.sqharm[2] = (unsigned long )0;
       sqlstm.sqadto[2] = (unsigned short )0;
       sqlstm.sqtdso[2] = (unsigned short )0;
       sqlstm.sqhstv[3] = (unsigned char  *)end_date;
       sqlstm.sqhstl[3] = (unsigned long )21;
       sqlstm.sqhsts[3] = (         int  )0;
       sqlstm.sqindv[3] = (         short *)0;
       sqlstm.sqinds[3] = (         int  )0;
       sqlstm.sqharm[3] = (unsigned long )0;
       sqlstm.sqadto[3] = (unsigned short )0;
       sqlstm.sqtdso[3] = (unsigned short )0;
       sqlstm.sqhstv[4] = (unsigned char  *)&value;
       sqlstm.sqhstl[4] = (unsigned long )sizeof(double);
       sqlstm.sqhsts[4] = (         int  )0;
       sqlstm.sqindv[4] = (         short *)0;
       sqlstm.sqinds[4] = (         int  )0;
       sqlstm.sqharm[4] = (unsigned long )0;
       sqlstm.sqadto[4] = (unsigned short )0;
       sqlstm.sqtdso[4] = (unsigned short )0;
       sqlstm.sqhstv[5] = (unsigned char  *)&agencyId;
       sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[5] = (         int  )0;
       sqlstm.sqindv[5] = (         short *)0;
       sqlstm.sqinds[5] = (         int  )0;
       sqlstm.sqharm[5] = (unsigned long )0;
       sqlstm.sqadto[5] = (unsigned short )0;
       sqlstm.sqtdso[5] = (unsigned short )0;
       sqlstm.sqhstv[6] = (unsigned char  *)&collectionId;
       sqlstm.sqhstl[6] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[6] = (         int  )0;
       sqlstm.sqindv[6] = (         short *)0;
       sqlstm.sqinds[6] = (         int  )0;
       sqlstm.sqharm[6] = (unsigned long )0;
       sqlstm.sqadto[6] = (unsigned short )0;
       sqlstm.sqtdso[6] = (unsigned short )0;
       sqlstm.sqhstv[7] = (unsigned char  *)&loadingId;
       sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[7] = (         int  )0;
       sqlstm.sqindv[7] = (         short *)0;
       sqlstm.sqinds[7] = (         int  )0;
       sqlstm.sqharm[7] = (unsigned long )0;
       sqlstm.sqadto[7] = (unsigned short )0;
       sqlstm.sqtdso[7] = (unsigned short )0;
       sqlstm.sqhstv[8] = (unsigned char  *)&methodId;
       sqlstm.sqhstl[8] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[8] = (         int  )0;
       sqlstm.sqindv[8] = (         short *)0;
       sqlstm.sqinds[8] = (         int  )0;
       sqlstm.sqharm[8] = (unsigned long )0;
       sqlstm.sqadto[8] = (unsigned short )0;
       sqlstm.sqtdso[8] = (unsigned short )0;
       sqlstm.sqhstv[9] = (unsigned char  *)&computationId;
       sqlstm.sqhstl[9] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[9] = (         int  )0;
       sqlstm.sqindv[9] = (         short *)0;
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
}


	      
	      if ((result = SqlErrorHandler 
		   (FUNC_NAME, FALSE,
		    "Problem in modify_r_base_raw.")) != OK)
		{
		  /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 11;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )145;
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
    }

    /* Commit; stored procedure does not */
    /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 11;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )164;
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


    result = SqlErrorHandler (FUNC_NAME, TRUE,
			      "Problem committing...\n");
    if (result != OK)
      return(ERROR); 
    
    return(OK);
}
