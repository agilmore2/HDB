alter table ref_div
add   
    constraint   ref_div_fk1
    foreign key  (site_id)
    references   hdb_site(site_id);

alter table ref_div
add   
    constraint   ref_div_fk2
    foreign key  (divtype)
    references   hdb_divtype(divtype);
