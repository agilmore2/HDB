
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
    "SqlBoundaryDates.pc"
};


static unsigned int sqlctx = 42116675;


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
   unsigned char  *sqhstv[1];
   unsigned long  sqhstl[1];
            int   sqhsts[1];
            short *sqindv[1];
            int   sqinds[1];
   unsigned long  sqharm[1];
   unsigned long  *sqharc[1];
   unsigned short  sqadto[1];
   unsigned short  sqtdso[1];
} sqlstm = {12,1};

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
};


/*****************************************************************************
* DESCRIPTION
*
* This file contains the function which loads driver data from HDB, 
* and the function which inserts a data-derivation record into
* the ref_data_derivation table.
*
*
*
* Author
*
* Carol Marra
* February, 1996
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aggDisagg.h"
#include "aggDisaggDefines.h"
#include "utils.h"
#include "dbutils.h"
#include "defines.h"
#include "typedefs.h"
#include "aggDisaggTypedefs.h"

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


extern TIME_INTERVALS timeIntervals[NUM_TIME_INTERVALS];

#define FUNC_NAME "SetBoundaryDates"
int SetBoundaryDates (RUN_CONTROL runControl, BOUNDARY_DATES **boundaryDates,
		      int *numBoundaries)
{
  int result;
  double divisor = 0.0;
  UNIT_INFO driverUnitInfo,
            subUnitInfo;
  
  /* Number of sets of boundary dates is equal to numPer. */
  *numBoundaries = runControl.numPer;

  if ((*boundaryDates = (BOUNDARY_DATES *) malloc (sizeof (BOUNDARY_DATES) *
						  *numBoundaries)) == NULL)
    {
      PrintError ("Error malloc'ing boundaryDates.\n\tExiting.");
      return (ERROR);
    }

  /* Get unit info for both observation periods. Do this so
     that if divisor must be calculated for every boundary date
     pair (one observation period > BY_DAY), the unit info retrieval 
     is done only once. Can bypass ConvertValue and jump to SimpleConvert, 
     because all time conversions are factors. */
/* fix attempting to find a unit for instantaneous, where it doesn't apply */

  if ((runControl.obsDriver != BY_INSTANT) &&
   (result = SqlGetUnitInfo (runControl.hdbObsDriver, 
				&driverUnitInfo)) != OK)
    {
      PrintError ("Can't get info for unit %d\n\tExiting.", 
		  runControl.hdbObsDriver);
      return (ERROR);
    }
  
  if ((runControl.obsSub != BY_INSTANT) &&
     (result = SqlGetUnitInfo (runControl.hdbObsSub, 
				&subUnitInfo)) != OK)
    {
      PrintError ("Can't get info for unit %d\n\tExiting.",
		  runControl.hdbObsSub);
      return (ERROR);
    }
  
  /* Now get the divisor. If source and destination observation periods
     are equal, it is 1. If both source and destination intervals are
     BY_DAY or less, divisor can be calculated once (it is constant
     and not dependent on boundary dates). Divisors are not applicable for 
     BY_INSTANT observation periods. Note that this divisor
     won't be used when stats are being created. */
  if (runControl.obsSub == BY_INSTANT)
    {
      divisor = NA;
    }
  else if (runControl.obsSub == runControl.obsDriver)
    {
      divisor = 1;
    }
  else if (runControl.obsSub <= BY_DAY && runControl.obsDriver <= BY_DAY)
    {
      /* Go ahead and calculate the divisor once. Start value is 1.
	 Start date is unimportant; it will not be used since it's 
	 guaranteed that sub and driver are BY_DAY or less. 
	 Pass in BY_DAY interval, as it will not be used. */
      if ((result = SimpleConvert (1, 1, driverUnitInfo, NULL_DATE,
				   &divisor, 1, subUnitInfo, NA)) != OK)
	{
	  PrintError ("Can't calculate divisor for driver unit %d, sub unit %d\n\tExiting.", runControl.hdbObsDriver, runControl.hdbObsSub);
	  return (ERROR);
	}
    }

  if ((result = CreateBoundaryDatePairs (runControl, *boundaryDates, 
					 *numBoundaries, divisor,
					 driverUnitInfo, subUnitInfo)) != OK)
    {
      return (ERROR);
    }

  return (OK);
}


