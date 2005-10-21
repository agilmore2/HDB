/*************************************************************************************************************
 * scadaTransfer.c  -   processes data in the scada log file
 *  
 *  written by T Ryan - January 1996
    ported to Oracle by T Ryan - November 1998
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
#include "scadaTransfer.h"

#define FUNC_NAME "main"

int main( int argc, char** argv )

{
  char   resname[14],rolePassword[20];
  char *dbName;
  char *appRole;
  SQL_DATE   dater, date1, formattedDates[NUM_HOURS];
  char  newDateFormat[] = "DD-MON-YYYY HH24:MI:SS\0";
  char   buffer[BUFFER_LENGTH];
  char   validationFlag[2];
  char   charYear[3], temp[15];
  int    i, j,k,sd[NUM_SITES][NUM_DATATYPES],index,length,result,found, year;
  int    site_datatype_id;
  ID  sourcetypeId,sourceId;
  SQL_DATE updateTime;
    
  float  values[NUM_SITES][NUM_DATATYPES][NUM_HOURS], tail[NUM_SITES][NUM_HOURS], vals_array[NUM_HOURS];
  float  hour[NUM_SITES][NUM_HOURS];

  FILE   *in;

  /*initialize site_datatypes */
  /* Not at all elegant.  Should probably be in a .h file at least */
  
  sd[0][0]=1928; /* Glen Canyon - Lake Powell wse */                         
  sd[0][1]=1975; /* Glen Canyon - Lake Powell head */                        
  sd[0][2]=1981; /* Glen Canyon - Lake Powell generation */                  
  sd[0][3]=5034; /* Glen Canyon - Lake Powell power releases */              
  sd[0][4]=1872; /* Glen Canyon - Lake Powell power and non power releases */
  sd[0][5]=4167; /* Glen Canyon - Lake Powell bypass releases */
  sd[0][6]=4166; /* Glen Canyon - Lake Powell spill */
  sd[1][0]=1924; /* Morrow Point wse */                         
  sd[1][1]=1971; /* Morrow Point head */                        
  sd[1][2]=1977; /* Morrow Point generation */                  
  sd[1][3]=1858; /* Morrow Point power releases */              
  sd[1][4]=1868; /* Morrow Point power and non power releases */
  sd[1][5]=4153; /* Morrow Point bypass releases */
  sd[1][6]=4151; /* Morrow Point spill */
  sd[2][0]=1923; /* Blue Mesa wse */                         
  sd[2][1]=1970; /* Blue Mesa head */                        
  sd[2][2]=1976; /* Blue Mesa generation */                  
  sd[2][3]=1857; /* Blue Mesa power releases */              
  sd[2][4]=1867; /* Blue Mesa Power and non power releases */
  sd[2][5]=4149; /* Blue Mesa bypass releases */
  sd[2][6]=4147; /* Blue Mesa spill */
  sd[3][0]=1925; /* Crystal wse */                         
  sd[3][1]=1972; /* Crystal head */                        
  sd[3][2]=1978; /* Crystal generation */                  
  sd[3][3]=1859; /* Crystal power releases */              
  sd[3][4]=1869; /* Crystal power and non power releases */
  sd[3][5]=4157; /* Crystal bypass releases */
  sd[3][6]=4155; /* Crystal spill */
  sd[4][0]=1927; /* Flaming Gorge wse */                         
  sd[4][1]=1974; /* Flaming Gorge head */                        
  sd[4][2]=1980; /* Flaming Gorge generation */                  
  sd[4][3]=1861; /* Flaming Gorge power releases */              
  sd[4][4]=1871; /* Flaming Gorge power and non power releases */
  sd[4][5]=4164; /* Flaming Gorge bypass releases */
  sd[4][6]=2416; /* Flaming Gorge spill */
  sd[5][0]=1926; /* Fontenelle wse */                         
  sd[5][1]=1973; /* Fontenelle head */                        
  sd[5][2]=1979; /* Fontenelle generation */                  
  sd[5][3]=1860; /* Fontenelle power releases */              
  sd[5][4]=1870; /* Fontenelle power and non power releases */
  sd[5][5]=4161; /* Fontenelle bypass releases */
  sd[5][6]=4159; /* Fontenelle spill */
  
  if (argc != 4)
    {
      printf("\nusage: scadaTransfer <scada_log_file> <user name> <password> \n\n");
      exit(2);
    }
  
/* set the name of the database */
  
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
  
  if ((result = SqlSetDateFormat (newDateFormat)) != OK)
    {
      PrintError ("Problem setting default date format.\n\tExiting.");
      SqlDisconnect();
      exit(1);
    }

  if ((in = fopen(argv[1],"r+")) == NULL)
    {
      fprintf(stderr, "can't open input file\n");
      exit(1);
    }

  /* Get all information needed to create sourceId for this application.
     Each data value will have the same sourceId as there are no data quality
     flags. */
  
