PROMPT CREATE OR REPLACE PROCEDURE update_r_base_raw

CREATE OR REPLACE PROCEDURE update_r_base_raw ( SITE_DATATYPE_ID_IN NUMBER,
			  INTERVAL_IN VARCHAR2,
			  START_DATE_TIME_IN DATE,
			  END_DATE_TIME_IN DATE,
			  VALUE_IN FLOAT,
			  AGEN_ID_IN NUMBER,
			  OVERWRITE_FLAG_IN VARCHAR2,
			  VALIDATION_IN CHAR,
			  COLLECTION_SYSTEM_ID_IN NUMBER,
			  LOADING_APPLICATION_ID_IN NUMBER,
			  METHOD_ID_IN NUMBER,
			  COMPUTATION_ID_IN NUMBER )
IS
manual_edit CHAR := 'N';
old_manual_edit CHAR;
old_priority number := 0;
new_priority NUMBER := 0;
old_value FLOAT := 0;
old_overwrite_flag VARCHAR2 (1);
old_validation CHAR;
old_collection_system_id NUMBER;
old_loading_application_id NUMBER;
old_method_id NUMBER;
old_computation_id NUMBER;
epsilon FLOAT := 1E-7;
BEGIN
/* The calling procedure must check that an update is required, and that only one row is affected. */
/* Data priority determination. */
/* Old and new priorities default to 0, if no priorities are found in the following queries,
   then the priority of the old data and new data will be considered equal.
   This means that data from an agency with a defined priority will not overwrite data from an agency without priority.
*/
    begin /* Find if this loading_application is manual */
    select nvl(manual_edit_app,'N')
      into manual_edit
      from hdb_loading_application
      where loading_application_id = loading_application_id_in;
      exception when others then null;
    end;

    IF manual_edit = 'N' then
    begin
	select nvl(priority_rank,0) 
          into new_priority
	  from ref_source_priority
	 where site_datatype_id = SITE_DATATYPE_ID_IN
	   and agen_id = AGEN_ID_IN;
 	exception when others THEN null; /*not an error to not have entry for this agency and sdi*/
    end;
    
    begin /* get the old foreign keys and priorities */
        SELECT nvl(priority_rank,0), a.value, a.overwrite_flag,
               a.validation, a.collection_system_id, a.loading_application_id,
               a.method_id, a.computation_id
    	  INTO old_priority, old_value, old_overwrite_flag,
    	       old_validation, old_collection_system_id, old_loading_application_id,
    	       old_method_id, old_computation_id
    	  FROM r_base a, ref_source_priority b
    	 WHERE a.site_datatype_id = site_datatype_id_in
    	   AND a.INTERVAL = interval_in
    	   AND a.start_date_time = start_date_time_in
    	   and a.end_date_time = END_DATE_TIME_IN
    	   and a.agen_id = b.agen_id(+)
    	   and a.site_datatype_id = b.site_datatype_id(+);
    end;

    begin /* Find if the old loading_application was manual */
    select manual_edit_app
      into old_manual_edit
      from hdb_loading_application
      where loading_application_id = old_loading_application_id;
      exception when others then null;
    end;
    end if;

/* DO THE UPDATE IF:*/
    IF (manual_edit = 'Y') 
        OR (old_manual_edit is null          -- this is a manual edit, or the old data was not a manual edit
	    and (overwrite_flag_in = 'O' or old_overwrite_flag is null) -- it is now an overwrite, or the old was not
	    and (new_priority < old_priority                   -- and the new priority is higher (closer to 0) than the old
                 or (new_priority = old_priority                -- or agencies have same priority
                     AND (abs(old_value - value_in) > epsilon    -- and value is different (difference larger than epsilon!
                          OR overwrite_flag_in != old_overwrite_flag  -- or one of the foreign keys (except agen_id) is different
                          OR validation_in != old_validation
                          or collection_system_id_in != old_collection_system_id
                          OR loading_application_id_in != old_loading_application_id
                          or method_id_in != old_method_id
                          or computation_id_in != old_computation_id)
                    )
                )
            )
    then

    UPDATE R_BASE
       SET value = VALUE_IN,
	   agen_id = AGEN_ID_IN,
	   overwrite_flag = OVERWRITE_FLAG_IN,
	   validation = VALIDATION_IN,
	   collection_system_id = COLLECTION_SYSTEM_ID_IN,
	   loading_application_id = LOADING_APPLICATION_ID_IN,
	   method_id = METHOD_ID_IN,
	   computation_id = COMPUTATION_ID_IN
     WHERE site_datatype_id = SITE_DATATYPE_ID_IN
       AND interval = INTERVAL_IN
       AND start_date_time = START_DATE_TIME_IN
       and end_date_time = END_DATE_TIME_IN;
    else
	NULL; /* update was not done*/
    end if; 
 
END;
/


/* This procedure does not have execute permission granted on purpose
   Only modify_r_base_raw should call it. */

show errors;

