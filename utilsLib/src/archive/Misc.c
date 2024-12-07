
/****************************************************************************
* DESCRIPTION
* This file contains functions for miscellaneous tasks.
* 
* AUTHOR - EDITORS
* Rick Casey
* Jan 2002
****************************************************************************/

/*
<HTML>

<P> <STRONG>Function name:</STRONG> trim

<P> <STRONG>Function signature:</STRONG> trim(char s[])

<P> <STRONG>Function description:</STRONG> Removes trailing white space from a string.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char []             : The string to be trimmed.
</DL>

<P> <STRONG>Function return value:</STRONG> Returns int, length of new string.
<BR>
<P> Useage: trim(arrayName);
</HTML>
*/


#include <stdio.h>

int trim(char s[])
{
	int n;

	for (n = strlen(s)-1; n >=0 ; n--)
		if (s[n] != ' ' && s[n] != '\t' && s[n] != '\n')
			break;
	s[n+1] = '\0';
	return n+1;
}



