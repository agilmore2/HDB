/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 16:59:15 */
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
          SUBROUTINE SHBFW(MSOURCE,LYEAR,LMON,LDAY,LHOUR,LMIN,LSEC,           
         >                 KYEAR,KMON,KDAY,KHOUR,KMIN,KSEC,            
         >                 MYEAR,MMON,MDAY,MHOUR,MMIN,MSEC,MEND,       
         >                 KODP,KODE,ICODD,IDCODD,KODT,KODS,KODEX,CODP,
         >                 KWAL,NADJTZ,KODU,FACTOR,IREV,KODD,KODPR)    
  
       PURPOSE                                                    
         Save the .B header
  
       VERSION and UPDATES
         1.0    NOV 84  Geoffrey M. Bonnin
                Original Version
         1.1    JUN 89   David G. Brandon
                Adapted to QNX NWS Hydromet
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
         2.1    JAN 96   DGB
                Remove FORC I/O and replace with C I/O
         2.2    Add lcent and kcent for century.
  --------------------------------------------------------------------- */


/* dgb cent add lcent, kcent, mcent */
void shbfw(msource, lcent, lyear, lmon, lday, lhour, lmin, lsec, 
	 kcent, kyear, kmon, kday, khour, kmin, ksec, mcent, myear, mmon, mday, mhour, 
	 mmin, msec, mend, kodp, kode, icodd, idcodd, kodt, kods, kodex, 
	 codp, kwal, nadjtz, kodu, factor, irev, kodd, kodpr)

short int msource[], *lcent, *lyear, *lmon, *lday, *lhour, *lmin, *lsec, 
     *kcent, *kyear, *mcent, 
	 *kmon, *kday, *khour, *kmin, *ksec, *myear, *mmon, *mday, *mhour, 
	 *mmin, *msec, *mend, *kodp, *kode, *icodd, *idcodd, *kodt, *kods, 
	 *kodex;
float *codp;
short int *kwal, *nadjtz, *kodu;
double *factor;
short int *irev, *kodd, *kodpr;
{
	static short int i, i_, mbuf;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	double *const Aar = &bbuf2_.aar[0] - 1;
	float *const Bar = &bbuf2_.bar[0] - 1;
	short *const Msource = &msource[0] - 1;

	/*             Set max virtual buffer size - must correspond with 
	 *             array dimensions in BBUF2 &3 */
	mbuf = 38;

	/*             Increment record counter */

	bbuf1_.nmrec = bbuf1_.nmrec + 1;

	/*             Is this a virtual or a disk record */

	if( bbuf1_.nmrec > mbuf )
		goto L_20;
	/*             Place record in virtual buffer */

	Aar[bbuf1_.nmrec] = *factor;
	Bar[bbuf1_.nmrec] = *codp;
	bbuf2_.iarray[bbuf1_.nmrec - 1][0] = *lyear;
	bbuf2_.iarray[bbuf1_.nmrec - 1][1] = *lmon;
	bbuf2_.iarray[bbuf1_.nmrec - 1][2] = *lday;
	bbuf2_.iarray[bbuf1_.nmrec - 1][3] = *lhour;
	bbuf2_.iarray[bbuf1_.nmrec - 1][4] = *lmin;
	bbuf2_.iarray[bbuf1_.nmrec - 1][5] = *lsec;
	bbuf2_.iarray[bbuf1_.nmrec - 1][6] = *kyear;
	bbuf2_.iarray[bbuf1_.nmrec - 1][7] = *kmon;
	bbuf2_.iarray[bbuf1_.nmrec - 1][8] = *kday;
	bbuf2_.iarray[bbuf1_.nmrec - 1][9] = *khour;
	bbuf2_.iarray[bbuf1_.nmrec - 1][10] = *kmin;
	bbuf2_.iarray[bbuf1_.nmrec - 1][11] = *ksec;
	bbuf2_.iarray[bbuf1_.nmrec - 1][12] = *myear;
	bbuf2_.iarray[bbuf1_.nmrec - 1][13] = *mmon;
	bbuf2_.iarray[bbuf1_.nmrec - 1][14] = *mday;
	bbuf2_.iarray[bbuf1_.nmrec - 1][15] = *mhour;
	bbuf2_.iarray[bbuf1_.nmrec - 1][16] = *mmin;
	bbuf2_.iarray[bbuf1_.nmrec - 1][17] = *msec;
	bbuf2_.iarray[bbuf1_.nmrec - 1][18] = *mend;
	bbuf2_.iarray[bbuf1_.nmrec - 1][19] = *icodd;
	bbuf2_.iarray[bbuf1_.nmrec - 1][20] = *idcodd;
	bbuf2_.iarray[bbuf1_.nmrec - 1][21] = *nadjtz;
	bbuf2_.iarray[bbuf1_.nmrec - 1][22] = *kodu;
	bbuf2_.iarray[bbuf1_.nmrec - 1][23] = *irev;
    bbuf2_.iarray[bbuf1_.nmrec - 1][24] = *lcent;
    bbuf2_.iarray[bbuf1_.nmrec - 1][25] = *kcent;
    bbuf2_.iarray[bbuf1_.nmrec - 1][26] = *mcent;

	for( i = 1; i <= 8; i++ )
		{
		i_ = i - 1;
		bbuf3_.jarray[bbuf1_.nmrec - 1][i_] = Msource[i];
		}

	bbuf3_.jarray[bbuf1_.nmrec - 1][8] = *kodp;
	bbuf3_.jarray[bbuf1_.nmrec - 1][9] = *kode;
	bbuf3_.jarray[bbuf1_.nmrec - 1][10] = *kodt;
	bbuf3_.jarray[bbuf1_.nmrec - 1][11] = *kods;
	bbuf3_.jarray[bbuf1_.nmrec - 1][12] = *kodex;
	bbuf3_.jarray[bbuf1_.nmrec - 1][13] = *kwal;
	bbuf3_.jarray[bbuf1_.nmrec - 1][14] = *kodd;
	bbuf3_.jarray[bbuf1_.nmrec - 1][15] = *kodpr;
	bbuf3_.jarray[bbuf1_.nmrec - 1][16] = sendflg_.nsflag;

	goto L_900;

	/*             Place record in disk buffer file */

L_20:

    /* 
      virtual buffer increased to handle 38 parameters in .B line
      disk buffer no longer used...program will error and exit
      if more than 38 values are detected
    */

    fprintf(fp_.icher,"\nError:more than 38 parameters detected in .B header line");
    error_.nerror++;
    shefcl();
  
    /*
	if( bbuf1_.nmrec == (mbuf + 1) )
		{
        rewind(fp_.mchn);
		}

	fprintf( fp_.mchn, "%& %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %f %hi %hi %hi %lf %hi %hi %hi ", 
	 (size_t)8*sizeof(short),msource, *lcent, *lyear, *lmon, *lday, *lhour, 
	 *lmin, *lsec, *kcent, *kyear, *kmon, *kday, *khour, *kmin, *ksec, *mcent, *myear, 
	 *mmon, *mday, *mhour, *mmin, *msec, *mend, *kodp, *kode, *icodd, 
	 *idcodd, *kodt, *kods, *kodex, *codp, *kwal, *nadjtz, *kodu, 
	 *factor, *irev, *kodd, *kodpr );
     */

L_900:
	return;

}

