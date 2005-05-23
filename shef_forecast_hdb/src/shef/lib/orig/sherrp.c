/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 17:02:13 */
/*FOR_C Options SET: c=2 com=u do=r4 ftn=2ln6k op=iv s=dvn str=l sq=i */
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
         SUBROUTINE SHERRP(IER)
  
       PURPOSE                                                    
         Print out error messages from sherr
  
       VERSION and UPDATES
         1.0    MAY 94   David G. Brandon 
                Original Version
                Also translated to 'C' using FOR_C
         1.1    JAN 24 96  DGB
                Increase message5 array to 63.
                Add message warning 39.  
         1.2    SEP 9 97 DGB
                Add printing capabilities for errors number
                39, 40.
         1.3    OCT 19 97 DGB
                Add an additional piece ( message221 ) to message22.
  --------------------------------------------------------------------- */


void sherrp(ier)
short int *ier;
{

	if( *ier == 1 )
		{
        fprintf(fp_.icher," %s\n",err1_.message1);
		return;
		}

	if( *ier == 2 )
		{
        fprintf( fp_.icher, " %s\n", err1_.message2 );
		return;
		}


	if( *ier == 3 )
		{
		fprintf( fp_.icher," %s\n", err1_.message3 );
		return;
		}


	if( *ier == 4 )
		{
		fprintf( fp_.icher, " %s\n", err1_.message4 );
		return;
		}


	if( *ier == 5 )
		{
		fprintf( fp_.icher, " %s\n", err1_.message5 );
		return;
		}

	if( *ier == 6 )
		{
		fprintf( fp_.icher, " %s\n", err2_.message6 );
		return;
		}

	if( *ier == 7 )
		{
		fprintf( fp_.icher, " %s\n", err2_.message7 );
		return;
		}


	if( *ier == 8 )
		{
		fprintf( fp_.icher, " %s\n", err2_.message8 );
		return;
		}

	if( *ier == 9 )
		{
		fprintf( fp_.icher, " %s\n", err2_.message9 );
		return;
		}

	if( *ier == 10 )
		{
		fprintf( fp_.icher, " %s\n",  err2_.message10 );
		return;
		}

	if( *ier == 11 )
		{
		fprintf( fp_.icher, " %s\n", err3_.message11 );
		return;
		}

	if( *ier == 12 )
		{
		fprintf( fp_.icher, " %s\n", err3_.message12 );
		return;
		}

	if( *ier == 13 )
		{
		fprintf( fp_.icher, " %s\n", err3_.message13 );
		return;
		}

	if( *ier == 14 )
		{
		fprintf( fp_.icher, " %s\n",err3_.message14 );
		return;
		}

	if( *ier == 15 )
		{
		fprintf( fp_.icher, " %s\n", err3_.message15 );
		return;
		}

	if( *ier == 16 )
		{
		fprintf( fp_.icher, " %s\n", err3_.message14 );
		return;
		}

	if( *ier == 17 )
		{
		fprintf( fp_.icher, " %s\n", err4_.message17 );
		return;
		}

	if( *ier == 18 )
		{
		fprintf( fp_.icher, " %s\n", err4_.message18 );
		return;
		}

	if( *ier == 19 )
		{
		fprintf( fp_.icher, " %s\n", err4_.message19 );
		return;
		}

	if( *ier == 20 )
		{
		fprintf( fp_.icher, " %s\n", err4_.message20 );
		return;
		}

	if( *ier == 21 )
		{
		fprintf( fp_.icher, " %s\n", err5_.message21 );
		return;
		}

	if( *ier == 22 )
		{
		fprintf( fp_.icher, " %s\n %s\n",err5_.message22,
                                         err5_.message221 );
		return;
		}

	if( *ier == 23 )
		{
		fprintf( fp_.icher, " %s\n", err5_.message23 );
		return;
		}

	if( *ier == 24 )
		{
		fprintf( fp_.icher, " %s\n", err5_.message24 );
		return;
		}

	if( *ier == 25 )
		{
		fprintf( fp_.icher, " %s\n", err5_.message25 );
		return;
		}

	if( *ier == 26 )
		{
		fprintf( fp_.icher, " %s\n",err6_.message26 );
		return;
		}

	if( *ier == 27 )
		{
		fprintf( fp_.icher, " %s\n",err6_.message27 );
		return;
		}

	if( *ier == 28 )
		{
		fprintf( fp_.icher, " %s\n", err6_.message28 );
		return;
		}

	if( *ier == 29 )
		{
		fprintf( fp_.icher, " %s\n", err6_.message29 );
		return;
		}

	if( *ier == 30 )
		{
		fprintf( fp_.icher, " %s\n", err6_.message30 );
		return;
		}

	if( *ier == 31 )
		{
		fprintf( fp_.icher, " %s\n", err7_.message31 );
		return;
		}

	if( *ier == 32 )
		{
		fprintf( fp_.icher, " %s\n", err7_.message32 );
		return;
		}

	if( *ier == 33 )
		{
		fprintf( fp_.icher, " %s\n", err7_.message33 );
		return;
		}

	if( *ier == 34 )
		{
		fprintf( fp_.icher, " %s\n", err7_.message34 );
		return;
		}

	if( *ier == 35 )
		{
		fprintf( fp_.icher, " %s\n", err7_.message35 );
		return;
		}

	if( *ier == 36 )
		{
		fprintf( fp_.icher, " %s\n", err8_.message36 );
		return;
		}

	if( *ier == 37 )
		{
		fprintf( fp_.icher, " %s\n %s\n", err8_.message37
		 , err8_.message371 );
		return;
		}

	if( *ier == 38 )
		{
		fprintf( fp_.icher, " %s\n", err8_.message38);
		return;
		}

	if( *ier == 39 )                                       /* dgb:09/10/97 */
		{
		fprintf( fp_.icher, " %s\n", err8_.message39);
		return;
		}

	if( *ier == 40 )                                       /* dgb:09/10/97 */
		{
		fprintf( fp_.icher, " %s\n", err8_.message40);
		return;
		}

	return;

} 
