create or replace trigger r_hourstat_sdi_perm
after             insert OR update OR delete
on                r_hourstat
for   each row
begin
	if not (DBMS_SESSION.Is_Role_Enabled ('SAVOIR_FAIRE')) then
	   check_sdi_auth (:new.site_datatype_id);
	end if;
end;
/
show errors trigger r_hourstat_sdi_perm;
/
