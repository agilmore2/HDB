create or replace trigger auth_sdi_chk_role
after             insert or update of role
on                ref_auth_site_datatype
for   each row
begin
     psswd_user.check_valid_role_name(:new.role);
end;
/
show errors trigger auth_sdi_chk_role;
/
