/*****************************************************************************
* DESCRIPTION
* This function calls the four Ingres update functions required for 
* snotel sites.
*
* ASSUMPTIONS 
*
* INPUT
* SNOTEL       - array of snotel site info
* int          - number of elements in array
* char *       - date
* ID           - source ID for this run of basins
*
* OUTPUT
* warnings and error messages
*
* RETURN
* OK or ERROR
*
*
* COMMENTS
*
* AUTHOR - EDITORS  
* Carol Marra
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "basins.h"
#include "basinTypedefs.h"
#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

#define FUNC_NAME "updateSnotel"

int UpdateSnotel (SNOTEL *snotelArray, int numSites, SQL_DATE date,
		  ID sourceId)

{
  int result,
      totalNumValues;

  if ((result = SqlUpdateCurSnow (snotelArray, numSites, date, sourceId,
				  &totalNumValues)) != OK)
    {
      exit (ERROR);
    }
  fprintf (stdout, "NUM cur snow SqlUpdates: %d\n", totalNumValues);
  
  if ((result = SqlUpdateCurPrecip (snotelArray, numSites, date, sourceId,
				    &totalNumValues)) != OK)
    {
      exit (ERROR);
    }
  fprintf (stdout, "NUM cur precip updates: %d\n", totalNumValues);  
  
  if ((result = SqlUpdatePctSnow (snotelArray, numSites, date, sourceId,
				  &totalNumValues)) != OK)
    {
      exit (ERROR);
    }
  fprintf (stdout, "NUM avg snow updates: %d\n", totalNumValues);
  
  if ((result = SqlUpdatePctPrecip (snotelArray, numSites, date, sourceId,
				    &totalNumValues)) != OK)
    {
      exit (ERROR);
    }
  fprintf (stdout, "NUM avg precip updates: %d\n", totalNumValues);  
  
  return (OK);
}
