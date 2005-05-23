/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 17:00:30 */
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
  
       SUBROUTINE SHDRIVE
  
       PURPOSE                                                    
         Controls the appropriate decode routine needed
  
       VERSION and UPDATES
         1.0    APR 82   Geoffrey M. Bonnin
                Original Version
         1.1    AUG 84   
                Count non-fatal errors
         1.2    JUN 89   David G. Brandon
                Adapted to QNX Hydromet
         2.0    JUN 94   David G. Brandon
                Also Translated to 'C' using FOR_C
                Removed alternate returns in subroutines
         2.1    OCT 95   DGB
                Add PE_SIZE and TS_SIZE macros for dimensioning.

 *--------------------------------------------------------------------- */


void shdrive()
{
    int DEBUG1;
	static short int iflag, irev, status;


	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Ibuf = &buffer_.ibuf[0] - 1;
	short *const Idate = &datim_.idate[0] - 1;

    DEBUG1 = 0;

    if ( DEBUG1 ) printf("\nshdrive: begin");
	status = 0;
	buffer_.nblnk = 0;
	format_.iform = 0;
	format_.nerr = 0;
	error_.nerror = 0;
	error_.nwarn = 0;
	goto L_25;

	/*             Look for a SHEF Format and Get the Type */

L_23:
	iflag = 0;
	goto L_30;
L_25:
	iflag = 1;
L_30:

	format_.lform = format_.iform;
	shfor( &format_.iform, &iflag, &status );
	if( status == 1 )
		{
		status = 0;
		goto L_9000;
		}

	sendflg_.nsflag = 0;

	/*             Decode this Format Type */
	switch(  abs(format_.iform) )
		{
		case 1: goto L_100;
		case 2: goto L_110;
		case 3: goto L_120;
		case 4: goto L_130;
		case 5: goto L_140;
		case 6: goto L_150;
		}

L_100:
    if ( DEBUG1 ) printf("\nshdrive:calling shdeca");
	irev = 0;

	shdeca( &irev, &status );	/* .A FORMAT */
	if( status == 1 )
		{
		status = 0;
		goto L_25;
		}
	else if( status == 2 )
		{
		status = 0;
		goto L_9000;
		}

	iflag = 0;
	goto L_30;

L_110:

    if ( DEBUG1 ) printf("\nshdrive:calling shdeca");

	irev = 1;

	shdeca( &irev, &status );	/* .AR FORMAT */
	if( status == 1 )
		{
		status = 0;
		goto L_25;
		}
	else if( status == 2 )
		{
		status = 0;
		goto L_9000;
		}

	iflag = 0;
	goto L_30;

L_120:
    if ( DEBUG1 ) printf("\nshdrive:calling shdecb");

	irev = 0;

	shdecb( &irev, &status );	/* .B FORMAT */
	if( status == 1 )
		{
		status = 0;
		goto L_25;
		}
	else if( status == 2 )
		{
		status = 0;
		goto L_9000;
		}
	else if( status == 3 )
		{
		status = 0;
		goto L_23;
		}

	iflag = 1;
	goto L_30;

L_130:
    if ( DEBUG1 ) printf("\nshdrive:calling shdecb");

	irev = 1;
	shdecb( &irev, &status );	/* .BR FORMAT */
	if( status == 1 )
		{
		status = 0;
		goto L_25;
		}
	else if( status == 2 )
		{
		status = 0;
		goto L_9000;
		}
	else if( status == 3 )
		{
		status = 0;
		goto L_23;
		}

	iflag = 1;
	goto L_30;

L_140:

    if ( DEBUG1 ) printf("\nshdrive:calling shdece");

	irev = 0;
	shdece( &irev, &status );	/* .E FORMAT */
	if( status == 1 )
		{
		status = 0;
		goto L_25;
		}
	else if( status == 2 )
		{
		status = 0;
		goto L_9000;
		}

	iflag = 0;
	goto L_30;


L_150:

    if ( DEBUG1 ) printf("\nshdrive:calling shdece");

	irev = 1;
	shdece( &irev, &status );	/* .ER FORMAT */
	if( status == 1 )
		{
		status = 0;
		goto L_25;
		}
	else if( status == 2 )
		{
		status = 0;
		goto L_9000;
		}

	iflag = 0;
	goto L_30;

	/*             Write Error Stats */

L_9000:

   stats_.parse_stats[2] = stats_.parse_stats[2] + error_.nerror ;
   stats_.parse_stats[3] = stats_.parse_stats[3] + error_.nwarn  ;
   fprintf( fp_.icher,
            "     NUMBER OF WARNING ERRORS IS %d \n     NUMBER OF FATAL ERRORS IS %d",
            error_.nwarn, error_.nerror);  
	return;

}

