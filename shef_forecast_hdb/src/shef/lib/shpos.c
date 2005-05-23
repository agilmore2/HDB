/*Translated by FOR_C, v3.4.2 (-), on 04/06/95 at 13:56:21 */
/*FOR_C Options SET: c=2 com=u do=r44 ftn=ln6kk op=iv s=dvnk str=l sq=i */
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
         SUBROUTINE SHPOS(STATUS)    
  
       PURPOSE                                                    
         This routine decodes the positional fields of the SHEF
         format. It returns the positional data in the common block
         data.
  
       VERSION and UPDATES
         1.0    MAY 82   Geoffrey M. Bonnin
                Original Version
         1.1    AUG 84 
                Add seconds to time 
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
         2.1    OCT 95   DGB
                Allow an underscore '_' as part of the id.
         2.4    JAN 96 DGB
                Update struct t_data to include lcent (century).
         2.5    FEB 11, 96
                Make strucutural changes to accomodate century
                in the positional date field.
         2.6    OCT 19 97 DGB
                Fix Msource or source for .B type messages.
                Remove a couple of unused variables e.g. _l0.  
         2.7    DEC 21 97 DGB
                Add centflag..set to 1 if an explicit century is
                specified in the postitional data.
         2.8    JAN 17 98 DGB
                Add yearflag...set to 1 if explicit year is specified
                in positional data.
         2.9    JAN 07 99 dgb
                Add check if century is missing and year is 00
                set century to 20...even if century from system
                clock is 19.
         3.0    JAN 18 99 DGB
                Fix century/year overrides for certain cases when the 
                century/year is not explicitly stated in the positional data.
         3.1    APR 12 99 DGB
                Fix obscure error of identifying the source for a .BR
                message.

 *--------------------------------------------------------------------- */


