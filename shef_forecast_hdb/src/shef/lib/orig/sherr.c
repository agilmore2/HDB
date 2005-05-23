/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 17:01:47 */
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
         SUBROUTINE SHERR(IER)        
  
       PURPOSE                                                    
         Decode .E Format                        
         IREV = 1 FOR REVISED, = 0 FOR NEW DATA
  
       VERSION and UPDATES
         1.0    APR 82   Geoffrey M. Bonnin
                Original Version
         1.1    AUG 84 
                Conut non fatal errors 
         2.0    MAY 94   David G. Brandon
                Add the ability to print out specific error messages
                to the error file.
                Also Translated to 'C' using FOR_C
         2.1    Add iscore to t_codes array.
         2.2    Add PARTIAL_ERROR option which only prints our line
                where error occurs.
         2.3x   SEP 10 97 DGB
                Add the use of MAX_SHEF_INPUT so that the input line can
                be longer than 80 characters.

 *--------------------------------------------------------------------- */


void sherr(ier)
short int *ier;
{
	static short int _fst, i, i_, line[MAX_SHEF_INPUT];         /* dgb:09/10/97 */

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Ibuf = &buffer_.ibuf[0] - 1;
	short *const Line = &line[0] - 1;

	if( (buffer_.ip <= 0) || (buffer_.ip >= MAX_SHEF_INPUT + 1) ) /* dgb:09/10/97 */
		goto L_30;
	for( i = 1, _fst=1; (i <= buffer_.ip) || _fst; i++, _fst=0 )
		{
		i_ = i - 1;
		Line[i] = codes_.iblnk;
		}

    if ( PARTIAL_ERROR )                                   /* DGB:05/30/96 */
         fprintf( fp_.icher,"%s\n",tempfiles_.last_line_read); /* dgb:05/30/96 */


	Line[buffer_.ip] = codes_.iarrow;
	for( i = 1, _fst=1; (i <= buffer_.ip) || _fst; i++, _fst=0 )
		{
        fprintf( fp_.icher, "%c", Line[i]);
		}
    fprintf( fp_.icher,"\n");

L_30:
	if( (*ier != 23) && (*ier != 32) && (*ier != 38) )
		{
		error_.nerror = error_.nerror + 1;
        fprintf( fp_.icher,"                    SHEF ERROR NUMBER %d \n",
             *ier);
		}
	else
		{
		error_.nwarn = error_.nwarn + 1;
        fprintf( fp_.icher,"                    SHEF WARNING NUMBER %d \n",
             *ier);
		}

	sherrp( ier );

	return;
}

