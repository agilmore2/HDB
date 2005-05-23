/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 17:01:33 */
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
         SUBROUTINE SHEFCL
  
       PURPOSE                                                    
         Close files used by the shef decoder during each iteration.
  
       VERSION and UPDATES
         1.0    MAY 94   David G. Brandon  
                Original Version
                Also Translated to 'C' using FOR_C
         1.1    OCT 95   DGB
                Do not delete input file if test version is requested.
	1.2     FEB 09 00 DGB
                Change all file pointers that are closed to NULL or 0.	       
 *--------------------------------------------------------------------- */


void shefcl()
{
	/*	Close Files */
        if ( !test_.test_flag )                            /* dgb:10/01/95 */
        {
           fclose(fp_.lchn);
	   fp_.lchn = 0;                                   /* dgb:02/09/00 */
           remove(files_.shef_in);    	
        }
/*      dotbtemp file no longer used..virtual buffer now allows for
        48 values in .B line
        fclose(fp_.mchn);
	fp_.mchn = 0;                                       
        remove(tempfiles_.dotbtemp);

*/
		if( cont_.out_flag[0] == '+' ) 
           fclose(fp_.jchn);
	   fp_.jchn = 0;                                   /* dgb:02/09/00 */


	/*	If there are errors, always write out error file.
	  	If the control variable is set to '+', write
		out error file.  If the control variable is set to
		'-', and there are no warnings, do not write. */

		if( error_.nerror != 0 || error_.nwarn != 0 )
		{
            fclose(fp_.icher);                             
	    fp_.icher = 0;                                /* dgb:02/09/00 */
		}
		else
		{

			if( cont_.error_flag[0] != '+' )
			{
                fclose(fp_.icher);
	        fp_.icher = 0;                             /* dgb:02/09/00 */
                remove(tempfiles_.err_file);
			}
			else
			{
                fclose(fp_.icher);
		fp_.icher = 0;                             /* dgb:02/09/00 */
			}

		}


	return;

}

