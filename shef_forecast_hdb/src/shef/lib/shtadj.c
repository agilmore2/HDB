/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 17:03:02 */
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

/*-------------------------------------------------------------------
  
       NAME
         SUBROUTINE SHTADJ(LYEAR,LMON,LDAY,LHOUR,LMIN,LSEC,KADJ,IADJ,STATUS)
  
       PURPOSE                                                    
         This routine makes adjustments to the current date/time
         by adding the value of IADJ. The units of LADJ are given
         in the code IADJ where:
              1 = MINUTES
              2 = HOURS
              3 = DAYS
              4 = MONTHS
              5 = YEARS
              6 = MONTHS; END OF MONTH
              7 = SECONDS
  
         It is intended that the range of the value LADJ
         be plus/minus 99, except for minutes where the range
         may be plus/minus 1440. Also the value of the year
         may not change by more than one.
  
       VERSION and UPDATES
         1.0     JUL 82    GEOFFREY M BONNIN  MBRFC  
         1.1     JAN 84                                           
                 Correct year change problem when adjusting days  
         1.2     AUG 84                                           
                 Add seconds to times                             
         1.3     MAY 85                                           
                 Don't go past 1999                               
         2.0     JUN 94  David G. Brandon
                 Allow for years 2000 - 2020
                 Also Translated to 'C' using FOR_C
                 Removed alternate returns for subroutine
  
  --------------------------------------------------------------------- */

void shtadj(lyear, lmon, lday, lhour, lmin, lsec, kadj, iadj, status)
short int *lyear, *lmon, *lday, *lhour, *lmin, *lsec, *kadj, *iadj, 
	 *status;
{
	static short int  ia, ladj, leap, flag;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Iday = &days_.iday[0] - 1;

	*status = 0;

	/*             Is there an adjustment to make? */

	if( *kadj == 0 )
		goto L_900;
	ladj = *kadj;

	/*             Check IADJ to see what's being adjusted */

	switch( *iadj )
		{
		case 1: goto L_100;
		case 2: goto L_200;
		case 3: goto L_300;
		case 4: goto L_400;
		case 5: goto L_500;
		case 6: goto L_600;
		case 7: goto L_50;
		}

	/*             Adjusting seconds */

L_50:
	*lsec = *lsec + ladj;
	if( *lsec < 60 )
		goto L_60;
	ladj = *lsec/60;
	*lsec = *lsec - ladj*60;
	goto L_100;

L_60:
	if( *lsec >= 0 )
		goto L_900;
	ladj = (*lsec - 60)/60;
	*lsec = *lsec - ladj*60;
	if( *lsec != 60 )
		goto L_100;
	*lsec = 0;
	ladj = ladj + 1;

	/*             Adjusting minutes */

L_100:
	*lmin = *lmin + ladj;
	if( *lmin < 60 )
		goto L_110;
	ladj = *lmin/60;
	*lmin = *lmin - ladj*60;
	goto L_200;

L_110:
	if( *lmin >= 0 )
		goto L_900;
	ladj = (*lmin - 60)/60;
	*lmin = *lmin - ladj*60;
	if( *lmin != 60 )
		goto L_200;
	*lmin = 0;
	ladj = ladj + 1;

	/*             Adjusting hours */

L_200:
	*lhour = *lhour + ladj;
	if( *lhour < 24 )
		goto L_210;
	ladj = *lhour/24;
	*lhour = *lhour - ladj*24;
	goto L_300;

L_210:
	if( *lhour >= 0 )
		goto L_900;
	ladj = (*lhour - 24)/24;
	*lhour = *lhour - ladj*24;
	if( *lhour != 24 )
		goto L_300;
	*lhour = 0;
	ladj = ladj + 1;

	/*             Adjusting days
	 *             Calculate if the current year is a leap year */

L_300:
	shleap( lyear, &leap );

	/*             Calculate the ordinal day of this year */

	ia = 30*(*lmon + 2) + (55*(*lmon + 2))/100 - 2*((*lmon + 10)/13) - 
	 91 + (leap*(*lmon + 10))/13 + *lday;

	/*             Adjust it */

	ia = ia + ladj;
	if( ia <= (365 + leap) )
		goto L_310;
	ladj = 1;
	ia = ia - 365 - leap;
	leap = 1 - leap;
	shcal( &ia, &leap, lmon, lday );
	goto L_500;

L_310:
	if( ia <= 0 )
		goto L_320;
	shcal( &ia, &leap, lmon, lday );
	goto L_900;

L_320:
	ladj = -1;
	leap = 1 - leap;
	ia = ia + 365 + leap;
	shcal( &ia, &leap, lmon, lday );
	goto L_500;

	/*             Adjusting months */

L_400:
	*lmon = *lmon + ladj;
	if( *lmon < 13 )
		goto L_410;
	ladj = *lmon/12;
	*lmon = *lmon - ladj*12;
	if( *lmon != 0 )
		goto L_500;
	*lmon = 12;
	ladj = ladj - 1;
	goto L_500;

L_410:
	if( *lmon > 0 )
		goto L_900;
	ladj = (*lmon - 12)/12;
	*lmon = *lmon - ladj*12;

	/*             Adjusting the year */

L_500:
	*lyear = *lyear + ladj;
    /* dgb 032494 */

    /*
	if( (*lyear > 20) && (*lyear < 76) )
		{	
		goto L_9010;
		}
    */
	goto L_900;

	/*             Adjusting months; End of month
	 *             Is this acutally the end of a month? */

L_600:
	leap = 0;
	if( *lmon != 2 )
		goto L_610;
	shleap( lyear, &leap );

L_610:
	ia = Iday[*lmon] + leap;
	if( ia != *lday )
		goto L_9000;	/* NO IT'S NOT */

	/*             Yes it is - adjust it */

	*lmon = *lmon + ladj;
	if( *lmon < 13 )
		goto L_620;
	ladj = *lmon/12;
	*lmon = *lmon - ladj*12;
	if( *lmon != 0 )
		goto L_630;
	*lmon = 12;
	ladj = ladj - 1;
	goto L_630;

L_620:
	if( *lmon > 0 )
		goto L_640;
	ladj = (*lmon - 12)/12;
	*lmon = *lmon - ladj*12;

	/*             Do the year */

L_630:
	*lyear = *lyear + ladj;

	/*             Now get the correct day */

L_640:
	leap = 0;
	if( *lmon != 2 )
		goto L_650;
	shleap( lyear, &leap );

L_650:
	*lday = Iday[*lmon] + leap;

	/*             Return */

L_900:
	return;

L_9000:
    flag = 36;
	sherr( &flag );
	*status = 1;
	return;
/*
L_9010:
    flag = 22;
	sherr( &flag );
	*status = 1;
	return;
*/

}

