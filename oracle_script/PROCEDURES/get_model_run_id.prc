/* Will get and return a model_run_id based on the supplied
   parameter values, including key-value pairs.

   All parameters can be null except model_id_in.

   If date_time_loaded_in is null, the procedure returns
   the model_run_id with the most recent date_time_loaded
   that matches the other input specifications. 

   For dates that are specified, if the hh24:mi:ss portion
   of the input date is 00:00:00 (either specified as 00:00:00,
   or defaulting to it), then the database date will be matched 
   to the day. If the hh24:mi:ss portion of the input date is
   non-0, then the database date will be matched on the full
   (dd-mon-yyyy hh24:mi:ss) date.

   If num_extra_keys = -1, the procedure does not care how
   many extra keys, if any, the model_run_id has, so long
   as other parameters match. If num_extra_keys = 0, the
   procedure will return only a model_run_id with 0 extra keys
   where all other input specifications are a match. If 
   num_extra_keys > 0, the procedure will return only a 
   model_run_id with exactly n extra keys, where the key names
   and values match those specified in extra_keys, and where all 
   other input specifications are a match.

   The only time that finding more than one matching model_run_id
   is acceptable is in the case where date_time_loaded is left
   null, and the most recently loaded model_run_id is returned. In
   all other cases, finding more than one match causes an error.

   Finding no matches always causes an error.

 */

