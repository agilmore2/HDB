
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
    "SqlError.pc"
};


static unsigned int sqlctx = 164955;


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


/****************************************************************************
* DESCRIPTION
* This file contains 4 functions to be used in handling database errors:
*   SqlErrorHandler
*   SqlCheckError
*   SqlCheckErrorNoRows
*   SqlCheckWarning
* 
*********************************
* SqlErrorHandler
* Purpose: Called after every EXEC SQL operation; checks for both
* 	   DB errors and warnings.  Prints error messages.
* 
* Input:
*	char *func_name - name of calling function
*	short noRowsOK - indicates which checkError function to call
*	char *message - message to print if there's an error or warning
*
* Return:
* 	short sqlError - indicates if error or warning was found
*
*********************************
* SqlCheckError
* Purpose: Checks if the database is in error. If the database has produced 
*	   an error it returns the error message.
* Input
* 	char *func_name - name of calling function
*
* Return
* 	char *errorMessage
*
*********************************
* SqlCheckErrorNoRows
* Purpose: Checks if the database is in error. An sqlca.sqlcode of >0 is
*	   *not* considered an error; if this function is called, it is ok
*	   if the statement processed no rows. If the database has produced 
*	   an error it returns the error message.
* Input
* 	char *func_name - name of calling function
*
* Return
* 	char *errorMessage
*
*********************************
* SqlCheckWarning
* Purpose: Checks if the database has produced a warning. If so, the warning 
*	   message is returned.
* Input
* 	char *func_name - name of calling function
*
* Return
* 	char *warningMessage
*
*********************************
* SqlFilePrintErrorHandler
* Purpose: Can be called after every EXEC SQL operation; checks for both
* 	   DB errors and warnings.  Prints error messages and sends them
*          to a logfile.
* 
* Input:
*       FILE * - logfile to print message to
*	char *func_name - name of calling function
*	short noRowsOK - indicates which checkError function to call
*	char *message - message to pass on if there's an error or warning
*
* Return:
* 	short sqlError - indicates if error or warning was found
*
*********************************
* SqlFileErrorHandler
* Purpose: Can be called after every EXEC SQL operation; checks for both
* 	   DB errors and warnings.  Sends error messages to a 
*          logfile only.
* 
* Input:
*       FILE * - logfile to print message to
*	char *func_name - name of calling function
*	short noRowsOK - indicates which checkError function to call
*	char *message - message to pass on if there's an error or warning
*
* Return:
* 	short sqlError - indicates if error or warning was found
*
*********************************
* AUTHOR - EDITORS
* Carol Marra
* Neil Wilson
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <defines.h> 
#include <typedefs.h>
#include <dbutils.h>
#include <utils.h>

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


/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlErrorHandler

<P> <STRONG>Function signature:</STRONG> int SqlErrorHandler(char *, short, char *, ...)


<P> <STRONG>Function description:</STRONG> Checks the results of the most recently executed SQL function.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *funcName: Name of calling function.
<BR><DD> short noRowsOk: Indicator (0 or 1) if it's OK for the SQL function to have processed no rows.
<BR><DD> char *message:  Message to be printed if there was an error; can contain C format elements.
<BR><DD> ...:            Variable arguments to the message string.
</DL>

<P> <STRONG>Function return value:</STRONG> OK, ERROR, or WARNING

</HTML>
*/

int SqlErrorHandler(char *funcName, short noRowsOk,
		      char *message, ...)
{
  va_list arg_list;
  char *returnMessage;

  va_start (arg_list, message);
  
  if (noRowsOk)
  {
     if ((returnMessage = SqlCheckErrorNoRows(funcName)) != NULL)
     {
	fprintf (stderr, "%s\n\t", returnMessage);
	PrintDbError (message, arg_list);
	return (ERROR);
     }
  }
  else 
     if ((returnMessage = SqlCheckError(funcName)) != NULL)
     {
	fprintf (stderr, "%s\n\t", returnMessage);
	PrintDbError (message, arg_list);
	return (ERROR);
     }

  if((returnMessage = SqlCheckWarning (funcName)) != NULL)
  {
     fprintf (stderr, "%s\n\t", returnMessage);
     PrintDbError (message, arg_list);
     return (WARNING);
  }
  
  va_end (arg_list);
  
  return(OK);
}

