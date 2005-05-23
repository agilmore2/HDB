/*Translated by FOR_C, v3.4.2 (-), on 04/06/95 at 14:16:27 */
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
         CHECKPARM                 
  
       PURPOSE                                                    
         To validate the PEDSTEP codes read into memory from inputparm
  
       VERSION and UPDATES
         1.0    MAY 94   David G. Brandon
                Original Version
                Also translated to 'C' using FOR_C
         1.1    OCT 95   DGB
                Change from fixed array sizes to values read out of the
                header file.  Add iscore to t_codes array.
         1.2    JAN 96  DGB
                Remove the FORC functions such as f_strncpy and 
                replace them with standard C functions.
 *--------------------------------------------------------------------- */

void checkparm(kodp, kode, kodd, kodt, kods, kodex, kodpr, value, ival, inum)
short int *kodp, *kode, *kodd, *kodt, *kods, *kodex, *kodpr;
double *value;
short int *ival, *inum;
{
	static char headr[3];
	static short int i;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Durvalue = &parms2_.durvalue[0] - 1;
	short *const Exvalue = &parms2_.exvalue[0] - 1;
	short *const Numval = &parms2_.numval[0] - 1;
	double *const Pevalue = &parms3_.pevalue[0] - 1;
	double *const Provalue = &parms3_.provalue[0] - 1;
	short *const Sendflag = &parms2_.sendflag[0] - 1;
	short *const Tsvalue = &parms2_.tsvalue[0] - 1;

	*value = -9.0e0;
	*ival = -9;

	/*             Validate PE code */
	if( *inum == 1 )
		{
        memset(headr,0,sizeof(headr));
        headr[0] = (char) *kodp;
        headr[1] = (char) *kode;
		i = 1;
		while( i <= Numval[1] )
			{
            if ( strcmp(headr,parms1_.peval[i-1]) == 0 )
				{
				*value = Pevalue[i];
				return;
				}
			i = i + 1;
			}
		return;

		}

	/*             Duration codes */

	if( *inum == 2 )
		{

		i = 1;
		while( i <= Numval[2] )
			{
            if ( parms1_.durval[i-1][0] == (char) *kodd )
				{
				*ival = Durvalue[i];
				return;
				}
			i = i + 1;
			}
        return;       
		}

	/*             Type Source Codes */

	if( *inum == 3 )
		{

        memset(headr,0,sizeof(headr));
        headr[0] = (char) *kodt;
        headr[1] = (char) *kods;

		i = 1;
		while( i <= Numval[3] )
			{
            if ( strcmp(headr,parms1_.tsval[i-1]) == 0 )
				{
				*ival = Tsvalue[i];
				return;
				}
			i = i + 1;
			}
		return;

		}

	/*             Extremum Codes */

	if( *inum == 4 )
		{

		i = 1;
		while( i <= Numval[4] )
			{
			if( parms1_.exval[i - 1][0] == (char)  *kodex ) 
				{
				*ival = Exvalue[i];
				return;
				}
			i = i + 1;
			}
		return;

		}

	/*             Probability Codes */

	if( *inum == 5 )
		{

		i = 1;
		while( i <= Numval[5] )
			{
			if( parms1_.proval[i - 1][0] == (char) *kodpr )
				{
				*value = Provalue[i];
				return;
				}
			i = i + 1;
			}
		return;

		}

	/*             Send Codes */

	if( *inum == 6 )
		{

        memset(headr,0,sizeof(headr));
        headr[0] = (char) *kodp;
        headr[1] = (char) *kode;
		i = 1;
		while( i <= Numval[6] )
			{
            if ( strcmp(headr,parms1_.senval[i - 1]) == 0 )
				{
				*ival = i;
				return;
				}
			i = i + 1;
			}
		return;

		}

	return;
}

