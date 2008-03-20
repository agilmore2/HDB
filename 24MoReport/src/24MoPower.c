#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <typedefs.h>
#include <defines.h>

#include <dbutils.h>
#include <utils.h>
#include <lc_apps.h>
#include "24MoReport.h"


int PowerReport (int model_run)
{
time_t  clock;
char  TimeString[TIMELENGTH];

double Mead, Moh, Hav, total=0, wytotal[MAX_NO_COLS][40];
int  begmo, col, year, index, yow;
int  success, m, i, j, k, tmp=1000;
int h2ocol[N_OF_POWER];
double  energy[N_OF_POWER][37],af[N_OF_POWER][37],data[15], temp[50];
double   cfs;
int  t, sdi, interval, mon, yr, prob;
ids  sdis[N_OF_POWER][MAX_NO_COLS];
char sysDate[25],begWY[50], newdate[50], the_date[50], begdate[50], moddate[50];
char buffer[12],runDate[25];
char my_string[300];
columndata h2opages[N_OF_POWER][MAX_NO_COLS];
uni_scale  units[N_OF_POWER][MAX_NO_COLS];
char *mo[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
char *preamble[12] = {"*","H","I","S","T","O","R","I","C","A","L","*"};

struct heading row[N_OF_POWER][5];

FILE *in_file, *out_file;

out_file = fopen(WATER,"a");
if(out_file == NULL)
{
  PrintError("Error opening file for writing\n");
  exit(ERROR);
}

in_file = fopen(POW_FILE,"r");

  h2ocol[HOOVER] = 10;
  h2ocol[DAVIS] = 10;
  h2ocol[PARKER] = 10;
  h2ocol[UBPOWER] = 6;

/*Get System Run Date */
if ((success = SqlGetSystemRunDate(model_run,sysDate)) !=OK)  {
  PrintError("Could not get system date\n");
  exit(ERROR); }


/* Get Model Run Date from model table */
if ((success = SqlGetModelRunDate(model_run,&mon, &yr,runDate,&prob)) !=OK)  {
  PrintError("Could not get model run date\n");
  exit(ERROR); }

sprintf(moddate,"%d/1/%d", mon, yr);

/* Start the loop to get the data */
for (j=0; j<N_OF_POWER; j++) {
for (i=0; i<h2ocol[j]; i++) {

   fgets (my_string,300,in_file);
   sscanf (my_string, "%d %d %d %d \n", &sdis[j][i].site_id, 
      &sdis[j][i].datatype_id, &units[j][i].scale, &units[j][i].unit);
   index = strspn(my_string,"0123456789 ");

   strcpy(h2opages[j][i].column_head,&my_string[index]);

/* Get site_datatype_ids */
if ((success = SqlGetSDI(sdis[j][i].datatype_id, sdis[j][i].site_id, &sdi))
  !=OK)  {
  PrintError("Could not get site datatype id from database\n");
    exit(ERROR);  }

sdis[j][i].site_datatype_id = sdi;

/* Fetch History data from table */
SqlGet24HisData(sdis[j][i].site_id, sdi, mon, yr, h2opages[j][i].values);

if (sdis[j][i].datatype_id==13){
  for (t=1; t<=12; ++t)
    h2opages[j][i].values[t]=0.0;}
if (sdis[j][i].site_datatype_id==1721)
   Mead = h2opages[j][i].values[12];
if (sdis[j][i].site_datatype_id==2086)
   Moh = h2opages[j][i].values[12];
if (sdis[j][i].site_datatype_id ==2087)
   Hav = h2opages[j][i].values[12];

/* Fetch model data from m_table */
SqlGet24ModData(sdis[j][i].site_id, model_run, sdi, mon, yr, h2opages[j][i].values);

/* Doing change in storage computation*/
if (sdis[j][i].datatype_id == 47 || sdis[j][i].datatype_id==1)  {
  for (t=13; t<37; ++t)  
   temp[t] = h2opages[j][i].values[t];
  for (t=13; t<37; t++)  {
   if (t==13 && sdis[j][i].site_id == 922) 
     h2opages[j][i].values[t] = temp[t] - Moh;
   else if (t==13 && sdis[j][i].site_id == 923)
     h2opages[j][i].values[t] = temp[t] - Hav;
   else if (t==13 && sdis[j][i].site_id == 921) 
     h2opages[j][i].values[t] = temp[t] - Mead;
   else
     h2opages[j][i].values[t] = temp[t] - 
 		temp[t-1];}}

/* Storing kwh and af for kwh/af computation */
if (sdis[j][i].datatype_id==74 || sdis[j][i].datatype_id==104)
  for (t=1; t<37; ++t)
    energy[j][t] = h2opages[j][i].values[t];
if (sdis[j][i].datatype_id==39)
  for (t=1; t<37; ++t)     
    af[j][t] = h2opages[j][i].values[t];

success = SqlFormatDate(moddate,moddate);

success = SqlDateMath(SUBTRACTION,moddate,begdate,1,"year");
if (success == ERROR) {
  PrintError("Could not get last years date\n");}

}   /* End column loop */
}   /* End site loop */


if((success = SqlExtractDatePart(begdate,"years", &yow)) != OK)
 {
  PrintError("Error in Extracting the Date\n");
 }
/* Get the time from operating system to print out on pages.*/
clock = time(NULL);
ctime_r(&clock,TimeString,TIMELENGTH);

/* Write data to file */

/* Print Column Headings*/ 

for (i=0; i<N_OF_POWER; i++){
for (j=0; j<h2ocol[i]; j++){
sscanf(h2opages[i][j].column_head,
	"%s %s %s %s ", row[i][1].column[j], row[i][2].column[j], 
                row[i][3].column[j], row[i][4].column[j]);}}
sprintf(buffer,"           ");

printf("Writing power pages\n\n");

for (i=0; i<N_OF_POWER-1; i++)
{
begmo = mon;
year = yow;
fprintf(out_file,"O P E R A T I O N   P L A N   F O R   C O L O R A D O   R I V E R \
  S Y S T E M   R E S E R V O I R S\n\n");
if (prob==50)
fprintf(out_file,"Bureau of Reclamation - CRFS %i/%i Most Prob Water Supply \
        %s\n",mon, yr, sysDate);
if (prob==10)
fprintf(out_file,"Bureau of Reclamation - CRFS %i/%i Prob Max Water Supply \
        %s\n",mon, yr, sysDate);
if (prob==90)
fprintf(out_file,"Bureau of Reclamation - CRFS %i/%i Prob Min Water Supply \
        %s\n",mon, yr, sysDate);

switch  ( i )
{
  case HOOVER:
     fprintf (out_file,"\t\t\t  Hoover Dam - Lake Mead\n\n");
     break;
  case DAVIS:
     fprintf (out_file,"\t\t\t  Davis Dam - Lake Mohave\n\n");
     break;
  case PARKER:
     fprintf (out_file,"\t\t\t  Parker Dam - Lake Havasu\n\n");
     break;
}

for (k=1; k<5; k++){
fprintf(out_file,"\t    ");
for (m=0; m<h2ocol[i]; ++m)
{
  if (m==9 && k>1)
     fprintf(out_file,"           ");
  else
	fprintf(out_file,"%10s ", row[i][k].column[m]);}
fprintf(out_file,"\n");}
fprintf(out_file,"\n");

strcpy(the_date, begdate);

/* Print out data now */
 for (t=1; t<37; ++t) {
if (mon==1 && t > 1)
  year = year + 1;
if (t<13)
  fprintf(out_file,"%s ", preamble[t-1]);
else
  fprintf(out_file,"  ");
if(begmo == mon && year == yr)
  fprintf(out_file,"\n  ");
sprintf(the_date,"%d/1/%d", mon, year);
fprintf(out_file,"%s %i  ", mo[mon-1], year);

  for (j=0; j<h2ocol[i]; ++j)
  {
  if(sdis[i][j].datatype_id == 0)
   fprintf(out_file,"%10.1f ", (h2opages[i][7].values[t]/h2opages[i][0].values[t])*1000);
  else if ( sdis[i][j].datatype_id == 49 || sdis[i][j].datatype_id == 13 )
   fprintf (out_file,"%10.2f ", h2opages[i][j].values[t]);
  else if (sdis[i][j].datatype_id == 47 || sdis[i][j].datatype_id==15 ||
              sdis[i][j].datatype_id == 17) 
        fprintf (out_file,"%10.0f ", h2opages[i][j].values[t]/1000);
  else if (sdis[i][j].datatype_id==86 )  {
     if ( t<13 )
        fprintf(out_file,"%10.1f ", h2opages[i][j].values[t]);
     else  {
       switch (sdis[i][j].site_id)
       {
        case 971:
          fprintf(out_file,"%10.1f ", 1731 * h2opages[i][j].values[t]);
          break;
        case 972:
/* Davis MW changed 6/10/03 per e-mail from Larry Karr at request of George Kraft*/
          fprintf(out_file,"%10.1f ", 255 * h2opages[i][j].values[t]);
          break;
        case 973:
          fprintf(out_file,"%10.1f ", 120 * h2opages[i][j].values[t]);
          break;
        }
       }
      }
  else if (sdis[i][j].datatype_id==77)
        fprintf (out_file,"%10.0f ", h2opages[i][j].values[t]*100);
  else if (units[i][j].unit == 1 || sdis[i][j].datatype_id ==74 || sdis[i][j].datatype_id ==104) {
     if (sdis[i][j].datatype_id == 25 )
        fprintf (out_file,"%10.0f ", h2opages[i][j].values[t]/1000);
     else if (sdis[i][j].datatype_id == 74 || sdis[i][j].datatype_id ==104) 
        fprintf (out_file,"%10.1f ", h2opages[i][j].values[t]/1000);
     else {
       cfs = h2opages[i][j].values[t];

       ConvertCFS_AF (units[i][j].scale, the_date, &cfs);
       h2opages[i][j].values[t] = cfs;

       fprintf(out_file,"%10.0f ", h2opages[i][j].values[t]/1000);}

     if (t == 1) {
      total=0.0;
      if (mon>= 10) {
        sprintf(begWY,"10/1/%d", yow);
        interval = mon - 10;}
      else
       { sprintf(begWY,"10/1/%d", yow - 1);
         interval = mon +2;}
      success = SqlWYdata(sdis[i][j].site_datatype_id, 
	sdis[i][j].site_id, begWY, begdate, data);
      if (mon >= 10)
	interval = mon - 10;
      else
	interval = mon + 2;
      for (k=1; k<=interval; ++k)  {
	if (sdis[i][j].datatype_id!=74 && sdis[i][j].datatype_id!=104){ 
	   cfs = data[k];
	   ConvertCFS_AF(units[i][j].scale, begWY, &cfs);
	   data[k] = cfs;}
	total = total + data[k];
        success = SqlFormatDate(begWY,begWY);
        success = SqlDateMath(ADDITION,begWY,newdate,1,"month");
	strcpy(begWY,newdate);}

      wytotal[j][t] = h2opages[i][j].values[t] + total;}
     else if (mon == 10)
      wytotal[j][t] = h2opages[i][j].values[t];
     else  
      wytotal[j][t] = h2opages[i][j].values[t] + wytotal[j][t-1];}
  else if (units[i][j].unit==2)
    fprintf(out_file,"%10.1f ",h2opages[i][j].values[t]/units[i][j].scale);
  else 
   fprintf(out_file,"%10.0f ", h2opages[i][j].values[t]/units[i][j].scale);

   }

  fprintf(out_file,"\n");
  if (mon == 9) { 
    fprintf(out_file,"  WY  %i  ", year);
    for (col=0; col<h2ocol[i]; ++col) {
     if (sdis[i][col].datatype_id == 74 || sdis[i][col].datatype_id == 104)
       fprintf(out_file,"%10.1f ", wytotal[col][t]/1000);
     else if (sdis[i][col].datatype_id == 17 || sdis[i][col].datatype_id == 15 ||
                 sdis[i][col].datatype_id == 47 || units[i][col].unit != 1)
       fprintf(out_file,"%s", buffer);
    /* else if (sdis[i][col].datatype_id == 25)
       fprintf(out_file,"%10.0f ", wytotal[col][t]/1000);*/
     else
       fprintf(out_file,"%10.0f ", wytotal[col][t]/1000);}
    fprintf(out_file,"\n\n");}

mon = mon + 1;
if (mon == 13) mon =1; }
fprintf(out_file,"\f");
}

/*Doing Upper Basin Power page*/
printf("Doing UB Power page\n");
mon = begmo;
year = yow;
fprintf(out_file,"O P E R A T I O N   P L A N   F O R   C O L O R A D O   R I V E R \
  S Y S T Y M   R E S E R V O I R S\n\n");
fprintf(out_file,"Bureau of Reclamation - CRFS %i/%i Most Prob Water Supply \
        %s\n",mon, yr, sysDate);

fprintf(out_file,"\t\t\t Upper Basin Power\n\n");

for (k=1; k<5; k++){
fprintf(out_file,"\t    ");
for (m=0; m<h2ocol[i]; ++m)
        fprintf(out_file,"%8s ", row[i][k].column[m]);
fprintf(out_file,"\n");}

strcpy(the_date, begdate);
 
/* Print out data now */
 for (t=1; t<37; ++t) {
if (mon==1 && t > 1)
  year = year + 1;
if (t<13)
  fprintf(out_file,"%s ", preamble[t-1]);
else
  fprintf(out_file,"  ");
if(begmo == mon && year == yr)
  fprintf(out_file,"\n  ");
sprintf(the_date,"%d/1/%d", mon, year);
fprintf(out_file,"%s %i  ", mo[mon-1], year);
/*if (mon==12) mon = 0;*/

for (j=0;j<h2ocol[UBPOWER]; ++j)
{
if (h2opages[UBPOWER][j].values[t]<0)  {
  h2opages[UBPOWER][j].values[t] = 0;
  fprintf(out_file,"         ");}
else{
  fprintf(out_file,"%8.0f ", h2opages[UBPOWER][j].values[t]/1000);}
if (t == 1)  { 
  total =0.0;
/* Check to see if it's summer or winter */
  if (mon >= 4 && mon <= 9) {
     interval = mon - 4;
     sprintf (begWY,"4/1/%d", yow -1 );}
  else  
    { 
     sprintf (begWY,"10/1/%d", yow - 1);
     if (mon >= 10)
       interval = mon - 10;
     else
       interval = mon + 2;
    }
  success = SqlWYdata(sdis[UBPOWER][j].site_datatype_id,
     sdis[UBPOWER][j].site_id, begWY, begdate, data);
  for (k=1; k<=interval; ++k)
     total = total + data[k];
  wytotal[j][t] = h2opages[UBPOWER][j].values[t] + total;}
else if (mon == 4 || mon == 10)  {
  wytotal[j][t] = h2opages[UBPOWER][j].values[t];}
else
  wytotal[j][t] = h2opages[UBPOWER][j].values[t] + wytotal[j][t-1];}

fprintf(out_file,"\n");
if (mon == 9 ) {  
  fprintf(out_file,"  Summer %i", year);
  for (col=0; col<h2ocol[i]; ++col)
    fprintf(out_file,"%7.0f  ", wytotal[col][t]/1000);
  fprintf(out_file,"\n\n");}
if (mon == 3 )  {
  fprintf(out_file,"  Winter %i", year); 
  for (col=0; col<h2ocol[i]; ++col) 
    fprintf(out_file,"%7.0f  ", wytotal[col][t]/1000); 
  fprintf(out_file,"\n\n");}

mon = mon +1;
if (mon ==13) mon =1;}  /* End of t loop for uB Power */
fprintf(out_file,"\f");


} /* End of Program */

