alter table r_base_update add constraint
    r_base_update_fk1
    foreign key  (interval)
    references hdb_interval(interval_name);

alter table r_base_update add constraint
    r_base_update_fk2
    foreign key  (overwrite_flag)
    references hdb_overwrite_flag (overwrite_flag);
