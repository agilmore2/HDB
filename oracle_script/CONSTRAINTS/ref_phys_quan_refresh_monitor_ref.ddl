alter table ref_phys_quan_refresh_monitor
add constraint ref_phys_quan_refresh_mon_fk1
foreign key    (db_site_db_name)
references     ref_hdb_installation(db_site_db_name);

