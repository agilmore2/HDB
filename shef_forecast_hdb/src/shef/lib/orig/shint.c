/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 17:02:36 */
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
         SUBROUTINE SHINT(INUM,NDIG,IFLAG,STATUS)
  
       PURPOSE                                                    
  
         This routine will extract an integer of NDIG digits from
         the input file and return it in INUM. If IFLAG is set to 0,
         it uses the last character read. The number of digits
         actually read is returned in NDIG.
  
       VERSION and UPDATES
         1.0    APR 82   Geoffrey M. Bonnin
                Original Version
         2.0    JUN 94   David G. Brandon
                Also Translated to 'C' using FOR_C
                Removed alternate returns in subroutines
         2.1    Add iscore to t_codes array.
 *--------------------------------------------------------------------- */


void shint(inum, ndig, iflag, status)
short int *inum, *ndig, iflag, *status;
{
	short int num();
	static short int idig;

int xtemp, xcode;
	*status = 0;
	*inum = 0;
	idig = 0;
	/*             Get the next digit and check it */

	if( iflag == 0 )
		goto L_20;
L_10:
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

L_20:
	irang( &xchar_.ichar, &codes_.ich0, &codes_.ich9, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9030;
		}
	/*             Build number */

/*	*inum = *inum*10 + (num( &xchar_.ichar ) - num( &codes_.ich0 )); */
	*inum = *inum*10 + ( xchar_.ichar - codes_.ich0 );
	/*             Check if another digit is required */

	idig = idig + 1;
	if( idig >= *ndig )
		goto L_9000;
	goto L_10;

	/*             Normal return */

L_9000:
	return;

	/*             Stop reading the line */

L_9010:
	*status = 1;
	return;

	/*             Error in reading the file */

L_9020:
	*status = 2;
	return;

	/*             Error in number of digits */

L_9030:
	*ndig = idig;
	*status = 3;
	return;

} 

