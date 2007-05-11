
CREATE OR REPLACE PROCEDURE get_next_model_run_id
  (is_coord IN NUMBER,
   installation_type IN varchar2,
   model_run_id_out OUT number) IS

  cur_max_uncoord_id      number;
  cur_max_coord_id        number;
  max_coord_id            number;
  local_max_coord_id      number;
  local_min_coord_id      number;
  uncoord_upper_limit     number;

  v_count                 number;
  text                    varchar2(1000);

  e_range_expired         exception;
  PRAGMA EXCEPTION_INIT(e_range_expired, -20102);

BEGIN
  model_run_id_out := -1;

  /* Simplest case -- no checking to do. */
  if (installation_type = 'island') then
    SELECT max(model_run_id) + 1
    INTO model_run_id_out
    FROM ref_model_run;

    /* Handle case of first model_run_id */
    if (model_run_id_out is null) then
      model_run_id_out := 1;
    end if;
	
  /* Not coordinated */
  elsif (is_coord = 0) then

    SELECT max(a.model_run_id)
    INTO cur_max_uncoord_id
    FROM ref_model_run a, hdb_model b
    WHERE a.model_id = b.model_id
      AND b.coordinated = 'N';
   
    SELECT max(max_coord_model_run_id) 
    INTO max_coord_id
    FROM ref_db_list;

    /* Handle case where there are not yet any uncoordinated
       model_run_ids. First case will increment this number by 1. */
    if (cur_max_uncoord_id is null) then
      cur_max_uncoord_id := max_coord_id;
    end if;
    
    /* Easy case, where coordinated IDs are below uncoordinated */
    if (cur_max_uncoord_id >= max_coord_id) then
      model_run_id_out := cur_max_uncoord_id + 1;
    else
      SELECT min(min_coord_model_run_id)
      INTO uncoord_upper_limit
      FROM ref_db_list
      WHERE min_coord_model_run_id > cur_max_uncoord_id;
       
      /* Coordinated IDs have gone beyond uncoordinated, but
         uncoordinated haven't hit their cap yet */
      if (cur_max_uncoord_id+1 < uncoord_upper_limit) then
        model_run_id_out := cur_max_uncoord_id + 1;

      /* Uncoordinated have hit cap and need to go beyond 
	 coordinated */
      else
        model_run_id_out := max_coord_id + 1;
      end if;
    end if;
  else
  /* Coordinated run */
    SELECT min_coord_model_run_id, max_coord_model_run_id
    INTO local_min_coord_id, local_max_coord_id
    FROM ref_db_list
    WHERE session_no = 1;

    SELECT count(model_run_id) 
    INTO v_count
    FROM ref_model_run
    WHERE model_run_id between local_min_coord_id and local_max_coord_id;

    /* Case where we have a new range for coordinated IDs that has
	not yet been used. Set new model run to bottom of range. */
    if (v_count = 0) then
      model_run_id_out := local_min_coord_id;
    else
      /* Get max coordinated model_run_id in use at this site. */
      SELECT max(model_run_id)
      INTO cur_max_coord_id
      FROM ref_model_run
      WHERE model_run_id between local_min_coord_id and local_max_coord_id;

      /* Common case; can assign max+1 for new model_run_id */
      if (cur_max_coord_id < local_max_coord_id) then
        model_run_id_out := cur_max_coord_id + 1;
      else
        /* Raise specific error so message is correct */
        text := 'ERROR: Coordinated model_run_id range for this site is used up. Update min_ and max_coord_model_run_id in ref_db_list to next valid range for this site. Do this on all coordinated databases. Next model_run_id creation will automatically find and use the new range.';
        raise_application_error (-20102, text);
      end if;
    end if; /* v_count = 0 */
  end if; /* installation = island */

  EXCEPTION
   WHEN e_range_expired THEN
     deny_action (text);
   WHEN OTHERS THEN
     text := 'ERROR: '||sqlcode||' '||substr(sqlerrm,1,100)||' when getting next model_run_id.';
     deny_action (text);
END;

/
show errors;

grant execute on get_next_model_run_id to public;
create public synonym get_next_model_run_id for get_next_model_run_id;
