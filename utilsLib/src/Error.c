/****************************************************************************
* DESCRIPTION
* This file contains functions for setting error outputs and printing error
* messages.
* 
* AUTHOR - EDITORS
* Carol Marra
* Neil Wilson
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <varargs.h>
#include <defines.h> 
#include <utils.h>

static FILE *errorFd = NULL;
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SetErrorFd

<P> <STRONG>Function signature:</STRONG> int SetErrorFd (char *)

<P> <STRONG>Function description:</STRONG> Opens the named file which will be used for all error output for the duration of an executable. Can be stdout.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *filename: Name of the error output.
</DL>

<P> <STRONG>Function return value:</STRONG> OK

</HTML>
*/

int SetErrorFd (char *fileName)

{
  if (!strcmp (fileName, "stdout"))
    {
      fclose (errorFd);
      errorFd = NULL;
    }

  else
    {
      if (errorFd)
	{
	  fclose (errorFd);
	}
      
      if ((errorFd = fopen (fileName, "w")) == NULL)
	printf ("Error opening %s\n. Error messages will go to stdout.\n",
		fileName);
    }

  return (OK);
}

/************************************************************************/

/*
<HTML>

<P> <STRONG>Function name:</STRONG> CloseErrorFd

<P> <STRONG>Function signature:</STRONG> int CloseErrorFd ()

<P> <STRONG>Function description:</STRONG> Closes the error file. Call at the end of a program which has called SetErrorFd.

<P><DL> <STRONG>Function arguments:</STRONG>
</DL>

<P> <STRONG>Function return value:</STRONG> OK

</HTML>
*/

int CloseErrorFd ()
{
    if (errorFd)
      fclose (errorFd);

    return (OK);
}

/************************************************************************/

/*
<HTML>

<P> <STRONG>Function name:</STRONG> PrintError

<P> <STRONG>Function signature:</STRONG> int PrintError(char *, ...)

<P> <STRONG>Function description:</STRONG> Print a message (format) string and all its arguments (variable number) which are passed in to the specified stream.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *messageString: The message string to be printed.
<BR><DD> ...:                 The arguments to the format string.
</DL>

<P> <STRONG>Function return value:</STRONG> OK

</HTML>
*/

int PrintError(char *messageString, ...)
{
  va_list arg_list;

  va_start (arg_list, messageString);
  if (errorFd)
    vfprintf (errorFd, messageString, arg_list);
  else
    vfprintf (stdout, messageString, arg_list);

  va_end (arg_list);

  return ((short) OK);
}

/************************************************************************/

/*
<HTML>

<P> <STRONG>Function name:</STRONG> PrintDbError

<P> <STRONG>Function signature:</STRONG> int PrintDbError(char *, va_list)

<P> <STRONG>Function description:</STRONG> Print a message (format) string and all its arguments (a va_list) which are passed in to the specified stream.  This function should be called only when the argument list is in the form of a va_list. That is, only when va_start and va_end are executed in the calling function.  For a variable number of args, use PrintError.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *messageString: The message to be printed.
<BR><DD> va_list arg_list:    The arguments to the message string.
</DL>

<P> <STRONG>Function return value:</STRONG>

</HTML>
*/
int PrintDbError(char *messageString, va_list arg_list)
{
  if (errorFd)
    vfprintf (errorFd, messageString, arg_list);
  else
    vfprintf (stdout, messageString, arg_list);

  return ((short) OK);
}

/************************************************************************/

/*
<HTML>

<P> <STRONG>Function name:</STRONG> FilePrintError

<P> <STRONG>Function signature:</STRONG> int FilePrintError(FILE *, char *, ...)

<P> <STRONG>Function description:</STRONG> Print and send to a log file a message (format) string and all its arguments (variable number) which are passed in to the specified stream.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> FILE *filePointer: Pointer to the log file.
<BR><DD> char *messageString: The message string to be printed.
<BR><DD> ...:                 The arguments to the format string.
</DL>

<P> <STRONG>Function return value:</STRONG> OK

</HTML>
*/

int FilePrintError(FILE *filePointer, char *messageString, ...)
{
  va_list arg_list;

  va_start (arg_list, messageString);
  if (errorFd)
    vfprintf (errorFd, messageString, arg_list);
  else
    vfprintf (stdout, messageString, arg_list);

  vfprintf (filePointer, messageString, arg_list);  

  va_end (arg_list);

  return ((short) OK);
}

/************************************************************************/

/*
<HTML>

<P> <STRONG>Function name:</STRONG> FileError

<P> <STRONG>Function signature:</STRONG> int FileError(FILE *, char *, ...)

<P> <STRONG>Function description:</STRONG> Send to a log file only a message (format) string and all its arguments (variable number) which are passed in to the specified stream.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> FILE *filePointer: Pointer to the log file.
<BR><DD> char *messageString: The message string to be printed.
<BR><DD> ...:                 The arguments to the format string.
</DL>

<P> <STRONG>Function return value:</STRONG> OK

</HTML>
*/

int FileError(FILE *filePointer, char *messageString, ...)
{
  va_list arg_list;

  va_start (arg_list, messageString);

  vfprintf (filePointer, messageString, arg_list);  

  va_end (arg_list);

  return ((short) OK);
}

