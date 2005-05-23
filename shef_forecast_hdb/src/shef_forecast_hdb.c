/*************************************************************************************************************
 *  shef_forecast_hdb  -   reads forecast "shef" decoded data from file provided by NWS 
 *     -   a sample file is located in this src directory called sample_shef_output
 *               
 *               -   to change reservoirs, site_ids and datatype ids that are processed you must
 *               -   edit the shef_forecast_hdb.h file
 *
 *  written by T Ryan - June 1996 - modified November 1996
 *  ported to Oracle by T Ryan November 1998
 *  rewritten to read shef decoded output by A Gilmore March 2000, and
 *   renamed "shef_forecast_hdb"
 *  Remaining issues: The forecast has these forecast date to july forecasts coded.
 *   we don't want them to be used, because the range of the forecast or the probability
 *   for the forecast may be wrong. The probability of all these forecasts are checked to
 *   make sure that the probability is 0.5. However, in April and July (when those forecasts
 *   are the same length (and hopefully the same numbers)) the forecasts will be processed twice.
 *   The reason for this is that the forecasts will have the same length as the later forecasts
 *   that we do want.
 *             
 **************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dbutils.h"
#include "typedefs.h"
#include "utils.h"
#include "defines.h"
#include "shef_forecast_hdb.h"
#include "shefLocations.h"

void modify_to_lower(char *);

int main( int argc, char** argv )

{
  
   /* declare variables for local use  */  
  
   FILE  *fp;
   SQL_DATE run_date;
   char  beg_date[20],end_date[20];
   char  buffer[buffer_length],model_run_name[64];
   char  temp[65];               /* char string of some data number */
   char  type[2];
   char  year_c[5];
   char *shefDebugString;
   char *dbName;
   char *appRole;
   char *location;
   char  interval_indicator;
   ID    run_id,model_id;
   int   shefDebug = 0;
   int   sitesfound[NUM_SITES];
   int   i,date_index,year,index,result,prob;
   int   forecast_length,forecast_month,found_type_id;
   int   site_datatype_id,site_datatype_id_tot,num_days;
   float val,val_tot, forecast_prob;
   
   prob=PROB;

   if (argc != 4)
   {
      printf("\nusage: shef_forecast_hdb <input_file> <user name> <password> \n\n");
      exit(2);
   }
  
   shefDebugString = NULL;
   shefDebugString = getenv("DEBUG_SHEF");

   if (shefDebugString != NULL)
   {
      PrintError("Environment variable DEBUG_SHEF is set, will not insert or update in the database\n");
      shefDebug = 1;
   }

   dbName = NULL;
   dbName = getenv("HDB_LOCAL");
  
   if (dbName == NULL)
   {
      PrintError("Environment variable HDB_LOCAL not set...\n");
      return(ERROR);
   }

   /* get the appRole information */

   appRole = NULL;
   appRole = getenv ("APP_ROLE");

   if (appRole == NULL)
   {
      PrintError("Environment variable APP_ROLE not set...\n");
      return(ERROR);
   }
  
   /* run  the signal handler function which will disconnect from the
      database and free up tables if things go wierd */

   RegisterSignalHandler();
  
   if ((result = SqlConnectRole (dbName, argv[2], argv[3], appRole)) == ERROR)
      exit (ERROR);
  
   fprintf(stdout, "Connected to ORACLE.\n");
  
  
   /* open the base file */
   /* open the file specified on the command line */
  
   if ((fp = fopen(argv[1],"r")) == NULL)
   {
      printf ("\nError opening file %s\n\n", argv[1]);
      exit(1);
   }
   
   found_type_id = 0;

   /* Fill this array that will be used later to check if at least one forecast
    was found for each site */
   for (i= 0; i < NUM_SITES; i++) {
      sitesfound[i] = 0;
   }

   while (feof (fp) == 0) /* while not EOF */
   {
      if (fgets (buffer, buffer_length, fp) == NULL) /*catch eof on newline */
         continue; 
      /*      modify_to_lower(buffer);*/
      switch (found_type_id)
      {
          case 0:
          if ((location = strstr(buffer,"E=")) != NULL)
          {
             /* figure out if its preliminary, mid month or final forecast */
             strncpy(type,location+2,1);

             if(strcmp(type,"F") == 0)
                printf("\nThe data is from a Final Forecast\n");
             else if (strcmp(type,"M") == 0)
                printf("\nThe data is from a Mid Month Forecast\n");
             else if (strcmp(type,"P") == 0)
                printf("\nThe data is from a Preliminary Forecast\n");
             else
             {
                printf("Is this a preliminary, mid month or a final forecast?\n");
                printf("Can not determine forecast type from file - exiting");
                exit(ERROR);
             }
                
             /* now figure out the date */ 
             temp[0] = buffer[SHEF_ISSUE_MONTH_LOC-1];
             temp[1] = buffer[SHEF_ISSUE_MONTH_LOC];
	     temp[2] = '\0';
             forecast_month = atoi(temp) - 1; /*array is 0 based!*/

             /* figure out the year*/

             strncpy(year_c,&buffer[SHEF_ISSUE_YEAR_LOC],4);
             year_c[4] = '\0';
             year = atoi(year_c);

             if(strcmp(type,"M")==0)
                sprintf(run_date,"15-%3s-%4d",mon[forecast_month],year);
             else
                sprintf(run_date,"01-%3s-%4d",mon[forecast_month],year);

             printf("\nThe model date for this data is %s\n",run_date);
             strcpy(model_run_name,mon[forecast_month]);
             model_run_name[0] = toupper(model_run_name[0]);
             strcat(model_run_name," Forecast");

             if (!shefDebug) {
             if((result = SqlGetModelId("NWS-NRCS Forecast", &model_id)) != OK)
             {
                SqlDisconnect();
                exit (ERROR);
             }          
             /* check to see if a modelrunid exist for this combination of model_id, run_date, probability, and modeltype */
              
             if((result = checkforModelrunid(model_id,run_date,prob,type,&run_id)) == 1)
                printf("Data will be inserted or updated using an existing model_run_id\n");
             else if (result == 2)
             {
                printf("ERROR! Duplicate records exist in table ref_model_run\n\n");
                exit (ERROR);
             }
             else
                                /* get a new model run id */
             {
                printf("Generating new model run id\n");
                if((result = SqlGetModelRunId(model_id,model_run_name,run_date,prob,type,NA,NULL,NULL,NULL,NULL,&run_id)) != OK)
                {
                   SqlDisconnect();
                   exit (ERROR);
                }
                printf("New Model Run Id is : %d\n", run_id);
             }
             }
          }
          found_type_id = 1;

          /* Intentional drop through to case 1. */
          case 1:

          strncpy(temp, &buffer[SHEF_FORECAST_PROB_LOC], 8);
          temp[8] ='\0';
          forecast_prob = atof (temp);

          /* IF the forecast probability is not 0.5, then we want to skip it and continue with the next line */
          if (forecast_prob < 0.055 || forecast_prob > 0.945 )
             continue;
          
          temp[0] = buffer[SHEF_FORECAST_LENGTH_LOC];
          temp[1] ='\0';
          forecast_length = atoi (temp);

          for (index = 0; index < NUM_SITES; index++)
          {
             if (strstr(buffer,str_array[index]) != NULL)
             {
                /* get the site_datatype_ids for both total and average */
                
                if((result = SqlGetSiteDatatypeIdWithId(site_id[index],
                      datatype_id_tot[index],&site_datatype_id_tot)) != OK)
                {
                   printf("Could not get a site_datatype for site %d and datatype %d\n",site_id[index],datatype_id_tot[index]);
                   SqlDisconnect();
                   exit (ERROR);
                }

                if((result = SqlGetSiteDatatypeIdWithId(site_id[index],
                      datatype_id[index],&site_datatype_id)) != OK)
                {
                   printf("Could not get a site_datatype for site %d and datatype %d\n",site_id[index],datatype_id[index]);
                   SqlDisconnect();
                   exit (ERROR);
                }

                if (forecast_length == 1)  /* single month forecast */
                {
                   if (site_id[index] == 920 &&
                       buffer[SHEF_FORECAST_TYPE_LOC] == 'A') /* do not process navajo unregulated forecast*/
                   {
                      fprintf(stderr,"Skipping Navajo unregulated forecast\n");
                      break; 
                   }
                   else if (site_id[index] == 933 &&
                       buffer[SHEF_FORECAST_TYPE_LOC] == 'A') /* do not process vallecito change in storage forecast */
                   {
                      fprintf(stderr,"Skipping Vallecito change in storage forecast\n");
                      break; 
                   }
                
                   interval_indicator = 'm';
                  
                   /* now figure out the date */ 
                   temp[0] = buffer[SHEF_FORECAST_MONTH_LOC-1];
                   temp[1] = buffer[SHEF_FORECAST_MONTH_LOC];
                   temp[2] ='\0';
                   date_index = atoi(temp) - 2 ; /* end of month, and zero based array */
                   /* figure out the year*/

                   strncpy(year_c,&buffer[SHEF_FORECAST_YEAR_LOC],4);
                   year_c[4] = '\0';
                   year = atoi(year_c);

                   /* fix the wrap around case where the forecast is for
		      december, but the month indicated in the shef output is
		      january, oh, and fix the year, too! */
		   if (date_index == -1) 
		     { 
		       date_index = 11;
		       year -= 1;
		     }

                   sprintf(beg_date,"01-%3s-%4d",mon[date_index],year);
                   strcpy(end_date,"1/1/1000");

                   if ((result = NumDaysMonth(beg_date, &num_days)) != OK)
                   {
                      SqlDisconnect();
                      exit (ERROR);
                   }     
                  
/*                    printf("\nThe number of days in %s is %d\n", */
/*                           beg_date, num_days); */

                }
                else if (forecast_length == 4) /* april-jul forecast */
                {
                   interval_indicator = 'r';

                   sprintf(beg_date,"01-apr-%4d 00:00:00",year);
                   sprintf(end_date,"01-jul-%4d",year);
                          
                   num_days = 122; /* 122 days in apr-jul runoff period */
                          
/*                    printf("\nThe number of days in April - July is %d\n", */
/*                           num_days); */

                }
                else continue; /* other length forecasts not used */
 
                fprintf(stderr,"Processing forecast at %s for site %s site id %d :",
                       beg_date, str_array[index],site_id[index]);

                strncpy(temp,&buffer[SHEF_VALUE_LOC],SHEF_VALUE_WIDTH);
                temp[SHEF_VALUE_WIDTH]='\0';
                sscanf(temp,"%f",&val);
                
                /* convert to acre-feet */
                  
                fprintf(stderr,"The af number is %f taf. ",val);

                val_tot = val * 1000;

                /* convert to average cfs values */

                val = (val * 1000) / (1.98347 * num_days);
                fprintf(stderr,"The cfs number is %f cfs\n",val);

                /* load the acre-foot number into the database */

                if (!shefDebug) {
                if((result = insertModeldata(run_id,site_datatype_id_tot,
                          beg_date,end_date,val_tot,interval_indicator)) != OK)
                {
                   SqlDisconnect();
                   exit (ERROR);
                }       
                
                /* load the converted (cfs) number into the database */
                
                if((result = insertModeldata(run_id,site_datatype_id,
                             beg_date,end_date,val,interval_indicator)) != OK) 
                { 
                   SqlDisconnect(); 
                   exit (ERROR); 
                }      
                }
                sitesfound[index] = 1;
             }
          }
          break;
          default:
             printf("We should never get to this print statement\n");
             exit(1);
      }
   }

   for (i = 0; i < NUM_SITES; i++) {
      if (sitesfound[i] == 0) {
         printf("WARNING!! Forecast not processed for site %s.\n", 
                str_array[i]);
      }
   }
   
   fclose(fp);
  
   SqlDisconnect();  
   return 0;

}

void modify_to_lower (char *line)
{
  while (*line != '\0')
    {
      if(isupper(*line))
        *line = tolower(*line);
      line++;
    }
}

int NumDaysMonth (SQL_DATE beg_date, int *num)

{
  SQL_DATE temp_day;
  int result;
  
  SqlFormatDate(beg_date,beg_date);
  if ((result = SqlDateMath(ADDITION,beg_date,temp_day,1,MONTH)) != OK)
    {
      SqlDisconnect();
      exit (ERROR);
    }       
  if ((result = SqlNumDaysBetweenDates(beg_date,temp_day,num)) != OK)
    {
      SqlDisconnect();
      exit (ERROR);
    }       
  
  *num = *num - 1;
  
  return (OK);
}
