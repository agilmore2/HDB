/*****************************************************************************
* DESCRIPTION
* This file contains function which finds site datatype id from obj/slot names.
*
*********************************
* LookupSiteDatatype
*
* Purpose: Find site_datatype_id for object/slot combination.
*
* Input
*       char *              - object_name
*       char *              - slot_name
*       int * site_datatype_id -- pointer to found site_datatype_id
*
* Output
*       int * site_datatype_id -- set to zero if not found
*
* Return
*       ERROR - site_datatype_id not successfully retrieved
*       OK - site_datatype_id successfully retrieved
*
*
*********************************
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "dbutils.h"
#include "dmi_utils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

#define FUNC_NAME       "LookupSiteDatatype"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> LookupSiteDatatype

<P> <STRONG>Function signature:</STRONG> int LookupSiteDatatype(char *, char *, int *)

<P> <STRONG>Function description:</STRONG> Given an object and slot name, lookup the corresponding site_datatype_id.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *object_name:     Riverware object name.
<BR><DD> char *slot_name:       Riverware slot name.
<BR><DD> int *site_datatype_id: Corresponding site_datatype_id. Looked up.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int LookupSiteDatatype(char * object_name,
                       char * slot_name,
                       int * site_datatype_id)
{
    int i;

    i = 0;
    while (object_slot_lookup_array[i].object_name[0] != '\0') {
        if (strcmp(object_name, object_slot_lookup_array[i].object_name) == 0) {
            if (strcmp(slot_name, object_slot_lookup_array[i].slot_name) == 0) {
                break;
            }
        }

        i++;
    }

    if (object_slot_lookup_array[i].object_name[0] != '\0') {
        *site_datatype_id = object_slot_lookup_array[i].site_datatype_id;
        return(OK);
    }
    else {
        *site_datatype_id = 0;
        return(ERROR);
    }
}

#define FUNC_NAME       "LookupLocation"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> LookupLocation

<P> <STRONG>Function signature:</STRONG> int LookupSiteDatatype(int, char *)

<P> <STRONG>Function description:</STRONG> Given a site_datatype_id, lookup the database location where this site_datatype is maintained.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> int *site_datatype_id: Site_datatype_id. 
<BR><DD> char *location:        Location (database) code. Looked up.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int LookupLocation(int site_datatype_id,
                   char * location)
{
    int i;

    i = 0;
    while (object_slot_lookup_array[i].object_name[0] != '\0') {
        if (object_slot_lookup_array[i].site_datatype_id == site_datatype_id) {
            break;
        }

        i++;
    }

    if (object_slot_lookup_array[i].object_name[0] != '\0') {
        strcpy(location, object_slot_lookup_array[i].data_location);
        return(OK);
    }
    else {
        location[0] = '\0';
        return(ERROR);
    }
}
    

#define FUNC_NAME       "LookupObjectSlot"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> LookupObjectSlot


<P> <STRONG>Function signature:</STRONG> int LookupSiteDatatype(ID, char *, char*)

<P> <STRONG>Function description:</STRONG> Given a site_datatype_id, lookup the corresponding Riverware object and slot names.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> int *site_datatype_id: Site_datatype_id. 
<BR><DD> char *objectName:      Riverware object name. Looked up.
<BR><DD> char *slotName:        Riverware slot name. Looked up.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int LookupObjectSlot (ID siteDatatypeId, char *objectName, char *slotName)

{
  int i = 0;

  while (object_slot_lookup_array[i].site_datatype_id != 0)
    {
      if (object_slot_lookup_array[i].site_datatype_id == siteDatatypeId)
        break;

      i++;
    }

  if (object_slot_lookup_array[i].site_datatype_id != 0)
    {
      strcpy (objectName, object_slot_lookup_array[i].object_name);
      strcpy (slotName, object_slot_lookup_array[i].slot_name);
    }
  else
    {
      objectName[0] = '\0';
      slotName[0] = '\0';
    }

  return (OK);
}
