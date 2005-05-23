/*
 ---------------------------------------------------------------------
   
       NAME
         stats_utilities
  
       PURPOSE                                              
         Collection of functions to be used with the shef_decode to
         account for statistics.
  
       VERSION and UPDATES
         1.0    Feb  95   David G. Brandon
         1.1    SEP  95   DGB
                Use 'strstr' for routines when accounting for type/source.      
         1.2    OCT  95   DGB
                Read only directory in shef_.files_log and not the 
                directory and the filename. Filename now always 'stats'.
                Add arrays of TS and TSNAMEs to automatically construct
                headers.  This is is utility, stats_print.  Change 
                stats_read to use strtok for more robust reading of 
                file.  Add the stop/start time in number of seconds 
                for easier manipulation.
          1.3   JUL 15 96 DGB
                Add a few exit(0) statements on file open errors.
          1.4   JUL 20 96 DGB
                Change SAO to MTR for metar reports.
          1.5   AUG 9 96 DGB
                Change RS to RO for ASOS type obs.  Add FA and FU
                (future model adjusted and future model unadjusted)
                to type source list.
          1.6   AUG 12 96 DGB
                Remove check for MTR/SAO products changing all to RV.
                Now RV and RO are separate and distinguishable.
          1.7   SEP 10 96 DGB
                Hold on a little partner...change MTR to RV for
                stats purposes until everything can be settled in 
                the database.
          1.8   SEP 22 96 DGB
                Include shef_structs_external from a file instead of
                hard coded.
                Include new option to log and print out the time
                of the last posting by TS codes.  For example, the
                last time that a domsat report was posted, or a metar
                report was posted.
          1.9   NOV 04 96 DGB
                Make changes to the quality flag fiddling around
                utilitity for accounting of domsat and hads reports..
                i.e. change checking of lwal flag from S to D and
                from F to A.
          2.0   MAR 18 97 DGB
                Add checking stats for PE codes and TS codes that
                are recevied but not located in the *.cfg file.
                Any unknown PE code will be summed in 'ZZ' and
                and unknown TS code will be summed in 'TS'. 
          2.1x  SEP 10 97 DGB
                Add the use of MAX_SHEF_INPUT so that the input line can
                be longer than 80 characters.
          2.2   JAN 3 99 DGB
                Change quality 'A' to 'Y'.

 ---------------------------------------------------------------------
 */
#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include "shef_structs_external.h"


void stats_init( char * logname )
{
   int i, k, ii, flag;
   char buff_temp[MAX_SHEF_INPUT], buffer[MAX_SHEF_INPUT];  /* dgb:09/10/97 */
   struct tm current, start;
   struct stat buf;

         /*
          *  Get/set time variables.
          */

          stats_.time_current   = time(NULL);
          current = *gmtime(&stats_.time_current);  

          strftime(buff_temp,sizeof(buff_temp),
                "/stats.%m%d.000000",
                  gmtime(&stats_.time_current));
          sprintf(buffer,"%s%s",logname,buff_temp);      /* dgb:10/01/95 add stats name */

          /*  See if file exists..if yes open it.  If not */
          /*  open a new file with the new day.           */

          /*  Open previous stats file                    */
          if ( stat(buffer, &buf) != -1 )
          {            

              if ( (fp_postlog = fopen(buffer,"r+w+")) == NULL )
              {
                  fprintf(stdout,"shef_decode:error on opening stats file->%s",
                          buffer);
                  exit(0);                                 /* dgb:07/15/96 */
              }
              /*  Read in previous stats file */
                  stats_.flag = 1;
                  stats_read();
                  stats_print();
	      }
          else
          {
              /* Open a new file for the day                */

              if ( (fp_postlog = fopen(buffer,"wr")) == NULL )
              {
                  stats_.time_start_log = stats_.time_current;
                  stats_.time_stop_log  = stats_.time_current;
                  stats_.flag = 0;
                  stats_print();
                  fprintf(stdout,"shef_decode:error on opening stats file->%s",
                          buffer);
                  exit(0);                                 /* dgb: 07/15/96 */
              }
              stats_.time_start_log = stats_.time_current;

              for ( k = 0; k < stats_.num_ts; k++)         /* dgb:09/22/96 */
                     strcpy(&stats_.ts_post_time[k][0],"  00:00");

              /*  Clear stats arrays */
              for ( i = 0; i < MAX_ACCEPTABLE_PE+1; i++)
              {
                 for ( k = 0; k < MAX_ACCEPTABLE_TS+1; k++ )
                       stats_.post_stats[ i ][ k ] = 0;
              }
         
              for ( k = 0; k < MAX_ACCEPTABLE_CATEGORIES+1; k++) 
                    stats_.parse_stats[ k ] = 0;
          }
 }

