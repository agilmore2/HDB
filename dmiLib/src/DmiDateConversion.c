
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
           char  filnam[21];
};
static const struct sqlcxp sqlfpn =
{
    20,
    "DmiDateConversion.pc"
};


static unsigned long sqlctx = 74531035;


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
            void  *sqhstv[1];
   unsigned int   sqhstl[1];
            int   sqhsts[1];
            void  *sqindv[1];
            int   sqinds[1];
   unsigned int   sqharm[1];
   unsigned int   *sqharc[1];
   unsigned short  sqadto[1];
   unsigned short  sqtdso[1];
} sqlstm = {10,1};

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
5,0,0,1,0,0,1055,306,0,0,0,0,0,0,128,1,97,0,0,
24,0,0,2,0,0,1055,312,0,0,0,0,0,0,128,1,97,0,0,
43,0,0,3,0,0,1055,318,0,0,0,0,0,0,128,1,97,0,0,
};


/*
**
**	IDENTIFICATION SECTION
**	----------------------
* DmiCharToNumericDate
*
*   Converts a character-based Oracle date to pure numeric format. Note that 
*   the input date must have an hour/minute/second portion.
*
* Purpose: Converts a date like '10-dec-1995 12:00:00' to '10/12/1995 12:00:00'
*          This is used by the dmi routines so can convert to RiverWare date 
*          format.
*
* NOTE: This behavior is new to the Oracle version. The Ingres version returned
*       a date in format mm/dd/yyyy; this version returns dd/mm/yyyy because
*       of necessary changes to other date functions.
* Input
*       char *              - char date
*	                      MUST BE IN dd-mon-yyyy hh24:mi:ss format!!
*       char *              - resulting numeric date
* Output
*       char * - calculated date/time WITHOUT ANY CHARACTERS IN MONTH
*
* Return
*       ERROR - date/time not successfully converted
*       OK - date successfully converted
*
*
*****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*  hdb apps include file to define OK and ERRO  */
#include "typedefs.h"
#include "defines.h"
#include "dbutils.h"
#include "utils.h"
#include "dmi_utils.h"

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


#define FUNC_NAME "riverwareToHdbDate"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> riverwareToHdbDate

<P> <STRONG>Function signature:</STRONG> int riverwareToHdbDate (char *, SQL_DATE, char *)

<P> <STRONG>Function description:</STRONG> Converts date in Riverware format to HDB format; e.g., yyyy-mm-dd -> dd-mon-yyyy.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *riverwareDate:  Date in Riverware format (yyyy-mm-dd).
<BR><DD> SQL_DATE hdbDate: Date in HDB format (dd-mon-yyyy). Output.
<BR><DD> char *timestep:   Timestep represented by the date.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int riverwareToHdbDate(char *riverwareDate, SQL_DATE hdbDate, char *timestep)
{
  int year, 
      month, 
      day, 
      hour = 0,
      minute = 0,
      second = 0;
  
  if (riverwareDate != NULL && hdbDate != NULL) 
    {
      /* Hourly -- subtract one hour */
      if (!strcmp (timestep, HOUR)) /* AND IF AVG DATA */
	{
	  if(sscanf(riverwareDate, "%d-%d-%d %d:%d:%d", &year, &month, &day,
		    &hour, &minute, &second) < 4) 
	    {
	      PrintError("DMI: Error interpreting prysm date (%s).\n", 
			 riverwareDate);
	      hdbDate[0] = '\0';
	      return ERROR;
	    }

	  sprintf(hdbDate, "%02d-%02d-%04d %02d:%02d:%02d", day, month,  
		  year, hour - 1, minute, second);
	}

      /* Daily -- just set the hours to 00. */
      else if (!strcmp (timestep, DAY)) 
	{
	  if(sscanf(riverwareDate, "%d-%d-%d", &year, &month, &day) != 3) 
	    {
	      PrintError("DMI: Error interpreting prysm date (%s).\n", 
			 riverwareDate);
	      hdbDate[0] = '\0';
	      return ERROR;
	    }
	  
	  sprintf(hdbDate, "%02d-%02d-%04d 00:00:00", day, month, year);
	}

      /* Month -- set hours to 00, use first day of month. */
      else if (!strcmp (timestep, MONTH))
	{
	  if(sscanf(riverwareDate, "%d-%d-%d", &year, &month, &day) != 3) 
	    {
	      PrintError("DMI: Error interpreting prysm date (%s).\n", 
			 riverwareDate);
	      hdbDate[0] = '\0';
	      return ERROR;
	    }
	  
	  sprintf(hdbDate, "01-%02d-%04d 00:00:00", month, year);
	}

      /* Unknown timestep */
      else
	{
	  PrintError ("DMI: Timestep %s cannot be processed by riverwareToHdbDate.\n\tExiting.", timestep);
	  hdbDate[0] = '\0';
	  return (ERROR);
	}
    }
  else 
    {
      PrintError("DMI: Null pointer passed in to riverwareToHdbDate().\n");
      return ERROR;
    }

  /* Format the hdbDate to ensure it's in 'dd-mon-yyyy hh24:mi:ss' format */
  if (SqlFormatDate (hdbDate, hdbDate) != OK)
  {
     PrintError("Error formatting hdb date (%s).\n", hdbDate);
     return ERROR;
  }
  
  return OK;
}

