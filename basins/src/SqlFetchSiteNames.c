
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
    "SqlFetchSiteNames.pc"
};


static unsigned long sqlctx = 84103779;


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

 static const char *sq0001 = 
"select primary.site_id ,primary.site_name ,LENGTH(primary.site_name) ,second\
ary.site_id  from hdb_site primary ,hdb_site secondary where (((primary.parent\
_objecttype_id=secondary.parent_objecttype_id and primary.parent_site_id=secon\
dary.parent_site_id) and primary.objecttype_id=:b0) and secondary.objecttype_i\
d=:b1)           ";

 static const char *sq0003 = 
"select site_id ,site_name ,LENGTH(site_name) ,(-1)  from hdb_site where obje\
cttype_id=:b0           ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4130,0,0,0,
5,0,0,1,327,0,9,85,0,0,2,2,0,1,0,1,3,0,0,1,3,0,0,
28,0,0,1,0,0,13,94,0,0,4,0,0,1,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,
59,0,0,1,0,0,15,114,0,0,0,0,0,1,0,
74,0,0,2,0,0,29,115,0,0,0,0,0,1,0,
89,0,0,3,100,0,9,154,0,0,1,1,0,1,0,1,3,0,0,
108,0,0,3,0,0,13,163,0,0,4,0,0,1,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,
139,0,0,3,0,0,15,183,0,0,0,0,0,1,0,
154,0,0,4,0,0,29,184,0,0,0,0,0,1,0,
169,0,0,5,67,0,4,243,0,0,2,1,0,1,0,2,3,0,0,1,5,0,0,
192,0,0,6,119,0,4,253,0,0,2,1,0,1,0,2,3,0,0,1,5,0,0,
215,0,0,7,107,0,4,263,0,0,2,1,0,1,0,2,3,0,0,1,5,0,0,
238,0,0,8,127,0,4,273,0,0,2,1,0,1,0,2,3,0,0,1,5,0,0,
};


/*
* DESCRIPTION
* These functions retrieve the site ID and site name for every site which is
* of the object type indicated.  These values are retrieved from hdb_site and
* inserted into the names array.
*
* The function SqlFetchSiteNamesPlusAltId, in addition, retrieves,, for every
* site, the site_id of another object type (altObjectTypeId) which is in the
* same geographic location as the original site.  For instance, by passing
* in objecttypeId for snotel sites, and altObjecttypeId for climate sites,
* the names array will be filled with all snotel site IDs and names, as well
* as all climate site IDs which correspond to a snotel site ID.
*
*
* INPUT
* ID	      	object type (primary site)
* NAME_ID *	array of names, IDs, etc to be filled in
* int *		number of sites found
* ID		object type of alternate site identifiers; in
*		  SqlFetchSiteNamesPlusAltId only
*
* OUTPUT
* warnings and error messages
*
* RETURN
* OK or ERROR
*
*
* COMMENTS
*
* AUTHOR - EDITORS  
* Carol Marra
* Rick Casey
* 
* Notes
* 8-April-2003 (rc) Added function to return the loading application id.
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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


#define FUNC_NAME "SqlFetchSiteNames"

int SqlFetchSiteNamesPlusAltId (ID objecttypeId, NAME_ID *names, int *numSites,
				ID altObjecttypeId)

{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    ID siteId;
    SITE_NAME siteName;
    ID altId;
    ID sqlObjecttypeId = objecttypeId,
       sqlAltObjecttypeId = altObjecttypeId;
    int nameLength;
  /* EXEC SQL END DECLARE SECTION; */ 


  int       siteCount,
	    result;

  /* EXEC SQL WHENEVER NOT FOUND GOTO close_c1; */ 

  /* EXEC SQL DECLARE c1 CURSOR FOR 
    SELECT primary.site_id, primary.site_name, 
	LENGTH (primary.site_name), secondary.site_id
    FROM hdb_site primary, hdb_site secondary
    WHERE primary.parent_objecttype_id = secondary.parent_objecttype_id
      AND primary.parent_site_id = secondary.parent_site_id
      AND primary.objecttype_id = :sqlObjecttypeId
      AND secondary.objecttype_id = :sqlAltObjecttypeId; */ 


  if ((result = SqlErrorHandler 
       (FUNC_NAME, FALSE, "Problem declaring cursor c1.")) != OK)
    {
      return (ERROR);
    }

  /* EXEC SQL OPEN c1; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 2;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = sq0001;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&sqlObjecttypeId;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&sqlAltObjecttypeId;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
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


  if ((result = SqlErrorHandler 
       (FUNC_NAME, FALSE, "Problem opening cursor c1.")) != OK)
    {
      return (ERROR);
    }

  for (siteCount = 0; ; siteCount++) 
    {
      /* EXEC SQL FETCH c1 INTO :siteId, :siteName, :nameLength, :altId; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )28;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&siteId;
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)siteName;
      sqlstm.sqhstl[1] = (unsigned int  )65;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)&nameLength;
      sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)&altId;
      sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
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
      if (sqlca.sqlcode == 1403) goto close_c1;
}


      if ((result = SqlErrorHandler 
	   (FUNC_NAME, FALSE, "Problem opening cursor c1.")) != OK)
	{
	  return (ERROR);
	}

      /* Assign selected values to names[siteCount]. */
      names[siteCount].siteId = siteId;
      strncpy (names[siteCount].siteName, siteName, nameLength);
      names[siteCount].altId = altId;
      