stats_print()
{
   char *ts[40], *tsname[40];
   int i, k, flag;
   char buff_temp[200];

   char *tsname_default = "UNKNWN";
   fpos_t position;
   long endfil;
   struct tm current, start;

   ts[0] = "RM";  tsname[0] = "SNOTEL";
   ts[1] = "RG";  tsname[1] = "S.DOM";
   ts[2] = "RF";  tsname[2] = "R.DOM";
   ts[3] = "RH";  tsname[3] = "HADS";
   ts[4] = "RO";  tsname[4] = "ASOS";      /* dgb:08/09/96 */
   ts[5] = "RV";  tsname[5] = "MTR";       /* dgb:07/20/96 */
   ts[6] = "R4";  tsname[6] = "ROSA";
   ts[7] = "RR";  tsname[7] = "ALERT";
   ts[8] = "RP";  tsname[8] = "LARCS";     /* dgb:11/04/96 */
   ts[9] = "R1";  tsname[9] = "RR #";
   ts[10]= "FZ";  tsname[10]= "FUTURE";
   ts[11]= "RZ";  tsname[11]= "DEFAULT";
   ts[12]= "1M";  tsname[12]= "P1SNOW";
   ts[13]= "2M";  tsname[13]= "P2SNOW";
   ts[14]= "3M";  tsname[14]= "P3SNOW";
   ts[15]= "1G";  tsname[15]= "P1GOES";
   ts[16]= "2G";  tsname[16]= "P2GOES";
   ts[17]= "3G";  tsname[17]= "P3GOES";
   ts[18]= "FA";  tsname[18]= "F  ADJ";    /* dgb:08/09/96 */
   ts[19]= "FU";  tsname[19]= "F UADJ";    /* dgb:08/09/96 */
   ts[20]= NULL;
   ts[21]= NULL;
   ts[22]= NULL;
   ts[23]= NULL;
   ts[24]= NULL;
   ts[25]= NULL;
   ts[26]= NULL;
   ts[27]= NULL;
   ts[28]= NULL;
   ts[29]= NULL;
   ts[30]= NULL;
   ts[31]= NULL;
   ts[32]= NULL;
   ts[33]= NULL;
   ts[34]= NULL;
   ts[35]= NULL;
   ts[36]= NULL;
   ts[37]= NULL;
   ts[38]= NULL;
   ts[39]= NULL;

   /*   Place file pointer at the beginning of the log  */

        rewind(fp_postlog);  
        stats_.time_current   = time(NULL);
        current = *gmtime(&stats_.time_current);  


        fprintf(fp_postlog,"\n  LOG FOR SHEF PARSING & POSTING - Program: %s  ",
                            pname);
        if ( stats_.flag == 1 )
        {
            strftime(buff_temp,sizeof(buff_temp),
                     "%H:%M %D",gmtime(&stats_.time_start_log));                                                                                                 
             fprintf(fp_postlog,
                     "\n  LOG_START_TIME------------->  %s   (%ld)",
                     buff_temp, stats_.time_start_log);
        }
        else
        {
           strftime(buff_temp,sizeof(buff_temp),"%H:%M %D",
                    gmtime(&stats_.time_start_log));
             fprintf(fp_postlog,
                     "\n  LOG_START_TIME------------->  %s   (%ld)",
                     buff_temp, stats_.time_start_log);
        }
        strftime(buff_temp,sizeof(buff_temp),"%H:%M %D",
                 gmtime(&stats_.time_current));
        fprintf(fp_postlog,"\n  LOG_CURRENT_TIME----------->  %s   (%ld)",
                 buff_temp, stats_.time_current);

        fprintf(fp_postlog,"\n\n< PARSING STATISTICS >");
        fprintf(fp_postlog,"\n  #PRODUCTS      #RECORDS      #ERRORS     #WARNINGS");
        fprintf(fp_postlog,"\n   %8ld      %8ld     %8ld      %8ld",
                stats_.parse_stats[0],
                stats_.parse_stats[1],
                stats_.parse_stats[2],
                stats_.parse_stats[3]);

        fprintf(fp_postlog,"\n\n< POSTING STATISTICS >\n   ");

        for ( i = 0; i < stats_.num_ts; i++ )
        {
            flag = 0;
            k = 0;
            while ( ts[k] != NULL )
            {
               if ( strcmp(stats_.acceptable_ts[i],ts[k]) == 0 )
               {
                    flag = 1;
                    break;
               }
               k++;
            }

            if ( !flag )
                    fprintf(fp_postlog," %s",tsname_default);
            else
                    fprintf(fp_postlog,"%7.7s",tsname[k]);
 
            flag = 0;

        }
        
        fprintf(fp_postlog,"\nPE ");


        for ( i = 0; i < stats_.num_ts; i++ )
              fprintf(fp_postlog,"     %-2.2s",stats_.acceptable_ts[i]);
        fprintf(fp_postlog,"  TOTALS");

        for ( i = 0; i < stats_.num_pe; i++ )
        {

           stats_.post_stats[i][stats_.num_ts] = 0; 
           fprintf(fp_postlog,"\n%-3.3s", stats_.acceptable_pe[i]);
           for ( k = 0; k < stats_.num_ts; k++) 
           {
              stats_.post_stats[i][stats_.num_ts] = 
              stats_.post_stats[i][stats_.num_ts] + 
              stats_.post_stats[i][k];
              fprintf(fp_postlog," %6ld", stats_.post_stats[i][k]);
           }

           fprintf(fp_postlog,"  %6ld",stats_.post_stats[i][stats_.num_ts]);
        }

        for ( i = 0; i < stats_.num_ts+1; i++ )
        {
            stats_.post_stats[stats_.num_pe][i] = 0;  
            for ( k = 0; k < stats_.num_pe; k++)
            {
               stats_.post_stats[stats_.num_pe][i] = 
               stats_.post_stats[stats_.num_pe][i] +
               stats_.post_stats[k][i];
            }
        }

        fprintf(fp_postlog,"\n   ");

        for ( k = 0; k < stats_.num_ts; k++)
           fprintf(fp_postlog," %6ld",stats_.post_stats[stats_.num_pe][k]);
        fprintf(fp_postlog," %7ld",stats_.post_stats[stats_.num_pe][k]);


        /* print last post update-time in Zulu for each TS */
        /* following code was added dgb:09/22/96 */
        fprintf(fp_postlog,"\n\n   ");
        for ( k = 0; k < stats_.num_ts; k++)
           fprintf(fp_postlog,"%-7.7s",&stats_.ts_post_time[k][0]);
        fprintf(fp_postlog,"\n");

        /* set file pointer to end of file */
        fseek(fp_postlog,0L,SEEK_END);
}