/******************************************************************************/
#define FUNC_NAME "CreateBoundaryDatePairs"
int CreateBoundaryDatePairs (RUN_CONTROL runControl, 
			     BOUNDARY_DATES *boundaryDates, 
			     int numBoundaries, 
			     double divisor, UNIT_INFO driverUnitInfo,
			     UNIT_INFO subUnitInfo)
{
  SQL_DATE dateLow,
           dateHigh;
  int calcDivisor = !(divisor),
      boundary,
      result;

  /* Initialize dateLow and dateHigh for first iteration. */
  strcpy (dateLow, runControl.dateStart);
  SqlDateMath (ADDITION, dateLow, dateHigh, 1, runControl.sqlObsDriver);

  /* If obsSub and obsDriver are equal, as they are for dimension changes
  in base area, need to subtract 1 second to get a valid dateHigh -- one
  that is not equal to dateLow. This allows for source values that 
  have a non-normalized startDate in r_base to be included. Otherwise,
  only startDates right on the interval start would be allowed. */
  if (!strcmp (runControl.sqlObsSub, runControl.sqlObsDriver))
     SqlDateMath (SUBTRACTION, dateHigh, dateHigh, 1, SECOND);
  else
     SqlDateMath (SUBTRACTION, dateHigh, dateHigh, 1, runControl.sqlObsSub);

  /* Now create each boundary date pair, setting (and calculating, if 
     necessary) the divisor for each date range. */
  for (boundary = 0; boundary < numBoundaries; boundary++)
    {
      /* Set values on current element of boundaryDates */
      strcpy (boundaryDates[boundary].dateLow, dateLow);
      strcpy (boundaryDates[boundary].dateHigh, dateHigh);
      
      /* Calculate divisor if necessary, using dateLow */
      if (calcDivisor)
	{
	  if ((result = SimpleConvert (1, 1, driverUnitInfo, dateLow,
				       &divisor, 1, subUnitInfo, NA)) != OK)
	    {
	      PrintError ("Can't calculate divisor for driver unit %d, sub unit %d\n\tExiting.", runControl.hdbObsDriver, runControl.hdbObsSub);
	      return (ERROR);
	    }
	}
      
      /* Round divisor to nearest integer; this will give the right
      integer result even when months and years are involved */
      boundaryDates[boundary].divisor = (int) (divisor + .5);
      
      /* Set dateLow and dateHigh for next iteration. */
      if (!strcmp (runControl.sqlObsSub, runControl.sqlObsDriver))
      {
	 SqlDateMath (ADDITION, dateHigh, dateLow, 1, SECOND);
	 SqlDateMath (ADDITION, dateLow, dateHigh, 1, runControl.sqlObsDriver);
	 SqlDateMath (SUBTRACTION, dateHigh, dateHigh, 1, SECOND);
      }
      else
      {
	 SqlDateMath (ADDITION, dateHigh, dateLow, 1, runControl.sqlObsSub);
	 SqlDateMath (ADDITION, dateLow, dateHigh, 1, runControl.sqlObsDriver);
	 SqlDateMath (SUBTRACTION, dateHigh, dateHigh, 1, runControl.sqlObsSub);
      }
    }
  
  return (OK);
}


/****************************************************************************/
#define FUNC_NAME "SetBoundaryDatesRange"
int SetBoundaryDatesRange (RUN_CONTROL runControl, 
			   BOUNDARY_DATES **boundaryDates, int *numBoundaries) 
{
  int result;
  double divisor = 0.0;
  SQL_INTERVAL sqlObsDest;
 UNIT_INFO driverUnitInfo,
            subUnitInfo;

  /* Number of sets of boundary dates is equal to numPer.
     Note that this is the number of ranges. */
  (int) *numBoundaries = runControl.numPer;
  if ((*boundaryDates = (BOUNDARY_DATES *) malloc (sizeof (BOUNDARY_DATES) *
						   *numBoundaries)) == NULL)
    {
      PrintError ("Error malloc'ing boundaryDates.\n\tExiting.");
      return (ERROR);
    }

  /* Get unit info for both observation periods. Do this so
     that if divisor must be calculated for every boundary date
     pair (one observation period > BY_DAY), the unit info retrieval 
     is done only once. Can bypass ConvertValue and jump to SimpleConvert, 
     because all time conversions are factors. */
  if ((result = SqlGetUnitInfo (runControl.hdbObsDriver, 
				&driverUnitInfo)) != OK)
    {
      PrintError ("Can't get info for unit %d\n\tExiting.", 
		  runControl.hdbObsDriver);
      return (ERROR);
    }
  
  if ((result = SqlGetUnitInfo (runControl.hdbObsSub, 
				&subUnitInfo)) != OK)
    {
      PrintError ("Can't get info for unit %d\n\tExiting.",
		  runControl.hdbObsSub);
      return (ERROR);
    }
  
  /* Now get the divisor. If both source and destination intervals are
     BY_DAY or less, divisor can be calculated once (it is constant
     and not dependent on boundary dates). Note that this divisor
     won't be used when stats are being created. */

  if (runControl.obsSub <= BY_DAY && runControl.obsDriver <= BY_DAY)
    {
      /* This is disaggregation. */
      if (!runControl.isStat)
	{
	  /* e.g., for 01Mar-15Mar --> hourly, get the number of *hours* 
	     between 01Mar and 15Mar of the first range. */
	  strcpy (sqlObsDest, 
		  timeIntervals[runControl.obsDest].ingresName);
	  SqlNumIntervalsBetweenDates (runControl.dateStart, 
				       runControl.dateEnd, sqlObsDest, 
				       &divisor);
	}
      /* This is aggregated stats (disagg and stats not allowed together); 
	 divisor doesn't matter, as number of values retrieved need not 
	 be verified. */
      else
	divisor = NA;
    }

  if ((result = CreateBoundaryDateRanges (runControl, *boundaryDates, 
					  *numBoundaries, divisor,
					  driverUnitInfo, subUnitInfo)) != OK)
    {
      return (ERROR);
    }

  return (OK);
}

