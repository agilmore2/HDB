alter table ref_interval_redefinition add constraint
    ref_interval_redefinition_fk1
    foreign key  (interval)
    references   hdb_interval(interval_name);
alter table ref_interval_redefinition add constraint
    ref_interval_redefinition_fk2
    foreign key  (offset_units)
    references   hdb_date_time_unit(date_time_unit);
