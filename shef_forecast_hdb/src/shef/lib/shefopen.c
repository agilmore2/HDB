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

       FUNCTION
         shefopen

       PURPOSE                                                    
         Open files used by the shef decoder.  All file names and some
         program control is read from the configuration file: shef.cfg.
         The file names can include path names, up to 80 characters long.
         The format of the shef.cfg files is as follows:

         shef_in       <- (dir) directory containing shef data to be decoded
         shef_out      <- (dir) directory containing ASCII decoded data
         shef_error    <- (dir) directory containing error files 
         shef_parm     <- (file) parmater file used by the decoder
         -SHEFOUT      <- (control) +=write shef_out, -=do not write 
         +ERRORFILE    <- (control) +=always write, -=only if errors
         +SHEFPASS     <- (control) +=call shef_pass -=no call to shef_pass
  
         You can either place a plus '+' or minus '-' sign in front of the
         control names.  The '+' turns the control on, the '-' turns it off.
  
       VERSION and UPDATES
         1.0    MAY 94   David G. Brandon  
                Original Version
                Also Translated to 'C' using FOR_C
         1.1    SEP 95   DGB
                Changed order of shef.cfg file, moved shef_error before
                shef_parm file.  Also changed shef_out and shef_error
                to directories, and not files. Set directory/file name
                for the shef.cfg in the header instead of hard coded.
         1.2    OCT 95  DGB
                Add code to open file for collecting unknown sensors.
                Add PE_SIZE and TS_SIZE macros for dimension.
         1.3    Remove all FORC I/O & replace with C I/O
	 1.4    FEB 09 00 DGB
	        Change all file pointers to 0 after closing.
 *--------------------------------------------------------------------- */


                             /* dgb:10/19/95 */
void shefopen()
{
   
    static char shef_parm[91], strg1[10];
	static short int ier, ier1, ier2, nchn;
	static long int itemp;

    char buffer[MAX_BUFF_CFG_LINE];
    char bufftemp[MAX_BUFF_CFG_LINE];
    char buff_sensors[91];                                 /* dgb:10/01/95 */

    char *c, *str1, *str2;
    int i;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Idate = &datim_.idate[0] - 1;

    memset(files_.shef_in,0,sizeof(files_.shef_in));
    memset(files_.shef_out,0,sizeof(files_.shef_out));
    memset(files_.shef_decode_err,0,sizeof(files_.shef_decode_err));
    memset(files_.shef_log,0,sizeof(files_.shef_log));


    /*        open the configuration file        */

    if ( ( fp_.nchn = fopen(homename,"r")) == NULL )
          goto L_925;
 
    if ( (fgets(files_.shef_in,sizeof(files_.shef_in),fp_.nchn)) == NULL )
          goto L_800;

    if ( (fgets(files_.shef_out,sizeof(files_.shef_out),fp_.nchn)) == NULL )
          goto L_800;

    if ( (fgets(files_.shef_decode_err,sizeof(files_.shef_decode_err),fp_.nchn)) == NULL )
          goto L_800;

    if ( (fgets(shef_parm,sizeof(shef_parm),fp_.nchn)) == NULL )
          goto L_800;

    if ( (fgets(files_.shef_log,sizeof(files_.shef_log),fp_.nchn)) == NULL )
          goto L_800;

    if ( (fgets(cont_.out_flag,sizeof(cont_.out_flag),fp_.nchn)) == NULL )
          goto L_800;

    if ( (fgets(cont_.error_flag,sizeof(cont_.error_flag),fp_.nchn)) == NULL )
          goto L_800;

    if ( (fgets(cont_.post_flag,sizeof(cont_.post_flag),fp_.nchn)) == NULL )
          goto L_800;

   /* Read in Acceptable PE Codes */
      if ( (fgets(bufftemp,MAX_BUFF_CFG_LINE, fp_.nchn )) != NULL )
      {
         strcpy(buffer,bufftemp);
         str2 = buffer; 
         i = 0;
         stats_.num_pe = 0;
      }

      while ( (str1=strtok(str2," \t\n\r")) != NULL ) 
      {
         str2 = 0;
         strncpy(stats_.acceptable_pe[i],str1,strlen(str1));
         i++;
         if ( i > MAX_ACCEPTABLE_PE ) {
              fprintf(stderr,"\nArray not big enough to hold PE codes specified in shef.cfg file");
              exit(1);
         }
      }
      stats_.num_pe = i ;

   /* Read in Acceptable TS Codes */
      if ( (fgets(bufftemp,MAX_BUFF_CFG_LINE,fp_.nchn )) != NULL )
      {
         strcpy(buffer,bufftemp);
         str2 = buffer; 
         i = 0;
         stats_.num_ts = 0;
      }

      while ( (str1=strtok(str2," \t\n\r")) != NULL ) 
      {
         str2 = 0;

         strncpy(stats_.acceptable_ts[i],str1,strlen(str1));

         i++;

         if ( i > MAX_ACCEPTABLE_TS ) {
              fprintf(stderr,"\nArray not big enough to hold TS codes specified in shef.cfg file");
              exit(1);
         }
      }
       stats_.num_ts = i ;

    fclose(fp_.nchn);
    fp_.nchn = 0;                                           /* dgb:02/09/00 */

    /* Mark the end of the files with a NULL */


    c = strchr(files_.shef_in,10);
    memset(c,0,1);
    c = strchr(files_.shef_out,10);
    memset(c,0,1);
    c = strchr(files_.shef_log,10);
    memset(c,0,1);
    c = strchr(files_.shef_decode_err,10);
    memset(c,0,1);
    c = strchr(shef_parm,10);
    memset(c,0,1);

	/*             Open the Parameter File, Read It & Close      */

    if ( ( fp_.kchn = fopen(shef_parm,"r" )) == NULL )
          goto L_905;
	getparm();

    fclose(fp_.kchn);
    fp_.kchn = 0;                                           /* dgb:02/09/00 */
printf("\n kchn  = %ld",fp_.j\kchn);fflush(stdout);
    /*  open file to collect unknown sensors if command arg SENSORS is true */
    /*  dgb:10/01/95 add code to open file */
    if ( SENSORS )
    {
       sprintf(buff_sensors,"%s/unknown_sensors",files_.shef_log);
       if ( ( sensors = fopen(buff_sensors,"a")) == NULL )
          fprintf(stdout,"\nshefopen: could not open file -> %s",buff_sensors );
 
    }

    /*  open file to collect INFORMIX SQL errors if command arg INFORMIX_ERR is true */
    /*  dgb:10/01/95 add code to open file */
    if ( INFORMIX_ERR )
    {
       sprintf(buff_sensors,"%s/sql_errors",files_.shef_log);
       if ( ( sqlerrors = fopen(buff_sensors,"a")) == NULL )
          fprintf(stdout,"\nshefopen: could no open file -> %s",buff_sensors );
    }

	return;


L_800:
    fprintf( stdout,"\nshef_decode:error on reading file --> %s\n",homename);
	fclose(fp_.nchn);
	fp_.nchn = 0;                                       /* dgb:02/09/00 */
	exit(0);


L_905:
    fprintf( stdout,"\nshef_decode:error on opening file --> %s\n", shef_parm);
	fclose(fp_.kchn);
	fp_.kchn = 0;                                       /* dgb:02/09/00 */
	exit(0);

L_925:
    fprintf( stdout,"\nshef_decode:error on opening file --> %s\n",homename);
	fclose(fp_.nchn);
        fp_.nchn = 0;                                       /* dgb:02/09/00 */
	exit(0);

}






