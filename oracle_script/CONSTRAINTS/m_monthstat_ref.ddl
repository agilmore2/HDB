alter table    m_monthstat
add
   constraint  m_monthstat_fk1
   foreign key (model_run_id)
   references  ref_model_run(model_run_id);

alter table    m_monthstat
add
   constraint  m_monthstat_fk2
   foreign key (site_datatype_id)
   references  hdb_site_datatype(site_datatype_id);

alter table    m_monthstat
add
   constraint  m_monthstat_ck1
   check (month between '1' and '12');
