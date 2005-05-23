/*    
       AUG  8 96 DGB
                Add cfg to tempfiles structure.
       SEP 22 96 DGB
                Add stats_.ts_post_time array to t_stats_
                Resize NUM_ arrays for inputparm
       NOV 7 96 DGB
                Include shef.h
       SEP 10 97 DGB
                Include MAX_SHEF_INPUT
                Add codes_.dquote
                Add t_comment structure
                Add error/warning messages numbers 39, 40
                to err8_ structure.
       OCT 19 97 DGB
                Change message 22...increase to 82 chars.
                Indicate that if year is out of bounds for
                1976-2020, and a local time zone is used,
                i.e., other than Z, a conversion of daylight/
                standard time cannot be made.
       DEC  01 97 DGB
                Include test_.atest_flag for advanced test option.
       DEC 21 97 DGB
                Add centflag and dccentflag in struct data.   
                This flag is set to on if an explicit century is 
                given in the positional or DC data.
       JAN  5 98 DGB
                Add database to tempfiles for compatibility with
                HP posting.
       JAN  8 98 DGB
                Add century flag.  This can explicityly set the
                system time to a century.  Used for testing.
       JAN 17 98 DGB
                Add yearflag flag.  Set to on ( i.e. 1) if year
                is not explicitly given in the positional data
                and a year has to be assumed. 

       SEP 17 98 DGB
                Add dotend, and dotbegin;
       NOV 14 98 DGB
                Add temp2 and temp2 to tempfiles structure...used
                for dot 3 preprocesing.
                Add DOTEPRE, INFORMIX_ERR, SENSORS, PARTIAL_ERROR,
                DEBUG AND VERBOSE.  Add *fp_postlog, *sqlerrors,
                *sensors, *testfile, *pname and *homename.
       JAN 18 99 DGB
                Add century_string to test_ structure.
                Add override variables to data_ structure.
       JAN 28 00 DGB
                Add TURNONR for use with the -q switch;

*/

/* The following 6 NUM_ values size the arrays for inputparm */

#include "shef.h"

extern struct t_pointers_ {
    FILE *lchn, *jchn, *kchn, *mchn, *icher, *nchn;
    }  fp_; 
extern struct t_cont_ {
	char out_flag[13], error_flag[13], post_flag[13];
	}	cont_;
extern struct t_codes_ {
	short int icha, ichb, ichc, ichd, iche, ichf, ichg, ichh, ichi, 
	 ichj, ichk, ichl, ichm, ichn, icho, ichp, ichq, ichr, ichs, icht, 
	 ichu, ichv, ichw, ichx, ichy, ichz, ich0, ich1, ich2, ich3, ich4, 
	 ich5, ich6, ich7, ich8, ich9, iblnk, islash, icolon, iplus, iminus, 
	 idot, iarrow, icomma, iscore, dquote;
	}	codes_;                                            /* dgb:09/10/97 */
extern struct t_parms1_ {
	char peval[NUM_PEVAL][3], durval[NUM_DURVAL][3], tsval[NUM_TSVAL][3], 
         exval[NUM_EXVAL][3], proval[NUM_PROVAL][3], senval[NUM_SENVAL][3];
	}	parms1_;
extern struct t_parms2_ {
	short int durvalue[NUM_DURVAL], tsvalue[NUM_TSVAL], exvalue[NUM_EXVAL], 
              sendflag[NUM_SENVAL], numval[7]; 
    char sendcodes[NUM_SENVAL][7];
	}	parms2_;
extern struct t_parms3_ {
	double pevalue[NUM_PEVAL], provalue[NUM_PROVAL];
	}	parms3_;
extern struct t_durprob_ {
    short int kodd, kodpr;
    }  durprob_;
extern struct t_datim_ {
	short int idate[6];
	}	datim_;
extern struct t_error_ {
	short int nerror, nwarn;
	}	error_;
extern struct t_files_ {
    char shef_decode_err[MAX_F], shef_in[MAX_F], shef_out[MAX_F],
         shef_log[MAX_F],shef_comments[MAX_F];                 /* dgb:01/02/99 */
    }  files_;
extern struct t_buffer_ {
	short int ibuf[MAX_SHEF_INPUT], ip, nblnk;             /* dgb:09/10/97 */
	}	buffer_;
extern struct t_xchar_ {
	short int ichar;
	}	xchar_;
extern struct t_sendflg_ {
	short int nsflag;
	}	sendflg_;
extern struct t_bbuf1_ {
	short int nmrec;
	}	bbuf1_;
extern struct t_bbuf2_ {
	double aar[38]; /* increase virutal buffer from 12 to 48 for .B line */
	float bar[38];
	short int iarray[38][27]; /* dgb change array to 27 */
	}	bbuf2_;
extern struct t_bbuf3_ {
	short int jarray[38][17];
	}	bbuf3_;
extern struct t_format_ {
	short int iform, lform, nerr;
	}	format_;
extern struct t_datrel_ {
	short int mcent, myear, mmon, mday, mhour, mmin, msec, mend;
	}	datrel_; /* dgb cent */
