create or replace PROCEDURE create_site_data_map ( 
                              EXT_DATA_SOURCE_ID NUMBER,
			      PRIMARY_SITE_CODE VARCHAR2,
			      PRIMARY_DATA_CODE VARCHAR2,
                              NUM_EXTRA_KEYS NUMBER,
                              HDB_SITE_DATATYPE_ID NUMBER, 
			      HDB_INTERVAL_NAME VARCHAR2,
                              HDB_METHOD_ID NUMBER, 
                              HDB_COMPUTATION_ID NUMBER, 
                              HDB_AGEN_ID NUMBER,
                              IS_ACTIVE VARCHAR2,
                              CMMNT VARCHAR2,
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
  new_mapping_id number;
  is_active_y_n varchar2(1);
BEGIN
  /*  First check for inappropriate NULL values */
  if (ext_data_source_id is null) then
      deny_action ( 'Invalid <NULL> ext_data_source_id');
  elsif (primary_site_code is null) then
      deny_action ( 'Invalid <NULL> primary_site_code');
  elsif (primary_data_code is null) then
      deny_action ( 'Invalid <NULL> primary_data_code');
  elsif (num_extra_keys is null) then
      deny_action ( 'Invalid <NULL> num_extra_keys');
  elsif (hdb_site_datatype_id is null) then
      deny_action ( 'Invalid <NULL> hdb_site_datatype_id');
  elsif (hdb_interval_name is null) then
      deny_action ( 'Invalid <NULL> hdb_interval_name');
  end if;

  /* if is_active is null, set it to Y */
  if (is_active is null) then
    is_active_y_n := 'Y';
  else
    is_active_y_n := is_active;
  end if;
    
  /* If there are no extra keys, insert record into ref_ext_site_data_map */
  if (num_extra_keys = 0) then
    if (extra_keys is not null) then
      deny_action ('Extra_keys must be NULL when num_extra_keys = 0');
    end if;
  
    insert into ref_ext_site_data_map (
      mapping_id,
      ext_data_source_id,
      primary_site_code,
      primary_data_code,
      extra_keys_y_n,
      hdb_site_datatype_id,
      hdb_interval_name,
      hdb_method_id,
      hdb_computation_id,
      hdb_agen_id,
      is_active_y_n,
      cmmnt)
    values (
      0, 
      ext_data_source_id, 
      primary_site_code, 
      primary_data_code,    
      'N',
      hdb_site_datatype_id,
      hdb_interval_name,
      hdb_method_id,
      hdb_computation_id,
      hdb_agen_id,
      is_active_y_n,
      cmmnt);

    commit; 
  else
    /* Do some checks on the extra_keys string to see if it looks valid */
    /* Not enough pairs? */
    select instr(extra_keys, '=', 1, num_extra_keys)
    into equals_check
    from dual;

    if (equals_check = 0) then
      deny_action ('Extra_keys string does not appear to contain enough key=value pairs; not enough = signs. Mapping will not be created.');  
    end if;

    /* Too many pairs? */
    select instr(extra_keys, '=', 1, num_extra_keys+1)
    into equals_check
    from dual;

    if (equals_check > 0) then
      deny_action ('There appear to be more than '||num_extra_keys||' key=value pairs in extra_keys. Mapping will not be created.');
    end if;

    /* Not enough delineation? */
    if (num_extra_keys > 1) then
      select instr(extra_keys, ',', 1, num_extra_keys-1)
      into comma_check
      from dual;

      if (comma_check = 0) then
        deny_action ('Cannot delineate key=value pairs in extra_keys string; not enough commas between pairs. Mapping will not be created.');  
      end if;
    end if;

    /* Continue with inserting mapping */
    insert into ref_ext_site_data_map (
      mapping_id,
      ext_data_source_id,
      primary_site_code,
      primary_data_code,
      extra_keys_y_n,
      hdb_site_datatype_id,
      hdb_interval_name,
      hdb_method_id,
      hdb_computation_id,
      hdb_agen_id,
      is_active_y_n,
      cmmnt)
     values (
      0, 
      ext_data_source_id, 
      primary_site_code, 
      primary_data_code,    
      'Y',
      hdb_site_datatype_id,
      hdb_interval_name,
      hdb_method_id,
      hdb_computation_id,
      hdb_agen_id,
      is_active_y_n,
      cmmnt);

    /* Parse extra_keys and insert row for each pair; if there are
       extra key=value pairs, the mapping will not be created. */
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

      select max(mapping_id) 
      into new_mapping_id
      from ref_ext_site_data_map;
      
      insert into ref_ext_site_data_map_keyval (
        mapping_id,
        key_name,
        key_value)
      values
        (new_mapping_id,
         key_name,
         key_value);

      start_pos := comma_pos + 1;
    end loop;
    
    commit;
  end if;
end;
/

show errors;
/
create public synonym create_site_data_map for create_site_data_map;
grant execute on create_site_data_map to ref_meta_role;


