/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 17:00:46 */
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
         SUBROUTINE SHDTYPE(IFLAG,STATUS)
  
       PURPOSE                                                    
  
         This modual decodes the date and data type elements.
         IFLAG is set to:   1 FOR .A FORMAT
                            2     .B
                            3     .E
                            4     .B WITHIN THE BODY
         The routine will take the standard return if the 
         element is not valid.
  
       VERSION and UPDATES
         1.0    JUN 82    GEOFFREY M BONNIN  MBRFC      
                Original Version
         1.1    JAN 84                        
                Zero date relative on explicit date             
         1.2    AUG 84                                          
                Allow negative DI increments                    
                Add seconds to times                            
         1.3    FEB 87
                Fix bug with date relative in .B body
         2.0    JUN 94   David G. Brandon
                Also Translated to 'C' using FOR_C
                Allow for years 2000 thru 2020
                Removed alternate returns in subroutines
         2.1    Add iscore to t_codes array.
         2.2    NOV 95   DGB
             	Allow for all valid qualifier codes excepting for
            	'O' and 'I'.
         2.3    FEB 11 96 DGB
                Modify code to accomodate century.
         2.4    SEP 10 97 DGB
                Add the use of MAX_SHEF_INPUT so that the input line can
                be longer than 80 characters.
                Check for variable duration, and keep duration code
                as 'V'.
        2.5     JAN 17 98 DGB
                Fix increments of NULL values in .E type messages.  
                These messages are flagged with *flag = 3.  
                Previously, the NULL would be read, but the time 
                would not be incremented.
        2.6     JAN 19 98 DGB
                Previously a julian day with only two days specified 
                did not work.  E.G.  DJ09 ( although DJ009 did work ).
                This was fixed.
        2.7     JAN 18 99 DGB
                Fix century/year overrides for certain cases when the 
                century/year is not explicitly stated in the positional data.
                
 *--------------------------------------------------------------------- */


void shdtype(iflag, status)
short int *iflag, *status;
{
	static short int itest, jchar, jday, jflag, jsign, leap, 
	                 ndig, num, numa, total_ndig, k;
    short int buffer_temp[MAX_SHEF_INPUT];                 /* dgb:09/10/97 dgb:02/01/96 */
    char  tempc[4];                                        /* dgb:02/01/96 */
    int DEBUG1, DEBUG2;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Idate = &datim_.idate[0] - 1;

	short *const Idstn = &data_.idstn[0] - 1;
	short *const Msource = &data_.msource[0] - 1;

    DEBUG1 = 0;
    DEBUG2 = 0;

    if ( DEBUG1 ) printf("\nshdtype:begin *******************************");
    /* copy existing char buffer into temp buffer */
      for ( ndig=0; ndig < MAX_SHEF_INPUT; ndig++)         /* dgb:09/10/97 */
          buffer_temp[ndig] = buffer_.ibuf[ndig];

	*status = 0;
	dotbee_.nelem = 0;

	/*             Look for the "D" of data and date type elements */

L_20:
    if ( DEBUG1 ) printf("\nshdtype:at 20");
	if( xchar_.ichar != codes_.iblnk )
		goto L_30;
L_25:
    if ( DEBUG1 ) printf("\nshdtype:at 25");
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	goto L_20;

L_30:
    if ( DEBUG1 ) printf("\nshdtype:at 30  iflag = %d",*iflag);
	if( xchar_.ichar == codes_.ichd )
		goto L_35;
	if( xchar_.ichar != codes_.islash )
		goto L_600;
	if( *iflag == 4 )
		goto L_600;
	goto L_25;

	/*             Check for the element possibilities */

L_35:
    if ( DEBUG1 ) printf("\nshdtype:at 35");
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	ndig = 2;
	dotbee_.nelem = 1;


	/*             DS - Second */

    if ( DEBUG1 ) printf("\nshdtype:getting DS");
	if( xchar_.ichar != codes_.ichs )
		goto L_37;

	shint( &data_.lsec, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9030;
		}
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9030;
		}
	if( data_.lsec > 59 )
    {
		goto L_9030;
    }
	if( data_.lsec < 0 )
    {
		goto L_9030;
    }
	if( data_.lhour == 24 )
		goto L_36;
	goto L_125;

