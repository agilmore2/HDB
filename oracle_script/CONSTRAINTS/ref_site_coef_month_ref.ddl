alter table ref_site_coef_month
add constraint ref_site_coef_month_ck1
check (month between '1' and '12');

alter table ref_site_coef_month
add constraint ref_site_coef_month_fk1
foreign key   (site_id)
references    hdb_site (site_id)
;

alter table ref_site_coef_month
add constraint ref_site_coef_month_fk2
foreign key   (attr_id)
references    hdb_attr (attr_id)
;
