
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
           char  filnam[20];
};
static const struct sqlcxp sqlfpn =
{
    19,
    "DmiSaveModelData.pc"
};


static unsigned int sqlctx = 37783811;


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
   unsigned char  *sqhstv[7];
   unsigned long  sqhstl[7];
            int   sqhsts[7];
            short *sqindv[7];
            int   sqinds[7];
   unsigned long  sqharm[7];
   unsigned long  *sqharc[7];
   unsigned short  sqadto[7];
   unsigned short  sqtdso[7];
} sqlstm = {12,7};

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
5,0,0,1,0,0,1053,190,0,0,0,0,0,0,128,1,5,0,0,
24,0,0,2,104,0,1030,248,0,0,5,5,0,0,128,3,3,0,0,3,3,0,0,3,5,0,0,3,5,0,0,3,5,0,
0,1,5,0,0,
63,0,0,3,0,0,1055,259,0,0,0,0,0,0,128,1,5,0,0,
82,0,0,4,123,0,1030,268,0,0,6,6,0,0,128,3,3,0,0,3,3,0,0,3,5,0,0,3,5,0,0,3,4,0,
0,3,5,0,0,1,5,0,0,
125,0,0,5,0,0,1055,280,0,0,0,0,0,0,128,1,5,0,0,
144,0,0,6,0,0,1053,294,0,0,0,0,0,0,128,1,5,0,0,
163,0,0,7,0,0,1048,304,0,0,1,1,0,0,128,1,5,0,0,1,5,0,0,
186,0,0,8,0,0,1055,309,0,0,0,0,0,0,128,1,5,0,0,
205,0,0,9,0,0,1053,319,0,0,0,0,0,0,128,1,5,0,0,
224,0,0,10,0,0,1048,427,0,0,1,1,0,0,128,1,5,0,0,1,5,0,0,
247,0,0,11,0,0,1055,432,0,0,0,0,0,0,128,1,5,0,0,
};