int stats_updates(  char kodp, char kode, char kodd, char kodt, char kods )
{
   int i, k,  len;
   int found;                                              /* dgb:03/18/97 */
   char buff_pe[PE_SIZE], buff_ts[TS_SIZE];
   char buff_temp[10];                                     /* dgb:09/22/96 */


        if ( DEBUG ) 
        {
             fprintf(stdout,"\n     stats_updates:begin");
             fprintf(stdout,
             "\n     kodp = %c kode = %c kodd = %c  kodt = %c kods = %c", kodp,kode,kodd,kodt,kods);
        }
        
        buff_pe[0] =  kodp;
        buff_pe[1] =  kode;
        buff_pe[2] =  kodd;
        buff_pe[3] =  0;
        buff_ts[0] =  kodt;
        buff_ts[1] =  kods;
        buff_ts[2] =  0;

        /* look to see if pe is in list...if not assign it ZZ */
        found = 0;
        for ( i = 0; i < stats_.num_pe; i++)
        {
           if ( strlen(stats_.acceptable_pe[i]) == 3 )
                len = 3;
           else
                len = 2;
           if ( strncmp(stats_.acceptable_pe[i],buff_pe,len) == 0 ) 
           {
                  found = 1;
                  i = stats_.num_pe;
           }
        }
        if ( !found )
        {
                  buff_pe[0] = 90;
                  buff_pe[1] = 90;
                  buff_pe[2] = 90;
                  buff_pe[3] = 0;
        } 

        for ( i = 0; i < stats_.num_pe; i++)
        {
           if ( strlen(stats_.acceptable_pe[i]) == 3 )      /* dgb:10/26/95 */
                len = 3;
           else
                len = 2;
                
           if ( strncmp(stats_.acceptable_pe[i],buff_pe,len) == 0 ) 
           {

              found = 0;
              for ( k = 0; k < stats_.num_ts; k++ ) 
              {
                 if ( strcmp(stats_.acceptable_ts[k],buff_ts) == 0 ) 
                 {
                       stats_.post_stats[i][k]++;
                       strftime(buff_temp,sizeof(buff_temp),"  %H:%M", gmtime(&stats_.time_current));
                       strcpy(&stats_.ts_post_time[k][0],buff_temp); /* dgb:09/22/96 */
                       found = 1;                          /* dgb:03/18/97 */
                       k = stats_.num_ts;                  /* dgb:03/18/97 */
                 }
              }  
              if ( !found )                                /* dgb:03/18/97 */
              {
                 strcpy(buff_ts,"RZ");                     /* dgb:03/18/97 */
                 for ( k = 0; k < stats_.num_ts; k++ ) 
                 {
                    if ( strcmp(stats_.acceptable_ts[k],buff_ts) == 0 ) 
                    {
                       stats_.post_stats[i][k]++;
                       strftime(buff_temp,sizeof(buff_temp),"  %H:%M", gmtime(&stats_.time_current));
                       strcpy(&stats_.ts_post_time[k][0],buff_temp); /* dgb:09/22/96 */
                       return(0);
                    }
                 }  
              }
              return(0);
           }
        }

return(0);

}

