#include <stdio.h>
#include <stdlib.h>
#include <time.h>

date_time(mon,day,year,hour,min,sec)
short int *mon, *day, *year, *hour, *min, *sec;
{
  int i;
  time_t cal_time;
  struct tm *bd_time;
  cal_time = time(NULL);
  bd_time = gmtime(&cal_time);

/*   fprintf(stdout,"%02d %02d %02d %02d %02d\n"
        ,bd_time->tm_mon+1
        ,bd_time->tm_mday
        ,bd_time->tm_year
        ,bd_time->tm_hour
        ,bd_time->tm_min);
*/

 *mon =   bd_time->tm_mon+1;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     *day =   bd_time->tm_mday;
 *year =  bd_time->tm_year + 1900;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               *hour = bd_time->tm_hour; 
 *min = bd_time->tm_min; 
 *sec = bd_time->tm_sec; 
 }