/*****************************************************************************/
 /*last changed 6/25/97*/

/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlCheckError

<P> <STRONG>Function signature:</STRONG> char *SqlCheckError(char *)

<P> <STRONG>Function description:</STRONG> Determine if any error, including no rows processed, was incurred by last SQL statement.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *funcName: Name of function passed to SqlErrorHandler.
</DL>

<P> <STRONG>Function return value:</STRONG> char *, error message string

</HTML>
*/
char *SqlCheckError(char *funcName)
{
  SQL_ERROR_MSG sqlError;
  
  ERROR_MSG tmp;
  char *errorMessage;
  
  if ((sqlca.sqlcode == 0))
    return(NULL);

  sqlError[0] = '\0'; 
  strcpy(sqlError,sqlca.sqlerrm.sqlerrmc);
  sprintf(tmp,"\nSQL error in %s\n\t%s", funcName, 
	  sqlError);
  errorMessage = strdup (tmp);
  return((char *) errorMessage);
}

/*****************************************************************************/
 /*last changed 6/25/97*/
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlCheckErrorNoRows

<P> <STRONG>Function signature:</STRONG> char *SqlCheckErrorNoRows(char *)

<P> <STRONG>Function description:</STRONG> Determine if any error, excluding no rows processed, was incurred by last SQL statement.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *funcName: Name of function passed to SqlErrorHandler.
</DL>

<P> <STRONG>Function return value:</STRONG> char *, error message string

</HTML>
*/
char *SqlCheckErrorNoRows(char *funcName)

{
  SQL_ERROR_MSG sqlError;
  
  ERROR_MSG tmp;
  char *errorMessage;
  
  if ((!sqlca.sqlcode) ||(sqlca.sqlcode > 0))
    return(NULL);

  sqlError[0] = '\0'; 
  strcpy(sqlError,sqlca.sqlerrm.sqlerrmc);
  sprintf(tmp,"\nSQL error in %s\n\t%s", funcName, 
	  sqlError);
  errorMessage = strdup (tmp);
  return((char *) errorMessage);
}
/*****************************************************************************/
    /*last changed 6/25/97*/
#define WARN_MESSAGE "Database SQL warning : "
#define NO_WARNING_CODE " "
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlCheckWarning

<P> <STRONG>Function signature:</STRONG> char *SqlCheckWarning(char *)

<P> <STRONG>Function description:</STRONG> Determine if any warnings were incurred by last SQL statement.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *funcName: Name of function passed to SqlErrorHandler
</DL>

<P> <STRONG>Function return value:</STRONG> char *, warning message string

</HTML>
*/

char *SqlCheckWarning(char *funcName)

{
  static SQL_ERROR_MSG message;
  SQL_ERROR_MSG sqlError;
  ERROR_MSG tmp;
  char *warningMessage;
  
  if(sqlca.sqlwarn[0] != 'W') /* there is no warning */
     return(NULL);
  
  strcpy (message, WARN_MESSAGE);
  
  if(sqlca.sqlwarn[1] == 'W')
     strcat(message, "truncated column...");

  if(sqlca.sqlwarn[2] == 'W')
     strcat(message, "NULL column unused in group function...");

  if(sqlca.sqlwarn[3] == 'W')
     strcat(message, "number of columns in query does not match number of columns in INTO clause...");

  if(sqlca.sqlwarn[4] == 'W')
     strcat(message, "unconditional UPDATE or DELETE...");

  if(sqlca.sqlwarn[5] == 'W')
     strcat(message, "PL/SQL compilation error...");

/* sqlca.sqlwarn[6] and sqlca.sqlwarn[7] are no longer in use 
   (Programmer's Guide to the Oracle Precompilers (CD-version 2.2);  */

  sqlError[0] = '\0'; 
  strcpy(sqlError,sqlca.sqlerrm.sqlerrmc);

  sprintf (tmp, "\nSQL warning in %s\n\t%s\n\t%s", funcName,
	   message, sqlError);

  warningMessage = (char*)strdup(tmp);
  return (warningMessage);
}
/*****************************************************************************/
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlFilePrintErrorHandler

