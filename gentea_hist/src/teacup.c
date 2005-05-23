#include "gd.h"
#include "gdfonts.h"
#include <string.h>
#include <math.h>

int foreground,background,foregroundtext;

char buf[100];

void teacup(gdImagePtr im,gdPoint base,int size,double maxfull,
            double currentfull,char *label,char *datadate)
{

   gdPoint corner[4];
   int percentfull,maxpercentfull,full;
   double area,p;

   corner[0].x=base.x;              /*lower left*/
   corner[0].y=base.y;
	
   corner[1].x=base.x+10*size;      /*lower right*/
   corner[1].y=base.y;
	
   corner[2].x=base.x+20*size;  /*upper right*/
   corner[2].y=base.y-20*size;
	
   corner[3].x=base.x-10*size;    /*upper left*/
   corner[3].y=base.y-20*size;

   gdImageFilledPolygon(im,corner,4,background);  /*clear background*/
   gdImagePolygon(im,corner,4,foreground);        /*draw polygon*/

   /* 	if(currentfull==998877.0) */
   /* 	   percentfull=0; */
   /* 	else */

   percentfull=(int)(currentfull*100/maxfull+0.5);
	
   if(percentfull>100)
      maxpercentfull=100;
   else
      maxpercentfull=percentfull;
  
   /*printf("%% full: %d\n",maxpercentfull);*/

   /*adjust for area of a trapazoid*/
   p=maxpercentfull;
   p/=100;

   area = 400*size*size+3200*size*size*p;
   area = sqrt(area) - 20*size;
   area /= 40*size;
     
   if(area > 1.000)
      area = 1.000;
   if(area < 0.000)
      area = 0.000;

   /*printf("%% Area full: %d\n",(int)(area*100));*/
   full = area*100;

   corner[2].x=base.x+10*size+size*full*10/100;  /*upper right*/
   corner[2].y=base.y-20*size*full/100;
	
   corner[3].x=base.x-10*size*full/100;    /*upper left*/
   corner[3].y=base.y-20*size*full/100;

   gdImageFilledPolygon(im,corner,4,foreground);     /*filled portion*/
   /*debugging*/
   printf("%lf %lf %d %d   %d %d   %d %d\n",maxfull,currentfull,percentfull,size,corner[2].x,corner[2].y,corner[3].x,corner[3].y);

/*   gdImageFilledRectangle(im,base.x-2,base.y+1,base.x+98,base.y+35,background); */ /*clear background for text    */

        
   gdImageString(im,gdFontSmall,base.x,base.y+3,(unsigned char *)label,foregroundtext);


   /* 	if(currentfull==998877.0) */
   /*            sprintf(buf,"MISSING/%ld",(long)maxfull); */
   /* 	else     */

   sprintf(buf,"%ld/%ld",(long)currentfull,(long)maxfull);
   gdImageString(im,gdFontSmall,base.x,base.y+13,(unsigned char *)buf,foregroundtext);

   /* 	if(currentfull==998877.0) */
   /*            sprintf(buf,"MISSING"); */
   /*         else */

   if(datadate) {
      sprintf(buf,"%d%% Full as of %s",percentfull,datadate);
      gdImageString(im,gdFontSmall,base.x,base.y+23,(unsigned char *)buf,foregroundtext);
   } else {
      sprintf(buf,"%d%% Full",percentfull);
   gdImageString(im,gdFontSmall,base.x,base.y+23,(unsigned char *)buf,foregroundtext);
   }

}

void lineprint(gdImagePtr im,gdPoint base,float value,char *label,char *units)
{
   gdImageFilledRectangle(im,base.x,base.y+3,base.x+86,base.y+14,background);    
   /*     if(value==998877.0) */
   /*        sprintf(buf,"%s MISSING",label); */
   /*     else */

   sprintf(buf,"%s %5.0f %s",label,value,units);
   gdImageString(im,gdFontSmall,base.x,base.y+2,
                 (unsigned char *)buf,foregroundtext);
}

void lineprint2(gdImagePtr im,gdPoint base,float value,char *label,char *units)
{
   gdImageFilledRectangle(im,base.x,base.y+3,base.x+86,base.y+14,background);    
   /*     if(value==998877.0) */
   /*        sprintf(buf,"%s MISSING",label); 
             else */

   if(strncmp(units,"CFS",3)==0)
      sprintf(buf,"%s %7.0f %s",label,value,units);
   else
      sprintf(buf,"%s %7.2f %s",label,value,units);

   gdImageString(im,gdFontSmall,base.x,base.y+2,
                 (unsigned char *)buf,foregroundtext);
}

