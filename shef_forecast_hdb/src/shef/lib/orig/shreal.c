/*Translated by FOR_C, v3.4.2 (-), on 04/06/95 at 13:56:27 */
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
         SUBROUTINE SHREAL(VALUE,IFLAG,NDIG,STATUS)
  
       PURPOSE                                                    
  
         THIS ROUTINE READS REAL NUMBERS WITH A   IF IFLAG IS SET TO
         ZERO
         CHARACTER READ.
  
       VERSION and UPDATES
         1.0    JUL 82   Geoffrey M. Bonnin
                Original Version
         1.1    AUG 84 
                Improve screening of signs 
         1.2    AUG 92   David G. Brandon
                Allow blanks between the minus or plus sign and the
                numerical digits in the value.  For example the value
                - 34 was not valid before, but now is.
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
         2.1    Add iscore to t_codes array.
         2.2    NOV 95 DGB
                If the charater O is placed as a data value, 
                cast it to a 0. NOTE: data qualifier of O should
                not be used.
         2.3    JAN 27 98 DGB
                Add check for a NULL field, in .E messages.  If a NULL
                field was determined, i.e. sucessive slashes without a
                value between, an 'N' is internally written and flagged.
 *--------------------------------------------------------------------- */


void shreal(value, iflag, ndig, status)
double *value;
short int *iflag, *ndig, *status;
{
	short int num();
	static short int _fst, i, i_, idec, iend_, isign;
	static double sign, x;

    int DEBUG1,DEBUG2;
    DEBUG1 = 0;
    DEBUG2 = 0;

	/*             Initialise */
	*status = 0;
	*value = 0e0;
	isign = 0;
	sign = 1e0;
	idec = 0;
	*ndig = 0;
	iend_ = 0;

	/*             Clear past blanks */

	if( *iflag == 0 )
		goto L_7;
L_5:
    if ( DEBUG1 ) printf("\nshreal: at L_5 CLEAR xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9020;
		}

L_7:
    if ( DEBUG1 ) printf("\nshreal: at L_7 xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	if( xchar_.ichar == codes_.iblnk )
		goto L_5;
	goto L_20;

	/*             Get the next character */

L_10:
    if ( DEBUG1 ) printf("\nshreal: at L_10 xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9020;
		}

	/*            Check for sign */

L_20:
    if ( DEBUG1 ) printf("\nshreal: at L_20 xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	if( (xchar_.ichar != codes_.iplus) && (xchar_.ichar != codes_.iminus) )
		goto L_30;
    if ( DEBUG1 ) printf("\nshreal: isign = %d",isign);
	if( isign != 0 )
		goto L_100;
	isign = 1;
	if( xchar_.ichar == codes_.iminus )
		sign = -1e0;

L_21:
    if ( DEBUG1 ) printf("\nshreal: at L_21 xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	nextch( &xchar_.ichar, status );
    if ( DEBUG2 ) printf("\nshreal: status from nextch = %d",*status);

	if( *status == 1 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9020;
		}
    if ( DEBUG2 ) printf("\nshreal: xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
/*	if( xchar_.ichar == codes_.iblnk )
		goto L_21;
*/	
	goto L_20;	

	/*             Check for decimal */


L_30:
    if ( DEBUG1 ) printf("\nshreal: at L_30 xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	if( xchar_.ichar != codes_.idot )
		goto L_40;
    if ( DEBUG1 ) printf("\nshreal: idec = %d",idec);
	if( idec > 0 )
		goto L_100;
	idec = 1;
	isign = 1;
	goto L_10;

	/*             Check for 'M' - missing */

L_40:
    if ( DEBUG1 ) printf("\nshreal: at L_40 xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	if( xchar_.ichar != codes_.ichm )
		goto L_45;
    if ( DEBUG1 ) printf("\nshreal: ndig = %d isign = %d",*ndig,isign);
	if( (*ndig > 0) || (isign != 0) )
		goto L_100;
	*value = -9999e0;
	*ndig = 1;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9020;
		}

	goto L_9000;

     /*  check for 'N' which represents a NULL */          /* dgb:01/27/98 */
     
L_45:                                                      /* dgb:01/27/98 */
	if( xchar_.ichar != codes_.ichn )                      /* dgb:01/27/98 */
		goto L_50;                                         /* dgb:01/27/98 */
	if( (*ndig > 0) || (isign != 0) )                      /* dgb:01/27/98 */
		goto L_100;                                        /* dgb:01/27/98 */  
	*value = -10000e0;                                     /* dgb:01/27/98 */
	*ndig = 1;                                             /* dgb:01/27/98 */
	nextch( &xchar_.ichar, status );                       /* dgb:01/27/98 */  
	if( *status == 1 )                                     /* dgb:01/27/98 */
		{                                                  /* dgb:01/27/98 */ 
		*status = 0;                                       /* dgb:01/27/98 */
		goto L_9010;                                       /* dgb:01/27/98 */
		}                                                  /* dgb:01/27/98 */
	else if( *status == 2 )                                /* dgb:01/27/98 */
		{                                                  /* dgb:01/27/98 */
		*status = 0;                                       /* dgb:01/27/98 */
		goto L_9020;                                       /* dgb:01/27/98 */
		}                                                  /* dgb:01/27/98 */  

	goto L_9000;                                           /* dgb:01/27/98 */  

	/*             Check for 'T' = TRACE = 0.001 */

L_50:
    if ( DEBUG1 ) printf("\nshreal: at L_50 trace xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	if( xchar_.ichar != codes_.icht )
		goto L_60;
    if ( DEBUG1 ) printf("\nshreal: ndig = %d  isign = %d",ndig,isign);
	if( (*ndig > 0) || (isign != 0) )
		goto L_100;
	*value = -9e10;
	*ndig = 1;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9020;
		}


	goto L_9000;

	/*             Check for a digit and adjust the value */

L_60:
    if ( DEBUG1 ) printf("\nshreal: at L_60 xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
    if ( xchar_.ichar == codes_.icho )                     /* dgb:11/05/95 */
    xchar_.ichar = codes_.ich0;

	irang( &xchar_.ichar, &codes_.ich0, &codes_.ich9, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_90;
		}
	*ndig = *ndig + 1;
	isign = 1;
	if( idec > 0 )
		goto L_70;
/*	*value = 10e0**value + (num( &xchar_.ichar ) - num( &codes_.ich0 )); */
	*value = 10e0**value + ( xchar_.ichar  -  codes_.ich0 ); 
	goto L_10;

L_70:
    if ( DEBUG1 ) printf("\nshreal: at L_70 xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
/*	x = num( &xchar_.ichar ) - num( &codes_.ich0 ); */
  	x =     xchar_.ichar  -  codes_.ich0 ; 
	for( i = 1, _fst=1; (i <= idec) || _fst; i++, _fst=0 )
		{
		i_ = i - 1;
		x = x/10e0;
		}

	*value = *value + x;
	idec = idec + 1;
	goto L_10;

	/*             Check for / +    = MISSING */

L_90:
    if ( DEBUG1 ) printf("\nshreal: at L_90 + xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	if( *ndig > 0 )
		goto L_100;
	if( (xchar_.ichar == codes_.islash) && (isign == 0) )
		goto L_110;
    if ( DEBUG1 ) printf("\nshreal: isign = %d",isign);
	if( isign == 0 )
		goto L_100;
	if( sign < 0e0 )
		goto L_100;
	*value = -9999e0;
	*ndig = 1;
	goto L_9000;

	/*             This is a NULL ? */

L_110:
    if ( DEBUG1 ) printf("\nshreal: at L_110 NULL xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	*ndig = -1;
	goto L_9000;

	/*             Multiply by the sign */

L_100:
    if ( DEBUG1 ) printf("\nshreal: at L_100 MULTIPLY BY SIGN xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	*value = sign**value;
	if( iend_ == 1 )
		goto L_9011;


L_9000:
    if ( DEBUG1 ) printf("\nshreal: at L_9000 xchar_.ichar = %d %c",xchar_.ichar,xchar_.ichar);
	return;

	/*             Stop reading the line */

L_9010:
	iend_ = 1;
	goto L_100;

L_9011:
	*status = 1;
	return;

	/*             Error in reading file */

L_9020:
	*status = 2;
	return;

}

