/* IDENTIFICATION SECTION
  	----------------------
  
  	Module Name: int main(void)  
  
  	File Name: main.c
  
  	Date Written: ??, 1997
  
  	Author(s): Paddy McCarthy - CADSWES
  	           Terry Fulp - USBR
               Yue Wu - CADSWES
               Rene Reitsma - CADSWES Febr 1999
               Nick Nielsen - CADSWES
  
  	Module Description:	
  	   Routine to output data from RiverWare to the HDB model table. 
     
       This DMI writes intermediate ASCII data files to the 
       directory specified by the environment variable:
 
               $RiverWare_DMI_DIR/<dmi_name>

        where dmi_name is set by the RiverWare user when the dmi is 
        defined; the default is /tmp/<dmi_name>

    Note: since some dmi executables are shell scripts which must pass
          on the model_run_id written, we write the model_run_id
    written file. The name of the file must get passed in in
    the (optional) -f flag.

*****************************************************************************/

#define MAIN
#define VERSION "1.2"

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* hdb includes */
#include <typedefs.h>
#include <defines.h>
#include <dbutils.h>
#include <utils.h>

/* application-specific includes */
#include <dmi_utils.h>
#include <dmiGlobals.h>
#include "dmiOut.h"

#define MAX_NUMBER_OF_FILES 100
#define MAX_LENGTH_OF_FILENAMES 500
#define MAX_LENGTH_OF_TIME_SERIES 500
#define OUTPUTDMI 1

