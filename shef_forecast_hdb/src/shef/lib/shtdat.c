
#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "shef_structs_external.h"

/*---------------------------------------------------------------------
  
       NAME
         SUBROUTINE SHTDAT(LYEAR,LMON,LDAY,STATUS)
  
       PURPOSE                                                    
         THIS ROUTINE CHECKS TO SEE IF A DATE IS VALID.
  
       VERSION and UPDATES
         1.0    AUG 82   Geoffrey M. Bonnin
                Original Version
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
  
  --------------------------------------------------------------------- */

void shtdat(lyear, lmon, lday, status)
short int *lyear, *lmon, *lday, *status;
{
	static short int leap, mon;
	void shleap();
	*status = 0;

	/*             Test the month */

	if( (*lmon > 12) || (*lmon < 1) )
		goto L_9000;

	/*             Test the day */

	mon = *lmon;
	if( *lday < 1 )
		goto L_9000;
	if( mon == 2 )
		goto L_10;
	if( mon >= 8 )
		mon = mon + 1;
	leap = 30 + mon - (mon/2)*2;
	if( *lday > leap )
		goto L_9000;
	goto L_100;

L_10:
	shleap( lyear, &leap );
	if( *lday > (leap + 28) )
		goto L_9000;

L_100:
	return;

L_9000:
	*status = 1;
	return;

}

