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
  
       FUNCTION shef_test
  
       PURPOSE  
         To test the shef_decoder.  The user will provide the filename
         of the file containing shef data.  Two files will be created,
         one containing the shef data and any errors.  The second file
         contains full shef-out records of all data.
  
       VERSION and UPDATES
         1.0    SEP 95   David G. Brandon
                         Original Version
         1.1    OCT 95   DGB
                Add PE_SIZE and TS_SIZE macros for dimensioning.
         1.2    JAN 96   DGB
                Add message that tells the user how many errors
                were detected. 
         1.3    JAN 96 DGB
                Update struct t_data to include lcent (century).
                Increase message5 array to 63.
         1.4    JAN 96 DGB
                Print out warnings as well as errors.
         1.5x   SEP 10 97 DGB
                Add the use of MAX_SHEF_INPUT so that the input line can
                be longer than 80 characters.

  --------------------------------------------------------------------- */


shef_test()
{
int i;

char buffer[MAX_SHEF_INPUT];                                    	/* dgb:09/10/97 */
  
   fprintf(stdout,"\n------------------------------");
   fprintf(stdout,"\n  shef_decode: test option.\n");
   fprintf(stdout,"\n------------------------------");
   fprintf(stdout,"\n  input file:    ");
   fgets(buffer,sizeof(buffer),stdin);
   memset(&buffer[strlen(buffer) -1],0,1);

   /* store file names */
   memset(files_.shef_in,0,sizeof(files_.shef_in));
   memset(files_.shef_decode_err,0,sizeof(files_.shef_decode_err));
   memset(files_.shef_out,0,sizeof(files_.shef_out));
   strcpy(files_.shef_in,buffer);
   sprintf(files_.shef_decode_err,"%s.err",buffer);
   sprintf(files_.shef_out,"%s.out",buffer);

   /* set flags */
   strncpy(&cont_.out_flag[0],  "+",1);
   strncpy(&cont_.error_flag[0],"+",1);
   strncpy(&cont_.post_flag[0], "-",1);

   /* call decoder */
   if ( shefop() == 0 )
   {
     shdrive();
     shefcl();
   }

   fprintf(stdout,"\n  *** YOU HAVE %d SHEF ERRORS   ***",error_.nerror);
   fprintf(stdout,"\n  *** YOU HAVE %d SHEF WARNINGS ***\n\n",error_.nwarn);
}
