/*************************************************************************************************************
 * inflow_24mo  -   creates inflow data for the 24 month study
 *  
 *  written by T Ryan - September 1996
 *  ported to Oracle by T Ryan November 19989
 *             
 **************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <stdlib.h>
#include "dbutils.h"
#include "typedefs.h"
#include "utils.h"
#include "defines.h"

/* include file inflow_24mo.h contains key information about the programs behavior */
/* see comments in inflow_24mo.h for more information */

#include "inflow_24mo.h"

#define FUNC_NAME "main"

int main( int argc, char** argv )

{
  char             type[20],prob[20],rolePassword[20];
  char *dbName;
  char *appRole;
  int              probability,fc_type,i,j,m,run_month,datasource[12],probab,temp_probab,index_int;
  int              test_aj,test_int,length,result,index,run_year,apr_jul_indicator,day;
  int              index_aj,kk,num_interp;
  /*  float            inflow_array[36],apr_jul_val; change to the following - 1/2000 tryan */
  float            inflow_array[72],apr_jul_val;
  SQL_DATE         dt,b_dt,e_dt,model_run_date,prev_date;
  ID               site_id,datatype_id,neighbor_site_id,neighbor_datatype_id;
  ID               model_run_id_24mo;
  ID               stats_model_run_id;
  MO_INPUT_SPEC    *spec; /* this is a pointer to a structure of MO_INPUT_SPEC */
  HYDROLOGY        *month_spec; /* this is a pointer to a structure of HYDROLOGY */
  VALUE            val;
  VALUE            most_hist_stats[12],max_hist_stats[12],min_hist_stats[12],neighbor_hist_stats[12];

  if (argc != 6)
    {
      printf("\nusage: inflow_24mo < run_date > < P | M | F > < Most | Max | Min>  <user name> <password>\n\n");
      exit(ERROR);
    }


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
  
  if ((result = SqlConnectRole (dbName, argv[4], argv[5], appRole)) == ERROR)
    exit (ERROR);
  
  fprintf(stdout, "Connected to ORACLE.\n");
  
  strcpy(model_run_date,argv[1]);

  SqlFormatDate(model_run_date,model_run_date);

  /* extract out the month, year, and day of the model run command line argument */

  if(SqlExtractDatePart(model_run_date,MONTH,&run_month) != OK)
    {
      printf("\n\nError! First Argument in Command Line Must a valid SQL date - Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }
  
  if(SqlExtractDatePart(model_run_date,YEAR,&run_year) != OK)
    
    {
      SqlDisconnect();
      exit (ERROR);
    }
  
  if(SqlExtractDatePart(model_run_date,DAY,&day) != OK)
    {
      printf("\nError! First Argument in Command Line Must a valid SQL date - Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }

  /* check to see that the date is either the 1st or the 15th */
  /* first valid day for preliminary or final model runs - 15th valid for preliminary only */

  if (day != 1 && day != 15)
    {
      printf("\nError! The day in the date must be the first or fifteenth day in the month - exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }

  /* do some error checking on the second argument */
  
  strcpy(type,argv[2]);
  modify_to_upper(type);
  if(type[0] == 'P')
      fc_type = PRELIM;
  else if(type[0] == 'F')
      fc_type = FINAL;
  else if(type[0] == 'M')
      fc_type = MID;
  else
    {
      printf("\n\nError! Second Argument in Command Line Must be either P M or F - Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }

  /* if the model day is the 15th make sure that this is a mid month forecast */

  if (day == 15)
    if (fc_type != MID)
     {
       printf("\n\nError! The model run date is 15 but the model type is not M (Mid Month) - Exiting\n\n");
       SqlDisconnect();
       exit (ERROR);
     } 

  /* trim the third argument down to 3 characters and do some error checking */

  strcpy(prob,argv[3]);
  modify_to_lower(prob);
  
  if(strncmp(prob,"mos",3) == 0)
    {  
      probability = MOST;
      probab = 50;
    }
  else if(strncmp(prob,"max",3) == 0)
    {
      probability = MAX;
      probab = 10;
    }
  else if(strncmp(prob,"min",3) == 0)
    {
      probability = MIN;
      probab = 90;
    }
  else
    {
      printf("\n\nError! Third Argument in Command Line Must be either Most Max or Min - Exiting\n\n");
      SqlDisconnect();
      exit (ERROR);
    }

  /* get or create the output model run id */

  if(probability == MOST)
    {
      if((result = checkforModelrunid(aop_model_id,model_run_date,probab,type,&model_run_id_24mo)) == 1)
	printf("The model_run_id will be %d\n",model_run_id_24mo);
      else if (result == 2)
	{
	  printf("ERROR! Duplicate records exist in table ref_model_run\n\n");
	  exit (ERROR);
	}
      else
	{
	  printf("Generating new model run id\n");
	  if((result = SqlGetModelRunId(aop_model_id,model_run_name_most,model_run_date,probab,type,NA,NULL,NULL,NULL,NULL,&model_run_id_24mo)) != OK)
	    {
	      SqlDisconnect();
	      exit (ERROR);
	    }	    
	  printf("The new model run id is %d\n",model_run_id_24mo);
	}
    }
  else if(probability == MAX)
    {
      if((result = checkforModelrunid(aop_model_id,model_run_date,probab,type,&model_run_id_24mo)) == 1)
	printf("The model_run_id will be %d\n",model_run_id_24mo);
      else if (result == 2)
	{
	  printf("ERROR! Duplicate records exist in table ref_model_run\n\n");
	  exit (ERROR);
	}
      else
	{
	  printf("Generating new model run id\n");
	  if((result = SqlGetModelRunId(aop_model_id,model_run_name_max,model_run_date,probab,type,NA,NULL,NULL,NULL,NULL,&model_run_id_24mo)) != OK)
	    {
	      SqlDisconnect();
	      exit (ERROR);
	    }	    
	  printf("The new model run id is %d\n",model_run_id_24mo);
	}
    }
  else if(probability == MIN)
    {
      if((result = checkforModelrunid(aop_model_id,model_run_date,probab,type,&model_run_id_24mo)) == 1)
	printf("The model_run_id will be %d\n",model_run_id_24mo);
      else if (result == 2)
	{
	  printf("ERROR! Duplicate records exist in table ref_model_run\n\n");
	  exit (ERROR);
	}
      else
	{
	  printf("Generating new model run id\n");
          if((result = SqlGetModelRunId(aop_model_id,model_run_name_min,model_run_date,probab,type,NA,NULL,NULL,NULL,NULL,&model_run_id_24mo)) != OK)
	    {
	      SqlDisconnect();
	      exit (ERROR);
	    }	    
	  printf("The new model run id is %d\n",model_run_id_24mo);
	}
    }
  else
    {
      printf("We should never reach this print statement\n");
      SqlDisconnect();
      exit (ERROR);
    }	    
  
  printf("\n\tBeginning Data Synthesis\n\n");
  printf("\tDeveloping Data for a %5s Probable Model Run\n",probc[probability]);
  printf("\tUsing a %12s Forecast\n",typec[fc_type]);
  printf("\tThis Model Run will begin on %4s 1, %4d\n",months[run_month-1],run_year);
  
  /* Begin loop for all of the reservoirs */

  for(i = 0; i < NUM_SITES; i++)
    {
      /* initialize the site and datatype_ids from the driver variable */

      site_id = driver[i].site_id;
      datatype_id = driver[i].datatype_id;

      printf("\n\tProcessing Reservoir  %2d\n",i+1);
      printf("\tReservoir id is %4d\n",site_id);
      printf("\tDatatype_id is %4d\n\n",datatype_id);

      /* initalize the pointer spec to point to the place in memory of defaulter or special (as appropriate) */
      
      if(driver[i].use == 'd')
	spec = &driver[i].defaulter;
      else if(driver[i].use == 's')
	spec = &driver[i].special;
      else
	{
	  printf("\n\nError! The use element of driver must be either d or s; check the .h file  - Exiting\n\n");
	  SqlDisconnect();
	  exit (ERROR);
	}
      
      /* within the switch block initialize the pointer month_spec to point at the beginning of the appropriate month */
      /* basically, using the value of run_month we are finding locating the correct place to point within the structure */

      switch(run_month - 1)
	{
	case 0:
	  month_spec = spec-> jan_run;
	  break;
	case 1:
	  month_spec = spec-> feb_run;
	  break;
	case 2:
	  month_spec = spec-> mar_run;
	  break;
	case 3:
	  month_spec = spec-> apr_run;
	  break;
	case 4:
	  month_spec = spec-> may_run;
	  break;
	case 5:
	  month_spec = spec-> jun_run;
	  break;
	case 6:
	  month_spec = spec-> jul_run;
	  break;
	case 7:
	  month_spec = spec-> aug_run;
	  break;
	case 8:
	  month_spec = spec-> sep_run;
	  break;
	case 9:
	  month_spec = spec-> oct_run;
	  break;
	case 10:
	  month_spec = spec-> nov_run;
	  break;
	case 11:
	  month_spec = spec-> dec_run;
	  break;
	default:
	  printf("We should never get to this print statementBB\n");
	  SqlDisconnect();
	  exit (ERROR);
	}
      
/*      for(j = 0; j < 12; j++)
	printf("For month %d the code is %d\n",j+1,month_spec[j].most); */
      
      /* the array datasource can now be populated with 12 values (integers) which correspond 
	 to how the data will be processed for this site */
      
      for(j = 0; j < 12; j++)  
	{
	  switch(probability)
	    {
	    case MOST:
	      datasource[j] = month_spec[j].most;
	      break;
	    case MAX:
	      datasource[j] = month_spec[j].max;
	      break;
	    case MIN:
	      datasource[j] = month_spec[j].min;
	      break;
	    default:
	      printf("We should never get to this print statementBB\n");
	      SqlDisconnect();
	      exit (ERROR);
	    }
	}
      
      /* load historical min - most and max probable values for all reservoirs as appropriate
         next we query the database and retrieve historical most probable, max probable or min probable
	 not that we get most probable data regardless of whether this is for a max or a min run.
	 If it is a most probable run get only most prob historical stats.
	 If it is a max probable run get most prob and max prob historical stats.
	 If it is a min probable run get most prob and min prob historical stats. 

	 See the .h file for valus of MODEL_RUN_ID_STATS_*                           */
      
      stats_model_run_id = MODEL_RUN_ID_STATS_MOST;
      
      if ((result = get_monthly_stats_data(site_id,datatype_id,stats_model_run_id,most_hist_stats)) != OK)
	{
	  printf("Error in function get_monthly_stats_data\nDisconnecting\n");
	  SqlDisconnect();
	  exit (ERROR);
	}

/*      for (j=0;j< 12;j++)
	printf("the stats value for site id %d index %d is %f\n",site_id,j,most_hist_stats[j]); */

      if (probability == MAX)
	{
	  stats_model_run_id = MODEL_RUN_ID_STATS_MAX;
	  if ((result = get_monthly_stats_data(site_id,datatype_id,stats_model_run_id,max_hist_stats)) != OK)
	    {
	      printf("Error in function get_monthly_stats_data\nDisconnecting\n");
	      SqlDisconnect();
	      exit (ERROR);
	    }
	}
      
      if (probability == MIN)
	{
	  stats_model_run_id = MODEL_RUN_ID_STATS_MIN;
	  if ((result = get_monthly_stats_data(site_id,datatype_id,stats_model_run_id,min_hist_stats)) != OK)
	    {
	      printf("Error in function get_monthly_stats_data\nDisconnecting\n");
	      SqlDisconnect();
	      exit (ERROR);
	    }
	}
      
      /* load in 998877 for first 11 values in the inflow array 
	 note that the inflow array has 36 items - thus the first 12 correspond to historical data
	 basiscally we are just putting blanks in the first 11.  The 12 (usually last month) is some
	 times needed to interpolation.  Thus we will load historical data for this element (element 11) */


      SqlDateMath(SUBTRACTION,model_run_date,prev_date,1,YEAR); /* use this function to get an sql date of one year previous */
      
      if(fc_type == MID)
	      SqlDateMath(SUBTRACTION,prev_date,prev_date,14,DAY); /* subtract 14 days off - i.e. first of the month */

      for (m=0; m < 12; m++)      /* load in the historical data for the previous 12 months */
	{
	  if ((result = get_monthly_data(site_id,datatype_id,prev_date,&val)) != OK)
	    {
	      printf("Error in function get_monthly_data\nDisconnecting\n");
	      SqlDisconnect();
	      exit (ERROR);
	    }
	  inflow_array[m] = val;

	  SqlDateMath(ADDITION,prev_date,prev_date,1,MONTH); /* use this function increment the date */
	}

      SqlDateMath(ADDITION,model_run_date,model_run_date,0,MONTH);

      /* load the model date into the varible dt */

      strcpy(dt,model_run_date);

      if(fc_type == MID)
	      SqlDateMath(SUBTRACTION,dt,dt,14,DAY); /* subtract 14 days off - i.e. first of the month */

      /* set some test variables to 0 (false) */

      test_aj = 0;
      test_int = 0;
      index_int = 0;
      num_interp = 0;
      apr_jul_indicator = 0;

      index = 12; /* offset by 12 months */

      /* begin the months loop - process 12 months */

      for(m = (run_month - 1); m < (run_month + 11); m++) /* loop through the months - j represents the month */
	{
	  if(m > 11)
	    j = m - 12;
	  else
	    j = m;
	  
	  if (datasource[j] == FORECAST) /* if the value in the driver structure is F then do the next block */
	    {
	      temp_probab = 50;
	      if ((result = get_monthly_model_data(site_id,datatype_id,dt,model_run_date,type,temp_probab,&val,nws_model_id)) != OK)
		{
		  printf("Error in function get_monthly_model_data\nDisconnecting\n");
		  SqlDisconnect();
		  exit (ERROR);
		}
	      
	      inflow_array[index] = val;
	    }
	  else if (datasource[j] == NEIGHBOR)  /* if the value in the driver structure is N then do the next block */
	    {
	      neighbor_site_id = driver[i].neighbor_site_id;
	      neighbor_datatype_id = driver[i].neighbor_datatype_id;
	      if ((result = get_monthly_model_data(neighbor_site_id,neighbor_datatype_id,dt,model_run_date,type,temp_probab,&val,nws_model_id)) != OK)
		{
		  printf("Error in function get_monthly_model_data\nDisconnecting\n");
		  SqlDisconnect();
		  exit (ERROR);
		}
	      
	      /* adjust the value based on the historical ratio of the neighbors and this sites monthly averages 
		 first get the historical most probable values for the neighbor */

	      if ((result = get_monthly_stats_data(neighbor_site_id,neighbor_datatype_id,stats_model_run_id,neighbor_hist_stats)) != OK)
		{
		  printf("Error in function get_monthly_stats_data\nDisconnecting\n");
		  SqlDisconnect();
		  exit (ERROR);
		}

	      inflow_array[index] = val * (most_hist_stats[j] / neighbor_hist_stats[j]); /* value adjusted */
	    }

	  else if (datasource[j] == HISTORICAL)  /* if the value in the driver structure is H then do the next block */
	    {
	      if (probability == MOST)
		inflow_array[index] = most_hist_stats[j];
	      if (probability == MAX)
		inflow_array[index] = max_hist_stats[j];
	      if (probability == MIN)
		inflow_array[index] = min_hist_stats[j];
	    }

	  else if (datasource[j] == MOST_PROBABLE)  /* if the value in the driver structure is H then do the next block */
		inflow_array[index] = most_hist_stats[j];

	  else if (datasource[j] == APR_JUL_DISAGG)  /* if the value in the driver structure is A then do the next block
							note that is this block we only pull the april-jul forecast out
							of the database - the actual monthly values for type 'A' will
							be processed after the monthly loop - the variable apr_jul_indicator
							is a significat variable - it contains a total of all the months 
							needing data from the Apr-Jul period.  I.E if april is needed it
							adds a 4, if may it adds a 5  and so on. */
	    {
	      apr_jul_indicator = apr_jul_indicator + (j + 1); 
	      if (test_aj == 0) /* true the first time we ecounter the 'A' */
		{
		  index_aj = index; /* store the index of the first month needing data from the apr-jul forecast */
		  sprintf(b_dt,"01-apr-%4d\0",run_year);
		  sprintf(e_dt,"01-jul-%4d\0",run_year);
		  
		  if ((result = get_monthly_model_range_data(site_id,datatype_id,b_dt,e_dt,model_run_date,type,probab,&val,nws_model_id)) != OK)
		    {
		      printf("Error in function get_monthly_model_data\nDisconnecting\n");
		      SqlDisconnect();
		      exit (ERROR);
		    }
		  printf("the value of the apr-jul forecast is %f\n",val);
		  apr_jul_val = val;
		  
		  test_aj = 1;  /* tells us we have been here before */
		}
	      inflow_array[index] = -999;
	    }
	  else if (datasource[j] == INTERPOLATE)  /* if the value in the driver structure is I then do the next block
						     note here like the A flag that this routine flags and stores which
						     months need interpolation, however, the actual monthly values will
						     not be processed until after the monthly loop */
	    {
	      if (test_int == 0)
		{		
		  index_int = index; /* store the index of the first month needing interpolation */
		  test_int = 1;
		}
	      num_interp++;
  	    }
	  else
	    {
	      printf("We should never get to this here print statementGG\n");
	      SqlDisconnect();  
	      exit(ERROR);
	    }      
	  
	  SqlDateMath(ADDITION,dt,dt,1,MONTH); /* add one month to the date */
	  index++;  /* increment the counter */
	  
	}	/* end  months loop */

      
      /* process the apr through july values - the function do_apr_jul processes these */

	 if (test_aj == 1)
	   {
	     if ((result = do_apr_jul(most_hist_stats,inflow_array,apr_jul_val,index_aj,apr_jul_indicator)) != OK)
	       {
		 printf("Error in function do_apr_jul\nDisconnecting\n");
		 SqlDisconnect();
		 exit (ERROR);
	       }
	   }

      /* fill in the interpolated values  - the function interpo_late does this */

      if ((result = interpo_late(most_hist_stats,run_month,inflow_array,index_int,num_interp)) != OK)
	   {
	     printf("Error in function interpo_late\nDisconnecting\n");
	     SqlDisconnect();
	     exit (ERROR);
	   }
      
      /* load in historical statistical data from the final 12 values in the inflow_array 
	 the final 12 of the 36 slots in the inflow array are always populated with historical values */

      index = 24;
      /* on the next line the 47 replaces an 11 so the run is for 5 years not 2 */
      for(m = (run_month - 1); m < (run_month + 47); m++) /* loop through the months - j represents the month */
	{

	  j = m;
	  if(j > 11 && j <= 23)
	    j = j - 12;
	  if(j > 23 && j <= 35)
	    j = j - 24;
	  if(j > 35 && j <= 47)
	    j = j - 36;
	  if(j > 47)
	    j = j - 48;
	  

/* the following block was deleted and replace by mearly assigning most probable stats to the out years
   this will simplify the running of the model for out years  in max and min runs having it revert
   back to average hydrology  1/2000 tryan

   if (probability == MOST) 
     inflow_array[index] = most_hist_stats[j];
   if (probability == MAX)
     inflow_array[index] = max_hist_stats[j];
   if (probability == MIN)
     inflow_array[index] = min_hist_stats[j]; */
	  
	  inflow_array[index] = most_hist_stats[j]; /* this replaces the previous block 1/2000 tryan */
	  
	  index++;
	}


      strcpy(dt,model_run_date);

      if(fc_type == MID)
	SqlDateMath(SUBTRACTION,dt,dt,14,DAY); /* subtract 14 days off - i.e. first of the month */
      
      /* load the values into the database - also echo the results to the screen */

      for (kk = 0; kk < 60; kk++) /* 60 replaces a 24 for 5 years 1/2000 tryan */
	{
	  j = kk + run_month - 1;
	  
	  if(j > 11 && j <= 23)
	    j = j - 12;
	  if(j > 23 && j <= 35)
	    j = j - 24;
	  if(j > 35 && j <= 47)
	    j = j - 36;
	  if(j > 47)
	    j = j - 48;

	  if(kk < 12)
	    printf("For date %s the value is %8.0f and the method used was %20s\n",dt,inflow_array[12 + kk],method[datasource[j]]);
	  else
	    printf("For date %s the value is %8.0f and the method used was  Most Probable\n",dt,inflow_array[12 + kk]);

	  val = inflow_array[12 + kk];

	  if ((result = load_monthly_model_data(model_run_id_24mo,site_id,datatype_id,dt,val)) != OK)
	    {
	      printf("Error in function load_monthly_model_data\nDisconnecting\n");
	      SqlDisconnect();
	      exit (ERROR);
	    }	  
	  SqlDateMath(ADDITION,dt,dt,1,MONTH);
	}

    } /* end of loop for all reservoirs */
  
  SqlDisconnect();  
  return(0)  ;
} /* end of main */

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

/**************************************************************************************************************
* do_apr_jul
* Purpose: disaggregates the april through jul volume and puts into appropriate months
*
*             Admitedly, the fuction is rather hardwired - there was not really an elegent method that could
*             have been used.  This function operates on brute force not elegance 
*
*             The function ckecks to see if one of 4 cases, depending on which months need processing,
*             apply.  It then processe within the case block.
* Input
* 	
*       float     *most_hist_stats
*	float     *inflow_array
*       float     apr_jul_val
*       int       index_aj
*	int       apr_jul_indicator
*
*       
* Return
* 	int	OK or ERROR: indicates if count was retrieved
*
*
*************************************************************************************************************/

int do_apr_jul (VALUE *most_hist_stats, float *inflow_array, float apr_jul_val, int index_aj, int apr_jul_indicator)

{

  float        ave,apr_factor,may_factor,jun_factor,jul_factor,adjust;
  
  switch(apr_jul_indicator)
    {
      
    case 22: /* need apr(4), may(5), jun(6), and july(7)    4+5+6+7 = 22  */
      
      ave = (most_hist_stats[3] * 30 + most_hist_stats[4] * 31 + most_hist_stats[5] * 30 + most_hist_stats[6] * 31);
      apr_factor = (most_hist_stats[3] * 30) / ave;
      may_factor = (most_hist_stats[4] * 31) / ave;
      jun_factor = (most_hist_stats[5] * 30) / ave;
      jul_factor = (most_hist_stats[6] * 31) / ave;
      inflow_array[index_aj] = (apr_jul_val * 122 * apr_factor) / 30;
      inflow_array[index_aj+1] = (apr_jul_val * 122 * may_factor) / 31;
      inflow_array[index_aj+2] = (apr_jul_val * 122 * jun_factor) / 30;
      inflow_array[index_aj+3] = (apr_jul_val * 122 * jul_factor) / 31;
      break;
      
    case 18: /* need may(5), jun(6), and july(7)            0+5+6+7 = 18  */
      
      ave = (most_hist_stats[4] * 31 + most_hist_stats[5] * 30 + most_hist_stats[6] * 31);
/*      printf("index aj is %d\n",index_aj);*/
      adjust = (apr_jul_val * 122 - inflow_array[index_aj - 1] * 30);
      may_factor = (most_hist_stats[4] * 31) / ave;
      jun_factor = (most_hist_stats[5] * 30) / ave;
      jul_factor = (most_hist_stats[6] * 31) / ave;
      inflow_array[index_aj] = (adjust * may_factor) / 31;
      inflow_array[index_aj+1] = (adjust * jun_factor) / 30;
      inflow_array[index_aj+2] = (adjust * jul_factor) / 31;
/*      printf("april may june and july are %f %f %f %f\n",inflow_array[index_aj-1],inflow_array[index_aj],inflow_array[index_aj+1],inflow_array[index_aj+2]);*/
      break;
      
    case 13: /* need jun(6), and july(7)                    0+0+6+7 = 13 */
      
      ave = (most_hist_stats[5] * 30 + most_hist_stats[6] * 31);
      adjust = (apr_jul_val * 122 - inflow_array[index_aj - 2] * 30 - inflow_array[index_aj - 1] * 31);
      jun_factor = (most_hist_stats[5] * 30) / ave;
      jul_factor = (most_hist_stats[6] * 31) / ave;
      inflow_array[index_aj] = (adjust * jun_factor) / 30;
      inflow_array[index_aj+1] = (adjust * jul_factor) / 31;
      break;
      
    case 7: /* need only july(7)                            0+0+0+7 =  7   */
      
      ave = (most_hist_stats[6] * 31);
      adjust = (apr_jul_val * 122 - inflow_array[index_aj - 3] * 30 - inflow_array[index_aj - 2] * 31 - inflow_array[index_aj - 1] * 30);
      inflow_array[index_aj] = adjust / 31;
      break;
      
    default:
      printf("We should never get to this print statement JJ\n");
      exit (ERROR);
    }
  return(OK);
}

/**************************************************************************************************************
* interpo_late
* Purpose: interpolates values in the inflow_array
*         
*
*          The interpo_late funtion first determines the percent of normal for the 
*          values that sit on the edges of the interpolation range.  It then interpolates
*          to find how the percentage of normal changes through the period.  It then
*          multiplies these number times the historical averages to get the numbers
*          that need to go into the inflow_array
*
*          Example - say we know that Jul is 65 and that Oct is 20, and that we need to 
*          interpolate to get Aug and Sep.  Lets say that the average for Jul is 50, the
*          average for Aug is 30, the average for Sep is 25 and the average for Oct is 20.
*          First we determine that for Jul 65 / 50 is 130 percent of normal.  Then for 
*          Oct we see that 20 / 20 is 100 percent of normal.  We then interpolate these
*          percentages and get 120 percent of normal for Aug and 110 percent of normal 
*          for Sep.  Then we mulitply by the historic values to get the values to put
*          into the input_array.   For Aug 1.2 * 30 = 36.  For Sep 1.1 * 25 = 27.5. 
*
*          note: this function always uses most probable stats - even for max and min
*          model runs.  This was discussed in operations and decided to be the correct
*          way to do things.
*
* Input
* 	
*       VALUE     *most_hist_stats
*       int       run_month
*	float     *inflow_array
*       int       index_int
*	int       num_interp
*
*       
* Return
* 	int	OK or ERROR: indicates if count was retrieved
*
*
*************************************************************************************************************/
int interpo_late (VALUE *most_hist_stats, int run_month, float *inflow_array,  int index_int, int num_interp)

{

  float        ratio,adjust;
  float        m,b;
  int          i,offset,indexer;

  offset = index_int + run_month - 2;

  if (offset > 11)
    offset = offset - 12;

  if (offset > 11)
    offset = offset - 12;

  if (offset > 11)
    return (ERROR);

  if (offset < 0)
    return(ERROR);

  ratio = inflow_array[index_int - 1] / most_hist_stats[offset];

  /* use the good old template y = mx + b */

  /* figure the slope */

  adjust = inflow_array[index_int -1] * ratio;

  m = (1 - ratio) / (num_interp + 1);

  /* figure the intercept */

  b = 1 - (m * num_interp);

  for(i = 0; i < num_interp; i++)
       {
	 adjust = (m * i) + b;
	 indexer = offset + i + 1;
	 if (indexer > 11)
	   indexer = indexer - 12;
	 inflow_array[i+index_int] = adjust * most_hist_stats[indexer];
       }

  return(OK);
}

