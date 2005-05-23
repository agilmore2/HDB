   
#include  <stdio.h>
#include  <stdlib.h>
#include  <time.h>
#include  <string.h>


int error_log(char *pname,FILE *fp_log,int pflag,int eflag,
          char *s1,char *s2,char *s3,char *s4,char *s5)


{


	unsigned char mon_name[12][4] = {
		{"Jan"}, {"Feb"}, {"Mar"}, {"Apr"}, {"May"}, {"Jun"},
		{"Jul"}, {"Aug"}, {"Sep"}, {"Oct"}, {"Nov"}, {"Dec"}
	};

    char buf[6][80],emsg[80];
	time_t  now;
	struct tm *tp;
    signed long offset;
    int i;


/*
    if ( fp_log == NULL )
         return(1);
*/
	/* Get time of error message */

	now = time(NULL);
	tp  = gmtime(&now);

    for ( i=0; i < 6; i++)
          buf[i][0]=0;

    if(s1 != NULL)
              strcpy(buf[0],s1);  

                  
    if(s2 != NULL)

                 strcpy(buf[1],s2);
                 

    if(s3 != NULL) 

                 strcpy(buf[2],s3);

                

    if(s4 != NULL) 
 
                 strcpy(buf[3],s4);

                
    if(s5 != NULL) 

                 strcpy(buf[4],s5);

                

	/* Format error message */

	sprintf(emsg, "\n\n%3s %2d %2d %2d:%02d  program:%-16s",
		mon_name[tp->tm_mon], tp->tm_mday, tp->tm_year,
		tp->tm_hour, tp->tm_min, pname);


    /*  write to standard error device if true */
    if ( pflag == 1 )
    {    
       fprintf(stdout,"\n%s",emsg);

       i = 0;
       while ( buf[i][0] != 0 )
       {
          fprintf(stdout,"\n    %s",buf[i]);
          i++;
       }
    }

   /*  write to log if you have a file pointer */
    if ( fp_log != NULL )
    {
       fprintf(fp_log,"\n%s",emsg);

       i = 0;
       while ( buf[i][0] != 0 )
       {
          fprintf(fp_log,"\n    %s",buf[i]);
          i++;
       }
   
    }


    if ( eflag )
    {
       if ( fp_log )
          fclose(fp_log);
       exit(1);
    }
  
    return (1);

}                                                     
