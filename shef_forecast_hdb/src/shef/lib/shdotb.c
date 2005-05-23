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
         SUBROUTINE SHDOTB(STATUS)
  
       PURPOSE                                                    
         Decode the body of the .B FORMAT.
         The header information for each expected data value is
         stored on the file.
  
       VERSION and UPDATES
         1.0    AUG 82     GEOFFREY M BONNIN  MBRFC        
         1.1    JAN 84                                          
                Correct -1 date relative bug                    
                Improve testing for bad dates on date           
                relative adjustment                             
         1.2    AUG 84                                          
                Change test for creation date to non-fatal      
                Change test for error 32 to non-fatal           
                Add seconds time                                
         1.3    NOV 84                                          
                Virtual buffering of DOTBTEMP                   
         1.4    MAY 85                                          
                Correct error in adjusting to PREV 7AM          
         1.5    JAN 94  Robert Hartman                                        
                Correct year error when .B header defaults to a 
                different year than the default for the .B     
                body.                                           
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
                Allow for all valid qualifier codes excepting for
                'O' and 'I'.
         2.4    DEC 95 DGB
                Do not process DC when they are missing, or the month
                k2 is set to 0
         2.5    JAN 96 DGB
                Update struct t_data to include lcent 
                and kcent for the century.
                Modify code to print out correct century for
                shefout file and shefpass.
         2.6    FEB 11, 96 DGB
                Changed code to accomodate the century in the positional
                date, date of creation and date override. 
         2.7x   SEP 10 97 DGB
                Add the use of MAX_SHEF_INPUT so that the input line can
                be longer than 80 characters.
                Add capability to output a database comment.
         2.8    DEC 1 97 DGB
                Add advanced test option will will print a shefout
                record to the error file.
         2.9    JAN 17 98 DGB
                The incorrect year was calculated for certain values
                when an override was specified.  For example, 
                if the system year was 98 but december was specified in 
                the positional data, the year was not calculated 
                correctly.
                A few sample messages that were not (but now are)
                all correctly determined is now fixed:
                               
                .B MCI 1231 Z DH07/HG
                KCDM6 DY940101/8.3
                SMHM6 DM1231/1.14
                SMHM6 DM0102/1.14
                AGYM6 11.9
                .END
                .B MCI 971231 Z DH07/HG
                KCDM6 DY940101/8.3
                SMHM6 DM0102/1.14
                SMHM6 DM1231/1.14
                AGYM6 11.9
                .END
                .B MCI 19971231 Z DH07/HG
                SMHM6 DM0102/1.14
                KCDM6 DY940101/8.3
                SMHM6 DM1231/1.14
                AGYM6 11.9
                .END
         3.0    MAR 3 98 DGB
                Try fixing what was supposed to be fixed in 2.9.    
         3.1    DEC 5 98 DGB
                Try to adjust mon/day on overrides only if year 
                is not respecified in the data line.  Check with
                an if test for ldum1.
         3.2    JAN 8 99 DGB
                Fix check for year when there is an override.
 *--------------------------------------------------------------------- */