#ifdef DEBUG	
      fprintf(stdout, "ID: %d  NAME: %s  ALT ID: %d\n", 
	      names[siteCount].siteId, names[siteCount].siteName,
	      names[siteCount].altId);
#endif

    }

 close_c1:  /* EXEC SQL CLOSE c1; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 10;
            sqlstm.arrsiz = 4;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )59;
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
            sqlstm.arrsiz = 4;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )74;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)256;
            sqlstm.occurs = (unsigned int  )0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



#ifdef DEBUG	
  fprintf (stdout, "Number of sites: %d\n", siteCount);
#endif

  /* return */
  *numSites = siteCount;
  return (OK);
}

/***************************************************************************/

int SqlFetchSiteNames (ID objecttypeId, NAME_ID *names, int *numSites)

{
  /* EXEC SQL BEGIN DECLARE SECTION; */ 

    ID siteId;
    SITE_NAME siteName;
    ID altId;
    ID sqlObjecttypeId = objecttypeId;
    int nameLength;
  /* EXEC SQL END DECLARE SECTION; */ 


  int       siteCount,
	    result;

  /* EXEC SQL WHENEVER NOT FOUND GOTO close_c2; */ 

  /* EXEC SQL DECLARE c2 CURSOR FOR 
    SELECT site_id, site_name, LENGTH (site_name), -1
    FROM hdb_site
    WHERE objecttype_id = :sqlObjecttypeId; */ 


  if ((result = SqlErrorHandler 
       (FUNC_NAME, FALSE, "Problem declaring cursor c2.")) != OK)
    {
      return (ERROR);
    }

  /* EXEC SQL OPEN c2; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = sq0003;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )89;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&sqlObjecttypeId;
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


  if ((result = SqlErrorHandler 
       (FUNC_NAME, FALSE, "Problem opening cursor c2.")) != OK)
    {
      return (ERROR);
    }

  for (siteCount = 0; ; siteCount++) 
    {
      /* EXEC SQL FETCH c2 INTO :siteId, :siteName, :nameLength, :altId; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )108;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)&siteId;
      sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)siteName;
      sqlstm.sqhstl[1] = (unsigned int  )65;
      sqlstm.sqhsts[1] = (         int  )0;
      sqlstm.sqindv[1] = (         void  *)0;
      sqlstm.sqinds[1] = (         int  )0;
      sqlstm.sqharm[1] = (unsigned int  )0;
      sqlstm.sqadto[1] = (unsigned short )0;
      sqlstm.sqtdso[1] = (unsigned short )0;
      sqlstm.sqhstv[2] = (         void  *)&nameLength;
      sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
      sqlstm.sqhsts[2] = (         int  )0;
      sqlstm.sqindv[2] = (         void  *)0;
      sqlstm.sqinds[2] = (         int  )0;
      sqlstm.sqharm[2] = (unsigned int  )0;
      sqlstm.sqadto[2] = (unsigned short )0;
      sqlstm.sqtdso[2] = (unsigned short )0;
      sqlstm.sqhstv[3] = (         void  *)&altId;
      sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
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
      if (sqlca.sqlcode == 1403) goto close_c2;
}


      if ((result = SqlErrorHandler 
	   (FUNC_NAME, FALSE, "Problem opening cursor c2.")) != OK)
	{
	  return (ERROR);
	}
      
      /* Assign selected values to names[siteCount]. */
      names[siteCount].siteId = siteId;
      strncpy (names[siteCount].siteName, siteName, nameLength);
      names[siteCount].altId = altId;

#ifdef DEBUG	
      fprintf(stdout, "ID: %d  NAME: %s  ALT ID: %d\n", 
	      names[siteCount].siteId, names[siteCount].siteName,
	      names[siteCount].altId);
#endif

    }

close_c2: /* EXEC SQL close c2; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 10;
          sqlstm.arrsiz = 4;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )139;
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
          sqlstm.arrsiz = 4;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )154;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)256;
          sqlstm.occurs = (unsigned int  )0;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



#ifdef DEBUG	
  fprintf (stdout, "Number of sites: %d\n", siteCount);
#endif

  /* return */
  *numSites = siteCount;
  return (OK);
}


