/*   Program basins    -  Reads a report about SCS Snotel retrieval 
 *   prosesses appropiate data into arrays, then updates HDB with the 
 *	 data collected from the logfie.
 *
 *   Developed by Sautins and Ryan 1/12/93     
 *   Updated to work for HDB by Carol Marra, Feb 1995 
 *   Updated to work for HDB2 by Rick Casey, April 2003
 * ---------------------------------------------------------------------
 * NOTES (chronologically descending):

 * (april 2003, rc) Modifed this and subroutines to insert and update
 * the r_base table instead of r_instant.
 *
 *   Program accepts one argument, the name of the logfile.  This file
 *   is opened along with to other files, basin_names and snotel_names, 
 *   which contain the basins and the snotel sites to be processed.  The
 *   names of these two latter files is hard coded into the program.
 *
 *   NOTE: basins which are retrieved in more than one query will be 
 *   represented multiple times in the basinData array; as a 	
 *   result, data for these basins will be inserted into the 
 *   database multiple times.  This COULD cause problems.
 *
 * Patched 10/00 to account for when a value has a trailing '*' at the
 * end. The application now works when it encouters this; it does 
 * write the value in question to the DB. However, it might be wise at
 * some point to determine the true meaning of the '*', and either
 * filter out such values, or write an appropriate validation flag with
 * the value.
 * ---------------------------------------------------------------------*/

/* (4/7/03, rc) #define VERSION "1.1" */
#define VERSION "HDB2 1.3"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>
#include "basins.h"
#include "basinTypedefs.h"
#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

/* Prototype to prevent warnings... */
char *strdup(const char *);

/* declare global variables prior to the main() line in the program
   These global variables will be available to all functions in the program
   even though they are not passed as arguments   */

/* assume that we will never have more than 300 snotel sites or 50 basins */
/* NOTE:  these may not need to be global; check to see what other functions
   use them!! */

SNOTEL snotelSites[MAX_SNOTEL_NAMES];
BASIN basinData[MAX_BASIN_NAMES];
int numBasinData = 0;

char *date;
NAME_ID  basinNames[MAX_BASIN_NAMES];
NAME_ID  snotelNames[MAX_SNOTEL_NAMES];
int numSnotelNames = 0;
int numBasinNames = 0;
int numSnotelSites = 0;

int main(int argc, char **argv)

