create or replace procedure check_valid_site_ot_id (cur_objecttype_id number, cur_site_id number)
IS
    check_val number;
BEGIN
    SELECT count(*) INTO check_val FROM hdb_site WHERE site_id = cur_site_id AND objecttype_id = cur_objecttype_id;
    if check_val < 1 then
        SELECT count(*) INTO check_val FROM hdb_site WHERE site_id = cur_site_id;
        if check_val < 1 then
            raise_application_error (-20001,'No site_id = ' || cur_site_id || ' in hdb_site.');
        end if;
        raise_application_error (-20002,'Objecttype_id ' || cur_objecttype_id || ' is inappropriate for site_id = ' || cur_site_id);
    end if;
END;
/
show errors;
/
grant execute on check_valid_site_ot_id to public;
/