void shpos(status)
short int *status;
{
	static short int i, itest, lchar, nchar, ndig, nflag, num;
        static short int total_ndig;     /* dgb:021196 add */

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Idate = &datim_.idate[0] - 1;
	short *const Idstn = &data_.idstn[0] - 1;
	short *const Msource = &data_.msource[0] - 1;
	*status = 0;
	data_.lhour = 12;	/*12 zulu default */
	data_.lmin = 0;
	data_.lsec = 0;
	data_.nadjtz = 0;
	Idstn[1] = codes_.iblnk;
	Idstn[2] = codes_.iblnk;
	Idstn[3] = codes_.iblnk;
	Idstn[4] = codes_.iblnk;
	Idstn[5] = codes_.iblnk;
	Idstn[6] = codes_.iblnk;
	Idstn[7] = codes_.iblnk;
	Idstn[8] = codes_.iblnk;
        data_.posovercentury = -1;                          /* dgb:01/18/99 */
        data_.posoveryear    = -1;                          /* dgb:01/18/99 */

	/*             Get the station Source, there should be a blank */

	if( xchar_.ichar != codes_.iblnk )
		goto L_260;
L_10:
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	if( xchar_.ichar == codes_.iblnk )
		goto L_10;

	/*             Read up to 8 characters , checking for legit ones */

	nchar = 0;

	for( i = 1; i <= 8; i++ )
	{

		/*        CALL IRANG(ICHAR,ICHA,ICHZ,*15)        ! CHECK IF A-Z */
		irang( &xchar_.ichar, &codes_.icha, &codes_.ichz, status );		/* CHECK IF A-Z */
		if( *status == 1 )
			{
			*status = 0;
			goto L_15;
			}
		goto L_17;

		/*  15    CALL IRANG(ICHAR,ICH0,ICH9,*250)       ! CHECK IF 0-9 */
L_15:
		irang( &xchar_.ichar, &codes_.ich0, &codes_.ich9, status );		/* CHECK IF 0-9 */
		if( *status == 1 )
			{
			*status = 0;
			goto L_16;
			}
        goto L_17;

L_16:   
        if ( xchar_.ichar == codes_.iscore )               /* dgb: 10/18/95 DGB */
             goto L_17;
        else
             goto L_250;

L_17:

        if ( format_.iform == 3 || format_.iform == 4 )    /* dgb:04/12/99 */
             Msource[i] = xchar_.ichar;                    /* dgb:10/19/97 */
        else                                               /* dgb:10/19/97 */ 
            Idstn[i] = xchar_.ichar;                       /* dgb:10/19/97 */

		nchar = nchar + 1;
		nextch( &xchar_.ichar, status );
		if( *status == 1 )
			{
			*status = 0;
			goto L_9000;
			}
		else if( *status == 2 )
			{
			*status = 0;
			goto L_9010;
			}
		if( xchar_.ichar == codes_.iblnk )
			goto L_30;
	}

	if( xchar_.ichar != codes_.iblnk )
		goto L_260;

L_30:
	if( nchar < 3 )
		goto L_220;
	goto L_50;

	/*             Find the data group */

L_40:
	if( xchar_.ichar != codes_.iblnk )
		goto L_60;
L_50:
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	goto L_40;

	/*      Decode the date group      */
	/*      Get the first two digits   */

L_60:
	ndig = 2;
	shint( &data_.lcent, &ndig, 0, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_240;
		}
    total_ndig = 2;
    data_.centflag = 1;                                       /* dgb:12/21/97 */

    /*      Get the next two digits */
	ndig = 2;
	shint( &data_.lyear, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_240;
		}
    total_ndig = total_ndig + 2;

	/*      Get the next two digits */

	ndig = 2;
	shint( &data_.lmon, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
        nflag = 0;
        goto L_75;
		}
    total_ndig = total_ndig + 2;

	/*      Get the last two digits */

    ndig = 2;
	shint( &data_.lday, &ndig, 1, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
        nflag = 0;
		goto L_75;
		}
    total_ndig = total_ndig + 2;
	nflag = 1;

	/*      if 8 digits decode -> CCYRMMDD */
    /*      If 6 digits decode -> YRMMDD   */
    /*      If 4 digits decode -> MMDD     */
    /*      any other combination -> error */

L_75:
     if ( total_ndig == 8 )
        {
        data_.lyear = data_.lyear;
        data_.lmon  = data_.lmon;
        data_.lday  = data_.lday;
        }
     else
     if ( total_ndig == 6 )
        {
        data_.lday = data_.lmon;
        data_.lmon = data_.lyear;
        data_.lyear = data_.lcent;
        data_.lcent = -1;
        data_.posovercentury = -1;                          /* dgb:01/18/99 */
        }
     else
     if ( total_ndig == 4 )
        {
        data_.lmon = data_.lcent;
        data_.lday = data_.lyear;
        data_.lcent = -1;
        data_.lyear = -1;
        data_.posovercentury = -1;                          /* dgb:01/18/99 */
        data_.posoveryear    = -1;                          /* dgb:01/18/99 */
        }
     else
        goto L_240;

	/*     Now default/compute the century and year if needed */
    
    if ( data_.lcent == -1 )
    {
         if ( data_.lyear >= 0  )                          /* dgb:01/07/99 */
         {
           if ( data_.lyear <= 30 )
              data_.lcent = 20;                            /* dgb:01/07/99 */
           else
              data_.lcent = 19;
         }
         data_.centflag = -1;                              /* dgb:12/21/97 */
    } 
    else
    {
         data_.lcent = data_.lcent;
         data_.centflag = 1;                               /* dgb:12/21/97 */
    }

    data_.yearflag = 1;                                    /* dgb:01/17/98 */
    if ( data_.lyear == -1 )
    {
      data_.yearflag = -1;                                 /* dgb: 01/18/98 */
/*	  data_.lyear = Idate[3] - ( data_.lcent * 100 ); */
      data_.lyear = Idate[3] - ( (int) (Idate[3]/100) ) * 100;

      if ( data_.lyear >= 0  )                          
      {
           if ( data_.lyear <= 30 )
              data_.lcent = 20;                            
           else
              data_.lcent = 19;
      }
      data_.lyear = (data_.lcent * 100) + data_.lyear;
    
	  itest = data_.lmon - Idate[1];
	  if( itest > 6 )
	    	data_.lyear = data_.lyear - 1;
	  if( itest < -6 )
	    	data_.lyear = data_.lyear + 1;
	  if( (itest == -6) && (data_.lday < Idate[2]) )
	    	data_.lyear = data_.lyear + 1;
	  if( (itest == 6) && (data_.lday > Idate[2]) )
	    	data_.lyear = data_.lyear - 1;
    }
    else
      data_.lyear = ( data_.lcent * 100 ) + data_.lyear;   /* dgb:01/18/99 */

	/*             Test for bad date  */

/* L_77: */

	shtdat( &data_.lyear, &data_.lmon, &data_.lday, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_240;
		}

	/*             Find the time zone group (OPTIONAL) */

	if( nflag == 0 )
		goto L_78;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
L_78:
	if( xchar_.ichar != codes_.iblnk )
		goto L_260;
L_80:
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	if( xchar_.ichar == codes_.iblnk )
		goto L_80;
	if( xchar_.ichar != codes_.ichz )
		goto L_95;	/* ZULU SPECIFIED */
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}


	goto L_205;

	/*             Get the actual time zone */

