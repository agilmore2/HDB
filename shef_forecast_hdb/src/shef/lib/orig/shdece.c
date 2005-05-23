/*Translated by FOR_C, v3.4.2 (-), on 04/06/95 at 20:26:05 */
/*FOR_C Options SET: c=2 com=u do=r4 ftn=2ln6kk op=iv s=dvn str=l sq=i */

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
         SUBROUTINE SHDECE(IREV,STATUS)
  
       PURPOSE                                                    
         Decode .E Format                        
         IREV = 1 FOR REVISED, = 0 FOR NEW DATA
  
       VERSION and UPDATES
         1.0    UGR 82   Geoffrey M. Bonnin
                Original Version
         1.1    JAN 84 
                Improve testing for bad dates on date 
                Relative Adjustment
         1.2    AUG 84
                Change test for creation data to non-fatal
                Allow negative DI increments
                Add seconds to times
         1.3    MAR 86
                Fix error in subsequent revision continuation
         2.0    JUN 94   David G. Brandon
                Include NSFLAG for selected send codes
                Also Translated to 'C' using FOR_C
                Removed alternate returns in subroutines
         2.1    Add iscore to t_codes array.
         2.3    NOV 95   DGB
                Add check to place n1 and k1 in correct century.
                Allow for all valid qualifier codes excepting
                for 'O' and 'I'.
         2.4    DEC 95 DGB
                Do not process DC when they are missing, or the month
                k2 is set to 0
         2.5    JAN 96 DGB
                Update struct t_data to include lcent 
                and kcent for century.
                Modify code to print out correct century in shefout
                and shefpass.
         2.6    SEP 10 97 DGB
                Add the use of MAX_SHEF_INPUT so that the input line can
                be longer than 80 characters.
                Add capability to output a database comment.
         2.7    OCT 30 97 DGB
                Fix time problem for .E messages like the one
                that follows:

                .E XXXXX 971020 DH12/HG/DI6
                .E1 / 12.2
                Messages that had a leading '/' were setting the
                initial time incorrectly. 
         2.8    DEC 1 97 DGB
                Include advanced test flag option.  This will print
                a shefout record to the error file.
         2.5    JAN 25 98 DGB
                Fix increments of NULL values in .E type messages.  
                These messages are flagged with *flag = 3.  
                Previously, the NULL would be read, but the time 
                would not be incremented.  Also, leading slashes 
                on .E continuation lines caused some problems.
                Add capabilities for preprocessing .E lines.  
                Preprocessing consists of detecting NULL fields, 
                i.e.,  a slash followed by another slash or blanks
                then a slash.  {  e.g.  12.2 /  /  4 }.  Also,
                leading slash marks on continuation lines are 
                replaced by a blank.  See function pre_process_dote()
                at the end of the function, shdece.
         2.6    SEP 15 98 DGB
                Change pre_process_dote to handle a NULL
                at the end of a line, e.g., 
                { before} .E1  / N / N / N /
                { after}  .E1  / N / N / N / N
         2.7    OCT 5 98 DGB
                Change placement of null_it inpre_process_dote.
         2.8    JAN 18 99 DGB
                Check/compute correct century/year if override
                is specified.          
 *--------------------------------------------------------------------- */