<P> <STRONG>Function signature:</STRONG> int SqlFilePrintErrorHandler(FILE *, 
char *, short, char *, ...)


<P> <STRONG>Function description:</STRONG> Checks the results of the most
recently executed SQL function and writes to log file as well as prints
any problems.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> FILE *filePointer: Pointer to logfile to write to.
<BR><DD> char *funcName: Name of calling function.
<BR><DD> short noRowsOk: Indicator (0 or 1) if it's OK for the SQL function to
have processed no rows.
<BR><DD> char *message:  Message to be printed if there was an error; can contain C format elements.
<BR><DD> ...:            Variable arguments to the message string.
</DL>

<P> <STRONG>Function return value:</STRONG> OK, ERROR, or WARNING

</HTML>
*/

int SqlFilePrintErrorHandler(FILE *filePointer, char *funcName, short noRowsOk,
		      char *message, ...)
{
  va_list arg_list;
  char *returnMessage;

  va_start (arg_list, message);
  
  if (noRowsOk)
  {
     if ((returnMessage = SqlCheckErrorNoRows(funcName)) != NULL)
     {
        /* Send message to file */
	fprintf (filePointer, "%s\n\t", returnMessage);
	vfprintf (filePointer, message, arg_list);

        /* Print message */
	fprintf (stdout, "%s\n\t", returnMessage);
	PrintDbError (message, arg_list);

	return (ERROR);
     }
  }
  else 
     if ((returnMessage = SqlCheckError(funcName)) != NULL)
     {
        /* Send message to file */
	fprintf (filePointer, "%s\n\t", returnMessage);
	vfprintf (filePointer, message, arg_list);

        /* Print message */
	fprintf (stdout, "%s\n\t", returnMessage);
	PrintDbError (message, arg_list);

	return (ERROR);
     }

  if((returnMessage = SqlCheckWarning (funcName)) != NULL)
  {
     /* Send message to file */
     fprintf (filePointer, "%s\n\t", returnMessage);
     vfprintf (filePointer, message, arg_list);

     /* Print message */
     fprintf (stdout, "%s\n\t", returnMessage);
     PrintDbError (message, arg_list);

     return (WARNING);
  }
  
  va_end (arg_list);
  
  return(OK);
}
/*****************************************************************************/
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SqlFileErrorHandler

<P> <STRONG>Function signature:</STRONG> int SqlFileErrorHandler(FILE *, 
char *, short, char *, ...)


<P> <STRONG>Function description:</STRONG> Checks the results of the most
recently executed SQL function and writes problems to log file.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> FILE *filePointer: Pointer to logfile to write to.
<BR><DD> char *funcName: Name of calling function.
<BR><DD> short noRowsOk: Indicator (0 or 1) if it's OK for the SQL function to
have processed no rows.
<BR><DD> char *message:  Message to be printed if there was an error; can contain C format elements.
<BR><DD> ...:            Variable arguments to the message string.
</DL>

<P> <STRONG>Function return value:</STRONG> OK, ERROR, or WARNING

</HTML>
*/

int SqlFileErrorHandler(FILE *filePointer, char *funcName, short noRowsOk,
		      char *message, ...)
{
  va_list arg_list;
  char *returnMessage;

  va_start (arg_list, message);
  
  if (noRowsOk)
  {
     if ((returnMessage = SqlCheckErrorNoRows(funcName)) != NULL)
     {
        /* Send message to file */
	fprintf (filePointer, "%s\n\t", returnMessage);
	vfprintf (filePointer, message, arg_list);

	return (ERROR);
     }
  }
  else 
     if ((returnMessage = SqlCheckError(funcName)) != NULL)
     {
        /* Send message to file */
	fprintf (filePointer, "%s\n\t", returnMessage);
	vfprintf (filePointer, message, arg_list);

	return (ERROR);
     }

  if((returnMessage = SqlCheckWarning (funcName)) != NULL)
  {
     /* Send message to file */
     fprintf (filePointer, "%s\n\t", returnMessage);
     vfprintf (filePointer, message, arg_list);

     return (WARNING);
  }
  
  va_end (arg_list);
  
  return(OK);
}
