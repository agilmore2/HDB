
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
    "DmiSaveRealData.pc"
};


static unsigned long sqlctx = 18891523;


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
5,0,0,1,0,0,1053,219,0,0,0,0,0,0,128,1,97,0,0,
24,0,0,2,0,0,1041,248,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
47,0,0,2,0,0,1069,261,0,0,0,0,0,0,128,1,97,0,0,
66,0,0,2,0,0,1037,276,0,0,2,0,0,0,128,2,97,0,0,2,4,0,0,1,97,0,0,
93,0,0,3,37,0,1071,337,0,0,0,0,0,0,128,1,97,0,0,
112,0,0,4,45,0,1070,396,0,0,1,1,0,0,128,1,4,0,0,1,97,0,0,
135,0,0,2,0,0,1039,437,0,0,0,0,0,0,128,1,97,0,0,
154,0,0,5,0,0,1041,461,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
177,0,0,5,0,0,1045,512,0,0,3,3,0,0,128,1,97,0,0,1,97,0,0,1,4,0,0,1,97,0,0,
208,0,0,6,0,0,1053,529,0,0,0,0,0,0,128,1,97,0,0,
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

       int           done = 0;
       int           found_in_array;
       SQL_DATE      end_date;
       SQL_DATE      fetch_date;
       SQL_DATE      new_date;
       SQL_DATE      run_control_start_date;
       SQL_INTERVAL  dataInterval;
       double        fetch_value;
       double        new_value;

       int           agencyId;
       int           array_locator = 0;
       int           collectionId;
       int           computationId;
       int           i, cnt;
       int           loadingId;
       int           methodId;
       int           result;

       char          select_statement[1000];
       char          insert_statement[1000];
       int           switched = FALSE;
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
       date will be saved to the DB.  */

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

    /* cnt = 0;

    * Search for the needed session in the array of sessions *

    while (cnt < number_of_db_sites) 
	{
	  if (!strcmp(dbsite_array[cnt].dbsite_alias, header->data_location))
	    break;
	  cnt++;
    }
      * The appropriate member was found *

    if (cnt < number_of_db_sites) 
	{ 
	  * Return if not connected to that session -- no way to save
	     the data *

	  if (!dbsite_array[cnt].connected) 
      {
	    PrintError("Not connected to session for %s. Cannot fetch data...\n",
	               dbsite_array[cnt].dbsite_alias);
	      
	      * switch the session back to default. *

        if (SwitchSession(dbsite_array[0].dbsite_alias) != OK) 
		{
		  PrintError("Error...changing to session 1 for %s\n",
			     dbsite_array[0].dbsite_alias);
		  return(ERROR);
        }

	    return(ERROR);
      }
	  else * attempt to switch to the needed session *
	  {
	    if (SwitchSession(dbsite_array[cnt].dbsite_alias) != OK) 
		{
          PrintError("ERROR occurred changing to session %d for %s\n",
		             dbsite_array[cnt].session_number,
			         dbsite_array[cnt].dbsite_alias);
		  return(ERROR);
		}
	    else  * Switch was successful. *
		  switched = TRUE;
	  }
	}
    else * The name is not in the array. Exit. *
	{
	  PrintError("Could not find database session ID for %s.\n",
                 header->data_location);
	  return(ERROR);
    } */
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

