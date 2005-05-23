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
  
         shef_in       <- directory containing shef data to be decoded
         shef_out      <- directory containing shefout files
         shef_error    <- directory containing error files
         shef_parm     <- (file) parmater file used by the decoder
         -SHEFOUT      <- (control) +=write shef_out, -=do not write 
         +ERRORFILE    <- (control) +=always write, -=only if errors
         +SHEFPASS     <- (control) +=call shef_pass -=no call to shef_pass
  
         You can either place a plus '+' or minus '-' sign in front of the
         control names.  The '+' turn the control on, the '-' turns it off.
  
         File names can include directory paths. This can be useful if
         you want to place the executable in a /bin directory, and the
         support file somewhere else.  An example may be:
                     /home/shef/inputparm
  
  
       VERSION and UPDATES
         1.0    MAY 94   David G. Brandon  
                Original Version
                Also Translated to 'C' using FOR_C
         1.1    SEP 95   DGB
                Change to accomodate test option from keyboard.
                Changed the name of the 'out' and 'err' file names.
                They are now in the format, 'out.xxx.YRMMDDHRMNSC'
                or 'err.xxx.YRMMDDHRMNSC' where xxx is the file name.
         1.2    OCT 95   DGB
                Add PE_SIZE and TS_SIZE macros for dimenioning.
         1.3    Remove all FORC I/O & change to C I/O
         1.4    JAN 3 98 DGB
                Add -c flag, which explicitly sets the century for
                the system clock from the command line.  Used for
                testing.
         1.5    JAN 18 98 DGB
                Change test for -c flag.  Allow for year, month,
                and day.  For example:
                -c 2000         override system year & set to 2000
                -c 200012       override year and month
                -c 20001231     override year, month and day
                -c 2000123100   override year,month, day, hour
                -c 200012311010 override year,month,day,hour,min
	1.6     FEB 09 00 DGB
	        Set all file pointers to 0 after closing.
 *--------------------------------------------------------------------- */



