#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "shef_generic.h"
#include "shef_structs.h"


int num_raw1;

/*---------------------------------------------------------------------
  
       FUNCTION
         shef_decode

       PURPOSE  
         Driver for the generic SHEF decoder.  
	 
         The control of the decoder is managed through a file called
         shef.cfg.
  
       VERSION   for the entire decoder
         1.0    FEB 10 00 
 
   --------------------------------------------------------------------- */

/*    EXEC SQL END DECLARE SECTION; */ 

int    STRANGER;                                           /* dgb:01/20/97 */
int    HOW_OLD;                                            /* dgb:02/05/97 */
int    CONTINUOUS;                                         /* dgb:04/30/98 */
int    LOOP;                                               /* dgb:04/30/98 */
int    SNOOZE_TIME = 2;                                    /* dgb:04/30/98 */

char *homename = DIRECTORY_OF_CFG;

main( argc, argv )
int argc; char *argv[];
{

   char  sheftemp1[90], sheftemp2[90];
   char buff_err[5][80], fname[100];
   char *suspend = "suspend";
   char *c;
   struct stat buf;

   int ii, ifil, executions;

   long start_time;

   FILE  *fp2;

   /* OFFSET Vectors w/subscript range: 1 to dimension */
		short *const Idate = &datim_.idate[0] - 1;
   /* end of OFFSET VECTORS */

   /* Blockdata infor for initialization */
      blk_data();

   /* Check command line options */
      usage(argc,argv);


      LOOP = 1;                                             /* dgb:04/30/98 */

      while ( LOOP )                                        /* dgb:04/30/98 */
      {                                                     /* dgb:04/30/98 */

      if ( CONTINUOUS )
      {
         if ( VERBOSE )
              printf("\nDecoder in Continuous Mode: beginning loop");
      }

   /* Open the shef.cfg and parameter files */
      shefopen();


   /* Clean and initialize file arrays  */

   memset(sheftemp1,0,sizeof(sheftemp1));
   strcpy(sheftemp1,files_.shef_in);
   strcpy(sheftemp2,files_.shef_in);
   
   executions = 0;

   /* Check if task should be suspended */
   if ( suspend_it(sheftemp1) != 0 )
        exit(0);


   /* Initialize statistics */
      stats_init(files_.shef_log);
      num_records = 1;
      
      num_raw1 = 0;
 
     /* open the directory */

      dirp = opendir(sheftemp1);
      if ( dirp != NULL )
      {

         /* Process each file name in directory  */
         while (  ( de=readdir(dirp)) != NULL )
         {                 
          
            memset(fname,0,sizeof(fname));
            strcpy(fname,sheftemp1);
            strcat(fname,"/");
            strcat(fname,de->d_name);
             
            /* process only files - not directories */
            stat(fname,&buf);
             
            if ( !S_ISDIR(buf.st_mode) )
            {

               if ( de->d_name[0] != '.' )
               {
 
                  if ( VERBOSE )
                      fprintf(stdout,"\n*****\nbegin processing file-> %s",de->d_name);
    
                  /* check if file is in use */

                  if ( !is_file_closed(fname,0,HOW_OLD) )
                  {
                     stat(fname,&buf);

                     if ( buf.st_size > 0 )
                     {
                        strcpy(files_.shef_in,fname);
                        strcpy(stats_.product_name,de->d_name);

                        if ( shefop() == 0 )
                        {  
                           if ( VERBOSE )
                                fprintf(stdout,"\ndecoding file -> %s",fname);
                           shdrive();
                           shefcl();

                           stats_.time_stop_prod = time(NULL);
                           stats_.parse_stats[0]++;
                           stats_print();
                           if ( VERBOSE ) fprintf(stdout,"\nnum_records = %d",num_records );

                        }
                     }   
                     else
                        remove( fname );
                  }    
             
                  executions++;
                  if ( VERBOSE ) fprintf(stdout,"\nending execution # = %d",executions);
                  if ( executions > NUM_EXECUTIONS )
                       break;
              
                  /* Check if task should be suspended */
                  if ( suspend_it(sheftemp1) != 0 )
                       break;
                   
               }
              
            }

         }
         if ( VERBOSE )  fprintf(stdout,"\nclosing directory");
              closedir(dirp);
      }

   stats_print();
   
   /* close files */
   shefclose();
 
   if ( !CONTINUOUS )                                       /* dgb:04/30/98 */
         LOOP = 0;                                          /* dgb:04/30/98 */
   else                                                     /* dgb:04/30/98 */
         sleep(SNOOZE_TIME);                                /* dgb:04/30/98 */
 
   }
}

