alter table    ref_agg_disagg
add
   constraint  ref_agg_disagg_ck1
   check (dest_datatype_unit_ind in ('D', 'U'));

alter table ref_agg_disagg
add
   constraint ref_agg_disagg_fk1
   foreign key (source_datatype_id)
   references hdb_datatype(datatype_id);

alter table ref_agg_disagg
add
   constraint ref_agg_disagg_fk2
   foreign key (source_observation_interval)
   references hdb_interval(interval_name);

alter table ref_agg_disagg
add
   constraint ref_agg_disagg_fk3
   foreign key (dest_observation_interval)
   references hdb_interval(interval_name);

alter table ref_agg_disagg
add
   constraint ref_agg_disagg_fk4
   foreign key (method_id)
   references hdb_method(method_id);