void shdotb(status)
short int *status;
{
    /* dgb:02/11/96 added lcent, kcent, ncent, lcentdum1,kcentdum1,lcentfl1,mcent */
	static short int i, i_, iadj, idcodd, idote, idur, iend_, 
	irev, item, itest, ival, k1, k2, k3, k4, k5, k6, kday, 
	kdum4, kdum5, kdum6, khour, kmin, kmon, kodu, ksec, kwal, kyear,
	lday, ldfk4, ldfk5, ldfk6, ldfl1, ldfl4, ldfl5, ldfl6, ldum1, 
	ldum4, ldum5, ldum6, lflag, lhour, lmin, lmon, lsec, lwal, 
        lyear, mday, mend, mhour, mmin, mmon, msec, myear, nchar, nday, ndig, 
	nflag, nhour, nmin, nmon, nsec, num, nyear, flag,
        lcentdum1, kcentdum1, kcent,ncent,lcent,lcentfl1, mcent;
	static double factor;
        int DEBUG1, DEBUG2;
   
	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Ibuf = &buffer_.ibuf[0] - 1;
	short *const Idate = &datim_.idate[0] - 1;
	short *const Idstn = &data_.idstn[0] - 1;
	short *const Msource = &data_.msource[0] - 1;

    DEBUG1 = 0;
    DEBUG2 = 0;

	/*             Initialise */
L_10:
    if ( DEBUG1 ) printf("\nshdotb: at L_10 ");
    data_.overcentury = -1;                                /* dgb:01/18/99 */
    data_.overyear    = -1;                                /* dgb:01/18/99 */
    data_.overmonth   = -1;                                /* dgb:01/18/99 */

	Msource[1] = codes_.iblnk;                         /* dgb:09/10/97 */
	Msource[2] = codes_.iblnk;
	Msource[3] = codes_.iblnk;
	Msource[4] = codes_.iblnk;
	Msource[5] = codes_.iblnk;
	Msource[6] = codes_.iblnk;
	Msource[7] = codes_.iblnk;
	Msource[8] = codes_.iblnk;

	*status = 0;
	item = 0;
	bbuf1_.nmrec = 0;
	dotbee_.nelem = 0;
	idote = 0;
	Idstn[1] = codes_.iblnk;
	Idstn[2] = codes_.iblnk;
	Idstn[3] = codes_.iblnk;
	Idstn[4] = codes_.iblnk;
	Idstn[5] = codes_.iblnk;
	Idstn[6] = codes_.iblnk;
	Idstn[7] = codes_.iblnk;
	Idstn[8] = codes_.iblnk;
	ldum1         = -31999;
        lcentdum1     = -31999;                                    /* dgb:02/11/96 add */
	data_.lmon    = -31999;
	data_.lday    = -31999;
	ldum4         = -31999;
	ldum5         = -31999;
	ldum6         = -31999;
        data_.kcent   = -31999;
	data_.kyear   = -31999;
	data_.kmon    = -31999;
	data_.kday    = -31999;
	kdum4         = -31999;
	kdum5         = -31999;
	kdum6         = -31999;
        datrel_.mcent = -31999;                                /* dgb:02/11/96 add*/
	datrel_.myear = -31999;
	datrel_.mmon  = -31999;
	datrel_.mday  = -31999;
	datrel_.mhour = -31999;
	datrel_.mmin  = -31999;
	datrel_.msec  = -31999;
	datrel_.mend  = -31999;
	data_.idcodd  = -31999;
	data_.kwal    = -31999;
	data_.kodu    = -31999;

	goto L_25;

	/*             Now get the station ID */

L_20:
    if ( DEBUG1 ) printf("\nshdotb: at L_20 getting ID");
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9020;
		}
L_25:
	if( xchar_.ichar == codes_.iblnk )
		goto L_20;

	/*             Read up to 8 chars, checking for legit ones */

	nchar = 0;

	for( i = 1; i <= 8; i++ )
		{
		i_ = i - 1;
		irang( &xchar_.ichar, &codes_.icha, &codes_.ichz, status );
		if( *status == 1 )
			{
			*status = 0;
			goto L_15;
			}
		goto L_40;

L_15:
		irang( &xchar_.ichar, &codes_.ich0, &codes_.ich9, status );
		if( *status == 1 )
			{
			*status = 0;
			goto L_16;
			}
        goto L_40;


L_16:   if ( xchar_.ichar == codes_.iscore )               /* dgb:11/07/95 */
             goto L_40;
        else
             goto L_9030;

L_40:   Idstn[i] = xchar_.ichar;
		nchar = nchar + 1;
		nextch( &xchar_.ichar, status );
		if( *status == 1 )
			{
			*status = 0;
			goto L_190;
			}
		else if( *status == 2 )
			{
			*status = 0;
			goto L_9020;
			}
		if( xchar_.ichar == codes_.iblnk )
			goto L_60;
		}

	if( xchar_.ichar != codes_.iblnk )
		goto L_9120;
