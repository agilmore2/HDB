
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
           char  filnam[14];
};
static const struct sqlcxp sqlfpn =
{
    13,
    "SqlConnect.pc"
};


static unsigned long sqlctx = 657547;


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
            void  *sqhstv[5];
   unsigned int   sqhstl[5];
            int   sqhsts[5];
            void  *sqindv[5];
            int   sqinds[5];
   unsigned int   sqharm[5];
   unsigned int   *sqharc[5];
   unsigned short  sqadto[5];
   unsigned short  sqtdso[5];
} sqlstm = {10,5};

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
5,0,0,1,0,0,1053,42,0,0,0,0,0,0,128,1,97,0,0,
24,0,0,2,0,0,27,98,0,0,4,4,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,10,0,0,
55,0,0,3,0,0,29,110,0,0,0,0,0,1,0,
70,0,0,4,0,0,1051,163,0,0,4,4,0,0,128,1,97,0,0,1,97,0,0,1,97,0,0,1,10,0,0,1,97,
0,0,
105,0,0,5,0,0,1053,178,0,0,0,0,0,0,128,1,97,0,0,
124,0,0,6,0,0,24,283,0,0,1,1,0,1,0,1,97,0,0,
143,0,0,7,0,0,1048,292,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
166,0,0,8,0,0,1048,405,0,0,1,1,0,0,128,1,97,0,0,1,97,0,0,
189,0,0,9,0,0,1053,417,0,0,0,0,0,0,128,1,97,0,0,
208,0,0,10,111,0,1028,460,0,0,3,2,0,0,128,2,3,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
239,0,0,11,0,0,1053,488,0,0,0,0,0,0,128,1,97,0,0,
258,0,0,12,67,0,1028,524,0,0,2,1,0,0,128,2,97,0,0,1,97,0,0,1,97,0,0,
285,0,0,13,0,0,1053,544,0,0,0,0,0,0,128,1,97,0,0,
304,0,0,14,0,0,30,614,0,0,0,0,0,1,0,
319,0,0,15,0,0,1054,625,0,0,0,0,0,0,128,1,97,0,0,
};



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <defines.h>
#include <typedefs.h>
#include <dbutils.h>
#include <utils.h>

/* EXEC SQL INCLUDE SQLCA;
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



#define MAX_CONNECTIONS 25
#define DB "db1"

/* Set the global vars to maintain connection info: num connections,
   current connections, list of connections */
int db_connected= 0;
DATABASE_ALIAS dbconnected_array[MAX_CONNECTIONS];

#define FUNC_NAME "SqlCommit"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlCommit

<P> <STRONG>Function signature:</STRONG> int SqlCommit ()

<P> <STRONG>Function description:</STRONG> Commits the open transaction to the database.

<P><DL> <STRONG>Function arguments:</STRONG>
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlCommit( )
{
   int result;
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


   if ((result = SqlErrorHandler
	(FUNC_NAME, FALSE,
	 "Error in SQL Commit \n")) == ERROR)
   {
      return (ERROR);
   }
   
   else
   {
      return (OK);
   }
}

/****************************************************************************/
#define FUNC_NAME    "SqlConnect"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlConnect

<P> <STRONG>Function signature:</STRONG> int SqlConnect (char *, char *, char *)

