CREATE OR REPLACE PROCEDURE delete_coord_model_run_keyval
  (model_run_id_in              IN number) IS

  v_count   number;
  e_bad_db  exception;
  PRAGMA EXCEPTION_INIT(e_bad_db, -20102);

  remote_db varchar2(25);
  db_link   varchar2(25);
  del_stmt  varchar2(1000);
  text      varchar2(1000);

  /* Cursor to get all remote coordinated DBs for
     this model; session_no = 1 is always local db */
  CURSOR remote_coord_dbs IS
  SELECT a.db_site_db_name
  FROM ref_db_list a, hdb_model_coord b, ref_model_run c
  WHERE c.model_run_id = model_run_id_in
   and b.model_id = c.model_id
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
    text := 'Problem on delete: model for MRI '||model_run_id_in||' lists coord DB not in ref_db_list.';
    raise_application_error (-20102, text);
  end if;

  FOR db_link IN remote_coord_dbs LOOP
    remote_db := db_link.db_site_db_name;

    del_stmt := 'DELETE FROM ref_model_run_keyval@'||remote_db||' WHERE model_run_id = :1';

    EXECUTE IMMEDIATE del_stmt USING model_run_id_in;

  END LOOP;

  EXCEPTION
   WHEN e_bad_db THEN
     deny_action (text);
   WHEN OTHERS THEN 
     text := sqlerrm||' when trying to delete at '||remote_db;
     deny_action (text);
END;

/
show errors;

grant execute on delete_coord_model_run_keyval to model_priv_role;
create public synonym delete_coord_model_run_keyval for delete_coord_model_run_keyval;
