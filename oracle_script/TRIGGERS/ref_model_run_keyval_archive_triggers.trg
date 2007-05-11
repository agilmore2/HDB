-- install the trigger to
--	update the archive_cmmnt on coordinated databases
-- 12/06 Carol Marra
--

create or replace trigger ref_model_run_kv_arch_cmmnt
before update of archive_cmmnt
on ref_model_run_keyval_archive
for each row
when (user <> 'META_DATA_USER')
declare
  v_model_id            number;
  v_mri_is_coord        number;
  v_installation_type   varchar2(32);
  text                  varchar2(1000);
begin

  select meta_data_installation_type
  into v_installation_type
  from ref_installation;

  /* See if this model_run_id is coordinated by checking its model; only 
     need to check master and snapshot installations */
  if (v_installation_type <> 'island') then
    SELECT model_id
    INTO v_model_id
    FROM ref_model_run
    WHERE model_run_id = :new.model_run_id;
 
   model_is_coord (v_model_id, v_mri_is_coord);
  else
     v_mri_is_coord := 0;
  end if;

  if (v_mri_is_coord <> 0) then

    /* Update archive_cmmnt on remote database(s) */
    update_coord_mri_kv_arch_cmmnt (:new.model_run_id, :new.key_name, 
      :new.archive_cmmnt);

  end if; /* is coordinated */

  EXCEPTION
    WHEN others THEN
      text := sqlerrm;
      deny_action (text);
  
end;
/
show errors trigger ref_model_run_kv_arch_cmmnt
/

