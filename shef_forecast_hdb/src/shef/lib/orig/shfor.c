/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 17:02:25 */
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
         SUBROUTINE SHFOR(ITYPE,IFLAG,STATUS)
  
       PURPOSE                                                    
         This routine reads characters one at a time from the input
         file and searches for a SHEF format specifier.
         The format type is returned in ITYPE.
         If IFLAG is set to zero, it looks at the last character read.
  
         SHEF FORMAT SPECIFIER    ITYPE
                     .A           1
                     .AR          2
                     .B           3
                     .BR          4
                     .E           5
                     .ER          6
  
       VERSION and UPDATES
         1.0    APR 82   Geoffrey M. Bonnin
                Original Version
         1.1    AUG 84   Geoffrey M. Bonnin
                ALLOW ":" AS TURN ON/OFF                 
         1.2    JUN 89   David G. Brandon
                Adapted to QNX NWS Hydromet
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
         2.1    Add iscore to t_codes array.
         2.2x   SEP 10 97 DGB
                Add the use of MAX_SHEF_INPUT so that the input line can
                be longer than 80 characters.
         2.3    AUG 2 98 DGB
                Decode test 'Y' formats the same as 'A'.
 *--------------------------------------------------------------------- */

void shfor(itype, iflag, status)
short int *itype, *iflag, *status;
{
	static short int flag;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Ibuf = &buffer_.ibuf[0] - 1;

	*status = 0;

	if( *iflag == 0 )
		goto L_15;

	/*             Read a Character */

L_10:
	buffer_.ip = MAX_SHEF_INPUT + 1;                            /* dgb:09/10/97 */

	/*             Check for A '.' in column 1 */

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_10;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
L_15:
	if( (xchar_.ichar == codes_.idot) && (buffer_.ip == 1) )
		goto L_19;
	goto L_10;

	/*             Check for 'A', 'B' or 'E' */

L_19:
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_10;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

    if ( xchar_.ichar == codes_.ichy ) /* dgb:08/02/98 */
         goto L_29;
	if( xchar_.ichar != codes_.icha ) 
		goto L_30;	/* 'A' */
L_29:	*itype = 1;
		goto L_50;

L_30:
	if( xchar_.ichar != codes_.ichb )
		goto L_40;	/* 'B' */
	*itype = 3;
	goto L_50;

L_40:
	if( xchar_.ichar != codes_.iche )
		goto L_9020;	/* 'E' */
	*itype = 5;
	goto L_50;

	/*             Check for an 'R' .. Revision */

L_50:
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_10;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	if( xchar_.ichar != codes_.ichr )
		goto L_56;
	*itype = *itype + 1;

	/*             Check for a continuation */

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_10;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

L_56:
	irang( &xchar_.ichar, &codes_.ich1, &codes_.ich9, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_60;
		}
	*itype = -*itype;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_10;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	goto L_9000;

	/*             Make sure we haven't picked up a '.END' */

L_60:
	if( xchar_.ichar == codes_.ichn )
		goto L_9020;

	/*             Got the format type */

L_9000:
	return;

	/*             Handle file errors */

L_9010:
	*status = 1;
	return;

L_9020:
    flag = 1;
	sherr( &flag );
	goto L_10;

} 

