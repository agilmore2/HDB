/*************************************************************************************************************
 *  Function:  retrieve data from HDB2 and create a continuous GC power release
 *   and calculate GC total release
 *
 *  Author:    Andrew Gilmore
 *
 *
 *  Purpose:   1. retrieve data from HDB2
 *             2. check continuity of AVM data
 *             3. fill in holes, or NOT?
 *             4. write GC power release
 *             5. compute GC total release
 *             6. write GC total release
 *
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
#include "typedefs.h"
#include "utils.h"
#include "defines.h"
#include "glenpowrelease.h"

double curve_rel[HOURS], avm_rel[HOURS], spill[HOURS], bypass[HOURS];

int main(int argc, char **argv)
{
   char   date[30];
   char   hour[6];
   int    i,j, missing_data=0;
   char   val_flag[] = "Z";
   
   double  tot_rel[HOURS];

   SQL_DATE formattedDates[HOURS];

   strncat(date,argv[3],11);

   for (i=0; i<HOURS; i++)
   {
      strcpy(formattedDates[i],date);
      snprintf(hour,6," %d",i);
      strcat(formattedDates[i],hour);
   }

   if (argc != 4) 
   {
     printf("Usage: <programname> app_user app_passwd YYMONDD\n");
     exit(1);
   }

   SqlGetAVMData(argv[1], argv[2], argv[3]);
   /*
     Now, we must determine where the holes in the data are, and
     whether or not to fill in the AVM data with the SCADA data

     for now, assume avm data is fine
   */

   /* calculate Total Release*/
   for (j=0; j<HOURS; j++)
   {
      tot_rel[j] = spill[j] + bypass[j] + avm_rel[j];
   }

   date[0] = '\0';


   /* Now, write data to database. Perhaps we should allow the new
      power release data to run through the derivation application
      before calculating total releases based on it?
   */

   for (j=0; j<HOURS; j++)
   {
      printf("%s %d: %s %.0f %s %.0f %s %.0f %s %.0f %s %.0f\n", date, j,
             "curve", curve_rel[j],"AVM",avm_rel[j], "total", tot_rel[j],
             "spill", spill[j], "bypass", bypass[j]);
   }
   
   insertAVM(formattedDates, 1862,avm_rel,val_flag);
   insertAVM(formattedDates, 1872,tot_rel,val_flag);   

   exit(0);


} /* end of main */

