/****************************************************************************
**
**	IDENTIFICATION SECTION
**	----------------------
**
**	Module Name:	int readInputControlFile(char *controlFile, 
**                          dmi_header_struct **list,
**                          int *TkInterfaceNeeded) 
**
**	Module Name:	int readOutputControlFile(char *controlFile, 
**                          dmi_header_struct **list) 
**
**	File Name:	readControlFile.c
**
**	Date Written:	November, 1995
**
**	Author(s): 	Terry Fulp
**              Paddy McCarthy
**              Rene Reitsma (febr. 1999) 
**
**	Module Description:	
**	routine to read RiverWare DMI metaControl file and parse out the 
**	  file names that are used to transfer the data,
**        the object and slot names for each file,
**	  and the units and scale factors for each object.slot
**		
**      Variables Passed In:  
**         char *controlFile = pointer string containing the controlFile name 
**              (string includes the complete path of the file)  
**         dmi_header_struct **list = pointer to the head of the linked list
**              of object.slot data 
**         int *TkInterfaceNeeded = flag to be set by the routine to
**              indicate whether or not the Tk Interface must be run.
**
**      Comments:  
**        This routine makes the following assumptions concerning RiverWare DMI'S:
**        a separate ASCII file is used for each object.slot combination
**        object.slot names can contain spaces
**        filenames will not contain spaces (underscores are substituted)
**	  
**	  This routine also uses a linked list structure defined in 
**          $HDB_ENV/globalInclude/dmi_utils.h, where $HDB_ENV 
**          is the root directory for HDB development environment
**
**        If a line is not readable, that obj/slot combo is dropped and 
**          a message is posted regarding the unreadability of the line.
**          The routine then continues to process other lines in the file.
**          Data will not be saved to the DB if
**          a row cannot be read from the control file.
**
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*  hdb apps include file to define OK and ERROR  */
#include <dbutils.h>
#include <utils.h>
#include <defines.h>
#include <typedefs.h>

/*  hdb dmi stuff */
#include <dmi_utils.h>

#define BUFFSIZE 700

#define FUNC_NAME "GetObjAndSlotName"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetObjAndSlotName

<P> <STRONG>Function signature:</STRONG> int GetObjAndSlotName (char*, char*, char*)

<P> <STRONG>Function description:</STRONG> Parse out the Riverware object and slot names from a line (of the control file).

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *line: The line from the control file.
<BR><DD> char *line: The object name; set in function.
<BR><DD> char *line: The slot name; set in function.
</DL>

<P> <STRONG>Function return value:</STRONG> Object name or NULL

