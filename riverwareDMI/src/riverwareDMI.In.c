/* IDENTIFICATION SECTION
   ----------------------

  Module Name: int main(void)  

  File Name: main.c

  Date Written: November, 1995/ July 1997, febr. 1998

  Author(s): 	Terry Fulp - USBR
              Paddy McCarthy - CADSWES
              Carol Marra - CADSWES
              Rene Reitsma - CADSWES
	      Nick Nielsen - CADSWES

  Module Description:	

  Main() routine to get daily data from HDB and write them to ASCII 
         files for input to RiverWare. This is an INPUT DMI for 
  daily data. It writes intermediate ASCII data files to the directory 
  specified by the environment variable:

  $RIVERWARE_DMI_DIR/<dmi_name>.

  If the environment variable is not set, the default is: 

  /tmp/<dmi_name> */

#define MAIN
#define VERSION "1.1"

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* hdb includes */
#include <dbutils.h>
#include <utils.h>
#include <defines.h>
#include <typedefs.h>
#include <ctype.h>

/* application-specific includes */
#include <dmi_utils.h>
#include <dmiGlobals.h>

/*  application stuff  */
#include "dailyDmiIn.h"

#define FUNC_NAME "main"
#define MAX_NUMBER_OF_FILES 100
#define MAX_LENGTH_OF_FILENAMES 500
#define MAX_LENGTH_OF_TIME_SERIES 500
#define INPUTDMI 0

extern TIME_INTERVALS timeIntervals[NUM_TIME_INTERVALS];

