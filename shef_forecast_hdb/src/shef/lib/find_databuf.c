/****************************************************************************\
*  find_databuf                                                              *
******************************************************************************
*  Find_databuf() locates the task id of the DATABUFFER program. It returns  *
*  the task id of the DATABUFFER program if successful. If not successful,   *
*  it returns a value of zero.                                               *
*                                      Wayne Martin - CNRFC  06/27/90        *
*  SEP 11 98 DGB
   Remove database.h file and inlucde that small portion that 
   is needed in the file.
\****************************************************************************/

#include  <stdio.h>
#include  <sys/name.h>

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

signed int find_databuf(void)  {

	signed int tid;
	
	/* Get registered task id of DATABUFFER.  */
	tid = qnx_name_locate(0, "/DATABUFF", sizeof(struct msgbuf), NULL);

	return(tid);
}