create or replace PROCEDURE get_model_run_id ( 
   model_run_id_out             OUT number,
   model_run_name_in            IN varchar2,
   model_id_in                  IN number,
   date_time_loaded_in          IN date,
   user_name_in                 IN varchar2,
   num_extra_keys               IN number,
   run_date_in                  IN date,
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
  v_key_name varchar2(32);
  v_key_value varchar2(32);

  mr_name  varchar2(100);
  rn_dt    varchar2(100);
  usr_name varchar2(100);
  st_dt    varchar2(100);
  end_dt   varchar2(100);
  hyd_ind  varchar2(100);
  modtype  varchar2(100);
  tmstp    varchar2(200);
  cmt      varchar2(2000);

  sel_stmt varchar2(1000);
  where_stmt varchar2(1000);
  not_exists_stmt varchar2(1000);
  key_value_stmt varchar2(1000);  
  mri_stmt varchar2(1000);  
  dt_load  varchar2(2000);
  qry_stmt varchar2(2000);

  one_quote varchar2(1);
  two_quotes varchar2(2);
 
  dt_load_out   DATE;
  text varchar2(1000);
BEGIN
  one_quote := '''';
  two_quotes := '''''';

  model_run_id_out := -1;

  /*  First check for inappropriate NULL values */
  if (model_id_in is null) then
    deny_action ( 'Invalid <NULL> model_id');
  end if;

  if (num_extra_keys is null) then
    deny_action ( 'Invalid <NULL> num_extra_keys');
  end if;

  /* Check consistency of extra_keys specification */
  if (num_extra_keys = 0 or num_extra_keys = -1) then
    if (extra_keys is not null) then
      deny_action ('Extra_keys must be NULL when num_extra_keys = 0 or -1');
    end if;
  else
    /* Do some checks on the extra_keys string to see if it looks valid */
    /* Not enough pairs? */
    select instr(extra_keys, '=', 1, num_extra_keys)
    into equals_check
    from dual;

    if (equals_check = 0) then
      deny_action ('Extra_keys string does not appear to contain enough key=value pairs; not enough = signs.');  
    end if;

    /* Too many pairs? */
    select instr(extra_keys, '=', 1, num_extra_keys+1)
    into equals_check
    from dual;

    if (equals_check > 0) then
      deny_action ('There appears to be more than '||num_extra_keys||' key=value pairs in extra_keys.');
    end if;

    /* Not enough delineation? */
    if (num_extra_keys > 1) then
      select instr(extra_keys, ',', 1, num_extra_keys-1)
      into comma_check
      from dual;

      if (comma_check = 0) then
        deny_action ('Cannot delineate key=value pairs in extra_keys string; not enough commas between pairs.');  
      end if;
    end if;
  end if; /* num_extra_keys = 0 */

  /* Continue with finding model_run_id. Need to create a statement
     based on values passed in. */

  /* If date_time_loaded is null, exclude date_time_loaded clause;
     The ORDER BY date_time_loaded descending will get largest /
     most recent date_time_loaded row first, and the exception
     handler will allow this row in, if indeed there is more than
     one match and date_time_loaded is important. */
  if (date_time_loaded_in is null) then
    dt_load := ' ';
  elsif (to_char(date_time_loaded_in,'hh24:mi:ss') = '00:00:00') then
    dt_load := ' and trunc(a.date_time_loaded) = to_date('''||to_char(date_time_loaded_in,'dd-mon-yyyy')||''',''dd-mon-yyyy'')';
  else
    dt_load := ' and a.date_time_loaded = to_date('''||to_char(date_time_loaded_in,'dd-mon-yyyy hh24:mi:ss')||''',''dd-mon-yyyy hh24:mi:ss'')';
  end if;

  if (model_run_name_in is null) then
    mr_name := ' ';
  else
    mr_name := ' and lower(model_run_name) like lower(''%'||replace(model_run_name_in,one_quote,two_quotes)||'%'')';
  end if;

  if (user_name_in is null) then
    usr_name := ' ';
  else
    usr_name := ' and lower(user_name) like lower(''%'||replace(user_name_in,one_quote,two_quotes)||'%'')';
  end if;

  if (run_date_in is null) then
    rn_dt := ' ';
  elsif (to_char(run_date_in,'hh24:mi:ss') = '00:00:00') then
    rn_dt := ' and trunc(a.run_date) = to_date('''||to_char(run_date_in,'dd-mon-yyyy')||''',''dd-mon-yyyy'')';
  else
    rn_dt := ' and a.run_date = to_date('''||to_char(run_date_in,'dd-mon-yyyy hh24:mi:ss')||''',''dd-mon-yyyy hh24:mi:ss'')';
  end if;

  if (start_date_in is null) then
    st_dt := ' ';
  elsif (to_char(start_date_in,'hh24:mi:ss') = '00:00:00') then
    st_dt := ' and trunc(a.start_date) = to_date('''||to_char(start_date_in,'dd-mon-yyyy')||''',''dd-mon-yyyy'')';
  else
    st_dt := ' and a.start_date = to_date('''||to_char(start_date_in,'dd-mon-yyyy hh24:mi:ss')||''',''dd-mon-yyyy hh24:mi:ss'')';
  end if;

  if (end_date_in is null) then
    end_dt := ' ';
  elsif (to_char(end_date_in,'hh24:mi:ss') = '00:00:00') then
    end_dt := ' and trunc(a.end_date) = to_date('''||to_char(end_date_in,'dd-mon-yyyy')||''',''dd-mon-yyyy'')';
  else
    end_dt := ' and a.end_date = to_date('''||to_char(end_date_in,'dd-mon-yyyy hh24:mi:ss')||''',''dd-mon-yyyy hh24:mi:ss'')';
  end if;

  if (hydrologic_indicator_in is null) then 
    hyd_ind := ' ';
  else
    hyd_ind := ' and lower(hydrologic_indicator) like lower(''%'||replace(hydrologic_indicator_in,one_quote,two_quotes)||'%'')';
  end if;

  if (modeltype_in is null) then 
    modtype := ' ';
  else
    modtype := ' and modeltype = '''||modeltype_in||'''';
  end if;

  if (time_step_descriptor_in is null) then 
    tmstp := ' ';
  else
    tmstp := ' and lower(time_step_descriptor) like lower(''%'||replace(time_step_descriptor_in,one_quote,two_quotes)||'%'')';
  end if;

  if (cmmnt_in is null) then 
    cmt := ' ';
  else
    cmt := ' and lower(cmmnt) like lower(''%'||replace(cmmnt_in,one_quote,two_quotes)||'%'')';
  end if;

  /* Build query statement for when there are no extra keys... */
  if (num_extra_keys = 0) then
    sel_stmt := 'SELECT a.model_run_id, a.date_time_loaded FROM ref_model_run a';
    where_stmt := ' WHERE model_id = :1 and extra_keys_y_n = ''N'' '||mr_name||usr_name||rn_dt||st_dt||end_dt||hyd_ind||modtype||tmstp||cmt;

    not_exists_stmt := ' ';
    key_value_stmt :=  ' ';
    mri_stmt := ' ';
  /* and when we don't care if there are extra keys */
  elsif (num_extra_keys = -1) then
    sel_stmt := 'SELECT a.model_run_id, a.date_time_loaded FROM ref_model_run a';
    where_stmt := ' WHERE model_id = :1 '||mr_name||usr_name||rn_dt||st_dt||end_dt||hyd_ind||modtype||tmstp||cmt;

    not_exists_stmt := ' ';
    key_value_stmt :=  ' ';
    mri_stmt := ' ';
  /* and when there are extra keys... */
  else
    sel_stmt := 'SELECT a.model_run_id, a.date_time_loaded FROM ref_model_run a';
    where_stmt := ' WHERE model_id = :1 and extra_keys_y_n = ''Y'' and a.model_run_id = key1.model_run_id'||mr_name||usr_name||rn_dt||st_dt||end_dt||hyd_ind||modtype||tmstp||cmt;
    not_exists_stmt := ' and not exists (select count(z.model_run_id) from ref_model_run_keyval z where z.model_run_id = key1.model_run_id having count(z.model_run_id) <> '||num_extra_keys||')';

    /* Parse extra_keys and build key_value part of query */
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

      sel_stmt := concat (sel_stmt, ', ref_model_run_keyval key'||i);
      key_value_stmt := concat (key_value_stmt, ' and key'||i||'.key_name = '''||replace(v_key_name,one_quote,two_quotes)||''' and key'||i||'.key_value = '''||replace(v_key_value,one_quote,two_quotes)||'''');
      if (i > 1) then
        mri_stmt := concat (mri_stmt, ' and key'||i||'.model_run_id = key'||to_char(i-1)||'.model_run_id');
      end if;

      start_pos := comma_pos + 1;
    end loop;

  end if; /* num_extra_keys = 0 */

  /* Add the order by clause so that most recent date_time_loaded
     is guaranteed to be first row returned. */
  qry_stmt := sel_stmt || where_stmt || key_value_stmt || mri_stmt || not_exists_stmt || dt_load ||' order by a.date_time_loaded desc';
 

  /* Execute query */
  EXECUTE IMMEDIATE qry_stmt INTO model_run_id_out, dt_load_out
   USING model_id_in;

  EXCEPTION
   WHEN no_data_found THEN
     deny_action ('No model_run_ids match the input specifications.'); 
   WHEN too_many_rows THEN
     /* Error if we're not getting most recent date_time_loaded where
        there may be more than one match. */
     if (date_time_loaded_in is not null) then 
       deny_action ('More than one model_run_id matches the input specifications.'); 
     end if; 
   WHEN OTHERS THEN 
     rollback;
     text := sqlerrm||' when trying to get model_run_id.';
     deny_action (text);

end;
/

show errors;
/
create public synonym get_model_run_id for get_model_run_id;
grant execute on get_model_run_id to model_priv_role;


