undefine tbl_owner
define tbl_owner = &1

PROMPT Creating TRIGGER 'ref_wa_supply_fk23_trg'

create or replace trigger   &&tbl_owner..ref_wa_supply_fk23_trg
before             insert OR update 
on                &&tbl_owner..ref_wa_supply
for   each row
declare
  v_count            number;
  text               varchar2(100);
begin

  if :new.downstream_acct_id is not null then

    select count(*) into v_count from v_wa_site_config where site_id = :new.downstream_acct_id
    and object_type = 'ACCT';

    if v_count = 0 then
      text := 'Illegal site_id / downstream_acct_id for this supply';
      deny_action(text);
    end if;

   end if;

  if :new.source_acct_id is not null then

    select count(*) into v_count from v_wa_site_config where site_id = :new.source_acct_id
    and object_type = 'ACCT';

    if v_count = 0 then
      text := 'Illegal site_id / source_acct_id for this supply';
      deny_action(text);
    end if;

  end if;


end;
/
show errors trigger &&tbl_owner..ref_wa_supply_fk23_trg;
