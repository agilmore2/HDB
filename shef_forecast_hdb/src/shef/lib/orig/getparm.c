/*Translated by FOR_C, v3.4.2 (-), on 04/06/95 at 14:16:22 */
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
         SUBROUTINE GETPARM                    
  
       PURPOSE                                                    
         Read in the file inputparm into memeory 
  
       VERSION and UPDATES
         1.0    MAY 94   David G. Brandon
                Original Version
                Aslo translated to 'C' using FOR_C
         1.1    APR 95   David G. Brandon
                Removed some of the I/O functions created by the FORTRAN_C
                translator and replaced them with standard 'C' 
                functions.
         1.1    OCT 95   DGB
                Change from fixed array sizes to values read out of the
                header file.  Add iscore to t_codes array;
         1.3    JAN 96   DGB
                Remove fixed format scanf and replace with fgets
                for more robust method of reading in parameter file.
 *--------------------------------------------------------------------- */


void  getparm()
{
    char *str1, *str2;
	static char headr[3];
	static int notfound;
	static short int i, i_, ii, ndurval, nexval, npeval, nproval, nsenval, 
	 ntsval;
    char temp_buff[20], buff_temp[50];

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Durvalue = &parms2_.durvalue[0] - 1;
	short *const Exvalue = &parms2_.exvalue[0] - 1;
	short *const Numval = &parms2_.numval[0] - 1;
	double *const Pevalue = &parms3_.pevalue[0] - 1;
	double *const Provalue = &parms3_.provalue[0] - 1;
	short *const Sendflag = &parms2_.sendflag[0] - 1;
	short *const Tsvalue = &parms2_.tsvalue[0] - 1;

	/*             
    Initialize Max Number or Values in inputparm file
	These values, as well as the dimensions above will
	need to be changed if additional values are used. 
     */

	npeval  = NUM_PEVAL;
	ndurval = NUM_DURVAL;
	ntsval  = NUM_TSVAL;
	nexval  = NUM_EXVAL;
	nproval = NUM_PROVAL;
	nsenval = NUM_SENVAL;

	/*             INITIALIZE    		*/

	for( i = 1; i <= 6; i++ )
		{
		i_ = i - 1;
		Numval[i] = 0;
		}
	/*             Find the Beginning at *1 		*/

    i = 0;
	notfound = 1;
	while( notfound )
	{
        while ( (fgets(buff_temp,sizeof(buff_temp),fp_.kchn)) != NULL )
        {
           if ( strpat(buff_temp,"*1",'$','%') >= 0 )
           {
              notfound = 0;
              break;
           }
          if ( i > npeval )  goto L_9000;
          i++;
        }
	}
        if ( notfound ) goto L_9000;


	/*             Read in Pe Codes *1 		*/

	notfound = 1;
	i = 1;
	while( notfound )
	{
		if( i > npeval )  goto L_9010;
        while ( (fgets(buff_temp,sizeof(buff_temp),fp_.kchn)) != NULL )
        {
           if ( strpat(buff_temp,"*2",'$','%') >= 0 )
           {
              notfound = 0;
              break;
           }
           str2 = buff_temp;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9010;
           strncpy(parms1_.peval[i-1],str1,2);
           str2 = 0;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9010;
           parms3_.pevalue[i-1] = atof(str1);
           memset(buff_temp,0,sizeof(buff_temp));
		i = i + 1;

        }

	}

	Numval[1] = i - 1;

	/*             Duration codes 		*/

	notfound = 1;
	i = 1;
	while( notfound )
	{
		if( i > ndurval ) goto L_9020;
        while ( (fgets(buff_temp,sizeof(buff_temp),fp_.kchn)) != NULL )
        {
           if ( strpat(buff_temp,"*3",'$','%') >= 0 )
           {
              notfound = 0;
              break;
           }
           str2 = buff_temp;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9020;
           strncpy(parms1_.durval[i-1],str1,1);
           str2 = 0;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9020;
           parms2_.durvalue[i-1] =  atof(str1);
           memset(buff_temp,0,sizeof(buff_temp));
		i = i + 1;

        }

	}
	Numval[2] = i - 1;

	/*             Type Source Codes 		*/

	notfound = 1;
	i = 1;

	while( notfound )
		{
		if( i > ntsval ) goto L_9030;

        while ( (fgets(buff_temp,sizeof(buff_temp),fp_.kchn)) != NULL )
        {
           if ( strpat(buff_temp,"*4",'$','%') >= 0 )
           {
              notfound = 0;
              break;
           }
           str2 = buff_temp;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9030;
           strncpy(parms1_.tsval[i-1],str1,2);
           str2 = 0;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9030;
           parms2_.tsvalue[i-1] = atoi(str1);
           memset(buff_temp,0,sizeof(buff_temp));
		i = i + 1;

        }

	}

	Numval[3] = i - 1;

	/*             Extremnum Codes 			*/
	notfound = 1;
	i = 1;

	while( notfound )
		{
		if( i > nexval ) goto L_9040;
        while ( (fgets(buff_temp,sizeof(buff_temp),fp_.kchn)) != NULL )
        {
           if ( strpat(buff_temp,"*5",'$','%') >= 0 )
           {
              notfound = 0;
              break;
           }
           str2 = buff_temp;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9040;
           strncpy(parms1_.exval[i-1],str1,1);
           str2 = 0;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9040;
           parms2_.exvalue[i-1] = atoi(str1);
           memset(buff_temp,0,sizeof(buff_temp));
		i = i + 1;

        }
	}

	Numval[4] = i - 1;

	/*             Probability Codes 		*/

	notfound = 1;
	i = 1;
	while( notfound )
		{
		if( i > nproval ) goto L_9050;

        while ( (fgets(buff_temp,sizeof(buff_temp),fp_.kchn)) != NULL )
        {
           if ( strpat(buff_temp,"*6",'$','%') >= 0 )
           {
              notfound = 0;
              break;
           }
           str2 = buff_temp;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9050;
           strncpy(parms1_.proval[i-1],str1,1);
           str2 = 0;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9050;
           parms3_.provalue[i-1] = (double) atof(str1);
           memset(buff_temp,0,sizeof(buff_temp));
		i = i + 1;

        }

	}

	Numval[5] = i - 1;

	/*             Send Codes 			*/


	notfound = 1;
	i = 1;

	while( notfound )
	{
        memset (temp_buff,0, sizeof(temp_buff));
		if( i > nsenval ) goto L_9060;

        while ( (fgets(buff_temp,sizeof(buff_temp),fp_.kchn)) != NULL )
        {
           if ( strpat(buff_temp,"**",'$','%') >= 0 )
           {
              notfound = 0;
              break;
           }
           str2 = buff_temp;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9060;
           strcpy(parms1_.senval[i-1],str1);
           str2 = 0;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9060;
           strcpy(parms2_.sendcodes[i-1],str1);
             str2 = 0;
           if ( (str1 = strtok(str2," \t\n\r")) == NULL )
                 goto L_9060;
           parms2_.sendflag[i-1] = atoi(str1);
           memset(buff_temp,0,sizeof(buff_temp));
           for ( ii = 0; ii < 8; ii++)
           {
              if ( parms2_.sendcodes[i-1][ii] == 0 )
                   parms2_.sendcodes[i-1][ii] = 32;
           }

		i = i + 1;

        }
	}

	Numval[6] = i - 1;

	return;

	/*             Errors 		*/

L_9000:
    fprintf( stdout, "\ngetparm:error - could not find *1 - start of PE codes\n");
	fclose(fp_.kchn);
	fp_.kchn = 0;                                       /* dgb:02/09/00 */
	exit(0);


L_9010:
    fprintf(stdout,"\ngetparm:error - could not find *2 - start of duration codes\n");
	fclose(fp_.kchn);
	exit(0);

L_9020:
    fprintf(stdout,"\ngetparm:error - could not find *3 - start of TS codes\n");
	fclose(fp_.kchn);
	exit(0);

L_9030:
    fprintf(stdout,"\ngetparm:error - could not find *4 - start of extremun codes\n");
	fclose(fp_.kchn);
	exit(0);

L_9040:
    fprintf(stdout,"\ngetparm:error - could not find *5 - start of probability codes\n");
	fclose(fp_.kchn);
	exit(0);

L_9050:
    fprintf(stdout,"\ngetparm:error - could not find *6 - start of send codes\n");
	fclose(fp_.kchn);
	exit(0);

L_9060:
    fprintf(stdout,"\ngetparm:error - could not find ** - end of file\n");
	fclose(fp_.kchn);
	exit(0);

}