/* delete the following blocks on 10/8/96 */

/*  if ((result = SqlGetSourcetypeId (argv[0], &sourcetypeId)) != OK)
    {
      SqlDisconnect();
      exit (ERROR);
    }
    

  if ((result = SqlGetDateTime (updateTime)) != OK)
    {
      SqlDisconnect();
      exit (ERROR);
    }
  
  if ((result = SqlGetSourceId (sourcetypeId, updateTime, NULL, &sourceId)) 
      != OK)
    {
      SqlDisconnect();
      exit (ERROR);
    }
  
    */

/* add a call to SqlGetAppSourceId on 10/8/96 */

/*  if((result = SqlGetAppSourceId(argv[0], &sourceId)) != OK)
    {
      SqlDisconnect();
      exit (ERROR);
    }
*/
  for( j=0; j < NUM_SITES; j++)
    { 

      /* read in a blank line */
      
      fgets (buffer,BUFFER_LENGTH,in);  
      
      fscanf(in, "%s%s",resname, date1);

      /* process the all of the sites in the file  */

      found = FALSE;
      for(i=0; i < NUM_SITES && !found; i++)
	{
	  length = strlen(reservoirName[i]);
	  if(!strncmp(resname,reservoirName[i],length))
	    {
	      index=i;
	      found = TRUE;
	    }
	}
      
      /* read in three blank lines*/
      for(k=0; k < 3; k++)
	fgets (buffer,BUFFER_LENGTH,in);  

      printf("Reading data for %s\n",resname);      

      for(i=0; i < NUM_HOURS; i++)
	{
	  fscanf(in,"%f%f%f%f%f%f%f%f%f",
		 &hour[index][i],&values[index][0][i],&tail[index][i],&values[index][1][i],
		 &values[index][2][i],&values[index][3][i],&values[index][6][i],&values[index][5][i],
		 &values[index][4][i]);
	}

      /* Manipulate the date string; do this only *once*, for the first site, as all
       sites will have the same set of dates and hours. */
      if (j == 0)
	{
	  /* Put date into format accepted by SqlFormatDate; make year 4-digit */
	  strncpy(&dater[0], &date1[5], 2);
	  dater[2]='-';
	  strncpy(&dater[3], &date1[2] ,3);
	  dater[6]='-';
	  /* Kludge, but works for Y2K */
	  strncpy (charYear, &date1[0],2);
          charYear[2]= '\0';
	  year = atoi (charYear);
	  if (year > 90)
	    strncpy(&dater[7], "19",2);
	  else
	    strncpy(&dater[7], "20",2);
	  strcpy(&dater[9], charYear);

	  /* Call SqlFormatDate for every hour; it accepts and corrects 1-digit hours */
	  for (i = 0; i < NUM_HOURS; i++)
	    {
	      sprintf (temp, " %d:00:00", i);
	      temp[9] = '\0';
	      strcpy (&(dater[11]), temp);
	      dater[20] = '\0';
/*	      printf ("Calling SqlFormatDate\n\n"); */
	      if (result = SqlFormatDate (dater, formattedDates[i]) != OK)
		{
		  PrintError ("Error formatting date %s\n\t Exiting.", dater);
		}
	    }
	}

      /*load all of the datatypes for this site*/
      for(k=0; k < NUM_DATATYPES; k++)
	{
	  printf("    Processing %s data for %s on %s\n",datatypeName[k],reservoirName[index],dater);


	  site_datatype_id = sd[index][k];

          /* do not process Powell total release,
             since we generate that in a different way. */
          if (site_datatype_id == 1872)
             continue;

          if ((result = SqlGetValidationFlag 
	       (site_datatype_id, validationFlag)) != OK)
	    {
	      SqlDisconnect();
	      exit (ERROR);
	    }	    

	  /* copy values for this SDI into array to be passed to insert function */
	  for (i=0;i < NUM_HOURS; i++)
	    {
	      vals_array[i]=values[index][k][i];
	    }

          
	  if((result = insertScada (formattedDates, site_datatype_id,vals_array,sourceId,
				    validationFlag)) != OK)
	     {
	       SqlDisconnect();
	       exit (ERROR);
	     }	    
	}

      /* read in four blank lines to get to the next reservoir*/

      for(k=0; k < 4; k++)
	fgets (buffer,BUFFER_LENGTH,in);  
      
    }

  fclose(in);
  
  SqlDisconnect();  
  return(0)  ;
}

/* end of main */

