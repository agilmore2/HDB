create or replace procedure check_site_id_auth (cur_site number)
IS
cursor c1 (c_site NUMBER) is
    select role from ref_auth_site where site_id = c_site;
result VARCHAR2(24);
BEGIN
    if (DBMS_SESSION.Is_Role_Enabled ('APP_ROLE')) then
        return;
    end if;
    if (DBMS_SESSION.Is_Role_Enabled ('SAVOIR_FAIRE')) then
        return;
    end if;
    for role_record IN c1(cur_site) LOOP
        result := role_record.role;
        if (DBMS_SESSION.Is_Role_Enabled (rtrim(result))) then
            return;
        end if;
    end LOOP;
    raise_application_error(-20001, 'Permission Failure: Role has no permissions to modify values for site_id = ' || cur_site);
END;
/
show errors;
/
grant execute on check_site_id_auth to public;
/
