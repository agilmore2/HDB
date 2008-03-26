#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <typedefs.h>
#include <defines.h>

#include <dbutils.h>
#include <utils.h>
#include <lc_apps.h>
#include "24MoReport.h"

#define VERSION "2.0"
/* Changed version number to 2.0 6/7/01 because of the*/
/* addition of the Vallecito page*/



double RoundValue24(double value, int round_amount) 
{
    int temp_value;
    double difference;
    double rounding_cutoff;
    /* round_amount must be 1 or a factor of ten, or this fails to round */
    if ( (round_amount != 1) && (round_amount % 10 != 0) )
    {
        printf("Illegal rounding amount: %d\n", round_amount);
        return(value);
    }

    rounding_cutoff = ((double) round_amount) / 2;

    temp_value = (int) (value / round_amount);
    temp_value = temp_value * round_amount;
    difference = (double) (value - temp_value);
 
    if (difference < rounding_cutoff)
    { /* round down */
value = (double) temp_value;
        return (value);
    }
    else
    {                               /* round up   */
value = (double) (temp_value + round_amount);
        return ( value);
    }
}


double ConvertCFS_AF ( int scale, char * date, double * values )
{
  int success,days,mon, yr;

success = SqlFormatDate(date,date);
if (success != OK)
    {
        PrintError("Failed to format date correctly... exiting\n");
        exit (ERROR);
    }

  SqlExtractDatePart(date,MONTH,&mon);
  SqlExtractDatePart(date,YEAR, &yr);

  if (mon==4 || mon ==6 || mon ==9 || mon == 11)
    days = 30;
  else if (mon == 2)
    if ((yr%4)==0 && (yr%100)!=0 || (yr%400) ==0)
     days=29;
    else
     days=28;
  else
   days = 31;

*values = (*values * 1.9835 * days);

}

