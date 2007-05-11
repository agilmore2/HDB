-- install the triggers to
--	do archiving on update or delete
--	set user_name and date_time_loaded
--	process coordinated model_runs to remote databases
-- 12/06 Carol Marra
--

create or replace trigger ref_model_run_before
before insert or update or delete
on ref_model_run
for each row
when (user <> 'META_DATA_USER')
declare
  v_user	     	varchar2(30);
  v_date		date;
  v_where		varchar2(10);
  v_mri_is_coord        number;
  v_model_id            number;
  v_valid_coord_db      number;
  v_installation_type   varchar2(32);
  v_count               number;
  new_model_run_id      number;
  text                  varchar2(1000);
begin

  /* Set new values for date_time_loaded and user_name */
  v_user := user;
  v_date := sysdate;

  if (inserting or updating) then
    :new.date_time_loaded := v_date;
    :new.user_name := v_user;
  end if;

  v_where := 'other';

  /* Do not allow setting extra_keys_y_n to N when
     there are key-value pairs */
  if (updating AND upper(:new.extra_keys_y_n) = 'N') then
    select count(*)
    into v_count
    from ref_model_run_keyval
    where model_run_id = :new.model_run_id;

    if (v_count > 0) then
      deny_action ('You must delete all key-value pairs before setting extra_keys_y_n to N');
    end if;
  end if;

  select meta_data_installation_type
  into v_installation_type
  from ref_installation;

  /* See if this model_run_id is coordinated by checking its model; only 
     need to check master and snapshot installations */
  if (v_installation_type <> 'island') then
    if (inserting or updating) then
        v_model_id := :new.model_id;
    else
        v_model_id := :old.model_id;
    end if;

    model_is_coord (v_model_id, v_mri_is_coord);
  else
     v_mri_is_coord := 0;
  end if;

  /* Get next model_run_id; depends on if coordinated or not */
  if (inserting) then
    get_next_model_run_id (v_mri_is_coord, v_installation_type, new_model_run_id);
    :new.model_run_id := new_model_run_id;
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
        insert_coord_model_run_id (:new.model_run_id, :new.model_run_name, 
	  :new.model_id, v_date, v_user, :new.extra_keys_y_n, :new.run_date, 
          :new.start_date, :new.end_date,
	  :new.hydrologic_indicator, :new.modeltype, :new.time_step_descriptor,
	  :new.cmmnt);
      elsif (updating) then
	v_where := 'update';
        /* Pass in old and new model_run_id so it will be updated 
           on remote DB if someone manually changes a model_run_id
           locally. */
        update_coord_model_run_id (:old.model_run_id, :new.model_run_id,
          :new.model_run_name, :new.model_id, v_date, v_user, 
          :new.extra_keys_y_n, :new.run_date, :new.start_date, :new.end_date,
	  :new.hydrologic_indicator, :new.modeltype, :new.time_step_descriptor,
	  :new.cmmnt);
      else
        v_where := 'delete';
	delete_coord_model_run_id (:old.model_run_id, :old.model_id);
      end if;
  end if; /* is coordinated */

  EXCEPTION
    WHEN others THEN
      if (v_where <> 'other') then
        text := 'Problem on '||v_where||' of coord run. '||sqlerrm;
      else
        text := sqlerrm;
      end if;

      deny_action (text);
  
end;
/
show errors trigger ref_model_run_before
/

create or replace trigger ref_model_run_after_update
after update on ref_model_run
for each row
begin

-- archive the row that was changed
  insert into ref_model_run_archive
    (model_run_id,
     model_run_name,
     model_id,
     date_time_loaded,
     user_name,
     extra_keys_y_n,
     run_date,
     start_date,
     end_date,
     hydrologic_indicator,
     modeltype,
     time_step_descriptor,
     cmmnt,
     archive_reason,
     date_time_archived)
  values
    (:old.model_run_id,
     :old.model_run_name,
     :old.model_id,
     :old.date_time_loaded,
     :old.user_name,
     :old.extra_keys_y_n,
     :old.run_date,
     :old.start_date,
     :old.end_date,
     :old.hydrologic_indicator,
     :old.modeltype,
     :old.time_step_descriptor,
     :old.cmmnt,
     'UPDATE',
     sysdate);
end;
/
show errors trigger ref_model_run_after_update;
/

create or replace trigger ref_model_run_after_delete
after delete on ref_model_run
for each row
begin

-- archive the row that was changed
  insert into ref_model_run_archive
    (model_run_id,
     model_run_name,
     model_id,
     date_time_loaded,
     user_name,
     extra_keys_y_n,
     run_date,
     start_date,
     end_date,
     hydrologic_indicator,
     modeltype,
     time_step_descriptor,
     cmmnt,
     archive_reason,
     date_time_archived)
  values
    (:old.model_run_id,
     :old.model_run_name,
     :old.model_id,
     :old.date_time_loaded,
     :old.user_name,
     :old.extra_keys_y_n,
     :old.run_date,
     :old.start_date,
     :old.end_date,
     :old.hydrologic_indicator,
     :old.modeltype,
     :old.time_step_descriptor,
     :old.cmmnt,
     'DELETE',
     sysdate);
end;
/
show errors trigger ref_model_run_after_delete;
/
