/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 17:01:19 */
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
         SUBROUTINE SHEFCLOSE
  
       PURPOSE                                                    
         Close the files
  
       VERSION and UPDATES
         1.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
         1.1    OCT 95   DGB
                Add closing of *sensors, *sqlerrors, *fp_postlog, *testfile
         1.2    FEB 09 2000 dgb
	        After closing file pointers, set then to 0 or NULL
		( needed on linux systems ).
 *--------------------------------------------------------------------- */


void  shefclose()
{

    fflush( fp_.icher);

    if ( fp_.lchn != NULL )
    {
         fclose(fp_.lchn);
         fp_.lchn = 0;                                      /* dgb: 02/09/00 */
    }
  
if ( fp_.mchn !=  NULL )
    {
       fclose(fp_.mchn);
       fp_.mchn = 0;                                        /* dgb: 02/09/00 */
    }
     
if ( fp_.kchn != NULL )
          {
         fclose(fp_.kchn);
	 fp_.kchn = 0;                                            /* dgb: 02/09/00 */
    }


	/*             If there are errors, always write out error file.
	 *             If the control variable is set to '+', write
	 *             out error file.  If the control variable is set to
	 *             '-', and there are now warnings, do not write.
	 * */

	if( error_.nerror != 0 || error_.nwarn != 0 )
	{
		if ( fp_.icher != NULL )
		{
                   fclose(fp_.icher);
		   fp_.icher = 0;                           /* dgb: 02/09/00 */
		}
	}
	else
	{

		if( cont_.error_flag[0] != '+' )
		{
		   if ( fp_.icher != NULL )
		   {
                      fclose(fp_.icher);
		      fp_.icher = 0;                        /* dgb: 02/09/00 */
		   }
                   remove(tempfiles_.err_file);
		}
		else
		{
		   if ( fp_.icher != NULL )
		   {
                      fclose(fp_.icher);
		      fp_.icher = 0;                        /* dgb: 02/09/00 */
		   }
	    
		}
			
	}

	if( cont_.out_flag[0] == '+' )
	{
		fflush(fp_.jchn);
		
		if ( fp_.jchn != NULL )
		{
                   fclose(fp_.jchn);                        /* dgb: 02/09/00 */
		   fp_.jchn = 0;
		}
	}

    if ( fp_postlog != NULL ) 
    {
        fflush(fp_postlog);
        fclose(fp_postlog);
	fp_postlog = 0;                                     /* dgb: 02/09/00 */
    }

    if ( sensors != NULL )
    {
        fflush(sensors);
        fclose(sensors);
	sensors = 0;                                        /* dgb: 02/09/00 */
    }

    if ( sqlerrors != NULL )
    {
       fflush(sqlerrors);
       fclose(sqlerrors);
       sqlerrors = 0;                                       /* dgb: 02/09/00 */
    }

    if ( testfile != NULL )
    {
       fflush(testfile);
       fclose(testfile);
       testfile = 0;                                        /* dgb: 02/09/00 */ 
    }

	return;

}

