create or replace PROCEDURE get_site_data_map ( 
                              EXT_DATA_SOURCE_ID_IN NUMBER,
			      PRIMARY_SITE_CODE_IN VARCHAR2,
			      PRIMARY_DATA_CODE_IN VARCHAR2,
                              NUM_EXTRA_KEYS NUMBER,
                              HDB_SITE_DATATYPE_ID_OUT OUT NUMBER,
			      HDB_INTERVAL_NAME_OUT OUT VARCHAR2,
                              HDB_METHOD_ID_OUT OUT NUMBER,
                              HDB_COMPUTATION_ID_OUT OUT NUMBER,
                              HDB_AGEN_ID_OUT OUT NUMBER,
                              IS_ACTIVE VARCHAR2,
                              EXTRA_KEYS VARCHAR2)
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
  map_count number;
  sel_count_stmt varchar2(2000);
  sel_stmt varchar2(2000);
  where_stmt varchar2(2000);
  active_stmt varchar2(2000);
  not_exists_stmt varchar2(2000);
  key_value_stmt varchar2(2000);
  map_id_stmt varchar2(2000);
  count_stmt varchar2(2000);
  whole_stmt varchar2(2000);
  is_active_y_n_in varchar2(1);
  active_ind varchar2(10);
BEGIN

  if (num_extra_keys is null) then
      deny_action ( 'Invalid <NULL> num_extra_keys');
  end if;

  /* if is_active is null, set it to y */
  if (is_active is null) then
    is_active_y_n_in := 'y';
  else
    is_active_y_n_in := is_active;
  end if;

  /* set is_active indicator string */
  if (lower(is_active_y_n_in) = 'y') then
    active_ind := 'active ';
  elsif (lower(is_active_y_n_in) = 'n') then
    active_ind := 'inactive ';
  else
    active_ind := NULL;
  end if;

  /* If there are no extra keys, get lookup values from ref_ext_site_data_map */
  if (num_extra_keys = 0) then
    select count(mapping_id)    
    into map_count
    from ref_ext_site_data_map
    where ext_data_source_id = ext_data_source_id_in
      and primary_site_code = primary_site_code_in
      and primary_data_code = primary_data_code_in
      and lower (extra_keys_y_n) = 'n'
      and ((lower (is_active_y_n_in) <> 'a'
            and lower (is_active_y_n) = lower(is_active_y_n_in)) OR
           (lower (is_active_y_n_in) = 'a'));

    if (map_count = 1) then
      select hdb_site_datatype_id, hdb_interval_name, hdb_method_id,
             hdb_computation_id, hdb_agen_id
      into hdb_site_datatype_id_out, hdb_interval_name_out, hdb_method_id_out,
           hdb_computation_id_out, hdb_agen_id_out
      from ref_ext_site_data_map
      where ext_data_source_id = ext_data_source_id_in
        and primary_site_code = primary_site_code_in
        and primary_data_code = primary_data_code_in
        and lower (extra_keys_y_n) = 'n'
        and ((lower (is_active_y_n_in) <> 'a'
              and lower (is_active_y_n) = lower(is_active_y_n_in)) OR
             (lower (is_active_y_n_in) = 'a'));
      
    elsif (map_count = 0) then
      deny_action('No '||active_ind ||'mappings with source='||ext_data_source_id_in||', site='||primary_site_code_in||', data='||primary_data_code_in);
    else
      deny_action('Too many mappings ('||map_count||') with source='||ext_data_source_id_in||', site='||primary_site_code_in||', data='||primary_data_code_in);
    end if;

  /* Lookup mapping where there are extra keys */
  else
    /* Do some checks on the extra_keys string to see if it looks valid */
    /* Not enough pairs? */
    select instr(extra_keys, '=', 1, num_extra_keys)
    into equals_check
    from dual;

    if (equals_check = 0) then
      deny_action ('Extra_keys string does not appear to contain enough key=value pairs; not enough = signs. Cannot retrieve mapping.');  
    end if;

    /* Too many pairs? */
    select instr(extra_keys, '=', 1, num_extra_keys+1)
    into equals_check
    from dual;

    if (equals_check > 0) then
      deny_action ('There appear to be more than '||num_extra_keys||' key=value pairs in extra_keys. Cannot retrieve mapping.');
    end if;

    /* Not enough delineation? */
    if (num_extra_keys > 1) then
      select instr(extra_keys, ',', 1, num_extra_keys-1)
      into comma_check
      from dual;

      if (comma_check = 0) then
        deny_action ('Cannot delineate key=value pairs in extra_keys string; not enough commas between pairs. Cannot retrieve mapping.');  
      end if;
    end if;

    sel_stmt := 'select distinct a.hdb_site_datatype_id, a.hdb_interval_name, a.hdb_method_id, a.hdb_computation_id, a.hdb_agen_id from ref_ext_site_data_map a';
    sel_count_stmt := 'select count (distinct a.mapping_id) from ref_ext_site_data_map a';
    where_stmt := ' where a.ext_data_source_id = '||ext_data_source_id_in||' and a.primary_site_code = '''||primary_site_code_in||''' and a.primary_data_code = '''||primary_data_code_in||''' and a.mapping_id = key1.mapping_id';

    if (lower (is_active_y_n_in) <> 'a') then
	active_stmt := ' and lower(is_active_y_n) = '''||is_active_y_n_in||'''';
    else
	active_stmt := NULL;
    end if;

    not_exists_stmt := ' and not exists (select count(z.mapping_id) from ref_ext_site_data_map_keyval z where z.mapping_id = key1.mapping_id having count(z.mapping_id) <> '||num_extra_keys||')';

    /* Parse extra_keys and build key_value part of query */
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

      sel_stmt := concat (sel_stmt, ', ref_ext_site_data_map_keyval key'||i);
      sel_count_stmt := concat (sel_count_stmt, ', ref_ext_site_data_map_keyval key'||i);
      key_value_stmt := concat (key_value_stmt, ' and key'||i||'.key_name = '''||key_name||''' and key'||i||'.key_value = '''||key_value||'''');
      if (i > 1) then
        map_id_stmt := concat (map_id_stmt, ' and key'||i||'.mapping_id = key'||to_char(i-1)||'.mapping_id');
      end if;

      start_pos := comma_pos + 1;
    end loop;

    count_stmt := sel_count_stmt || where_stmt || active_stmt || key_value_stmt || map_id_stmt || not_exists_stmt;
    whole_stmt := sel_stmt || where_stmt || active_stmt || key_value_stmt || map_id_stmt || not_exists_stmt;

    dbms_output.put_line ('COUNT');
    dbms_output.put_line (sel_count_stmt);
    dbms_output.put_line ('WHOLE');
    dbms_output.put_line (sel_stmt);
    dbms_output.put_line (where_stmt);
    dbms_output.put_line (active_stmt);
    dbms_output.put_line (key_value_stmt);
    dbms_output.put_line (map_id_stmt);
    dbms_output.put_line (not_exists_stmt);

    execute immediate count_stmt 
    into map_count; 

    if (map_count = 1) then
      execute immediate whole_stmt 
      into  hdb_site_datatype_id_out, hdb_interval_name_out, hdb_method_id_out,
            hdb_computation_id_out, hdb_agen_id_out;
    elsif (map_count = 0) then
      deny_action('No '||active_ind||'mappings with source='||ext_data_source_id_in||', site='||primary_site_code_in||', data='||primary_data_code_in||' and extra keys '||key_value_stmt);
    else
      deny_action('Too many mappings ('||map_count||') with source='||ext_data_source_id_in||', site='||primary_site_code_in||', data='||primary_data_code_in||' and extra keys '||key_value_stmt);
    end if;

  end if;
end;
/

show errors;
/
create public synonym get_site_data_map for get_site_data_map;
grant execute on get_site_data_map to public;




