/*************************************************************************************************************
 *  Function:  retrieve and parse data from SCADA via a serial connection between SUN and SCADA
 *
 *  Author:    Brad Vickers
 *
 *  Date:      December 1, 1992, revised January 31, 1994, revised for Solaris 2.3 September 1994
 *
 *  Purpose:   1. recieve and parse reservoir parameters from the Colorado River SCADA system
 *             2. submit daily data to hydromet for processing daily report
 *             3. submit hourly data for Glen Canyon to Flagstaff Office
 *             4. submit daily data to the River Forcast Center
 *
 **************************************************************************************************************/



#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stropts.h>
#include <poll.h>
#include <errno.h>
#include <sys/termios.h>
#include <ctype.h>

#define SIZE 40
#define HYDRO "hydromet_"
#define CRSP "crsp_"
#define GLEN  "glen_"



main()
{
	char   buf[132], buffer[BUFSIZ], resname1[14], resname2[14], resname3[14], resname4[14];
        char   resname5[14], resname6[14], date[30],dum[41],dum1[41],dum2[41],dum3[41],dum4[41],dum5[41];
        char   dum6[41], output[40], outputt[40], outputtt[40], DATE[30]; 
        char   *orgin = HYDRO;
        char   *orgin1 = CRSP;
        char   *orgin2 = GLEN;
        char   hydro[SIZE], crsp[SIZE], glen[SIZE];
        char   mon[3];
        char   day[3];
        char    yr[3];


        struct pollfd pollfds[1];
	struct strbuf *dataptr;
        struct termios *arg;

	int    fid, i, j, m, k, num_res, num_row, num_col, str_cursor, cur_baud, cur_size, cur_cread, cur_odd, cur_cr;
        int    cur_hupcl, cur_clocal, cur_crtscts, cur_parenb, cur_cstopb, cur_cibaud, cur_echo;
        int    hour[24][6];

	float  elev[24][6], tail[24][6], head[24][6], gen[24][6], pow_rel[24][6], spill[24][6], bypass[24][6], riv[24][6];
        float  telev[6],ttail[6],thead[6],tgen[6],tpow_rel[6],tspill[6],tbypass[6],triv[6];
        float  aelev[6],atail[6],ahead[6],agen[6],apow_rel[6],aspill[6],abypass[6],ariv[6];
        float  pow_relaf[6],spillaf[6],bypassaf[6],rivaf[6],bypassaf1[6];

	FILE   *fd, *out, *outt, *outtt, *out1, *out1_alpha, *out2, *out3, *out4, *out5;

	if ((fid = open("/dev/ttyb", O_RDONLY)) < 0)
	{
	   perror("open serial port /dev/ttyb for reading failed");
           exit (1);
     	}
        arg = (struct termios *)malloc(sizeof(struct termios));
        if (ioctl(fid, TCGETS, arg) > 0);
            {
            printf("\n\n\n%s\n\n", "ready to download SCADA data");
            }

        cur_baud = arg->c_cflag & CBAUD;
        cur_size = arg->c_cflag & CSIZE;
        cur_cstopb = arg->c_cflag & CSTOPB;
        cur_cread = arg->c_cflag & CREAD;
        cur_parenb = arg->c_cflag & PARENB;
        cur_odd = arg->c_cflag & PARODD;
        cur_hupcl = arg->c_cflag & HUPCL;
        cur_clocal = arg->c_cflag & CLOCAL;
        cur_cibaud = CBAUD << IBSHIFT;
        cur_crtscts = arg->c_cflag & CRTSCTS; 

	printf("curbaud= %i\n", cur_baud);
	printf("cursize= %i\n", cur_size);
	printf("curcstopb= %i\n", cur_cstopb);
	printf("curcread= %i\n", cur_cread);
	printf("curparenb= %i\n", cur_parenb);
	printf("curodd= %i\n", cur_odd);
	printf("curhubcl= %i\n", cur_hupcl);
	printf("curclocal= %i\n", cur_clocal);
	printf("curcibaud= %i\n", cur_cibaud);
	printf("curcrtscts= %i\n", cur_crtscts); 

        arg->c_cflag = arg->c_cflag - cur_baud + B2400;
        arg->c_cflag = arg->c_cflag - cur_size + CS7;
        arg->c_cflag = arg->c_cflag & ~CSTOPB;
        arg->c_cflag = arg->c_cflag | CREAD;
        arg->c_cflag = arg->c_cflag | PARENB;
        arg->c_cflag = arg->c_cflag | PARODD;
        arg->c_cflag = arg->c_cflag & ~HUPCL;
        arg->c_cflag = arg->c_cflag | CLOCAL;
        arg->c_cflag = arg->c_cflag & ~CRTSCTS;
        arg->c_lflag = arg->c_lflag & ~ECHO;
        arg->c_iflag = arg->c_iflag & ~ICRNL;
        arg->c_iflag = arg->c_iflag | IGNCR; 

        cur_baud = arg->c_cflag & CBAUD;
        cur_size = arg->c_cflag & CSIZE;
        cur_cstopb = arg->c_cflag & CSTOPB;
        cur_cread = arg->c_cflag & CREAD;
        cur_parenb = arg->c_cflag & PARENB;
        cur_odd = arg->c_cflag & PARODD;
        cur_hupcl = arg->c_cflag & HUPCL;
        cur_clocal = arg->c_cflag & CLOCAL;
        cur_cibaud = CBAUD << IBSHIFT;
        cur_crtscts = arg->c_cflag & CRTSCTS; 

	printf("curbaud= %i\n", cur_baud);
	printf("cursize= %i\n", cur_size);
	printf("curcstopb= %i\n", cur_cstopb);
	printf("curcread= %i\n", cur_cread);
	printf("curparenb= %i\n", cur_parenb);
	printf("curodd= %i\n", cur_odd);
	printf("curhubcl= %i\n", cur_hupcl);
	printf("curclocal= %i\n", cur_clocal);
	printf("curcibaud= %i\n", cur_cibaud);
	printf("curcrtscts= %i\n", cur_crtscts); 


        if (ioctl(fid, TCSETS, arg) > 0);
            {
            printf("%s\n", "");
            }

        cur_echo = arg->c_lflag & ECHO;
        cur_cr = arg->c_iflag & IGNCR;

        num_res = 1;
        num_col =1;

        j = 0;

        while (num_res  <= 6)
         {
            num_row = 1;
            while (num_row <=48)
            {
                num_row = num_row + 1;
                 read(fid, buf, 132);
		printf("%s\n",buf);
	      } /* end of read while */
             num_res = num_res + 1;

	  } /* end of first while */


} /* end of main */


