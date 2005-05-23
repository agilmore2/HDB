/*****************************************************************************
* DESCRIPTION
*    Formats one page of report for 24-month Flood Control.
*    Several dmi routines are called to retrieve data from the db.
*
* ASSUMPTIONS
*
* INPUT
*    The routines here receive a current date and a FILE * pointer.
*
* OUTPUT
*    
* RETURN STATUS
*    ERROR - Could not create a page of the report.
*    OK - The report page was created successfully.
*
* AUTHOR
* Yue Wu 
* Feb 1997
* Oracle Version:
* Julien Chastang
* August 1998
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"
#include <lc_apps.h>
#include "FormatOutput.h"

extern char *month_b[12];

void AddMiddleSpaces(char * line, int number_of_spaces)
{
  int i;

  for (i = 0; i < number_of_spaces; i++)
  {
    strcat(line, " ");
  }
}

void AddBeginningSpaces(char * line, int number_of_spaces) 
{
  int i;
  
  for (i = 0; i < number_of_spaces; i++) 
    {
      if (i==0)
	strcpy(line, " ");
      else
	strcat(line, " ");
    }
}

int Create24moFldReport (FILE * fp1,
			 char * DateString,
			 int month,
			 int year, 
			 int model_run,
			 int prec) 
{
  Fld24mon values; /*the Fld24mon struct is defined in
		     HDB_applications/globalInclude/lc_apps.h */
  char line[170];
  char buffer[170];
  char tmp[4]; /*6/27/97 chnged from temp[5] to temp[4]*/
  int i,month_temp;
  int jan_flag = 0;
  
  printf("Warning: The program may take some time to complete.\n");
  
  /* composing the title of the report */  
  AddBeginningSpaces(line, 35);
  sprintf(buffer, "BUREAU OF RECLAMATION - CRFS   %d/%d MOST PROB WATER SUPPLY\t", month+1, year);
  strcat(line, buffer);
  sprintf(buffer, "JOB STARTED:%s\n", DateString);
  strcat(line, buffer);
  fprintf(fp1, "%s", line);
  
  AddBeginningSpaces(line, 68);
  sprintf(buffer, "model_run_id = %d\n\n", model_run);
  strcat(line, buffer);
  fprintf(fp1, "%s", line);

  AddBeginningSpaces(line, 58);
  sprintf(buffer, "F L O O D   C O N T R O L   C R I T E R I A\n");
  strcat(line, buffer);
  fprintf(fp1, "%s", line);
  
  AddBeginningSpaces(line, 50);
  sprintf(buffer, "B E G I N N I N G   O F   M O N T H    C O N D I T I O N S\n\n");
  strcat(line, buffer);
  fprintf(fp1, "%s", line);
  