L_36:
	if( (data_.lmin > 0) || (data_.lsec > 0) )
    {
		goto L_9030;
    }
	goto L_125;

	/*             DN - Minute */

L_37:
    if ( DEBUG1 ) printf("\nshdtype:getting DN");
	if( xchar_.ichar != codes_.ichn )
		goto L_40;
	shint( &data_.lmin, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9030;
		}
	if( data_.lmin > 59 )
    {
		goto L_9030;
    }
	if( data_.lmin < 0 )
    {
		goto L_9030;
    }
	if( data_.lhour == 24 )
		goto L_38;
	goto L_115;

L_38:
	if( (data_.lmin > 0) || (data_.lsec > 0) )
    {
		goto L_9030;
    }
	goto L_115;

	/*             DH - Hour */

L_40:
	if( xchar_.ichar != codes_.ichh )
		goto L_50;
	shint( &data_.lhour, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9030;
		}
	if( data_.lhour > 24 )
    {
		goto L_9030;
    }
	if( data_.lhour < 0 )
    {
		goto L_9030;
    }
	if( data_.lhour == 24 )
		goto L_41;
	goto L_110;

L_41:
	if( (data_.lmin > 0) || (data_.lsec > 0) )
    {
		goto L_9030;
    }
	goto L_110;

	/*             DD - Day */

L_50:
    if ( DEBUG1 ) printf("\nshdtype:getting DD");
	if( xchar_.ichar != codes_.ichd )
		goto L_60;
	shint( &data_.lday, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9030;
		}
	if( data_.lday > 31 )
    {
		goto L_9030;
    }
	if( data_.lday < 0 )
    {
		goto L_9030;
    }
	goto L_100;

	/*             DM - Month */

L_60:
    if ( DEBUG1 ) printf("\nshdtype:getting DM");
	if( xchar_.ichar != codes_.ichm )
		goto L_70;
	shint( &data_.lmon, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9030;
		}
	if( data_.lmon > 12 )
    {
		goto L_9030;
    }
	if( data_.lmon < 1 )
    {
		goto L_9030;
    }
    data_.overmonth = data_.lmon;                          /* dgb:01/18/99 */
	goto L_90;

	/*             DY - Year */

L_70:
    if ( DEBUG1 ) printf("\nshdtype:getting DY");
	if( xchar_.ichar != codes_.ichy )
		goto L_80;
	shint( &data_.lyear, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9030;
		}
	if( data_.lyear < 0 )
    {
		goto L_9030;
    } 
    if ( *iflag == 2 )
      data_.posoveryear = data_.lyear;                     /* dgb:01/18/99 */
    data_.overyear = data_.lyear;                          /* dgb:01/18/99 */ 
    data_.lyear = (data_.lcent * 100) + data_.lyear;

    goto L_85;
  


	/*             DT - Century */

L_80:
    if ( DEBUG1 ) printf("\nshdtype:getting DT");
	if( xchar_.ichar != codes_.icht )                      /* dgb:09/10/97 */
		goto L_130;
	shint( &data_.lcent, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_120;
		}
	if( data_.lcent > 30 )
    {
		goto L_9030;
    }
	if( data_.lcent < 17 )
    {
		goto L_9030;
    }
    if ( *iflag == 2 )                                     /* dgb:01/18/99 */
    {
      data_.posovercentury = data_.lcent;
      data_.posoveryear = data_.lyear;
    }
    data_.overyear = data_.lyear;                          /* dgb:01/18/99 */
    data_.overcentury = data_.lcent;                        /* dgb:01/1899 */
   
    data_.lyear = 
    ( data_.lcent * 100 ) + (data_.lyear -  (( (int) (data_.lyear/100))*100)); /* dgb:01/18/99 */
	/*             DY - Year */


    if ( DEBUG1 ) printf("\nshdtype: DY");
	shint( &num, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_120;
		}

    data_.lyear = (data_.lcent * 100) + num;
 