</HTML>
*/
int GetObjAndSlotName(char * line, char* obj_name, char* slot_name)
{
   char * colSpace = ": ";
   char * obj = "obj";
   char * space = " ";
   char * payback = "Payback";
   char * obj_slot_name_start,
      * obj_slot_name_end,
      * colon_space,
      * rightmostPeriod,
      * leftmostPeriod,
        objTypeName[OBJECTTYPE_NAME_LENGTH],
      * objTypeStart,
      * objTypeEnd;
   int len;
   
   /*
    * Name is first thing in line -- walk past any spaces.
    */
   obj_slot_name_start = line;
   while (isspace (*obj_slot_name_start))
      obj_slot_name_start++;
   if (!obj_slot_name_start) {
      PrintError("DMI: No object name.\n");
      return(ERROR);
   }
   
   /*
    * search for the first occurrence of a ': '
    */
   colon_space = strstr(line, colSpace);
   if (!colon_space) {
      PrintError("DMI: Could not determine object name (colon_space missing).\n");
      return(ERROR);
   }
   
   
   /*
    * colon_space is the end of the object.slot name
    */
   obj_slot_name_end = colon_space;
   if (!obj_slot_name_end) {
      PrintError("DMI: No object name in line (obj_slot_name_end not found).\n");
      return(ERROR);
   }
   
   
   /*
    * Walk back to the rightmost period; if that is the only period, it
      separates the obj name from the slot name. If there is another,
      must determine if middle portion belongs with object name or slot name
    */
   rightmostPeriod = obj_slot_name_end;
   while ( (*rightmostPeriod != '.') && (rightmostPeriod >= obj_slot_name_start) )
      rightmostPeriod--;
   if (rightmostPeriod == obj_slot_name_start) { /* if there was no period, rightmostPeriod = obj_slot_name_start */
      PrintError("DMI: No delimiter between object and slot name in line:\n%s\n",
		 line);
      return(ERROR);
   }

   /* Now see if there's another period. */
   leftmostPeriod = rightmostPeriod-1;
   while ( (*leftmostPeriod != '.') && (leftmostPeriod >= obj_slot_name_start) )
      leftmostPeriod--;

   /* if there is only one period, can set object and slot name now */
   if (leftmostPeriod == obj_slot_name_start)
   {
      /*
       * Calculate the length of the object name.
       */
      len = rightmostPeriod - obj_slot_name_start;
   
      /*
       * copy the object name and NULL terminate
       */
      strncpy(obj_name, obj_slot_name_start, len);
      obj_name[len] = '\0';


      /*
       * Calculate the length of the slot name.
       */
      len = obj_slot_name_end - rightmostPeriod - 1;
   
      /*
       * copy the slot name to that location and NULL terminate
       */
      strncpy(slot_name, rightmostPeriod+1, len);
      slot_name[len] = '\0';
   }
   else
     {
       objTypeStart = strstr(line, obj);
       objTypeStart += 4; /* skip over "obj=" */
       objTypeEnd = strstr (objTypeStart, space);
       len = objTypeEnd - objTypeStart;
       strncpy (objTypeName, objTypeStart, len);
       objTypeName[len] = '\0';

       /* If not a payback, structure is obj.tableSlot.slot */
       if (strncmp (objTypeName, payback, len))
	 {
	   len = leftmostPeriod - obj_slot_name_start;
	   strncpy (obj_name, obj_slot_name_start, len);
	   obj_name[len] = '\0';

	   len = obj_slot_name_end - leftmostPeriod - 1;
	   strncpy (slot_name, leftmostPeriod + 1, len);
	   slot_name[len] = '\0';
	 }
       /* otherwise, payback structure is exchange.supply.slot */
       else
	 {
	   len = rightmostPeriod - obj_slot_name_start;
	   strncpy (obj_name, obj_slot_name_start, len);
	   obj_name[len] = '\0';

	   len = obj_slot_name_end - rightmostPeriod - 1;
	   strncpy (slot_name, rightmostPeriod + 1, len);
	   slot_name[len] = '\0';
	 }
     }
   
   return(OK);
}


#define FUNC_NAME "GetToken"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetToken

<P> <STRONG>Function signature:</STRONG> char * GetToken(char *, char *)

<P> <STRONG>Function description:</STRONG> Finds the value of the indicated keyword in the line (from the control file).

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *line:    The line from the control file.
<BR><DD> char *keyword: The keyword whose value is to be found.
</DL>

<P> <STRONG>Function return value:</STRONG> Desired token or NULL.

