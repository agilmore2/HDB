alter table ref_model_run
add
   constraint ref_model_run_fk1
   foreign key (model_id)
   references  hdb_model(model_id);

alter table ref_model_run
add
   constraint ref_model_run_fk2
   foreign key (modeltype)
   references  hdb_modeltype(modeltype);
