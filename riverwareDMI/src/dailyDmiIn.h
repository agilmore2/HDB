/*  function prototypes  */

#ifndef DMIIN_INC
#define DMIIN_INC 1

#include <dmi_utils.h>

/* this array defines what values will have their model data loaded by the dmi */
typedef struct {
    char object_name[OBJECT_NAME_LEN + 1];
    char slot_name[SLOT_NAME_LEN + 1];
    int site_datatype_id;
    char fetch_model_values;
} model_data_flag_member;

extern model_data_flag_member model_data_flag_array[];

int DmiReadData(
#ifdef ANSIPROTO
  dmi_header_struct *
#endif
);

int SqlGetDmiData(
#ifdef ANSIPROTO
  dmi_header_struct *,
  TIME_INTERVAL_TYPES
#endif
);

int SqlDateGap(
#ifdef ANSIPROTO
  SQL_DATE,
  SQL_DATE,
  char *,
  SQL_DATE
#endif
);

int writeDataFile(
#ifdef ANSIPROTO
  dmi_header_struct *list
#endif
);

int PrepareForQueries(
#ifdef ANSIPROTO
  dmi_header_struct *,
  char *,
  char *,
  TIME_INTERVAL_TYPES
#endif
);

int LookupDataFetchFlag(
#ifdef ANSIPROTO
  int,
  dmi_header_struct *
#endif
);

int PrintMember (
#ifdef ANSIPROTO
  dmi_header_struct *
#endif
);

#endif /* put nothing after this line */