</HTML>
*/
char * GetToken(char * line, char * keyword)
{
   char * token,
      * token_start,
      * token_end,
      * keyword_location;
   int len;
   
   /*
    * search for the keyword.
    */
   keyword_location = strstr(line, keyword);
   if (!keyword_location) {
      /* PrintError("DMI: Keyword '%s' not found in control file.\n", keyword); */
      return(NULL);
   }
   
   /*
    * Set the token_start to the first char after the keyword
    */
   token_start = keyword_location + strlen(keyword);
   if (!token_start) {
      PrintError("DMI: Could not locate beginning of token for '%s'.\n", keyword);
      return(NULL);
   }
   
   /*
    * The token ends at the next space character. Search for it.
    */
   token_end = strchr(token_start,' ');
   if (!token_end) {
      token_end = strchr(token_start,'\n');
      if (!token_end) {
	 PrintError("DMI: Could not locate end of token for '%s'.\n", keyword);
	 return(NULL);
      }
   }
   
   /*
    * Calculate the length of the token.
    */
   len = token_end - token_start;
   if (len <= 0) {
      PrintError("DMI: Token for '%s' has length %d.\n", keyword, len);
      return(NULL);
   }
   
   /*
    * allocate memory for the slot token string,
    *   including for the NULL terminator
    */
   token = (char *) malloc(len+1);
   if (!token) {
      printf("DMI: Failure to allocate memory for File Name\n");
      return(NULL);
   }
   
   /*
    * copy the file token to that location and NULL terminate
    */
   strncpy(token, token_start, len);
   token[len] = '\0';
   
   return(token);
}

#define FUNC_NAME "readInputControlFile"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> readInputControlFile

<P> <STRONG>Function signature:</STRONG> int readInputControlFile(char *, dmi_header_struct **)

<P> <STRONG>Function description:</STRONG> Reads DMI input control file, parsing out objects, slots and keywords and loading the dmi_header_struct list with values from the file.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *controlFile:        Name of input control file to read.
<BR><DD> dmi_header_struct **list: Array of object/slot specifications. Output.
<BR><DD> int *TkInterfaceNeeded: set to TRUE or FALSE depending on
whether the Tk Interface must be run. 
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int readInputControlFile(char *controlFile, dmi_header_struct **list,
                         int *TkInterfaceNeeded)
   
