undefine tbl_owner
define tbl_owner = &1

PROMPT Creating TRIGGER 'ref_wa_ex_payback_fk2_trg'


create or replace trigger   &&tbl_owner..ref_wa_ex_payback_fk2_trg
before             insert OR update 
on                &&tbl_owner..ref_wa_exchange_payback
for   each row
declare
  v_count            number;
  text               varchar2(100);
begin

    select count(*) into v_count from v_wa_site_config where site_id = :new.exchange_site_id
    and object_type = 'EXCHANGE';

    if v_count = 0 then
      text := 'Illegal exchange site_id / exchange_site_id for this exchange payback';
      deny_action(text);
    end if;

    select count(*) into v_count from v_wa_site_config where site_id = :new.payback_supply_id
    and object_type = 'SUPPLY';

    if v_count = 0 then
      text := 'Illegal Supply site_id / payback_supply_id for this exchange payback';
      deny_action(text);
    end if;


end;
/
show errors trigger &&tbl_owner..ref_wa_ex_payback_fk2_trg;
