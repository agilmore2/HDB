undefine tbl_owner
define tbl_owner = &1

PROMPT Creating TRIGGER 'ref_wa_model_site_config_fk1'


create or replace trigger   &&tbl_owner..ref_wa_model_site_config_fk1
before             insert OR update 
on                &&tbl_owner..ref_wa_model_site_config
for   each row
declare
  v_count            number;
  text               varchar2(100);
begin
    
    select count(*) into v_count from v_wa_site_config where site_id = :new.site_id
    and config_id  = :new.config_id; 

    if v_count = 0 then
      text := 'Illegal site_id / config_id for this configuration';
      deny_action(text);
    end if;


end;
/
show errors trigger &&tbl_owner..ref_wa_annotation_fk2_trg;