/****************************************************************************
**
**      IDENTIFICATION SECTION
**      ----------------------
**
**      Module Name:    int DmiSaveModelData(dmi_header_struct * header)
**
**      File Name:      DmiSaveModelData.pc
**
**      Date Written:   December, 1995
**
**      Author(s):      Paddy McCarthy
**                      Carol Marra (modified for Oracle 10/98)
**
**      Module Description:
**         Writes data for a single object/slot to the DB in appropriate 
**         model (m_) table.
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


#define FUNC_NAME "DmiSaveModelData"
int DmiSaveModelData(dmi_header_struct * header)
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 

       SQL_DATE      end_date;
       SQL_DATE      start_date;
       SQL_DATE      run_control_start_date,
	             run_control_end_date;
       SQL_INTERVAL  dataInterval;
       double        value;

       int           i, cnt;
       int           compare1, compare2;
       int           switched = FALSE;

       char          deleteRemaining[200],
              	     model_table_name[DATABASE_NAME_LENGTH];

       int           siteDatatypeId;
       int           modelRunId;
    /* EXEC SQL END DECLARE SECTION; */ 


   int result;

   /* Determine interval */
    if (!strcmp (header->timestep, HOUR))
      {
	strcpy(dataInterval, "hour");
	sprintf (model_table_name, MODEL_HOUR);
      }
    else if (!strcmp (header->timestep, DAY))
      {
	strcpy(dataInterval, "day");
	sprintf (model_table_name, MODEL_DAY);
      }
    else if (!strcmp (header->timestep, MONTH))
      {
	strcpy(dataInterval, "month");
	sprintf (model_table_name, MODEL_MONTH);
      }
    else
      {
	PrintError ("Error in %s...timestep %s unknown...\n.", 
		    FUNC_NAME,header->timestep);
	return (ERROR);
      }
    
    /* Get the start and end dates from run control. No data outside these 
       dates will be saved to the DB.  */

    if (riverwareToHdbDate(header->pr_runcontrol_startdate, run_control_start_date,
	header->timestep) != 0) {
        PrintError("DMI: Unable to convert start date (%s) to HDB format when saving data.\n", 
                    header->pr_runcontrol_startdate);
        return (ERROR);
    }

    if (riverwareToHdbDate(header->pr_runcontrol_enddate, run_control_end_date,
	header->timestep) != 0) {
        PrintError("DMI: Unable to convert end date (%s) to HDB format when saving data.\n", 
                    header->pr_runcontrol_enddate);
        return (ERROR);
    }

  /* If the data are not to be written to the default database, 
     set the session accordingly */

  if (strcmp(header->data_location, dbsite_array[0].dbsite_alias))
  {
    cnt = 0;

    /* Search for the needed session in the array of sessions */

    while (cnt < number_of_db_sites) 
	{
	  if (!strcmp(dbsite_array[cnt].dbsite_alias, header->data_location))
	    break;
	  cnt++;
    }
      /* The appropriate member was found */

    if (cnt < number_of_db_sites) 
      { 
	/* Return if not connected to that session -- no way to save
	   the data */

	if (!dbsite_array[cnt].connected) 
	  {
	    PrintError("Not connected to session for %s. Cannot fetch data...\n",
		       dbsite_array[cnt].dbsite_alias);
	    
	    /* switch the session back to default. */
	    
	    if (SwitchSession(dbsite_array[0].dbsite_alias) != OK) 
	      {
		PrintError("Error...changing to session 1 for %s\n",
			   dbsite_array[0].dbsite_alias);
		return(ERROR);
	      }
	    
	    return(ERROR);
	  }
	else /* attempt to switch to the needed session */
	  {
	    if (SwitchSession(dbsite_array[cnt].dbsite_alias) != OK) 
	      {
		PrintError("ERROR occurred changing to session %d for %s\n",
			   dbsite_array[cnt].session_number,
			   dbsite_array[cnt].dbsite_alias);
		return(ERROR);
	      }
	    else  /* Switch was successful. */
	      {
		switched = TRUE;

	      }
	  }
      }
    
    /* The name is not in the array. Exit. */
    else 
      {
	PrintError("Could not find database session ID for %s.\n",
		   header->data_location);
	return(ERROR);
      }
  }

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

    /* Create statement to delete any DB rows outside of model run dates */
    sprintf (deleteRemaining, "delete from %s where model_run_id = %d \
and site_datatype_id = %d and (start_date_time < to_date ('%s') OR \
start_date_time > to_date ('%s'))", model_table_name, 
	     header->model_destination, header->hdb_site_datatype_id, 
	     run_control_start_date, run_control_end_date);
    
    for (i = 0; i < header->number_of_timesteps; i++) 
    { 
      modelRunId = header->model_destination;
      siteDatatypeId = header->hdb_site_datatype_id;
      strcpy (start_date, header->data[i].hdb_date);
      strcpy (end_date, header->data[i].end_date);
      value = header->data[i].value;

      /* If header->data[i].hdb_date is between (inclusive)
	run_control_start_date and run_control_end_date, process the value;
	otherwise, print warning message and do not process value */
      result = SqlDateCompare(header->data[i].hdb_date,
				  run_control_start_date,
				  &compare1);
      if (result != OK) 
	{
	  PrintError("Could not compare dates %s and %s.\n",
		     run_control_start_date,
		     header->data[i].hdb_date);
	  return(ERROR);
	}
      
      result = SqlDateCompare(header->data[i].hdb_date,
				  run_control_end_date,
				  &compare2);
      if (result != OK) 
	{
	  PrintError("Could not compare dates %s and %s.\n",
		     run_control_end_date,
		     header->data[i].hdb_date);
	  return(ERROR);
	}

      /* If this item is between start and end dates of model run, 
	 process it */
      if ((compare1 >= 0) & (compare2 <= 0)) 
	{
	  /* Delete old value if new is NaN; execute the delete
	     at the current database*/ 
	  if (header->data[i].is_nan == 'Y')
	    {
	      /* EXEC SQL AT :current_dbsite EXECUTE
		BEGIN
		delete_m_table (:modelRunId, :siteDatatypeId,
				:start_date, :end_date, :dataInterval);
	      END;
	      END-EXEC; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 6;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "begin delete_m_table ( :modelRunId , :siteDatatypeId ,\
 :start_date , :end_date , :dataInterval ) ; END ;";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )24;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)&modelRunId;
       sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (unsigned char  *)&siteDatatypeId;
       sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
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
       sqlstm.sqhstv[4] = (unsigned char  *)dataInterval;
       sqlstm.sqhstl[4] = (unsigned long )33;
       sqlstm.sqhsts[4] = (         int  )0;
       sqlstm.sqindv[4] = (         short *)0;
       sqlstm.sqinds[4] = (         int  )0;
       sqlstm.sqharm[4] = (unsigned long )0;
       sqlstm.sqadto[4] = (unsigned short )0;
       sqlstm.sqtdso[4] = (unsigned short )0;
       sqlstm.sqhstv[5] = (unsigned char  *)current_dbsite;
       sqlstm.sqhstl[5] = (unsigned long )10;
       sqlstm.sqhsts[5] = (         int  )0;
       sqlstm.sqindv[5] = (         short *)0;
       sqlstm.sqinds[5] = (         int  )0;
       sqlstm.sqharm[5] = (unsigned long )0;
       sqlstm.sqadto[5] = (unsigned short )0;
       sqlstm.sqtdso[5] = (unsigned short )0;
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
		    "Problem in delete_m_table.")) != OK)
		{
		  /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 6;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )63;
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

	      /* EXEC SQL AT :current_dbsite EXECUTE
		BEGIN
		modify_m_table_raw (:modelRunId, :siteDatatypeId,
				   :start_date, :end_date, :value,
	                           :dataInterval, 'Y'); 
	      END;
	      END-EXEC; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 7;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "begin modify_m_table_raw ( :modelRunId , :siteDatatype\
Id , :start_date , :end_date , :value , :dataInterval , 'Y' ) ; END ;";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )82;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)&modelRunId;
       sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (unsigned char  *)&siteDatatypeId;
       sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
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
       sqlstm.sqhstv[5] = (unsigned char  *)dataInterval;
       sqlstm.sqhstl[5] = (unsigned long )33;
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
		   (FUNC_NAME, FALSE,
		    "Problem in modify_m_table_raw.")) != OK)
		{
		  /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 7;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )125;
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

      else if (compare1 < 0)
	PrintError ("Data date %s is before run control start date\n\t %s. Value will be ignored.\n", header->data[i].hdb_date, run_control_start_date);
      else 
	PrintError ("Data date %s is after run control end date\n\t %s. Value will be ignored.\n", header->data[i].hdb_date, run_control_end_date);

    } /* end looping through values in header */

    /* Commit; stored procedure doesn't */
    /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 7;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )144;
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


    if ((result = SqlErrorHandler (FUNC_NAME, TRUE,
       "Problem committing new values.\n")) != OK) {
       return(ERROR);
    }

    /* Take additional steps to ensure that data in database
       is equivalent to data in current model run:
       - If there is data in HDB for this model_run and SDI, but
         outside the model run date range, delete it. */
    /* EXEC SQL AT :current_dbsite EXECUTE IMMEDIATE :deleteRemaining; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 7;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )163;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)deleteRemaining;
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
	  "Problem executing: %s.", deleteRemaining)) != OK)
      {	
	/* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 7;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )186;
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


	return (ERROR)	;
      }
    
    /* Post a warning that one or more rows were deleted with the 
       deleteRemaining statement. */
    if (sqlca.sqlcode == 0)
      PrintError("DMI: Removed values from database because they were outside the current run dates for this model run.\n");
    
    /* Commit; stored procedure doesn't */
    /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 7;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )205;
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


    if ((result = SqlErrorHandler (FUNC_NAME, TRUE,
       "Problem committing delete of rows outside model run dates.\n")) != OK) {
       return(ERROR);
    }

  /* Set session back to default, if switch was made. */
  if (switched)
    {
      if (SwitchSession(dbsite_array[0].dbsite_alias) != OK) 
	{
	  PrintError("ERROR occurred changing to default session 1 for %s\n",
		     dbsite_array[0].dbsite_alias);
	  return(ERROR);
	}
    }

  return(OK);
}



