alter table ref_derivation_source add constraint
    ref_derivation_source_fk1
    foreign key  (site_datatype_id)
    references   hdb_site_datatype(site_datatype_id);
alter table ref_derivation_source add constraint
    ref_derivation_source_fk2
    foreign key  (interval)
    references   hdb_interval(interval_name);
alter table ref_derivation_source add constraint
    ref_derivation_source_fk3
    foreign key  (first_destination_interval)
    references   hdb_interval(interval_name);
