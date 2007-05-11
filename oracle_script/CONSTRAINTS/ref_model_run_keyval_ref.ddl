alter table ref_model_run_keyval
add
   constraint ref_model_run_keyval_fk1
   foreign key (model_run_id)
   references  ref_model_run(model_run_id);