/* ORACLE: Can I use order by clause with FOR UPDATE??
   clause? Or does it still ruin efficiency of circular array. */

    sprintf(select_statement, 
            "SELECT to_char (start_date_time, 'dd-mon-yyyy hh24:mi:ss'), value \
             FROM r_base \
             WHERE site_datatype_id = %d \
             AND interval = '%s' \
             AND start_date_time BETWEEN to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') \
                    AND to_date ('%s', 'dd-mon-yyyy hh24:mi:ss') \
             ORDER BY start_date_time FOR UPDATE OF value",
	         header->hdb_site_datatype_id,
                 dataInterval,
	         header->hdb_startdate, header->hdb_enddate);

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
                "Problem preparing statement for select from r_base...\n");
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
                     "Problem fetching from r_base.\n");
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

      if (result == 0) /* a match was found for the fetched data */
      {
        /* PrintError("Found a match in %d attempt(s).\n", i+1); */

        /* If the value in the array is NaN, delete the record
           from the r_table. */

        if (header->data[(array_locator + i) % 
            header->number_of_timesteps].is_nan == 'Y')
        {
           /* EXEC SQL AT :current_dbsite DELETE FROM r_base 
           WHERE CURRENT OF UpdCur; */ 

{
           struct sqlexd sqlstm;
           sqlstm.sqlvsn = 10;
           sqlstm.arrsiz = 3;
           sqlstm.sqladtp = &sqladt;
           sqlstm.sqltdsp = &sqltds;
           sqlstm.stmt = "delete  from r_base  where rowid = :x";
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


  
           result = SqlErrorHandler (FUNC_NAME, TRUE,
                                   "Problem deleting value.\n");
           if (result != OK) 
              return(ERROR);
        }
        else 
        {
          /* PrintError("Updating value.\n"); */

          /* The value in the array is NOT NaN, so set the fetched
             value to that in the data array.

             Only update values within the time span of the model run.
             Any data prior to the run control start date are NOT
             saved to the database. This prevents people from 
             overwriting old forecast data with initial condition
             data. In general, don't want to save results outside
             the span of the model run dates.
		      
		     This implementation was chosen to avoid resizing the
		     data arrays and having to walk forward in the RiverWare
		     data array to match the first date of HDB data.  */

          the_result = SqlDateCompare(fetch_date,
                                      run_control_start_date,
                                      &result);
          if (the_result != OK) 
          {
            PrintError("Could not compare dates %s and %s.\n",
                       run_control_start_date,
                       fetch_date);
            return(ERROR);
          }
                        
          if (result < 0) 
          {
            /* PrintError("Value found before run control date. Not changing.\n"); */
            /* Data before run control start date, do nothing */

          }
          else 
          {
            /* The data are after the run control start
               date, so these must be updated */

            new_value = header->data[(array_locator + i) % 
                        header->number_of_timesteps].value;

            /* Only update if value has changed - difference factor put here */
            /* to account for differences in how values stored at small */
            /* decimal places - Note could affect the storage of very small */
            /* values into the database */     
            if( fabs(new_value - fetch_value) > 0.0000001 )

            {
		       
               /* EXEC SQL AT :current_dbsite UPDATE r_base
                  SET value = :new_value
                  WHERE CURRENT OF UpdCur; */ 

{
               struct sqlexd sqlstm;
               sqlstm.sqlvsn = 10;
               sqlstm.arrsiz = 3;
               sqlstm.sqladtp = &sqladt;
               sqlstm.sqltdsp = &sqltds;
               sqlstm.stmt = "update r_base  set value=:b1 where rowid = :x";
               sqlstm.iters = (unsigned int  )2;
               sqlstm.offset = (unsigned int  )112;
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


		
               result = SqlErrorHandler (FUNC_NAME, TRUE,
                                "Problem updating value in r_base...\n");
               if (result != OK)
   			  return(ERROR);
            }
		  }
        }
                
        /* either skip or update is done, so set the processed flag to 'Y'.  */

        header->data[(array_locator + i) % 
                       header->number_of_timesteps].processed_flag = 'Y';

        /* Set the array_locator to one position ahead of the match
           that was just found in the array.  */

        array_locator = (array_locator + i + 1) % 
                            header->number_of_timesteps;

        /* Indicate that something was found in the array.
           This is used later to decide if the record should be further
           processed. */

        found_in_array = 1;
        break; /* stop searching the array for a match */
      } /* end if (result == 0) -- a match was found */
      else  /* no match found. Try another */
      {
        if (result < 0) /* unexpected case because of order */
          PrintError("Error...in %s. Found date values out of order...\n",FUNC_NAME);
        continue;
      }
    } /* end for-loop; done searching array for a match to the fetched row. */
    } /* end while(!done) -- done fetching rows from the database */

    /* Close the cursor.  */

    /* EXEC SQL CLOSE UpdCur; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 3;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )135;
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
               "Problem closing cursor after select from r_base...\n");
    if (result != OK)
       return(ERROR);

    /* PrintError("Done fetching values.\n"); */
                     
    /* Now go back and see what values were not originally present in the db 
       for update. All values with processed_flag == 'N' must be inserted */

    sprintf(insert_statement, 
            "INSERT INTO r_base (site_datatype_id, interval, start_date_time, end_date_time, value, agen_id, collection_system_id, loading_application_id, method_id, computation_id) VALUES(%d, '%s', to_date (:d, 'dd-mon-yyyy hh24:mi:ss'), to_date (:e, 'dd-mon-yyyy hh24:mi:ss'), :v, %d, %d, %d, %d, %d)",
                header->hdb_site_datatype_id,
                dataInterval,
                agencyId,
                collectionId,
                loadingId,
                methodId,
                computationId);

    /* EXEC SQL AT :current_dbsite DECLARE InsStmt STATEMENT; */ 


    /* ORACLE -- this could choke!! */
    /* EXEC SQL PREPARE InsStmt FROM :insert_statement; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 3;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )154;
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


    result = SqlErrorHandler (FUNC_NAME, TRUE,
               "Problem preparing statement for insert into r_base...\n");
    if (result != OK)
      return(ERROR);

    /* Look at every member of the array and handle (update or insert) any not 
       marked as processed */

    for (i = 0; i < header->number_of_timesteps; i++) 
    {
      if (header->data[i].processed_flag == 'N') 
      {
        new_value = header->data[i].value;
        strcpy(new_date, header->data[i].hdb_date);
        strcpy(end_date, header->data[i].end_date);

        if (header->data[i].is_nan == 'Y') 
        {
          /* Do nothing -- don't insert the record */
          /* PrintError("Did not insert an NaN value.\n"); */
        }
        else 
        {
          /* Only insert values within the time span of the model run.
             Any data prior to the run control start date are NOT
             saved to the database. This prevents people from
             overwriting old forecast data with initial condition
             data. In general, don't want to save results outside
             the span of the model run dates.  */

          the_result = SqlDateCompare(new_date, run_control_start_date,
                                      &result);
          if (the_result != OK) 
          {
            PrintError("Could not compare dates %s and %s.\n",
                       run_control_start_date,
                       new_date);
            return(ERROR);
          }
                   
          if (result < 0) /* value before run control start date: Do nothing */
          {
            /* PrintError("Value found before run control date. Not changing.\n"); */
          }
          else 
          {
            /* Date is after run control start date -- insert a record */

            /* PrintError("Inserting a value.\n"); */
 
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
       sqlstm.offset = (unsigned int  )177;
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


 		    result = SqlErrorHandler (FUNC_NAME, TRUE,
 		                "Problem executing insert into r_base...\n");
            if (result != OK)
 		      return(ERROR);
          }
        }
        header->data[i].processed_flag = 'Y';
      } /* end if (value not processed) */
      else /* the value was already processed: Do nothing */
      {
        /* PrintError("Found Value Already Processed.\n"); */
      }
    } /* end for-loop */

    /* ORACLE -- can't set autocommit on, but commit anyway. */
    /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 4;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )208;
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
                    "Problem committing...\n");
    if (result != OK)
      return(ERROR);

    /* Set session back to default, if switch was made. */
    if (switched)
      if (SwitchSession(dbsite_array[0].dbsite_alias) != OK) 
	  {
	    PrintError("Error... changing to default session 1 for %s\n",
                   dbsite_array[0].dbsite_alias);
	    return(ERROR);
	  }

    return(OK);
}