stats_read(void)

{
   int i, k, month, day, year, ztime, find_data;
   char buffer[200];
   char zbuff[8];
   char *c, *str1, *str2;
   fpos_t position;
   long endfil;
   struct tm tempdate;

   /*   Place file pointer at the beginning of the log  */
        memset(stats_.dtg,0,sizeof(stats_.dtg));


        find_data = 0;
        /*  find start at string -> .begin */
        while ( fgets(buffer,sizeof(buffer),fp_postlog) != NULL )
        { 

           if ( strstr(buffer,"START_") != NULL )
           {
               if ( (c = strchr(buffer,'(')) != NULL )
               {
                   stats_.time_start_log = atol(c+1);
                   strftime(stats_.dtg,sizeof(stats_.dtg),
                   "%H:%M %D",gmtime(&stats_.time_start_log));                                                                                                 

               }
           }
           if ( strstr(buffer,"#PROD") != NULL )
           {
               if ( fgets(buffer,sizeof(buffer),fp_postlog) == NULL )
                    break;
               else
               {
                  sscanf(buffer,"%ld %ld %ld %ld",
                  &stats_.parse_stats[0],
                  &stats_.parse_stats[1],
                  &stats_.parse_stats[2],
                  &stats_.parse_stats[3]);
               }
           }
           if ( strstr(buffer,"PE ") != NULL )
           {
              for ( i = 0; i < stats_.num_pe; i++ )
              {
                 fgets(buffer,sizeof(buffer),fp_postlog);
                 str2 = buffer;
                 for ( k = 0; k < stats_.num_ts + 1 ; k++)
                 {
                    str1=strtok(str2," \t\n\r");
                    str2=0;
                    if ( k != 0 )
                       stats_.post_stats[i][k-1] = atoi(str1);
                 }
              }

              /* read update times each TS */
              /* following code was added dgb:09/22/96 */
              memset(buffer,0,sizeof(buffer));

              fgets(buffer,sizeof(buffer),fp_postlog);
              fgets(buffer,sizeof(buffer),fp_postlog);
              fgets(buffer,sizeof(buffer),fp_postlog);

              str2 = buffer;
              for ( k = 0; k < stats_.num_ts; k++)
              {
                 str1 = strtok(str2," \t\n\r");
                 str2 = 0;
                    sprintf(&stats_.ts_post_time[k][0],"  %-5.5s",str1);
              }
           } 
        }
}

