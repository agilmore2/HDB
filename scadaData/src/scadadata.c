/*************************************************************************************************************
 *  Function:  retrieve data from HDB2 and create a file to be loaded into Hydromet
 *
 *  Author:    Andrew Gilmore
 *
 *
 *  Purpose:   1. retrieve data from HDB2
 *             2. submit daily data to hydromet for processing daily report
 *
 **************************************************************************************************************/



#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stropts.h>
#include <poll.h>
#include <errno.h>
#include <sys/termios.h>
#include <ctype.h>

#define SIZE 40
#define NUMSITES 6
#define NUMDATATYPES 3
#define HYDRO "new_hydromet_\0"
int SqlGetSCADAData(char*,char*,char*);

double elev[NUMSITES], apow_rel[NUMSITES], atot_rel[NUMSITES];

int main(int argc, char **argv)
{
   char   *orgin = HYDRO;
   char   date[30];
   char   hydro[30];
   char   uchydro[30];
   char   mon[3];
   char   day[3];
   char    yr[3];

   int    j;

   double  pow_relaf[NUMSITES],tot_relaf[NUMSITES],oth_relaf[NUMSITES];

   FILE   *out;

   if (argc != 4) 
   {
     printf("Usage: <programname> app_user apppasswd MM/DD/YYYY\n");
     exit(1);
   }

   SqlGetSCADAData(argv[1], argv[2], argv[3]);

  /* convert to AF, then calculate AF of Other Release*/
   for (j=0; j<NUMSITES; j++)
   {
      pow_relaf[j] = apow_rel[j]*1.9835;
      tot_relaf[j] = atot_rel[j]*1.9835;
      
      oth_relaf[j] = tot_relaf[j] - pow_relaf[j];
   }

   strncpy(&mon[0], &argv[3][0], 2);
   mon[2]='\0';
   /*
     printf("%s\n",&mon[0]);
   */
   strncpy(&day[0], &argv[3][3], 2);
   day[2]= '\0';
   /*
     printf("%s\n",&day[0]);
   */ 
   strncpy(&yr[0], &argv[3][8], 2);
   yr[2]= '\0';
   /*
     printf("%s\n",&yr[0]);
   */   

   date[0] = '\0';

   if (strcmp(&mon[0], "11") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"NOV");  
      strcat(&date[0],&day[0]);
   }


   if (strcmp(&mon[0], "12") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"DEC");  
      strcat(&date[0],&day[0]);
   }

   if (strcmp(&mon[0], "01") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"JAN");  
      strcat(&date[0],&day[0]);
   }

   if (strcmp(&mon[0], "02") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"FEB");  
      strcat(&date[0],&day[0]);
   }

   if (strcmp(&mon[0], "03") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"MAR");  
      strcat(&date[0],&day[0]);
   }

   if (strcmp(&mon[0], "04") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"APR");  
      strcat(&date[0],&day[0]);
   }

   if (strcmp(&mon[0], "05") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"MAY");  
      strcat(&date[0],&day[0]);
   }

   if (strcmp(&mon[0], "06") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"JUN");  
      strcat(&date[0],&day[0]);
   }

   if (strcmp(&mon[0], "07") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"JUL");  
      strcat(&date[0],&day[0]);
   }

   if (strcmp(&mon[0], "08") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"AUG");  
      strcat(&date[0],&day[0]);
   }

   if (strcmp(&mon[0], "09") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"SEP");  
      strcat(&date[0],&day[0]);
   }

   if (strcmp(&mon[0], "10") == 0)
   {
      strcat(&date[0],&yr[0]);
      strcat(&date[0],"OCT");  
      strcat(&date[0],&day[0]);
   }

   hydro[0] = '\0';

   strcat(&hydro[0],orgin);
   strcat(&hydro[0],&date[0]);
   strcat(&hydro[0],".dat");

   for (j=0;j<strlen(hydro);j++)
   {
      hydro[j] = tolower(hydro[j]);
      uchydro[j] = toupper(hydro[j]);
   }
        
   uchydro[j] = '\0';
   
   printf("%s\n", &hydro[0]);

   if ((out = fopen("./dum","w+")) == NULL)
   {
      fprintf(stderr, "can't open output for dum\n");
      exit(1);
   }
   fprintf(out, "%s\n", &hydro[0]);

   fclose(out);

   if ((out = fopen("./UCdum","w+")) == NULL)
   {
      fprintf(stderr, "can't open output for UCdum\n");
      exit(1);
   }
   fprintf(out, "%s\n", &uchydro[0]);

   fclose(out);

   if ((out = fopen(&hydro[0],"w+")) == NULL)
   {
      fprintf(stderr, "can't open output for writing\n");
      exit(1);
   }
              

   for (j=0; j<NUMSITES; j++)
   {
      if (j == 0)
      {
         fprintf(out,"    %s %s      %s%8.2f  %s%8.0f  %s%8.0f\n", "GLDA",date,"FB",
                 elev[j],"PWRREL", pow_relaf[j],"OTHREL",oth_relaf[j]);
      }

      if (j == 1)
      {
         fprintf(out,"    %s %s      %s%8.2f  %s%8.0f  %s%8.0f\n", "MPRC",date,"FB",
                 elev[j],"PWRREL", pow_relaf[j],"OTHREL",oth_relaf[j]);
      }

      if (j == 2)
      {
         fprintf(out,"    %s %s      %s%8.2f  %s%8.0f  %s%8.0f\n", "BMDC",date,"FB",
                 elev[j],"PWRREL", pow_relaf[j],"OTHREL",oth_relaf[j]);
      }

      if (j == 3)
      {
         fprintf(out,"    %s %s      %s%8.2f  %s%8.0f  %s%8.0f\n", "CRRC",date,"FB",
                 elev[j],"PWRREL", pow_relaf[j],"OTHREL",oth_relaf[j]);
      }

      if (j == 4)
      {
         fprintf(out,"    %s %s      %s%8.2f  %s%8.0f  %s%8.0f\n", "FGRU",date,"FB",
                 elev[j],"PWRREL", pow_relaf[j],"OTHREL",oth_relaf[j]);
      }

      if (j == 5)
      {
         fprintf(out,"    %s %s      %s%8.2f  %s%8.0f  %s%8.0f\n", "FTRW",date,"FB",
                 elev[j],"PWRREL", pow_relaf[j],"OTHREL",oth_relaf[j]);
      }

   }

   fclose(out);

} /* end of main */

