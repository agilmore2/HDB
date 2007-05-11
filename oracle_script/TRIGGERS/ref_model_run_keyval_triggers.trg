-- install the triggers to
--	do archiving on update or delete
--	set user_name and date_time_loaded
--	process coordinated model_runs to remote databases
-- 12/06 Carol Marra
--

create or replace trigger ref_model_run_keyval_before
before insert or update or delete
on ref_model_run_keyval
for each row
when (user <> 'META_DATA_USER')
declare
  v_date                date;
  v_where		varchar2(10);
  v_mri_is_coord        number;
  v_model_id            number;
  v_valid_coord_db      number;
  v_installation_type   varchar2(32);
  text                  varchar2(1000);
begin

  v_date := sysdate;

  if (inserting or updating) then
    :new.date_time_loaded := v_date;
  end if;

  v_where := 'other';

  select meta_data_installation_type
  into v_installation_type
  from ref_installation;

  /* See if this model_run_id is coordinated by checking its model; only 
     need to check master and snapshot installations */
  if (v_installation_type <> 'island') then
    if (inserting or updating) then
        SELECT model_id
        INTO v_model_id
        FROM ref_model_run
        WHERE model_run_id = :new.model_run_id;
    else
        SELECT model_id
        INTO v_model_id
        FROM ref_model_run
        WHERE model_run_id = :old.model_run_id;
    end if;

    model_is_coord (v_model_id, v_mri_is_coord);
  else
     v_mri_is_coord := 0;
  end if;

  if (v_mri_is_coord <> 0) then
      /* Ensure local DB is one of DBs listed for this coordinated model */
      select count(a.db_site_code) 
      into v_valid_coord_db
      from hdb_model_coord a, ref_db_list b
      where a.model_id = v_model_id
	and a.db_site_code = b.db_site_code
	and b.session_no = 1;

      if (v_valid_coord_db = 0) then
        text := 'Local database is not valid for coordinated model '||v_model_id;
	deny_action (text);
      end if;

      /* Process rows into remote database(s) */
      if (inserting) then
	v_where := 'insert';
        insert_coord_model_run_keyval (:new.model_run_id, :new.key_name, 
	  :new.key_value, v_date);
      elsif (updating) then
	v_where := 'update';
        /* Pass in old and new model_run_id so it will be updated 
           on remote DB if someone manually changes a model_run_id
           locally. */
        update_coord_model_run_keyval (:old.model_run_id, :new.model_run_id,
          :old.key_name, :new.key_name, :new.key_value, v_date);
      else
        v_where := 'delete';
	delete_coord_model_run_keyval (:old.model_run_id);
      end if;
  end if; /* is coordinated */

  EXCEPTION
    WHEN others THEN
      if (v_where <> 'other') then
        text := 'Problem on '||v_where||' of coordinated run keyvals. '||sqlerrm;
      else
        text := sqlerrm;
      end if;

      deny_action (text);
  
end;
/
show errors trigger ref_model_run_keyval_before
/

create or replace trigger ref_model_run_key_after_update
after update on ref_model_run_keyval
for each row
begin

-- archive the row that was changed
  insert into ref_model_run_keyval_archive
    (model_run_id,
     key_name,
     key_value,
     date_time_loaded,
     archive_reason,
     date_time_archived)
  values
    (:old.model_run_id,
     :old.key_name,
     :old.key_value,
     :old.date_time_loaded,
     'UPDATE',
     sysdate);
end;
/
show errors trigger ref_model_run_key_after_update;
/

create or replace trigger ref_model_run_key_after_delete
after delete on ref_model_run_keyval
for each row
begin

-- archive the row that was changed
  insert into ref_model_run_keyval_archive
    (model_run_id,
     key_name,
     key_value,
     date_time_loaded,
     archive_reason,
     date_time_archived)
  values
    (:old.model_run_id,
     :old.key_name,
     :old.key_value,
     :old.date_time_loaded,
     'DELETE',
     sysdate);
end;
/
show errors trigger ref_model_run_key_after_delete;
/