void shdece(irev, status)
short int *irev, *status;
{

    int DEBUG1;
    int itest;
	static short int  iadj, idote, idur, iend_, intval, 
	         iunit, ival, k1, k2, k3, k4, k5, k6, lwal, 
                 n1, n2, n3, n4, n5, n6, ndig, nflag, null_, 
                 num, flag,
                 n11 ;  /* dgb cent */

	static double factor;

	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Idate   = &datim_.idate[0] - 1;           /* dgb:01/01/96 */
	short *const Ibuf    = &buffer_.ibuf[0] - 1;
	short *const Idstn   = &data_.idstn[0] - 1;
	short *const Msource = &data_.msource[0] - 1;

        data_.overmonth   = -1;                                /* dgb:01/18/99 */ 
        data_.overyear    = -1;                                /* dgb:01/18/99 */
        data_.overcentury = -1;                                /* dgb:01/18/99 */

        DEBUG1 = 0;
        if ( DEBUG1 ) printf("\nshdece:******************************begin");
	/*             Initialise */
	*status = 0;
	Msource[1] = codes_.iblnk;
	Msource[2] = codes_.iblnk;
	Msource[3] = codes_.iblnk;
	Msource[4] = codes_.iblnk;
	Msource[5] = codes_.iblnk;
	Msource[6] = codes_.iblnk;
	Msource[7] = codes_.iblnk;
	Msource[8] = codes_.iblnk;

	/*             Check for Continuation */
    if ( DEBUG1 ) printf("\nshdece:format_.iform = format_.iform %d",format_.iform);
	if( format_.iform > 0 )
		goto L_10;
	format_.iform = -format_.iform;
	if( (format_.lform != 5) && (format_.lform != 6) )
		goto L_9020;
	if( (format_.iform == 6) && (format_.lform == 5) )
		goto L_9030;
	if( (format_.iform == 5) && (format_.lform == 6) )
		*irev = 1;
	if( (format_.iform == 5) && (format_.lform == 6) )
		format_.iform = 6;
	if( format_.nerr > 0 )
		goto L_9040;
	goto L_21;

	/*             Defaults */

L_10:
    if ( DEBUG1 ) printf("\nshdece:at L_10");
    /* get default century */
        data_.lcent = 0;                       /* dgb:01/10/97 */ 
	data_.kyear = 0;
	data_.kmon = 0;
	data_.kday = 0;
	data_.khour = 0;
	data_.kmin = 0;
	data_.ksec = 0;
	data_.kwal = codes_.ichz;
	data_.kodu = 1;
        datrel_.mcent = 0;  /* dgb cent */
	datrel_.myear = 0;
	datrel_.mmon = 0;
	datrel_.mday = 0;
	datrel_.mhour = 0;
	datrel_.mmin = 0;
	datrel_.msec = 0;
	datrel_.mend = 0;
	data_.idcodd = 5000;
	data_.itz = -9999;
	data_.kflag = 0;
	format_.nerr = 0;
	dotee_.inul = 0;

	/*             Get the Positional Data */

    if ( DEBUG1 ) printf("\nshdece:getting positional data");
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
	goto L_30;

	/*             Check for a NULL */

L_21:
    if ( DEBUG1 ) printf("\nshdece:at L_21 dotee_.inul = %d",dotee_.inul);
	if( dotee_.inul == 0 )
		goto L_30;
	goto L_25;

L_24:
    if ( DEBUG1 ) printf("\nshdece:at L_24");
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_170;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
L_25:
    if ( DEBUG1 ) printf("\nshdece:at L_25");
	if( xchar_.ichar == codes_.iblnk )
		goto L_24;
	if( xchar_.ichar != codes_.islash )
		goto L_30;
	null_ = 1;
	goto L_100;

	/*             Get the data and data type elements */

L_30:
    if ( DEBUG1 ) printf("\nshdece:at L_30 dtype_.ieee = %d",dtype_.ieee);
	dtype_.ieee = 0;
    flag = 3;
	shdtype( &flag, status );

    if ( DEBUG1 ) printf("\nshdece:return status from shdtype = %d",*status);

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
		goto L_165;
		}
    if ( DEBUG1 ) printf("\nshdece:dtype_.ieee = %d data_.kflag=%d",dtype_.ieee,data_.kflag);

	if( (dtype_.ieee == 1) && (data_.kflag > 0) )
	   	data_.kflag = 1;
	if( data_.kflag != 0 )
		goto L_45;
    if ( DEBUG1 ) printf("\nshdece:dtype_.ieee = %d data_.kflag=%d",dtype_.ieee,data_.kflag);

	/*             Get the Parameter Code */
    if ( DEBUG1 ) printf("\nshdece:calling shpcode");

	shpcode( &data_.kodp, &data_.kode, &data_.icodd, &data_.kodt, 
	 &data_.kods, &data_.kodex, &data_.codp, &factor, status );
	 
    if ( DEBUG1 ) printf("\nshdece:status from shdtype = %d",*status);
	if( *status == 1 )
		{
		*status = 0;
		goto L_170;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_170;
		}

	if( buffer_.ip > MAX_SHEF_INPUT )                           /* dgb:09/10/97 */
		goto L_48;
	data_.kflag = 1;
	if( xchar_.ichar == codes_.iblnk )
		goto L_30;
	if( xchar_.ichar == codes_.islash )
		goto L_30;
	goto L_9130;

	/*             Get the Value */

