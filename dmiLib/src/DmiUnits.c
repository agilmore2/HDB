
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
           char  filnam[12];
};
static const struct sqlcxp sqlfpn =
{
    11,
    "DmiUnits.pc"
};


static unsigned int sqlctx = 148371;


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
   unsigned char  *sqhstv[4];
   unsigned long  sqhstl[4];
            int   sqhsts[4];
            short *sqindv[4];
            int   sqinds[4];
   unsigned long  sqharm[4];
   unsigned long  *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {12,4};

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
5,0,0,1,150,0,1028,85,0,0,3,2,0,0,128,2,3,0,0,1,3,0,0,1,3,0,0,1,97,0,0,
36,0,0,2,170,0,1028,99,0,0,2,1,0,0,128,2,97,0,0,1,3,0,0,1,97,0,0,
63,0,0,3,185,0,1028,162,0,0,3,1,0,0,128,2,97,0,0,2,3,0,0,1,3,0,0,1,97,0,0,
};


/*****************************************************************************
* DESCRIPTION
* This file contains functions which perform various Unit operations.
*
*********************************
* DmiVerifyUnits
*
* Return
*       ERROR - 
*       OK - 
*
*
*********************************
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
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


#define FUNC_NAME       "DmiVerifyUnits"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> DmiVerifyUnits

<P> <STRONG>Function signature:</STRONG> int DmiVerifyUnits(char *, int)

<P> <STRONG>Function description:</STRONG> Determines if the Riverware unit string passed in is a valid unit for the indicated site_datatype_id.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *riverware_unit_name:    Name of Riverware units.
<BR><DD> int hdb_site_datatype_id: HDB site_datatype_id.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int DmiVerifyUnits(char * riverware_unit_name,
                   int hdb_site_datatype_id)
{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    int i;
    int  unit_id_value;
    ID   sqlUnitId,
         sqlSDI;
    char proper_units_string[UNIT_NAME_LEN + 1];
  /* EXEC SQL END DECLARE SECTION; */ 


    if (!riverware_unit_name) {
        PrintError("DMI: No units passed to DmiVerifyUnits.\n");
        return(ERROR);
    }

    if (hdb_site_datatype_id <= 0) {
        PrintError("DMI: Invalid site_datatype_id (%d)passed to DmiVerifyUnits.\n", hdb_site_datatype_id);
        return(ERROR);
    }

    i = 0;
    while (unit_lookup_array[i].hdb_unit_id != 0) {
        if (strcmp(riverware_unit_name, unit_lookup_array[i].riverware_unit_name)
            == 0) {
                break;
        }
        i++;
    }

    if (unit_lookup_array[i].hdb_unit_id != 0) {
       
       /* Set values on host variables to be used in SELECT statement. */
       sqlSDI = hdb_site_datatype_id;
       sqlUnitId = unit_lookup_array[i].hdb_unit_id;
        
       /* EXEC SQL AT :current_dbsite SELECT b.unit_id 
	  INTO :unit_id_value
	  FROM hdb_site_datatype a, hdb_datatype b 
	  WHERE a.site_datatype_id = :sqlSDI
	  AND a.datatype_id = b.datatype_id 
	  AND b.unit_id = :sqlUnitId; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 4;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "select b.unit_id into :b1  from hdb_site_datatype a ,h\
db_datatype b where ((a.site_datatype_id=:b2 and a.datatype_id=b.datatype_id) \
and b.unit_id=:b3)";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )5;
       sqlstm.selerr = (unsigned short)1;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)256;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)&unit_id_value;
       sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (unsigned char  *)&sqlSDI;
       sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[1] = (         int  )0;
       sqlstm.sqindv[1] = (         short *)0;
       sqlstm.sqinds[1] = (         int  )0;
       sqlstm.sqharm[1] = (unsigned long )0;
       sqlstm.sqadto[1] = (unsigned short )0;
       sqlstm.sqtdso[1] = (unsigned short )0;
       sqlstm.sqhstv[2] = (unsigned char  *)&sqlUnitId;
       sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
       sqlstm.sqhsts[2] = (         int  )0;
       sqlstm.sqindv[2] = (         short *)0;
       sqlstm.sqinds[2] = (         int  )0;
       sqlstm.sqharm[2] = (unsigned long )0;
       sqlstm.sqadto[2] = (unsigned short )0;
       sqlstm.sqtdso[2] = (unsigned short )0;
       sqlstm.sqhstv[3] = (unsigned char  *)current_dbsite;
       sqlstm.sqhstl[3] = (unsigned long )10;
       sqlstm.sqhsts[3] = (         int  )0;
       sqlstm.sqindv[3] = (         short *)0;
       sqlstm.sqinds[3] = (         int  )0;
       sqlstm.sqharm[3] = (unsigned long )0;
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


	  
       if (SqlErrorHandler (FUNC_NAME, TRUE,
			    "Problem Selecting the unit_id from hdb_unit.\n") != OK) {
	  return(ERROR);
       }
       
       if (sqlca.sqlcode > 0) { /* Data is not stored in HDB with these units */

            /* EXEC SQL AT :current_dbsite SELECT c.unit_name 
	       INTO :proper_units_string
	       FROM hdb_site_datatype a, hdb_datatype b, hdb_unit c 
	       WHERE a.site_datatype_id = :sqlSDI
	       AND a.datatype_id = b.datatype_id 
	       AND b.unit_id = c.unit_id; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 12;
            sqlstm.arrsiz = 4;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.stmt = "select c.unit_name into :b1  from hdb_site_dataty\
pe a ,hdb_datatype b ,hdb_unit c where ((a.site_datatype_id=:b2 and a.datatype\
_id=b.datatype_id) and b.unit_id=c.unit_id)";
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )36;
            sqlstm.selerr = (unsigned short)1;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqhstv[0] = (unsigned char  *)proper_units_string;
            sqlstm.sqhstl[0] = (unsigned long )33;
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         short *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned long )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (unsigned char  *)&sqlSDI;
            sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
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
}



            if (SqlErrorHandler (FUNC_NAME, FALSE,
				 "Problem Selecting proper units from hdb_unit.\n") != OK) {
               return(ERROR);
            }
	    
            PrintError("DMI: RiverWare units '%s' not appropriate for site_datatype_id %d.\nThe appropriate units are '%s'.",
		       riverware_unit_name,
		       hdb_site_datatype_id,
		       proper_units_string);

            return(ERROR);
       }
    }
    else { /* no mapping from RiverWare units to a unit_id in unit_lookup_array */
       PrintError("DMI: RiverWare Units '%s' Not represented in HDB.\n",
		  riverware_unit_name);
       return(ERROR);
    }
    
    return (OK);
}

