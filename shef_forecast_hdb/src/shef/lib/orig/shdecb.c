/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 16:59:59 */
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
         SUBROUTINE SHDECB(IREV,STATUS)
  
       PURPOSE                                                    
         Decode SHEF .B Format
         IREV = 1 for Revised, IREV = 0 for New Data
  
       VERSION and UPDATES
         1.0    AUG 82   Geoffrey M. Bonnin
                Original Version
         1.1    JAN 84  
                Force Error for No .END at end of file
         1.2    AUG 84
                Add seconds to time
         1.3    NOV 84
                Virtual buffering of DOTBTEMP
         1.4    MAR 86
                Fix error in subsequent revision continuation
         2.0    JUN 94   David G. Brandon
                Also Translated to 'C' using FOR_C
                Removed alternate returns in subroutines
         2.1    Add iscore to t_codes array.
         2.2    JAN 24 96  DGB
                Add lcent to data_ structure to accomodate century.        
         2.3x   SEP 10 97 DGB
                Add the use of MAX_SHEF_INPUT so that the input line can
                be longer than 80 characters.
         2.4    JAN 18 99 DGB
                Add changes so that century and year can be determined
                correctly if there is no explicit century/year specified
                in the positional data, but overrides do occur.

 *--------------------------------------------------------------------- */

