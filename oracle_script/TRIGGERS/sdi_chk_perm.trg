create or replace trigger sdi_chk_perm
after             insert OR update OR delete
on                hdb_site_datatype
for   each row
begin
     if not (DBMS_SESSION.Is_Role_Enabled ('SAVOIR_FAIRE')
             OR DBMS_SESSION.Is_Role_Enabled ('HDB_META_ROLE')) then
	   check_sdi_auth_with_site (:new.site_datatype_id, :new.site_id);
	end if;
end;
/
show errors trigger sdi_chk_perm;
/