int main(int argc, char *argv[])
{
   int i;
   int result;
   int modelId;
   int TkInterfaceNeeded = FALSE;
   char initialDate[50],
      riverwareEndDate[50],
      initialTime[10],
      riverwareEndTime[10];
   char controlFile[MAX_LENGTH_OF_FILENAMES],
      workDir[MAX_LENGTH_OF_FILENAMES];
   char *dbName, *appUser, *appRole;
   char riverware_timestep[20];
   TIME_INTERVAL_TYPES hdb_timestep;
   SQL_INTERVAL sql_time_interval;
   dmi_header_struct *list = NULL;
   dmi_header_struct *previous = NULL; 
   dmi_header_struct *current = NULL; 
   
   /* Get command line arguments and check them
      
      NOTE:  C assumes argv[0] is the name of the program called.
      Riverware will send 8 additional arguments:
      
      argv[1] = filename including full path of RiverWare-supplied control file
      argv[2] = working directory (location of DMI exec and input files to RiverWare)  
      argv[3] = Start Date of the run:  i.e., 1994-10-14
      argv[4] = Start Time of the run:  i.e., 24:00 
      argv[5] = End Date of the run:  i.e., 1994-11-30
      argv[6] = End Time of the run:  i.e., 24:00 
      argv[7] = Timestep of run: i.e., 1DAY 
      argv[8] = Required User Arg: Model_id used to lookup object/slot mapping */

   if((argv[1] != NULL) && (!strcmp(argv[1], "-v")))
   {
      printf("\nriverwareDMI.In version number: %s\n\n",VERSION);
      return(OK);
   }
   
   if (argc != 9)
   {
      printf("\nDMI: ERROR in call to DMI:  argc = %d\n", argc);
      printf("DMI: useage:\n");
      printf("DMI: argv[1] = filename including full path of RiverWare-supplied control file\n");
      printf("DMI: argv[2] = working directory (location of DMI exec and input files to RiverWare)\n");
      printf("DMI: argv[3] = Initial Date of the run:  i.e., 1994-10-14\n");
      printf("DMI: argv[4] = Initial Time of the run:  i.e., 24:00\n");
      printf("DMI: argv[5] = End Date of the run:  i.e., 1994-11-30\n");
      printf("DMI: argv[6] = End Time of the run:  i.e., 24:00 \n");
      printf("DMI: argv[7] = Time Step of the run:  i.e., 1DAY \n");
      printf("DMI: argv[8] = User Arg: Model_id for looking up object/slot mapping \n");
      exit(ERROR);
   }

   RegisterSignalHandler();

   /* Connect to the local database to retrieve mapping info. Then 
      disconnect so that multiple connections can be established in 
      the proper order. */
   
   dbName = NULL;
   dbName = getenv("HDB_LOCAL");
   if (dbName == NULL)
   {
      PrintError("Environment variable HDB_LOCAL not defined...\nExiting...\n");
      exit(ERROR);
   }

   appUser = NULL;
   appUser = getenv("APP_USER");
   if (appUser == NULL)
   {
      PrintError("Environment variable APP_USER not defined...\nExiting...\n");
      exit(ERROR);
   }

   appRole = NULL;
   appRole = getenv("APP_ROLE");
   if (appRole == NULL)
   {
      PrintError("Environment variable APP_ROLE not defined...\nExiting...\n");
      exit(ERROR);
   }

   result = SqlConnectRole(dbName, appUser, dbName, appRole);
   if (result != OK)
   {
      PrintError("Could not connect to local database...\nExiting...");
      exit(ERROR);
   }

   /* No need to set lockmode in Oracle */
   
   /* Am currently on the default (local) session */
   /* must copy arguments to local variables before getting mapping
      
      NOTE:
      argv[3] through argv[6] are from the RiverWare Run Control 
      menu where:
      argv[3] = INITIAL date of the run; ie, the day prior to the start 
      of the simulation for a daily model
      argv[4] = INITIAL time of the run; ie, 24:00 hours for a daily model
      argv[5] = end date of the run
      argv[6] = end time of the run
      argv[7] = time step of the run */
   
   strcpy(controlFile,argv[1]);
   strcpy(workDir, argv[2]);
   strcpy(initialDate, argv[3]);
   strcpy(initialTime, argv[4]);
   strcpy(riverwareEndDate,argv[5]);
   strcpy(riverwareEndTime,argv[6]);
   strcpy(riverware_timestep, argv[7]);
   modelId = atoi (strchr (argv[8], '=') + 1);
   
   /* Get all mappings from the database, including units, 
      object.slot -> SDIs, and DB site list. */
   result = DmiLoadMappings(modelId);
   if (result != OK)
   {
      PrintError ("Problem loading mappings into global arrays...\nExiting...");
      exit(ERROR);
   }
   
   result = SqlDisconnect();
   if (result != OK)
   {
      PrintError ("Problem Disconnecting from the database...\nExiting...");
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
	    PrintError("Could not connect to local database...\nExiting...\n");
	    SqlDisconnectAll();
	    exit(ERROR);
	 }
      }
      
      /* Otherwise, connection succeeded. Set flag indicating successful 
	 connect to session */
      
      else 
      {
	 PrintError("Connected to %s.\n", dbsite_array[i].dbsite_alias);
	 dbsite_array[i].connected = TRUE;

	 /* Don't need to set lockmode in Oracle */
      }
   }
   
   /* Create single date/time strings from input dates and times. */
   
   strcat (initialDate, " ");
   strcat (initialDate, initialTime);
   strcat (riverwareEndDate, " ");
   strcat (riverwareEndDate, riverwareEndTime);
   
   /* Map RiverWare timestep string to HDB timestep and Ingres time 
      interval. */
   
   result = LookupTimestep (riverware_timestep, &hdb_timestep);
   if (result != OK)
   {
      PrintError("No HDB timestep corresponding to RiverWare's %s...\nExiting...", riverware_timestep);
      SqlDisconnectAll();
      exit(ERROR);
   }
   
   strcpy (sql_time_interval, timeIntervals[hdb_timestep].ingresName);
   
   /*  DEBUG prints 
       printf("DMI: dmi name = %s\n",argv[0]);
       printf("DMI: model name = %s\n",argv[0]);
       printf("DMI: RiverWare controlFile = %s\n",controlFile);
       printf("DMI: RiverWare workDir = %s\n",workDir);
       printf("DMI: RiverWare initialDate = %s\n",initialDate);
       printf("DMI: RiverWare endDate = %s\n",riverwareEndDate); */
   
   /* read the input file names from RiverWare control file */
   
   result = readInputControlFile(controlFile, 
                                 &list, 
                                 &TkInterfaceNeeded);
   if(result != OK )
   {
      PrintError("Error...reading input DMI control file: %s...\n", 
                 controlFile);
      SqlDisconnectAll();
      exit(ERROR);
   }

   if (TkInterfaceNeeded)
   {
     result = StartTkInputInterface(argv[0]);
     if (result != OK)
     {
        PrintError("Error...running StartTkInputInterface in %s...\n",
                   FUNC_NAME);
        result = SqlDisconnectAll();
        exit(ERROR);
     }
   }
   else
   {
     printf("Input DMI control file fully specified\n");
     printf("Interface for selecting model_run_id(s) skipped...\n");
   }

   /* modelRunMapList is the global variable that Tcl uses, so we need to 
      set list equal to that so that the rest of the program can use it.  
      Since it is a pointer, and the Tcl functions use the same list, it 
      should be the same still, but just to make sure...*/

   list = *modelRunMapList;

   /* create the maplist from the model_run_ids */

   mapList(&list);

