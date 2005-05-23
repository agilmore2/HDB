#include <stdio.h>
#include <string.h>
#include <defines.h>
#include <typedefs.h>
#include <dbutils.h>
#include <utils.h>
#include "Lookup.h"

#include <stdlib.h>

extern ID agen_id,collection_system_id,loading_application_id,method_id,computation_id;

int main(int argc, char **argv) {

  char agen_name[] = "USBR";
  char collection_system_name[] = "SCADA";
  char loading_application_name[] = "scadaTransfer";
  char method_name[] = "unknown";
  char computation_name[] = "unknown";
  int a = 0;

  printf("Testing\n");

  int result;
  char *dbName;

  /* set the name of the database */
  
  dbName = NULL; 
  dbName = getenv("HDB_LOCAL");
  
  if (dbName == NULL)
    {
      PrintError("Environment variable HDB_LOCAL not set...\n");
      return(ERROR);
    }
  
  
  if  ((result = SqlConnect (dbName, "app_user", dbName)) == ERROR)
    exit (ERROR);
  
  fprintf(stdout, "Connected to ORACLE.\n");

  printf ("%d\n",a);

  a = SqlLookupIds(agen_name,collection_system_name,
		       loading_application_name,method_name,computation_name);
  return 0;
}
