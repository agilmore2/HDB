/*****************************************************************************
* DESCRIPTION
* This file contains the three functions used to get and set the date
* values on the dmi header structure. It also contains some operations
* used to maintain the list of header members.
*
****************************
* SetListStartEndDates
*
* Purpose: This function determines the appropriate start date of the data
* (to be loaded from the database) for the member passed in (based on
* object.slot), and then sets the start dates, end dates, and number
* of time steps on every member of the list accordingly.
*
* Input
*
*
* Output
*
* Return
* OK or ERROR
*
****************************
* InitializeOutputMember
*
* Purpose:
*   This function initializes a member of the Header List for the Output
*   DMI. At the moment it only looks up the site_datatype_id for the
*   object/slot.
*
****************************
* InitializeInputMember
*
* Purpose: This function determines the appropriate date to begin the data time
*   series for the indicated site/datatype. Currently set to one week
*   previous to startDate; eventually will depend on the site_datatype_id.
*
*   Also calculates start and end dates for the historical and model data 
*     intervals and initializes these components of the list member.
*
*   Also sees if the RiverWare units specified in the metaControl file are
*     appropriate for the site_datatype_id, and if not, determines the 
*     RiverWare units appropriate for the site_datatype_id.
*
* Input
*
*
* Output
*
* Return
* OK or ERROR
*
* COMMENTS
*
* AUTHOR - EDITORS
* Carol Marra, Paddy McCarthy, December 1995
*****************************************************************************/
/*  standard stuff  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  hdb associated stuff - need the lib dir specified by -I flag  */
#include <dbutils.h>
#include <utils.h>
#include <dmi_utils.h>
#include <defines.h>
#include <typedefs.h>
#include <ctype.h>

/*  hdb dmi stuff  */
#include <dmi_utils.h>

#define FUNC_NAME "SetListStartEndDates"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SetListStartEndDates

<P> <STRONG>Function signature:</STRONG> int SetListStartEndDates (char *, char *, char *, dmi_header_struct *, int)

<P> <STRONG>Function description:</STRONG> Set the start and end dates for each objects/slot to be processed by the DMI.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *initialDate:       Riverware initial date (one timestep before start date of model run). In format yyyy-mm-dd.
<BR><DD> char *riverwareEndDate:      Riverware end date of the model run. In format yyyy-mm-dd.
<BR><DD> char *timestep:          Timestep of the model run.
<BR><DD> dmi_header_struct *list: Array of object/slots to be processed by the DMI, plus associated information.
<BR><DD> int dmi_type:          0 for input DMI, 1 for output DMI.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SetListStartEndDates (char *initialDate,
                          char *riverwareEndDate,
                          char *timestep,
                          dmi_header_struct * list,
                          int dmi_type)
{
   double num_rec;
   int input_dmi = 0;
   int result;
   SQL_DATE hdbInitialDate,
            hdbStartDate,
            hdbEndDate;
    char riverwareStartDate[50];
    dmi_header_struct * current;

   /*  Convert RiverWare start date to data base date, then add one timestep to get
       model start date. */
   if (riverwareToHdbDate(initialDate, hdbInitialDate, timestep) != OK)
     {
       return (ERROR);
     }

   /* If we're running an input DMI, we must add 1 timestep to
      the initial date in order to get the start date. We don't do 
      this for an output DMI because we want the initial date to be 
      output to the database */

   if (dmi_type == input_dmi)
   {
     result = SqlDateMath(ADDITION, hdbInitialDate, hdbStartDate, 1,
                          timestep);
     if (result != OK)
     {
       PrintError("Error... adding 1 timestep to hdbInitialDate in SetListStartEndDates()...\n");
       return (ERROR);
     }
   }
   else /* just copy initial_date into start date */
     strcpy(hdbStartDate,hdbInitialDate);
    

   /* Now convert the start date (one day after initial date) back to RiverWare
      format. This is used to compare with the date in the data file to
      see if date processing is necessary when that date is read. */
   if (hdbToRiverWareDate(hdbStartDate, riverwareStartDate, timestep) != OK)
     {
       return (ERROR);
     }

   /*  Convert RiverWare end date to data base date */
   if (riverwareToHdbDate(riverwareEndDate, hdbEndDate, timestep) != OK)
     {
       return (ERROR);
     }

    /* Compute total number of records to be written */
    if (SqlNumIntervalsBetweenDates(hdbStartDate, hdbEndDate, timestep, 
				    &num_rec)
        != OK) {
        PrintError ("Problem getting total number of records to be written for dates %s to %s.\n", hdbStartDate, hdbEndDate);
        return (ERROR);
    }


    current = list;
    while(current) {
        strcpy(current->hdb_startdate, hdbStartDate);
        strcpy(current->hdb_enddate, hdbEndDate);
        strcpy(current->pr_runcontrol_startdate, riverwareStartDate);
        strcpy(current->pr_runcontrol_enddate, riverwareEndDate);
        current->number_of_timesteps = (int)num_rec;
        strcpy(current->timestep, timestep);

        current = current->next;
    }

   return (OK);
}

