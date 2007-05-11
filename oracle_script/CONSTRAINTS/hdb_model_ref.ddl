alter table hdb_model
add constraint hdb_model_ck1
check (coordinated in ('Y','N'));