/******************************************************************************/
#define FUNC_NAME "CreateBoundaryDateRanges"
int CreateBoundaryDateRanges (RUN_CONTROL runControl, 
			      BOUNDARY_DATES *boundaryDates, 
			      int numBoundaries,
			      double divisor, UNIT_INFO driverUnitInfo,
			      UNIT_INFO subUnitInfo)
{
  SQL_DATE dateLow,
           dateHigh;
  int calcDivisor = !(divisor),
      numDays,
      numSeconds,
      boundary,
      result;

  /* NOTE: Make sure this works!! 
      - if source is not range, and dest is, use beg and end dates
	as first set of source data (see below for boundary dates)
        and use numRanges to determine how many ranges are calculated.
      - here, boundary dates are same except that *high* date of first
        (and each subsequent) boundary is not f (dateEndRange)
	but  of (dateEndRange + 1obsDriver - 1obsSub)
	eg 01-jul-95 + 1month - 1day = 31-jul-95 */

  /* Initialize dateLow and dateHigh for first iteration. If you're creating
     range data, use runControl.dateEnd for dateHigh (as you want to
     grab source data up to the end of the interval. Otherwise, 
     just use dateEndRange, as you need to query based on the exact
     end-of-range date. */
  strcpy (dateLow, runControl.dateStart);
  if (runControl.sourceIsRange)
    {
      strcpy (dateHigh, runControl.dateEndRange);
    }
  else
    {
      strcpy (dateHigh, runControl.dateEnd);
    }

  /* Now create each boundary date pair, setting (and calculating, if necessary)
     the divisor for each date range. */
  for (boundary = 0; boundary < numBoundaries; boundary++)
    {
	/* Set values on current element of boundaryDates */
	strcpy (boundaryDates[boundary].dateLow, dateLow);
	strcpy (boundaryDates[boundary].dateHigh, dateHigh);

	/* Calculate divisor if necessary, using dateLow */
	if (calcDivisor)
	  {
	    /* For range values, I supply the numSeconds value; it's the total
	       number of days in the range * SECONDS_IN_DAY -- rather than 
	       letting the function calculate it based on only a single 
	       interval. */
	    SqlNumDaysBetweenDates (runControl.dateStart, runControl.dateEnd,
				    &numDays);
	    numSeconds = numDays * SECONDS_IN_DAY;

	    if ((result = SimpleConvert (1, 1, driverUnitInfo, dateLow,
					 &divisor, 1, subUnitInfo, 
					 numSeconds)) != OK)
	      {
		PrintError ("Can't calculate divisor for driver unit %d, sub unit %d\n\tExiting.", runControl.hdbObsDriver, runControl.hdbObsSub);
		return (ERROR);
	      }
	  }
	boundaryDates[boundary].divisor = (int) divisor;

	/* Boundaries are arrived at by adding one year to each date! 
	   This assumes we deal only with BY_MONTH range data. */
	SqlDateMath (ADDITION, dateLow, dateLow, 1, 
		     timeIntervals[(int)BY_YEAR].ingresName);
	SqlDateMath (ADDITION, dateHigh, dateHigh, 1, 
		     timeIntervals[(int)BY_YEAR].ingresName);
      }

  return (OK);
}

/***************************************************************************/
#define FUNC_NAME "PrintBoundaryDates"

int PrintBoundaryDates (BOUNDARY_DATES *boundaryDates, 
			int numBoundaries)

{
  int count;

  printf ("\n\n\n");
  for (count = 0; count < numBoundaries; count++)
    {
      printf ("Boundary%d: %s\t%s\t%d\n", count, boundaryDates[count].dateLow,
	      boundaryDates[count].dateHigh, boundaryDates[count].divisor);
    }
  printf ("\n\n\n");
  
  return (OK);
}

/***************************************************************************/
#define FUNC_NAME "GetEndOfInterval"

int GetEndOfInterval (SQL_DATE startDate, SQL_INTERVAL interval,
		      SQL_DATE endOfIntervalDate)

{
  int result;
  SQL_DATE tempDate;

  /* Get all dates to common starting point at beginning of interval. */
  if ((result = SqlDateTrunc (startDate, interval, tempDate)) != OK)
    {
      PrintError ("%s: Problem temporarily truncating date %s for end-of-interval calculation.\n\tExiting.", FUNC_NAME, startDate);
      return (ERROR);
    }

  /* Calculate end of interval; add a full interval, subtract 1 second. */
  if ((result = SqlDateMath (ADDITION, tempDate, endOfIntervalDate,
			     1, interval)) != OK)
    {
      PrintError ("%s: Problem computing end-of-interval for date %s.",
		  FUNC_NAME, startDate);
      return (ERROR);
    }

  if ((result = SqlDateMath (SUBTRACTION, endOfIntervalDate, 
			     endOfIntervalDate, 1, SECOND)) != OK)
    {
      PrintError ("%s: Problem computing end-of-interval for date %s.",
		  FUNC_NAME, startDate);
      return (ERROR);
    }
  
  return (OK);
}

