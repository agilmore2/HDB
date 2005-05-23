/*************************************************************************************************************
 * inflow_24mo  -   creates inflow data for the 24 month study
 *  
 *  written by T Ryan - September 1996

 *             
 **************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include "dbutils.h"
#include "typedefs.h"
#include "utils.h"
#include "defines.h"

/* include file inflow_24mo.h contains key information about the programs behavior */
/* see comments in inflow_24mo.h for more information */

#include "create_model_run_id.h"

#define FUNC_NAME "main"

int main( int argc, char** argv )

{
  char             type[20],prob[20],rolePassword[20];
  int              probability,fc_type,i,j,m,run_month,datasource[12],probab,temp_probab,index_int;
  int              test_aj,test_int,length,result,index,run_year,apr_jul_indicator,day;
  int              index_aj,kk,num_interp;
  float            inflow_array[36],apr_jul_val;
  SQL_DATE         dt,b_dt,e_dt,model_run_date,prev_date;
  ID               site_id,datatype_id,neighbor_site_id,neighbor_datatype_id;
  ID               model_run_id_24mo;
  ID               stats_model_run_id;
  MO_INPUT_SPEC    *spec; /* this is a pointer to a structure of MO_INPUT_SPEC */
  HYDROLOGY        *month_spec; /* this is a pointer to a structure of HYDROLOGY */
  VALUE            val;
  VALUE            most_hist_stats[12],max_hist_stats[12],min_hist_stats[12],neighbor_hist_stats[12];

  if (argc != 5)
    {
      printf("\nusage: inflow_24mo < run_date > < P | M | F > < Most | Max | Min> <app_role_password>\n\n");
      exit(ERROR);
    }

  length = strlen (argv[4]);
  strcpy (rolePassword, argv[4]);
  rolePassword[length] = '\0';
  
  if ((result = SqlConnectRole (DB_NAME,NULL, APP_ROLE, 
				rolePassword)) != OK)
    {
      SqlDisconnect();
      exit (ERROR);
    } 

  strcpy(model_run_date,argv[1]);

  if((result = checkforModelrunid(aop_model_id,model_run_date,probab,type,&model_run_id_24mo)) == 1)
    printf("The model_run_id will be %d\n",model_run_id_24mo);
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
  
  SqlDisconnect();  
  return(0)  ;
} /* end of main */

