#include <rms>
#include <stdio.h>
#include <stdlib>
#include <starlet>
#include <string>
#include <ctype>
#include <time.h>

#define ARCHIVE_SIZE 1500
#define PCODES_PER_YEAR 366
#define MAX_PCODES 30
#define PCODE_SIZE 10

struct datetime {
	int yr;
	int mn;
	int dy;
	int hr;
	int mi;
	int sc;
};

/*
void main(void)
{
	float readone(char *,char *,struct datetime *);
    double date_to_jd(struct datetime *);
 	char station[13]="BIGI",pcode[10]="QD";
	float x;
    struct datetime dt;
	time_t tn;
	struct tm *timep;

    time(&tn);                     //get current date
	timep = localtime(&tn);	
	dt.yr = timep->tm_year;
	if(dt.yr>70)
		dt.yr+=1900;
	else
		dt.yr+=2000;
	dt.mn = timep->tm_mon+1;
	dt.dy = timep->tm_mday;
	dt.hr = 0;dt.mi = 0;dt.sc = 0;

	x=readone(station,pcode,&dt);

	printf("%f\n",x);
}
*/

float readone(char *station,char *pcode,struct datetime *dt)
{
	void errorexit(char *);
	int openfab(struct FAB *,char *,char *);
	int connectrab(struct FAB *,struct RAB *,char,char *,int);
	int closefab(struct FAB);
    double date_to_jd(struct datetime *);
	int iread(struct RAB *,char *,int);
	struct FAB infab;
	struct RAB inrab;
	char record[ARCHIVE_SIZE];
	int retval,i,numpcodes,j,y,skip,keysize=27;
	char
filename[100],fileprefix[3]="WY",filedir[255]="dra1:[dms_v4.slc.archives].acf";
	char year[5];
	struct datetime wy;
	char valchar[4];
	float *val;
	double today,startwateryear;
	val=(float *)&valchar[0];

	today = date_to_jd(dt);	

	wy.yr=dt->yr;                             //water year starts in october
	if(dt->mn<10)
		wy.yr-=1;
	wy.mn = 10;
	wy.dy=0;                           // 0 for slc 2 for boise
	wy.hr=0;wy.mi=0;wy.sc=0;
	startwateryear = date_to_jd(&wy);

	skip = today-1-startwateryear;          // julean day for yesterday

	y=dt->yr;
	if(dt->mn>=10)
		y+=1;

	strcpy(filename,fileprefix);
	sprintf(year,"%04d",y);
	strncat(filename,year,4);

	if((retval=openfab(&infab,&filename[0],&filedir[0]))!=0)
		errorexit("Error: file access opening fab\n");

	if((retval=connectrab(&infab,&inrab,'r',&record[0],ARCHIVE_SIZE))!=0)
		errorexit("Error: file access connect rab\n");

	
	record[0] =0x40;
	record[1] =0x00;
	for(i=0;i<12&&station[i]!='\0';i++)
		record[2+i]=station[i];
	for(j=i;j<12;j++)
		record[2+j]=' ';
	for(i=0;i<9&&pcode[i]!='\0';i++)
		record[14+i]=pcode[i];
	for(j=i;j<9;j++)
		record[14+j]=' ';
	record[23]='0';
	record[24]='0';
	record[25]='0';
	record[26]='0';

	if((retval=iread(&inrab,record,keysize))<0)
		errorexit("No record found\n");
	
	for(i=0;i<4;i++)
		valchar[i]=record[32+skip*4+i];

	closefab(infab);

	return(*val);
}

void errorexit(char *msg)
{	
	fprintf(stdout,"%s",msg);
	exit(EXIT_FAILURE);
}


int openfab(struct FAB *xfab,char *filename,char *filedir)
{
	long status;

	*xfab=cc$rms_fab;
	xfab->fab$b_fac = FAB$M_GET;
	xfab->fab$b_shr = FAB$M_SHRGET|FAB$M_SHRPUT|FAB$M_SHRUPD|FAB$M_SHRDEL;
	xfab->fab$l_dna = (char *)filedir;
	xfab->fab$b_dns = strlen(filedir);
	xfab->fab$l_fna = (char *)filename;
	xfab->fab$b_fns = strlen(filename);
	
	status = sys$open(xfab);
	if(!(status&1))
		return(-1);
	return(0);            //Good
}

int connectrab(struct FAB *xfab,struct RAB *xrab,char accessmode,
			   char *record,int recordsize)
{
	long status;

	*xrab=cc$rms_rab;
	xrab->rab$l_fab = xfab;

	if(accessmode=='r' || accessmode=='R')
		xrab->rab$b_rac = RAB$C_KEY;       //Random access
	else if(accessmode=='s' || accessmode=='S')
		xrab->rab$b_rac = RAB$C_SEQ;       //Sequential access
	else
		return(-1);     

	xrab->rab$l_kbf = record;
	xrab->rab$l_ubf = record;
	xrab->rab$w_usz = recordsize;

	status = sys$connect(xrab);
	if(!(status&1))
		return(-2);      

	return(0);                          //Good
}

int iread(struct RAB *xrab,char *record,int keysize)
{
	int rsize;
	long status;

	xrab->rab$b_rac = RAB$C_KEY;    //switch to random access
	xrab->rab$b_ksz = keysize;      //keysize affects search results

	status = sys$get(xrab);           //RMS get record
	if(!(status&1))
		return(-1);
	
	rsize=xrab->rab$w_rsz;          //record size returned from RMS
	
	return(rsize);
}

int closefab(struct FAB xfab)
{
	void errorexit(char *);
	long status;
	
	status=sys$close(&xfab);

	if(!(status&1))
		errorexit("Error closing fab\n");

	return(0);                  //Good
}


/* converts a calender date into a Julian day */
double date_to_jd(struct datetime *dt)
{
	double jd;
	long c,ya,j,y,m,d;
	
	y=dt->yr;
	m=dt->mn;
	d=dt->dy;

	if(m > 2)
		m-=3;
	else
	{
		m+=9;
		y--;
	}

	c = y / 100L;
	
	ya = y - (100L * c);
	
	j = (146097L * c) / 4L + (1461L * ya) / 4L + (153L * m + 2L) / 5L + d + 1721119L;

	jd = j - 0.5; /* day starts at noon */
	jd += (double)dt->hr/24+(double)dt->mi/24/60+(double)dt->sc/24/60/60;
 
	return(jd);
}       

