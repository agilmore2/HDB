/*****************************************************************************
* DESCRIPTION
* This function calls the two Ingres update functions required for 
* updating basin info in the database.
*
* ASSUMPTIONS 
*
* INPUT
* BASIN        - array of basin site info
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

#define FUNC_NAME "UpdateBasins"

int UpdateBasins (BASIN *basinArray, int numSites, SQL_DATE date)

{
  int result,
      totalNumValues;
  
  if ((result = SqlUpdateBasPctSnow (basinArray, numSites, date,
				     &totalNumValues)) != OK)
    {
      exit (ERROR);
    }
  fprintf (stdout, "NUM basin pct snow updates: %d\n", totalNumValues);
  
  if ((result = SqlUpdateBasPctPrecip (basinArray, numSites, date,
				       &totalNumValues)) != OK)
    {
      exit (ERROR);
    }
  fprintf (stdout, "NUM basin pct precip updates: %d\n", totalNumValues);  
  
  return (OK);
}



