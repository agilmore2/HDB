create or replace trigger r_year_before_insert_update
before             insert OR update 
on                r_year
for   each row
declare
  v_count            number;
begin
    
    select count(*) into v_count from all_tables where table_name = 'R_INTERVAL_UPDATE' 
    and owner = 'APP_USER';

    if (not (DBMS_SESSION.Is_Role_Enabled ('DERIVATION_ROLE'))) or (v_count = 0) or (user <> 'APP_USER') then
      :new.derivation_flags := substr(:new.derivation_flags,1,19) || 'M';
      :new.date_time_loaded := sysdate;
    end if;


end;
/
show errors trigger r_year_before_insert_update;
/