L_85:
    if ( DEBUG1 ) printf("\nshdtype:at 85");
	shint( &num, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_120;
		}

	data_.lmon = num;
	if( data_.lmon > 12 )
    {
		goto L_9030;
    }
	if( data_.lmon < 1 )
    {
		goto L_9030;
    }

L_90:
    if ( DEBUG1 ) printf("\nshdtype:at 90");
	shint( &num, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_120;
		}

	data_.lday = num;
	if( data_.lday > 31 )
    {
		goto L_9030;
    }
	if( data_.lday < 0 )
    {
		goto L_9030;
}

L_100:
    if ( DEBUG1 ) printf("\nshdtype:at 100");
	shint( &num, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_120;
		}

	data_.lhour = num;
	if( data_.lhour > 24 )
		goto L_9030;
	if( data_.lhour < 0 )
		goto L_9030;

L_110:
    if ( DEBUG1 ) printf("\nshdtype:at 110");
	shint( &num, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_120;
		}

	data_.lmin = num;
	if( data_.lmin > 59 )
		goto L_9030;
	if( data_.lmin < 0 )
		goto L_9030;
	if( (data_.lhour == 24) && (data_.lmin > 0) )
		goto L_9030;

L_115:
    if ( DEBUG1 ) printf("\nshdtype:at 115");
	shint( &num, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_120;
		}

	data_.lsec = num;
	if( data_.lsec > 59 )
		goto L_9030;
	if( data_.lsec < 0 )
		goto L_9030;
	if( data_.lhour == 24 )
		goto L_116;
	goto L_125;

L_116:
    if ( DEBUG1 ) printf("\nshdtype:at 116");
	if( (data_.lmin > 0) || (data_.lsec > 0) )
		goto L_9030;
	goto L_125;

L_120:
    if ( DEBUG1 ) printf("\nshdtype:at 120");
	if( data_.lhour == 24 )
		goto L_121;
	if( ndig != 0 )
		goto L_9020;
	goto L_122;

L_121:
    if ( DEBUG1 ) printf("\nshdtype:at 121");
	if( (data_.lmin > 0) || (data_.lsec > 0) )
		goto L_9030;


	if( ndig != 0 )
		goto L_9020;

	/*             Zero date relatives */

L_122:
    if ( DEBUG1 ) printf("\nshdtype:at 122");
	if( *iflag == 4 )
		goto L_510;
    datrel_.mcent = 0;                                     /* dgb cent */
	datrel_.myear = 0;
	datrel_.mmon = 0;
	datrel_.mday = 0;
	datrel_.mhour = 0;
	datrel_.mmin = 0;
	datrel_.msec = 0;
	datrel_.mend = 0;
	goto L_510;

L_125:
    if ( DEBUG1 ) printf("\nshdtype:at 125  iflag = %d",*iflag);

	if( *iflag == 4 )
		goto L_500;
    datrel_.mcent = 0;                                     /* dgb cent */
	datrel_.myear = 0;
	datrel_.mmon = 0;
	datrel_.mday = 0;
	datrel_.mhour = 0;
	datrel_.mmin = 0;
	datrel_.msec = 0;
	datrel_.mend = 0;
	goto L_500;


	/*      Decode the DC group        */
	/*      Get the first two digits   */