/* Initialize the list with the start and end dates. Both RiverWare 
   and HDB formats. This transforms run control initial date to 
   start date and does format changes. Then sets dates on every 
   member of list. */
  
   result = SetListStartEndDates (initialDate, riverwareEndDate,
				  sql_time_interval, list, INPUTDMI);
   if (result != OK)
   {
      PrintError("DMI: Problem setting model start date for initial date %s...\nExiting...\n", initialDate);
      SqlDisconnectAll();
      exit(ERROR);
   }
   
   current = previous = list;
   while (current) 
   {
      
      /* Set the site_datatype_id for this member */
      
      result = LookupSiteDatatype(current->object_name,
				  current->slot_name,
				  &(current->hdb_site_datatype_id));
      if (result != OK)
      {
	    PrintError("DMI: Could not find site_datatype_id for\n");
	    PrintError("Object: %s Slot: %s\n", current->object_name,
	   	    current->slot_name);
	    PrintError("Ignoring this object.slot.\n");
	 
	    /* Fix up the pointers and delete this item. */
	 
	    result = SetPointersDeleteItem (&current, &list, &previous);
	    if (result != OK)
	    {
	       PrintError("DMI: Error in SetPointersDeleteItem...\nExiting...\n");
	       SqlDisconnectAll();
	       exit(ERROR);
	    }
	    continue;
      }
      
      result = LookupLocation(current->hdb_site_datatype_id,
			      current->data_location);
      if (result != OK)
      {
	    PrintError("DMI: Could not find location for site_datatype_id %d\n",
		    current->hdb_site_datatype_id);
	 
	 /* Fix up the pointers and delete this item. */
	 
	 result = SetPointersDeleteItem (&current, &list, &previous);
	 if (result != OK)
	 {   
	    PrintError("DMI: Error in SetPointersDeleteItem...\nExiting...\n");
	    SqlDisconnectAll();
	    exit(ERROR);
	 }
	 continue;
      }
      
      /* Set the data start dates on the member (based on the object.slot
	 and the model start date), and set the number of timesteps.  */
      
      result = InitializeInputMember (current);
      if (result != OK) 
      {
	 PrintError("DMI: Problem initializing list member for %s.%s\n",
		    current->object_name, current->slot_name);
	 PrintError("This object.slot will be ignored...\n\n");
	 
	 /* Fix up the pointers and delete this item. */
	 
	 result = SetPointersDeleteItem (&current, &list, &previous);
	 if (result != OK) 
	 {
	    PrintError("DMI: Error in SetPointersDeleteItem...\nExiting...\n");
	    SqlDisconnectAll();
	    exit(ERROR);
	 }
	 continue;
      }
      
      PrintMember (current);
      
      /* Get the data for this member */
      
      result = SqlGetDmiData (current, hdb_timestep);
      if (result != OK) 
      {
	 PrintError("DMI: Problem retrieving database data for\n");
	 PrintError("Object: %s Slot: %s\n", current->object_name, 
		    current->slot_name);
	 PrintError("This object.slot will be ignored...\n\n"), 
	    
	    /* Fix up the pointers and delete this item. */
	    
	    result = SetPointersDeleteItem (&current, &list, &previous);
	 if (result != OK)
	 {
	    PrintError("DMI: Error in SetPointersDeleteItem...\nExiting...\n");
	    SqlDisconnectAll();
	    exit(ERROR);
	 }
	 continue;
      }
      
      result = writeDataFile(current);
      if (result != OK)
      {
	 PrintError("DMI: Could not write data file for\n");
	 PrintError("Object: %s Slot: %s\n", current->object_name, 
		    current->slot_name);
	 PrintError("This object.slot will be ignored...\n\n");
	 
	 /* Fix up the pointers and delete this item. */
	 
	 result = SetPointersDeleteItem (&current, &list, &previous);
	 if (result != OK)
	 {
	    PrintError("DMI: Error in SetPointersDeleteItem...\nExiting...\n");
	    exit(ERROR);
	 }
	 continue;
      }
      
      /* move on to the next item in the list regardless of success */
      
      previous = current;
      current = current->next;
   }
   
   /*  disconnect from database  */
   SqlDisconnectAll();
   PrintError("DMI: sucessfully disconnected from DB\n");
   
   exit(OK);
   
} /* end routine */

/*****************************************************************/

int PrintMember  (dmi_header_struct *current)
   
{
   printf ("********************************************\n");
   printf ("datafile: %s\n", current->pr_datafile_name);
   printf ("object_name: %s\n", current->object_name);
   printf ("slot_name: %s\n", current->slot_name);
   printf ("pr_runcontrol_startdate: %s\n", current->pr_runcontrol_startdate);
   printf ("hdb_startdate: %s\n", current->hdb_startdate);
   printf ("pr_runcontrol_enddate: %s\n", current->pr_runcontrol_enddate);
   printf ("hdb_enddate: %s\n", current->hdb_enddate);
   printf ("number timesteps: %d\n", current->number_of_timesteps);
   printf ("hdb_hist_data_enddate: %s\n", current->hdb_hist_data_enddate);
   printf ("hdb_model_data_startdate: %s\n", current->hdb_model_data_startdate);
   printf ("number hist timesteps: %d\n", current->number_of_hist_timesteps);
   printf ("number model timesteps: %d\n", current->number_of_model_timesteps);
   printf ("\n\n");
   
   return (OK);
}


