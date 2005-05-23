/*Translated by FOR_C, v3.4.2 (-), on 04/06/95 at 13:53:42 */
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
         BLOCKDATA
  
       PURPOSE                                                    
         Initialize data arrays
  
       VERSION and UPDATES
         1.0    APR 82   Geoffrey M. Bonnin
                Original Version
         1.1    JUN 89   David G. Brandon
                Adapted to QNX NWS Hydromet
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
                Added error messages, moved daylight/standard time
                switch over times.
         2.1    OCT 95   dgb
                Include the underscore '_' as a character.
         2.2    JAN 96 
                Remove FORC initializations and replace them with
                simple assignment statements.
         2.3    Increase message5 array to 63.  Modify error message5
                to state, 'not enough digits or bad value'. Add warning message
                about not checking for daylights savings time if
                year is between {1975 and 2021}.
         2.4    Add codes_.dquote.
                Add error messages 39 and 40.
         2.5    OCT 19 97 DGB
                Change datet_.itable array numbers for switchover 
                times to daylight savings time.
 *--------------------------------------------------------------------- */


void blk_data()
{
	/* OFFSET Vectors w/subscript range: 1 to dimension */
	short *const Iday = &days_.iday[0] - 1;


	/*  Error Messages */

		strcpy( err1_.message1, "Error No.  1 ->This line not decoded" );
		strcpy( err1_.message2, "Error No.  2 ->No space in positional data" );
		strcpy( err1_.message3, "Error No.  3 ->Less than 3 characters in ID of message source");
		strcpy( err1_.message4, "Error No.  4 ->TZ code error" );
		strcpy( err1_.message5, "Error No.  5 ->Date group error-not enough digits or bad value" );
		strcpy( err2_.message6, "Error No.  6 ->Illegal character in Id or message source" );
		strcpy( err2_.message7, "Error No.  7 ->Error in date code" );
		strcpy( err2_.message8, "Error No.  8 ->Observation time error" );
		strcpy( err2_.message9, "Error No.  9 ->Date relative code error" );
		strcpy( err2_.message10, "Error No. 10 ->Julian day error" );
		strcpy( err3_.message11, "Error No. 11 ->Illegal data string qualifier" );
		strcpy( err3_.message12, "Error No. 12 ->Units code error" );
		strcpy( err3_.message13, "Error No. 13 ->Not a date or data type element" );
		strcpy( err3_.message14, "Error No. 14 ->Not a date or data type...maybe missing slash");
		strcpy( err3_.message15, "Error No. 15 ->Illegal character in parameter code" );
		strcpy( err4_.message16, "Error No. 16 ->File read error on shef_parm" );
		strcpy( err4_.message17, "Error No. 17 ->Non-existant parameter code" );
		strcpy( err4_.message18, "Error No. 18 ->Parameters coded with a send cod" );
		strcpy( err4_.message19, "Error No. 19 ->Continuation of a format does not follow the correct format");
		strcpy( err4_.message20, "Error No. 20 ->A format revision continuation follows an original");
		strcpy( err5_.message21, "Error No. 21 ->The format that this is continuing had an error");
		strcpy( err5_.message22, "Error No. 22 ->Year not in the range 1976-2020 for local time zone");
        strcpy( err5_.message221,"               ...use Z for time zone for historical data");
		strcpy( err5_.message23, "Warng No. 23 ->Forecast data without creation date" );
		strcpy( err5_.message24, "Error No. 24 ->Bad data somehow" );
		strcpy( err5_.message25, "Error No. 25 ->DV not defined for ZZV" );
		strcpy( err6_.message26, "Error No. 26 ->DV code error" );
		strcpy( err6_.message27, "Error No. 27 ->DI code error" );
		strcpy( err6_.message28, "Error No. 28 ->Trace specified for other than PP,PC,SF,SD or SW");
		strcpy( err6_.message29, "Error No. 29 ->No time increment specified" );
		strcpy( err6_.message30, "Error No. 30 ->To many items in .B body line" );
		strcpy( err7_.message31, "Error No. 31 ->Bad character in the line" );
		strcpy( err7_.message32, "Warng No. 32 ->Not enough items in .B body line" );
		strcpy( err7_.message33, "Error No. 33 ->No value specified" );
		strcpy( err7_.message34, "Error No. 34 ->No .END at end of .B" );
		strcpy( err7_.message35, "Error No. 35 ->Zulu,DR or DI coded with send code QY,PY or HY");
		strcpy( err8_.message36, "Error No. 36 ->The explicit date referenced by DRE is not the end of the month");
		strcpy( err8_.message37, "Error No. 37 ->Obervation or creation time is between 020001 and 025959");
		strcpy( err8_.message371, "on the date of change from standard to daylight time" );
        strcpy( err8_.message38, "Warning No.38 ->No check for daylight savings time-year out of bounds [1976-2020]");
        strcpy( err8_.message39, "Error No. 39 ->Embedded database comments with internal comments not allowed");
        strcpy( err8_.message40, "Error No. 40 ->Database comment too long");

        days_.iday[0] = 31;
        days_.iday[1] = 28;
        days_.iday[2] = 31;
        days_.iday[3] = 30;
        days_.iday[4] = 31;
        days_.iday[5] = 30;
        days_.iday[6] = 31;
        days_.iday[7] = 31;
        days_.iday[8] = 30;
        days_.iday[9] = 31;
        days_.iday[10] = 30;
        days_.iday[11] = 31;


	/*              Daylight/Standard Switch over Dates */
	/*      The switch over dates for daylight and standard time for the
	        years 1976 through 2020 are contained in the structure
            datet_.itable[][].
	        In 1987, the law went into effect that made the change over
	        on the first Sunday in April, and the last Sunday in October.
	        The datet_.itable[y][x] structure contains years 1976 - 2020.
            The [y] index contains the years..the [x] index is sized to
            2 elements.  The first element is the switchover day in
            April, the second element is the switchover day in
            October. 
	        The change occurs at 2AM. 
     */

        /* 1976 */        
		datet_.itable[0][0]  = 26; 
		datet_.itable[0][1]  = 31;

        /* 1977 */
	    datet_.itable[1][0]  = 24; 
	    datet_.itable[1][1]  = 30;

        /* 1978 */
		datet_.itable[2][0]  = 30; 
		datet_.itable[2][1]  = 29;

        /* 1979 */
		datet_.itable[3][0]  = 29; 
	    datet_.itable[3][1]  = 28;

        /* 1980 */
		datet_.itable[4][0]  = 27; 
		datet_.itable[4][1]  = 26;

        /* 1981 */
		datet_.itable[5][0] = 26;  
		datet_.itable[5][1] = 25;

        /* 1982 */
		datet_.itable[6][0] = 25;  
		datet_.itable[6][1] = 31;

        /* 1983 */
		datet_.itable[7][0] = 24;  
	    datet_.itable[7][1] = 30;

        /* 1984 */
		datet_.itable[8][0] = 29;  
		datet_.itable[8][1] = 28;

        /* 1985 */
		datet_.itable[9][0] = 28;  
		datet_.itable[9][1] = 27;

        /* 1986 */
		datet_.itable[10][0] = 27; 
		datet_.itable[10][1] = 26;

        /* 1987 */
	    datet_.itable[11][0] = 5;  
		datet_.itable[11][1] = 25;

        /* 1988 */
		datet_.itable[12][0] = 3;  
		datet_.itable[12][1] = 30;

        /* 1989 */
		datet_.itable[13][0] = 2;  
		datet_.itable[13][1] = 29;

        /* 1990 */
		datet_.itable[14][0] = 1;  
		datet_.itable[14][1] = 28;

        /* 1991 */
		datet_.itable[15][0] = 7;  
		datet_.itable[15][1] = 27;

        /* 1992 */
		datet_.itable[16][0] = 5;  
		datet_.itable[16][1] = 25;

        /* 1993 */
		datet_.itable[17][0] = 4;  
		datet_.itable[17][1] = 31;

        /* 1994 */
		datet_.itable[18][0] = 3;  
		datet_.itable[18][1] = 30;

        /* 1995 */
		datet_.itable[19][0] = 2;  
		datet_.itable[19][1] = 29;

        /* 1996 */
		datet_.itable[20][0] = 7;  
		datet_.itable[20][1] = 27;

        /* 1997 */
		datet_.itable[21][0] = 6;  
		datet_.itable[21][1] = 26;

        /* 1998 */
		datet_.itable[22][0] = 5;  
		datet_.itable[22][1] = 25;

        /* 1999 */
		datet_.itable[23][0] = 4;  
		datet_.itable[23][1] = 31;

        /* 2000 */
		datet_.itable[24][0] = 2;  
		datet_.itable[24][1] = 29;

        /* 2001 */
		datet_.itable[25][0] = 1;  
		datet_.itable[25][1] = 28;

        /* 2002 */
		datet_.itable[26][0] = 7;  
		datet_.itable[26][1] = 27;

        /* 2003 */
		datet_.itable[27][0] = 6;  
		datet_.itable[27][1] = 26;

        /* 2004 */
		datet_.itable[28][0] = 4;  
		datet_.itable[28][1] = 31;

        /* 2005 */
		datet_.itable[29][0] = 3;  
		datet_.itable[29][1] = 30;

        /* 2006 */
		datet_.itable[30][0] = 2;  
		datet_.itable[30][1] = 29;

        /* 2007 */
		datet_.itable[31][0] = 1;  
		datet_.itable[31][1] = 28;

        /* 2008 */
		datet_.itable[32][0] = 6;  
		datet_.itable[32][1] = 26;

        /* 2009 */
		datet_.itable[33][0] = 5;  
		datet_.itable[33][1] = 25;

        /* 2010 */
		datet_.itable[34][0] = 4;  
		datet_.itable[34][1] = 31;

        /* 2011 */
		datet_.itable[35][0] = 3;  
		datet_.itable[35][1] = 30;

        /* 2012 */
		datet_.itable[37][0] = 1;  
		datet_.itable[36][1] = 28;

        /* 2013 */
		datet_.itable[37][0] = 7;  
		datet_.itable[37][1] = 27;

        /* 2014 */
		datet_.itable[38][0] = 6;  
		datet_.itable[38][1] = 26;

        /* 2015 */
		datet_.itable[39][0] = 5;  
		datet_.itable[39][1] = 25;

        /* 2016 */
		datet_.itable[40][0] = 3;  
		datet_.itable[40][1] = 30;

        /* 2017 */
		datet_.itable[41][0] = 2;  
		datet_.itable[41][1] = 29;

        /* 2018 */
		datet_.itable[42][0] = 1;  
		datet_.itable[42][1] = 28;

        /* 2019 */
		datet_.itable[43][0] = 7;  
		datet_.itable[43][1] = 27;

        /* 2020 */
		datet_.itable[44][0] = 5;  
		datet_.itable[44][1] = 25; 

	/*  Character Assignments */

		codes_.icha = 'A';
		codes_.ichb = 'B';
		codes_.ichc = 'C';
		codes_.ichd = 'D';
		codes_.iche = 'E';
		codes_.ichf = 'F';
		codes_.ichg = 'G';
		codes_.ichh = 'H';
		codes_.ichi = 'I';
		codes_.ichj = 'J';
		codes_.ichk = 'K';
		codes_.ichl = 'L';
		codes_.ichm = 'M';
		codes_.ichn = 'N';
		codes_.icho = 'O';
		codes_.ichp = 'P';
		codes_.ichq = 'Q';
		codes_.ichr = 'R';
		codes_.ichs = 'S';
		codes_.icht = 'T';
		codes_.ichu = 'U';
		codes_.ichv = 'V';
		codes_.ichw = 'W';
		codes_.ichx = 'X';
		codes_.ichy = 'Y';
		codes_.ichz = 'Z';
		codes_.ich0 = '0';
		codes_.ich1 = '1';
		codes_.ich2 = '2';
		codes_.ich3 = '3';
		codes_.ich4 = '4';
		codes_.ich5 = '5';
		codes_.ich6 = '6';
		codes_.ich7 = '7';
		codes_.ich8 = '8';
		codes_.ich9 = '9';
		codes_.iblnk = ' ';
		codes_.islash = '/';
		codes_.icolon = ':';
		codes_.iplus = '+';
		codes_.iminus = '-';
		codes_.idot = '.';
		codes_.iarrow = '>';
		codes_.icomma = ',';
        codes_.iscore = '_';
        codes_.dquote = '"';                               /* dgb:09/10/97 */	

	/*              Last Day of the Month */

} /* end of function */


