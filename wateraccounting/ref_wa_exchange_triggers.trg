undefine tbl_owner
define tbl_owner = &1

PROMPT Creating TRIGGER 'ref_wa_exchange_fk23_trg'


create or replace trigger   &&tbl_owner..ref_wa_exchange_fk23_trg
before             insert OR update 
on                &&tbl_owner..ref_wa_exchange
for   each row
declare
  v_count            number;
  text               varchar2(100);
begin

 if :new.borrow_supply_id is not null then
    select count(*) into v_count from v_wa_site_config where site_id = :new.borrow_supply_id
    and object_type = 'SUPPLY';

    if v_count = 0 then
      text := 'Illegal site_id / borrow_supply_id for this exchange';
      deny_action(text);
    end if;
  end if;

  if :new.payback_destination_supply_id is not null then

    select count(*) into v_count from v_wa_site_config where site_id = :new.payback_destination_supply_id
    and object_type = 'SUPPLY';

    if v_count = 0 then
      text := 'Illegal site_id / payback_destination_supply_id for this exchange';
      deny_action(text);
    end if;
  
  end if;


end;
/
show errors trigger &&tbl_owner..ref_wa_exchange_fk23_trg;
