/*****************************************************************************
**
**	IDENTIFICATION SECTION
**	----------------------
**
**	Module Name:	int readDataFile(dmi_header_struct * header)  
**
**	File Name:	readDataFile.c
**
**	Date Written:	December, 1995
**
**	Author(s): 	Paddy McCarthy, CADSWES
**                      Terry Fulp, USBR
**
**	Module Description:	
**	   routine to read output data files from RiverWare 
**
**
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int isnan(double); /* although in math.h, compiler does not find */

/*  hdb apps include file to define OK and ERROR  */
#include "dbutils.h"
#include "utils.h"
#include "defines.h"
#include "typedefs.h"

#include "dmiOut.h"

int readDataFile(dmi_header_struct * header)
{
    int i;
    FILE *fp;
    char buffer1[100],
         buffer2[100];
    char line_buffer[300];
    SQL_DATE hdb_date;
    SQL_DATE hdb_data_date;
    double tmp_value,
           num_timesteps;
 
    data_array * data;

    if (!header) {
      PrintError("DMI: Invalid header passed to readDataFile\n");
      return ERROR;
    }
    
    if (!header->pr_datafile_name) {
      PrintError("DMI: No datafile name in header passed to readDataFile.\n");
      return ERROR;
    }
    
    fp = fopen(header->pr_datafile_name, "r");
    if (!fp) {
      PrintError("DMI: Could not open data file: %s for reading.\n", 
		 header->pr_datafile_name);
      fclose(fp);
      return ERROR;
    }
    
    /*
     * Read in the start and end dates. For now, with only daily time step,
     *   ignore the time (it is always 24:00)
     */
    buffer1[0] = '\0';
    buffer2[0] = '\0';
    
    while (fgets(line_buffer, 299, fp)) { /* will return zero if EOF reached */
      if (sscanf(line_buffer, "start_date: %s %s", buffer1, buffer2) == 2) {
	break;
      }
    }
    
    if ((buffer1[0] == '\0') || (buffer2[0] == '\0')) {
      PrintError("DMI: Unable to get start date/time from data file for %s.%s.\n",
	     header->object_name, header->slot_name);
        fclose(fp);
        return ERROR;
      }

    /* Create a single start date/time string */
    strcat (buffer1, " ");
    strcat (buffer1, buffer2);

    if (strcmp(header->pr_runcontrol_startdate, buffer1) != 0) {
        /*
        PrintError("DMI: While reading data file for %s.%s,\n start date read (%s) is different from date from run control (%s).\n",
                    header->object_name,
                    header->slot_name,
                    buffer1,
                    header->pr_runcontrol_startdate);
         */

        /* convert the file start date to hdb_format */
        if (riverwareToHdbDate(buffer1, hdb_date, header->timestep) != OK) {
            PrintError("DMI: Unable to convert start date in data file for %s.%s to hdb format: %s.\n",
                        header->object_name,
                        header->slot_name,
                        buffer1);
            fclose(fp);
            return ERROR;
        }

        sprintf(header->hdb_startdate, hdb_date);
    }

    buffer1[0] = '\0';
    buffer2[0] = '\0';

    while (fgets(line_buffer, 299, fp)) { /* will return zero if EOF reached */
        if (sscanf(line_buffer, "end_date: %s %s", buffer1, buffer2) == 2) {
            break;
        }
    }

    if ((buffer1[0] == '\0') || (buffer2[0] == '\0')) {
        PrintError("DMI: Unable to get end date/time from data file for %s.%s.\n",
                    header->object_name,
                    header->slot_name);
        fclose(fp);
        return ERROR;
    }

    /* Create a single end date/time string */
    strcat (buffer1, " ");
    strcat (buffer1, buffer2);
    
    if (strcmp(header->pr_runcontrol_enddate, buffer1) != 0) {
        /*
        PrintError("DMI: While reading data file for %s.%s,\n end date read (%s) is different from date from run control (%s).\n",
                    header->object_name,
                    header->slot_name,
                    buffer1,
                    header->pr_runcontrol_enddate);
         */

        if (riverwareToHdbDate(buffer1, hdb_date, header->timestep) != OK)
        {
            PrintError("DMI: Unable to convert end date in data file for %s.%s to hdb format: %s.\n",
                        header->object_name,
                        header->slot_name,
                        buffer1);
            fclose(fp);
            return ERROR;
        }

        sprintf(header->hdb_enddate, hdb_date);
    }

    /* 
     * Calclulate the number of timesteps represented by the dates
     */
    if (SqlNumIntervalsBetweenDates(header->hdb_startdate,
				    header->hdb_enddate,
				    header->timestep,
				    &num_timesteps) != OK)
    {
        PrintError("DMI: Unable to calculate the number of data values between %s and %s in datafile for %s.%s.\n",
                    header->hdb_startdate,
                    header->hdb_enddate,
                    header->object_name,
                    header->slot_name);
        fclose(fp);
        return ERROR;
    }

    /* Set the number of timesteps on the header */
    header->number_of_timesteps = (int)num_timesteps;
    /*
     * Get the units out of the file. These MAY have also been specified in
     *   the metaControl file, so verify them if so.
     *
     * The units may not be in the file. The next item might be a piece of data.
     */
    buffer1[0] = '\0';
  
    while (fgets(line_buffer, 299, fp)) { /* will return zero if EOF reached */
        if ( (sscanf(line_buffer, "units: %s", buffer1) == 1) ||
             (sscanf(line_buffer, "%lf", &tmp_value) == 1) ) {
            break;
        }
    }
  
    if ( (buffer1[0] == '\0') &&
         (sscanf(line_buffer, "%lf", &tmp_value) != 1) ) {
        /* Neither the keyword nor a double value was found in the file */
        PrintError("DMI: Unable to get units or data value from data file for %s.%s.\n",
                    header->object_name,
                    header->slot_name);
        fclose(fp);
        return ERROR;
    }

    /* either the units or a data value were found -- check which */
    if (buffer1[0] != '\0') {
        /* The units keyword was found -- process the units */
        if (!header->pr_units) { /* This indicates units were not specified */
                                 /*   in metaControl file                   */
            /* put the fetched units in the header */
            PrintError("Units not specified in metaControl, using '%s' from data file.\n", buffer1);

            header->pr_units = malloc(strlen(buffer1) + 1);
            if (!header->pr_units) {
                PrintError("DMI: Error mallocing space for units string.\n");
                fclose(fp);
                return ERROR;
            }
            strcpy(header->pr_units, buffer1);
        }
        else { /* the units were specified in metaControl file. See if same. */
            if (strcmp(header->pr_units, buffer1) != 0) {
                PrintError("Units specified in metaControl (%s) different from units from data file (%s). Using %s.\n", header->pr_units, buffer1, buffer1);
                /* Use the units specified in the data file */
                header->pr_units = realloc(header->pr_units, strlen(buffer1) +1);
                if (!header->pr_units) {
                    PrintError("DMI: Error reallocing space for units string.\n");
                    fclose(fp);
                    return ERROR;
                }
                strcpy(header->pr_units, buffer1);
            }
        }

        /* Done processing the units: thing. Walk to the first data value */
        while (fgets(line_buffer, 299, fp)) { /* will return zero if EOFreached*/
            if (sscanf(line_buffer, "%lf", &tmp_value) == 1) {
                break;
            }
        }
        /* check that left above loop with value (ugly) */
        if (sscanf(line_buffer, "%lf", &tmp_value) != 1) {
            PrintError("Error finding first data value in file.\n");
            fclose(fp);
            return ERROR;
        }
    }
    else { /* the found item was a data value -- process it */
        if (sscanf(line_buffer, "%lf", &tmp_value) != 1) {
            PrintError("Error parsing data file header.\n");
            fclose(fp);
            return ERROR;
        }
    }

    if (header->number_of_timesteps > 0) {
        /*
         * Make the space to hold the values
         */
        header->data = malloc(sizeof(data_array) * header->number_of_timesteps);
        if (!header->data) {
            PrintError("Could not malloc data array.\n");
            fclose(fp);
            return ERROR;
        }

        data = header->data;

        /*
         * Put the first value into the array (already read from file).
         */
        i = 0;
        strcpy(data[i].hdb_date, header->hdb_startdate);
        if (SqlDateMath (ADDITION,
                         header->hdb_startdate,
	                 hdb_data_date,
                         1,
                         header->timestep) != OK)
        {
            PrintError("DMI: Unable to increment for end date from (%s) %d %s(s) in data file for %s.%s.\n",
                       header->hdb_startdate,
                       1,
		       header->timestep,
                       header->object_name,
                       header->slot_name);
            fclose(fp);
            return ERROR;
        }
        strcpy(data[i].end_date, hdb_data_date);
        data[i].processed_flag = 'N';
        data[i].value = tmp_value;
        if (isnan(tmp_value))
            data[i].is_nan = 'Y';
        else
            data[i].is_nan = 'N';
    
        /*
         * Read in the rest of the data values.
         */
        for (i = 1; i < header->number_of_timesteps; i++) {
            if (!fgets(line_buffer, 299, fp)) {
                PrintError("DMI: Unable to get line of data from data file for %s.%s.\nExpected: %d data values; Got: %d\n",
                            header->object_name,
                            header->slot_name,
                            header->number_of_timesteps,
                            i /* not i+1 b/c only i were successful */ );
                free(data);
                data = NULL;
                fclose(fp);
                return ERROR;
            }

            strcpy(data[i].hdb_date, data[i - 1].end_date);
      
            if (SqlDateMath (ADDITION,
                             data[i].hdb_date,
			     hdb_data_date,
                             1,
                             header->timestep) != OK) {
                PrintError("DMI: Unable to increment for end date from (%s) %d %s(s) in data file for %s.%s.\n",
                            data[i].hdb_date,
                            1,
			    header->timestep,
                            header->object_name,
                            header->slot_name);
                fclose(fp);
                return ERROR;
            }
            strcpy(data[i].end_date, hdb_data_date);

            /* REMOVED call to charToNumeric (tmp_date, hdb_data_date) */

            data[i].processed_flag = 'N';
           
            if (sscanf(line_buffer, "%lf", &tmp_value) != 1) {
                PrintError("DMI: Unable to convert contents of line %d to a double in data file for %s.%s.\n",
                       i+1,
                       header->object_name,
                       header->slot_name);
                fclose(fp);
                return ERROR;
            }
            else {
                data[i].value = tmp_value;
            }
    
            if (isnan(tmp_value))
                data[i].is_nan = 'Y';
            else
                data[i].is_nan = 'N';
        } /* end for (i = ...) */
    } /* end if (header->number_of_timesteps > 0) */
    
    /*
     * Verify the units that the data is supplied in from RiverWare
     */
    if (DmiVerifyUnits(header->pr_units,
                       header->hdb_site_datatype_id) != OK) {
        PrintError("DMI: The units check failed for\n\tObject: %s\n\tSlot: %s\n",
                    header->object_name,
                    header->slot_name);
            fclose(fp);
            return(ERROR);
    }

    /* PrintError("Found Site Data Type: %d.\n", header->hdb_site_datatype_id); */
    fclose(fp);
    return OK;
}
