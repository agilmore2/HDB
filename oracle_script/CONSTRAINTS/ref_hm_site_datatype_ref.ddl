alter table ref_hm_site_datatype
add
    constraint ref_hm_site_datatype_fk1
    foreign key (site_datatype_id)
    references  hdb_site_datatype(site_datatype_id);
