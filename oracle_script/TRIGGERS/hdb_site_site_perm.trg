create or replace trigger hdb_site_site_perm
after             insert OR update OR delete
on                hdb_site
for   each row
begin
     if not (DBMS_SESSION.Is_Role_Enabled ('SAVOIR_FAIRE')
             OR DBMS_SESSION.Is_Role_Enabled ('HDB_META_ROLE')) then
	   check_site_id_auth (:new.site_id);
	end if;
end;
/
show errors trigger hdb_site_site_perm;
/