{
  int i,j;
  FILE *logFile;
  char buffer[BUFFER_SIZE];
  NAME_ID  *validBasinID;

  int test,
      result;
  char elevation[50], pctAveragePrecip[50], pctAverageSnow[50];
  char currentSnow[50], percentSnow[50], currentPrecip[50], percentPrecip[50];
  char basinAverageSnow[50], basinAveragePrecip[50];

  char *appRole;
  char *dbName;
  SQL_DATE oracleDate,
           tempDate;
  ID sourceId,
     basinTypeId,
     climateTypeId,
     snotelTypeId;
  
  if((argv[1] != NULL) && (!strcmp(argv[1], "-v")))
   {
      printf("\nBasins version number: %s\n\n", VERSION);
      exit (OK);
   }

  if(argc != 4)
    {
      printf("Usage: basins <userName> <password> <logfile>\n");
      exit(ERROR);
    }

   /* set the name of the database and get the application role name*/
   dbName = NULL;
   dbName = getenv("HDB_LOCAL");

   if (dbName == NULL)
   {
      PrintError("Environment variable HDB_LOCAL not set...\n");
      exit(ERROR);
   }

   appRole = NULL;
   appRole = getenv ("APP_ROLE");

   if (appRole == NULL)
   {
      PrintError("Environment variable APP_ROLE not set...\n");
      exit(ERROR);
   }

   RegisterSignalHandler();

   if ((result = SqlConnectRole (dbName, argv[1], argv[2], appRole)) == ERROR)
      exit (ERROR);  
  
  fprintf(stdout, "Connected to ORACLE.\n");

  /* Get the source ID for this app; source will be same for every piece of
     data loaded. */
  if ((result = SqlGetAppSourceId (argv[0], &sourceId)) == ERROR)
    exit (ERROR);

  /* Get the objecttype_ids for Snotel sites and basins */
  if ((result = SqlGetObjecttypeId (BAS, &basinTypeId)) == ERROR)
    exit (ERROR);

  if ((result = SqlGetObjecttypeId (SNO, &snotelTypeId)) == ERROR)
    exit (ERROR);

  if ((result = SqlGetObjecttypeId (CLI, &climateTypeId)) == ERROR)
    exit (ERROR);

  /* Load the basinNames and climateNames array by querying the database */
  if ((result = SqlFetchSiteNames (basinTypeId, basinNames, &numBasinNames))
	== ERROR)
    {
      exit (ERROR);
    }

  if (!numBasinNames)
    {
      fprintf (stdout, "There are no basins for which data is required.  Exiting.\n");
      exit (OK);
    }

  if ((result = SqlFetchSiteNamesPlusAltId (snotelTypeId, snotelNames, 
					    &numSnotelNames, climateTypeId)) 
      == ERROR)
    {
      exit (ERROR);
    }

  if (!numSnotelNames)
    {
      fprintf (stdout, "There are no snotel sites for which data is required.  Exiting.\n");
      exit (OK);
    }

  if((logFile = fopen(argv[3],"r")) == NULL)
    {
      printf("log file %s does not exist\n", argv[3]);
      exit(0);
    }
  
  /* Loop until you find key line */
  /* We Assume that the entire record is available after the key */
  
  /* feop returns a non zero when the end of file is reached  
     thus, the following says 'while not end of file'  */
  
  /* the following loop will be repeated as many times as there were basin/state queries
     which will usually be four */
  
  while(!feof(logFile))
    {
      while ((fgets (buffer, BUFFER_SIZE, logFile) != NULL) && 
	     (!strstr(buffer,
		      "S N O W  -  P R E C I P I T A T I O N    U P D A T E")));
      
      /*if another update exists, read it in */
      
      if(!feof(logFile))
	{
	  /* Read two lines (Discard cause we don't need them) */
	  fgets(buffer,BUFFER_SIZE, logFile);
	  fgets(buffer,BUFFER_SIZE, logFile);
	  
	  /* extract the date from the file  */
	  
	  fgets(buffer, BUFFER_SIZE, logFile);
	  date = (char *)strdup(buffer);
	  
	  /* discard next five lines */
	  
	  for(i =0; i < 5; i++)
            if(fgets(buffer, BUFFER_SIZE, logFile) == 0)
	      {
		printf("File is not in correct format\n");
		exit(0);
	      }
	  
	  /* begin a new loop,  this loop will run until the end of file is reached or until 
	     the "---------------" string is found in the buffer, indicating that the last snotel
	     site for a basin has been processed.  Within the loop we will test to see if a valid
	     basin is encountered by comparing each the buffer with all basin names  */
	  
	  while((fgets(buffer, BUFFER_SIZE, logFile) != NULL) && 
		strncmp(buffer,"------------------",15) && !feof(logFile))
            {
	      
	      /* find the column of the carriage return in the buffer. Then work backwards until a non space character 
		 is found, add a null terminator to the next colunm.  We then have a string which can be compared to 
		 the list of basins stored in the basins */
	      i = 0;
	      while(buffer[i] != '\n')
		i++;
	      while(isspace(buffer[i]) && i > 0)
		i--;
	      buffer[i+1] = (int)NULL;
	      /* check to see if this string matches a valid basin, note that the following line has the function call
		 validBasin embedded within it  */
	      
	      /* if it is a valid basin, process all of the snotel sites that are associated with that basins */
	      
	      if((validBasinID = validBasin(buffer)))
		{
		  
		  /* when a valid basin is encouneted first load the name of it into the basinData structure  
		     load the name and the bid   check out strdup function */
		  
		  basinData[numBasinData].siteName = 
		    (char *) strdup(validBasinID->siteName);
		  basinData[numBasinData].siteId = validBasinID->siteId;
		  /* one blank line */
		  fgets(buffer, BUFFER_SIZE, logFile);
		  
		  /* begin a new loop that will continue until record beginning with 10 blanks is found
		     this will indicate there are no more snotel sites for this basin  */
		  
		  while(((fgets(buffer,BUFFER_SIZE,logFile)) != NULL) &&
			(strncmp(buffer,"          ",10)))
		    {
		      
		      /* skip 22 characters of the record (the name) and then parse out the values into string variables */
		      
		      i=22;
		      sscanf(&(buffer[i]),"%s %s %s %s %s %s %s",
			     elevation,
			     currentSnow, percentSnow,
			     pctAverageSnow,
			     currentPrecip, percentPrecip,
			     pctAveragePrecip);
		      
		      /* now work backwords from i and put a null teminator at the end of the snotel name in the buffer */
		      
		      while((isspace(buffer[i])) && (i > 0))
			i--;
		      buffer[i+1] = (int)NULL;
		      
		      /* use the snotelSiteExists function to see if the snotel name in the buffer has already bee
			 processed.  If it has not then processes it into the snotelSites structure  */
		      
		      if(!snotelSiteExists(buffer))
			{
			  snotelSites[numSnotelSites].siteName = 
			     (char *)strdup(buffer);
			  
			  /* loop through all the snotel names in snotelNames, until a the one that matches the buffer is
			     found,  when it is found set the wid */
			  j=0;
			  while(isspace(buffer[j]))
			    j++;
			  test=0;
			  for(i=0; i < numSnotelNames; i++)
			    if(!strcmp(&(buffer[j]),snotelNames[i].siteName))
			      {
				snotelSites[numSnotelSites].siteId = 
				  snotelNames[i].siteId;
				snotelSites[numSnotelSites].altId = 
				  snotelNames[i].altId;
				test=1;
			      }
			  if(test == 0)
			    printf("\nWarning, a snotel site is in the report file but no HDB site exists\nfor basin %s for site %s\n", basinData[numBasinData].siteName,&(buffer[j]));
			  
			  /* use the validValue to determine if a site has missing data
			     if it is missing(not valid0 store BAD_DATA (-999) otherwise store data  */
			  
			  if(validValue(currentSnow))
			    snotelSites[numSnotelSites].currentSnow =    
			      atof(currentSnow);
			  else
			    snotelSites[numSnotelSites].currentSnow=BAD_DATA;
			  if(validValue(pctAverageSnow))
			    snotelSites[numSnotelSites].percentSnow =    
			      atoi(pctAverageSnow);
			  else
			    snotelSites[numSnotelSites].percentSnow=BAD_DATA;
			  
			  if(validValue(currentPrecip))
			    snotelSites[numSnotelSites].currentPrecip =    
			      atof(currentPrecip);
			  else
			    snotelSites[numSnotelSites].currentPrecip=BAD_DATA;
			  if(validValue(pctAverageSnow))
			    snotelSites[numSnotelSites].percentPrecip =    
			      atoi(pctAveragePrecip);
			  else
			    snotelSites[numSnotelSites].percentSnow=BAD_DATA;
			  numSnotelSites++;
			}
		    }
		  fgets(buffer, BUFFER_SIZE, logFile);
		  i=0;
		  
		  /* now extract the basin percent of normals and store in the basinData structure */
		  
		  while((!isdigit(buffer[i])) && (i < BUFFER_SIZE))
		    i++;

		  sscanf(&(buffer[i]),"%s %s", &basinAverageSnow, &basinAveragePrecip);

		  if (validValue(basinAverageSnow))
		    basinData[numBasinData].percentSnow =    
		      atoi(basinAverageSnow);
		  else
		    basinData[numBasinData].percentSnow = BAD_DATA;
		  
		  if (validValue(basinAveragePrecip))
		    basinData[numBasinData].percentPrecip =    
		      atoi(basinAveragePrecip);
		  else
		    basinData[numBasinData].percentSnow = BAD_DATA;

		  numBasinData++;
		}
	    }
	  fgets(buffer, BUFFER_SIZE, logFile);
	}
    }
  
  /* Process the data into a valid Oracle date, then format it. */
  parseDate (date, tempDate);
  if (SqlFormatDate (tempDate, oracleDate) != OK)
    {
      fprintf (stdout, "Error formatting date %s.\n", tempDate);
      exit (ERROR);
    }
 
  /* Here, process snotel and basin arrays */
  if ((result = UpdateSnotel (snotelSites, numSnotelSites, oracleDate, sourceId)) == ERROR)
    {
      fprintf (stdout, "Error updating snotel site values.\n");
      exit (ERROR);
    } 
  
  if ((result = UpdateBasins (basinData, numBasinData, oracleDate,
			      sourceId))
      == ERROR) 
    {
      fprintf (stdout, "Error updating basin values.\n");
      exit (ERROR);
    }

#ifdef DEBUG
  for(i=0; i < numSnotelSites; i++)
    printf("\nStation %s with wid %d has swe of  %f which is %d percent of normal %f tpc which is %d percent of normal\n",
	   snotelSites[i].siteName,snotelSites[i].siteId,
	   snotelSites[i].currentSnow,
	   snotelSites[i].percentSnow,snotelSites[i].currentPrecip,
	   snotelSites[i].percentPrecip);
  
  for(i=0; i < numBasinData; i++)
    printf("\nBasin %s with bid %d has normswe of %d normtpc of %d\n",
	   basinData[i].siteName,basinData[i].siteId,
	   basinData[i].percentSnow,
	   basinData[i].percentPrecip);
#endif /* DEBUG*/

  if ((result = SqlDisconnect ()) == ERROR)
    exit (ERROR);
  
  exit (OK);
  
}


