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

int UpdateSnotel (SNOTEL *snotelArray, int numSites, SQL_DATE date)

{
  int result,
      totalNumValues;

  if ((result = SqlUpdateCurSnow (snotelArray, numSites, date,
				  &totalNumValues)) != OK)
    {
      exit (ERROR);
    }
  fprintf (stdout, "NUM cur snow SqlUpdates: %d\n", totalNumValues);
  
  if ((result = SqlUpdateCurPrecip (snotelArray, numSites, date,
				    &totalNumValues)) != OK)
    {
      exit (ERROR);
    }
  fprintf (stdout, "NUM cur precip updates: %d\n", totalNumValues);  
  
  if ((result = SqlUpdatePctSnow (snotelArray, numSites, date,
				  &totalNumValues)) != OK)
    {
      exit (ERROR);
    }
  fprintf (stdout, "NUM avg snow updates: %d\n", totalNumValues);
  
  if ((result = SqlUpdatePctPrecip (snotelArray, numSites, date,
				    &totalNumValues)) != OK)
    {
      exit (ERROR);
    }
  fprintf (stdout, "NUM avg precip updates: %d\n", totalNumValues);  
  
  return (OK);
}