{
   FILE *fptr;
   char buff[BUFFSIZE];
   char obj_name[OBJECT_NAME_LEN + 1],
        slot_name[SLOT_NAME_LEN + 1],
      *file_name,
      *unit_descriptor,
      *load_model_data_flag,
      *char_hist_timesteps,
      *char_hist_source,
      *char_model_source;
   dmi_header_struct *current, *new;
   
   int num_hist_timesteps,
      hist_source,
      model_source = 0;
   int i, result, counter;
   
   /* Initialize head of linked list to NULL */
   
   *list = NULL;
   counter = 1;
   strcpy(modelRunMap[0].code, "DEF\0");

  /* open metaControl file */

   if((fptr = fopen(controlFile,"r")) == NULL)
   {
      printf("DMI: Failure to open RiverWare control file %s\n",controlFile);
      return(ERROR);
   }
   printf("DMI: RiverWare control file opened successfully:\n %s\n", controlFile);

   /* read each line of file and get the object.slot and filename */

   *TkInterfaceNeeded = FALSE;
 
   while(fgets(buff,BUFFSIZE,fptr) != NULL) 
   { 
      /* Get the object name.
         This requires a special routine b/c there can be multiple colons
         and multiple periods in an object/slot name.  */

      GetObjAndSlotName(buff, obj_name, slot_name);
      if (!obj_name) 
      {
	    PrintError("DMI: Could not determine object name from line:\n %s\n",
		           buff);
	    continue;
      }
      
      if (!slot_name) 
      {
	    PrintError("DMI: Could not determine slot name from line:\n %s\n",
		           buff);
	    obj_name[0] = '\0';
	    continue;
      }
      
      /* Get the file name.
         This is a mandatory item. Free the header if not found.  */

      file_name = GetToken(buff, "file=");
      if (!file_name) 
      {
	    PrintError("DMI: Could not determine file name from line:\n %s\n",
		           buff);
	    obj_name[0] = '\0';
	    slot_name[0] = '\0';
	    continue;
      }
      
      /* Get the units.
         Soon RiverWare may supply this in the data file as well, so don't
         consider this a fatal error if not found here.  */

      unit_descriptor = GetToken(buff, "units=");
      if (!unit_descriptor) 
	    PrintError("DMI: Could not determine units in control file.\n");
      
      /* Get the flag that indicates whether to load model data for the
         object/slot.  */

      load_model_data_flag = GetToken(buff, "load_model_data=");
      if (!load_model_data_flag) 
      {
	    /* Do not print a warning, as this is not required -- 
           default is "N" */
	    load_model_data_flag = (char *) malloc(2);
	    strcpy(load_model_data_flag, "N");
      }
      else if ((strcmp(load_model_data_flag, "Y") != 0) &&
		       (strcmp(load_model_data_flag, "N") != 0))
      {
	     PrintError("Invalid load_model_data keyword in control file: %s\n", 
                    load_model_data_flag);
	     obj_name[0] = '\0';
	     slot_name[0] = '\0';
	     if (unit_descriptor) 
	     {
	       free(unit_descriptor);
	       unit_descriptor = NULL;
	     }
	     free(load_model_data_flag);
	     load_model_data_flag = NULL;
	     continue;
      }
      else /* flag is either "Y" or "N" */
      {
	 
/* Rules to check the content of char_model_source, correspondingly, 
   the string input by user after "model_source=" token in METACONTROL 
   file:

   1. If the contents is a wildcard (, check if this wildcard has already 
      been found and stored into modelRunMap structure array.

      found  -- the current position of this certain wildcard in 
                modelRunMap will be assigned to modelRunMap.modelRunId 
                as a negative number.

      ~found -- append the wildcard into modelRunMap, and the current 
                position of this certain wildcard in modelRunMap will 
                be assigned to modelRunMap.modelRunId as a negative 
                number.

   2. Check if the content is a valid model_run_id.

   Note that the Tk interface is needed only if:
   (ANY load_model_data_flag == "Y" && 
       ((flag == wildcard) OR (model_source == UNUSED)) */

        if (!strcmp(load_model_data_flag, "Y")) /* flag == "Y" */
        {
          char_model_source = GetToken(buff, "model_source=");
          /* printf("%s\n", char_model_source);*/
          if (char_model_source)
          {
            if (!strcspn(char_model_source,"0123456789"))
              /* flag contains integers only */
            {
              model_source = atoi(char_model_source);
              if (!model_source) /* model_run_id == 0 */
              {
                PrintError("Error...model_run_id 0 specified in input DMI control file is not a valid_model_run_id...\n");
                return(ERROR);
              }
              else /* flag is positive integer */
              {
                result = SqlChkModelRunID(model_source);
                if (result != OK)
                {
                  PrintError("Error...invalid model_run_id: %d specified in DMI input control file...\n",model_source);
                  return(ERROR);
                }
              }
            }
            else /* flag contains characters other than integers;
                    consider the flag a wildcard */
            {
              *TkInterfaceNeeded = TRUE;
              for (i = 0; i < counter; i++)
		      {
		        /* printf("%s\n", modelRunMap[i]);*/
		        if(!strcmp(modelRunMap[i].code, char_model_source))
			      break;
		      }
		      if (i == counter) 
  		      {
		        strcpy(modelRunMap[counter].code, char_model_source);
		        model_source = 0 - counter;
		        counter++;
		      }
		      else model_source = 0 - i;
	        }
	     }
	     else 
         {
           model_source = 0; /* load_model_data=Y, model_source not
                                specified */
           *TkInterfaceNeeded = TRUE;
         }
        }
      } /* end else flag is either "Y" or "N" */
      
      /* Get the flag that indicates how many timesteps of historical
         data to load.  */

      char_hist_timesteps = GetToken(buff, "num_hist_timesteps=");
      if (char_hist_timesteps)
	 num_hist_timesteps = (atoi (char_hist_timesteps));
      else
	 /* Do not print a warning, as this is not required -- just set
	    default. */
	 num_hist_timesteps = NUM_HIST_TIMESTEPS;
      
      /*
       * Get the flag that indicates source of historical data.
       */
      char_hist_source = GetToken(buff, "hist_source=");
      
      /* The methods to check the content of char_hist_source is same as
       *   the ones to check the content of char_model_source
       */
      
      if (char_hist_source)
	 if(((char_hist_source[0] >= 97) && (char_hist_source[0] <= 122))||((char_hist_source[0] >= 65) && (char_hist_source[0] <= 90)))
	 {
	    for(i=0; i<counter; i++)
	    {
	       /* printf("%s\n", modelRunMap[i]);*/
	       if(!strcmp(modelRunMap[i].code, char_hist_source))
	       {
		  break;
	       }
	    }
	    if(i == counter)
	    {
	       strcpy(modelRunMap[counter].code, char_hist_source);
	       hist_source = 0 - counter;
	       counter++;
	    }
	    else hist_source = 0 - i;
	 }
	 else hist_source = (atoi (char_hist_source));
      else
	 /*  Do not print a warning, as this is not required -- just set
	     default. */
	 hist_source = 0; 

      /*
       * Line was successfully read, so can create new member of header list
       */
      if (*list == NULL) 
      { /* New list -- create the head */
	 *list = malloc(sizeof(dmi_header_struct));
	 if (!*list) 
	 {
	    PrintError("DMI: Could not malloc first member of header list.\n");
	    exit (1);
	 }
	 (*list)->next = NULL;
            new = current = *list;
      }
      else 
      { /* Add new member to end of list */
	 new = malloc(sizeof(dmi_header_struct));
	 if (!new) 
	 {
	    PrintError("DMI: Could not malloc next member of header list.\n");
	    exit (1);
	 }
	 new->next = NULL;
	 current->next = new;
	 current = current->next;
      }
      
      /*
       * Put the parsed values into the new list member.
         Strdup object and slot name as they are not
	 malloc'd memory 
       */
      current->object_name = strdup(obj_name);
      current->slot_name = strdup(slot_name);
      current->pr_datafile_name = file_name;
      current->pr_units = unit_descriptor; 
      current->load_model_data = load_model_data_flag;
      current->number_of_hist_timesteps = num_hist_timesteps;
      current->hist_source = hist_source;
      current->model_source = model_source;
      
      /* Init data to NULL, to protect against erroneous free statements. */
      current->data = NULL;
   }
   
   /*
   ** close file
   */
   fclose(fptr);

   modelRunMapCount = counter;
   modelRunMapList = list;

   /*  Here, I store the counter, modelRunMap, and list variables into global
       variables so that I can call C functions from Tcl that have access to
       this information.  It is a poor solution, but seems to be the best one
       in this case since you can only link C variables to TCL variables if 
       they are the basic ones (int, char, string...);  you cannot link 
       complex data structures.  These global variables are declared in the
       globalInclude/dmi_utils.h file.
       */
   return(OK);
}
/* end routine */


