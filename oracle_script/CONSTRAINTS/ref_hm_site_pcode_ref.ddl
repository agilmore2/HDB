alter table ref_hm_site_pcode
add
    constraint ref_hm_site_pcode_fk1
    foreign key (hm_site_code)
    references  ref_hm_site(hm_site_code);

alter table ref_hm_site_pcode
add
    constraint ref_hm_site_pcode_fk2
    foreign key (hm_pcode)     
    references  ref_hm_pcode(hm_pcode);

alter table ref_hm_site_pcode
add
    constraint ref_hm_site_pcode_fk3
    foreign key (hm_filetype)
    references  ref_hm_filetype(hm_filetype);

alter table ref_hm_site_pcode
add
    constraint ref_hm_site_pcode_fk4
    foreign key (site_datatype_id)
    references hdb_site_datatype(site_datatype_id);
