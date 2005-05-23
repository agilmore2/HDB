/*Translated by FOR_C, v3.4.2 (-), on 04/06/95 at 13:53:50 */
/*FOR_C Options SET: c=2 com=u do=r44 ftn=ln6kk op=iv s=dvnk str=l sq=i */
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
  
       NAME                              
         SUBROUTINE IRANG(I,MIN,MAX,STATUS)
  
       PURPOSE                                                    
         To check if an integer is in the range MIN <= I <= MAX
  
       VERSION and UPDATES
         1.0    APR 82   Geoffrey M. Bonnin
                Original Version
         1.1    JUN 89   David G. Brandon
                Adapted to QNX NWS Hydromet
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
   
  ---------------------------------------------------------------------*/

void irang(i, min_, max_, status)
short int *i, *min_, *max_, *status;
{

	*status = 0;
	if( *i < *min_ )
		*status = 1;
	if( *i > *max_ )
		*status = 1;

	return;

} 

