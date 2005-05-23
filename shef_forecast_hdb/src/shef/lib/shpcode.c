/*Translated by FOR_C, v3.4.2 (-), on 04/06/95 at 13:56:16 */
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
         SUBROUTINE SHPCODE(KODP,KODE,ICODD,KODT,KODS,KODEX,CODP,FACTOR,STATUS)
  
       PURPOSE                                                    
         This routine gets the parameter code, validates it
         and interprets it.
  
       VERSION and UPDATES
         1.0    JUL 82   Geoffrey M. Bonnin
                Original Version
         1.1    MAY 85 
                Allow for numeric source code 
         2.0    JUN 94   David G. Brandon
                Also Translated to 'C' using FOR_C
                PEDSTEP validation is now done in memory. There is 
                no longer disk IO with the shefparm file.
                Removed alternate returns to subroutines.
         2.1    OCT 95 DGB
                Allow for the 'T' in the PEDTSEP to be numeric.
                Change from fixed array sizes to values read out of the
                header file. Add iscore to t_codes array.
  
 *--------------------------------------------------------------------- */


void shpcode(kodp, kode, icodd, kodt, kods, kodex, codp, factor, status)
short int *kodp, *kode, *icodd, *kodt, *kods, *kodex;
float *codp;
double *factor;
short int *status;
{

	static short int iend_, inum, irec, ival, jchar, koddd, kodtt;
	static double value;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Durvalue = &parms2_.durvalue[0] - 1;
	short *const Exvalue = &parms2_.exvalue[0] - 1;
	short *const Numval = &parms2_.numval[0] - 1;
	double *const Pevalue = &parms3_.pevalue[0] - 1;
	double *const Provalue = &parms3_.provalue[0] - 1;
	short *const Sendflag = &parms2_.sendflag[0] - 1;
	short *const Tsvalue = &parms2_.tsvalue[0] - 1;

	/*             Set defaults */
	*status = 0;
	*kodp = codes_.iblnk;
	*kode = codes_.iblnk;
	durprob_.kodd = codes_.iblnk;
	koddd = codes_.iblnk;
	*kodt = codes_.iblnk;
	*kods = codes_.iblnk;
	*kodex = codes_.iblnk;
	durprob_.kodpr = codes_.iblnk;
	kodtt = codes_.iblnk;
	*icodd = 0;
	*codp = -1.0;
	iend_ = 0;
	sendflg_.nsflag = 0;


	/*             Search for a non blank character */

L_10:
	if( xchar_.ichar != codes_.iblnk )
		goto L_20;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9500;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	goto L_10;

	/*             Is it a letter ? */

L_20:

	irang( &xchar_.ichar, &codes_.icha, &codes_.ichz, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9020;
		}
	jchar = xchar_.ichar;

	/*             Get the second character of 'PE' */

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9500;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	irang( &xchar_.ichar, &codes_.icha, &codes_.ichz, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9020;
		}
	*kodp = jchar;
	*kode = xchar_.ichar;

	/*             Get the 'D' character */

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_65;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	if( xchar_.ichar == codes_.iblnk )
		goto L_70;
	if( xchar_.ichar == codes_.islash )
		goto L_70;
	irang( &xchar_.ichar, &codes_.icha, &codes_.ichz, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9020;
		}
	durprob_.kodd = xchar_.ichar;

	/*             Get the 'T' of the 'TS' code */

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_65;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	if( xchar_.ichar == codes_.iblnk )
		goto L_70;
	if( xchar_.ichar == codes_.islash )
		goto L_70;
	irang( &xchar_.ichar, &codes_.icha, &codes_.ichz, status );

        /* dgb:10/16/95  allow for 'T' to be numeric */
	if( *status == 1 )
		{
		*status = 0;
		irang( &xchar_.ichar, &codes_.ich1, &codes_.ich9, status );
                if( *status == 1 )
                     {
                     *status = 0;
		     goto L_9020;
		     }
		}
		
		
	*kodt = xchar_.ichar;

	/*             Get the 'S' of the 'TS' code */

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_65;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	if( xchar_.ichar == codes_.iblnk )
		goto L_70;
	if( xchar_.ichar == codes_.islash )
		goto L_70;
	irang( &xchar_.ichar, &codes_.icha, &codes_.ichz, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_30;
		}
	goto L_40;

L_30:
	irang( &xchar_.ichar, &codes_.ich1, &codes_.ich9, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9020;
		}
L_40:
	*kods = xchar_.ichar;

	/*             Get the 'E' of the extremum code */

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_65;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	if( xchar_.ichar == codes_.iblnk )
		goto L_70;
	if( xchar_.ichar == codes_.islash )
		goto L_70;
	irang( &xchar_.ichar, &codes_.icha, &codes_.ichz, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9020;
		}
	*kodex = xchar_.ichar;

	/*             Get the 'P' of the probability code */

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_65;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	if( xchar_.ichar == codes_.iblnk )
		goto L_70;
	if( xchar_.ichar == codes_.islash )
		goto L_70;
	irang( &xchar_.ichar, &codes_.icha, &codes_.ichz, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_50;
		}
	goto L_60;

L_50:
	irang( &xchar_.ichar, &codes_.ich1, &codes_.ich9, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9020;
		}
L_60:
	durprob_.kodpr = xchar_.ichar;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_65;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	goto L_70;

	/*             Validate the 'PE' code */

L_65:
	iend_ = 1;
L_70:
	inum = 1;
	checkparm( kodp, kode, &durprob_.kodd, kodt, kods, kodex, &durprob_.kodpr, 
	 factor, &ival, &inum );
	if( *factor < -1.5e0 )
		goto L_9040;

	/*             Check for send codes */

	inum = 6;
	checkparm( kodp, kode, &durprob_.kodd, kodt, kods, kodex, &durprob_.kodpr, 
	 &value, &irec, &inum );
	if( irec <= 0 )
		goto L_90;

	*kodp = parms2_.sendcodes[irec - 1][0];
	*kode = parms2_.sendcodes[irec - 1][1];
	koddd = parms2_.sendcodes[irec - 1][2];
	kodtt = parms2_.sendcodes[irec - 1][3];
	if( (durprob_.kodd != codes_.iblnk) && (kodtt != codes_.iblnk) )
		goto L_9050;
	if( durprob_.kodd != codes_.iblnk )
		goto L_90;

	durprob_.kodd = koddd;
	*kodt = kodtt;
	sendflg_.nsflag = 0;

	*kods = parms2_.sendcodes[irec - 1][4];
	*kodex = parms2_.sendcodes[irec - 1][5];
	durprob_.kodpr = parms2_.sendcodes[irec - 1][6];

	sendflg_.nsflag = Sendflag[irec];

	/*             Validate and Interprept the Duration Code */

L_90:
	if( durprob_.kodd == codes_.iblnk )
		goto L_140;
	if( durprob_.kodd != codes_.ichz )
		goto L_92;
	if( koddd == codes_.iblnk )
		goto L_95;
	durprob_.kodd = koddd;

L_92:
	inum = 2;
	checkparm( kodp, kode, &durprob_.kodd, kodt, kods, kodex, &durprob_.kodpr, 
	 &value, &ival, &inum );
	if( ival == -9 )
		goto L_9040;
	*icodd = ival;

	/*             Validate the 'TS' code */

L_95:
	if( *kodt == codes_.iblnk )
		goto L_140;
	if( *kods == codes_.iblnk )
		*kods = codes_.ichz;
	if( *kodt != codes_.ichz )
		goto L_98;
	if( *kods != codes_.ichz )
		goto L_9040;
	goto L_105;

L_98:
	irang( kods, &codes_.ich1, &codes_.ich9, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_99;
		}
	goto L_100;

L_99:
	inum = 3;
	checkparm( kodp, kode, &durprob_.kodd, kodt, kods, kodex, &durprob_.kodpr, 
	 &value, &ival, &inum );

L_100:
	if( ival < 0 )
		goto L_9040;

	/*             Validate the extremum code */

L_105:
	if( *kodex == codes_.iblnk )
		goto L_160;
	inum = 4;
	checkparm( kodp, kode, &durprob_.kodd, kodt, kods, kodex, &durprob_.kodpr, 
	 &value, &ival, &inum );
	if( ival < 0 )
		goto L_9040;

	/*             Validate and interpret the probability code */

	if( durprob_.kodpr == codes_.iblnk )
		goto L_9000;
	irang( &durprob_.kodpr, &codes_.icha, &codes_.ichz, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_110;
		}

L_110:
	inum = 5;
	checkparm( kodp, kode, &durprob_.kodd, kodt, kods, kodex, &durprob_.kodpr, 
	 &value, &ival, &inum );
	if( value < -8.0e0 )
		goto L_9040;
	*codp = value;
	goto L_9000;

	/*             Real defaults */

L_140:
	*kodt = codes_.ichr;
	*kods = codes_.ichz;
L_160:
	*kodex = codes_.ichz;

	/*             Got it - return */

L_9000:
	if( durprob_.kodd == codes_.iblnk )
		durprob_.kodd = codes_.ichi;
	if( durprob_.kodpr == codes_.iblnk )
		durprob_.kodpr = codes_.ichz;
	if( iend_ == 1 )
		goto L_9600;
	return;

	/*             Error returns */

L_9010:
	*status = 2;
	return;

L_9020:
	inum = 15;
	goto L_9400;

L_9040:
	inum = 17;
	goto L_9400;

L_9050:
	inum = 18;

L_9400:
	sherr( &inum );
	format_.nerr = 1;

L_9500:
	*status = 1;
	return;

L_9600:
	*status = 3;
	return;

} 