<P> <STRONG>Function description:</STRONG> Connects twice to specified database as specified user. One connection is named, the other is not.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *dbName:  Name (SID) of the database to which to connect.
<BR><DD> char *usrname: Name of Oracle user connecting.
<BR><DD> char *id:      Password of Oracle user.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlConnect(char *dbName, char* usrname, char* id)
{
   /*Database host variables*/
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char *usr;
   char *uid; 
   char *dbstring;
   /* EXEC SQL END DECLARE SECTION; */ 

   
   int   result;
   
   usr = usrname;
   uid = id;

   /* set the name of the database, and the logical name for the 2nd 
      connection */
   dbstring = dbName;

   /* connect to the specified database, first with an unnamed (default)
      connection, then with a named connection */
   /* EXEC SQL CONNECT :usr IDENTIFIED BY :uid USING :dbstring; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )10;
   sqlstm.offset = (unsigned int  )24;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)usr;
   sqlstm.sqhstl[0] = (unsigned int  )0;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)uid;
   sqlstm.sqhstl[1] = (unsigned int  )0;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)dbstring;
   sqlstm.sqhstl[2] = (unsigned int  )0;
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


   result = SqlErrorHandler(FUNC_NAME, FALSE,
	 "Error connecting to %s.\n\t as %s %s", dbstring, usr, uid);
   if (result != OK)
     return (ERROR);

   db_connected++; 
   if ((result = AddConnection (NULL)) == ERROR)
   {
      printf ("Problem adding default connection to dbsite_array. Exiting.\n");
      return (ERROR);
   }
   /* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )55;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   /* Make the named connection. */
   if ((result = SqlConnectAt (DB, dbstring, usrname, id)) == ERROR)
     return (ERROR);

   return (OK);
}

/****************************************************************************/
#define FUNC_NAME "SqlConnectAt"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlConnectAt

<P> <STRONG>Function signature:</STRONG> int SqlConnectAt (char *, char *, char *, char *)