#define FUNC_NAME "DmiDeleteObsoleteModelSDIs"

#define CHARS_PER_SDI 7
#define NUM_SDIS 10000
#define CHARS_PER_HEADER 100
int DmiDeleteObsoleteModelSDIs(dmi_header_struct * list)
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 

    char          *deleteStatement,
	          tempDeleteStatement[CHARS_PER_HEADER],
                  model_table_name[DATABASE_NAME_LENGTH];
    /* EXEC SQL END DECLARE SECTION; */ 


    char tempStatement[CHARS_PER_SDI + 1];
    dmi_header_struct *current = list;

    int result,
      count;

   /* Proceed only if first element is written to model tables.
      DMIs currently (3/04) do not allow for some slots to be
      written to r_ tables and others to m_ tables. So, it's safe
      to exit if the first element goes to a real table. */
   if (current->destination == REAL)
     return (OK);

   /* Get model table */
   if (!strcmp (list->timestep, HOUR))
     sprintf (model_table_name, MODEL_HOUR);
   else if (!strcmp (list->timestep, DAY))
     sprintf (model_table_name, MODEL_DAY);
   else if (!strcmp (list->timestep, MONTH))
     sprintf (model_table_name, MODEL_MONTH);
   else
     {
       PrintError ("Error in %s...timestep %s unknown...\n.", 
		   FUNC_NAME,list->timestep);
       return (ERROR);
     }

   /* Malloc enough space for about 10000 site datatype IDs in the
      string */
   deleteStatement = malloc (CHARS_PER_HEADER + (NUM_SDIS * CHARS_PER_SDI));
   if (!deleteStatement)
     {
       PrintError ("DMI: Error allocating space for deleteStatement.\n\t");
       return (ERROR);
     }

   count = 0;

   /* Output model_run_id must be the same for all SDIs, so use
      the first one in the delete statement */
   sprintf (tempDeleteStatement, "delete from %s where model_run_id = %d and site_datatype_id not in (%d", model_table_name, current->model_destination,
	    current->hdb_site_datatype_id);
   strcpy (deleteStatement, tempDeleteStatement);
   
   /* Walk through list to build up list of SDIs for delete statement */
   current = current->next;
   count++;

   while (current)
     {
       /* increase size of string if necessary */
       if (!(count % NUM_SDIS))
	 {
	   deleteStatement =  realloc (deleteStatement,
				       CHARS_PER_HEADER +  
				       ((count / NUM_SDIS + 1) * 
					(NUM_SDIS * CHARS_PER_SDI)));
	   
	   if (!deleteStatement)
	     {
	       PrintError ("DMI: Error allocating space for deleteStatement.\n\t");
	       return (ERROR);
	     }
	 }

       sprintf (tempStatement, ", %d", current->hdb_site_datatype_id);
       strcat (deleteStatement, tempStatement);
       current = current->next;
       count++;
     }
   
   strcat (deleteStatement, ")");

   /* EXEC SQL AT :current_dbsite EXECUTE IMMEDIATE :deleteStatement; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 7;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )224;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (unsigned char  *)deleteStatement;
   sqlstm.sqhstl[0] = (unsigned long )0;
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



   if ((result = SqlErrorHandler (FUNC_NAME, TRUE,
				  "Error deleting obsolete site_datatype_ids from model run: \n\t %s\n", deleteStatement)) != OK)
     {
       /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 7;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )247;
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


       return(ERROR);
     }

   if (sqlca.sqlcode == 0)
     PrintError ("Successfully deleted model data from the database for any \n\t site_datatype_ids that are no longer part of this model run.\n");

}
