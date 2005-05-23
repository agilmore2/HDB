/*Translated by FOR_C, v3.4.2 (-), on 04/06/95 at 13:56:10 */
/*FOR_C Options SET: c=4 com=u do=r44 ftn=ln6kk op=iv s=dvnk str=l sq=i */
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
         SUBROUTINE SHLOCL(NYEAR,NMON,NDAY,NHOUR,NMIN,NSEC,NADJTZ,IADJ,STATUS)
  
       PURPOSE                                                    
  
         This routine adjusts the time difference between the local time
         and zulu time depending on whether or not daylight
         savings is in effect.
         If IADJ comes in as 1 then the time coming in is zulu.
         If IADJ comes in as 0 then the time coming in is local time.
  
               Daylight/Standard Switch over Dates
  
         The switch over dates for daylight and standard time for the
         years 1976 through 2020 are contained in the table ITABLE.
         In 1987, the law went into effect that made the change over
         on the first Sunday in April, and the last Sunday in October.
         ITABLE(1,I) IS THE CHANGE OVER DATE IN APRIL TO DAYLIGHT TIME.
         ITABLE(2,I) IS THE CHANGE OVER DATE IN OCTOBER TO STANDARD TIME.
         The change occurs at 2AM.
  
               **** The TABLE is located in BLOCKDATA ****
         Each pair of numbers is associated with a year, beginning
         with 1976.  The first number is the first Sunday in April
         and the second number is the last Sunday in October.

         DATA ITABLE
       >          /26,31, 24,30, 30,29, 29,28, 27,26, 26,25,
       >           25,31, 24,30, 29,28, 28,27, 27,26,  5,25,
       >            3,30,  2,29,  1,28,  7,27,  5,25,  4,31,
       >            3,30,  2,29,  7,27,  6,26,  5,25,  4,31,
       >            2,29,  1,28,  7,27,  6,26,  4,31,  3,30,
       >            2,29,  1,28,  6,26,  5,25,  4,31,  3,30,
       >            1,28,  7,27,  6,26,  5,25,  3,30,  2,29,
       >            1,28,  7,27,  5,25/
  
         VERSION and UPDATES
         1.0    AUG 82   Geoffrey M. Bonnin
                Original Version
         1.1    AUG 84 
                Add seconds to time.
         2.0    JUN 94   David G. Brandon
                Daylight switchover date now good through year 2020
                Also Translated to 'C' using FOR_C
                Removed alternate returns to subroutines
         2.1    Include check to test if years are valid between
                1976 and 2020 (since these are the only years which
                are included in the table). Include t_data struct
                so that century is brought over. 
         2.2    OCT 19 97 DGB
                Fix problem with associating dates in the array
                datet_.itable[][] with the correct years and days.
                Previously, the switchover days for daylight/standard
                did not match up properly.
   -------------------------------------------------------------------- */



void shlocl(nyear, nmon, nday, nhour, nmin, nsec, nadjtz, iadj, status)
short int *nyear, *nmon, *nday, *nhour, *nmin, *nsec, *nadjtz, *iadj, 
	 *status;
{
	static short int ia, ib, iy, num, ntemp;
int k,i;

	*status = 0;

	/*             Test if adjustment necesary */

	if( *nadjtz >= 0 )
		goto L_50;


    /*   check if the year is between 1976 and 2020 & if adjustment is  */
    /*   necessary.                                                     */

     if ( *nyear < 1976 || *nyear > 2020 )
          goto L_9010;

	/*              Check whether the time is in local or zulu */
	ia = 2;
	ib = 2;
	if( *iadj != 1 )
		goto L_5;
	ia = 2 - *nadjtz/60;
	ib = 1 - *nadjtz/60;

	/*             Adjust the year to suit the bounds of the ITABLE */

L_5:
	if( ( *nyear >= 1976) && ( *nyear <= 1999) )

		{
        iy = *nyear -1976;                                  /* dgb:10/19/97 */
		}
	else
		{
		if( ( *nyear >= 2000) && ( *nyear <= 2020) )
			{
            iy = *nyear + 2024;                              /* dgb:10/19/97 */
			}
		else
			{
			goto L_9010;
			}
		}


	/*             Test if daylight time */

	if( (*nmon < 4) || (*nmon > 10) )
		goto L_40;
	if( (*nmon > 4) && (*nmon < 10) )
		goto L_20;
	if( *nmon == 10 )
		goto L_10;

	/*             April */

	if( *nday < datet_.itable[iy][0] )                     /* dgb:10/19/97 */
		goto L_40;
	if( *nday > datet_.itable[iy][0] )                     /* dgb:10/19/97 */
		goto L_20;

	if( *iadj == 1 )
		goto L_8;
	if( *nhour != 2 )
		goto L_8;
/*
	if( *nmin != 0 )
		goto L_9020;
	if( *nsec != 0 )
		goto L_9020;
*/
L_8:
	if( *nhour < ia )
		goto L_40;
	if( *nhour > ia )
		goto L_20;
	if( *nmin > 0 )
		goto L_20;
	if( *nsec > 0 )
		goto L_20;
	goto L_40;

	/*             October */

L_10:
	if( *nday > datet_.itable[iy][1] )                     /* dgb:10/19/97 */
		goto L_40;
	if( *nday < datet_.itable[iy][1] )                     /* dgb:10/19/97 */
		goto L_20;
	if( *nhour > ib )
		goto L_40;
	if( *nhour < ib )
		goto L_20;
	if( *nmin > 0 )
		goto L_40;
	if( *nsec > 0 )
		goto L_40;

	/*             Adjust for daylight time */

L_20:
	*iadj = -(*nadjtz + 60);
	goto L_100;

	/*            Standard time */

L_40:
	*iadj = -*nadjtz;
	goto L_100;

	/*            No adjustment necessary */

L_50:
	*iadj = *nadjtz;


L_100:
	return;

	/*            Error return */

L_9010:
	num = 22;
	goto L_9500;

/*
L_9020:
	num = 37;
*/

L_9500:
	sherr( &num );
	*status = 1;
	return;

}

