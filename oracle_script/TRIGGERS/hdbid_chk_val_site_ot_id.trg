create or replace trigger hdbid_chk_val_site_ot_id
after             insert or update
on                ref_hm_site_hdbid
for   each row
begin
     check_valid_site_ot_id(:new.objecttype_id, :new.site_id);
end;
/
show errors trigger hdbid_chk_val_site_ot_id;
/