/***************************************************************************/

#define FUNC_NAME "InitializeOutputMember"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> InitializeOutputMember

<P> <STRONG>Function signature:</STRONG> int InitializeOutputMember (dmi_header_struct *)

<P> <STRONG>Function description:</STRONG> Set known values on the current member of the list (SDI, location), and null out the remaining, unknown values.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> dmi_header_struct *current: Object/slot member to be initialized.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int InitializeOutputMember (dmi_header_struct *current)
{

    if (LookupSiteDatatype(current->object_name,
                           current->slot_name,
                           &(current->hdb_site_datatype_id)) != OK) {
        PrintError("DMI: Could not find site datatype id for\n\tObject: %s\n\tSlot: %s\n",
                    current->object_name,
                    current->slot_name);
        return(ERROR);
    }

    /* Set data location for this sdi. */
    if (LookupLocation(current->hdb_site_datatype_id,
		       current->data_location) != OK) 
      {
	PrintError("DMI: Could not find location for site_datatype_id %d.\n",
		   current->hdb_site_datatype_id);
	return (ERROR);
      }

    current->hdb_hist_data_enddate[0] = '\0';
    current->hdb_model_data_startdate[0] = '\0';
    current->number_of_hist_timesteps = 0;
    current->number_of_model_timesteps = 0;
    
    return (OK);
  }

/***************************************************************************/

#define FUNC_NAME "InitializeInputMember"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> InitializeInputMember

<P> <STRONG>Function signature:</STRONG> int InitializeInputMember (dmi_header_struct *)