<P> <STRONG>Function description:</STRONG> Connects to specified database as specified user, using the AT syntax. Sets the global variable current_dbsite and adds the connection name to the global list of connections.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *dbSiteAlias:  Logical name of connection (db_site_code)
<BR><DD> char *dbName:       Name (SID) of the database to which to connect.
<BR><DD> char *usrname:      Name of Oracle user connecting.
<BR><DD> char *id:           Password of Oracle user.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlConnectAt(char *dbSiteAlias, char *dbName, char* usrname, char* id)
{
   /*Database host variables*/
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char *usr;
   char *uid; 
   char *dbstring;
   char *dbSite;
   /* EXEC SQL END DECLARE SECTION; */ 

   
   int   result;
   
   usr = usrname;
   uid = id;

   /* set the name of the database and its logical name*/
   dbstring = dbName;
   dbSite = dbSiteAlias;

   /* connect to the specified database with a named connection,
      and set the global pointer to the current database. */
   /* EXEC SQL CONNECT :usr IDENTIFIED BY :uid AT :dbSite USING :dbstring; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )10;
   sqlstm.offset = (unsigned int  )70;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)usr;
   sqlstm.sqhstl[0] = (unsigned int  )0;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)uid;
   sqlstm.sqhstl[1] = (unsigned int  )0;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)dbstring;
   sqlstm.sqhstl[2] = (unsigned int  )0;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         void  *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned int  )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[4] = (         void  *)dbSite;
   sqlstm.sqhstl[4] = (unsigned int  )0;
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         void  *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned int  )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
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


   result = SqlErrorHandler(FUNC_NAME, FALSE,
	 "Error connecting AT %s to %s.\n\t as %s %s", dbSite, dbstring, usr, 
			    uid);
   if (result != OK)
     return (ERROR);

   db_connected++; 
   if ((result = AddConnection (dbSite)) == ERROR)
   {
      printf ("Problem adding connection %s to dbsite_array. Exiting.\n",
	      dbSite);
      return (ERROR);
   }

   /* EXEC SQL AT :dbSite COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )105;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)dbSite;
   sqlstm.sqhstl[0] = (unsigned int  )0;
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


   strcpy (current_dbsite, dbSite);
  
   return (OK);

}

/****************************************************************************/
#define FUNC_NAME    "SqlConnectRole"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlConnectRole

<P> <STRONG>Function signature:</STRONG> int SqlConnectRole (char *, char *, char *, char *)

<P> <STRONG>Function description:</STRONG> Connects to the specified database twice as the specified user, once using a named connection and once an unnamed one. Enables the indicated role on both connections.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *dbName:  Name (SID) of the database to which to connect.
<BR><DD> char *usrname: Name of Oracle user connecting.
<BR><DD> char *id:      Password of Oracle user.
<BR><DD> char *role:    Role to enable.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlConnectRole(char *dbName, char* userName, char* id, char *roleName)
{
   /*Database host variables*/
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char dbSite[3];
   char *user;
   char *uid;
   char *dbstring;
   char *psswdUser;
   char rolePsswd[24];
   char setRoleStatement[100];
   /* EXEC SQL END DECLARE SECTION; */ 

   
   /*integer for checking the result of a function call*/
   int   result;

   user = userName;
   uid = id;

   /* set the name of the database and get the password user*/
   dbstring = dbName;
   strcpy (dbSite, DB);

   psswdUser = NULL;
   psswdUser = getenv("PSSWD_USER");

   if (psswdUser == NULL)
   {
      PrintError("Environment variable PSSWD_USER not set...\n");
      return(ERROR);
   }

   /* Connect to the local database as the psswd_user, to get the roleName's
      if the user (userName) has been granted that role. 
      Use SqlConnectAt so that global current_dbsite will be set
      and the two functions called can refer to the global. */
   
   if ((result = SqlConnectAt (DB, dbstring, psswdUser, dbstring)) != OK)
   {
      return (ERROR);
   }
   else
   {
      /* First, verify that specified user has been granted the needed
	 application role. */
      
      if ((result = SqlCheckGrantedRole (userName, roleName)) != OK)
      {
	 return (ERROR);
      }

      if ((result = SqlGetRolePsswd (roleName, rolePsswd)) != OK)
      {
	 return (ERROR);
      }

      SqlDisconnectAt(DB);

   }
   /* Now connect as the user for the remainder of the program,
      enabling needed roles.
      Use SqlConnect to establish both the named and unnamed connection. */
   if ((result = SqlConnect (dbstring, user, uid)) != OK)
   {
      return (ERROR);
   }
   
   else
   {
      /* Enable needed role, plus CONNECT, for both connections */
      /* For unnamed connection. */
      /* EXEC SQL WHENEVER NOT FOUND continue; */ 

      sprintf (setRoleStatement, "set role %s identified by %s, CONNECT",
	       roleName, rolePsswd);
      
      /* EXEC SQL EXECUTE IMMEDIATE :setRoleStatement; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )124;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)setRoleStatement;
      sqlstm.sqhstl[0] = (unsigned int  )100;
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
	   (FUNC_NAME, FALSE,
	    "Error setting role to: %s on default connection.\n\tExiting.", roleName)) == ERROR)
      {
	 SqlDisconnect();
	 return (ERROR);
      }

      /* EXEC SQL AT :dbSite EXECUTE IMMEDIATE :setRoleStatement; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )143;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)setRoleStatement;
      sqlstm.sqhstl[0] = (unsigned int  )100;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)dbSite;
      sqlstm.sqhstl[1] = (unsigned int  )3;
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
	   (FUNC_NAME, FALSE,
	    "Error setting role to: %s AT %s.\n\tExiting.", roleName, 
	    dbSite)) == ERROR)
      {
	 SqlDisconnect();
	 return (ERROR);
      }
   }
   return (OK);
}


/****************************************************************************/
#define FUNC_NAME    "SqlConnectRoleAt"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlConnectRoleAt

<P> <STRONG>Function signature:</STRONG> int SqlConnectRoleAt (char *, char *, char *, char *, char *)

<P> <STRONG>Function description:</STRONG> Connects to the specified database as the specified user, using the AT syntax. Enables the indicated role.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *dbSiteAlias:  Logical name of connection (db_site_code)
<BR><DD> char *dbName:       Name (SID) of the database to which to connect.
<BR><DD> char *usrname:      Name of Oracle user connecting.
<BR><DD> char *id:           Password of Oracle user.
<BR><DD> char *role:         Role to enable.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlConnectRoleAt(char *dbSiteAlias, char *dbName, char* userName, 
		     char* id, char *roleName)
{
   /*Database host variables*/
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char *user;
   char *uid;
   char *dbstring;
   char *dbSite;
   char *psswdUser;
   char rolePsswd[24];
   char setRoleStatement[100];
   /* EXEC SQL END DECLARE SECTION; */ 

   
   /*integer for checking the result of a function call*/
   int   result;

   user = userName;
   uid = id;;

   /* set the name of the database and its logical name,
      and get the password user*/
   dbstring = dbName;
   dbSite = dbSiteAlias;

   psswdUser = NULL;
   psswdUser = getenv("PSSWD_USER");

   if (psswdUser == NULL)
   {
      PrintError("Environment variable PSSWD_USER not set...\n");
      return(ERROR);
   }

   /* Connect to the local database as the psswd_user, to get the roleName's
      if the user (userName) has been granted that role. 
      Use SqlConnectAt so that global current_dbsite will be set
      and the two functions called can refer to the global. */

   if ((result = SqlConnectAt (dbSite, dbstring, psswdUser, dbstring)) != OK)
   {
      return (ERROR);
   }
   else
   {
      /* First, verify that specified user has been granted the needed
	 application role. */

      if ((result = SqlCheckGrantedRole (userName, roleName)) != OK)
      {
	 return (ERROR);
      }

      if ((result = SqlGetRolePsswd (roleName, rolePsswd)) != OK)
      {
	 return (ERROR);
      }

      SqlDisconnectAt(dbSite);
   }
    
   /* Now connect as the user for the remainder of the program,
      enabling needed roles. */
   if ((result = SqlConnectAt (dbSite, dbstring, userName, uid)) != OK)
   {
      return (ERROR);
   }
   
   else
   {
      /* Enable needed role, plus CONNECT role */
      /* EXEC SQL WHENEVER NOT FOUND continue; */ 

      sprintf (setRoleStatement, "set role %s identified by %s, CONNECT",
	       roleName, rolePsswd);
      
      /* EXEC SQL AT :dbSite EXECUTE IMMEDIATE :setRoleStatement; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 10;
      sqlstm.arrsiz = 5;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )166;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)256;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (         void  *)setRoleStatement;
      sqlstm.sqhstl[0] = (unsigned int  )100;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqhstv[1] = (         void  *)dbSite;
      sqlstm.sqhstl[1] = (unsigned int  )0;
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
	   (FUNC_NAME, FALSE,
	    "Error setting role to: %s AT %s.\n\tExiting.", roleName, 
	    dbSite)) == ERROR)
      {
	 SqlDisconnectAt(dbSite);
	 return (ERROR);
      }
   }
	
   /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )189;
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


   return (OK);
}


/*****************************************************************************/
#define FUNC_NAME    "SqlCheckGrantedRole"

/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlCheckGrantedRole

<P> <STRONG>Function signature:</STRONG> int SqlCheckGrantedRole (char *, char *)

<P> <STRONG>Function description:</STRONG> Called by SqlConnectRole to verify that the specified user has been granted the needed role.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *userName: Oracle user needing the role.
<BR><DD> char *roleName: The role needed.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlCheckGrantedRole (char* userName, char *roleName)
{
   /*Database host variables*/
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char user[20];
   char role[20];
   int  result = 0;   
   /* EXEC SQL END DECLARE SECTION; */ 


   char *message;

   strcpy(user, userName);
   strcpy(role, roleName);

   /* EXEC SQL WHENEVER SQLWARNING GOTO check_result; */ 


   /* EXEC SQL AT :current_dbsite SELECT count(granted_role)
      INTO :result
   FROM dba_role_privs
   WHERE grantee = UPPER(:user)
      AND granted_role = UPPER(:role); */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select count(granted_role) into :b1  from dba_role_privs w\
here (grantee=UPPER(:b2) and granted_role=UPPER(:b3))";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )208;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&result;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)user;
   sqlstm.sqhstl[1] = (unsigned int  )20;
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)role;
   sqlstm.sqhstl[2] = (unsigned int  )20;
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
   if (sqlca.sqlwarn[0] == 'W') goto check_result;
}



   /* Check to see if no rows were selected. */

check_result:
   /* first undo thw whenever checking for the rest of this file */

   /* EXEC SQL WHENEVER SQLWARNING CONTINUE; */ 


   if (result < 1)
   {
      PrintError ("User %s has not been granted role %s \n needed for this application", userName, roleName);
      return (ERROR);
   }

/* must still check for an SQL error */

   message = SqlCheckError(FUNC_NAME);
   if (message != NULL)
   {
      PrintError ("Error in %s:\n %s\n",FUNC_NAME,message); 
      return (ERROR);
   }
   
   /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )239;
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


   return (OK);
}
/****************************************************************************/
#define FUNC_NAME    "SqlGetRolePsswd"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlGetRolePsswd

<P> <STRONG>Function signature:</STRONG> int SqlGetRolePsswd (char *, char *)

<P> <STRONG>Function description:</STRONG> Retrieves the password of the specified role from a protected table and sets the password variable with the value.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *roleName:  Name of role whose password is needed.
<BR><DD> char *rolePsswd: Holds value of retrieved password.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/


int SqlGetRolePsswd (char *roleName, char *rolePsswd)
{
   /*Database host variables*/
   /* EXEC SQL BEGIN DECLARE SECTION; */ 

   char role[20];
   char psswd[20];
   int   result = 0;   
   /* EXEC SQL END DECLARE SECTION; */ 


   strcpy(role, roleName);
   
   /* EXEC SQL AT :current_dbsite SELECT psswd
      INTO :psswd
   FROM role_psswd
   WHERE UPPER (role) = UPPER (:role); */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "select psswd into :b1  from role_psswd where UPPER(role)=U\
PPER(:b2)";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )258;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)psswd;
   sqlstm.sqhstl[0] = (unsigned int  )20;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)role;
   sqlstm.sqhstl[1] = (unsigned int  )20;
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



   /* Check to see if no rows were selected. */
   if (sqlca.sqlcode > 0)
   {
      PrintError ("There is no entry for role %s in role_psswd\n", roleName);
      return (ERROR);
   }

   if ((result = SqlErrorHandler
	(FUNC_NAME, FALSE,
	 "Error selecting from role_psswd for role %s\n\tExiting.", role)) 
       == ERROR)
   {
      return (ERROR);
   }

   /* EXEC SQL AT :current_dbsite COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 5;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )285;
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


   strcpy (rolePsswd, psswd);

   return (OK);
}

/*****************************************************************************/
#define FUNC_NAME "SqlDisconnect"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlDisconnect

