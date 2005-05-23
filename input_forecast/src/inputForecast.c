/*************************************************************************************************************
 *  inputForecast_month  -   loads forecasted monthly flows into HDB
 *
 *  written by T Ryan - December 1996
 *
 *             
 **************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <ctype.h>
#include "dbutils.h"
#include "typedefs.h"
#include "stdlib.h"
#include "utils.h"
#include "defines.h"
#include "inputForecast.h"

void modify_to_lower(char *);

int main( int argc, char** argv )
     
{
  
  /* Declare variables for local use  */  
  
  SQL_DATE run_date,beg_date,end_date;
  char	rolePassword[20];
  char  model_run_name[64],prob_test[4],range_test[4];
  char  type[2],interval_indicator;
  char *dbName;
  char *appRole;
  ID    run_id,model_id;
  int   day_run;
  int   index,day,month,result,prob,length,site_datatype_id,site_datatype_id_tot,num_days,test,sid,i;
  float val,val_tot;
  
  if (argc != 10)
    {
      printf("\nusage: inputForecast\n");
      printf(" \t< run_date >          Date this forecast value was issued\n");
      printf(" \t< P | M | F >         Preliminary  Mid Month  or  Final Forecast\n");
      printf(" \t< Most | Max | Min>   Probability\n");
      printf(" \t<site_id>             Site ID\n");
      printf(" \t<value date>          Date of Forecasted Value\n");
      printf(" \t<value>               Forecasted value in Thousands of Acre-Feet\n"); 
      printf(" \t< MON | A-J >         Flag for monthly or April through July volume\n"); 
      printf(" \t<User>                Oracle User\n");
      printf(" \t<oracle password>     Password for Oracle User\n\n");
      exit(ERROR);
    }

  sid = atoi(argv[4]); /* note must include stdlib.h to use atof and atoi */
  val = atof(argv[6]);

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
  
  if ((result = SqlConnectRole (dbName, argv[8], argv[9], appRole)) == ERROR)
    exit (ERROR);
  
  fprintf(stdout, "Connected to ORACLE.\n");
  
  
  strcpy(run_date,argv[1]);
  
  /* extract out the day of the model run command line argument */

  SqlFormatDate(run_date,run_date);  

  if(SqlExtractDatePart(run_date,DAY,&day_run) != OK)
    {
      printf("\nError! First Argument in Command Line Must a valid SQL date - Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }
  
  /* check to see that the date is either the 1st or the 15th */
  /* first valid day for preliminary or final model runs - 15th valid for preliminary only */
  
  if (day_run != 1 && day_run != 15)
    {
      printf("\nError! The day in the run date must be the first or fifteenth day in the month - exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }
  
  strcpy(beg_date,argv[5]);
  SqlFormatDate(beg_date,beg_date);  
  
  /* extract out the day and month from the date of the forecasted value */
  
  if(SqlExtractDatePart(beg_date,DAY,&day) != OK)
    {
      printf("\nError! Fifth Argument in Command Line Must a valid SQL date - Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }

  if(SqlExtractDatePart(beg_date,MONTH,&month) != OK)
    {
      printf("\nError! Fifth Argument in Command Line Must a valid SQL date - Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }
  
  /* check to see that the date of the forecasted value is the 1st */
  
  if (day != 1)
    {
      printf("\nError! The day in the date of the forecasted value must be the first of the month - exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }
  
  /* do some error checking on the second argument */
  
  test = 0;
  strcpy(type,argv[2]);
  modify_to_upper(type);
  if(type[0] == 'P')
    test = 1;
  else if(type[0] == 'F')
    test = 1;
  else if(type[0] == 'M')
    test = 1;
  else
    {
      printf("\n\nError! Second Argument in Command Line Must be either P M or F - Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }
  
  /* if the model run day is the 15th make sure that this is a mid month forecast */
  
  if (day_run == 15)
    if (type[0] != 'M')
      {
	printf("\n\nError! The model run date is 15 but the model type is not M (Mid Month) - Exiting\n\n");
	SqlDisconnect();
	exit (ERROR);
      } 
  
  /* trim the third argument down to 3 characters and do some error checking */
  
  strcpy(prob_test,argv[3]);
  modify_to_lower(prob_test);
  
  if(strncmp(prob_test,"mos",3) == 0)
    {  
      prob = 50;
    }
  else if(strncmp(prob_test,"max",3) == 0)
    {
      prob = 10;
    }
  else if(strncmp(prob_test,"min",3) == 0)
    {
      prob = 90;
    }
  else
    {
      printf("\n\nError! Third Argument in Command Line Must be either Most Max or Min - Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }
  
  /* trim the seventh argument down to 3 characters and do some error checking */
  
  strcpy(range_test,argv[7]);
  modify_to_lower(range_test);
  
  if(strncmp(range_test,"mon",3) == 0)
      interval_indicator = 'm';
  else if(strncmp(range_test,"a-j",3) == 0)
      interval_indicator = 'r';
  else
    {
      printf("\n\nError! Seventh Argument in Command Line Must be either MON or A-J - Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }

  /* if the range flag was A-J (interval indicator is m) make sure that */
  
  if (interval_indicator == 'r')
    if (month != 4)
      {
	printf("\n\nError! The range indicator is A-J but the forecasted value date is not for April - Exiting\n\n");
	SqlDisconnect();
	exit (ERROR);
      } 
  

  /* check to see if the site is in the valid_sites array */
  
  test = 0;
  for (i = 0; i < NUM_SITES; i++)
    {
      if(valid_sites[i].site_id == sid)
	{
	  index = i;
	  test = 1;
	}
    }
  
  if (test != 1)
    {
      printf("\nError! The site on the command line is not in the .h file\n");
      SqlDisconnect();
      exit (ERROR);
    }
  
  /* get the model id from the database */


  strcpy(model_run_name,"Inflow Forecast");
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
    }
  /* get the site_datatype_ids for both total and average */
  
  if((result = SqlGetSiteDatatypeIdWithId(valid_sites[index].site_id,valid_sites[index].total_datatype_id,&site_datatype_id_tot)) != OK)
    {
      printf("Could not get a site_datatype is for site %d and datatype %d\n",valid_sites[index].site_id,valid_sites[index].total_datatype_id);
      printf("Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }	    
  
  if((result = SqlGetSiteDatatypeIdWithId(valid_sites[index].site_id,valid_sites[index].ave_datatype_id,&site_datatype_id)) != OK)
    {
      printf("Could not get a site_datatype is for site %d and datatype %d\n",valid_sites[index].site_id,valid_sites[index].ave_datatype_id);
      SqlDisconnect();
      exit (ERROR);
    }	    

  
  if (interval_indicator == 'm')
    {
      printf ("its a month\n");
      /*      exit (ERROR);  */
      /* Not sure why this was broken */

      strcpy(end_date,"1/1/1000");
      
      val_tot = val * 1000;
      
      /* load the acre-foot number into the database */
      
      if((result = insertModeldata(run_id,site_datatype_id_tot,beg_date,end_date,val_tot,interval_indicator)) != OK)
	{
	  SqlDisconnect();
	  exit (ERROR);
	}	    
      
      /* convert to average cfs values */
      
      if ((result = NumDaysMonth(beg_date, &num_days)) != OK)
	{
	  SqlDisconnect();
	  exit (ERROR);
	}	    
      
      val = (val * 1000) / (1.98347 * num_days);

      /* load the converted (cfs) number into the database */
      
      if((result = insertModeldata(run_id,site_datatype_id,beg_date,end_date,val,interval_indicator)) != OK)
	{
	  SqlDisconnect();
	  exit (ERROR);
	}	    
    }
  else if (interval_indicator == 'r')
    {
      printf ("Loading Range Data\n");
      if ((result = SqlDateMath(ADDITION,beg_date,end_date,3,MONTH)) != OK)      
	{
	  SqlDisconnect();
	  exit (ERROR);
	}	    

      /* convert to acre-feet */
      
      val_tot = val * 1000;
      
      /* load the acre-foot number into the database */
      
      if((result = insertModeldata(run_id,site_datatype_id_tot,beg_date,end_date,val_tot,interval_indicator)) != OK)
	{
	  SqlDisconnect();
	  exit (ERROR);
	}	    
      
      /* convert to average cfs values */
      
      val = (val * 1000) / (1.98347 * 122); /* 122 days in apr-jul runoff period */
      
      /* load the converted (cfs) number into the database */
      
      if((result = insertModeldata(run_id,site_datatype_id,beg_date,end_date,val,interval_indicator)) != OK)
	{
	  SqlDisconnect();
	  exit (ERROR);
	}	    
    }
  else
    printf("\nWe should never reach this print statement\n");

  
  SqlDisconnect();  
  return;
  
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

/**************************************************************************************************************
 * modify_to_lower and modify_to_upper
 * Purpose: converts string to all lower case or all upper case
 *
 *************************************************************************************************************/

void modify_to_lower (char *line)
{
  while (*line != '\0')
    {
      if(isupper(*line))
	*line = tolower(*line);
      line++;
    }
}

void modify_to_upper (char *line)
{
  while (*line != '\0')
    {
      if(islower(*line))
	*line = toupper(*line);
      line++;
    }
}

/**************************************************************************************************************/