<P> <STRONG>Function description:</STRONG> Calculate date, timestep and unit information for the current object/slot, and set the values on the member.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> dmi_header_struct *current: Object/slot member to be initialized.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int InitializeInputMember (dmi_header_struct *current)
{
  SQL_DATE hdbDataStartDate;
  double num_rec;
  double num_model;
  SQL_DATE hdb_model_run_startdate;
  SQL_DATE hdb_hist_data_enddate;
  char * pr_units = NULL;

    /*
    ** Convert the RiverWare model run start date to hdb format
    */
    if (riverwareToHdbDate(current->pr_runcontrol_startdate,
                       hdb_model_run_startdate, current->timestep) != OK) {
        PrintError("DMI: Unable to convert run control start date to hdb format.\n");
        return(ERROR);
    }

  /* Initialize start date which indicates number of hist values to be
     retrieved. This returns an HDB-ish (numeric) date. This really should
     be done at the list level since it does not vary by SDI, but for now
     we'll leave it. */
    if (GetDataStartDate(current->hdb_site_datatype_id,
			 current->number_of_hist_timesteps,
			 current->timestep,
                         hdb_model_run_startdate,
                         hdbDataStartDate) != OK) {
        PrintError ("Problem getting data start date for site_datatype_id: %d\n",
                     current->hdb_site_datatype_id);
        return (ERROR);
    }
    strcpy(current->hdb_startdate, hdbDataStartDate);

    /* Compute total number of records to be written */
    if (SqlNumIntervalsBetweenDates(hdbDataStartDate, current->hdb_enddate,
				    current->timestep, &num_rec)
        != OK) {
        PrintError ("Problem getting total number of records to be written for dates %s to %s.\n", hdbDataStartDate, current->hdb_enddate);
        return (ERROR);
    }

    /*
    ** Set the components of the mList member to the appropriate values 
    */
    current->number_of_timesteps = (int)num_rec;

    /*
    ** Now calculate the dates and timesteps for the hist and model data.
    **   To break the data into two intervals, need a date when the
    **     hist data ends and a date when the model data starts.
    */

    /*
    ** The model run start date is the first day of model data.
    */
    strcpy(current->hdb_model_data_startdate, hdb_model_run_startdate);

    /*
    ** Subtract one timestep to get the end of the hist data interval
    */
    if (SqlDateMath (SUBTRACTION,
                     hdb_model_run_startdate,
		     hdb_hist_data_enddate,
                     1,
                     current->timestep) != OK) {
        PrintError("DMI: Unable to decrement run control start date.\n");
        return (ERROR);
    }
    /* REMOVED call to charToNumeric here (tmpDate, hdb_hist_data_enddate) */

    strcpy(current->hdb_hist_data_enddate, hdb_hist_data_enddate);

    /*
    ** Finally, calculate the number of time steps in the 
    **   model interval; hist was set in readControlFile.
    */
    if (SqlNumIntervalsBetweenDates(current->hdb_model_data_startdate,
				    current->hdb_enddate,
				    current->timestep,
				    &num_model) != OK) {
        PrintError ("DMI: Unable to get number of days between %s and %s.\n",
                     current->hdb_model_data_startdate, current->hdb_enddate);
        return (ERROR);
    }

    if ( (current->number_of_hist_timesteps + (int)num_model) 
	!= current->number_of_timesteps) 
      {
        PrintError("DMI: Timesteps don't add up.\n");
        return(ERROR);
      }

    current->number_of_model_timesteps = (int)num_model;

    /*
     * Check to see if the RiverWare units agree with the units
     *   in which data for the site_datatype_id are stored.
     */
    if (DmiVerifyUnits(current->pr_units,
                       current->hdb_site_datatype_id) != OK) {
        PrintError("DMI: The units check failed for\nObject: %s\nSlot: %s\nUnits: %s\nLoading default units for  this object/slot\n",
                    current->object_name,
                    current->slot_name,
                    (current->pr_units ? current->pr_units : "NULL") );

        if (DmiLookupRiverWareUnitsForSDI(current->hdb_site_datatype_id,
                                      &pr_units) != OK) {
            PrintError("Could not determine RiverWare units for site_datatype_id: %d\nCannot process this object/slot.",
                       current->hdb_site_datatype_id);
            return(ERROR);
        }
        else {
            /*
            ** Set the units in the member -- these are the units sent to data file
            */
            if (current->pr_units) { /* initialized before, but to inapproriate unit */
                free(current->pr_units);
                current->pr_units = NULL;
            }
            current->pr_units = pr_units;
        }
    }
    
    return (OK);
}

/***************************************************************************/

#define FUNC_NAME "GetDataStartDate"  
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetDataStartDate

<P> <STRONG>Function signature:</STRONG> int GetDataStartDate (ID, int, char *, SQL_DATE, SQL_DATE)