L_45:
    if ( DEBUG1 ) printf("\nshdece:calling shreal iend_ = %d",iend_);
    flag = 0;
	shreal( &data_.value, &flag, &ndig, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_47;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	iend_ = 0;
	goto L_48;
L_47:
    if ( DEBUG1 ) printf("\nshdece:at L_47");
	iend_ = 1;
L_48:
    if ( DEBUG1 )
        printf("\nshdece:at L_48  data_.kflag = %d ndig = %d iend_ = %d",data_.kflag,ndig,iend_);
	null_ = 0;

	if( ndig > 0 )
		goto L_49;
	if( (ndig == 0) && (iend_ == 0) )
		goto L_9110;
	if( (ndig == 0) && (iend_ == 1) )
		goto L_9000;
	if( dotee_.inul == 1 )
		null_ = 1;
	goto L_100;

	/*             Test if Creation date specified for forecast data */

L_49:
    if ( DEBUG1 ) printf("\nshdece:at 49");
	if( (data_.kodt == codes_.ichf) && (data_.kmon == 0) )
        {
            flag = 23;
            sherr( &flag );
        }

	/*             Test for 7AM Prev day */

	if( sendflg_.nsflag == 1 )
		goto L_9120;	/*dgb: Check selected send codes */

	/*             Test for correct duration */

	if( (data_.icodd == 5003) && (data_.idcodd == 5000) )
		goto L_9080;
	idur = data_.icodd;
	if( data_.icodd == 5003 )
		idur = data_.idcodd;

	/*             Adjust for trace */

	if( data_.value >= -8e10 )
		goto L_50;
	if( data_.kodp != codes_.ichp )
		goto L_491;
	if( data_.kode == codes_.ichc )
		goto L_495;
	if( data_.kode == codes_.ichp )
		goto L_495;
	goto L_9090;

L_491:
    if ( DEBUG1 ) printf("\nshdece:at 491");
	if( data_.kodp != codes_.ichs )
		goto L_9090;
	if( data_.kode == codes_.ichd )
		goto L_495;
	if( data_.kode == codes_.ichf )
		goto L_495;
	if( data_.kode == codes_.ichw )
		goto L_495;
	goto L_9090;

L_495:
    if ( DEBUG1 ) printf("\nshdece:at 495");
	data_.value = 0.001e0;
	goto L_60;

	/*             Convert SI to englist units if necessary */

L_50:
    if ( DEBUG1 ) printf("\nshdece:at 50 data_.kflag=%d",data_.kflag);
	if( data_.kodu == 1 )
		goto L_60;
	ival = data_.value - 0.01;
	if( ival == -9999 )
		goto L_60;
	if( ival == -9002 )
		goto L_60;
	if( factor < 0e0 )
		data_.value = data_.value*1.8e0 + 32e0;
	if( factor > 0e0 )
		data_.value = data_.value*factor;

	/*             Test the qualifier - it should be in ICHAR already */

L_60:
    if ( DEBUG1 ) printf("\nshdece:at 60  iend_ = %d",iend_);
	if( iend_ == 1 )
            goto L_65;
	
	/* pass all alphabetical characters except 'O' and 'I'   dgb:11/13/95 */
	irang(&xchar_.ichar,&codes_.icha,&codes_.ichz,status);
	if ( !*status )
	{
	   if ( xchar_.ichar == codes_.icho )  goto L_9130;
	   if ( xchar_.ichar == codes_.ichi )  goto L_9130;
	   *status = 0;
	   goto L_70;
	}

	if( xchar_.ichar == codes_.iblnk )
		goto L_65;
	if( xchar_.ichar == codes_.islash )
		goto L_65;
	goto L_9130;

L_65:
    if ( DEBUG1 ) printf("\nshdece:at 65");
	lwal = data_.kwal;
	nflag = 0;
	goto L_100;

L_70:
    if ( DEBUG1 ) printf("\nshdece:at 70");
	lwal = xchar_.ichar;
	nflag = 1;

	/*             Do the date relative and time increment calculations
	 *             Get the time increment value and units */

L_100:
    if ( DEBUG1 ) printf("\nshdece:at 100 ");
	if( data_.itz == -9999 )
		goto L_9100;
	iunit = (data_.itz + 99)/1000 + 1;
	intval = data_.itz - (iunit - 1)*1000;
    if ( DEBUG1 ) printf("\nshdece: data_.kflag = %d iunit = %d intval = %d",data_.kflag,iunit,intval);

	/*             Adjust KFLAG and set up the time variables
	 *               1 = NOT YET STARTED A TIME SERIES
	 *               2 = WITHIN A TIME SERIES
	 *               3 = FIRST ELEMENT OF A TIME SERIES */

    
	if( data_.kflag == 3 )
		data_.kflag = 2;
	if( data_.kflag == 2 )
		goto L_105;
	if( data_.kflag == 1 )
		idote = 1;
	if( data_.kflag == 1 )
		data_.kflag = 3;

    if ( DEBUG1 ) printf("\nshdece:data_.kflag = %d",data_.kflag);
        dotee_.ncent = data_.lcent;                            /* dgb cent */
	dotee_.nyear = data_.lyear;
	dotee_.nmon = data_.lmon;
	dotee_.nday = data_.lday;
	dotee_.nhour = data_.lhour;
	dotee_.nmin = data_.lmin;
	dotee_.nsec = data_.lsec;

	/*             Adjust the forecast date to ZULU */

L_105:
    if ( DEBUG1 ) printf("\nshdece:at 105");
	k1 = data_.kyear;
	k2 = data_.kmon;
	k3 = data_.kday;
	k4 = data_.khour;
	k5 = data_.kmin;
	k6 = data_.ksec;
	if( data_.kmon == 0 )
		goto L_110;
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

	/*             Do the date relative adjustment for year, month
	 *             and day (LOCAL TIME) if necessary */


L_110:
    if ( DEBUG1 ) printf("\nshdece:at 110 data_.kflag = %d",data_.kflag);
	if( data_.kflag == 2 )
		goto L_130;
	if( ((datrel_.myear == 0) && (datrel_.mmon == 0)) && (datrel_.mday == 
	 0) )
		goto L_120;

    flag = 3;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &datrel_.mday, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 4;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &datrel_.mmon, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 5;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &datrel_.myear, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 6;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &datrel_.mend, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	shtdat( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9070;
		}

	/*             Convert to ZULU if necessary
	 *              - DIHXX, DINXX OR DISXX */

	if( (iunit >= 3 && iunit != 7) || (data_.nadjtz >= 0) )
		goto L_130;
	iadj = 0;

	shlocl( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &data_.nadjtz, &iadj, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 1;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &iadj, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	goto L_130;

	/*             Do the date relative adjustment for hour and minute
	 *             (ZULU TIME) if necessary */

L_120:
    if ( DEBUG1 ) printf("\nshdece:at 120");
	if( ((datrel_.mhour == 0) && (datrel_.mmin == 0)) && (datrel_.msec == 
	 0) )
		goto L_125;
	iadj = 0;

	shlocl( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &data_.nadjtz, &iadj, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 1;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &iadj, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 7;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &datrel_.msec, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 1;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &datrel_.mmin, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 2;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &datrel_.mhour, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

	/*             Convert back to local time if necessary
	 *             DIYXX,DIMXX,DIDXX,DIEXX */

	if( (iunit < 3 || iunit == 7) && (data_.nadjtz <= 0) )
		goto L_130;
	iadj = 1;
	shlocl( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &data_.nadjtz, &iadj, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	iadj = -iadj;
    flag = 1;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &iadj, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	goto L_130;

	/*             No date relatives, make sure the time zone is correct */

L_125:
    if ( DEBUG1 ) printf("\nshdece:at 125");
	if( (iunit >= 3 && iunit != 7) || (data_.nadjtz >= 0) )
		goto L_130;
	iadj = 0;
	shlocl( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &data_.nadjtz, &iadj, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 1;
	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &iadj, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

	/*             Now do the time increment
	 *             - we should be in the correct time zone at this stage */

L_130:
    if ( DEBUG1 ) printf("\nshdece:at 130 increment data_.kflag = %d",data_.kflag);

	if( data_.kflag != 2 )
		goto L_135;
	idote = 2;
    if ( DEBUG1 ) printf("\nshdece:before %d %d %d %d %d %d   intval=%d iunit=%d",
        dotee_.nyear, dotee_.nmon, dotee_.nday, dotee_.nhour, 
	    dotee_.nmin, dotee_.nsec, intval, iunit );

	shtadj( &dotee_.nyear, &dotee_.nmon, &dotee_.nday, &dotee_.nhour, 
	 &dotee_.nmin, &dotee_.nsec, &intval, &iunit, status );

    if ( DEBUG1 ) printf("\nshdece:before %d %d %d %d %d %d   intval=%d iunit=%d",
        dotee_.nyear, dotee_.nmon, dotee_.nday, dotee_.nhour, 
	    dotee_.nmin, dotee_.nsec, intval, iunit );

	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

	/*             If this is a NULL bypass the rest */

L_135:
    if ( DEBUG1 ) printf("\nshdece:at 135  null_ = %d",null_);
	if( null_ == 1 )
		goto L_160;

	/*             Adjust to ZULU for output if necessary */

        n11= dotee_.ncent;  /* dgb cent */
	n1 = dotee_.nyear;
	n2 = dotee_.nmon;
	n3 = dotee_.nday;
	n4 = dotee_.nhour;
	n5 = dotee_.nmin;
	n6 = dotee_.nsec;
	if( (iunit < 3 || iunit == 7) && (data_.nadjtz <= 0) )
		goto L_140;
	iadj = 0;

	shlocl( &n1, &n2, &n3, &n4, &n5, &n6, &data_.nadjtz, &iadj, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 1;
	shtadj( &n1, &n2, &n3, &n4, &n5, &n6, &iadj, &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

	/*             Test that the date is OK */

L_140:
	shtdat( &n1, &n2, &n3, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9070;
		}
		
      if ( data_.overyear > -1 && data_.overcentury == -1 ) /* dgb:01/18/99 */
         if ( data_.overyear > 30 ) n1 = data_.overyear + 1900;
      else
         if ( data_.overcentury > -1 && data_.overyear > -1 )
              n1 = (data_.overcentury * 100) + data_.overyear; /* dgb:01/18/99 */


      if ( data_.overmonth > -1 && data_.overyear == -1 )   /* dgb:01/18/99 */
      {
        itest = n2 - Idate[1];     
        if ( itest > 6 ) n1--;
        if ( itest < -6 ) n1++;
        if ( (itest == -6 ) & ( n3 < Idate[2] ) )
          n1++;
        if ( (itest == 6 ) && ( n3 > Idate[2] ) )
          n1--;
      }

	/* Write the data to SHEF_OUT */

    if ( DEBUG1 ) printf("\nshdece:wrting shefout");
    if ( data_.value != -10000 )                          /* dgb:01/27/98 */
    {
	if( cont_.out_flag[0] == '+' )
	{
           fprintf(fp_.jchn,
           "%c%c%c%c%c%c%c%c %4d %2d %2d %2d %2d %2d %4d %2d %2d %2d %2d %2d ",
           data_.idstn[0], data_.idstn[1], data_.idstn[2], data_.idstn[3],
           data_.idstn[4], data_.idstn[5], data_.idstn[6], data_.idstn[7],
           n1, n2, n3, n4, n5, n6, k1, k2, k3, k4, k5, k6);
           fprintf(fp_.jchn,
           "%c%c%c%c%c%c%c %4d %20.6f %20.6f %c %d %c%c%c%c%c%c%c%c %d %s\n",
           data_.kodp, data_.kode, durprob_.kodd, data_.kodt, data_.kods, 
           data_.kodex, durprob_.kodpr, idur, data_.codp, data_.value, lwal, *irev, 
           data_.msource[0], data_.msource[1], data_.msource[2], data_.msource[3],
           data_.msource[4], data_.msource[5], data_.msource[6], data_.msource[7],
           idote, comment_.comment );                      /* dgb:09/10/97 */

           if ( test_.atest_flag )                         /* dgb:12/01/97 */
           {
           fprintf(fp_.icher,
           "%c%c%c%c%c%c%c%c %4d %2d %2d %2d %2d %2d %4d %2d %2d %2d %2d %2d ",
           data_.idstn[0], data_.idstn[1], data_.idstn[2], data_.idstn[3],
           data_.idstn[4], data_.idstn[5], data_.idstn[6], data_.idstn[7],
           n1, n2, n3, n4, n5, n6, k1, k2, k3, k4, k5, k6);
           fprintf(fp_.icher,
           "%c%c%c%c%c%c%c %4d %20.6f %20.6f %c %d %c%c%c%c%c%c%c%c %d %s\n",
           data_.kodp, data_.kode, durprob_.kodd, data_.kodt, data_.kods, 
           data_.kodex, durprob_.kodpr, idur, data_.codp, data_.value, lwal, *irev, 
           data_.msource[0], data_.msource[1], data_.msource[2], data_.msource[3],
           data_.msource[4], data_.msource[5], data_.msource[6], data_.msource[7],
           idote, comment_.comment );                      /* dgb:09/10/97 */
           }

	}

	/*             Pass data to SHEFPASS */

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
                  n1, 
                  n2, 
                  n3, 
                  n4, 
                  n5, 
                  n6, 
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
       }                                                   /* dgb:01/27/98 */
     memset(comment_.comment,0,sizeof(comment_.comment));  /* dgb:09/10/97 */
     data_.overmonth   = -1;                               /* dgb:01/18/99 */
     data_.overyear    = -1;                               /* dgb:01/18/99 */
     data_.overcentury = -1;                               /* dgb:01/18/99 */

 	if( iend_ == 1 )
		goto L_9000;

	/*             Clear past the next slash */

	if( nflag == 0 )
		goto L_160;
L_150:
    if ( DEBUG1 ) printf("\nshdece:at 150");
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
L_160:
    if ( DEBUG1 ) printf("\nshdece:at 160");
	if( xchar_.ichar == codes_.iblnk )
		goto L_150;
	if( xchar_.ichar != codes_.islash )
		goto L_9130;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_170;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	dotee_.inul = 0; 
	goto L_30;

L_165:
    if ( DEBUG1 ) printf("\nshdece:at 165 dtype_.ieee = %d  data_.kflag=%d",dtype_.ieee, data_.kflag);
	if( (dtype_.ieee == 1) && (data_.kflag > 0) )
		data_.kflag = 1;
L_170:
    if ( DEBUG1 ) printf("\nshdece:at 170");
	dotee_.inul = 1;

	/*             Error return - stop looking at this format */

L_9000:
    if ( DEBUG1 ) printf("\nshdece:at 9000");
	*status = 1;

	return;
	/*             Error return - stop looking at this file */

L_9010:
    if ( DEBUG1 ) printf("\nshdece:at 9010");
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
	num = 29;
	format_.nerr = 1;
	goto L_9200;

L_9110:
	num = 33;
	format_.nerr = 1;
	goto L_9200;

L_9120:
	num = 35;
	sendflg_.nsflag = 0;
	goto L_9200;

L_9130:
	num = 31;
	format_.nerr = 1;

L_9200:
	sherr( &num );
	goto L_9000;

}

