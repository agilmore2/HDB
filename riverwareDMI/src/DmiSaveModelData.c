
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
           char  filnam[20];
};
static const struct sqlcxp sqlfpn =
{
    19,
    "DmiSaveModelData.pc"
};


static unsigned long sqlctx = 37783811;


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
            void  *sqhstv[4];
   unsigned int   sqhstl[4];
            int   sqhsts[4];
            void  *sqindv[4];
            int   sqinds[4];
   unsigned int   sqharm[4];
   unsigned int   *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {10,4};

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
5,0,0,1,0,0,1053,182,0,0,0,0,0,0,128,1,97,0,0,
24,0,0,2,0,0,1041,205,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
47,0,0,2,0,0,1069,219,0,0,0,0,0,0,128,1,97,0,0,
66,0,0,2,0,0,1037,234,0,0,2,0,0,0,128,2,97,0,0,2,4,0,0,1,97,0,0,
93,0,0,3,37,0,1071,299,0,0,0,0,0,0,128,1,97,0,0,
112,0,0,4,36,0,1071,304,0,0,0,0,0,0,128,1,97,0,0,
131,0,0,5,38,0,1071,309,0,0,0,0,0,0,128,1,97,0,0,
150,0,0,6,45,0,1070,369,0,0,1,1,0,0,128,1,4,0,0,1,97,0,0,
173,0,0,7,44,0,1070,376,0,0,1,1,0,0,128,1,4,0,0,1,97,0,0,
196,0,0,8,46,0,1070,383,0,0,1,1,0,0,128,1,4,0,0,1,97,0,0,
219,0,0,9,37,0,1071,438,0,0,0,0,0,0,128,1,97,0,0,
238,0,0,10,36,0,1071,443,0,0,0,0,0,0,128,1,97,0,0,
257,0,0,11,38,0,1071,448,0,0,0,0,0,0,128,1,97,0,0,
276,0,0,2,0,0,1039,476,0,0,0,0,0,0,128,1,97,0,0,
295,0,0,12,0,0,1041,498,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
318,0,0,13,0,0,1041,503,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
341,0,0,14,0,0,1041,508,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
364,0,0,15,0,0,1041,513,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
387,0,0,12,0,0,1045,572,0,0,3,3,0,0,128,1,97,0,0,1,97,0,0,1,4,0,0,1,97,0,0,
418,0,0,13,0,0,1045,577,0,0,3,3,0,0,128,1,97,0,0,1,97,0,0,1,4,0,0,1,97,0,0,
449,0,0,14,0,0,1045,582,0,0,3,3,0,0,128,1,97,0,0,1,97,0,0,1,4,0,0,1,97,0,0,
480,0,0,15,0,0,1045,587,0,0,3,3,0,0,128,1,97,0,0,1,97,0,0,1,4,0,0,1,97,0,0,
511,0,0,16,0,0,1053,614,0,0,0,0,0,0,128,1,97,0,0,
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

       int           done = 0;
       int           found_in_array;
       SQL_DATE      end_date;
       SQL_DATE      fetch_date;
       SQL_DATE      new_date;
       SQL_DATE      run_control_start_date;
       double        fetch_value;
       double        new_value;

       int           array_locator = 0;
       int           i, cnt;
       int           result;

       char          model_table_name[DATABASE_NAME_LENGTH];
       char          select_statement[1000];
       char          insert_statement[1000];
       int           switched = FALSE;
    /* EXEC SQL END DECLARE SECTION; */ 


   int the_result;

    /* Determine table and date column names based on destination
       and timestep. */

    if (!strcmp (header->timestep, HOUR))
    {
	  sprintf (model_table_name, MODEL_HOUR);
    }
    else if (!strcmp (header->timestep, DAY))
         {
	        sprintf (model_table_name, MODEL_DAY);
         }
         else if (!strcmp (header->timestep, MONTH))
              {
	            sprintf (model_table_name, MODEL_MONTH);
              }
              else
              {
	            PrintError ("Error...timestep %s unknown...\n.", 
                            header->timestep);
	            return (ERROR);
              }
    
    /* Get the start date from run control. No data from before this date 
       will be saved to the DB.  */

    if (riverwareToHdbDate(header->pr_runcontrol_startdate, run_control_start_date,
	header->timestep) != 0) {
        PrintError("DMI: Unable to convert start date (%s) to HDB format when saving data.\n", 
                    header->pr_runcontrol_startdate);
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
		  switched = TRUE;
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

/* ORACLE -- can't set Autocommit Off. Need to do other DB operations 
   while cursor is open. Does it work?? Commit to set new transaction. */

    /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 1;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)current_dbsite;
    sqlstm.sqhstl[0] = (unsigned int  )10;
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


    result = SqlErrorHandler (FUNC_NAME, TRUE,
                              "Problem committing at %s.\n",
                              current_dbsite);
    if (result != OK)
    {
       PrintError("Error...committing at :current_dbsite\n");
       return(ERROR);
    }

    /* ORACLE: Can I use order by clause w/ FOR UPDATE??
	   clause? Or does it still ruin efficiency of circular array. */

    sprintf(select_statement, "SELECT to_char (start_date_time, 'dd-mon-yyyy hh24:mi:ss'), value FROM %s WHERE model_run_id = %d AND site_datatype_id = %d AND start_date_time BETWEEN to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') AND to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') ORDER BY start_date_time FOR UPDATE OF value",
	    model_table_name,
            model_run_id,
            header->hdb_site_datatype_id,
            header->hdb_startdate,
            header->hdb_enddate);

