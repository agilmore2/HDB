create or replace procedure check_valid_unit(cur_unit_id number)
IS
     check_val   number;
begin
     select count(*) into check_val from hdb_unit
            where  hdb_unit.unit_id = cur_unit_id;
     if  SQL%NOTFOUND then
        raise_application_error(-20001,'Error: Error selecting from hdb_unit with unit_id = ' || cur_unit_id);
     end if;
     if check_val < 1 then
        raise_application_error(-20002,'Integrity Failure: No unit_id = ' || cur_unit_id);
     end if;
end;
/
show errors;
/
grant execute on  check_valid_unit to public;
/
