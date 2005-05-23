#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <share.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "shef_structs_external.h"


/*---------------------------------------------------------------------
  
       FUNCTION 
         shef_decode
  
       PURPOSE  
         Driver for the SHEF decoder.  This version will 'pluck' any
         file from a user specified directory, decode the data, and
         either create a 'shef_out' file qnd/or pass the data directly
         to a user created function that is tailored to writing to a 
         database server. The driver is designed to run continuously,
         looking for 'shef' files every few seconds. The driver can
         also be controlled to make a single passthru.  The flexibility
         is controlled by the user.
  
         This method will alow for shef data input from a variety of 
         sources. For example, a user could FTP a file into the
         directory over INTERNET or LAN.  A micro that is collecting
         data could write the shef files to the user specifiec
         directory through NFS (or FTP).  ALERT data could be 
         encoded in SHEF and written to the directory through NFS
         or FTP.  Another example is a task that receives AFOS products 
         could write the SHEF products to the directory.
    
         The control of the decoder is managed through a file called
         shef.cfg.
  
         Most of the I/O in this version maitains compatabiltiy
         with the FORTRAN to C translator.  A number of I/O
         libraries are used for the translation.


       VERSION and UPDATES for this function
         1.0    APR 95   David G. Brandon
                Original Version

         1.1    JUL 95   Dave G. Brandon
                Added check to see if file was closed or opened.

		 1.2	SEP 95   DGB
                Changed to read directory instead of index files.
                Added the test option for reading in a test file
                from the stdin. This option is started as follows:
                shef_decode -t.  Input is this read in from the
                keyboard. Add -d for the debug option.  Add
                -v for the verbose option.  Move check for command
                line options to its own function.

         1.3    OCT 95   usage and print_usage functions added. Add 
                the command line options, -s for listing out 
                unknown stations/sensors (INFORMIX VER) and the
                -e option for logging all SQL errors in INFORMIX.
                These options should not be turned on for the 
                QNX hydromet version.

         1.4    OCT 95   DGB
                Change from fixed array sizes to values read out 
                of the shef.h file. Add iscore to t_codes array.

         1.5    JAN 96   DGB
                Update struct t_data to include lcent (century).
                Increase message5 array to 63.

         1.6    MAY 30 96 DGB
                Include partial error option.  This option will tell
                the decoder to output only the line where a parsing
                error occurs, if an error occurs.  The default is to
                output the entire message, even if a single error
                occurs.

                Also get task id for QNX version.  Before, this was
                obtained for each record on each iteration.

         1.7    NOV 7 96 DGB
                Resize arrays ( make them bigger ) for inputparm
                values.

         1.8    OCT 19 97 DGB
                Fix detection of message source for .B type messages.

                Fix switchover dates for daylight/standard time.  There
                was previously an array alignment problem.  

                Revise error message for checking between the 
                years 1976 - 2020 & using a time zone other than Z.         

                Add database comment capabilities.

                Change century designation from 'DZ' to 'DT'.

                Change max characters on a line from 80 to 1000.

         1.9    OCT 30 97 DGB
                Fix time problem for .E messages like the one
                that follows:

                .E XXXXX 971020 DH12/HG/DI6
                .E1 / 12.2 / 10.0

                Messages that had a leading '/' were setting the
                initial time incorrectly. However, messages like
                the following did work:
                
                .E XXXXX 971020 DH12/HG/DI6
                .E1 12.2 / 10.0

                .E XXXXX 971020 DH12/HG/DI6/12.2
                .E1 / 10.0
                
         2.0    DEC  1 97 DGB
                Add new flag, '-at', which stands for advanced 
                testing.  Each 'shefout' record will be printed
                in the error file beneath the encoded shef data.
                This will be useful and easier to match the shef_out
                record with the shef encoded data.

         2.3    JAN  13 98 DGB
                Add new flag, '-c #', which stand for century.  This
                option is used for testing the decoder.  When set, 
                the value for the century obtained from the system 
                clock is overrided with the value, #.  For example,
                -c 2000 says to assume the century is 2000.

                Fix time increments for NULL values in .E messages.
        
  --------------------------------------------------------------------- */