/* PrintError("Statement:\n%s.\n", select_statement); */

    /* EXEC SQL AT :current_dbsite DECLARE SelStmt STATEMENT; */ 

    /* EXEC SQL PREPARE SelStmt FROM :select_statement; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
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
    sqlstm.sqhstv[0] = (         void  *)select_statement;
    sqlstm.sqhstl[0] = (unsigned int  )1000;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)current_dbsite;
    sqlstm.sqhstl[1] = (unsigned int  )10;
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


    result = SqlErrorHandler (FUNC_NAME, TRUE,
                "Problem preparing statement for select from %s.\n", 
	             model_table_name);
    if (result != OK)
      return(ERROR);

    /* EXEC SQL DECLARE UpdCur CURSOR FOR SelStmt; */ 

    result = SqlErrorHandler (FUNC_NAME, TRUE,
                "Problem declaring cursor for %s...\n",
                 select_statement);
    if (result != OK)
      return(ERROR);

    /* EXEC SQL OPEN UpdCur; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
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
    sqlstm.sqhstv[0] = (         void  *)current_dbsite;
    sqlstm.sqhstl[0] = (unsigned int  )10;
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


    result = SqlErrorHandler (FUNC_NAME, TRUE,
                "Problem opening cursor for %s...\n",
                select_statement);
    if (result != OK)
      return(ERROR);
    
    done = 0;
    while (!done) 
    {
     /* Note: The exit condition for this loop is not found on the FETCH.
        This is managed as a special case after the FETCH.  */

/* printf("Fetching a value.\n"); */

      /* EXEC SQL FETCH UpdCur INTO :fetch_date, :fetch_value; */ 

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
      sqlstm.sqhstv[0] = (         void  *)fetch_date;
      sqlstm.sqhstl[0] = (unsigned int  )21;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)&fetch_value;
      sqlstm.sqhstl[1] = (unsigned int  )sizeof(double);
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)current_dbsite;
      sqlstm.sqhstl[2] = (unsigned int  )10;
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


      result = SqlErrorHandler (FUNC_NAME, TRUE,
                     "Problem fetching from %s.\n",
                     model_table_name);
      if (result != OK)
        return(ERROR);

      if (sqlca.sqlcode > 0)  /* FETCH returned no row */
      {
        done = TRUE;
        continue;
      }

