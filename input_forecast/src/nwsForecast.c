/*************************************************************************************************************
 *  nwsForecast  -   reads forecast data from file provided by NWS
 *               -   a sample file is located in this src directory called SAMPLE_INPUT_SAVE.TXT
 *               -   writes this data out to a text file called nws_forecast_input.dat
 *               -   rereads the file nws_forecast_input.dat (does so that editing can occur if necessary)
 *               
 *               -   to change reservoirs, site_ids and datatype ids that are processed you must
 *               -   edit the nwsForecast.h file
 *
 *  written by T Ryan - June 1996 - modified November 1996
 *  ported to Oracle by T Ryan November 1998
 *
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
#include "nwsForecast.h"

void modify_to_lower(char *);

int main( int argc, char** argv )

{
  
  /* declare variables for local use  */  
  
  FILE  *fp;
  SQL_DATE run_date;
  char	hday[20],beg_date[20],end_date[20],rolePassword[20];
  char  buffer[buffer_length],model_run_name[64];
  char	temp[65];		/* char string of some data number */
  char  ch,type[2],interval_indicator;
  char  year_c[10];
  char *dbName;
  char *appRole;
  ID    run_id,model_id;
  int   date_index,year,index,day,result,prob,length,site_datatype_id,site_datatype_id_tot,found,num_days;
  float val,val_tot;

  
  prob=PROB;

  if (argc != 4)
    {
      printf("\nusage: nwsForecast <input_file> <user name> <password> \n\n");
      exit(2);
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

  
  ch = 'a';
  
  while (feof (fp) == 0) /* while not EOF */
    {
      fgets (buffer, buffer_length, fp);
      modify_to_lower(buffer);
      switch (ch)
	{
	case 'a':
	  if (strstr(buffer,"colorado basin river forecast center") != NULL)
	    ch = 'b';
	  break;
	case 'b':
	  if (strstr(buffer,"forecast") != NULL)
	    {
	      /* figure out if its preliminary, mid month or final forecast */
	      if(strstr(buffer,"final") != NULL)
		strcpy(type,"F");
	      else if(strstr(buffer,"prelim") != NULL)
		strcpy(type, "P");
	      else if(strstr(buffer,"mid") != NULL)
		strcpy(type, "M");
	      else
		{
		  printf("Error, could not determine forecast type\n");
		  printf("Is this a preliminary, mid month or a final forecast?\n");
		  exit(1);
		}
	      /* now figure out the date */
	      for (index=0;index< 12; index++)
		{
		  if (strstr(buffer,mon[index]) != NULL)
		    {
		      date_index = index;
		      found = 1;
		    }
		}
	      if (found != 1)
		{
		  printf("Error, could not determine the date\n");
		  printf("Exiting\n");
		  exit(1);
		}

	      ch = 'c';
	      /* figure out the year*/
	      index = 0;
	      found = 0;
	      while (found !=1)
		{
		  if (buffer[index]  == ',')
		    {
		      strncpy(year_c,&buffer[index+1],8);
		      year_c[8] = '\0';
		      found = 1;
		    }
		  index = index + 1;
		  if (index > 60)
		    {
		      printf("Could not figure out the year\n");
		      printf("exiting");
		      exit(ERROR);
		    }
		}
	      year = atoi(year_c);

	      if(strcmp(type,"M")==0)
		sprintf(run_date,"15-%3s-%4d",mon[date_index],year);
	      else
		sprintf(run_date,"01-%3s-%4d",mon[date_index],year);

	      if(strcmp(type,"F") == 0)
		printf("\nThe data being loaded is for a Final Forecast\n");
	      else if (strcmp(type,"M") == 0)
		printf("\nThe data being loaded for a Mid Month Forecast\n");
	      else if (strcmp(type,"P") == 0)
		printf("\nThe data being loaded for a Preliminary Forecast\n");
	      else
		{
		  printf("Can not determine forecast type from file - exiting");
		  exit(ERROR);
		}
	      printf("\nThe model date for this data is %s\n",run_date);
	      strcpy(model_run_name,mon[date_index]);
	      model_run_name[0] = toupper(model_run_name[0]);
	      strcat(model_run_name," Forecast");

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
	    }
	  break;
	case 'c':
	  if (strstr(buffer,"monthly distribution of flow") != NULL)
	    ch = 'd';
	  break;
	case 'd':
	  for (index = 0; index < NUM_SITES; index++)
	    {
	      if (strstr(buffer,str_array[index]) != NULL)
		{		
		  /* get the site_datatype_ids for both total and average */

		  if((result = SqlGetSiteDatatypeIdWithId(site_id[index],datatype_id_tot[index],&site_datatype_id_tot)) != OK)
		    {
		      printf("Could not get a site_datatype is for site %d and datatype %d\n",site_id[index],datatype_id_tot[index]);
		      SqlDisconnect();
		      exit (ERROR);
		    }	    

		  if((result = SqlGetSiteDatatypeIdWithId(site_id[index],datatype_id[index],&site_datatype_id)) != OK)
		    {
		      printf("Could not get a site_datatype is for site %d and datatype %d\n",site_id[index],datatype_id[index]);
		      SqlDisconnect();
		      exit (ERROR);
		    }	    

		  interval_indicator = 'm';
		  strcpy(end_date,"1/1/1000");
		  
		  sprintf(beg_date,"01-%3s-%4d",mon[date_index],year);

		  printf("\nProcessing site %s   site id %d\n",str_array[index],site_id[index]);
		  strncpy(temp,&buffer[55],4);
		  temp[4]='\0';
		  sscanf(temp,"%f",&val);

		  /* convert to acre-feet */
		  
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
		  
		  /* printf("The number of days in %s is %d\n",beg_date,num_days); */
		  
		  /* printf("The af number is %f taf\n",val); */

		  val = (val * 1000) / (1.98347 * num_days);

		  /* printf("The cfs number is %f cfs\n",val);*/

		  /* load the converted (cfs) number into the database */
		  
		  if((result = insertModeldata(run_id,site_datatype_id,beg_date,end_date,val,interval_indicator)) != OK)
		    {
		      SqlDisconnect();
		      exit (ERROR);
		    }	    

		  strncpy(temp,&buffer[60],4);
		  temp[4]='\0';
		  sscanf(temp,"%f",&val);
		  if ((result = SqlDateMath(ADDITION,beg_date,hday,1,MONTH)) != OK)
		    {
		      SqlDisconnect();
		      exit (ERROR);
		    }	    
		  strcpy(beg_date,hday);

		  /* convert to acre-feet */
		  
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

		  strncpy(temp,&buffer[65],4);
		  temp[4]='\0';
		  sscanf(temp,"%f",&val);
		  if ((result = SqlDateMath(ADDITION,beg_date,hday,1,MONTH)) != OK)
		    {
		      SqlDisconnect();
		      exit (ERROR);
		    }	    

		  strcpy(beg_date,hday);

		  /* convert to acre-feet */
		  
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
		  if (date_index < 10 && date_index > 2) /* implies an april through july forecast*/
		    {
		      if (site_id[index] != 920) /* do not process navajo */
			{
			  interval_indicator = 'r';
			  strncpy(temp,&buffer[72],5);
			  temp[5]='\0';
			  sscanf(temp,"%f",&val);
			  sprintf(beg_date,"01-apr-%4d",year);
			  sprintf(end_date,"01-jul-%4d",year);
			  
			  /* convert to acre-feet */
			  
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
			  
			  val = (val * 1000) / (1.98347 * 122); /* 122 days in apr-jul runoff period */
			  
			  /* load the converted (cfs) number into the database */
			  
			  if((result = insertModeldata(run_id,site_datatype_id,beg_date,end_date,val,interval_indicator)) != OK)
			    {
			      SqlDisconnect();
			      exit (ERROR);
			    }	    
			}
		      else
			printf("Skiping April-July volume forecast for Navajo Regulated\n");
		    }
		}
	    }
	  break;
	default:
	  printf("We should never get to this print statement\n");
	  exit(1);
	}
    }

  fclose(fp);
  
  SqlDisconnect();  
  return;

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
