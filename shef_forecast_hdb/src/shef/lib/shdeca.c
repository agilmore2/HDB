/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 16:59:43 */
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
  
       SUBROUTINE SHDECA(IREV,STATUS)
  
       PURPOSE                                                    
         Decode SHEF .A format
         IREV = 1 FOR REVISED, = 0 FOR NEW DATA
  
       VERSION and UPDATES
         1.0    APR 82   Geoffrey M. Bonnin
                Original Version
         1.1    JAN 84   Geoff Bonnin
                Improve testing for bad dates on date relative adjustment
         1.2    AUG 84   Geoff Bonnin
                Change creation date test to non-fatal and seconds to times
         1.3    MAY 85   Geoff Bonnin
                Correct error in adjusting to prev 7am
         1.4    MAR 86
                Fix error in subsequet revision continuation
         1.5    JUN 89   David G. Brandon
                Adapted to QNX Hydromet
         2.0    JUN 94   David G. Brandon
                Include NSFLAG for selected send codes
                Also Translated to 'C' using FOR_C
                Removed alternate returns in subroutines
         2.1    APR 95   David G. Brandon
                Changed shefpass to pass all parameters in argument
                list...prepare for INFORMIX posting.
         2.2    Add iscore to t_codes array.
         2.3    NOV 95   DGB
                Add check to place nyear and k1 in correct century.
                Allow for all valid qualifier codes...excepting
                'O' and 'I'.
         2.4    DEC 95 DGB
                Do not process DC when they are missing, or the month
                k2 is set to 0.
         2.5    JAN 96 DGB
                Update struct t_data to include lcent
                and kcent for the century.
         2.6    SEP 10 97 DGB
                Add the use of MAX_SHEF_INPUT so that the input line can
                be longer than 80 characters.
                Add capability to output a database comment.
         2.7    DEC 1 97 DGB
                Add advanced test option will will print a shefout
                record to the error file.
         2.8    FEB 4 98 DGB
                Fixed -10000 null problem...so -10000 would not print.
         2.9    JAN 18 99 DGB
                Check/compute correct century/year if override
                was specified.
 *--------------------------------------------------------------------- */


