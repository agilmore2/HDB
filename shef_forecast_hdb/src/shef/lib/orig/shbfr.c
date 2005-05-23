/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 16:59:02 */
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
         SUBROUTINE SHBFR(MSOURCE,LYEAR,LMON,LDAY,LHOUR,LMIN,LSEC,
        >                 KYEAR,KMON,KDAY,KHOUR,KMIN,KSEC,
        >                 MYEAR,MMON,MDAY,MHOUR,MMIN,MSEC,MEND,
        >                 KODP,KODE,ICODD,IDCODD,KODT,KODS,KODEX,CODP,
        >                 KWAL,NADJTZ,KODU,FACTOR,IREV,KODD,KODPR)
  
       PURPOSE                                                    
         Recall the .B header record
  
       VERSION and UPDATES
         1.0    NOV 84   Geoffrey M. Bonnin
                Original Version
         1.1    JUN 89   David G. Brandon
                Adapted to QNX NWS Hydromet
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
         2.1    JAN 96   DGB
                Remove FORC I/O and replace with C I/O
  --------------------------------------------------------------------- */


/* dgb add lcent and kcent mcent */
void shbfr(msource,lcent,lyear, lmon, lday, lhour, lmin, lsec, 
	 kcent,kyear, kmon, kday, khour, kmin, ksec, mcent, myear, mmon, mday, mhour, 
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
		/* end of OFFSET VECTORS */


	/*             Set max virtual buffer size - must correspond with array 
	 *             dimension in BBUF2 & 3                                                                                                                                                                                                                            */
	mbuf = 38;

	/*             Increment record counter */

	bbuf1_.nmrec = bbuf1_.nmrec + 1;

	/*             is this a virtual or a disk record */

	if( bbuf1_.nmrec > mbuf )
		goto L_20;

	/*             Retrieve record from virtual buffer */

	*factor = Aar[bbuf1_.nmrec];
	*codp = Bar[bbuf1_.nmrec];
	*lyear = bbuf2_.iarray[bbuf1_.nmrec - 1][0];
	*lmon = bbuf2_.iarray[bbuf1_.nmrec - 1][1];
	*lday = bbuf2_.iarray[bbuf1_.nmrec - 1][2];
	*lhour = bbuf2_.iarray[bbuf1_.nmrec - 1][3];
	*lmin = bbuf2_.iarray[bbuf1_.nmrec - 1][4];
	*lsec = bbuf2_.iarray[bbuf1_.nmrec - 1][5];
	*kyear = bbuf2_.iarray[bbuf1_.nmrec - 1][6];
	*kmon = bbuf2_.iarray[bbuf1_.nmrec - 1][7];
	*kday = bbuf2_.iarray[bbuf1_.nmrec - 1][8];
	*khour = bbuf2_.iarray[bbuf1_.nmrec - 1][9];
	*kmin = bbuf2_.iarray[bbuf1_.nmrec - 1][10];
	*ksec = bbuf2_.iarray[bbuf1_.nmrec - 1][11];
	*myear = bbuf2_.iarray[bbuf1_.nmrec - 1][12];
	*mmon = bbuf2_.iarray[bbuf1_.nmrec - 1][13];
	*mday = bbuf2_.iarray[bbuf1_.nmrec - 1][14];
	*mhour = bbuf2_.iarray[bbuf1_.nmrec - 1][15];
	*mmin = bbuf2_.iarray[bbuf1_.nmrec - 1][16];
	*msec = bbuf2_.iarray[bbuf1_.nmrec - 1][17];
	*mend = bbuf2_.iarray[bbuf1_.nmrec - 1][18];
	*icodd = bbuf2_.iarray[bbuf1_.nmrec - 1][19];
	*idcodd = bbuf2_.iarray[bbuf1_.nmrec - 1][20];
	*nadjtz = bbuf2_.iarray[bbuf1_.nmrec - 1][21];
	*kodu = bbuf2_.iarray[bbuf1_.nmrec - 1][22];
	*irev = bbuf2_.iarray[bbuf1_.nmrec - 1][23];
    *lcent = bbuf2_.iarray[bbuf1_.nmrec - 1][24];
    *kcent = bbuf2_.iarray[bbuf1_.nmrec - 1][25];
    *mcent = bbuf2_.iarray[bbuf1_.nmrec - 1][26];

	for( i = 1; i <= 8; i++ )
		{
		i_ = i - 1;
		Msource[i] = bbuf3_.jarray[bbuf1_.nmrec - 1][i_];
		}

	*kodp = bbuf3_.jarray[bbuf1_.nmrec - 1][8];
	*kode = bbuf3_.jarray[bbuf1_.nmrec - 1][9];
	*kodt = bbuf3_.jarray[bbuf1_.nmrec - 1][10];
	*kods = bbuf3_.jarray[bbuf1_.nmrec - 1][11];
	*kodex = bbuf3_.jarray[bbuf1_.nmrec - 1][12];
	*kwal = bbuf3_.jarray[bbuf1_.nmrec - 1][13];
	*kodd = bbuf3_.jarray[bbuf1_.nmrec - 1][14];
	*kodpr = bbuf3_.jarray[bbuf1_.nmrec - 1][15];
	sendflg_.nsflag = bbuf3_.jarray[bbuf1_.nmrec - 1][16];

	goto L_900;

	/*             Retrieve record from disk buffer file */

L_20: /* 
        virtural buffer changed to handle 24 items in .B line 
        the dotbtemp file no longer used and commented out
     */

    /*
	if( bbuf1_.nmrec == (mbuf + 1) )
		{
        rewind(fp_.mchn);
		}

    
    */
    /* add kcent and lcent */
	fscanf( fp_.mchn, "%& %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %hi %f %hi %hi %hi %lf %hi %hi %hi ", 
	 (size_t)8*sizeof(short),msource, lcent, lyear, lmon, lday, lhour, lmin, 
	 lsec, kcent, kyear, kmon, kday, khour, kmin, ksec, mcent, myear, mmon, mday, 
	 mhour, mmin, msec, mend, kodp, kode, icodd, idcodd, kodt, kods, 
	 kodex, codp, kwal, nadjtz, kodu, factor, irev, kodd, kodpr );


L_900:
{
	return;
}
}