#define FUNC_NAME       "DmiLookupRiverWareUnitsForSDI"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> DmiLookupRiverWareUnitsForSDI

<P> <STRONG>Function signature:</STRONG> int DmiLookupRiverWareUnitsForSDI (int, char **)

<P> <STRONG>Function description:</STRONG> For the indicated SDI, finds the appropriate Riverware units string.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> int site_datatype_id: HDB site_datatype_id.
<BR><DD> char ** pr_units:     Riverware units string for this SDI; output.    
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int DmiLookupRiverWareUnitsForSDI(int site_datatype_id,
                              char ** pr_units)
{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    int i;
    int  unit_id_value;
    char units_string[UNIT_NAME_LEN + 1];
    ID sqlSDI = site_datatype_id;
  /* EXEC SQL END DECLARE SECTION; */ 


/*
 * Note: pr_units is not initialized -- allocate here.
 */

    /* EXEC SQL AT :current_dbsite SELECT c.unit_name, c.unit_id 
       INTO :units_string, :unit_id_value
       FROM hdb_site_datatype a, hdb_datatype b, hdb_unit c 
       WHERE a.site_datatype_id = :sqlSDI
       AND a.datatype_id = b.datatype_id 
       AND b.unit_id = c.unit_id; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 4;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "select c.unit_name ,c.unit_id into :b1,:b2  from hdb_site\
_datatype a ,hdb_datatype b ,hdb_unit c where ((a.site_datatype_id=:b3 and a.d\
atatype_id=b.datatype_id) and b.unit_id=c.unit_id)";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )63;
    sqlstm.selerr = (unsigned short)1;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)units_string;
    sqlstm.sqhstl[0] = (unsigned long )33;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&unit_id_value;
    sqlstm.sqhstl[1] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (unsigned char  *)&sqlSDI;
    sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (unsigned char  *)current_dbsite;
    sqlstm.sqhstl[3] = (unsigned long )10;
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         short *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned long )0;
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



    if (SqlErrorHandler (FUNC_NAME, FALSE,
       "Problem Selecting the unit_id from hdb_unit.\n") != OK) {
       return(ERROR);
    }

    /*
    ** Find the RiverWare units for the unit_id.
    */
    i = 0;
    while (unit_lookup_array[i].hdb_unit_id != 0) {
        if (unit_lookup_array[i].hdb_unit_id == unit_id_value) {
                break;
        }
        i++;
    }

    if (unit_lookup_array[i].hdb_unit_id != 0) {
        /*
        ** The search was successful. Malloc space for the string.
        */
        PrintError("Units: %s are default for site_datatype_id %d\n",
                    unit_lookup_array[i].riverware_unit_name, 
                    site_datatype_id);
        *pr_units = (char *) malloc(strlen(unit_lookup_array[i].riverware_unit_name) + 1);
        strcpy(*pr_units, unit_lookup_array[i].riverware_unit_name);
        return(OK);
    }
    else {
        PrintError("Could not find mapping for site_datatype_id: %d (%s) in DMI\n",
                    site_datatype_id,
                    units_string);
        return(ERROR);
    }
}

#define FUNC_NAME       "DmiLookupRiverWareUnitScale"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> DmiLookupRiverWareUnitsScale

<P> <STRONG>Function signature:</STRONG> int DmiLookupRiverWareUnitsScale (int, char *, int *)

<P> <STRONG>Function description:</STRONG> Given an HDB unit_id, determines the corresponding units and scale for Riverware.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> int unit_id:     HDB unit_id.
<BR><DD> char * pr_units: Riverware units string for this HDB unit_id; output.
<BR><DD> int * pr_units:  Scale; output.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int DmiLookupRiverWareUnitScale (int hdb_unit_id,
                             char *pr_units,
                             int *pr_scale)
{
  int i = 0;

  while (unit_lookup_array[i].hdb_unit_id != 0)
    {
      if (unit_lookup_array[i].hdb_unit_id == hdb_unit_id)
        break;

      i++;
    }

  if (unit_lookup_array[i].hdb_unit_id != 0)
    {
      strcpy (pr_units, unit_lookup_array[i].riverware_unit_name);
      *pr_scale = unit_lookup_array[i].scale;
    }
  else
    {
      pr_units[0] = '\0';
      *pr_scale = NA;
    }

  return (OK);
}
