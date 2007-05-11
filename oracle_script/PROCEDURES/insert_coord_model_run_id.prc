CREATE OR REPLACE PROCEDURE insert_coord_model_run_id
  (model_run_id_in              IN number,
   model_run_name_in            IN varchar2,
   model_id_in                  IN number,
   date_time_loaded_in          IN date,
   user_name_in                 IN varchar2,
   extra_keys_in                IN varchar2,
   run_date_in                  IN date,
   start_date_in                IN date,
   end_date_in                  IN date,
   hydrologic_indicator_in      IN varchar2,
   modeltype_in                 IN varchar2,
   time_step_descriptor_in      IN varchar2,
   cmmnt_in                     IN varchar2) IS

  v_count   number;
  e_bad_db  exception;
  PRAGMA EXCEPTION_INIT(e_bad_db, -20102);

  remote_db varchar2(25);
  db_link   varchar2(25);
  ins_stmt  varchar2(1000);
  text      varchar2(1000);

  /* Cursor to get all remote coordinated DBs for
     this model; session_no = 1 is always local db */
  CURSOR remote_coord_dbs IS
  SELECT a.db_site_db_name
  FROM ref_db_list a, hdb_model_coord b
  WHERE b.model_id = model_id_in
   AND a.db_site_code = b.db_site_code
   AND a.session_no <> 1;

BEGIN
  /* Verify that FK constraint is working: make sure DB in hdb_model_coord
     is also in ref_db_list. Error if not. */
  SELECT count(*) 
  INTO v_count
  FROM hdb_model_coord 
  WHERE db_site_code not in (select db_site_code from ref_db_list);

  if (v_count > 0) then
    /* Raise specific error so message is correct */
    text := 'Problem on insert: model '||model_id_in||' lists coord DB not in ref_db_list.';
    raise_application_error (-20102, text);
  end if;

  FOR db_link IN remote_coord_dbs LOOP
    remote_db := db_link.db_site_db_name;

    ins_stmt := 'INSERT INTO ref_model_run@'||remote_db||' (model_run_id, model_run_name, model_id, date_time_loaded, user_name, extra_keys_y_n, run_date, start_date, end_date, hydrologic_indicator, modeltype, time_step_descriptor, cmmnt) VALUES (:1,:2,:3,:4,:5,:6, :7, :8, :9, :10, :11, :12, :13)';

    EXECUTE IMMEDIATE ins_stmt USING model_run_id_in, model_run_name_in, model_id_in, date_time_loaded_in, user_name_in, extra_keys_in, run_date_in, start_date_in, end_date_in, hydrologic_indicator_in, modeltype_in, time_step_descriptor_in, cmmnt_in;

  END LOOP;

  EXCEPTION
   WHEN e_bad_db THEN
     deny_action (text);
   WHEN OTHERS THEN 
     text := sqlerrm||' when trying to insert at '||remote_db;
     deny_action (text);
END;

/
show errors;

grant execute on insert_coord_model_run_id to model_priv_role;
create public synonym insert_coord_model_run_id for insert_coord_model_run_id;
