/*  function prototypes  */

#ifndef DMIOUT_INC
#define DMIOUT_INC 1

#include <dmi_utils.h>

int readDataFile(
#ifdef ANSIPROTO
  dmi_header_struct *
#endif
);

int DmiSaveModelData(
#ifdef ANSIPROTO
  dmi_header_struct *
#endif
);

int DmiSaveRealData(
#ifdef ANSIPROTO
  dmi_header_struct *
#endif
);

#endif /* put nothing after this line */