FILE   *testfile;
FILE   *fp_postlog;
FILE   *sensors;                                            /* dgb:10/02/95 */
FILE   *sqlerrors;                                          /* dgb:10/01/95 */

char   *pname = "shef_decode";
char   *homename = DIRECTORY_OF_CFG;
char   *directory;

int    num_records;
int    VERBOSE, DEBUG, SENSORS, INFORMIX_ERR, PARTIAL_ERROR;/* dgb:05/30/96 */
int    STATS;                                               /* dgb:09/22/96 */
int    tid;    /* For Hydromet Database (QNX) only */       /* dgb:05/30/95 */
DIR    *dirp;
struct dirent *de;


main( argc, argv )
int argc; char *argv[];
{

   char  sheftemp1[90], sheftemp2[90];
   char buff_err[5][80], fname[100];
   char *suspend = "suspend";
   char *c;
   struct stat buf;

   int ii, ifil;

   long start_time;

   FILE  *fp2;


   /* Blockdata info for initialization */
      blk_data(); 

   /* Check command line options */
      usage(argc,argv);

   /* Open the shef.cfg and parameter files */
      shefopen();

   /* For Hydromet only - get task id (tid) of databuffer */
      if ( cont_.post_flag[0] == '+' )                    /* dgb:05/30/96 */
      {
         if( ( tid = find_databuf() ) == -1 )
         {
            if ( VERBOSE )
               fprintf(stdout, "\nshef_decode: No 'databuffer' task found\n");
/*            exit(EXIT_FAILURE); */
         }
      }

   /* Clean and initialize file arrays  */

   memset(sheftemp1,0,sizeof(sheftemp1));
   strcpy(sheftemp1,files_.shef_in);
   strcpy(sheftemp2,files_.shef_in);

   /* Initialize statistics */
      if ( STATS )                                         /* dgb:09/22/96 */
         stats_init(files_.shef_log);  
      

   /* check if task should be suspended */
   if ( suspend_it(sheftemp1) != 0 )
        exit(0);

   /* open the directory */

      dirp = opendir(sheftemp1);
      if ( dirp != NULL )
      {

      /* Process each file in directory  */
      while (  ( de=readdir(dirp))  != NULL )
      {
         if ( de->d_name[0] != '.' )
         {
            if ( VERBOSE )
                fprintf(stdout,"\n*****\nbegin processing file-> %s",de->d_name);
            strcpy(fname,sheftemp1);
            strcat(fname,"/");
            strcat(fname,de->d_name);

            /* check if file is in use */
            ifil = sopen(fname,O_RDONLY,SH_DENYWR);

            if ( ifil != -1 )
            {
               stat(de->d_name,&buf);

               if ( buf.st_size > 0 )
               {

                  strcpy(files_.shef_in,fname);
                  strcpy(stats_.product_name,de->d_name); 
                  if ( shefop() == 0 )
                  {
                     if ( VERBOSE )
                          fprintf(stdout,"\n     decoding file -> %s",fname);
                     shdrive();
                     shefcl();
                     if ( STATS )                          /* dgb:09/22/96 */
                     {
                        stats_.time_stop_prod = time(NULL); 
                        stats_.parse_stats[0]++; 
                     }
                     if ( VERBOSE )
                          fprintf(stdout,"\n     num_records = %d",num_records);
                     if ( STATS )                          /* dgb:09/22/96 */ 
                        stats_print();
                  }
               }
               else
               {
                  remove( fname );
                  close(ifil);
               }
            }
            else
            {
               close(ifil);
               if ( DEBUG )
                    printf("\n file opened..not ready");
            }
            strcpy(files_.shef_in,sheftemp1);
         }
      }
      if ( VERBOSE )
           fprintf(stdout,"\n     closing directory");
      closedir(dirp);
   }

   if ( STATS )                                            /* dgb:09/22/96 */
      stats_print(); 

   /* close files */
   shefclose();
}