L_130:

    /*       DC Century      */
    if ( xchar_.ichar != codes_.ichc )
         goto L_150;
	ndig = 2;
	shint( &data_.kcent, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9020;
		}
    total_ndig = 2;
    data_.dccentflag = 1;                                    /* dgb:12/24/96 */


    /*       DC year      */
    /*      Get the next two digits */
	ndig = 2;
	shint( &data_.kyear, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
        jflag = 0;
		goto L_75;
		}
    total_ndig = total_ndig + 2;

	/*      Get the next two digits */
    /*       DC Month      */

	ndig = 2;
	shint( &data_.kmon, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
        jflag = 0;
        goto L_75;
		}
    total_ndig = total_ndig + 2;

	/*      Get the next two digits */
    /*       DC Day      */
	ndig = 2;
	shint( &data_.kday, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
        jflag = 0;
        goto L_75;
		}
    total_ndig = total_ndig + 2;

	/*      Get the next two digits */
    /*       DC Hour      */

	ndig = 2;
	shint( &data_.khour, &ndig, 1, status );

	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
        jflag = 0;
        goto L_75;
		}
    total_ndig = total_ndig + 2;

	/*      Get the last two digits */
    /*       DC Minute      */

    ndig = 2;
	shint( &data_.kmin, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
        jflag = 0;
		goto L_75;
		}
    total_ndig = total_ndig + 2;
	jflag = 1;

    /*      if 12 digits decode->    CCYRMMDDHRMN */
    /*      id 10 digits decode ->   YRMMDDHRMN */ 
	/*      if 8 digits decode ->    MMDDHRMN */
    /*      If 6 digits decode ->    MMDDHR   */
    /*      If 4 digits decode ->    MMDD     */
    /*      any other combination -> error */

L_75:
    if ( DEBUG1 ) printf("\nshdtype:at 75");

     if ( DEBUG2 ) printf("\nshdtype: at L_75\n   total_ndig = %d\n  data_.kcent=%d\n   data_.kyear=%d\n   data_.kmon=%d\n   data_.kday=%d\n   data_.khour=%d\n   data_.kmin=%d",
          total_ndig,data_.kcent,data_.kyear,data_.kmon, data_.kday, data_.khour,data_.kmin);   
     if ( total_ndig == 12 )
     {}
     else
     if ( total_ndig == 10 )
        {
        data_.kmin  = data_.khour;
        data_.khour = data_.kday;
        data_.kday  = data_.kmon;
        data_.kmon  = data_.kyear;
        data_.kyear = data_.kcent;
        data_.kcent  = -1;
        }
     else
     if ( total_ndig == 8 )
        {
        data_.kmin  = data_.kday;
        data_.khour = data_.kmon;
        data_.kday  = data_.kyear;
        data_.kmon   = data_.kcent;
        data_.kyear = -1;
        data_.kcent = -1;
        }
     else
     if ( total_ndig == 6 )
        {
        data_.khour = data_.kmon;
        data_.kday = data_.kyear;
        data_.kmon = data_.kcent;
        data_.kyear = -1;
        data_.kcent = -1;
        data_.kmin  = -1;
        }
     else
     if ( total_ndig == 4 )
        {
        data_.kday = data_.kyear;
        data_.kmon = data_.kcent;
        data_.khour = -1;
        data_.kcent = -1;
        data_.kyear = -1;
        data_.kmin  = -1;
        }
     else
        goto L_240;


	/*     Now get defaults */
    
    if ( data_.kmin == -1 )
         data_.kmin = 0;
    if ( data_.khour == -1 )
         data_.khour = 0;


    if ( data_.kcent == -1 )
    {
         if ( data_.kyear >= 0  )                          /* dgb:01/07/99 */
         {
           if ( data_.kyear <= 30 )
              data_.kcent = 20;                            /* dgb:01/07/99 */
           else
              data_.kcent = 19;
         }
         data_.dccentflag = -1;                              /* dgb:12/21/97 */
    } 
    else
    {
         data_.kcent = data_.kcent;
         data_.dccentflag = 1;                               /* dgb:12/21/97 */
    }

    if ( data_.kyear == -1 )
    {
      data_.kyear = Idate[3] - ( (int) (Idate[3]/100)) * 100;
      if ( data_.kyear >= 0 )
      {
         if ( data_.kyear <= 30 )
              data_.kcent = 20;
         else
              data_.kcent = 19;
      }
      data_.kyear = ( data_.kcent *100) + data_.kyear;
	  itest = data_.kmon - Idate[1];
	  if( itest > 6 )
	    	data_.kyear = data_.kyear - 1;
	  if( itest < -6 )
	    	data_.kyear = data_.kyear + 1;
	  if( (itest == -6) && (data_.kday < Idate[2]) )
	    	data_.kyear = data_.kyear + 1;
	  if( (itest == 6) && (data_.kday > Idate[2]) )
	    	data_.kyear = data_.kyear - 1;
    }
    else
      data_.kyear = ( data_.kcent *100) + data_.kyear;