usage( int argc, char *argv[])
{
int ii;
   /* dgb:10/01/95 usage function added */
   /*
      -t             turn on test option 
      -d             turn on debug option
      -v             turn on verbose option
      -c             turn on override of CCYRMMDDHRMN, e.g., -c 20000101
      -p             turn on partial error option
      -q             assume the revision flag is on permanently
      -o             run decoder in continuous mode
      -fcfg filename input configuration path/filename from command line
      -howold #      number of minutes to wait before processing file-default = 1
      -loop #        where # equals the number of iterations (files processed) before stopping


   */

      DEBUG           = 0;
      VERBOSE         = 0;
      SENSORS         = 0;
      INFORMIX_ERR    = 0;
      test_.test_flag = 0;
      test_.atest_flag = 0;                                /* dgb:11/03/98 */
      PARTIAL_ERROR   = 0;                                 /* dgb:07/18/96 */
      FCFG            = 0;                                 /* dgb:08/08/96 */
      STRANGER        = 0;                                 /* dgb:01/20/97 */
      HOW_OLD         = 1;                                 /* dgb:01/02/97 */
      NUM_EXECUTIONS  = 20;                                /* dgb:03/19/97 */
      CONTINUOUS      = 0;                                 /* dgb:04/30/98 */
      DOTEPRE         = 1;                                 /* dgb:11/03/98 */
      TURNONR          = 0;                                 /* DGB:01/28/00 */
      test_.century_flag = 0;                               /* dgb:01/18/99 */

      strcpy(tempfiles_.database,"fastetc");               /* dgb:03/19/97 */
            
      for ( ii = 0; ii < argc; ii++)
      {
           if ( strncmp(argv[ii],"-?",2) == 0 )      /* dgb: 10/16/95 add -? */
                print_usage();
      }
      for ( ii = 0; ii < argc; ii++ ) 
      {

         if ( strncmp(argv[ii],"-at",3) == 0 )              /* dgb:09/25/98 */
         {                                                  /* dgb:09/25/98 */
             test_.atest_flag = 1;                          /* dgb:09/25/98 */
             test_.test_flag = 1;                           /* dgb:09/25/98 */
         }                                                  /* dgb:09/25/98 */

         if ( strncmp(argv[ii],"-c",2) == 0 )               /* dgb:01/18/99 */ 
         {
              test_.century_flag  = 1;                      /* dgb:01/18/99 */ 
              strcpy(test_.century_string,argv[ii+1]);      /* dgb:01/18/99 */
         
         }       
         if ( strncmp(argv[ii],"-d",2) == 0 )
              DEBUG = 1;
         if ( strncmp(argv[ii],"-q",2) == 0 )               /* dgb:01/28/99 */
              TURNONR = 1;                                  /* dgb:01/28/99 */
         if ( strncmp(argv[ii],"-v",2) == 0 )
              VERBOSE = 1;
         if ( strncmp(argv[ii],"-p",2) == 0 )                 /* dgb:07/18/96 */
              PARTIAL_ERROR = 1;
         if ( strncmp(argv[ii],"-o",2) == 0 )                 /* dgb:10/06/98 */
              CONTINUOUS = 1;
         if ( strncmp(argv[ii],"-fcfg",5) == 0 )              /* dgb 08/08/96 */
         {
              FCFG    = 1;
              if ( ii+1 < argc )
              {
                 if ( strchr(argv[ii+1],'-') != NULL )
                 {
                    fprintf(stdout,"\nshef_decode_raw:ERROR-no filename specified for -fcfg option\n");
                    shefclose();
                    exit(0);
                 }
                 strcpy(tempfiles_.cfg,argv[ii+1]);
              }
              else
              {
                 if ( VERBOSE ) 
                    fprintf(stdout,"\nshef_decode_raw:ERROR-no filename specified for -fcfg option\n"); 
                 shefclose();
                 exit(0);
              }
          }

         if ( strncmp(argv[ii],"-howold",7) == 0 )              /* dgb 02/05/97 */
         {

              if ( ii+1 < argc )
              {
                 if ( strchr(argv[ii+1],'-') != NULL )
                 {
                    fprintf(stdout,"\nshef_decode_raw:ERROR-no value specified for -howold option\n");
                    shefclose();
                    exit(0);
                 }
                 HOW_OLD = atoi(argv[ii+1]);
              }
              else
              {
                 if ( VERBOSE ) 
                    fprintf(stdout,"\nshef_decode_raw:ERROR-no value specified for -howold option\n"); 
                 shefclose();
                 exit(0);
              }
          }

         if ( strncmp(argv[ii],"-loop",5) == 0 )              /* dgb: 03/19/97 */
         {

              if ( ii+1 < argc )
              {
                 if ( strchr(argv[ii+1],'-') != NULL )
                 {
                    fprintf(stdout,"\nshef_decode_raw:ERROR-no value specified for -loop option\n");
                    shefclose();
                    exit(0);
                 }
                 NUM_EXECUTIONS = atoi(argv[ii+1]);
              }
              else
              {
                 if ( VERBOSE ) 
                    fprintf(stdout,"\nshef_decode_raw:ERROR-no value specified for -loop option\n"); 
                 shefclose();
                 exit(0);
              }
          }




          if ( strncmp(argv[ii],"-t",2) == 0 )
          {
            test_.test_flag = 1;
          }
         
       }
        
       if ( test_.test_flag )
       {
            shefopen();	   
            shef_test();
	
	    shefclose();
            exit(0);
       }
    
}

print_usage()
{
   /* dgb:10/10/95 - print_usage function added */
   fprintf(stdout,
      "\n\nshef_decode_raw V1.0 - FEB 10, 2000 - NOAA/NWS/CBRFC \n"); 
   fprintf(stdout,
      "shef_decode -t -at -d -v -c CCYR -p -q -o -fcfg filename -howold #  -loop #\n");
   fprintf(stdout,"%s %s %s %s %s %s %s %s %s %s %s %s\n\n",
	  "where:\n",
      "   -t             execute test version\n",
      "   -at            turn on advanced testing\n",
      "   -d             turn on debug information\n",
      "   -v             turn on verbose option\n",
      "   -c             CCYRMMDD  override system date with switch for testing\n",
      "   -p             only write lines with errors when parsing shef\n",
      "   -q             assume the revision flag is on permanently\n",
      "   -o             run decoder/poster in continuous mode\n",
      "   -fcfg filename input configuration path/filename from command line\n",
      "   -howold #      number of minutes to wait before processing file (default=1)\n",
      "   -loop #        where # equals the number of iterations (files processed) before stopping (default=20)\n\n");
      
      
 	exit(0) ;
}

/* ********************************************************* */
