alter table    m_day
add
   constraint  m_day_fk1
   foreign key (model_run_id)
   references  ref_model_run(model_run_id);

alter table    m_day
add
   constraint  m_day_fk2
   foreign key (site_datatype_id)
   references  hdb_site_datatype(site_datatype_id);