/********************************************************************/
#define FUNC_NAME "hdbToRiverWareDate"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> hdbToRiverWareDate

<P> <STRONG>Function signature:</STRONG> int hdbToRiverWareDate (SQL_DATE, char *, char *)

<P> <STRONG>Function description:</STRONG> Converts date in HDB format to Riverware format; e.g., dd-mm-yyyy -> yyyy-mm-dd.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE hdbDate: Date in HDB format (dd-mon-yyyy).
<BR><DD> char *riverwareDate:  Date in Riverware format (yyyy-mm-dd). Output.
<BR><DD> char *timestep:   Timestep represented by the date.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int hdbToRiverWareDate(SQL_DATE hdbDate, char *riverwareDate, char *timestep)
{
   int year, 
       month, 
       day,
       hour,
       minute = 0,   
       second = 0,
       result;

   char charMonth[3];

  if (riverwareDate != NULL && hdbDate != NULL) 
    {
       if (SqlExtractDatePart (hdbDate, MONTH, &month) != OK)
       {
	  PrintError("Error extracting month from (%s).\n", hdbDate);
	  riverwareDate[0] = '\0';
	  return ERROR;
       }

       /* Hourly -- add one hour */
      if (!strcmp (timestep, HOUR)) /* AND IF AVG DATA */
	{
	  if(sscanf(hdbDate, "%d-%3c-%d %d:%d:%d", &day, charMonth, &year,
		    &hour, &minute, &second) < 4) 
	    {
	      PrintError("DMI: Error interpreting hdb date (%s).\n", hdbDate);
	      riverwareDate[0] = '\0';
	      return ERROR;
	    }

	  sprintf(riverwareDate, "%04d-%02d-%02d %02d:%02d", year, month, day,
		  hour + 1, minute);
	}

      /* Daily -- hour = 24:00 */
      else if (!strcmp (timestep, DAY)) 
	{
	  if(sscanf(hdbDate, "%d-%3c-%d", &day, charMonth, &year) != 3) 
	    {
	      PrintError("DMI: Error interpreting hdb date (%s).\n", hdbDate);
	      riverwareDate[0] = '\0';
	      return ERROR;
	    }
	  
	  sprintf(riverwareDate, "%04d-%02d-%02d 24:00", year, month, day);
	}

      /* Month -- add 1 month, subtract 1 day, hour = 24:00.
	 ==> Last day of month. */
      else if (!strcmp (timestep, MONTH))
	{
	  if ((result = SqlDateMath (ADDITION, hdbDate, riverwareDate,
				     1, timestep)) != OK)
	    {
	      PrintError ("DMI: Problem adding 1 month to %s.\n\tExiting.",
			  hdbDate);
	      riverwareDate[0] = '\0';
	      return (ERROR);
	    }

	  if ((result = SqlDateMath (SUBTRACTION, riverwareDate, riverwareDate,
				     1, DAY)) != OK)
	    {
	      PrintError ("DMI: Problem subtracting 1 day from %s.\n\tExiting.",
			  hdbDate);
	      riverwareDate[0] = '\0';
	      return (ERROR);
	    }
	    
	  /* Convert date back to numeric */
	  if ((result = DmiCharToNumericDate (riverwareDate, riverwareDate)) != OK)
	    {
	      PrintError("DMI: Error converting %s to numeric format.\n", 
			 riverwareDate);
	      riverwareDate[0] = '\0';
	      return ERROR;
	    }

	  if(sscanf(riverwareDate, "%d-%d-%d", &day, &month, &year) != 3) 
	    {
	      PrintError("DMI: Error interpreting RiverWare date (%s).\n", 
			 riverwareDate);
	      riverwareDate[0] = '\0';
	      return ERROR;
	    }

	  sprintf(riverwareDate, "%04d-%02d-%02d 24:00", year, month, day);
	}

      /* Unknown timestep */
      else
	{
	  PrintError ("DMI: Timestep %s cannot be processed by hdbToRiverWareDate.\n\tExiting.", timestep);
	  riverwareDate[0] = '\0';
	  return (ERROR);
	}
    }
  else 
    {
      PrintError("DMI: Null pointer passed in to hdbToRiverWareDate().\n");
      return ERROR;
    }
   
   return OK;
   
 }