/* A value was fetched successfully. Find the corresponding time step
   in the data array. 

   The search looks at every member of the array. But it starts
   at the position of the last successful match. Since records
   from the db are (mostly) in order by date, and the array is
   DEFINATELY in order by date, usually the first comparison
   succeeds.
   
   The variable 'array_locator' is initialized to zero, then each
   time a match is found, this variable is reset to the following
   position in the array.
   
   The '%', or mod, function makes this a circular array. By 
   performing a mod(size of array) on the position, the position
   wraps around to the beginning of the array when i+array_locator
   exceed the size of the array.  */

    found_in_array = 0;
    for (i = 0; i < header->number_of_timesteps; i++) 
    { 
      /* SqlDateCompare does NOT do a commit. If it did, it would be 
          necessary to select the rowid and update/delete based on value 
          of rowid, rather than relying on CURRENT OF cursor.  
          (Precompiler Guide 6-13) */

      the_result = SqlDateCompare(fetch_date,
                              header->data[(array_locator + i) % 
                                header->number_of_timesteps].hdb_date,
                              &result);
      if (the_result != OK) 
      {
         PrintError("Could not compare dates %s and %s.\n",
                    fetch_date,
                    header->data[(array_locator + i) % 
                       header->number_of_timesteps].hdb_date);
         return(ERROR);
      }

            if (result == 0) { /* a match was found for the fetched data */

                /* PrintError("Found a match in %d trie(s).\n", i+1); */

                /* 
                 * If the value in the array is NaN, delete the record
                 *   from the database.
                 */
                if (header->data[(array_locator + i) 
				 % header->number_of_timesteps].is_nan == 'Y')
		  {
		    if (!strcmp (header->timestep, HOUR))
		      {
			/* EXEC SQL AT :current_dbsite DELETE FROM m_hour 
			  WHERE CURRENT OF UpdCur; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 3;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "delete  from m_hour  where rowid = :x";
   sqlstm.iters = (unsigned int  )2;
   sqlstm.offset = (unsigned int  )93;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)current_dbsite;
   sqlstm.sqhstl[0] = (unsigned int  )10;
   sqlstm.sqhsts[0] = (         int  )10;
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


		      }
		    else if (!strcmp (header->timestep, DAY))
		      {
			/* EXEC SQL AT :current_dbsite DELETE FROM m_day
			  WHERE CURRENT OF UpdCur; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 3;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "delete  from m_day  where rowid = :x";
   sqlstm.iters = (unsigned int  )2;
   sqlstm.offset = (unsigned int  )112;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)current_dbsite;
   sqlstm.sqhstl[0] = (unsigned int  )10;
   sqlstm.sqhsts[0] = (         int  )10;
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


		      }
		    else if (!strcmp (header->timestep, MONTH))
		      {
			/* EXEC SQL AT :current_dbsite DELETE FROM m_month
			  WHERE CURRENT OF UpdCur; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 3;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "delete  from m_month  where rowid = :x";
   sqlstm.iters = (unsigned int  )2;
   sqlstm.offset = (unsigned int  )131;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)current_dbsite;
   sqlstm.sqhstl[0] = (unsigned int  )10;
   sqlstm.sqhsts[0] = (         int  )10;
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


		      }
		    else
		      {
			PrintError ("Timestep %s unknown. Exiting.", 
				    header->timestep);
			return (ERROR);
		      }

                    if ((result = SqlErrorHandler (FUNC_NAME, TRUE,
                       "Problem deleting value.\n")) != OK) {
                       return(ERROR);
		     }

		  }
                else {

                    /* PrintError("Updating value.\n"); */

                    /* 
                     * The value in the array is NOT NaN, so set the fetched
                     *   value to that
                     *   in the data array.
                     *
                     * Only update values within the time span of the model run.
                     *   Any data prior to the run control start date are NOT
                     *     saved to the database. This prevents people from 
                     *     overwriting old forecast data with initial condition
                     *     data. In general, don't want to save results outside
                     *     the span of the model run dates.
		     *
		     * This implementation was chosen to avoid resizing the
		     * data arrays and having to walk forward in the RiverWare
		     * data array to match the first date of HDB data.
                     */
                    if (SqlDateCompare(
                        fetch_date,
                        run_control_start_date,
                        &result) != OK) {
                            PrintError("Could not compare dates %s and %s.\n",
                                        run_control_start_date,
                                        fetch_date);
                            return(ERROR);
                    }
                        
                    if (result < 0) {
                        /* PrintError("Value found before run control date. Not changing.\n"); */
                        /* Data is before run control start date, do nothing */
                    }
                    else {
                        /*
                         * Otherwise, the data is after the run control start
                         *   date, so update the value in the database.
                         */
		       new_value = header->data[(array_locator + i) % header->number_of_timesteps].value;
		       
		       if (!strcmp (header->timestep, HOUR))
		       {
			  /*printf("Updating...");*/
			  /* EXEC SQL AT :current_dbsite UPDATE m_hour
			     SET value = :new_value
			     WHERE CURRENT OF UpdCur; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 10;
     sqlstm.arrsiz = 3;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.stmt = "update m_hour  set value=:b1 where rowid = :x";
     sqlstm.iters = (unsigned int  )2;
     sqlstm.offset = (unsigned int  )150;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)256;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (         void  *)&new_value;
     sqlstm.sqhstl[0] = (unsigned int  )sizeof(double);
     sqlstm.sqhsts[0] = (         int  )sizeof(double);
     sqlstm.sqindv[0] = (         void  *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned int  )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqhstv[1] = (         void  *)current_dbsite;
     sqlstm.sqhstl[1] = (unsigned int  )10;
     sqlstm.sqhsts[1] = (         int  )10;
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


		       }
		       else if (!strcmp (header->timestep, DAY))
		       {
			  /*printf("Updating...");*/
			  /* EXEC SQL AT :current_dbsite UPDATE m_day
			     SET value = :new_value
			     WHERE CURRENT OF UpdCur; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 10;
     sqlstm.arrsiz = 3;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.stmt = "update m_day  set value=:b1 where rowid = :x";
     sqlstm.iters = (unsigned int  )2;
     sqlstm.offset = (unsigned int  )173;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)256;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (         void  *)&new_value;
     sqlstm.sqhstl[0] = (unsigned int  )sizeof(double);
     sqlstm.sqhsts[0] = (         int  )sizeof(double);
     sqlstm.sqindv[0] = (         void  *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned int  )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqhstv[1] = (         void  *)current_dbsite;
     sqlstm.sqhstl[1] = (unsigned int  )10;
     sqlstm.sqhsts[1] = (         int  )10;
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


		       }
		       else if (!strcmp (header->timestep, MONTH))
		       {
			  /*printf("Updating...");*/
			  /* EXEC SQL AT :current_dbsite UPDATE m_month
			     SET value = :new_value
			     WHERE CURRENT OF UpdCur; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 10;
     sqlstm.arrsiz = 3;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.stmt = "update m_month  set value=:b1 where rowid = :x";
     sqlstm.iters = (unsigned int  )2;
     sqlstm.offset = (unsigned int  )196;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)256;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (         void  *)&new_value;
     sqlstm.sqhstl[0] = (unsigned int  )sizeof(double);
     sqlstm.sqhsts[0] = (         int  )sizeof(double);
     sqlstm.sqindv[0] = (         void  *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned int  )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqhstv[1] = (         void  *)current_dbsite;
     sqlstm.sqhstl[1] = (unsigned int  )10;
     sqlstm.sqhsts[1] = (         int  )10;
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


		       }
		       else
		       {
			  PrintError ("Timestep %s unknown. Exiting.", 
				      header->timestep);
			  return (ERROR);
		       }
			
                        if ((result = SqlErrorHandler (FUNC_NAME, TRUE,
				      "Problem updating value in m_day.\n")) 
			    != OK) 
			  {
			    return(ERROR);
			  }
		      }
		}
                
                /* 
                 * Set the processed flag to 'Y'.
                 */
                header->data[(array_locator + i) % header->number_of_timesteps].processed_flag = 'Y';

                /* 
                 * Set the array_locator to one position ahead of them match
                 *   that was just found in the array.
                 */
                array_locator = (array_locator + i + 1) % header->number_of_timesteps;

                /* 
                 * Indicate that something was found in the array.
                 *   This is used later to decide if record should be deleted.
                 */
                found_in_array = 1;
                break; /* stop searching the array for a match */
            } /* end if (result == 0) -- a match was found */
            else { /* the comparison did not provide a match. Try another */
                if (result < 0) { 
                    /* The negative case is an unexpected case b/c of order */
                    PrintError("DMI: DmiSaveData() found date values out of order.\n");
                }

                continue;
            }
        } /* done searching array for a match to the fetched row. */

        /* 
         * If the array did not hold a match, delete the row from the database.
         */
        if (!found_in_array) {
	  /* The fetched value is not in data array -- delete it! */
	  if (!strcmp (header->timestep, HOUR))
	    {
	      /* EXEC SQL AT :current_dbsite DELETE FROM m_hour 
		WHERE CURRENT OF UpdCur; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 3;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "delete  from m_hour  where rowid = :x";
       sqlstm.iters = (unsigned int  )2;
       sqlstm.offset = (unsigned int  )219;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (         void  *)current_dbsite;
       sqlstm.sqhstl[0] = (unsigned int  )10;
       sqlstm.sqhsts[0] = (         int  )10;
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


	    }
	  else if (!strcmp (header->timestep, DAY))
	    {
	      /* EXEC SQL AT :current_dbsite DELETE FROM m_day
		WHERE CURRENT OF UpdCur; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 3;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "delete  from m_day  where rowid = :x";
       sqlstm.iters = (unsigned int  )2;
       sqlstm.offset = (unsigned int  )238;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (         void  *)current_dbsite;
       sqlstm.sqhstl[0] = (unsigned int  )10;
       sqlstm.sqhsts[0] = (         int  )10;
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


	    }
	  else if (!strcmp (header->timestep, MONTH))
	    {
	      /* EXEC SQL AT :current_dbsite DELETE FROM m_month
		WHERE CURRENT OF UpdCur; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 3;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "delete  from m_month  where rowid = :x";
       sqlstm.iters = (unsigned int  )2;
       sqlstm.offset = (unsigned int  )257;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (         void  *)current_dbsite;
       sqlstm.sqhstl[0] = (unsigned int  )10;
       sqlstm.sqhsts[0] = (         int  )10;
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


	    }
	  else
	    {
	      PrintError ("Timestep %s unknown. Exiting.", 
			  header->timestep);
	      return (ERROR);
	    }
	  
	  if ((result = SqlErrorHandler (FUNC_NAME, TRUE,
	       "Problem deleting value from m_day.\n")) != OK) {
               return(ERROR);
            }

                /* 
                 * Post a warning that the row is being deleted.
                 */
            PrintError("DMI: Removing value from model table because it was not found in the datafile.\nSite Datatype Id: %d\nDate: %s\n",
                        header->hdb_site_datatype_id,
                        fetch_date);
        }

    } /* end while(!done) -- done fetching rows from the database */

    /* 
     * Close the cursor.
     */
    /* EXEC SQL CLOSE UpdCur; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 3;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )276;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)current_dbsite;
    sqlstm.sqhstl[0] = (unsigned int  )10;
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


    if ((result = SqlErrorHandler (FUNC_NAME, TRUE,
       "Problem closing cursor after select from m_day.\n")) != OK) {
       return(ERROR);
    }

/* PrintError("Done fetching values.\n"); */
                     
    /*
     * Now go back and see what values were not originally present in the db 
     *   for update. All values with processed_flag == 'N' must be inserted
     */
    sprintf(insert_statement, "INSERT INTO %s (model_run_id, site_datatype_id, start_date_time, end_date_time, value) VALUES (%d, %d, to_date (:d, 'dd-mon-yyyy hh24:mi:ss'), to_date (:e, 'dd-mon-yyyy hh24:mi:ss'), :v)",
	                       model_table_name,
	                       model_run_id,
                               header->hdb_site_datatype_id);

    /* Declare and prepare insert statement depending on which
       database is current */
    if (!strcmp (current_dbsite, dbsite_array[0].dbsite_alias))
    {
       /* EXEC SQL AT :current_dbsite DECLARE InsStmt STATEMENT; */ 

       /* EXEC SQL PREPARE InsStmt FROM :insert_statement; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 3;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )295;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (         void  *)insert_statement;
       sqlstm.sqhstl[0] = (unsigned int  )1000;
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         void  *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned int  )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (         void  *)current_dbsite;
       sqlstm.sqhstl[1] = (unsigned int  )10;
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


    }
    else if (!strcmp (current_dbsite, dbsite_array[1].dbsite_alias))
    {
       /* EXEC SQL AT :current_dbsite DECLARE InsStmt1 STATEMENT; */ 

       /* EXEC SQL PREPARE InsStmt1 FROM :insert_statement; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 3;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )318;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (         void  *)insert_statement;
       sqlstm.sqhstl[0] = (unsigned int  )1000;
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         void  *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned int  )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (         void  *)current_dbsite;
       sqlstm.sqhstl[1] = (unsigned int  )10;
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


    }
    else if (!strcmp (current_dbsite, dbsite_array[2].dbsite_alias))
    {
       /* EXEC SQL AT :current_dbsite DECLARE InsStmt2 STATEMENT; */ 

       /* EXEC SQL PREPARE InsStmt2 FROM :insert_statement; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 3;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )341;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (         void  *)insert_statement;
       sqlstm.sqhstl[0] = (unsigned int  )1000;
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         void  *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned int  )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (         void  *)current_dbsite;
       sqlstm.sqhstl[1] = (unsigned int  )10;
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


    }
    else if (!strcmp (current_dbsite, dbsite_array[3].dbsite_alias))
    {
       /* EXEC SQL AT :current_dbsite DECLARE InsStmt3 STATEMENT; */ 

       /* EXEC SQL PREPARE InsStmt3 FROM :insert_statement; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 10;
       sqlstm.arrsiz = 3;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )364;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (         void  *)insert_statement;
       sqlstm.sqhstl[0] = (unsigned int  )1000;
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         void  *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned int  )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (         void  *)current_dbsite;
       sqlstm.sqhstl[1] = (unsigned int  )10;
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


    }
    else
    {
       PrintError ("current_dbsite does not match any sites in dbsite_array.\n Exiting.");
       return (ERROR);
    }
 
    /* Check prepare for Oracle errors */
    if ((result = SqlErrorHandler (FUNC_NAME, TRUE,
       "Problem preparing statement for insert into %s.\n", 
       model_table_name)) != OK) {
       return(ERROR);
    }

    /*
     * Look at every member of the array and handle any not marked as processed
     */
    for (i = 0; i < header->number_of_timesteps; i++) {
        if (header->data[i].processed_flag == 'N') {
            new_value = header->data[i].value;
            strcpy(new_date, header->data[i].hdb_date);
            strcpy(end_date, header->data[i].end_date);

            if (header->data[i].is_nan == 'Y') {
                /* Do nothing -- don't insert the record */
                /* PrintError("Did not insert a NaN value.\n"); */
            }
            else {

                /*
                 * Only insert values within the time span of the model run.
                 *   Any data prior to the run control start date are NOT
                 *     saved to the database. This prevents people from
                 *     overwriting old forecast data with initial condition
                 *     data. In general, don't want to save results outside
                 *     the span of the model run dates.
                 */

                if (SqlDateCompare(
                    new_date,
                    run_control_start_date,
                    &result) != OK) {
                        PrintError("Could not compare dates %s and %s.\n",
                                    run_control_start_date,
                                    new_date);
                        return(ERROR);
                }
                   
                if (result < 0) {
                    /* value before run control start date */
                    /* Do Nothing */
                    /* PrintError("Value found before run control date. Not changing.\n"); */
                }
                else {
                    /* Data is after run control start date -- insert it */
                    /* PrintError("Inserting a value.\n"); */
		   if (!strcmp (current_dbsite, dbsite_array[0].dbsite_alias))
		   {
		      /* EXEC SQL AT :current_dbsite EXECUTE InsStmt
			 USING :new_date, :end_date, :new_value; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )387;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)256;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)new_date;
        sqlstm.sqhstl[0] = (unsigned int  )21;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)end_date;
        sqlstm.sqhstl[1] = (unsigned int  )21;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)&new_value;
        sqlstm.sqhstl[2] = (unsigned int  )sizeof(double);
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)current_dbsite;
        sqlstm.sqhstl[3] = (unsigned int  )10;
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


		   }
		   else if (!strcmp (current_dbsite, dbsite_array[1].dbsite_alias))
		   {
		      /* EXEC SQL AT :current_dbsite EXECUTE InsStmt1
			 USING :new_date, :end_date, :new_value; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )418;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)256;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)new_date;
        sqlstm.sqhstl[0] = (unsigned int  )21;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)end_date;
        sqlstm.sqhstl[1] = (unsigned int  )21;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)&new_value;
        sqlstm.sqhstl[2] = (unsigned int  )sizeof(double);
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)current_dbsite;
        sqlstm.sqhstl[3] = (unsigned int  )10;
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


		   }
		   else if (!strcmp (current_dbsite, dbsite_array[2].dbsite_alias))
		   {
		      /* EXEC SQL AT :current_dbsite EXECUTE InsStmt2
			 USING :new_date, :end_date, :new_value; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )449;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)256;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)new_date;
        sqlstm.sqhstl[0] = (unsigned int  )21;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)end_date;
        sqlstm.sqhstl[1] = (unsigned int  )21;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)&new_value;
        sqlstm.sqhstl[2] = (unsigned int  )sizeof(double);
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)current_dbsite;
        sqlstm.sqhstl[3] = (unsigned int  )10;
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


		   }
		   else if (!strcmp (current_dbsite, dbsite_array[3].dbsite_alias))
		   {
		      /* EXEC SQL AT :current_dbsite EXECUTE InsStmt3
			 USING :new_date, :end_date, :new_value; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )480;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)256;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)new_date;
        sqlstm.sqhstl[0] = (unsigned int  )21;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)end_date;
        sqlstm.sqhstl[1] = (unsigned int  )21;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)&new_value;
        sqlstm.sqhstl[2] = (unsigned int  )sizeof(double);
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         void  *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned int  )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (         void  *)current_dbsite;
        sqlstm.sqhstl[3] = (unsigned int  )10;
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


		   }
		   else
		   {
		      PrintError ("current_dbsite does not match any sites in dbsite_array.\n Exiting.");
		      return (ERROR);
		   }
		   /* Check results of insert for Oracle error */
		   if ((result = SqlErrorHandler (FUNC_NAME, TRUE,
		       "Problem executing insert into %s.\n", model_table_name
                        )) != OK) {
		      return(ERROR);
		   }
                }
            }

            header->data[i].processed_flag = 'Y';
        } /* end if (value not processed) */
        else {
	   /* the value was already processed */
	   /* Do Nothing */
	   /* PrintError("Found Value Already Processed.\n"); */
        }
    }

    /* ORACLE -- can't set autocommit on, but commit anyway. */
    /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 4;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )511;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)current_dbsite;
    sqlstm.sqhstl[0] = (unsigned int  )10;
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


    if ((result = SqlErrorHandler (FUNC_NAME, TRUE,
       "Problem committing.\n")) != OK) {
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
