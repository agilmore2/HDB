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
#define HYDRO "hydromet_\0"
int SqlGetSCADAData(char*,char*,char*);

double elev[NUMSITES], apow_rel[NUMSITES], aspill[NUMSITES], abypass[NUMSITES];

int main(int argc, char **argv)
{
   char   *orgin = HYDRO;
   char   date[30];
   char   hydro[30];
   char   lchydro[30];
   char   mon[3];
   char   day[3];
   char    yr[3];

   int    j;

   double  pow_relaf[NUMSITES],spillaf[NUMSITES],bypassaf[NUMSITES],
     oth_relaf[NUMSITES];

   FILE   *out;

   if (argc != 4) 
   {
     printf("Usage: <programname> app_user app_passwd YYMONDD\n");
     exit(1);
   }

   SqlGetSCADAData(argv[1], argv[2], argv[3]);

  /* convert to AF, then calculate AF of Other Release*/
   for (j=0; j<NUMSITES; j++)
   {
      pow_relaf[j] = apow_rel[j]*1.9835;
      spillaf[j] = aspill[j]*1.9835;
      bypassaf[j] = abypass[j]*1.9835;
      
      oth_relaf[j] = spillaf[j] + bypassaf[j];
/* NASTY HACK to handle Morrow Point releases calculated by Hydromet */
/* Floating Point comparison?, hope it works! */
      if (apow_rel[j] == 998877)   pow_relaf[j] = 998877;
   }

   date[0] = '\0';

   strcat(&date[0],argv[3]);

   hydro[0] = '\0';

   strcat(&hydro[0],orgin);
   strcat(&hydro[0],&date[0]);
   strcat(&hydro[0],".dat");

   for (j=0;j<strlen(hydro);j++)
   {
      lchydro[j] = tolower(hydro[j]);
   }
        
   lchydro[j] = '\0';
   
   printf("%s\n", &hydro[0]);

   if ((out = fopen("./hydro_file","w+")) == NULL)
   {
      fprintf(stderr, "can't open output for hydro_file\n");
      exit(1);
   }
   fprintf(out, "%s\n", &hydro[0]);

   fclose(out);

   if ((out = fopen("./LChydro_file","w+")) == NULL)
   {
      fprintf(stderr, "can't open output for LChydro_file\n");
      exit(1);
   }
   fprintf(out, "%s\n", &lchydro[0]);

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

