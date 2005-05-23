#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/sendmx.h>
#include <sys/kernel.h>
#include "shef_structs_external.h"

/*---------------------------------------------------------------------
  
       FUNCTION shefpass
  
       PURPOSE  
         To pass data to the databuffer task in HYDROMET (QNX 4.x)
  
       VERSION and UPDATES
         1.0    SEP 95  David G. Brandon
                Original Version
         1.1    MAY 30 96 DGB
                Move part of code which finds the task id (tid)
                of the databuffer to the main driver.  The tid
                is then only found once.
         1.2    SEP 22 96 DGB
                Add num_records counter for stats_utilities.
         1.3    DEC 3 96 DGB
                For domsat messages... if quality flag is D 
                switch it to S.      
         1.4    OCT 19 97 DGB
                Insert an error message if message is not passed
                to databuffer...only printed in verbose mode.
         1.5    SEP 12 98 DGB
                Remove database.h header file and replace
                the msgbuf struct in the code.  This was the
                only piece of code used in the database.h file.
  --------------------------------------------------------------------- */




/*  Define Externals */

extern int STATS;                                          /* dgb:09/22/96 */
extern int tid;                                            /* dgb:05/30/96 */
extern int num_records;                                    /* dgb:09/22/96 */
#define  DATA_IN       0x02
#define  NOT_USED		0xFFFFFFFF
#define  HB5_ID_LEN      17
#define  PC_LEN          11
#define  NAME_LEN        51

struct msgbuf {
	unsigned char       action;
	unsigned long int   sta_id;
	unsigned char       hb5_id[HB5_ID_LEN];
	unsigned char       pc[PC_LEN];
	float               report;
	time_t              clock;
	unsigned char       dqr;
	unsigned char       qds;
	unsigned char       qpi;
	signed short int    node;
	signed short int    tty_num;
	unsigned char       gage_type;
};


void shefpass(
               int    idstn0,    /* station id char0   */
               int    idstn1,    /*            char1   */
               int    idstn2,    /*            char2   */
               int    idstn3,    /*            char3   */
               int    idstn4,    /*            char4   */
               int    idstn5,    /*            char5   */
               int    idstn6,    /*            char6   */
               int    idstn7,    /*            char7   */
               int    nyear,     /* observation year   */
               int    nmon,      /* observation month  */
               int    nday,      /* observation day    */
               int    nhour,     /* observation hour   */
               int    nmin,      /* observation minute */
               int    nsec,      /* observation second */
               int    cyear,     /* creation    year   */
               int    cmon,      /* creation    month  */
               int    cday,      /* creation    day    */
               int    chour,     /* creation    hour   */
               int    cmin,      /* creation    minute */
               int    csec,      /* creation    second */
               int    kodp,      /* P of PEDTSEP       */
               int    kode,      /* E of PEDTSEP       */
               int    kodd,      /* D of PEDTSEP       */
               int    kodt,      /* T of PEDTSEP       */
               int    kods,      /* S of PEDTSEP       */
               int    kodex,     /* E of PEDTSEP       */
               int    kodpr,     /* P of PEDTSEP       */
               int    idur,      /* 4 digit duration   */
               double codp,      /* probability value  */
               double value,     /* data value         */
               int    lwal,      /* qualifier          */
               int    irev,      /* revision flag      */
               int    source0,   /* msg source char0   */
               int    source1,   /* msg source char1   */
               int    source2,   /* msg source char2   */
               int    source3,   /* msg source char3   */
               int    source4,   /* msg source char4   */
               int    source5,   /* msg source char5   */
               int    source6,   /* msg source char6   */
               int    source7,   /* msg source char7   */
               int    idote      /* dot e indicator    */
             )
{

int  fd, i, ntime;

long tinsec;

char reply_msg;

struct msgbuf msg;
struct _mxfer_entry smsg, rmsg;

   stats_.parse_stats[1]++;
   num_records++;

   /* construct id */
      msg.hb5_id[0] = idstn0;
      msg.hb5_id[1] = idstn1;
      msg.hb5_id[2] = idstn2;
      msg.hb5_id[3] = idstn3;
      msg.hb5_id[4] = idstn4;
      msg.hb5_id[5] = idstn5;
      msg.hb5_id[6] = idstn6;
      msg.hb5_id[7] = idstn7;
      for ( i=8; i < HB5_ID_LEN; i++)
            msg.hb5_id[i] = 0;
     
      
      for ( i = 0; i < 8; i ++ )
      {
          if ( msg.hb5_id[i] == ' ' )
               msg.hb5_id[i] = 0;
      }

   /* construct PEDTSEP */
      msg.pc[0] = kodp;
      msg.pc[1] = kode;
      msg.pc[2] = kodd;
      msg.pc[3] = kodt;
      msg.pc[4] = kods;
      msg.pc[5] = kodex;
      msg.pc[6] = kodpr;
      msg.pc[7] = 0;
      msg.pc[8] = 0;
      
   /* if irev is on 
         set 8th bit for revision flag
      else
         leave alone
   */

   /* for domsat only...if quality flag is D switch to S */
      if ( lwal == 68 ) lwal = 83;                         /* dgb:12/04/96 */

      if ( irev == 1 )
           lwal = lwal | 128;

   /* construct the time */
      tinsec      = ouptime( nyear, nmon, nday, nhour, nmin, nsec);
      ntime = ( nhour * 100 ) + nmin; 


   /* stuff remaining values into msg structure for passing to databuffer */

      msg.action    = DATA_IN;
      msg.sta_id    = NOT_USED;
      msg.report    = ( float ) value;
      msg.clock     = tinsec;
      msg.dqr       = lwal;
      msg.qds       = 0;
      msg.qpi       = 0;
      msg.tty_num   = 0;
      msg.node      = 0;
      msg.gage_type = 0;
      


   /* format the message for a Sendmx */
	  _setmx(&smsg, &msg, sizeof(struct msgbuf));
	  _setmx(&rmsg, &reply_msg, sizeof(unsigned char));

   /* send message to databuffer task */

            i = Sendmx(tid, 1, 1, &smsg, &rmsg);

            if( i == 0 )
            {
               if ( VERBOSE )
               {
                  fprintf(stdout,"shef_decode: %s%s ",msg.hb5_id,msg.pc);
                  fprintf(stdout," %10.4f %2d/%2d/%4d %04d sent to databuffer\n",
                          msg.report,nmon,nday,nyear,ntime);                                                                                                                                                                                                                                                                                                                                                                                                                   
               }
            }
            else                                           /* dgb:10/19/87 */
            {
               if ( VERBOSE )
               {
                  fprintf(stdout,"***ERROR***:   %s%s ",msg.hb5_id,msg.pc);
                  fprintf(stdout," %10.4f %2d/%2d/%4d %04d NOT sent\n",
                          msg.report,nmon,nday,nyear,ntime);                                                                                                                                                                                                                                                                                                                                                                                                                   
               }
            }

   if ( STATS )                                            /* dgb:09/22/96 */
       stats_updates(kodp,kode,kodd,kodt,kods); 

}