#define FUNC_NAME "main"
int main(int argc, char *argv[])
{
   FILE *fpout = NULL;
   int result, i;
   int modelId;
   
   char initialDate[50], RiverWareEndDate[50], initialTime[10], 
      RiverWareEndTime[10], controlFile[MAX_LENGTH_OF_FILENAMES],
      workDir[MAX_LENGTH_OF_FILENAMES],
      fileName[500], *char_pointer,
      RiverWareTimeStep[20];
   char *dbName, *appUser, *appRole;
   
   TIME_INTERVAL_TYPES hdb_timestep;
   
   SQL_INTERVAL sql_time_interval;
   
   dmi_header_struct *list = NULL; 
   dmi_header_struct *previous = NULL; 
   dmi_header_struct *current = NULL; 

   int n_slots_for_model_destination, n_slots_for_real_destination;
   int Tkinterface_needed = FALSE;
   int real_output = FALSE;
   int model_output = FALSE;
   int first_model_run_id_spec = TRUE;
   int model_run_id_spec;
   
   /******
     Get command line arguments and check them.
     
     argv[0] is the name of the program called.
     
     RiverWare will send the following 8 arguments, (arg[1]-arg[8]):
     
     argv[1] = filename including full path of RiverWare-supplied control 
     file
     argv[2] = working directory (location of DMI exec and input files 
     to RiverWare)  
     argv[3] = Start Date of the run:  i.e., 1994-10-14
     argv[4] = Start Time of the run:  i.e., 24:00 
     argv[5] = End Date of the run:  i.e., 1994-11-30
     argv[6] = End Time of the run:  i.e., 24:00 
     argv[7] = Timestep of run: i.e., DAY
     argv[8] = Required User Arg: Model_id used to lookup object/slot mapping
     ******/

   if((argv[1] != NULL) && (!strcmp(argv[1], "-v")))
   {
      printf("\nriverwareDMI.Out version number: %s\n\n",VERSION);
      exit(OK);
   }

   if (argc < 9 || !strncmp(argv[8], "-f", 2))
   {
      printf("\nDMI: ERROR in call to DMI:  argc = %d\n", argc);
      printf("DMI: useage:\n");
      printf("options: -v   prints version number\n");
      printf("DMI: argv[1] = filename including full path of RiverWare-supplied control file\n");
      printf("DMI: argv[2] = working directory (location of DMI exec and input files to RiverWare)\n");
      printf("DMI: argv[3] = Initial Date of the run:  i.e., 1994-10-14\n");
      printf("DMI: argv[4] = Initial Time of the run:  i.e., 24:00\n");
      printf("DMI: argv[5] = End Date of the run:  i.e., 1994-11-30\n");
      printf("DMI: argv[6] = End Time of the run:  i.e., 24:00 \n");
      printf("DMI: argv[7] = Time step of run:  i.e., DAY \n");
      printf("DMI: argv[8] = Required User Arg: Model_id for looking up object/slot mapping \n");
      exit(OK);
   }
   
   strcpy(controlFile,argv[1]);
   strcpy(workDir, argv[2]);
   strcpy(initialDate, argv[3]);
   strcpy(initialTime, argv[4]);
   strcpy(RiverWareEndDate,argv[5]);
   strcpy(RiverWareEndTime,argv[6]);
   strcpy(RiverWareTimeStep, argv[7]);
   modelId = atoi (strchr (argv[8], '=') + 1);

   if (argc == 10)
   {
     strcpy(fileName, argv[9]);
     if (fileName[0] == '-' &&
            (fileName[1] == 'f' || fileName[1] == 'F'))
     {

       char_pointer = &(fileName[2]); /* offset of 2 characters */
       fpout = fopen(char_pointer,"w");
       if (fpout == NULL)
       {
         PrintError("Failed to open file for writing model_run_id\n");
         exit(ERROR);
       }
     }
   }

   /* Create date/time strings from input dates and times. */
   
   strcat (initialDate, " ");
   strcat (initialDate, initialTime);
   strcat (RiverWareEndDate, " ");
   strcat (RiverWareEndDate, RiverWareEndTime);
   
   RegisterSignalHandler();

   /* Connect to the local database to retrieve mapping info. Then 
      disconnect so that multiple connections can be established in 
      the proper order. */
   
   dbName = NULL;
   dbName = getenv("HDB_LOCAL");
   if (dbName == NULL)
   {
      PrintError("Environment variable HDB_LOCAL not defined...\n");
      exit(ERROR);
   }

   appUser = NULL;
   appUser = getenv("APP_USER");
   if (appUser == NULL)
   {
      PrintError("Environment variable APP_USER not defined...\n");
      exit(ERROR);
   }

   appRole = NULL;
   appRole = getenv("APP_ROLE");
   if (appRole == NULL)
   {
      PrintError("Environment variable APP_ROLE not defined...\n");
      exit(ERROR);
   }

   result = SqlConnectRole(dbName, appUser, dbName, appRole);
   if (result != OK)
   {
      PrintError("Could not connect to local database...\n");
      exit(ERROR);
   }

   /* Get all mappings from the database, including units, object.slot ->
      SDIs, and DB site list. */
   
   result = DmiLoadMappings(modelId);
   if (result != OK)
   {
      PrintError("Error... loading mappings into global arrays...\n");
      exit(ERROR);
   }

   result = SqlDisconnect(); 
   if (result != OK)
   {
      PrintError("Error...disconnecting from the database...\n");
      exit(ERROR);
      } 
   
   /* Make sure number of db sites does not exceed max number that
      can be handled by the DmiSaveModelData procedure */
   if (number_of_db_sites > max_number_of_db_sites)
   {
      PrintError("Number of DB sites (%d) exceeds max number allowable (%d).\nChange value of max_number_of_db_sites in dmiGlobals.h, recompile, and re-run.", number_of_db_sites, max_number_of_db_sites);
      exit(ERROR);
   }

   /* Go through the dbsites, and only exit if the connection
      to the default site (session 1, element 1 of dbsite_array) isn't
      established. We go backwards as a remnant from Ingres, but Oracle
      does NOT close previously opened connections if there is a subsequent
      failed attempt to open a connection. */
   
   for (i = (number_of_db_sites - 1); i >= 0; i--) 
   {
      result = SqlConnectRoleAt (dbsite_array[i].dbsite_alias,
				 dbsite_array[i].dbsite_db_name,
				 appUser,
				 dbsite_array[i].dbsite_db_name,
				 appRole);
      if (result != OK)
      {
	 PrintError("Could not connect to site database %s\n",
		    dbsite_array[i].dbsite_alias);

	 /* Set the flag for this DB indicating that there is no connection. */
     	 dbsite_array[i].connected = FALSE;
	 
	 /* Exit if the connection failure was on the local, default DB. */
	 if (i == 0)
	   {
	     PrintError("Could not connect to local database.\n");
	     result = SqlDisconnectAll();
	     if (result != OK)
	       PrintError("Error... disconnecting from the Database...\n");
	     exit(ERROR);
	   }
      }

      /* Otherwise, set flag indicating success */ 
      PrintError("Connected to %s.\n", dbsite_array[i].dbsite_db_name);
      dbsite_array[i].connected = TRUE;      

      /* Set nls_date_format for every opened session;
	 each session remains open throughout the run, so one
	 setting is enough. */
      if ((result = SqlSetDateFormatAt ("DD-MON-YYYY HH24:MI:SS")) != OK)
	{
	  PrintError ("%s: Problem setting date format.\n", FUNC_NAME);
	  if (i==0) {
	    PrintError ("%s: Error is at local database, exiting!\n",FUNC_NAME);
	      return (ERROR);
	    }
	  } 
      
	  }  /* end for loop */
   
   /* Am currently on the default (local) session */

   /* Map RiverWare timestep string to HDB timestep and Ingres time interval. */
   
   result = LookupTimestep(RiverWareTimeStep, &hdb_timestep);
   if (result != OK)
   {
      PrintError ("No HDB timestep corresponding to RiverWare's: %s...\n", 
                  RiverWareTimeStep);

      result = SqlDisconnectAll();
      if (result != OK)
         PrintError("Error...disconnecting from the Database...\n");
      exit(ERROR);
   }
   
   strcpy(sql_time_interval, timeIntervals[hdb_timestep].ingresName);
   
   /* Read the output file names from the RiverWare control file */
   
   result = readOutputControlFile(controlFile, &list);
   if(result != OK ) 
   {
      PrintError("DMI:ERROR reading RiverWare control file = %s...\n",
                 controlFile);
      result = SqlDisconnectAll();
      if (result != OK)
         PrintError("Error...disconnecting from the Database...\n");
      exit(ERROR);
   }
   
   /* Initialize the list with the start and end dates -- RiverWare and HDB
      formats. This transforms run control initial date to start date and
      does format changes. Then sets dates on every member of the list. */
   
   result = SetListStartEndDates(initialDate, 
				 RiverWareEndDate,
				 sql_time_interval,
				 list,
                 OUTPUTDMI);
   if (result != OK)
   {
      PrintError("DMI: Problem setting model start date for initial date %s...\n", initialDate);
      result = SqlDisconnectAll();
      if (result != OK)
        PrintError("Error...disconnecting from the Database...\n");
      exit(ERROR);
   }
   
   /* Walk down the list; initialize each member and read the data file. */
   
   current = previous = list;
   n_slots_for_model_destination = n_slots_for_real_destination = 0;

   while (current) 
   {
      /* Initializes some key components of list member */
      
      result = InitializeOutputMember(current);
      if (result != OK)
      {
	    PrintError("DMI: Could not initialize list member for %s.%s\n",
                   current->object_name, current->slot_name);
	    PrintError("The data for this object/slot will not be saved to the database.\n");
	 
	 /* Fix up the pointers. This also frees the excluded member. */
	 
	    result = SetPointersDeleteItem (&current, &list, &previous);
	    if (result != OK) 
	    {
	       PrintError("DMI: Error...in SetPointersDeleteItem...\n");
           result = SqlDisconnectAll();
           if (result != OK)
             PrintError("Error...disconnecting from the Database...\n");
	       exit(ERROR);
	    }
	    continue;
      }

      result = readDataFile(current);
      if (result != OK)
      {
	    PrintError("DMI: Could not process data file for %s.%s\n",
                   current->object_name, current->slot_name);
        PrintError("The data for this object/slot will not be saved to the database.\n");
	 
	 /* Fix up the pointers. This also frees the excluded member. */
	 
	    result = SetPointersDeleteItem (&current, &list, &previous);
	    if (result != OK) 
	    {
	       PrintError("DMI: Error in SetPointersDeleteItem...\n");
           result = SqlDisconnectAll();
           if (result != OK)
             PrintError("Error...disconnecting from the Database...\n");
	       exit(ERROR);
	    }
	    continue;
      }

/* now see if this slot needs the Tk interface. Since the default
   destination in the output DMI == MODEL, we must only check to see 
   if there is a slot that has both a MODEL destination and the 
   model_run_id == 0 (non specified with the 
   !destination=m<model_run_id> flag */

      if (current->destination == MODEL &&
          current->model_destination == UNUSED) /* no model_run_id 
                                                   specified */
        Tkinterface_needed = TRUE; 

/* Also check to see that !destination=r flag(s) do not occur in the
   file with either no !destination flag (==MODEL) or !destination=m
   flags. Such mixing is currently not allowed */

      if (current->destination == MODEL)
         model_output = TRUE;
      else
         real_output = TRUE;
      if (model_output && real_output)
      {
        PrintError("Error... both real and model destination flags in MetaControlFile...\n");
        exit(ERROR);
      }

/* Next, check to see that if all destinations == MODEL, they are
   either all unspecified (get the model_run_id from TkInterface) or they
   are all specified identically; i.e., all current->model_destinations
   are identical. Also check to see if the specified model_run_id
   is valid; i.e., that it already exists (cannot create a new
   model_run_id without the Tk interface) */

     if (current->destination == MODEL)
     {
       if (first_model_run_id_spec)
       {
         model_run_id_spec = current->model_destination;
         if (model_run_id_spec != UNUSED)
         {
           result = SqlChkModelRunID(model_run_id_spec);
           if (result != OK)
           {
             PrintError("Error...model_run_id (%n specified in MetaControlFile does not exist...\n",model_run_id_spec);
             exit(ERROR);
           }
         }
         first_model_run_id_spec = FALSE;
       }
       else /* at least one model_run_id_spec exists */
         if (current->model_destination != model_run_id_spec)
         {
           PrintError("Error...not all model_run_ids are either unspecified or specified identically in MetaControlFile...\n");
           exit(ERROR);
         }
     }

     previous = current;
     current = current->next;
   } /* end while(current) */

   /* If needed, get the model_run_id through the Tk interface*/

   if (Tkinterface_needed) /* run the interface and check the result */
   {
     result = StartTkOutputInterface(argv[0]);
     if (result == ERROR)
     {
       result = SqlDisconnectAll();
       if (result != OK)
         PrintError("Error...disconnecting from the Database...\n");
       exit(ERROR);
     }

   /* test to see if user quit the interface(model_run_id == USER_QUIT) */

     if (model_run_id == USER_QUIT)
     {
       if (fpout != NULL) /* file was opened */
       {
         fprintf(fpout,"%d",USER_QUIT);
         fclose(fpout);
       }
       exit (OK);
     }
   }
   else /* Tkinterface_needed == FALSE; check to see if a model_run_id
           was specified with the !destination=m flag. If so, set the
           global model_run_id to model_run_id_spec */
   {
     if (model_output == TRUE) /* all slots to m_table */
       model_run_id = model_run_id_spec;

     printf("Output DMI control file fully specified\n");
     printf("Interface for selecting model_run_id(s) skipped...\n");
   }


   /* Got all the info we need, now save the data to HDB */

   current = list;
   while (current)  /* save each object.slot combination separately.
                       Use the current->destination or model_run_id
                       to decide where/how to store the results */
   {
     if (current->destination == MODEL)
     {
           result = DmiSaveModelData(current);
	   if (result != OK) /* print an error but continue trying to save
                            the remaining slots */
       	     PrintError("Error... saving data for:\nObject: %s\nSlot: %s\n",
                    current->object_name,
                    current->slot_name); 
     }
     else /* current->destination == REAL */
     {
       result = DmiSaveRealData(current);
       if (result != OK) /* print an error but continue trying to save
                           the remaining slots */
         PrintError("Error... saving data for:\nObject: %s\nSlot: %s\n",
                    current->object_name,
                    current->slot_name);
     }

     current = current->next;
   } /* end while(current) */

   /* After processing all site_datatype_ids, make one final
      pass through the model data table: Delete data for this model
      run that is for site_datatype_ids not represented in this
      model run output. Note that this is done only for data
      that is written to model tables, not to real tables. */
   DmiDeleteObsoleteModelSDIs (list);
      
/* Almost done, only thing remaining is to update the sys_date field 
   in the ref_model_run table. Yes, in case the model_run_id
   was newly created, it is already there, but just forcing it here
   is the easiest way to do it. Note that this solution will not work
   in case, in the future, people are indeed allowed to write to
   multiple model_run_ids. In that case, the update must happen
   elsewhere. However, only call it when the data were written
   to the ref_model_table */

   if (model_output)
   {
     result = SqlUpdateSys_date(model_run_id);
     if (result != OK)
     {
        PrintError("Error...updating the sys_date field...\n");
        PrintError("   Note that all data have been comitted...\n");
        exit(ERROR);
     }
   }
   
   /*  Disconnect from the database. */
   
   result = SqlDisconnectAll();
   if (result != OK)
   {
      PrintError("Error...disconnecting from the Database...\n");
      exit(ERROR);
   }

/*  Now write the model_run_id to the file pointed to by out_fp
    (if passed in through -f flag and opened).
    We must do this as UNIX does not support an exit status > 255
    whereas model_run_ids have no limit. Also, in case 
    all data where stored to the r_tables, write a -1 (as in the
    case of user quit the TkInterface */

   if (fpout != NULL) /* file was opened */
   {
     if (model_output)
       fprintf(fpout,"%d\n",model_run_id);
     else
       fprintf(fpout,"%d\n",USER_QUIT);
     fclose(fpout);
   }

   exit (OK);
} /* end main */