void main (int argc, char **argv)
{
time_t  clock;
char TimeString[TIMELENGTH];

double data[15], total=0, wytotal[MAX_NO_COLS][40];
int  interval, begmo, col, year, index, yow, mrid;
int  flag = 0, success, m, i, j, k;
int h2ocol[N_OF_SITES];
double  temp,cfs, af;
int  t, sdi, mon, yr, prob;
ids  sdis[N_OF_SITES][MAX_NO_COLS];
char runDate[25],sysDate[25],begWY[50],newdate[50],the_date[50], begdate[50], moddate[50];
char *dbName, *userName, buffer[12];
char my_string[300], lcdatabase[200], ucdatabase[200];
columndata h2opages[N_OF_SITES][MAX_NO_COLS];
uni_scale  units[N_OF_SITES][MAX_NO_COLS];
char *mo[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
char *preamble[12]={"*","H","I","S","T","O","R","I","C","A","L","*"};

struct heading row[N_OF_SITES][5];

FILE *in_file, *out_file;


out_file = fopen(WATER,"w");
if(out_file == NULL)
{
  PrintError("Error opening file for writing\n");
  exit(ERROR);
}

RegisterSignalHandler();

/* Check command options */

if (argc == 1 || argc > 3)
{
  printf ("Illegal command option\n\n");
  printf("Usage:  24MoReport [-v] [model_run_id] [-pa]\n\n");
  printf("Command line arguments:\n\n");
  printf("[-v]: prints version number.  Must be first argument.  Additional arguments ignored\n");
  printf("[model_run_id]: Model run id of the run (required unless -v option is specified).\n");
  printf("No option beside model_run_id: Writes just the water pages.\n");
  printf("[-p]: Specifies to write just power output.  Must be last argument.  Optional\n");
  printf("[-a]: Specifies to write the whole report.  Must be last argument.  Optional\n");

  exit(ERROR);
}

/* If the first argument is -v, execute and ignore the rest */

if(!strcmp(argv[1],"-v"))
{
  printf("\n%s version %s \n\n", argv[0], VERSION);
  exit(OK);
}

dbName = NULL;
dbName = getenv("HDB_LOCAL");
if (dbName == NULL)
{
  PrintError("Environment variable HDB_LOCAL not set... exiting\n");
  exit(ERROR);
}

userName = NULL;
userName = getenv ("APP_USER");
if (userName == NULL)
  {
    PrintError("Environment variable APP_USER not set... \n");
    exit (ERROR);
  }

success = SqlConnect(dbName,userName,dbName);
if (success != OK )
{
  PrintError("Failed to connect to database... exiting\n");
  exit(ERROR);
}

success = SqlGetDBName("LC", lcdatabase);
if (success != OK)
{
  PrintError("Failed to get name of LC database... exiting\n");
  exit(ERROR);
}

success = SqlGetDBName("UC", ucdatabase);
if (success != OK) 
{ 
  PrintError("Failed to get name of UC database... exiting\n"); 
  exit(ERROR);
}

SqlDisconnect();

if ((success = SqlConnectAt("LC",lcdatabase,userName,lcdatabase)) != OK ) {
  PrintError("Could not connect to lchdb database\n");
  exit(ERROR);
}
PrintError("Connected to lchdb.\n");


if ((success = SqlConnectAt("UC",ucdatabase,userName,ucdatabase)) != OK ) {
  PrintError("Could not connect to uchdb database\n");
  exit(ERROR);
}
PrintError("Connected to uchdb.\n");

/* Test for <command [model_run_id]> */
if (argc > 1)
{
  mrid = atoi(argv[1]);
  if ((success = SqlCheckModelRunId(mrid)) !=OK)  {
     PrintError("\n\nModel Run Id %i does not exist.  Exiting.\n",mrid);
     exit(ERROR);  }
  if (mrid <= 0)
  {
    printf("Error: Invalid model run id\n");
    exit (ERROR);
  }
  if(argc == 3)  {
    if (!strcmp(argv[2],"-p")) 
     flag = 1;
    else {
      if (strcmp(argv[2],"-a")){
      printf("Error:  Invalid option %s\n", argv[2]);
      exit (ERROR); }
    flag = 2; }
   }
}

if (flag==1) goto power;

/*Get System Run Date */
if ((success = SqlGetSystemRunDate(mrid,sysDate)) !=OK)  {
  PrintError("Could not get system date\n");
  exit(ERROR); }

/* Get Model Run Date from model table */
if ((success = SqlGetModelRunDate(mrid,&mon, &yr,runDate,&prob)) !=OK)  {
  PrintError("Could not get model run date\n");
  exit(ERROR); }

sprintf(moddate,"%d/1/%d", mon, yr);
printf("Generating report for model_run_id = %i\n", mrid);
printf("Model date:  %s %i\n", mo[mon-1], yr);
printf("System date: %s \n\n",sysDate);

  in_file = fopen(WAT_FILE,"r");
  if(in_file ==NULL)
  {
    PrintError("Error opening input file\n");
    exit(ERROR);
  }

/* Set the number of columns for the site */
  h2ocol[FONTNEL] = 7;
  h2ocol[FLAMING] = 11;
  h2ocol[TAYLOR] = 4;
  h2ocol[BLUE] = 8;
  h2ocol[MORROW] = 10;
  h2ocol[CRYSTAL] = 11;
  h2ocol[NAVAJO] = 9;
  h2ocol[POWELL] = 10;
  h2ocol[MEAD] = 10;
  h2ocol[MOHAVE] = 8;
  h2ocol[HAVASU] = 10;
  h2ocol[VALLECITO] = 4;


/* Start the loop to get the data */
for (j=0; j<N_OF_SITES; j++) {
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

/* Fetch model data from m_table */
SqlGet24ModData(sdis[j][i].site_id, mrid, sdi, mon, yr, h2opages[j][i].values);

success = SqlFormatDate(moddate,moddate);
if (success != OK)
    {
        PrintError("Failed to format date correctly... exiting\n");
        exit (ERROR);
    }

success = SqlDateMath(SUBTRACTION,moddate,begdate,1,YEAR);
if (success == ERROR) {
  PrintError("Could not get last years date\n");
  exit(ERROR); }

}   /* End column loop */
}   /* End site loop */


if((success = SqlExtractDatePart(begdate,"years", &yow)) != OK)
 {
  PrintError("Error in Extracting the Date\n");
 }

/* Write data to file */

/* Print Column Headings*/ 

for (i=0; i<N_OF_SITES; i++){
for (j=0; j<h2ocol[i]; j++){
sscanf(h2opages[i][j].column_head,
	"%s %s %s %s ", row[i][1].column[j], row[i][2].column[j], 
                row[i][3].column[j], row[i][4].column[j]);}}

/* Convert data and also compute Bypass data if ness */
for (j=0; j<N_OF_SITES; j++)  {
for (i=0; i<h2ocol[j]; i++)  {
strcpy(newdate,begdate);
for (t=1; t<37; ++t)  {

  if (units[j][i].unit == 1 && sdis[j][i].datatype_id !=17
     && sdis[j][i].datatype_id!=25 && sdis[j][i].datatype_id!=15
     && sdis[j][i].datatype_id!=80)  {
     temp = h2opages[j][i].values[t];
     ConvertCFS_AF(units[i][j].scale,newdate,&temp);
     h2opages[j][i].values[t] = temp;}

  if (sdis[j][i].datatype_id == 25)  {
     temp = h2opages[j][i].values[t];
     h2opages[j][i].values[t] = temp;}

  if (!strcmp(row[j][1].column[i],"Bypass"))  {
     h2opages[j][i].values[t] = fabs((double) h2opages[j][i].values[t]-
            (double) h2opages[j][i-1].values[t]);  }
  success = SqlDateMath(ADDITION,newdate,the_date,1,"month");
  strcpy(newdate,the_date);
        }
   }
}

/* Get the time from operating system to print out on pages.*/
clock = time(NULL);
ctime_r(&clock,TimeString,TIMELENGTH);

sprintf(buffer,"           ");
printf("Writing water pages\n\n");
for (i=0; i<N_OF_SITES; i++)
{
year = yow;
begmo = mon;
fprintf(out_file,"O P E R A T I O N   P L A N   F O R   C O L O R A D O   R I V E R \
  S Y S T E M   R E S E R V O I R S\n\n");
if(prob == 50)
fprintf(out_file,"Bureau of Reclamation - CRFS %i/%i Most Prob Water Supply \
        %s\n",mon, yr, sysDate);
if(prob == 10)
fprintf(out_file,"Bureau of Reclamation - CRFS %i/%i Prob Max Water Supply \
        %s\n",mon, yr, sysDate);
if(prob == 90)
fprintf(out_file,"Bureau of Reclamation - CRFS %i/%i Prob Min Water Supply \
        %s\n",mon, yr, sysDate);

switch  ( i )
{
  case FONTNEL:
     fprintf (out_file,"\t\t\t\t  Fontenelle Reservoir\n\n");
     break;
  case FLAMING:
     fprintf (out_file,"\t\t\t\t  Flaming Gorge Reservoir\n\n");
     break;
  case TAYLOR:
     fprintf (out_file,"\t\t\t\t  Taylor Park Reservoir\n\n");
     break;
  case BLUE:
     fprintf (out_file,"\t\t\t\t  Blue Mesa Reservoir\n\n");
     break;
  case MORROW:
     fprintf (out_file,"\t\t\t\t  Morrow Point Reservoir\n\n");
     break;
  case CRYSTAL:
     fprintf (out_file,"\t\t\t\t  Crystal Reservoir\n\n");
     break;
  case VALLECITO:
     fprintf (out_file,"\t\t\t\t  Vallecito Reservoir\n\n");
     break;
  case NAVAJO:
     fprintf (out_file,"\t\t\t\t  Navajo Reservoir\n\n");
     break;
  case MEAD:
     fprintf (out_file,"\t\t\t\t  Hoover Dam - Lake Mead\n\n");
     break;
  case POWELL:
     fprintf (out_file,"\t\t\t\t  Lake Powell\n\n");
     break;
  case MOHAVE:
     fprintf (out_file,"\t\t\t\t  Davis Dam - Lake Mohave\n\n");
     break;
  case HAVASU:
     fprintf (out_file,"\t\t\t\t  Parker Dam - Lake Havasu\n\n");
     break;
}

for (k=1; k<5; k++){
fprintf(out_file,"\t    ");
for (m=0; m<h2ocol[i]; ++m)
	fprintf(out_file,"%10s ", row[i][k].column[m]);
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
if (begmo==mon && year==yr)
  fprintf(out_file,"\n  ");
sprintf(the_date,"%d/1/%d", mon, year);
fprintf(out_file,"%s %i  ", mo[mon-1], year);

  for (j=0; j<h2ocol[i]; ++j)
  {
  if ( sdis[i][j].datatype_id == 49)
   fprintf (out_file,"%10.2f ", h2opages[i][j].values[t]);
  else if (sdis[i][j].datatype_id==47 || sdis[i][j].datatype_id==15 ||
	sdis[i][j].datatype_id==17)
	fprintf(out_file,"%10.0f ", RoundValue24(h2opages[i][j].values[t],1000) /1000);
  else if (units[i][j].unit == 1 && sdis[i][j].datatype_id!=17)  { 
     if (sdis[i][j].datatype_id == 25||sdis[i][j].datatype_id==80 )
        fprintf (out_file,"%10.0f ", RoundValue24(h2opages[i][j].values[t],1000) /1000);
     else {
     fprintf(out_file,"%10.0f ", RoundValue24(h2opages[i][j].values[t],1000) /1000);}

    if (t == 1) {
      total = 0;
      if (mon>= 10) {
        sprintf(begWY,"10/1/%d", yow);
        interval = mon - 10;}
      else 
       { sprintf(begWY,"10/1/%d", yow - 1);
         interval = mon +2;}


      success = SqlWYdata(sdis[i][j].site_datatype_id, 
	sdis[i][j].site_id, begWY, begdate, data);

      for (k=1; k<=interval; ++k)  {
        cfs = data[k];

        if (sdis[i][j].datatype_id !=25 && sdis[i][j].datatype_id!=80)  {
        ConvertCFS_AF(units[i][j].scale,begWY,&cfs);}
        data[k] = cfs;
        total = total + data[k];
        success = SqlFormatDate(begWY,begWY);
	success = SqlDateMath(ADDITION, begWY, newdate, 1,"month");
	strcpy(begWY, newdate);  }

      if (sdis[i][j].datatype_id==39) temp =total;
      else if (!strcmp(row[i][1].column[j],"Bypass")) total = fabs((double)total - (double)temp);
      wytotal[j][t] = h2opages[i][j].values[t]/1000 + total/1000;}
    else if (mon == 10)
      wytotal[j][t] = h2opages[i][j].values[t]/1000;
    else  
      wytotal[j][t] = h2opages[i][j].values[t]/1000 + wytotal[j][t-1];}
  else if (units[i][j].unit==2)
   fprintf(out_file,"%10.1f ", h2opages[i][j].values[t]/units[i][j].scale);
  else 
   fprintf(out_file,"%10.0f ", h2opages[i][j].values[t]);

   }

  fprintf(out_file,"\n");
  if (mon == 9) {
    fprintf(out_file,"  WY  %i  ", year);
    for (col=0; col<h2ocol[i]; ++col) {
     if (sdis[i][col].datatype_id == 17 || sdis[i][col].datatype_id==15
          || units[i][col].unit != 1 || sdis[i][col].site_datatype_id==1379)
/* Took out the following line because we now have data for Vallecito */
/* Did this on 6/12/01.  LMS */
/*         || sdis[i][col].site_datatype_id==1882)*/
       fprintf(out_file,"%s", buffer);
     else if (wytotal[col][t] > 50000000)
       fprintf(out_file,"%s", buffer);
     else if (sdis[i][col].datatype_id == 25 )
       fprintf(out_file,"%10.0f ", RoundValue24(wytotal[col][t],1) );
     else
       fprintf(out_file,"%10.0f ", RoundValue24(wytotal[col][t],1) );}
    fprintf(out_file,"\n\n");}
if (mon==12) mon=0;
mon = mon + 1;}
fprintf(out_file,"\f");
}

fclose(out_file);

power: if (flag == 1) {
  PowerReport(mrid);}

if (flag == 2)  {
  PowerReport(mrid);}

printf("Output written to 24MoReport.out\n");
SqlDisconnectAll();

}
