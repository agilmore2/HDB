#ifndef BASINTYPEDEFS_INC
#define BASINTYPEDEFS_INC 1
#include "defines.h"
#include "typedefs.h"
#define BUFFER_SIZE 132
#define BAD_DATA -999
#define MAX_BASIN_NAMES 200
#define MAX_SNOTEL_NAMES 300
#define QUALITY_FLAG NULL
/* old (HDB1) datatype codes. HDB2 uses datatype_names.
*	#define CUR_SNOW "sweeop"
*	#define PCT_SNOW "swenorwy"
*	#define CUR_PCP "pcptotwy"
*	#define PCT_PCP "pcpnorwy"
*	#define BAS_PCT_SNOW "swenorwy"
*	#define BAS_PCT_PCP "pcpnorwy"
*/
/* new (HDB2) datatype names
 * (rc, 5/7/03) Note: These names came from querying the hdb_datatype table
 * for datatype_id, datatype_code and datatype_name. The datatype 
 * codes above were used to get the datatype_names below.
 * Here are the datatype id's at CADSWES HDB2 table at this time:
 * data
 * type_id  #define       datatype_name
 * -------  -------       ------------------
 *    50    CUR_SNOW      snow water equivalent, end of per reading used as value for per
 *    2     PCT_SNOW      percent of normal snow water equivalent, end of period reading
 *    72    CUR_PCP       cumulative water year precipitation, end of period reading
 *    1     PCT_PCP       percent of normal precipitation to date, end of period reading
 *    2     BAS_PCT_SNOW  percent of normal snow water equivalent, end of period reading
 *    1     BAS_PCT_PCP   percent of normal precipitation to date, end of period reading
 */
#define CUR_SNOW "snow water equivalent, end of per reading used as value for per"
#define PCT_SNOW "percent of normal snow water equivalent, end of period reading"
#define CUR_PCP "cumulative water year precipitation, end of period reading"
#define PCT_PCP "percent of normal precipitation to date, end of period reading"
#define BAS_PCT_SNOW "percent of normal snow water equivalent, end of period reading"
#define BAS_PCT_PCP "percent of normal precipitation to date, end of period reading"

 /* (rc, 5/9/03) Added methods for each datatype; these were selected
  * by Andrew Gilmore at UC from the list in hdb2:hdb_method. 
  * (rc, 5/13/03) Changed CUR_PCP_METHOD_NAME to unknown to facilitate testing.
  */
#define CUR_SNOW_METHOD_NAME "data-point end of period or last value in period"
#define PCT_SNOW_METHOD_NAME "fraction of normal for the interval"
#define CUR_PCP_METHOD_NAME "unknown"
#define PCT_PCP_METHOD_NAME "fraction of normal to-date accumulation"
#define BAS_PCT_SNOW_METHOD_NAME "fraction of normal for the interval"
#define BAS_PCT_PCP_METHOD_NAME "fraction of normal to-date accumulation"

/* HDB2 Foreign key lookup names needed for inserts to r_base */
#define LOADING_APPLICATION_NAME "Basins"
#define AGEN_NAME "National Resources Conservation Service"
#define COMPUTATION_NAME "N/A"
#define COLLECTION_SYSTEM_NAME "Snotel"

/* define three data types one called snotel with the structure _snotel,
   one called basin with the structure _basin, and one called NAME_ID with
   the structure _id    */

  typedef struct snotel {
    ID siteId;
    ID altId;
    char *siteName;
    float currentSnow;
    int percentSnow;
    float currentPrecip;
    int percentPrecip;
  } SNOTEL;

  typedef struct basin {
    ID siteId;
    char *siteName;
    int percentSnow;
    int percentPrecip;
  } BASIN;

  typedef struct name_id {
    ID siteId;
    SITE_NAME siteName;
    ID altId;
  } NAME_ID;
#endif

