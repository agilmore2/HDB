create or replace trigger ref_site_coeflu_site_perm
after             insert OR update OR delete
on                ref_site_coeflu
for   each row
begin
     if not (DBMS_SESSION.Is_Role_Enabled ('SAVOIR_FAIRE')
             OR DBMS_SESSION.Is_Role_Enabled ('REF_META_ROLE')) then
	   check_site_id_auth (:new.site_id);
	end if;
end;
/
show errors trigger ref_site_coeflu_site_perm;
/
