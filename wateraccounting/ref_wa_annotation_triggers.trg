undefine tbl_owner
define tbl_owner = &1

PROMPT Creating TRIGGER 'ref_wa_annotation_fk2_trg'


create or replace trigger   &&tbl_owner..ref_wa_annotation_fk2_trg
before             insert OR update 
on                &&tbl_owner..ref_wa_annotation
for   each row
declare
  v_count            number;
  text               varchar2(100);
begin
    
  if :new.auxiliary_annotation_id is not null then
    select count(*) into v_count from v_wa_auxiliary_annotation where annotationtype_id = :new.annotationtype_id
    and auxiliary_annotation_id  = :new.auxiliary_annotation_id; 

    if v_count <> 1 then
      text := 'Illegal auxiliary annotation id for that annotation type';
      deny_action(text);
    end if;
  
  end if;

    select count(*) into v_count from v_wa_site_config where site_id = :new.supply_id
    and object_type = 'SUPPLY';

    if v_count = 0 then
      text := 'Illegal site_id / supply_id for this annotation';
      deny_action(text);
    end if;

/*  truncate the release start date so that it has no minutes nd seconds   */

/*   :new.release_start_date := trunc(:new.release_start_date);  */

end;
/
show errors trigger &&tbl_owner..ref_wa_annotation_fk2_trg;