<P> <STRONG>Function signature:</STRONG> int SqlDisconnect ()

<P> <STRONG>Function description:</STRONG> Commits all open transactions on both DB connections and disconnects from both.

<P><DL> <STRONG>Function arguments:</STRONG>
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlDisconnect()
{
  int result1, result2;

  /* db_connected is decremented in SqlDisconnectAt */
  /* Disconnect from default connection. */
  result1 = SqlDisconnectAt (NULL);

  /* Disconnect from named connection */
  result2 = SqlDisconnectAt (DB);

  if (result1 == ERROR || result2 == ERROR)
     return (ERROR);
  else
     return (OK);
}

/******************************************************************/
#define FUNC_NAME "SqlDisconnectAt"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlDisconnectAt

<P> <STRONG>Function signature:</STRONG> int SqlDisconnectAt (char *)

<P> <STRONG>Function description:</STRONG> Commits all open transactions on the named connection, and disconnects.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *dbSiteAlias:  Logical name of connection (db_site_code)
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SqlDisconnectAt(char *dbAlias)
{
  int result;
  DATABASE_ALIAS dbName;

  if (!dbAlias)
  {
     /* Disconnect from default connection. */
     /* EXEC SQL COMMIT RELEASE; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 10;
     sqlstm.arrsiz = 5;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )304;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)256;
     sqlstm.occurs = (unsigned int  )0;
     sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


     if ((result = SqlErrorHandler 
	  (FUNC_NAME, FALSE, 
	   "Error disconnecting from default database.\n\tExiting.")) == ERROR)
	return (ERROR);
  }
  else
  {
     /* Disconnect from named connection */
     strcpy (dbName, dbAlias);

     /* EXEC SQL AT :dbName COMMIT RELEASE; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 10;
     sqlstm.arrsiz = 5;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )319;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)256;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (         void  *)dbName;
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


     if ((result = SqlErrorHandler 
	  (FUNC_NAME, FALSE, 
	   "Error disconnecting from database %s.\n\tExiting.", dbName)) == ERROR)
	return (ERROR);
  }

  db_connected--;
  return (OK);
}

/******************************************************************/

#define FUNC_NAME "SqlDisconnectAll"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlDisconnectAll

<P> <STRONG>Function signature:</STRONG> int SqlDisconnectAll (void)

<P> <STRONG>Function description:</STRONG> Disconnects from (all) databases.
This function is primarily called by SignalHandler, and apps connecting to 
more than one database.

<P><DL> <STRONG>Function arguments:</STRONG>
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SqlDisconnectAll(void)
{
/* This function is called by SignalHandler() and all applications
   which open connections to more than on database. It is responsible for
   closing all open database connections. It uses the global variable
   "db_connected" to check the number of open sessions

   db_connected is modified only by the SqlConnect and
   SQlDisconnect functions. */

   int counter;
   int result;
   ERROR_MSG errorMessage;
   int err = FALSE,
       num_connections;

   if (db_connected == 0) /* no open connections */
   {
      PrintError("No database connections open...\n");
      return(OK);
   }

   strcpy (errorMessage, "Problem disconnecting from database(s):");

   /* remember db_connected, as it will be changed by SqlDisconnectAt! */
   num_connections = db_connected;

   for (counter = 0; counter < num_connections; counter++)
   {
      result = SqlDisconnectAt(dbconnected_array[counter]);
      if (result != OK)
      {
	 strcat (errorMessage, " ");
	 strcat (errorMessage, dbconnected_array[counter]);
	 err = TRUE;
      }
   }

   if (err)
   {
      PrintError ("%s\n", errorMessage);
      PrintError ("All other connection(s) closed.\n");
   }
   else
      PrintError("DB connection(s) closed...\n");

   return(OK);
}

/******************************************************************/
#define FUNC_NAME "AddConnection"

/*
<HTML>

<P> <STRONG>Function name:</STRONG> AddConnection

<P> <STRONG>Function signature:</STRONG> int AddConnection (char *)

<P> <STRONG>Function description:</STRONG> Adds the named connection to the global list of connections (dbconnected_array).

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *dbSiteAlias:  Logical name of connection (db_site_code)
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int AddConnection (char *dbAlias)
{
   if (dbAlias)
      strcpy (dbconnected_array[db_connected - 1], dbAlias);
   else
      dbconnected_array[db_connected - 1][0] = '\0';
   return (OK);
}

/******************************************************************/
#define FUNC_NAME "SwitchSession"

/*
<HTML>

<P> <STRONG>Function name:</STRONG> SwitchSession

<P> <STRONG>Function signature:</STRONG> int SwitchSession (char *)

<P> <STRONG>Function description:</STRONG> Sets the global variable current_dbsite to the indicated connection, effectively making all subsequent SQL statements affect the indicated connection.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *dbSiteAlias:  Logical name of connection (db_site_code)
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SwitchSession (char *dbAlias)
{
   int i;

   for (i = 0; i < db_connected; i++)
   {
      if (!strcmp (dbAlias, dbconnected_array[i]))
      {
	 strcpy (current_dbsite, dbAlias);
	 current_dbsite[strlen(dbAlias)] = '\0';
	 return (OK);
      }
   }

   /* Didn't find dbAlias in list of connections. */
   PrintError ("%s: Alias %s not a valid connection identifier.\n", 
	       FUNC_NAME, dbAlias);
   return (ERROR);
}