void shdeca(irev, status)
short int *irev, *status;
{
    int DEBUG1;
    int itest, overflag;                                   /* dgb:01/16/99 */

	static char iname[9];
	static short int iadj, idote, idur, iend_, ival, k1, 
	             k2, k3, k4, k5, k6, lwal, nday, ndig, nflag, 
                 nhour, nmin, nmon, nsec, num, nyear, i, flag,
                 ncent;

	static double factor;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Idate = &datim_.idate[0] - 1;             /* dgb:01/96 */
	short *const Ibuf = &buffer_.ibuf[0] - 1;
	short *const Idstn = &data_.idstn[0] - 1;
	short *const Msource = &data_.msource[0] - 1;

  
        DEBUG1 = 0;
        if ( DEBUG1 ) printf("\nshdeca:begin");

        data_.overmonth   = -1;                                /* dgb:01/18/99 */
        data_.overyear    = -1;                                /* dgb:01/18/99 */
        data_.overcentury = -1;                                /* dgb:01/18/99 */

	*status = 0;
	data_.itz = 0; 
	idote = 0;
	Msource[1] = codes_.iblnk;
	Msource[2] = codes_.iblnk;
	Msource[3] = codes_.iblnk;
	Msource[4] = codes_.iblnk;
	Msource[5] = codes_.iblnk;
	Msource[6] = codes_.iblnk;
	Msource[7] = codes_.iblnk;
	Msource[8] = codes_.iblnk;

	/*             Check for Continuation */

	if( format_.iform > 0 )
		goto L_10;
	format_.iform = -format_.iform;
	if( (format_.lform != 1) && (format_.lform != 2) )
		goto L_9020;
	if( (format_.iform == 2) && (format_.lform == 1) )
		goto L_9030;
	if( (format_.iform == 1) && (format_.lform == 2) )
		*irev = 1;
	if( (format_.iform == 1) && (format_.lform == 2) )
		format_.iform = 2;
	if( format_.nerr > 0 )
		goto L_9040;
	goto L_30;

	/*             Defaults */

L_10:
	data_.kyear   = 0;
	data_.kmon    = 0;
	data_.kday    = 0;
	data_.khour   = 0;
	data_.kmin    = 0;
	data_.ksec    = 0;
	data_.kwal    = codes_.ichz;
	data_.kodu    = 1;
        datrel_.mcent = 0;                                     /* dgb cent */
	datrel_.myear = 0;
	datrel_.mmon  = 0;
	datrel_.mday  = 0;
	datrel_.mhour = 0;
	datrel_.mmin  = 0;
	datrel_.msec  = 0;
	datrel_.mend  = 0;
	data_.idcodd  = 5000;
	format_.nerr  = 0;

	/*             Get the Positional Data */

	shpos( status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9050;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}

	/*             Get the date and data type elements */

L_30:
        flag = 1;
	shdtype( &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9000;
		}

	/*             Get the Parameter Code */

	shpcode( &data_.kodp, &data_.kode, &data_.icodd, &data_.kodt, 
	         &data_.kods, &data_.kodex, &data_.codp, &factor, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9000;
		}

	if( xchar_.ichar != codes_.iblnk )
		goto L_9120;

	/*             Test for Correct Duration */

	if( (data_.icodd == 5003) && (data_.idcodd == 5000) )
		 goto L_9080;
	idur = data_.icodd;
	if( data_.icodd == 5003 )
		idur = data_.idcodd;

	/*             Test if Creation Date Specified for Forecast Data */

	if( (data_.kodt == codes_.ichf) && (data_.kmon == 0) )
        {
            flag = 23;
            sherr( &flag );
        }

	/*             Get the Value */

	if( buffer_.ip > MAX_SHEF_INPUT )                      /* dgb:09/10/97 */
		goto L_9000;

        flag = 0;
	shreal( &data_.value, &flag, &ndig, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_43;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	iend_ = 0;
	goto L_45;

L_43:
	iend_ = 1;
L_45:
	if( ndig <= 0 )
		goto L_9100;

	/*             Adjust for Trace */

	if( data_.value >= -8e10 )
		goto L_50;
	if( data_.kodp != codes_.ichp )
		goto L_47;
	if( data_.kode == codes_.ichc )
		goto L_48;
	if( data_.kode == codes_.ichp )
		goto L_48;
	goto L_9090;

L_47:
	if( data_.kodp != codes_.ichs )
		goto L_9090;
	if( data_.kode == codes_.ichd )
		goto L_48;
	if( data_.kode == codes_.ichf )
		goto L_48;
	if( data_.kode == codes_.ichw )
		goto L_48;
	goto L_9090;

L_48:
	data_.value = 0.001e0;
	goto L_60;

	/*             Convert SI to English Units if Necessary */

L_50:
	if( data_.kodu == 1 )
		goto L_60;
	ival = data_.value - 0.01;
	if( ival == -9999 )
		goto L_60;
	if( ival == -9002 )
		goto L_60;
	if( factor < 0e0 )
		data_.value = data_.value*1.8e0 + 32.0e0;
	if( factor > 0e0 )
		data_.value = data_.value*factor;

	/*             Test the Qualifier - It Should be in ICHAR Already */

L_60:
	if( iend_ == 1 )
		goto L_65;
	/* pass all alphabetical characters except 'O' and 'I'   dgb:11/13/95 */
	irang(&xchar_.ichar,&codes_.icha,&codes_.ichz,status);
	if ( !*status )
	{
	   if ( xchar_.ichar == codes_.icho )  goto L_9120;
	   if ( xchar_.ichar == codes_.ichi )  goto L_9120;
	   *status = 0;
	   goto L_70;
	}
	if( xchar_.ichar == codes_.iblnk )
		goto L_65;
	if( xchar_.ichar == codes_.islash )
		goto L_65;
	goto L_9120;

L_65:
	lwal = data_.kwal;
	nflag = 0;
	goto L_80;

L_70:
	lwal = xchar_.ichar;
	nflag = 1;

	/*             Adjust the Observation time to 7AM Previous Day.
	 *             Can't do it if ZULU time or Date REL has Been Specified */

L_80:

        ncent = data_.lcent;  
	nyear = data_.lyear;
	nmon  = data_.lmon;
	nday  = data_.lday;
	nhour = data_.lhour;
	nmin  = data_.lmin;
	nsec  = data_.lsec;

	if( sendflg_.nsflag == 0 )
		goto L_83;
	if( data_.nadjtz == 0 )
 		goto L_9110;
        if( datrel_.mcent != 0 )                               /* dgb:01/96 */
                goto L_9110;  
	if( datrel_.myear != 0 )
		goto L_9110;
	if( datrel_.mmon != 0 )
		goto L_9110;
	if( datrel_.mday != 0 )
		goto L_9110;
	if( datrel_.mhour != 0 )
		goto L_9110;
	if( datrel_.mmin != 0 )
		goto L_9110;
	if( datrel_.msec != 0 )
		goto L_9110;
	sendflg_.nsflag = 0;
	iadj = -1;
	if( nhour < 7 )
        {
                flag = 3;
		shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &iadj, 
		 &flag, status );
        }
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	nhour = 7;
	nmin = 0;
	nsec = 0;
	goto L_85;

	/*             Adjust for Date Relative for Year, Month and Day */

L_83:
        flag = 3;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &datrel_.mday, 
	 &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
        flag = 4;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &datrel_.mmon, 
	 &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
        flag = 5;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &datrel_.myear, 
	 &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
        flag = 6;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &datrel_.mend, 
	 &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	shtdat( &nyear, &nmon, &nday, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9070;
		}

	/*             Test for Local Time Zone and Adjust to ZULU Time */

L_85:
	iadj = 0;
	shlocl( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &data_.nadjtz, 
	 &iadj, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
        flag = 1;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &iadj, &flag, 
	 status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	k1 = data_.kyear;
	k2 = data_.kmon;
	k3 = data_.kday;
	k4 = data_.khour;
	k5 = data_.kmin;
	k6 = data_.ksec;

	if( data_.kmon == 0 )
		goto L_87;
	iadj = 0;

	shlocl( &k1, &k2, &k3, &k4, &k5, &k6, &data_.nadjtz, &iadj, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
        flag = 1;
	shtadj( &k1, &k2, &k3, &k4, &k5, &k6, &iadj, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

	/*             Adjust for Date Relative for Hour, Minute and Second */

L_87:
        flag = 7;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &datrel_.msec, 
	        &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
        flag = 1;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &datrel_.mmin, 
	        &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
        flag = 2;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &datrel_.mhour, 
	        &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

   /* Test for correct year if a month override was specified 
      && explicit year was not specified */
      if ( data_.overyear > - 1 && data_.overcentury == -1 ) /* dgb:01/18/99 */
         if ( data_.overyear > 30 ) nyear = data_.overyear + 1900; /* dgb:01/18/99 */
      else
      if ( data_.overyear > -1 && data_.overcentury > -1 )
           nyear = (data_.overcentury * 100) + data_.overyear;

      if ( data_.overmonth > -1 && data_.overyear == -1 )   /* dgb:01/18/99 */
      {
        itest = nmon - Idate[1];                            
        if ( itest > 6 ) nyear--;                           
        if ( itest < -6 ) nyear++;                          
        if ( (itest == -6 ) & ( nday < Idate[2] ) )         
              nyear++;                                          
        if ( (itest == 6 ) && ( nday > Idate[2] ) )         
              nyear--;                                          
      }

	/* Write the shef_out File */
       if ( data_.value != -10000 )                           /* dgb:02/04/98 */
       {   
          if( cont_.out_flag[0] == '+' )
	  {
             fprintf(fp_.jchn,
             "%c%c%c%c%c%c%c%c %4d %2d %2d %2d %2d %2d %4d %2d %2d %2d %2d %2d ",
	      data_.idstn[0], data_.idstn[1], data_.idstn[2], data_.idstn[3],
              data_.idstn[4], data_.idstn[5], data_.idstn[6], data_.idstn[7],
              nyear, nmon, nday, nhour, nmin, nsec, k1, k2, k3, k4, k5, k6);

              fprintf(fp_.jchn,
              "%c%c%c%c%c%c%c %4d %20.6f %20.6f %c %d %c%c%c%c%c%c%c%c %d %s\n",
              data_.kodp, data_.kode, durprob_.kodd, data_.kodt, data_.kods, 
              data_.kodex, durprob_.kodpr, idur, data_.codp, data_.value, lwal, *irev, 
              data_.msource[0], data_.msource[1], data_.msource[2], data_.msource[3],
              data_.msource[4], data_.msource[5], data_.msource[6], data_.msource[7],
              idote,comment_.comment );                        /* dgb:09/10/97 */


          if ( test_.atest_flag )                          /* dgb:12/01/97 */
          {
              fprintf(fp_.icher,
              "%c%c%c%c%c%c%c%c %4d %2d %2d %2d %2d %2d %4d %2d %2d %2d %2d %2d ",
              data_.idstn[0], data_.idstn[1], data_.idstn[2], data_.idstn[3],
              data_.idstn[4], data_.idstn[5], data_.idstn[6], data_.idstn[7],
              nyear, nmon, nday, nhour, nmin, nsec, k1, k2, k3, k4, k5, k6);
  
              fprintf(fp_.icher,
              "%c%c%c%c%c%c%c %4d %20.6f %20.6f %c %d %c%c%c%c%c%c%c%c %d %s\n",
              data_.kodp, data_.kode, durprob_.kodd, data_.kodt, data_.kods, 
              data_.kodex, durprob_.kodpr, idur, data_.codp, data_.value, lwal, *irev, 
              data_.msource[0], data_.msource[1], data_.msource[2], data_.msource[3],
              data_.msource[4], data_.msource[5], data_.msource[6], data_.msource[7],
              idote,comment_.comment );                        /* dgb:09/10/97 */
          }


	}


	/*             Pass Data to shefpass Routine */

	if( cont_.post_flag[0] == '+' )
		{

		shefpass( data_.idstn[0],
                  data_.idstn[1],
                  data_.idstn[2],
                  data_.idstn[3],
                  data_.idstn[4],
                  data_.idstn[5],
                  data_.idstn[6],
                  data_.idstn[7],
                  nyear, 
                  nmon, 
                  nday, 
                  nhour, 
                  nmin, 
                  nsec, 
                  k1,
                  k2,
                  k3,
                  k4,
                  k5,
                  k6,
                  data_.kodp,
                  data_.kode,
                  durprob_.kodd,
                  data_.kodt,
                  data_.kods,
                  data_.kodex,
                  durprob_.kodpr,
                  idur,
                  data_.codp,
                  data_.value,
                  lwal, 
                  *irev,
                  data_.msource[0],
                  data_.msource[1],
                  data_.msource[2],
                  data_.msource[3],
                  data_.msource[4],
                  data_.msource[5],
                  data_.msource[6],
                  data_.msource[7],
                  idote
                );
           }
       } 
                                                            /* dgb:02/04 98 */
    memset(comment_.comment,0,sizeof(comment_.comment));    /* dgb:09/10/97 */
    data_.overmonth = -1;                                   /* dgb:01/18/99 */
    data_.overyear  = -1;                                   /* dgb:01/18/99 */
    data_.overcentury = -1;                                 /* dgb:01/18/99 */
    
	if( iend_ == 1 )
		goto L_9000;

	/*             Clear Past the Next Slash */

	if( nflag == 0 )
		goto L_120;
L_110:
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
L_120:
	if( xchar_.ichar == codes_.iblnk )
		goto L_110;

	if( xchar_.ichar != codes_.islash )
		goto L_9120;

	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	goto L_30;

	/*             Error Return - Stop Looking at this Format */

L_9000:
	*status = 1;
	return;

	/*             Error Return - Stop Looking at this File */

L_9010:
	*status = 2;
	return;

L_9020:
	num = 19;
	goto L_9200;

L_9030:
	num = 20;
	goto L_9200;

L_9040:
	num = 21;
	goto L_9200;

L_9050:
	format_.nerr = 1;
	goto L_9000;

L_9070:
	num = 24;
	format_.nerr = 1;
	goto L_9200;

L_9080:
	num = 25;
	format_.nerr = 1;
	goto L_9200;

L_9090:
	num = 28;
	format_.nerr = 1;
	goto L_9200;

L_9100:
	num = 33;
	format_.nerr = 1;
	goto L_9200;

L_9110:
	num = 35;
	sendflg_.nsflag = 0;
	format_.nerr = 1;
	goto L_9200;

L_9120:
	num = 31;
	format_.nerr = 1;

L_9200:
	sherr( &num );
	goto L_9000;

}