L_60:
	if( nchar < 3 )
		goto L_9040;

	/*             Test for too many items */

L_65:
    if ( DEBUG1 ) printf("\nshdotb: at L_65 reading data header");
	item = item + 1;
	if( item > luns_.mrec )
		goto L_70;




	/*             Read the data header */

    /* dgb:02/11/96 add &lcent, *kcent, &mcent */
	shbfr( data_.msource, &lcent, &lyear, &lmon, &lday, &lhour, &lmin, &lsec, 
	 &kcent,&kyear, &kmon, &kday, &khour, &kmin, &ksec, &mcent, &myear, &mmon, &mday, 
	 &mhour, &mmin, &msec, &mend, &data_.kodp, &data_.kode, &data_.icodd, 
	 &idcodd, &data_.kodt, &data_.kods, &data_.kodex, &data_.codp, 
	 &kwal, &data_.nadjtz, &kodu, &factor, &irev, &durprob_.kodd, 
	 &durprob_.kodpr );

	/*             Get the value */
L_70:
    if  ( DEBUG1 ) printf("\nshdotb: at L_70 getting value");
    flag = 1;
	shreal( &data_.value, &flag, &ndig, status );
    if ( DEBUG1 ) printf("\nshdotb: status from shreal = %d",*status);
	if( *status == 1 )
		{
		*status = 0;
		goto L_74;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9010;
		}
	iend_ = 0;
	goto L_77;

L_74:
	iend_ = 1;
L_77:
    if ( DEBUG2 ) printf("\nshdotb:  ndig = %d iend_ = %d luns_.mrec = %d",ndig,iend_,luns_.mrec);
	if( (ndig == 0) && (iend_ == 0) )
		goto L_150;
	if( (ndig == 0) && (iend_ == 1) )
		goto L_190;
	if( ndig == -1 )
		goto L_65;
	if( item > luns_.mrec )
		goto L_9050;

	/*             Check for changes in D type elements */
    if( lcentdum1 != -31999 )                              /* dgb:02/11/96 add */
        lcent = lcentdum1;      
	if( ldum1 != -31999 )
		lyear = ldum1;
	if( data_.lmon != -31999 )
		lmon = data_.lmon;
	if( data_.lday != -31999 )
		lday = data_.lday;
	if( ldum4 != -31999 )
		lhour = ldum4;
	if( ldum5 != -31999 )
		lmin = ldum5;
	if( ldum6 != -31999 )
		lsec = ldum6;
    if( data_.kcent != -31999 )
        kcent = data_.kcent;  
	if( data_.kyear != -31999 )
		kyear = data_.kyear;
	if( data_.kmon != -31999 )
		kmon = data_.kmon;
	if( data_.kday != -31999 )
		kday = data_.kday;
	if( kdum4 != -31999 )
		khour = kdum4;
	if( kdum5 != -31999 )
		kmin = kdum5;
	if( kdum6 != -31999 )
		ksec = kdum6;
    if( datrel_.mcent != -31999 )                          /* dgb:02/11/96 add */
        mcent = datrel_.mcent;
	if( datrel_.myear != -31999 )
		myear = datrel_.myear;
	if( datrel_.mmon != -31999 )
		mmon = datrel_.mmon;
	if( datrel_.mday != -31999 )
		mday = datrel_.mday;
	if( datrel_.mhour != -31999 )
		mhour = datrel_.mhour;
	if( datrel_.mmin != -31999 )
		mmin = datrel_.mmin;
	if( datrel_.msec != -31999 )
		msec = datrel_.msec;
	if( datrel_.mend != -31999 )
		mend = datrel_.mend;
	if( data_.idcodd != -31999 )
		idcodd = data_.idcodd;
	if( data_.kwal != -31999 )
		kwal = data_.kwal;
	if( data_.kodu != -31999 )
		kodu = data_.kodu;

	/*             Adjust for TRACE */

	if( data_.value >= -8e10 )
		goto L_80;
	if( data_.kodp != codes_.ichp )
		goto L_78;
	if( data_.kode == codes_.ichc )
		goto L_79;
	if( data_.kode == codes_.ichp )
		goto L_79;
	goto L_9060;

