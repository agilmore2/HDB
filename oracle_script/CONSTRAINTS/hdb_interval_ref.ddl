alter table hdb_interval
add constraint hdb_interval_fk1
foreign key    (previous_interval_name)
references     hdb_interval(interval_name)
;

alter table hdb_interval
add constraint hdb_interval_fk2
foreign key    (interval_unit)
references     hdb_date_time_unit(date_time_unit)
;