/****************************************************************************
 *
 * SqlFetchRbasekeys
 * Author: Rick Casey
 * 8-April-2003
 * 
 * Input parameters (all pointers):
 *  loading_application_id
 *  agen_id
 *  collection_system_id
 *  computation_id
 * Output:
 *  populates the function arguements
 *	returns 0 or 1 (NO_ERROR or ERROR)
 * 
 * Description:
 * Looks up foreign keys required for r_base inserts based on #define's in
 * basinTypedefs.h.
 * 
 * (note: at this point in time, the function that looks up the validation 
 * value is hardcoded to return 'Z'. A future validation function would be plugged 
 * in there. (see SqlGetValidationFlag in $HDB_ENV/hdbLib/src/SqlValidation.pc).
 *
 ****************************************************************************/
int SqlFetchRbasekeys (int * p_loading_application_id, int * p_agen_id, int * p_collection_system_id, int * p_computation_id)
{
	int		result;

	/* EXEC SQL BEGIN DECLARE SECTION; */ 

	  int           agen_id,
	  				collection_system_id, 
					computation_id, 
					loading_application_id; 
	  char          agen_name[64],
					collection_system_name[64],
					computation_name[64],
					loading_application_name[64];
	/* EXEC SQL END DECLARE SECTION; */ 


	/* EXEC SQL WHENEVER NOT FOUND go to close_FetchRbaseKeys; */ 


	/* Assign #define's to host variables. */
	strcpy(agen_name,AGEN_NAME);
	strcpy(collection_system_name,COLLECTION_SYSTEM_NAME);
	strcpy(computation_name,COMPUTATION_NAME);
	strcpy(loading_application_name,LOADING_APPLICATION_NAME);

	/* EXEC SQL SELECT agen_id INTO :agen_id
		FROM hdb_agen 
		WHERE hdb_agen.agen_name = :agen_name; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select agen_id into :b0  from hdb_agen where hdb_agen.agen_n\
ame=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )169;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&agen_id;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)agen_name;
 sqlstm.sqhstl[1] = (unsigned int  )64;
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
 if (sqlca.sqlcode == 1403) goto close_FetchRbaseKeys;
}



	if ( (result = SqlErrorHandler 
		(FUNC_NAME, FALSE, "Problem selecting foreign keys from hdb_agen for r_base.") ) != OK)
    {
		return (ERROR);
    }

	/* EXEC SQL SELECT collection_system_id INTO :collection_system_id 
		FROM hdb_collection_system 
		WHERE hdb_collection_system.collection_system_name = :collection_system_name; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select collection_system_id into :b0  from hdb_collection_sy\
stem where hdb_collection_system.collection_system_name=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )192;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&collection_system_id;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)collection_system_name;
 sqlstm.sqhstl[1] = (unsigned int  )64;
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
 if (sqlca.sqlcode == 1403) goto close_FetchRbaseKeys;
}



	if ( (result = SqlErrorHandler 
		(FUNC_NAME, FALSE, "Problem selecting foreign keys from hdb_collection_system for r_base.") ) != OK)
    {
		return (ERROR);
    }

	/* EXEC SQL SELECT computation_id INTO :computation_id
		FROM hdb_computed_datatype
		WHERE hdb_computed_datatype.computation_name = :computation_name; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select computation_id into :b0  from hdb_computed_datatype w\
here hdb_computed_datatype.computation_name=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )215;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&computation_id;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)computation_name;
 sqlstm.sqhstl[1] = (unsigned int  )64;
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
 if (sqlca.sqlcode == 1403) goto close_FetchRbaseKeys;
}



	if ( (result = SqlErrorHandler 
		(FUNC_NAME, FALSE, "Problem selecting foreign keys from hdb_computated_datatype for r_base.") ) != OK)
    {
		return (ERROR);
    }

	/* EXEC SQL SELECT loading_application_id INTO :loading_application_id
		FROM hdb_loading_application
		WHERE hdb_loading_application.loading_application_name = :loading_application_name; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select loading_application_id into :b0  from hdb_loading_app\
lication where hdb_loading_application.loading_application_name=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )238;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&loading_application_id;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)loading_application_name;
 sqlstm.sqhstl[1] = (unsigned int  )64;
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
 if (sqlca.sqlcode == 1403) goto close_FetchRbaseKeys;
}



	if ( (result = SqlErrorHandler 
		(FUNC_NAME, FALSE, "Problem selecting foreign keys from hdb_loading_application for r_base.") ) != OK)
    {
		return (ERROR);
    }

	/* Assign results to parameters by reference. */
	*p_loading_application_id = loading_application_id;
	*p_agen_id = agen_id;
	*p_collection_system_id = collection_system_id;
	*p_computation_id = computation_id;

	/* debugging statements...
	printf("Return values from HDB calls:\n");
	printf("loading_application_id = %d\n",loading_application_id);
	printf("agen_id = %d\n",agen_id);
	printf("collection_system_id = %d\n",collection_system_id);
	printf("computation_id = %d\n",computation_id);
	*/


	close_FetchRbaseKeys: 
	if ( (result = SqlErrorHandler 
		(FUNC_NAME, FALSE, "Problem selecting foreign keys for r_base.") ) != OK)
    {
		return (ERROR);
    }
	return (OK);
}
