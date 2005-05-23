#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
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

return;
}