extern struct t_err1_ {
	char message1[37], message2[43], message3[62], message4[29], message5[63];
	}	err1_;
extern struct t_err2_ {
	char message6[57], message7[34], message8[38], message9[40], message10[32];
	}	err2_;
extern struct t_err3_ {
	char message11[45], message12[32], message13[47], message14[61], 
	 message15[51];
	}	err3_;
extern struct t_err4_ {
	char message16[44], message17[43], message18[48], message19[75], 
	 message20[66];
	}	err4_;
extern struct t_err5_ {
	char message21[63], message22[67], message221[58], message23[51], 
         message24[32], message25[38];
	}	err5_;
extern struct t_err6_ {
	char message26[29], message27[29], message28[64], message29[43], 
	 message30[45];
	}	err6_;
extern struct t_err7_ {
	char message31[41], message32[48], message33[34], message34[36], 
	 message35[79];
	}	err7_;
extern struct t_err8_ {
	char message36[80], message37[72], message371[53], message38[82],
         message39[77], message40[41];
	}	err8_;                                             /* dgb:09/10/97 */
extern struct t_dotbee_ {
	short int nelem, nerror;
	}	dotbee_;
extern struct t_dtype_ {
	short int ieee;
	}	dtype_; 
extern struct t_days_ {
	short int iday[12];
	}	days_;
extern struct t_datet_ {
	short int itable[45][2];
	}	datet_;
extern struct t_data_ {
	short int idstn[8], lcent, lyear, lmon, lday, lhour, lmin, lsec, 
        kcent, kyear, kmon, kday, khour, kmin, ksec, kodp, kode, icodd, 
        idcodd, kodt, kods, kodex;
	float codp;
	short int kwal, msource[8], itz, nadjtz, kodu;
	double value;
	short int kflag;
        short int centflag;                                    /* dgb:12/21/97 */
        short int dccentflag;                                  /* dgb:12/24/97 */
        short int yearflag;                                    /* dgb:01/18/98 */
        short int overcentury;                                 /* dgb:01/18/99 */
        short int overyear;                                    /* dgb:01/18/99 */
        short int overmonth;                                   /* dgb:01/18/99 */
        short int posovercentury;                              /* dgb:01/18/99 */
        short int posoveryear;                                 /* dgb:01/18/99 */ 
	}	data_;
extern struct t_stats_ {
    long time_current;
    long time_start_log;
    long time_stop_log;
    long time_start_prod;
    long time_stop_prod;
    int  post_stats[MAX_ACCEPTABLE_PE + 1][MAX_ACCEPTABLE_TS + 1];
    char ts_post_time[MAX_ACCEPTABLE_TS + 1][8];    /* dgb:09/22/96 */
    int  parse_stats[MAX_ACCEPTABLE_CATEGORIES +1];
    char acceptable_pe[MAX_ACCEPTABLE_PE+1][PE_SIZE];
    char acceptable_ts[MAX_ACCEPTABLE_TS+1][TS_SIZE];
    int  num_pe;
    int  num_ts;
    char dtg[40];
    int  flag;
    char product_name[30];
    } stats_;
extern struct s_test {
    int test_flag, atest_flag, century_flag;               /* dgb:01/02/98 */
    char century_string[13];                                 /* dgb:01/16/99 */
   } test_;
extern struct t_luns {
     short int mrec;
   } luns_;
extern struct t_dotee_ {
	short int inul, ncent, nyear, nmon, nday, nhour, nmin, nsec;
	}	dotee_;
extern struct t_tempfiles_ {
    char shef_decode_err[MAX_F],shef_in[MAX_F], shef_out[MAX_F], shef_log[MAX_F],
         err_file[MAX_F], dotbtemp[MAX_F], last_line_read[MAX_SHEF_INPUT+1],cfg[MAX_F],
         database[MAX_F], dote[MAX_F],temp1[MAX_SHEF_INPUT],temp2[MAX_SHEF_INPUT];
    }  tempfiles_;                                         /* dgb:01/05/98 */
extern struct t_comment {
     short int colon_on, dquote_on, pos;
     char comment[MAX_COMMENT_LENGTH];
     char token_name[MAX_F],table_name[MAX_F],id_name[MAX_F],
          token_value[MAX_F];                                /* dgb:01/02/00 */
   } comment_;                                               /* dgb:09/10/97 */

extern int dotend, dotbegin, econtinuation, efound;                 /* dgb:09/17/98 */
extern int DOTEPRE, INFORMIX_ERR, SENSORS, PARTIAL_ERROR, DEBUG, VERBOSE;
extern int num_records;
extern int FCFG;
extern int DATABASE_NAME;
extern int CHECKTABLES;
extern int HOW_OLD;
extern int NUM_EXECUTIONS;
extern int IREV;
extern int STATS;
extern int tid;
extern int TURNONR;	                                    /* dgb:01/28/00 */

extern FILE *fp_postlog, *sqlerrors, *sensors, *testfile;

extern char *pname, *homename;
extern DIR *dirp;
extern struct dirent *de;
extern char *directory;

