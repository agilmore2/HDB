/****************************************************************************
**
**	IDENTIFICATION SECTION
**	----------------------
**
**	Module Name:	int ReadOptionalArgs(char **optionalArgs,
**                          int numArgs)
**
**	File Name:	GetOptionalArgs.c
**
**	Date Written:	July, 1996
**
**	Author(s): 	Carol Marra
**
**	Module Description:	
**	routine to read all system-or-user specified args passed to 
**        DMIs on the command line, and load them into the proper
**        global variables for use by the DMI modules.
**		
**      Variables Passed In:  
**         char ** -- array of strings containing remaining args
**                    in -U<keyword>=<value> or -S<keyword> format.
**         int     -- number of args in array
**
**      Comments:  
**        This routine assumes that all variables which are needed by the
**        DMIs have been globally declared in dmiGlobals.h and externed
**        in dmi_utils.h. This routine must be added to in order to handle
**        new args.
**
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*  hdb apps include file to define OK and ERROR  */
#include <dbutils.h>
#include <utils.h>
#include <defines.h>
#include <typedefs.h>

/*  hdb dmi stuff */
#include <dmi_utils.h>


#define FUNC_NAME "GetOptionalArgs"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetOptionalArgs

<P> <STRONG>Function signature:</STRONG> int GetOptionalArgs (char **, int)

<P> <STRONG>Function description:</STRONG> Read all system-or-user specified args passed to DMIs on the command line, and load them into the proper global variables for use by the DMI modules.


<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char ** args: Array of strings containing remaining args in -U<keyword>=<value> or -S<keyword> format.
<BR><DD> int numArgs:  Number of args in the array.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int GetOptionalArgs(char **args, int numArgs)

{
  int numSysArgs,
      result;

  if ((result = GetSystemArgs (args, numArgs, &numSysArgs)) != OK)
    {
      PrintError ("Problem getting system arguments.\n\tExiting.");
      return (ERROR);
    }

  if ((result = GetUserArgs (&(args[numSysArgs]), numArgs - numSysArgs)) 
      != OK)
    {
      PrintError ("Problem getting user arguments.\n\tExiting.");
      return (ERROR);
    }

  return (OK);
}

/**************************************************************************/
#define FUNC_NAME "GetSystemArgs"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetSystemArgs

<P> <STRONG>Function signature:</STRONG> int GetSystemArgs (char **, int, int *)

<P> <STRONG>Function description:</STRONG> Read all system args passed to DMIs on the command line. Note that there are currently no system arguments.


<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char ** systemArgs: Array of strings containing args.
<BR><DD> int numArgs:        Number of args in the array.
<BR><DD> int numArgs:        Number of system args in the array. Output.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int GetSystemArgs (char **systemArgs, int numArgs, int *numSysArgs)

{
  PrintError ("Note that no system args are currently processed by DMI code.\n");
  
  *numSysArgs = 0;

  return (OK);
}

/**************************************************************************/
#define FUNC_NAME "GetUserArgs"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetUserArgs

<P> <STRONG>Function signature:</STRONG> int GetUserArgs (char **, int)

<P> <STRONG>Function description:</STRONG> Read all user args passed to DMIs on the command line.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char ** systemArgs: Array of strings containing args.
<BR><DD> int numArgs:        Number of args in the array.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int GetUserArgs (char **userArgs, int numArgs)

{
  char  charValue[32];
  int   result;

  /* Get the model run id. */
  if ((result = FindUserArg (userArgs, numArgs, MODEL_RUN, charValue)) != OK)
    {
      return (ERROR);
    }
  else if (result == NA)
    {
      PrintError ("Model_run_id must be supplied. Exiting.");
      return (ERROR);
    }
  else
    {
      model_run_id = atoi (charValue);
    }

  /* Put checks for other args here. */

  return (OK);
}

/**************************************************************************/
#define FUNC_NAME "FindUserArg"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> FindUserArg

<P> <STRONG>Function signature:</STRONG> int FindUserArg (char **, int, char *, char *)

<P> <STRONG>Function description:</STRONG> Find the specified user argument within the list of args, and set supplied pointer to its value.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char ** systemArgs: Array of strings containing args.
<BR><DD> int numArgs:        Number of args in the array.
<BR><DD> char *kword:        Keyword indicating the argument to find.
<BR><DD> char *charValue:    The value of the desired argument.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int FindUserArg (char **userArgs, int numArgs, char *kword, char *charValue)
{
  char keyword[32];
  int count = 0,
      keywordLength,
      argLength;
  char *cur;

  for (count = 0; count < numArgs; count++)
    {
      cur = &(userArgs[count][2]);
      argLength = strlen (cur);
      keywordLength = strcspn (cur, "=");
      
      if (keywordLength == argLength)
	{
	  PrintError ("Improper user argument format for: %s. Needs '='.\n\tExiting.\n", cur);
	  return (ERROR);
	}

      strncpy (keyword, cur, keywordLength);
      keyword[keywordLength] = '\0';
  
      if (!strcmp (keyword, kword))
	{
	  strcpy (charValue, &(cur[keywordLength + 1]));
	  return (OK);
	}
    }

  PrintError ("Warning: Parameter %s not found.\n", kword);
  return (NA);
}




