#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "gd.h"
#include "gdfonts.h"
#include "gdfontmb.h"
#include "dbutils.h"
#include "utils.h"

struct datetime {
   int yr;
   int mn;
   int dy;
   int hr;
   int mi;
   int sc;
};

/* colors used in teacup.c */
extern int foreground,background,foregroundtext;

/* defined in readdata.pc (embedded sql in C) */
float readone(int,int,char*);

/* defined in teacup.c */
void teacup(gdImagePtr,gdPoint,int,double,double,char *,char*);
void lineprint(gdImagePtr,gdPoint,float,char *,char *);
void lineprint2(gdImagePtr,gdPoint,float,char *,char *);

int main(int argc,char **argv) 
{
   gdImagePtr im;	
   FILE *ofs,*ifs,*cfs;	
   int retval,row,col,size,red,white,blue,nblue,green,sofar,i,j;
   time_t t;
   char datelabel[50],drawitem[30],description[100],line[100],units[30];
   char label[25];
   char datestr[25];
   char datadate[25];
   float value,capacity;
   int site, datatype;
   gdPoint base;
   struct tm *timep;
   struct datetime dt;

   if(argc!=4)
   {
      printf("Usage: gentea_hist infile outfile configfile\n");
      exit(1);
   }
   if((ifs=fopen(argv[1],"rb"))==NULL)
   {
      printf("Error opening input file: %s\n",argv[1]);
      exit(1);
   }
   if((ofs=fopen(argv[2],"wb"))==NULL)
   {
      printf("Error opening output file: %s\n",argv[2]);
      exit(1);
   }
   if((cfs=fopen(argv[3],"r"))==NULL)
   {
      printf("Error opening configuration file: %s\n",argv[3]);
      exit(1);
   }

   /*read in the image*/
   im = gdImageCreateFromPng(ifs);
   white = gdImageColorAllocate(im, 255, 255, 255);	
   blue = gdImageColorAllocate(im, 100,100, 255);		
   nblue = gdImageColorAllocate(im, 0,0, 140);		
   red = gdImageColorAllocate(im,255,0,0);
   green = gdImageColorAllocate(im,0,220,0);
   foreground=blue;
   background=white;
   foregroundtext=nblue;

   /* connect to the database */
   char *dbName;
   int result;
  
   dbName = NULL; 
   dbName = getenv("HDB_LOCAL");

   if (dbName == NULL)
   {
      PrintError("Environment variable HDB_LOCAL not set...\n");
      exit(ERROR);
   }
  
  
   if  ((result = SqlConnect (dbName, "app_user", dbName)) == ERROR)
      exit (ERROR);

   /*get the current date & time and print on the image*/
   t=time((time_t)0);
   t-=86400;             /*subtract a day*/
   timep = localtime(&t);
   dt.yr = timep->tm_year;
   dt.yr+=1900;              /*local time subtracts 1900*/
   dt.mn = timep->tm_mon+1;
   dt.dy = timep->tm_mday;
   dt.hr=0;dt.mi=0;dt.sc=0;

   sprintf(datestr,"%02d/%02d",dt.mn,dt.dy);
   sprintf(label,"Data Current as of:");
   sprintf(datelabel,"%s/%04d",datestr,dt.yr);

   gdImageFilledRectangle(im,0,0,100,14,background);    
   gdImageString(im,gdFontMediumBold,0,0,(unsigned char *)label,foreground);
   
   gdImageFilledRectangle(im,0,14,100,14,background);    
   gdImageString(im,gdFontMediumBold,0,14,(unsigned char *)datelabel,foreground);
	

   /* read the config file, get the data, put it on the image per line */
   while(fgets(line,100,cfs)!=NULL)
   {

      retval=sscanf(line,"%30s %d %d %d %d %d %f %n",drawitem,&row,&col,&site,
                    &datatype,&size,&capacity,&sofar);

      for(i=sofar,j=0;i<100 && line[i]!='\n';i++,j++)
         description[j]=line[i];
      description[j]='\0';

      if(retval==7 && (drawitem[0]=='t' || drawitem[0]=='T'))
      {
         base.x=row;
         base.y=col;
         value = readone(site,datatype,datadate);
         if (strncmp(datestr,datadate,25) !=0) {
            printf ("'%s' '%s'\n",datestr,datadate);
            
            foregroundtext=red;
            teacup(im,base,size,capacity,value,description,datadate);
            foregroundtext=nblue;
         }
         else {
            teacup(im,base,size,capacity,value,description,0);
         }
      }

      else
      {
         retval=sscanf(line,"%30s %d %d %d %d %30s %n",
                       drawitem,&row,&col,&site,
                       &datatype,units,&sofar);
		
         for(i=sofar,j=0;i<100 && line[i]!='\n';i++,j++)
            description[j]=line[i];
         description[j]='\0';

                        
         if(retval==6 && (drawitem[0]=='l' || drawitem[0]=='L'))
         {
            base.x=row;
            base.y=col;
            value = readone(site,datatype,datadate);
            lineprint2(im,base,value,description,units);
         }
         else
            printf("Error interpreting draw request: %d\n",retval);
      }
   }

   if ((result = SqlDisconnect ()) == ERROR)
      exit(ERROR);


   /* Output the image to the disk file. */	

   gdImagePng(im, ofs);	
   fclose(ifs);
   fclose(ofs);
   fclose(cfs);
        
   gdImageDestroy(im);

   exit(0);
}