int shefop()
{   
    char log_file[91], err_file[91], buff_temp[20];
    char buff[15],buff1[5];
    char *c;
    long current_time;
	static char strg1[10], strg2[10];
	static short int ier, ier1, ier2, nchn, i;
	static long int itemp;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Idate = &datim_.idate[0] - 1;

    memset(tempfiles_.shef_in,0,sizeof(tempfiles_.shef_in));
    strcpy(tempfiles_.shef_in,files_.shef_in);

    memset(tempfiles_.shef_decode_err,0,sizeof(tempfiles_.shef_decode_err));
    strcpy(tempfiles_.shef_decode_err,files_.shef_decode_err);

    memset(tempfiles_.shef_out,0,sizeof(tempfiles_.shef_out));
    strcpy(tempfiles_.shef_out,files_.shef_out);

    memset(tempfiles_.shef_log,0,sizeof(tempfiles_.shef_log));
    strcpy(tempfiles_.shef_log,files_.shef_log);

    memset(tempfiles_.dotbtemp,0,sizeof(tempfiles_.dotbtemp));
    strcpy(tempfiles_.dotbtemp,"dotbtemp");

	/*             Get the Current Date
	 *             This will vary depending upon the operating system
	 *             used.  Place the year, month, day, etc. into the
	 *             array values as follows:
	 *               IDATE(1) = MONTH
	 *               IDATE(2) = DAY
	 *               IDATE(3) = YEAR (4 digit year, e.g., 1994)
	 *               IDATE(4) = HOUR
	 *               IDATE(5) = MINUTE
	 *               IDATE(6) = SECOND
     */

	 date_time( &Idate[1], &Idate[2], &Idate[3], &Idate[4], &Idate[5], 
	            &Idate[6] );


     if ( test_.test_flag==1 || test_.atest_flag==1 )
     {

       if ( test_.century_flag != 0 )
      {

        memset(buff,0,sizeof(buff));
        if ( strlen(test_.century_string) == 4 )           /* dgb:01/18/98 */
        {
          memset(buff1,0,sizeof(buff1));
          strncpy(buff1,&test_.century_string[0],4);
          Idate[3] = atoi(buff1);                   /* dgb:01/02/98 */
          printf("\n***NOTE: system year  reset to: %d\n",Idate[3]);
        }
        else
        if ( strlen(test_.century_string) == 6 )           /* dgb:01/18/98 */
        {
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[0],4);       /* dgb:01/18/98 */
          Idate[3] = atoi(buff1);                          /* dgb:01/18/98 */   
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[4],2);       /* dgb:01/18/98 */
          Idate[1] = atoi(buff1);                          /* dgb:01/18/98 */ 
          printf("\n***NOTE: system year  reset to: %d\n",Idate[3]);
          printf("\n***NOTE: system month reset to: %d\n",Idate[1]);
        }
        else                                               /* dgb:01/18/98 */
        if ( strlen(test_.century_string) == 8 )           /* dgb:01/18/98 */
        {
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[0],4);       /* dgb:01/18/98 */
          Idate[3] = atoi(buff1);                          /* dgb:01/18/98 */  
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[6],2);       /* dgb:01/18/98 */
          Idate[2] = atoi(buff1);                          /* dgb:01/18/98 */ 
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[4],2);       /* dgb:01/18/98 */
          Idate[1] = atoi(buff1);                          /* dgb:01/18/98 */  
          printf("\n***NOTE: system year  reset to: %d\n",Idate[3]);
          printf("\n***NOTE: system month reset to: %d\n",Idate[1]);
          printf("\n***NOTE: system day   reset to: %d\n",Idate[2]);
        }
        else                                               /* dgb:01/18/98 */
        if ( strlen(test_.century_string) == 10)           /* dgb:01/18/98 */
        {
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[0],4);       /* dgb:01/18/98 */
          Idate[3] = atoi(buff1);                          /* dgb:01/18/98 */  
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[6],2);       /* dgb:01/18/98 */
          Idate[2] = atoi(buff1);                          /* dgb:01/18/98 */ 
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[4],2);       /* dgb:01/18/98 */
          Idate[1] = atoi(buff1);                          /* dgb:01/18/98 */  
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[8],2);       /* dgb:01/18/98 */
          Idate[4] = atoi(buff1);                          /* dgb:01/18/98 */  
          printf("\n***NOTE: system year  reset to: %d\n",Idate[3]);
          printf("\n***NOTE: system month reset to: %d\n",Idate[1]);
          printf("\n***NOTE: system day   reset to: %d\n",Idate[2]);
          printf("\n***NOTE: system hour  reset to: %d\n",Idate[4]);
        }
        else                                               /* dgb:01/18/98 */
        if ( strlen(test_.century_string) == 12)           /* dgb:01/18/98 */
        {
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[0],4);       /* dgb:01/18/98 */
          Idate[3] = atoi(buff1);                          /* dgb:01/18/98 */  
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[6],2);       /* dgb:01/18/98 */
          Idate[2] = atoi(buff1);                          /* dgb:01/18/98 */ 
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[4],2);       /* dgb:01/18/98 */
          Idate[1] = atoi(buff1);                          /* dgb:01/18/98 */  
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[8],2);       /* dgb:01/18/98 */
          Idate[4] = atoi(buff1);                          /* dgb:01/18/98 */  
          memset(buff1,0,sizeof(buff1));                   /* dgb:01/18/98 */
          strncpy(buff1,&test_.century_string[10],2);      /* dgb:01/18/98 */
          Idate[5] = atoi(buff1);                          /* dgb:01/18/98 */  
          printf("\n***NOTE: system year  reset to: %d\n",Idate[3]);
          printf("\n***NOTE: system month reset to: %d\n",Idate[1]);
          printf("\n***NOTE: system day   reset to: %d\n",Idate[2]);
          printf("\n***NOTE: system hour  reset to: %d\n",Idate[4]);
          printf("\n***NOTE: system min   reset to: %d\n",Idate[5]);
        }
       }
     } 

    /*  use 'c' routine to get date/time and construct part of file */
    current_time = time(NULL);
    strftime(buff_temp,sizeof(buff_temp),".%m%d.%H%M%S",gmtime(&current_time));


			/*      Open the Shef_in and dotbtemp files */


    if ( (fp_.lchn = fopen(files_.shef_in,"r") ) == NULL )
          goto L_900;

    /* comment out dotbtemp file .. virtual buffer now allows for 48 items in .B line */
    /*
    sprintf(tempfiles_.dotbtemp,"%s","dotbtemp");
    if ( (fp_.mchn = fopen(tempfiles_.dotbtemp,"w") ) == NULL )
          goto L_915;
    */

	/*             Open a shef_out file if SHEFOUT is set to '+'.
                   The file name will be constructed from the
                   name of the shef_out file in the shef.cfg file
                   with the datetime stamp.
    */

	if( cont_.out_flag[0] == '+' )
	{

            /* dgb: 09/20/95 added change to out file name */
            if ( test_.test_flag )
            {
               sprintf(log_file,"%s%s",
                       tempfiles_.shef_out,buff_temp);
            }
            else
            {

                 if ( strlen(stats_.product_name) >= 9 )
                 {
                    sprintf(log_file,"%s/out.%-9.9s%s",
                    tempfiles_.shef_out,stats_.product_name,buff_temp);
                 }
                 else
                 {
                    sprintf(log_file,"%s/out.%s%s",
                    tempfiles_.shef_out,stats_.product_name,buff_temp);
                 }

            }

         /* open as ascii file */

          
          if ( (fp_.jchn = fopen(log_file,"w")) == NULL ) 
                goto L_920;

        /* print to screen if in test mode */
         if ( test_.test_flag )
               fprintf(stdout,"  shef_out_file: %s\n",log_file);
    }


			/*  Open the shef_decode_err file */

            /* dgb: 09/20/95 added change to err file name */
            if ( test_.test_flag )
               sprintf(err_file,"%s%s",
                       tempfiles_.shef_decode_err,buff_temp);
            else
            {

                 if ( strlen(stats_.product_name) >= 9 )
                 {
                    sprintf(err_file,"%s/err.%-9.9s%s",
                    tempfiles_.shef_decode_err,stats_.product_name,buff_temp);
                 }
                 else
                 {
                    sprintf(err_file,"%s/err.%s%s",
                    tempfiles_.shef_decode_err,stats_.product_name,buff_temp);
                 }

            }

            strcpy(tempfiles_.err_file,err_file);
            if ( (fp_.icher = fopen(err_file,"w")) == NULL )
                  goto L_910;

            /* print to screen if in test mode */
            if ( test_.test_flag )
               fprintf(stdout,"  error file:    %s\n",err_file);

	return(0);


L_900:
    fprintf(stdout,"\nshefop:error on opening input file --> %s",files_.shef_in);
	fclose(fp_.lchn);
	fp_.lchn = 0;                                       /* dgb:02/09/00 */
	exit(0);

L_910:
    fprintf(stdout,"\nshefop:error on opening file --> %s",files_.shef_decode_err);
	fclose(fp_.icher);
	fp_.icher = 0;                                      /* dgb:02/09/00 */
	exit(0);
/*
L_915:
    fprintf(stdout,"\nshefop:error on opening file --> %s",tempfiles_.dotbtemp);
	fclose(fp_.mchn);
	fp_.mchn = 0;                                      
	exit(0);
*/

L_920:
    fprintf(stdout,"\nshefop:error on opening file --> %s",files_.shef_out);
	fclose(fp_.jchn); 
	fp_.jchn = 0;                                       /* dgb:02/09/00 */
	exit(0);


}

