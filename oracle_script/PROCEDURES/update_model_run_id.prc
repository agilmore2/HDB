/* Will update all attributes of a model_run_id except for
   the model_run_id itself and the model_id. All null-supplied
   attributes remain unchanged. Date_time_loaded and user_name
   are automatically updated by the table-level trigger.

   Note that the parameter num_extra_keys indicates the number
   of key-value pairs being updated, which may or may not be
   the number of key-value pairs associated with the model_run_id.
   Because the model_run_id in question is known -- passed in --
   the number of key-value pairs for the model_run_id is not needed
   to identify it. Only the key_value can be updated, not the key_name.
   The key_name in the extra_keys string specifies which key is to
   be updated.

   Coordinated model_run_ids are handled at the trigger level;
   that is, updated as needed.

   Because this procedure does not allow the addition of new
   key-value pairs, by definition it does not update the value
   of extra_keys_y_n.
*/

create or replace PROCEDURE update_model_run_id ( 
   model_run_id_in              IN number,
   model_run_name_in            IN varchar2,
   run_date_in                  IN date,
   num_extra_keys               IN number,
   start_date_in                IN date,
   end_date_in                  IN date,
   hydrologic_indicator_in      IN varchar2,
   modeltype_in                 IN varchar2,
   time_step_descriptor_in      IN varchar2,
   cmmnt_in                     IN varchar2,
   extra_keys                   IN varchar2)
IS
  equals_check number;
  comma_check number;
  start_pos number;
  equals_pos number;
  comma_pos number;
  str_size number;
  i number;
  v_count number;
  v_key_name varchar2(32);
  v_key_value varchar2(32);

  mr_name  varchar2(100);
  run_dt   varchar2(100);
  st_dt    varchar2(100);
  end_dt   varchar2(100);
  hyd_ind  varchar2(100);
  modtype  varchar2(100);
  tmstp    varchar2(100);
  cmt      varchar2(100);
  upd_stmt varchar2(2000);
  upd_keyval_stmt varchar2(2000);

  e_no_match  exception;
  PRAGMA EXCEPTION_INIT(e_no_match, -20102);

  text varchar2(1000);
