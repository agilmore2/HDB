alter table    m_wy
add
   constraint  m_wy_fk1
   foreign key (model_run_id)
   references  ref_model_run(model_run_id);

alter table    m_wy
add
   constraint  m_wy_fk2
   foreign key (site_datatype_id)
   references  hdb_site_datatype(site_datatype_id);