L_78:
	if( data_.kodp != codes_.ichs )
		goto L_9060;
	if( data_.kode == codes_.ichd )
		goto L_79;
	if( data_.kode == codes_.ichf )
		goto L_79;
	if( data_.kode == codes_.ichw )
		goto L_79;
	goto L_9060;

L_79:
	data_.value = 0.001e0;
	goto L_90;

	/*             Convert SI to english units if necessary */

L_80:
	if( kodu == 1 )
		goto L_90;
	ival = data_.value - 0.01;
	if( ival == -9999 )
		goto L_90;
	if( ival == -9002 )
		goto L_90;
	if( factor < 0e0 )
		data_.value = data_.value*1.8e0 + 32e0;
	if( factor > 0e0 )
		data_.value = data_.value*factor;

	/*  Test the qualifier - it should be in ICHAR already */

L_90:
    if ( DEBUG1 ) printf("\nshdotb: at L_90 testing qualifier");
	if( iend_ == 1 ) goto L_95;
	
	/* pass all alphabetical characters except 'O' and 'I'   dgb:11/13/95 */
	irang(&xchar_.ichar,&codes_.icha,&codes_.ichz,status);
	if ( !*status )
	{
	   *status = 0;
	   if ( xchar_.ichar == codes_.icho )  goto L_9090;
	   if ( xchar_.ichar == codes_.ichi )  goto L_9090;
	   goto L_100;
	}

    *status = 0;
	if( xchar_.ichar == codes_.iblnk )  goto L_95;
	if( xchar_.ichar == codes_.islash ) goto L_95;
	if( xchar_.ichar == codes_.icomma ) goto L_95;

	goto L_9090;

L_95:
	lwal = kwal;
	nflag = 0;
	goto L_110;

L_100:
	lwal = xchar_.ichar;
	nflag = 1;

	/*             Adjust the observation time to 7AM previous day if necessary.
	 *             Can't do it if ZULU time or date rel has been specified */

L_110:
    ncent = lcent;                                         /* dgb:02/11/96 add */
	nyear = lyear;
	nmon = lmon;
	nday = lday;
	nhour = lhour;
	nmin = lmin;
	nsec = lsec;
	if( sendflg_.nsflag == 0 )
		goto L_115;  	                                   /*DGB: CHECK SEND FLAG */
	if( data_.nadjtz == 0 )
		goto L_9130;
    if( mcent != 0 )                                       /* dgb:02/11/96 add */
        goto L_9130;
	if( myear != 0 )
		goto L_9130;
	if( mmon != 0 )
		goto L_9130;
	if( mday != 0 )
		goto L_9130;
	if( mhour != 0 )
		goto L_9130;
	if( mmin != 0 )
		goto L_9130;
	if( msec != 0 )
		goto L_9130;
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
		goto L_9100;
		}

	nhour = 7;
	nmin = 0;
	nsec = 0;
	goto L_117;

	/*             Adjust for date relative for year, month and day */

L_115:
    flag = 3;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &mday, &flag, 
	 status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

    flag = 4;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &mmon, &flag, 
	 status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

    flag = 5;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &myear, &flag, 
	 status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

    flag = 6;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &mend, &flag, 
	 status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

	shtdat( &nyear, &nmon, &nday, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9100;
		}

	/*             Test for local time zone and adjust to ZULU time */

L_117:
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
	k1 = kyear;
	k2 = kmon;
	k3 = kday;
	k4 = khour;
	k5 = kmin;
	k6 = ksec;
	if( kmon == 0 )
		goto L_119;
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

	/*             Adjust for date relative for hour and minute */