BEGIN
  /*  First check for inappropriate NULL values */
  if (model_run_id_in is null) then
    deny_action ( 'Invalid <NULL> model_run_id');
  end if;

  /* Before going any further, make sure this model_run_id
     exists. If not, send back an error message. */
  select count(*)
  into v_count
  from ref_model_run
  where model_run_id = model_run_id_in;

  if (v_count = 0) then
    text := 'WARNING! There is no model_run_id = '||model_run_id_in||'. No update processed.';
    raise_application_error (-20102, text);
  end if;

  /* Check consistency of extra_keys specification */
  if (num_extra_keys = 0) then
    if (extra_keys is not null) then
      deny_action ('Extra_keys must be NULL when num_extra_keys = 0');
    end if;
  else
    /* Do some checks on the extra_keys string to see if it looks valid */
    /* Not enough pairs? */
    select instr(extra_keys, '=', 1, num_extra_keys)
    into equals_check
    from dual;

    if (equals_check = 0) then
      deny_action ('Extra_keys string does not appear to contain enough key=value pairs; not enough = signs. MRI will not be updated.');  
    end if;

    /* Too many pairs? */
    select instr(extra_keys, '=', 1, num_extra_keys+1)
    into equals_check
    from dual;

    if (equals_check > 0) then
      deny_action ('There appears to be more than '||num_extra_keys||' key=value pairs in extra_keys. MRI will not be updated.');
    end if;

    /* Not enough delineation? */
    if (num_extra_keys > 1) then
      select instr(extra_keys, ',', 1, num_extra_keys-1)
      into comma_check
      from dual;

      if (comma_check = 0) then
        deny_action ('Cannot delineate key=value pairs in extra_keys string; not enough commas between pairs. MRI will not be updated.');  
      end if;
    end if;
  end if; /* num_extra_keys = 0 */

  /* Continue with updating model_run_id. Need to create a statement
     based on values passed in. */
  if (model_run_name_in is null) then
    mr_name := ' model_run_name = model_run_name,';
  else
    mr_name := ' model_run_name = '''||model_run_name_in||''',';
  end if;

  if (run_date_in is null) then
    run_dt := ' run_date = run_date,';
  else
    run_dt := ' run_date = to_date('''||run_date_in||''',''dd-mon-yyyy hh24:mi:ss''),';
  end if;

  if (start_date_in is null) then
    st_dt := ' start_date = start_date,';
  else
    st_dt := ' start_date = to_date('''||start_date_in||''',''dd-mon-yyyy hh24:mi:ss''),';
  end if;

  if (end_date_in is null) then
    end_dt := ' end_date = end_date,';
  else
    end_dt := ' end_date = to_date('''||end_date_in||''',''dd-mon-yyyy hh24:mi:ss''),';
  end if;

  if (hydrologic_indicator_in is null) then 
    hyd_ind := ' hydrologic_indicator = hydrologic_indicator,';
  else
    hyd_ind := ' hydrologic_indicator = '''||hydrologic_indicator_in||''',';
  end if;

  if (modeltype_in is null) then 
    modtype := ' modeltype = modeltype,';
  else
    modtype := ' modeltype = '''||modeltype_in||''',';
  end if;

  if (time_step_descriptor_in is null) then 
    tmstp := ' time_step_descriptor = time_step_descriptor,';
  else
    tmstp := ' time_step_descriptor = '''||time_step_descriptor_in||''',';
  end if;

  if (cmmnt_in is null) then 
    cmt := ' cmmnt = cmmnt';
  else
    cmt := ' cmmnt = '''||cmmnt_in||'''';
  end if;

  upd_stmt := 'UPDATE ref_model_run SET'||mr_name||run_dt||st_dt||end_dt||hyd_ind||modtype||tmstp||cmt||' WHERE model_run_id = :1';

  EXECUTE IMMEDIATE upd_stmt USING model_run_id_in;

  /* Update key-value pairs */
  if (num_extra_keys <> 0) then
    /* Parse extra_keys and update row for each pair; if there are
       extra key=value pairs, the MRI will not be updated. */
    start_pos := 1;
    for i in 1..num_extra_keys loop
      select instr(extra_keys, '=', 1, i)
      into equals_pos 
      from dual;

      str_size := equals_pos - start_pos;
      v_key_name := substr (extra_keys, start_pos, str_size);

      if (i < num_extra_keys) then
        select instr(extra_keys, ',', 1, i)
        into comma_pos
        from dual;

        str_size := comma_pos - equals_pos - 1; 
        v_key_value := substr (extra_keys, equals_pos + 1, str_size);
      else
        /* Get the last key_value */
        str_size := length(extra_keys) - equals_pos;
        v_key_value := substr (extra_keys, equals_pos + 1, str_size);
      end if;

      /* Before going any further, make sure this key_name
         exists for this model_run_id. If not, send back an error message. */
      select count(*)
      into v_count
      from ref_model_run_keyval
      where model_run_id = model_run_id_in
        and key_name = v_key_name;

      if (v_count = 0) then
        text := 'WARNING! There is no key_name = '''||v_key_name||''' for model_run_id '||model_run_id_in||'. No update processed.';
        raise_application_error (-20102, text);
      end if;

      upd_keyval_stmt := 'UPDATE ref_model_run_keyval SET key_value = '''||v_key_value||''' WHERE model_run_id = :2 and key_name = '''||v_key_name||'''';
      EXECUTE IMMEDIATE upd_keyval_stmt USING model_run_id_in;

      start_pos := comma_pos + 1;
    end loop;
  end if; /* num_extra_keys <> 0 */

  /* Got through all updates, local and remote; OK to commit */
  commit;

  EXCEPTION
   WHEN e_no_match THEN
     deny_action (text);
   WHEN OTHERS THEN 
     rollback;
     text := sqlerrm||' when trying to update MRI '||model_run_id_in;
     deny_action (text);

end;
/

show errors;
/
create public synonym update_model_run_id for update_model_run_id;
grant execute on update_model_run_id to model_priv_role;


