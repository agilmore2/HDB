/* Contains declaration of global arrays.                          */
/* This file can be included in only one file for each application */

#ifndef DMI_GLOBAL_INC
#define DMI_GLOBAL_INC 1

#include <dmi_utils.h>

/* Global declarations */
int number_of_db_sites = 0;

int max_number_of_db_sites = 4;

dbsite_member *dbsite_array;

object_slot_lookup_member *object_slot_lookup_array;
unit_lookup_member *unit_lookup_array;

/*******************************************************************/
/* Add variable declarations here for all
   user and sytem arguments to DMIs */

/* For instance, the user argument for model run ID uses the keyword
   "model_run" and the values is stored in the variable model_run_id. 
   The variable must be declared here, and the keyword defined in dmi_utils.h
   Note also that model_run_id must be externed in dmi_utils.h */
int model_run_id;

timestep_lookup timestep_lookup_array [] = 
{ 
 {"1HOUR", BY_HOUR},
 {"1DAY", BY_DAY},
 {"1MONTH", BY_MONTH},
 {{0},0}
};

#endif /* nothing after this line */