L_119:
    flag = 7;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &msec, &flag, 
	 status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 1;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &mmin, &flag, 
	 status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
    flag = 2;
	shtadj( &nyear, &nmon, &nday, &nhour, &nmin, &nsec, &mhour, &flag, 
	 status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}

	/*             Test for correct duration */

	if( (data_.icodd == 5003) && (idcodd == 5000) )
    {
		goto L_9070;
    }
	idur = data_.icodd;
	if( data_.icodd == 5003 )
		idur = idcodd;

	/*             Test if creation date specified for forecast data */

	if( (data_.kodt == codes_.ichf) && (kmon == 0) )
    {
        flag = 23;
		sherr( &flag );
    }


   /* Test for correct year if a date override was specified */
      if ( data_.overyear > -1 && data_.overcentury == -1 )
          if ( data_.overyear > 30 ) nyear = data_.overyear + 1900;
      else
         if ( data_.overcentury > -1 && data_.overyear > -1 )
            nyear = ( data_.overcentury * 100 ) + data_.overyear; /* dgb:01/18/99 */


     if ( data_.overmonth > -1 && data_.overyear == -1 )                                     /* dgb:01/07/99 */
     {
      itest = nmon - bbuf2_.iarray[bbuf1_.nmrec - 1][1];     
      if ( itest > 6 ) nyear--;
      if ( itest < -6 ) nyear++;
      if ( (itest == -6 ) & ( lday < bbuf2_.iarray[bbuf1_.nmrec-1][2] ) )
        nyear++;
      if ( (itest == 6 ) && ( lday > bbuf2_.iarray[bbuf1_.nmrec-1][2] ) )
        nyear--;
     }
    
    /* bbuf2_.iarray[bbuf1_.nmrec - 1][2];  lday */
    /* bbuf2_.iarray[bbuf1_.nmrec - 1][3];  lhour */
    /* bbuf2_.iarray[bbuf1_.nmrec - 1][4];  lmin */
    /* bbuf2_.iarray[bbuf1_.nmrec - 1][5];  lsec */
    /* bbuf2_.iarray[bbuf1_.nmrec - 1][24]; lcent */

	/*             Write the data to SHEF_OUT */

    if ( DEBUG1 ) printf("\nshdotb: writing shefout record");
    if ( data_.value != -10000 )                           /* dgb:02/04 98 */
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
           data_.kodex, durprob_.kodpr, idur, data_.codp, data_.value, lwal, irev, 
		   data_.msource[0], data_.msource[1], data_.msource[2], data_.msource[3],
		   data_.msource[4], data_.msource[5], data_.msource[6], data_.msource[7],
           idote, comment_.comment );                      /* dgb:09/10/97 */ 

           if ( test_.atest_flag )                         /* dgb:12/01/97 */
           {
           fprintf(fp_.icher,
           "%c%c%c%c%c%c%c%c %4d %2d %2d %2d %2d %2d %4d %2d %2d %2d %2d %2d ",
		   data_.idstn[0], data_.idstn[1], data_.idstn[2], data_.idstn[3],
		   data_.idstn[4], data_.idstn[5], data_.idstn[6], data_.idstn[7],
           nyear, nmon, nday, nhour, nmin, nsec, k1, k2, k3, k4, k5, k6);
           fprintf(fp_.icher,
           "%c%c%c%c%c%c%c %4d %20.6f %20.6f %c %d %c%c%c%c%c%c%c%c %d %s\n",
           data_.kodp, data_.kode, durprob_.kodd, data_.kodt, data_.kods, 
           data_.kodex, durprob_.kodpr, idur, data_.codp, data_.value, lwal, irev, 
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
                  irev,
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
     }                                                     /* dgb:02/04/98 */
    memset(comment_.comment,0,sizeof(comment_.comment));   /* dgb:/09/10/97 */

    data_.overmonth   = -1;                                /* dgb:01/18/99 */
    data_.overyear    = -1;                                /* dgb:01/18/99 */ 
    data_.overcentury = -1;                                /* dgb:01/18/99 */
	if( iend_ == 1 )
		goto L_190;
	goto L_160;

	/*             Check for date and data type elements */

L_150:
    if ( DEBUG1 ) printf("\nshdotb: at L_150 check for data and data type");
	if( xchar_.ichar == codes_.islash )
		goto L_65;

	ldfl1 = 0;
	if( ldum1 != -31999 )
		ldfl1 = 1;
	data_.lyear = lyear;
	if( ldfl1 == 1 )
		data_.lyear = ldum1;

    lcentfl1 = 0;                                          /* dgb: 02/11/96 add */
    if ( lcentdum1 != -31999 )                             /* dgb: 02/11/96 add */
        lcentfl1 = 1;                                      /* dgb: 02/11/96 add */
    data_.lcent = lcent;                                   /* dgb: 02/11/96 add */
    if ( lcentfl1 == 1 )                                   /* dgb: 02/11/96 add */
         data_.lcent = lcentdum1;                          /* dgb: 02/11/96 add */


	/*             The following five lines of code were added by
	 *             Robert Hartman to correct year error when .B header
	 *             defaults to a difference year than the default
	 *             for the .B body */

/*
	itest = lmon - Idate[1];
	if( itest > 6 )
		data_.lyear = data_.lyear - 1;
	if( itest < -6 )
		data_.lyear = data_.lyear + 1;
	if( (itest == -6) && (lday < Idate[2]) )
		data_.lyear = data_.lyear + 1;
	if( (itest == 6) && (lday > Idate[2]) )
		data_.lyear = data_.lyear - 1;
*/
	ldfl4 = 0;
	if( ldum4 != -31999 )
		ldfl4 = 1;
	data_.lhour = lhour; 
	if( ldfl4 == 1 )
		data_.lhour = ldum4;

	ldfl5 = 0;
	if( ldum5 != -31999 )
		ldfl5 = 1;
	data_.lmin = lmin;
	if( ldfl5 == 1 )
		data_.lmin = ldum5;

	ldfl6 = 0;
	if( ldum6 != -31999 )
		ldfl6 = 1;
	data_.lsec = lmin;
	if( ldfl6 == 1 )
		data_.lsec = ldum6;

	ldfk4 = 0;
	if( kdum4 != -31999 )
		ldfk4 = 1;
	data_.khour = khour;
	if( ldfk4 == 1 )
		data_.khour = kdum4;

	ldfk5 = 0;
	if( kdum5 != -31999 )
		ldfk5 = 1;
	data_.kmin = kmin;
	if( ldfk5 == 1 )
		data_.kmin = kdum5;

	ldfk6 = 0;
	if( kdum6 != -31999 )
		ldfk6 = 1;
	data_.ksec = kmin;
	if( ldfk6 == 1 )
		data_.ksec = kdum6;

    flag = 4;
	shdtype( &flag, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9010;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9020;
		}
	else if( *status == 3 )
		{
		*status = 0;
		goto L_9000;
		}


	if( (ldfl1 == 0) && (data_.lyear != lyear) )
    {
        data_.overyear = data_.lyear;
		ldum1 = data_.lyear;
    }
	if( ldfl1 == 1 )
		ldum1 = data_.lyear;

    /* dgb: 02/11/96 add next 4 lines */
	if( (lcentfl1 == 0) && (data_.lcent != lcent) )
    {
        data_.overcentury = data_.lcent;
		lcentdum1 = data_.lcent;
    }

	if( lcentfl1 == 1 )
		lcentdum1 = data_.lcent;


	if( (ldfl4 == 0) && (data_.lhour != lhour) )
		ldum4 = data_.lhour;
	if( ldfl4 == 1 )
		ldum4 = data_.lhour;

	if( (ldfl5 == 0) && (data_.lmin != lmin) )
		ldum5 = data_.lmin;
	if( ldfl5 == 1 )
		ldum5 = data_.lmin;

	if( (ldfl6 == 0) && (data_.lsec != lmin) )
		ldum6 = data_.lsec;
	if( ldfl6 == 1 )
		ldum6 = data_.lsec;

	if( (ldfk4 == 0) && (data_.khour != khour) )
		kdum4 = data_.khour;
	if( ldfk4 == 1 )
		kdum4 = data_.khour;

	if( (ldfk5 == 0) && (data_.kmin != kmin) )
		kdum5 = data_.kmin;
	if( ldfk5 == 1 )
		kdum5 = data_.kmin;

	if( (ldfk6 == 0) && (data_.ksec != kmin) )
		kdum6 = data_.ksec;
	if( ldfk6 == 1 )
		kdum6 = data_.ksec;

	goto L_170;

	/*             Clear past the next slash */

L_160:
    if ( DEBUG1 ) printf("\nshdotb: at L_160 clear past next shlash");
	if( nflag == 0 )
		goto L_170;
L_165:
    if ( DEBUG1 ) printf("\nshdotb: at L_165");
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_190;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9020;
		}
