/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 17:02:47 */
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
         SUBROUTINE SHLEAP(LYEAR,LEAP)
  
       PURPOSE                                                    
         Determine if the year is a leap year or not.
  
         Returns:
           LEAP = 0 ,  not a leap year
           LEAP = 1 ,  leap year
  
           LYAER = 2 digit year, e.g., 1999 = 99, 2010 = 10
              
       VERSION and UPDATES
         1.0    APR 82   Geoffrey M. Bonnin
                Original Version
         1.1    JUN 89   David G. Brandon
                Adapted to QNX NWS Hydromet
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
                Alow to check years: 1976 - 2020
         2.1    JAN 4 2000 DGB
                Allow year to be either 2 or 4 digits.
                If 2 digits, get century from system.
                If 4 digits, use CCYY as given.
  --------------------------------------------------------------------- */

void shleap(lyear, leap)
short int *lyear, *leap;
{
	static short int ia, ib, ic;
        static char tempb[5];

        sprintf(tempb,"%d",*lyear);                          /* dgb:01/04/00 */
        if ( strlen(tempb) > 2 )                             /* dgb:01/04/00 */
        {
           *leap = *lyear;
        }                                                    /* dgb:01/04/00 */       
        else                                                 /* dgb:01/04/00 */
           *leap = data_.lcent*100 + *lyear;

	ia = (*leap - (*leap/4)*4 + 3)/4;
	ib = (*leap - (*leap/100)*100 + 99)/100;
	ic = (*leap - (*leap/400)*400 + 399)/400;

	*leap = 1 - ia + ib - ic;	/* LEAP IS 1 IF LEAP YEAR */

	return;

}



