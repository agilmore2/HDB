alter table    ref_hdb_installation
add
   constraint  ref_hdb_installation_ck1
   check       (is_czar_db in ('Y','N'));