L_170:
    if ( DEBUG1 ) printf("\nshdotb: at L_170");
	if( xchar_.ichar == codes_.iblnk )
		goto L_165;
	if( xchar_.ichar == codes_.islash )
		goto L_175;
	if( xchar_.ichar == codes_.icomma )
		goto L_180;
	goto L_9090;

L_175:
    if ( DEBUG1 ) printf("\nshdotb: at L_175");
    if ( DEBUG2 ) printf("\nshdotb: dotbee_.nelem = %d",dotbee_.nelem);
	if( dotbee_.nelem == 0 )
		goto L_65;
	dotbee_.nelem = 0;
	goto L_70;

	/*             Check if all the items appeared */

L_180:
    if ( DEBUG1 ) printf("\nshdotb: at L_180 check if items appeared");
	lflag = 1;
	goto L_200;

L_190:
    if ( DEBUG1 ) printf("\nshdotb: at L_190");
	lflag = 0;
L_200:
    if ( DEBUG1 ) printf("\nshdotb: at L_200");
	if( item < luns_.mrec )
    {
        flag = 32;
		sherr( &flag );
    }
	if( iend_ == 1 )
		goto L_9000;
	if( lflag == 0 )
		goto L_9000;
	goto L_230;

	/*             Clear to a comma or the end of the line */