void stats_ts( char *t, char *s , char *lwal)
{


    if ( DEBUG ) 
    {
         fprintf(stdout,"\n     stats_ts:begin");
         fprintf(stdout,"\n     t = %c  s = %c  q = %c",*t,*s,*lwal);
    }
    
   /* 
      Play around with type/source and possibly change base
      upon the product name in which the data are received.
      This is necessary, since some systems/sensors send
      only the default type/source codes.
      
   */


   /*  
      GOES data  ...  keep routine reports as 'RG'
      for accounting purposes, convert random transmissions
      to 'RF'.
   */


   if ( *t == 'R' && *s == 'G' )
   {
       if ( *lwal == 'Y' )      /* dgb:01/03/99 */
          *s = 'F';
       else
       if ( *lwal == 'D' )      /* dgb:11/04 96 */
          *s = 'G';
       else
          *s = 'H';
   }

   else

  /* MTR for METAR Products */
  if ( strstr(stats_.product_name,"MTR") != 0 ) 
  {
      *t = 'R';
      *s = 'V';
  } 
  

   /*  RR1 Data products */
   if ( strstr(stats_.product_name,"RR1") != 0 ) /* dgb:09/26/95 change to strstr */
   {
       *t = 'R';
       *s = '1';
   }

   else

   /*  RR2 Data products */
   if ( strstr(stats_.product_name,"RR2") != 0 ) /* dgb:09/26/95 change to strstr */
   {
       *t = 'R';
       *s = '1';
   }

    else
   /*  ROSA RR3 Data products */
   if ( strstr(stats_.product_name,"RR3") != 0 ) /* dgb:09/26/95 change to strstr */
   {
       *t = 'R';
       *s = '4';
   }

    else
   /*  ROSA RR4 Data products */
   if ( strstr(stats_.product_name,"RR4") != 0 ) /* dgb:09/26/95 change to strstr */
   {
       *t = 'R';
       *s = '4';
   }
    
    else
   /*  ASOS RR6 Data products */
   if ( strstr(stats_.product_name,"RR6") != 0 )  /* dgb:09/26/95 change to strstr */
   {
       *t = 'R';
       *s = 'S';
   }
   
   else
   
   /*  ASOS RR7 Data products */
   if ( strstr(stats_.product_name,"RR7") != 0 )  /* dgb:09/26/95 change to strstr */
   {
       *t = 'R';
       *s = 'S';
   }

     else
   /*   RR8 products */
   if ( strstr(stats_.product_name,"RR8SLR") != 0 )  /* dgb:09/26/95 change to strstr */
   {
       *t = 'R';
       *s = 'V';
   }

 
}

