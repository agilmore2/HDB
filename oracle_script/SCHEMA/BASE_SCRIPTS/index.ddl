create index site_datatype_dt_idx
       on hdb_site_datatype   (datatype_id)        
pctfree 60
tablespace HDB_idx
storage(initial 150k next 150k pctincrease 0)   
;
Rem create index ixd252  
create index ref_auth_site_datatype_idx
       on ref_auth_site_datatype (role) 
pctfree 60
tablespace HDB_idx
storage(initial 30k next 30k pctincrease 0)   
;                                         
Rem create index ixd253
create index ref_auth_site_idx
       on ref_auth_site(role) 
pctfree 60
tablespace HDB_idx
storage(initial 30k next 30k pctincrease 0)   
;                               

create index ref_interval_redef_arch_idx
on ref_derivation_source_archive (interval, effective_start_date_time)
tablespace HDB_idx
storage (initial 70k next 70k pctincrease 0)
;

create index ref_derivation_source_arch_idx
on ref_derivation_source_archive (site_datatype_id,  effective_start_date_time)
tablespace HDB_idx
storage (initial 70k next 70k pctincrease 0)
;

create index ref_derivation_dest_arch_idx
on ref_derivation_dest_archive (base_site_datatype_id, dest_site_datatype_id,
  effective_start_date_time)
tablespace HDB_idx
storage (initial 70k next 70k pctincrease 0)
;