/*     strcpy(line, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
     fprintf(fp1, "%s", line);
*/

  
  /* composing the head row of the report */
  
  sprintf(line, "         |       |      |       |       | UPPER |      |        ||       |      |       |  TOT OR|        |       |       | BOM   | MEAD  |  MEAD |      \n"); /* |      \n");*/
  fprintf(fp1, "%s", line);
  
  sprintf(line, "         |FLAMING| BLUE |       |  LAKE | BASIN | LAKE |        ||FLAMING| BLUE |       |   MAX  |  LAKE  | LAKE  |       | SPACE | SCHED |  FC   | SYS  \n"); /* |  MAX \n"); */
  fprintf(fp1, "%s", line);
  
  sprintf(line, "MON  YEAR| GORGE | MESA | NAVAJO| POWELL| TOTAL | MEAD |  TOTAL || GORGE | MESA | NAVAJO|  ALLOW | POWELL | MEAD  | TOTAL | REQD  |  REL  |  REL  | CONT \n"); /* | CONT \n"); */
  fprintf(fp1, "%s", line);
  
  sprintf(line, "         |  KAF  | KAF  |  KAF  |   KAF |  KAF  | KAF  |   KAF  ||  KAF  | KAF  |  KAF  |   KAF  |  KAF   | KAF   |  KAF  |  KAF  |  KAF  |  KAF  | MAF  \n"); /* |  MAF \n"); */
  fprintf(fp1, "%s", line);
  
  for (i = 0; i < 154; i++)
    {
      if (i==0)
	strcpy(line, "-");
      else
	strcat(line, "-");
    }
  fprintf(fp1, "%s\n", line);
  
  AddBeginningSpaces(line, 20);
  strcat(line, pred);
  AddMiddleSpaces(line, 25);
  if ((month + 1) >= 8 || (month + 1) == 1) 
    /* creditable space starts from aug or jan itself */
    {
      strcat(line, cred);
    }
  if ((month + 1) >= 2 && (month + 1) <= 7) 
    /* effective space starts from jan through jul */
      {
        strcat(line, effe);
      }
  fprintf(fp1, "%s\n", line);
  
  i = 0;
  month_temp = month;
  
  while (i < 24)
    { 
      month_temp = (month + i) % NUM_MONTH;
      if ((month_temp == 0) && (i != 0) && (jan_flag == 0))
	{
	  year += 1;
	}
      
      if (SqlFetch24monFldValues(&values, month_temp, year, i, model_run) 
	  != OK)
	{
	  return(ERROR);
	}
      
      /* compose the format for printout */
      if (month_temp == 0)
	{
	  if (jan_flag == 0) jan_flag = 1;
	  else
	    {
	      jan_flag = 0;
	      AddBeginningSpaces(line, 90);
	      strcat(line, effe);
	      fprintf(fp1, "%s\n", line);
	      i++;
	    }
	}
      else i++;
      
      if (month_temp == 7)
	{
	  AddBeginningSpaces(line, 90);
	  strcat(line, cred);
	  fprintf(fp1, "%s\n", line);
	} 
      
      sprintf(tmp, "%df", prec);
      strcpy(buffer, "%s  %d %7."); /*flgor predicted space */
      strcat(buffer, tmp);
      strcat(buffer, " %6.");  /* blue mesa */
      strcat(buffer, tmp);
      strcat(buffer, " %7.");  /* navajo */
      strcat(buffer, tmp);     
      strcat(buffer, " %8.");  /* Lake Powell */
      strcat(buffer, tmp);     
      strcat(buffer, " %7.");  /* Upper Basin total */
      strcat(buffer, tmp);
      strcat(buffer, " %6.");  /* Lake Mead */
      strcat(buffer, tmp);
      strcat(buffer, " %8.");  /* Total Predicted space */
      strcat(buffer, tmp);
      strcat(buffer, "  %7.");  /* flgor creditable/effetive space */
      strcat(buffer, tmp);
      strcat(buffer, " %6.");  /* blue mesa */
      strcat(buffer, tmp);
      strcat(buffer, " %7.");  /* navajo */
      strcat(buffer, tmp);
      strcat(buffer, " %8.");  /* Total allow */
      strcat(buffer, tmp);
      strcat(buffer, " %8.");  /* Lake Powell */
      strcat(buffer, tmp);
      strcat(buffer, " %7.");  /* Lake Mead */
      strcat(buffer, tmp);
      strcat(buffer, " %7.");  /* Total */
      strcat(buffer, tmp);
      strcat(buffer, " %7.");  /* Bom Space Reqd */
      strcat(buffer, tmp);
      if(values.mead_sched_rel < 0)
	strcat(buffer, "   NULL ");
      else
	{
	  strcat(buffer, " %7.");  /* Mead sched release */
	  strcat(buffer, tmp);
	}
      
      if(values.mead_fc_rel < 0)
	strcat(buffer, "   NULL ");
      else
	{ 
	  strcat(buffer, " %7.");  /* Mead Flood Control release */
	  strcat(buffer, tmp);
	}

      if( values.sys_cont < 0)
	strcat(buffer, "   NULL ");
      else
	{
	  strcat(buffer, " %5.");  /* System Content */
	}
      if (prec < 1) sprintf(tmp, "1f");
      strcat(buffer, tmp);
      strcat(buffer, "\n");
      
      
      if (values.mead_fc_rel >= 0)
	{
	  sprintf(line, buffer, month_b[month_temp],
		  year,
		  values.flgorg_pred_space,
		  values.bluemesa_pred_space,
		  values.navajo_pred_space,
		  values.powell_pred_space,
		  values.upbasin_total,
		  values.mead_pred_space,
		  values.pred_total,
		  values.flgorg_ce_space,
		  values.bluemesa_ce_space,
		  values.navajo_ce_space,
		  values.tot_allow,
		  values.powell_ce_space,
		  values.mead_ce_space,
		  values.total,
		  values.bom_space,
		  values.mead_sched_rel,
		  values.mead_fc_rel,
		  values.sys_cont);
	}
      else
	{
	  sprintf(line, buffer, month_b[month_temp],
		  year,
		  values.flgorg_pred_space,
		  values.bluemesa_pred_space,
		  values.navajo_pred_space,
		  values.powell_pred_space,
		  values.upbasin_total,
		  values.mead_pred_space,
		  values.pred_total,
		  values.flgorg_ce_space,
		  values.bluemesa_ce_space,
		  values.navajo_ce_space,
		  values.tot_allow,
		  values.powell_ce_space,
		  values.mead_ce_space,
		  values.total,
		  values.bom_space,
		  values.mead_sched_rel,
		  values.sys_cont);
	}
      
      fprintf(fp1, "%s", line);
      
    }
  return(OK);
}
