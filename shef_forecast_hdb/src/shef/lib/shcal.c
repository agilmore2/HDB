/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 16:59:29 */
/*FOR_C Options SET: c=2 com=u do=r4 ftn=2ln6k op=iv s=dvn str=l sq=i */

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
         SUBROUTINE SHCAL(IORD,LEAP,LMON,LDAY)
  
       PURPOSE                                                    
         Calculate the calender month and day given the ordinal
         day in IORD. LEAP is 1 is a leap year, 0 otherwise.
  
       VERSION and UPDATES
         1.0    JUL 82   Geoffrey M. Bonnin
                Original Version
         1.1    FEB 84 
                Tableless date conversion             
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
  
  --------------------------------------------------------------------- */

void shcal(iord, leap, lmon, lday)
short int *iord, *leap, *lmon, *lday;
{
	static short int ia;

	ia = *iord + ((305 + *iord - *leap)/365)*(2 - *leap);
	*lmon = ((ia + 91)*20)/611 - 2;
	*lday = ia + 30 - 30**lmon - (56**lmon)/100;

	return;

}