<P> <STRONG>Function description:</STRONG> Determine first date for which to grab data from HDB, based on the start date of the model and the number of historical timesteps of data required.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> ID siteDatatypeId:         SDI for which data start date is being set. Currently unused.
<BR><DD> int numHistTimesteps:      Number of historical timesteps of data required.
<BR><DD> char *timestep:            Timestep of model.
<BR><DD> SQL_DATE hdbStartDate:     Start data of model, in HDB format. (dd-mon-yyyy hh24:mi:ss)
<BR><DD> SQL_DATE hdbDataStartDate: Start date for data, in HDB format. (dd-mon-yyyy hh24:mi:ss)
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int GetDataStartDate (ID siteDatatypeId, int numHistTimesteps,
		      char *timestep, SQL_DATE hdbStartDate, 
		      SQL_DATE hdbDataStartDate)
{
  /* NOTE: this could be done based on SDI, but as of now it's at model
     level; every SDI has same hdbDataStartDate. */

  /* Set the data start date. */
  if (SqlDateMath (SUBTRACTION, hdbStartDate, hdbDataStartDate,
		   numHistTimesteps, timestep) != OK)
    {
      PrintError ("Problem subtracting %d %ss from model start date %s.\n",
                  numHistTimesteps, timestep, hdbStartDate);
      return (ERROR);
    }

  /* REMOVED call to charToNumeric here (tmpDate, hdbDataStartDate) */

  return (OK);
}

/***************************************************************************/

#define FUNC_NAME       "FreeDmiHeaderMember"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> FreeDmiHeaderMember

<P> <STRONG>Function signature:</STRONG> int FreeDmiHeaderMember(dmi_header_struct *)


<P> <STRONG>Function description:</STRONG> Free all elements of the current member.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> dmi_header_struct *header_member: Current member to be freed.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int FreeDmiHeaderMember(dmi_header_struct * header_member)
{

    if (!header_member)
        return(ERROR);

    if (header_member->data) {
       free(header_member->data);
       header_member->data = NULL;
    }
    if (header_member->pr_datafile_name) {
       free(header_member->pr_datafile_name);
       header_member->pr_datafile_name = NULL;
    }
    if (header_member->object_name) {
       free(header_member->object_name);
       header_member->object_name = NULL;
    }
    if (header_member->slot_name) {
       free(header_member->slot_name);
       header_member->slot_name = NULL;
    }
    if (header_member->pr_units) {
       free(header_member->pr_units);
       header_member->pr_units = NULL;
    }

    free(header_member);
    return(OK);
}

/***************************************************************************/

#define FUNC_NAME       "SetPointersDeleteItem"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SetPointersDeleteItem

<P> <STRONG>Function signature:</STRONG> int SetPointersDeleteItem (dmi_header_struct **, dmi_header_struct **, dmi_header_struct **)

<P> <STRONG>Function description:</STRONG> Remove an item from the list and reconnect the still wanted members.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> dmi_header_struct **current:  Current member of the list (to be deleted). 
<BR><DD> dmi_header_struct **list:     List header.
<BR><DD> dmi_header_struct **previous: Previous member of list.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int SetPointersDeleteItem (dmi_header_struct **current,
                           dmi_header_struct **list,
                           dmi_header_struct **previous)


{
  dmi_header_struct *cur, *prev, *item_to_delete;
 
  cur = *current;
  prev = *previous;

  if (cur == *list)
    /* at head of list, must move head */
    {
      prev = *list = cur->next;
      item_to_delete = cur;
      cur = cur->next;
    }
  /* not at head of list */
  else
    {
      prev->next = cur->next;
      /* do not move previous forward --> it is still correct */
      item_to_delete = cur;
      cur = cur->next;
    }
 
  /* Free the excluded member. */
  if (FreeDmiHeaderMember(item_to_delete) != OK)
    {
      PrintError("DMI: Could not free member of header list.\n");
      exit (1);
      cur = cur->next;
    }

  item_to_delete = NULL;
  *current = cur;
  *previous = prev;

  return (OK);
}


