create or replace procedure check_valid_site_objtype (cur_objecttype_tag varchar2, cur_site_id number)
IS
    check_val number;
BEGIN
        SELECT count(*) INTO check_val FROM hdb_site a, hdb_objecttype b WHERE a.site_id = cur_site_id AND a.objecttype_id = b.objecttype_id AND b.objecttype_tag = cur_objecttype_tag;
    if check_val < 1 then
        SELECT count(*) INTO check_val FROM hdb_site WHERE site_id = cur_site_id;
        if check_val < 1 then
            raise_application_error(-20001,'No site_id = ' || cur_site_id ||  ' in hdb_site');
        end if;
        raise_application_error(-20002,'Objecttype ' || cur_objecttype_tag ||' is inappropriate for site_id = ' || cur_site_id);
    end if;
END;
/
show errors;
/
grant execute on check_valid_site_objtype to public;
/
