/* Will create a new model_run_id with the given attributes.
   The newly-assigned model_run_id is returned in the output
   variable model_run_id_out. Date_time_loaded and user_name
   are set automatically by the table-level trigger.

   Num_extra_keys is the number of key-value pairs this model_run_id
   will be created with.

   Coordinated model_run_ids are handled at the trigger level;
   that is, created as needed.
*/

create or replace PROCEDURE create_model_run_id ( 
   model_run_id_out             OUT number,
   model_run_name_in            IN varchar2,
   model_id_in                  IN number,
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
  key_name varchar2(32);
  key_value varchar2(32);
  null_date date;

  text varchar2(1000);
BEGIN
  /*  First check for inappropriate NULL values */
  if (model_run_name_in is null) then
      deny_action ( 'Invalid <NULL> model_run_name');
  elsif (model_id_in is null) then
      deny_action ( 'Invalid <NULL> model_id');
  elsif (run_date_in is null) then
      deny_action ( 'Invalid <NULL> run_date');
  elsif (num_extra_keys is null) then
      deny_action ( 'Invalid <NULL> num_extra_keys');
  end if;

  null_date := to_date ('01-jan-1900','dd-mon-yyyy');

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
      deny_action ('Extra_keys string does not appear to contain enough key=value pairs; not enough = signs. MRI will not be created.');  
    end if;

    /* Too many pairs? */
    select instr(extra_keys, '=', 1, num_extra_keys+1)
    into equals_check
    from dual;

    if (equals_check > 0) then
      deny_action ('There appears to be more than '||num_extra_keys||' key=value pairs in extra_keys. MRI will not be created.');
    end if;

    /* Not enough delineation? */
    if (num_extra_keys > 1) then
      select instr(extra_keys, ',', 1, num_extra_keys-1)
      into comma_check
      from dual;

      if (comma_check = 0) then
        deny_action ('Cannot delineate key=value pairs in extra_keys string; not enough commas between pairs. MRI will not be created.');  
      end if;
    end if;
  end if; /* num_extra_keys = 0 */

  dbms_output.put_line('RUN: '|| to_char(to_date(run_date_in,'dd-mon-yyyy hh24:mi:ss'),'dd-mon-yyyy hh24:mi:ss'));
  dbms_output.put_line('RUN: '|| to_char(to_date(start_date_in,'dd-mon-yyyy hh24:mi:ss'),'dd-mon-yyyy hh24:mi:ss'));
  dbms_output.put_line('RUN: '|| to_char(to_date(end_date_in,'dd-mon-yyyy hh24:mi:ss'),'dd-mon-yyyy hh24:mi:ss'));

  /* Continue with creating model_run_id. */
  insert into ref_model_run (
    model_run_id,
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
    cmmnt)
   values (
    0,
    model_run_name_in,
    model_id_in,
    null,
    null,
    decode (num_extra_keys, 0, 'N', 'Y'),
    run_date_in,
    start_date_in,
    end_date_in,
    hydrologic_indicator_in,
    modeltype_in,
    time_step_descriptor_in,
    cmmnt_in);

/*    to_date(run_date_in,'dd-mon-yyyy hh24:mi:ss'),
    to_date(start_date_in,'dd-mon-yyyy hh24:mi:ss'),
    to_date(end_date_in,'dd-mon-yyyy hh24:mi:ss'),
*/
  dbms_output.put_line ('GOT PAST CREATE');

  get_just_created_model_run_id (model_run_id_out, model_run_name_in,
    model_id_in, run_date_in, num_extra_keys, start_date_in, end_date_in,
    hydrologic_indicator_in, modeltype_in, time_step_descriptor_in, cmmnt_in);

  /* Insert key-value pairs */
  if (num_extra_keys <> 0) then
    /* Parse extra_keys and insert row for each pair; if there are
       extra key=value pairs, the MRI will not be created. */
    start_pos := 1;
    for i in 1..num_extra_keys loop
      select instr(extra_keys, '=', 1, i)
      into equals_pos 
      from dual;

      str_size := equals_pos - start_pos;
      key_name := substr (extra_keys, start_pos, str_size);

      if (i < num_extra_keys) then
        select instr(extra_keys, ',', 1, i)
        into comma_pos
        from dual;

        str_size := comma_pos - equals_pos - 1;
        key_value := substr (extra_keys, equals_pos + 1, str_size);
      else
        /* Get the last key_value */
        str_size := length(extra_keys) - equals_pos;
        key_value := substr (extra_keys, equals_pos + 1, str_size);
      end if;

      insert into ref_model_run_keyval (
        model_run_id,
        key_name,
        key_value,
        date_time_loaded)
      values
        (model_run_id_out,
         key_name,
         key_value,
         null);

      start_pos := comma_pos + 1;
    end loop;
  end if; /* num_extra_keys = 0 */

  /* Got through all inserts, local and remote; OK to commit */
  commit;

  EXCEPTION
   WHEN OTHERS THEN 
     rollback;
     text := sqlerrm||' when trying to create new model_run_id';
     deny_action (text);

end;
/

show errors;
/
create public synonym create_model_run_id for create_model_run_id;
grant execute on create_model_run_id to model_priv_role;