void shdecb(irev, status)
short int *irev, *status;
{
    int DEBUG1;
	static short int _i0, ibod, idote, iend_, lerr, num, flag;
	static double factor;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Idate = &datim_.idate[0] - 1; /* dgb cent */
	short *const Ibuf = &buffer_.ibuf[0] - 1;
	short *const Idstn = &data_.idstn[0] - 1;
	short *const Msource = &data_.msource[0] - 1;

    DEBUG1 = 0;
    if ( DEBUG1 ) printf("\nshdecb:begin");
	/*             Initialise */

	Msource[1] = codes_.iblnk;                             /* dgb:09/10/97 */
	Msource[2] = codes_.iblnk;                             /* dgb:09/10/97 */
	Msource[3] = codes_.iblnk;                             /* dgb:09/10/97 */
	Msource[4] = codes_.iblnk;                             /* dgb:09/10/97 */
	Msource[5] = codes_.iblnk;                             /* dgb:09/10/97 */
	Msource[6] = codes_.iblnk;                             /* dgb:09/10/97 */
	Msource[7] = codes_.iblnk;                             /* dgb:09/10/97 */
	Msource[8] = codes_.iblnk;                             /* dgb:09/10/97 */

	*status = 0;
	data_.itz = 0;
	idote = 0;
	lerr = 0;
	ibod = 0;

    /* get default century */
       data_.lcent = ( Idate[3]/100 ) ; /* dgb cent */ 

	/*             Check for Continuation */

	if( format_.iform > 0 )
		goto L_10;
	format_.iform = -format_.iform;
	if( (format_.lform != 3) && (format_.lform != 4) )
		goto L_9020;
	if( (format_.iform == 4) && (format_.lform == 3) )
		goto L_9030;
	if( (format_.iform == 3) && (format_.lform == 4) )
		*irev = 1;
	if( (format_.iform == 3) && (format_.lform == 4) )
		format_.iform = 4;
	if( format_.nerr > 0 )
		goto L_9040;
	goto L_30;

	/*             Defaults */

L_10:
        data_.kcent = 0; /* dgb cent */
	data_.kyear = 0;
	data_.kmon = 0;
	data_.kday = 0;
	data_.khour = 0;
	data_.kmin = 0;
	data_.ksec = 0;
	data_.kwal = codes_.ichz;
	data_.kodu = 1;
        datrel_.mcent = 0;  /* dgb cent */
	datrel_.myear = 0;
	datrel_.mmon = 0;
	datrel_.mday = 0;
	datrel_.mhour = 0;
	datrel_.mmin = 0;
	datrel_.msec = 0;
	datrel_.mend = 0;
	data_.idcodd = 5000;
	dotbee_.nerror = 0;
        luns_.mrec = 0;
	format_.nerr = 0;
	bbuf1_.nmrec = 0;

	/*             Get the Positional Data */


	shpos( status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9050;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	/*             Get the Date and Data Type Elements */

L_30:

    flag = 2;
	shdtype( &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_150;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_130;
		}

	/*             Get the Parameter Code */


	shpcode( &data_.kodp, &data_.kode, &data_.icodd, &data_.kodt, 
	 &data_.kods, &data_.kodex, &data_.codp, &factor, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_150;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_50;
		}
	iend_ = 0;
	if( xchar_.ichar == codes_.iblnk )
		goto L_90;
	if( xchar_.ichar == codes_.islash )
		goto L_90;
	goto L_9080;
L_50:
	iend_ = 1;

	/*             Write the Data to the Buffer */

L_90:
        if ( data_.posoveryear > -1 && data_.posovercentury == -1 ) /*dgb:01/18/99 */
        {
           if ( data_.posoveryear < 30 ) 
                data_.lyear = data_.posoveryear + 2000;
           else
                data_.lyear = data_.posoveryear + 1900;
           /*
           if ( data_.lcent == 19  && data_.lyear < 1930 )       
               data_.lyear = ( data_.lyear - ((int) data_.lyear/100 )*100) + 2000; 
           */
        }
        
    /* dgb add data_.lcent, data_.kcent, datrel_.mcent */
	shbfw( data_.msource, &data_.lcent,&data_.lyear, &data_.lmon, &data_.lday, 
	 &data_.lhour, &data_.lmin, &data_.lsec, &data_.kcent,&data_.kyear, &data_.kmon, 
	 &data_.kday, &data_.khour, &data_.kmin, &data_.ksec, &datrel_.mcent,&datrel_.myear, 
	 &datrel_.mmon, &datrel_.mday, &datrel_.mhour, &datrel_.mmin, 
	 &datrel_.msec, &datrel_.mend, &data_.kodp, &data_.kode, &data_.icodd, 
	 &data_.idcodd, &data_.kodt, &data_.kods, &data_.kodex, &data_.codp, 
	 &data_.kwal, &data_.nadjtz, &data_.kodu, &factor, irev, &durprob_.kodd, 
	 &durprob_.kodpr );

         data_.posoveryear    = -1;                         /* dgb:01/18/99 */
         data_.posovercentury = -1;                         /* dgb:01/18/99 */

         luns_.mrec = luns_.mrec + 1;
 	 if( iend_ == 1 )
		goto L_130;
	 goto L_120;

	/*             Clear Past the Next Slash */

L_110:

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_130;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
L_120:
	if( xchar_.ichar == codes_.iblnk )
		goto L_110;
	if( xchar_.ichar != codes_.islash )
		goto L_9080;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_130;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	goto L_30;

	/*             Check Next Line to See if it Should be Searched for Data */

L_125:

	buffer_.ip = MAX_SHEF_INPUT + 1;                            /* dgb:09/10/97 */

L_130:

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_130;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	if( xchar_.ichar != codes_.idot )
		goto L_140;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9060;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	if( xchar_.ichar != codes_.iche )
		goto L_135;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9060;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	if( xchar_.ichar != codes_.ichn )
		goto L_9070;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9060;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	if( xchar_.ichar != codes_.ichd )
		goto L_9070;
	goto L_900;

L_135:

	if( ibod == 1 )
		goto L_9070;
	if( xchar_.ichar != codes_.ichb )
		goto L_9070;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9060;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	irang( &xchar_.ichar, &codes_.ich1, &codes_.ich9, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_137;
		}
	goto L_139;

L_137:
	if( xchar_.ichar != codes_.ichr )
		goto L_9070;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9060;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	irang( &xchar_.ichar, &codes_.ich1, &codes_.ich9, &_i0 );
	switch( _i0 )
		{
		case 1: goto L_9070;
		}
	if( *status == 1 )
		{
		*status = 0;
		goto L_9070;
		}

L_139:
	buffer_.ip = 0;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9060;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	*status = 3;
	return;

L_140:
	ibod = 1;
	shdotb( status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_160;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	lerr = 0;
	goto L_130;

	/*             Error Checking */

L_150:
	dotbee_.nerror = dotbee_.nerror + 1;
L_160:
	if( lerr == 1 )
		goto L_9000;
	lerr = 1;
	if( dotbee_.nerror >= 3 )
		goto L_9000;
	goto L_125;


L_900:
	return;

	/*             Error Return - Stop Looking at this Format */

L_9000:
	*status = 1;
	return;

	/*             Error Return - Stop Looking at this File */

L_9010:
    flag = 34;
	sherr( &flag );
	*status = 2;
	return;

L_9020:
	num = 19;
	goto L_9200;

L_9030:
	num = 20;
	goto L_9200;

L_9040:
    flag = 21;
	sherr( &flag );
	goto L_125;

L_9050:
	format_.nerr = 1;
	goto L_9000;

L_9060:
	num = 34;
	goto L_9200;

L_9070:
	buffer_.ip = 0;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9060;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
    flag = 34;
	sherr( &flag );
	*status = 3;
	return;

L_9080:
    flag = 31;
	sherr( &flag );
	format_.nerr = 1;
	goto L_125;

L_9200:
	sherr( &num );
	goto L_9000;

}