#define FUNC_NAME "readOutputControlFile"

/*
<HTML>

<P> <STRONG>Function name:</STRONG> readOutputControlFile

<P> <STRONG>Function signature:</STRONG> int readOutputControlFile(char *, dmi_header_struct **)

<P> <STRONG>Function description:</STRONG> Reads DMI output control file, parsing out objects, slots and keywords and loading the dmi_header_struct list with values from the file.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *controlFile:        Name of output control file to read.
<BR><DD> dmi_header_struct **list: Array of object/slot specifications. Output.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int readOutputControlFile(char *controlFile, dmi_header_struct **list)
   
{
   FILE *fptr;
   char buff[BUFFSIZE];
   char obj_name[OBJECT_NAME_LEN + 1],
        slot_name[SLOT_NAME_LEN + 1],
        *file_name,
        *unit_descriptor,
        *char_destination;

   dmi_header_struct *current, *new;
   
   int destination = MODEL;    /* default for output DMI is MODEL */
   int model_destination = UNUSED;
   
   /*
    * Initialize head of linked list to NULL
    */
   
   *list = NULL;
   
   /*
   ** open metaControl file
   */
   if((fptr = fopen(controlFile,"r")) == NULL)
   {
      printf("DMI: Failure to open RiverWare control file %s\n",controlFile);
      return(ERROR);
   }
   printf("DMI: RiverWare control file opened successfully:\n %s\n", controlFile);
   /*
   ** read each line of file and get the object.slot and filename
   */
   while(fgets(buff,BUFFSIZE,fptr) != NULL)  
   { /* reads in to newline char*/

      /*
       * Get the object name.
       *   This requires a special routine b/c there can be multiple colons
       *     in an object name, and multiple periods in an object/slot combo
       */

      GetObjAndSlotName(buff, obj_name, slot_name);

      if (!obj_name) 
      {
	 PrintError("DMI: Could not determine object name from line:\n %s\n",                        buff);
	 continue;
      }
      
      if (!slot_name) 
      {
	 PrintError("DMI: Could not determine slot name from line:\n %s\n",
		    buff);
	 obj_name[0] = '\0';
	 continue;
      }
      
      /*
       * Get the file name.
       *   This is a mandatory item. 
       */
      file_name = GetToken(buff, "file=");
      if (!file_name) 
      {
	 PrintError("DMI: Could not determine file name from line:\n %s\n",
		    buff);
	 obj_name[0] = '\0';
	 slot_name[0] = '\0';
	 continue;
      }
      
      /*
       * Get the units.
       *   Soon RiverWare may supply this in the data file as well, so don't
       *     consider this a fatal error if not found here.
       */
      unit_descriptor = GetToken(buff, "units=");
      if (!unit_descriptor) 
	    PrintError("DMI: Could not determine units in control file.\n");

/* check for a !destination= flag. values can be r (real)
   or m<model_run_id>. If the flag = m<model_run_id>
   set model_destination = model_run_id */

     char_destination = GetToken(buff, "destination=");
     
     if (char_destination)
       switch (char_destination[0])
       {
         case 'r': destination = REAL;
                   model_destination = UNUSED;
                   break;

         case 'm':  destination = MODEL;
                    model_destination = atoi(&char_destination[1]);
                    if (!model_destination)
                    {
                      PrintError("Invalid model_run_id %s...\n",
                                 char_destination);
                      return(ERROR);
                    }
                    break;

         default: PrintError("Invalid !destination flag value; must be r or m<model_run_id>\n");
                  return(ERROR);
       }
     else
        model_destination = UNUSED; /* don't set destination to UNUSED
                                       as MODEL is the default */

      /*
       * Line was successfully read, so can create new member of header list
       */
      if (*list == NULL) 
      { /* New list -- create the head */
	 *list = malloc(sizeof(dmi_header_struct));
	 if (!*list) 
	 {
	    PrintError("DMI: Could not malloc first member of header list.\n");
	    exit (1);
	 }
	 (*list)->next = NULL;
	 new = current = *list;
      }
      else 
      { /* Add new member to end of list */
	 new = malloc(sizeof(dmi_header_struct));
	 if (!new) 
	 {
	    PrintError("DMI: Could not malloc next member of header list.\n");
	    exit (1);
	 }
	 new->next = NULL;
	 current->next = new;
	 current = current->next;
      }
      
      /*
       * Put the parsed values into the new list member.
         Strdup object and slot name as they are not
	 malloc'd memory 
       */
      current->object_name = strdup(obj_name);
      current->slot_name = strdup(slot_name);
      current->pr_datafile_name = file_name;
      current->pr_units = unit_descriptor;
      current->destination = destination;
      current->model_destination = model_destination;
      
      /* Init data to NULL, to protect against erroneous free statements. */
      current->data = NULL;
   }
   
   /*
   ** close file
   */
   fclose(fptr);
   return(OK);
}