/*   
    if ( data_.khour == 1 )
         data_.khour = 0;
*/

     if ( DEBUG2 ) printf("\nshdtype: at L_75\n   data_.kcent=%d\n   data_.kyear=%d\n   data_.kmon=%d\n   data_.kday=%d\n   data_.khour=%d\n   data_.kmin=%d",
          data_.kcent,data_.kyear,data_.kmon, data_.kday, data_.khour,data_.kmin);   


	/*             Test for bad date */

/* L_147: */
	shtdat( &data_.kyear, &data_.kmon, &data_.kday, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9020;
		}
	if( (data_.khour < 0) || (data_.khour > 24) )
		goto L_9020;
	if( (data_.kmin < 0) || (data_.kmin > 59) )
		goto L_9020;
	if( (data_.khour == 24) && (data_.kmin > 0) )
		goto L_9020;
	if( jflag == 0 )
		goto L_510;
	if( ndig == 0 )
		goto L_510;
	goto L_500;

	/*             DJ - Julian date */

L_150:
    if ( DEBUG1 ) printf("\nshdtype:at 150");
    if ( DEBUG2 ) printf("\nshdtype: xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	if( xchar_.ichar != codes_.ichj )
		goto L_180;
    total_ndig = 0;
    k = buffer_.ip;

L_151:
    if ( DEBUG1 ) printf("\nshdtype:at 151");
    /* count the digits */
    irang(&buffer_temp[k],&codes_.ich0,&codes_.ich9,status);
    if ( *status == 1 )
          goto L_155;
    total_ndig = total_ndig + 1;
    k = k + 1; 
    goto L_151;

L_155:
    if ( DEBUG1 ) printf("\nshdtype:at 155");
    if ( DEBUG2 ) printf("\nshdtype: total_ndig = %d",total_ndig);
    /* look for 7 digits 'CCYRDDD' or 5 digits 'YRDDD' or 3 digits 'DDD' */
    /* or 2 digits DD */

    if ( total_ndig != 7 && total_ndig != 5 && total_ndig != 3 
          && total_ndig != 2 )
         goto L_9020;

    if ( total_ndig == 7 ) 
    {
       /* calculate 'CC' */
       tempc[0] = buffer_temp[buffer_.ip];
       tempc[1] = buffer_temp[buffer_.ip+1];
       tempc[2] = 0;
       data_.lcent = atoi(tempc);

       /* calculate 'YR' */
       tempc[0] = buffer_temp[buffer_.ip+2];
       tempc[1] = buffer_temp[buffer_.ip+3];
       tempc[2] = 0;
       data_.lyear = atoi(tempc);
       data_.lyear = (data_.lcent * 100) + data_.lyear;    /* dgb:01/18/99 */

       /* calculate 'DDD' */
       tempc[0] = buffer_temp[buffer_.ip+4];
       tempc[1] = buffer_temp[buffer_.ip+5];
       tempc[2] = buffer_temp[buffer_.ip+6];
       tempc[3] = 0;
       jday     = atoi(tempc);
       buffer_.ip = buffer_.ip + 7;
    }
    else
    if ( total_ndig == 5 )
    {
       /* calculate 'YR' */
       tempc[0] = buffer_temp[buffer_.ip];
       tempc[1] = buffer_temp[buffer_.ip+1];
       tempc[2] = 0;
       data_.lyear = atoi(tempc);
       if ( data_.lyear <= 30 ) data_.lyear = data_.lyear + 2000; /* dgb:01/18/99 */
       else
       if ( data_.lyear > 29 ) data_.lyear = data_.lyear + 1900;  /* dgb:01/18/99 */

       /* calculate 'DDD' */
       tempc[0] = buffer_temp[buffer_.ip+2];
       tempc[1] = buffer_temp[buffer_.ip+3];
       tempc[2] = buffer_temp[buffer_.ip+4];
       tempc[3] = 0;
       jday     = atoi(tempc);
       buffer_.ip = buffer_.ip + 5;
    }
    else
    if ( total_ndig == 3 )
    {
       /* calculate 'DDD' */
       tempc[0] = buffer_temp[buffer_.ip];
       tempc[1] = buffer_temp[buffer_.ip+1];
       tempc[2] = buffer_temp[buffer_.ip+2];
       tempc[3] = 0;
       jday     = atoi(tempc);
       buffer_.ip = buffer_.ip + 3;

    }
    else                                                   /* dgb:01/19/98 */ 
    if ( total_ndig == 2 )                                 /* dgb:01/19/98 */
    {                                                      /* dgb:01/19/98 */
       /* calculate 'DD' */                                /* dgb:01/19/98 */
       tempc[0] = buffer_temp[buffer_.ip];                 /* dgb:01/19/98 */
       tempc[1] = buffer_temp[buffer_.ip+1];               /* dgb:01/19/98 */
       tempc[2] = 0;                                       /* dgb:01/19/98 */
       tempc[3] = 0;                                       /* dgb:01/19/98 */  
       jday     = atoi(tempc);                             /* dgb:01/19/98 */
       buffer_.ip = buffer_.ip + 2;                        /* dgb:01/19/98 */ 
    }              



	/*             Convert julian day of LYEAR To month, day */

/* L_170: */
    if ( DEBUG1 ) printf("\nshdtype:at 170");
	shleap( &data_.lyear, &leap );
	if( jday > 366 )
		goto L_9050;
	if( (leap == 0) && (jday > 365) )
		goto L_9050;

	shcal( &jday, &leap, &data_.lmon, &data_.lday );
	if( ndig != 2 )
		goto L_510;
	goto L_500;

	/*             DQ - Data qualifier */

L_180:
    if ( DEBUG1 ) printf("\nshdtype:at 180");
	if( xchar_.ichar != codes_.ichq )
		goto L_200;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	/* pass all alphabetical characters except 'O' and 'I'   dgb:11/13/95 */
	irang(&xchar_.ichar,&codes_.icha,&codes_.ichz,status);

	if ( !*status )
	{
	   if ( xchar_.ichar == codes_.icho )  goto L_9060;
	   if ( xchar_.ichar == codes_.ichi )  goto L_9060;
	   *status = 0;
	   goto L_190;
	}

	goto L_9060;

L_190:
	data_.kwal = xchar_.ichar;
	goto L_500;

	/*             DU - Units code */

L_200:
    if ( DEBUG1 ) printf("\nshdtype:at 200");
	if( xchar_.ichar != codes_.ichu )
		goto L_220;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	if( xchar_.ichar != codes_.ichs )
		goto L_210;
	data_.kodu = 0;	/* STANDARD INTERNATIONAL UNITS */
	goto L_500;

L_210:
	if( xchar_.ichar != codes_.iche )
		goto L_9070;
	data_.kodu = 1;	/* ENGLISH UNITS (US SYSTEM) */
	goto L_500;

	/*             DR - Date relative increment */

L_220:
    if ( DEBUG1 ) printf("\nshdtype:at 220");

	if( xchar_.ichar != codes_.ichr )
		goto L_290;
    datrel_.mcent = 0;                                     /* dgb cent */
	datrel_.myear = 0;
	datrel_.mmon = 0;
	datrel_.mday = 0;
	datrel_.mhour = 0;
	datrel_.mmin = 0;
	datrel_.msec = 0;
	datrel_.mend = 0;
	nextch( &jchar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	nextch( &num, status );	/* SIGN ? */
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	jflag = 0;
	jsign = 1;
	if( num != codes_.iplus )
		goto L_230;
	jflag = 1;
	goto L_240;

L_230:
	if( num != codes_.iminus )
		goto L_9075;
	jsign = -1;
	jflag = 1;

L_240:
    if ( DEBUG1 ) printf("\nshdtype:at 240");
	ndig = 2;
	shint( &num, &ndig, jflag, status );	/* NUMBER ? */
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_245;
		}

L_245:
    if ( DEBUG1 ) printf("\nshdtype:at 245");
	if( ndig == 0 )
		goto L_9075;
	num = num*jsign;
	if( jchar != codes_.ichs )
		goto L_247;	/* SECONDS */
	datrel_.msec = num;
	if( ndig == 1 )
		goto L_510;
	goto L_500;

L_247:
    if ( DEBUG1 ) printf("\nshdtype:at 247");
	if( jchar != codes_.ichn )
		goto L_250;	/* MINUTES */
	datrel_.mmin = num;
	if( ndig == 1 )
		goto L_510;
	goto L_500;

L_250:
    if ( DEBUG1 ) printf("\nshdtype:at 250");
	if( jchar != codes_.ichh )
		goto L_260;	/* HOURS */
	datrel_.mhour = num;
	if( ndig == 1 )
		goto L_510;
	goto L_500;

L_260:
    if ( DEBUG1 ) printf("\nshdtype:at 260");
	if( jchar != codes_.ichd )
		goto L_270;	/* DAYS */
	datrel_.mday = num;
	if( ndig == 1 )
		goto L_510;
	goto L_500;

L_270:
    if ( DEBUG1 ) printf("\nshdtype:at 270");
	if( jchar != codes_.ichm )
		goto L_280;	/* MONTHS */
	datrel_.mmon = num;
	if( ndig == 1 )
		goto L_510;
	goto L_500;

L_280:    
if ( DEBUG1 ) printf("\nshdtype: at 280");

	if( jchar != codes_.ichy )
		goto L_282;	/* YEARS */
	datrel_.myear = num;
	if( ndig == 1 )
		goto L_510;
	goto L_500;

/* dgb add cent */
L_282:

    if ( jchar != codes_.ichz )
         goto L_285;
    datrel_.mcent = num;
    if ( ndig == 1 )
         goto L_510;
    goto L_500;

L_285:
    if ( DEBUG1 ) printf("\nshdtype:at 285");
	if( jchar != codes_.iche )
		goto L_9075;	/* END OF MONTH */
	datrel_.mend = num;
	if( ndig == 1 )
		goto L_510;
	goto L_500;

	/*             DV - Duration code */

L_290:

    if ( DEBUG1 ) printf("\nshdtype:at 290");
	if( xchar_.ichar != codes_.ichv )
		goto L_360;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	if( xchar_.ichar != codes_.ichs )
		goto L_295;
	numa = 7000;
	goto L_350;

L_295:
    if ( DEBUG1 ) printf("\nshdtype:at 295");
	if( xchar_.ichar != codes_.ichn )
		goto L_300;
	numa = 0;
	goto L_350;

L_300:
    if ( DEBUG1 ) printf("\nshdtype:at 300");
	if( xchar_.ichar != codes_.ichh )
		goto L_310;
	numa = 1000;
	goto L_350;

L_310:
    if ( DEBUG1 ) printf("\nshdtype:at 310");
	if( xchar_.ichar != codes_.ichd )
		goto L_320;
	numa = 2000;
	goto L_350;

L_320:
    if ( DEBUG1 ) printf("\nshdtype:at 320");
	if( xchar_.ichar != codes_.ichm )
		goto L_330;
	numa = 3000;
	goto L_350;

L_330:
    if ( DEBUG1 ) printf("\nshdtype:at 330");

	if( xchar_.ichar != codes_.ichy )
		goto L_340;
	numa = 4000;
	goto L_350;

L_340:
    if ( DEBUG1 ) printf("\nshdtype:at 340");
	if( xchar_.ichar != codes_.ichz )
		goto L_9095;
	data_.idcodd = 5000;
	goto L_500;

	/*             NOW GET THE DURATION */

L_350:
    if ( DEBUG1 ) printf("\nshdtype:at 350");
	ndig = 2;
	jflag = 1;
	shint( &num, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9095;
		}

	data_.idcodd = num + numa;
	goto L_500;

	/*             DI - Time interval for .E format */

L_360:

    if ( DEBUG1 ) printf("\nshdtype:360 getting DI for increnemtn iflag = %d",*iflag);
	if( *iflag != 3 )
		goto L_9080;
    
	if( xchar_.ichar != codes_.ichi )
		goto L_9080;
    
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	if( xchar_.ichar != codes_.ichs )
		goto L_365;
	numa = 6000;
    if ( DEBUG1 ) printf("\nshdtype:going to 410");
	goto L_410;

L_365:
    if ( DEBUG1 ) printf("\nshdtype:at 365");
	if( xchar_.ichar != codes_.ichn )
		goto L_370;
	numa = 0;
	goto L_410;

L_370:
    if ( DEBUG1 ) printf("\nshdtype:at 370");
	if( xchar_.ichar != codes_.ichh )
		goto L_380;
	numa = 1000;
	goto L_410;

L_380:
    if ( DEBUG1 ) printf("\nshdtype:at 380");
	if( xchar_.ichar != codes_.ichd )
		goto L_390;
	numa = 2000;
	goto L_410;

L_390:
    if ( DEBUG1 ) printf("\nshdtype:at 390");
	if( xchar_.ichar != codes_.ichm )
		goto L_400;
	numa = 3000;
	goto L_410;

L_400:
    if ( DEBUG1 ) printf("\nshdtype:at 400");
	if( xchar_.ichar != codes_.ichy )
		goto L_405;
	numa = 4000;
	goto L_410;

L_405:
    if ( DEBUG1 ) printf("\nshdtype:at 405");
	if( xchar_.ichar != codes_.iche )
		goto L_9085;
	numa = 5000;

	/*             Now get the interval */

L_410:
    if ( DEBUG1 ) printf("\nshdtype:at 410");

	ndig = 2;
	jflag = 0;
	jsign = 1;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	if( xchar_.ichar != codes_.iplus )
		goto L_412;
	jflag = 1;
	goto L_414;

L_412:
    if ( DEBUG1 ) printf("\nshdtype:at 412");
	if( xchar_.ichar != codes_.iminus )
		goto L_414;
	jsign = -1;
	jflag = 1;

L_414:

    if ( DEBUG1 ) printf("\nshdtype:at 414");
	shint( &num, &ndig, jflag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_420;
		}
	data_.itz = numa + num*jsign;
	goto L_500;

L_420:
    if ( DEBUG1 ) printf("\nshdtype:at 420");
	if( ndig == 0 )
		goto L_9085;
	data_.itz = numa + num*jsign;
	goto L_510;

	/*             Scan past blanks to the next slash */

L_500:
    if ( DEBUG1 ) printf("\nshdtype:at 500 dtype_.ieee = %d",dtype_.ieee);

	dtype_.ieee = 1;

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9120;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

L_510:
    if ( DEBUG1 ) printf("\nshdtype:510 dtype_.ieee = %d iflag = %d",dtype_.ieee,*iflag);
	dtype_.ieee = 1;
	if( *iflag == 4 )
		goto L_600;
	if( xchar_.ichar == codes_.iblnk )
		goto L_500;
	if( xchar_.ichar == codes_.islash )
		goto L_25;
	goto L_9090;

	/*             Not a date or data type element */

L_600:
    if ( DEBUG1 ) printf("\nshdtype:at 600 returning dtype_.ieee = %d iflag = %d",dtype_.ieee,*iflag);
	return;

	/*             Error returns */

L_9010:
	*status = 2;
	return;

L_9020:
	num = 7;
	goto L_700;

L_9030:
	num = 8;
	goto L_700;

L_9050:
	num = 10;
	goto L_700;

L_9060:
	num = 11;
	goto L_700;

L_9070:
	num = 12;
	goto L_700;

L_9075:
	num = 9;
	goto L_700;

L_9080:
	num = 13;
	goto L_700;

L_9085:
	num = 27;
	goto L_700;

L_9090:
	num = 14;
	goto L_700;

L_9095:
	num = 26;

L_700:
	sherr( &num );
	if( *iflag != 4 )
		format_.nerr = 1;

	*status = 1;
	return;

L_9120:
	*status = 3;
	return;

}
