#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "gd.h"
#include "gdfonts.h"

struct datetime {
   int yr;
   int mn;
   int dy;
   int hr;
   int mi;
   int sc;
};

extern int foreground,background,foregroundtext;

extern char buf[100];

void main(int argc,char **argv) 
{
   	float readone(char *,char *,struct datetime *);
	void teacup(gdImagePtr,gdPoint,int,double,double,char *);
	void lineprint(gdImagePtr,gdPoint,float,char *,char *);
	void lineprint2(gdImagePtr,gdPoint,float,char *,char *);
	gdImagePtr im;	
	FILE *ofs,*ifs,*cfs;	
	int retval,row,col,size,red,white,blue,green,sofar,i,j;
	time_t t;
	char timestring[50],drawitem[30],description[100],line[100],units[30];
        float value,capacity;
        int station, pcode;
	gdPoint points[4],base;
    struct tm *timep;
    struct datetime dt;


	if(argc!=4)
	{
		printf("Usage: gentea infile outfile configfile\n");
		exit(0);
	}
	if((ifs=fopen(argv[1],"rb"))==NULL)
	{
		printf("Error opening input file: %s\n",argv[1]);
		exit(0);
	}
	if((ofs=fopen(argv[2],"wb"))==NULL)
	{
		printf("Error opening output file: %s\n",argv[2]);
		exit(0);
	}
	if((cfs=fopen(argv[3],"r"))==NULL)
	{
		printf("Error opening configuration file: %s\n",argv[3]);
		exit(0);
	}

	/*read in the image*/
	im = gdImageCreateFromPng(ifs);
	white = gdImageColorAllocate(im, 255, 255, 255);	
	blue = gdImageColorAllocate(im, 100,100, 255);		
	red = gdImageColorAllocate(im,255,0,0);
	foreground=blue;
	background=white;
	foregroundtext=red;


	/*get the current date & time and print on the image*/
	t=time((time_t)0);
	t-=86400;             /*subtract a day*/
	timep = localtime(&t);
	dt.yr = timep->tm_year;
    dt.yr+=1900;              /*local time subtracts 1900*/
    dt.mn = timep->tm_mon+1;
    dt.dy = timep->tm_mday;
    dt.hr=0;dt.mi=0;dt.sc=0;

	sprintf(timestring,"%02d/%02d/%4d",dt.mn,dt.dy,dt.yr);
    gdImageFilledRectangle(im,0,0,100,14,background);    
    gdImageString(im,gdFontSmall,0,0,(unsigned char *)timestring,foreground);
	
	
	while(fgets(line,100,cfs)!=NULL)
	{
	
		retval=sscanf(line,"%s %d %d %d %d %d %f %n",drawitem,&row,&col,&station,
		                                     &pcode,&size,&capacity,&sofar);
		
		for(i=sofar,j=0;i<100 && line[i]!='\n';i++,j++)
			description[j]=line[i];
		description[j]='\0';

	
		if(retval==7 && (drawitem[0]=='t' || drawitem[0]=='T'))
		{
			base.x=row;
			base.y=col;
			value = readone(station,pcode,&dt);
			teacup(im,base,size,capacity,value,description);
		}

		else if(retval==5 && (drawitem[0]=='c' || drawitem[0]=='C'))
		{

			base.x=row;
			base.y=col;
			value = readone(station,pcode,&dt);
                        char str_site_id[20];
                        sprintf(str_site_id,"%d",station);
		    lineprint(im,base,value,str_site_id,"cfs");
		}
		else
		{
                        printf("Not Here\n");

                   retval=sscanf(line,"%s %d %d %d %d %s %n",
                                 drawitem,&row,&col,&station,
                                 &pcode,units,&sofar);
		
			for(i=sofar,j=0;i<100 && line[i]!='\n';i++,j++)
				description[j]=line[i];
			description[j]='\0';

                        
			if(retval==6 && (drawitem[0]=='l' || drawitem[0]=='L'))
			{
				base.x=row;
				base.y=col;
				value = readone(station,pcode,&dt);
                                printf("%f\n",value);
				lineprint2(im,base,value,description,units);
			}
			else
				printf("Error interpreting draw request: %d\n",retval);
		}
	}	



	/* Output the image to the disk file. */	

	gdImagePng(im, ofs);	
	fclose(ifs);
	fclose(ofs);
	fclose(cfs);
        
        gdImageDestroy(im);

}
