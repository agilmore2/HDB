alter table ref_hm_site_hdbid
add
    constraint ref_hm_site_hdbid_fk1
    foreign key (hm_site_code)
    references  ref_hm_site(hm_site_code);

alter table ref_hm_site_hdbid
add
    constraint ref_hm_site_hdbid_fk2
    foreign key (objecttype_id)
    references  hdb_objecttype(objecttype_id);

alter table ref_hm_site_hdbid
add
    constraint ref_hm_site_hdbid_fk3
    foreign key (site_id)
    references  hdb_site(site_id);