/****************************************************************************/
#define FUNC_NAME "DmiCharToNumericDate"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> DmiCharToNumericDate

<P> <STRONG>Function signature:</STRONG> int DmiCharToNumericDate (char *, char *)

<P> <STRONG>Function description:</STRONG> Converts a character-based Oracle date to pure numeric format; e.g., '10-dec-1995 12:00:00' to '10/12/1995 12:00:00'

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *char_date:    Character date. MUST be in format dd-mon-yyyy hh24:mi:ss
<BR><DD> char *numeric_date: Resulting numeric date.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int DmiCharToNumericDate(char * char_date, char * numeric_date)
{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    int        month,
               day,
               year;
    char       *isTime,
               *time;
  /* EXEC SQL END DECLARE SECTION; */ 


    if (SqlExtractDatePart(char_date, DAY, &day) != OK) {
        PrintError("DMI: Error getting day from date in DmiCharToNumericDate.\n");
        /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

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


        return (ERROR);
    }

    if (SqlExtractDatePart(char_date, MONTH, &month) != OK) {
        PrintError("DMI: Error getting month from date in DmiCharToNumericDate.\n");
        /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

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


        return (ERROR);
    }

    if (SqlExtractDatePart(char_date, YEAR, &year) != OK) {
        PrintError("DMI: Error getting year from date in DmiCharToNumericDate.\n");
        /* EXEC SQL AT :current_dbsite ROLLBACK; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 10;
        sqlstm.arrsiz = 1;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )43;
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


        return (ERROR);
    }

  /* There must be a time portion to the input date. So, look for a ":" 
     in order to find hour, minute, second. */
  isTime = strstr (char_date, ":");
  if (isTime)
    {
      time = &(char_date[strlen(char_date) - strlen(isTime) - 2]);
      if ((strlen (time) < 8) || 
	  ((strlen (time) > 8) && (strcspn (time, " ") != 8)))
      {
	 PrintError ("Time portion of date (%s) is incorrect; must be hh24:mi:ss. Exiting.\n", time);
	 return (ERROR);
      }
      /* Create dd-mm-yyyy portion, as you know you have everything you need */
      sprintf(numeric_date, "%02d-%02d-%04d", day, month, year);

      time[8] = '\0';
      strcat (numeric_date, " ");
      strcat (numeric_date, time);
    }
  else
  {
     PrintError ("Date is missing time portion; must be hh24:mi:ss. Exiting.\n", time);
     return (ERROR);
  }
  
  return (OK);
}

/*****************************************************************************/
#define FUNC_NAME       "DmiFastDateCompare"

/* int DmiFastDateCompare(SQL_DATE firstDate,
                                    SQL_DATE secondDate,
                                    int *compareResult) */

/* Use SqlDateCompare in hdbLib in place of this!! */


/*****************************************************************************/
#define FUNC_NAME	"DmiNumIntervalsBetweenDates"

/* int DmiNumIntervalsBetweenDates(char *sDate, char *eDate, 
				char *interval, int *numIntervals) */

/* Use SqlNumIntervalsBetweenDates in place of this function */