L_95:
	if( xchar_.ichar != codes_.ichc )
		goto L_100;	/* CENTRAL TIME */
	data_.nadjtz = 360;
	goto L_180;

L_100:
	if( xchar_.ichar != codes_.iche )
		goto L_110;	/* EASTERN TIME */
	data_.nadjtz = 300;
	goto L_180;

L_110:
	if( xchar_.ichar != codes_.ichm )
		goto L_120;	/* MOUNTAIN TIME */
	data_.nadjtz = 420;
	goto L_180;

L_120:
	if( xchar_.ichar != codes_.icha )
		goto L_130;	/* ATLANTIC TIME */
	data_.nadjtz = 240;
	goto L_180;

L_130:
	if( xchar_.ichar != codes_.ichp )
		goto L_140;	/* PACIFIC TIME */
	data_.nadjtz = 480;
	goto L_180;

L_140:
	if( xchar_.ichar != codes_.ichl )
		goto L_150;	/* ALASKAN TIME */
	data_.nadjtz = 600;
	goto L_180;

L_150:
	if( xchar_.ichar != codes_.ichy )
		goto L_160;	/* YUKON TIME */
	data_.nadjtz = 540;
	goto L_180;

L_160:
	if( xchar_.ichar != codes_.ichh )
		goto L_170;	/* HAWAIIAN TIME */
	data_.nadjtz = 600;
	goto L_180;

L_170:
	if( xchar_.ichar != codes_.ichb )
		goto L_175;	/* BERING TIME */
	data_.nadjtz = 660;
	goto L_180;

L_175:
	if( xchar_.ichar != codes_.ichn )
		goto L_210;	/* NEWFOUNDLAND TIME */
	data_.nadjtz = 210;

	/*             Now check for daylight or standard time */

L_180:
	lchar = xchar_.ichar;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	if( xchar_.ichar != codes_.iblnk )
		goto L_185;
	if( (lchar == codes_.ichn) || (lchar == codes_.ichh) )
		goto L_200;
	data_.nadjtz = -data_.nadjtz;
	goto L_200;

L_185:
	if( xchar_.ichar == codes_.ichs )
		goto L_195;
	if( xchar_.ichar != codes_.ichd )
		goto L_230;
	if( (lchar == codes_.ichn) || (lchar == codes_.ichh) )
		goto L_195;
	data_.nadjtz = data_.nadjtz - 60;

L_195:
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	/*             Now default to 0000 */

L_200:
	data_.lhour = 24;

L_205:
	if( xchar_.ichar != codes_.iblnk )
		goto L_260;

	/*             Return options */

L_210:
	return;

L_220:
	num = 3;	/*LESS THAN 3 CHARS IN ID OR MSG SOURCE */
	goto L_300;

L_230:
	num = 4;	/*TZ CODE ERROR */
	goto L_300;

L_240:
	num = 5;	/* DATE GROUP ERROR */
	goto L_300;

L_250:
	num = 6;	/* BAD CHAR IN MSG SOURCE OR STN ID */
	goto L_300;

L_260:
	num = 2;

L_300:
	sherr( &num );
	format_.nerr = 1;

L_9000:
	*status = 1;
	return;

	/*             File read error */

L_9010:
	*status = 2;
	return;

}