/* function validBasin */
NAME_ID  *validBasin (char *buffer)

{
  int i;
  
  /* loop through all of the basin names to see if we get a match between the 
     buffer and the basin names in the basinNames structure, note that the 
     comparison can be made because prior to calling the function, a null 
     terminator was placed at the end of the string  */
  
  /* note that here we can use the '.' instead of the -> in the struture 
     because it was not passed as an argument to the function.  The function
     knows about basinNames because it was declared as a global variable at 
     the beginning of the program (it was declared before the main()  */
  
  for(i=0; i < numBasinNames; i++)
    if(!strcmp (buffer,basinNames[i].siteName))
      {
	return(&(basinNames[i]));
      }
  
  return(0);
}



/* function snotelSiteExists lets us know if a given snotel has been 
   processed yet or not */

int snotelSiteExists (char *buffer)
{
  int i;
  
  /* loop through all the snotel sites to see if a site name is already in the array of names in the
     snotelSites data structure */
  
  for(i=0; i < numSnotelSites; i++)
    {
      if(!strcmp(buffer,snotelSites[i].siteName))
	{
	  return(1);
	}
    }
  return(0);
}



/* check to see if value is valid */

int validValue (char *buffer)

{
  if((!strcmp(buffer,"-M")) || (!strcmp(buffer,"*")))
    return(0);
  else
    return(1);
}