L_220:
    if ( DEBUG1 ) printf("\nshdotb: at L_220 clear to comma or end of line");
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_9000;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9020;
		}
L_230:
    if ( DEBUG1 ) printf("\nshdotb: at L_230");
	if( xchar_.ichar != codes_.icomma )
		goto L_220;
	nextch( &xchar_.ichar, status );
	if( *status == 1 )
		{
		*status = 0;
		goto L_190;
		}
	else if( *status == 2 )
		{
		*status = 0;
		goto L_9020;
		}
	goto L_10;

L_9000:
    if ( DEBUG1 ) printf("\nshdotb: at L_9000  return ");
	return;

L_9010:
	buffer_.ip = MAX_SHEF_INPUT + 1;                       /* dgb:09/10/97 */
	*status = 1;
	return;

L_9020:
	*status = 2;
	return;

L_9030:
	num = 6;
	goto L_9200;

L_9040:
	num = 3;
	goto L_9200;

L_9050:
    flag = 30;
	sherr( &flag );
	goto L_230;

L_9060:
	num = 28;
	goto L_9200;

L_9070:
	num = 25;
	goto L_9200;

L_9090:
	num = 31;
	goto L_9200;

L_9100:
	num = 24;
	goto L_9200;

L_9120:
	num = 2;
	goto L_9200;

L_9130:
	num = 35;
L_9200:
	sherr( &num );
	dotbee_.nerror = dotbee_.nerror + 1;
	if( dotbee_.nerror >= 3 )
		goto L_9010;
	goto L_180;

}