usage( int argc, char *argv[])
{
int ii;
   
   /* dgb:10/10/95 usage function added */
   /*
       -at  turn on advanced test option
       -t   turn on test option 
       -d   turn on debug option
       -v   turn on verbose option
       -l   turn on log option
       -e   turn on logging of all INFORMIX SQL errors
       -s   turn on logging of unknown stations/sensors (INFORMIX VER)
       -p   turn on option to output only the line where a parsing error occurs
       -c # explicitly set system clock to this century, e.g., #=2000 
   */

      DEBUG              = 0;
      VERBOSE            = 0;
      SENSORS            = 0;
      INFORMIX_ERR       = 0;
      PARTIAL_ERROR      = 0;                              /* dgb:05/30/96 */
      STATS              = 0;                              /* dgb:09/22/96 */
      test_.test_flag    = 0;
      test_.atest_flag   = 0;                              /* dgb:12/01/97 */
      test_.century_flag = 0;                              /* dgb:01/02/98 */

      for ( ii = 0; ii < argc; ii++ )
      {
         if ( strncmp(argv[ii],"-?",2) == 0 ) 
              print_usage();
      }

      for ( ii = 0; ii < argc; ii++ )
      {
         if ( strncmp(argv[ii],"-at",3) == 0 )             /* dgb:12/01/97 */
         {                                                 /* dgb:12/01/97 */ 
              test_.atest_flag = 1;                        /* dgb:12/01/97 */
              test_.test_flag  = 1;                        /* dgb:12/01/97 */
         }                                                 /* dgb:12/01/97 */
         if ( strncmp(argv[ii],"-d",2) == 0 )
              DEBUG = 1;
         if ( strncmp(argv[ii],"-l",2) == 0 )              /* dgb:09/22/96 */
              STATS = 1;
         if ( strncmp(argv[ii],"-v",2) == 0 )
              VERBOSE = 1;
         if ( strncmp(argv[ii],"-p",2) == 0 )              /* dgb:05/30/96 */
              PARTIAL_ERROR = 1;
/*       comment these two options out for QNX
         if ( strncmp(argv[ii],"-s",2) == 0 )
              SENSORS = 1;
         if ( strncmp(argv[ii],"-e",2) == 0 )
              INFORMIX_ERR = 1;
*/
         if ( strncmp(argv[ii],"-t",2) == 0 )
            test_.test_flag = 1;
         if ( strncmp(argv[ii],"-c",2) == 0 )              /* dgb:01/02/98 */
            test_.century_flag = atoi(argv[ii+1]);

      } 
   
      if ( test_.test_flag )                               /* dgb:05/28/96 */
      {
            shefopen();
            shef_test();
            shefclose();
            exit(0);
      }

}

void print_usage()
{
   /* dgb:10/10/95 - print_usage function added */
   fprintf(stdout,
      "\n\nshef_decode V2.3(QNX) - JAN 13, 98 - NOAA/NWS/CBRFC \n"); 
   fprintf(stdout,
      "shef_decode -at -t -d -v -l -p -c #\n");
   fprintf(stdout,"%s %s %s %s %s %s %s %s\n",
	  "where:\n",
      "   -at            execute advanced testing\n",
      "   -t             execute test version\n",
      "   -d             turn on debug information\n",
      "   -v             turn on verbose option\n",
      "   -l             turn on stats log\n",
      "   -p             turn on partial error option\n",     /* dgb:05/30/96 */
      "   -c #           explicity set century to the value = #\n"); /* dgb:01/02/98 */
     
/*    comment these options out for QNX
      "   -s           create file of unknown stations/sensors (INFORMIX VER)\n",
      "   -e           write all INFORMIX error to file\n");
*/    
 	exit(0) ;
}



