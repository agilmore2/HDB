CREATE OR REPLACE PROCEDURE update_coord_mri_kv_arch_cmmnt
  (model_run_id_in              IN number,
   key_name_in                  IN varchar2,
   archive_cmmnt_in             IN varchar2) IS

  remote_db varchar2(25);
  db_link   varchar2(25);
  upd_stmt  varchar2(1000);
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
  FOR db_link IN remote_coord_dbs LOOP
    remote_db := db_link.db_site_db_name;

    upd_stmt := 'UPDATE ref_model_run_keyval_archive@'||remote_db||' SET archive_cmmnt=:1 WHERE model_run_id=:2 and key_name =:3 and date_time_archived = (select max(date_time_archived) from ref_model_run_keyval_archive@'||remote_db||' WHERE model_run_id =:4 and key_name =:5)';

    EXECUTE IMMEDIATE upd_stmt USING archive_cmmnt_in, model_run_id_in, key_name_in, model_run_id_in, key_name_in;

  END LOOP;

  EXCEPTION
   WHEN OTHERS THEN 
     text := sqlerrm||' when trying to update MRI keyval archive_cmmnt at '||remote_db;
     deny_action (text);
END;

/
show errors;

grant execute on update_coord_mri_kv_arch_cmmnt to model_priv_role;
create public synonym update_coord_mri_kv_arch_cmmnt for update_coord_mri_kv_arch_cmmnt;
