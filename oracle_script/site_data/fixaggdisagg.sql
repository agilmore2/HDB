create database link hdb connect to app_user identified by hdb using 'hdb';

drop database link hdb;

select * from ref_agg_disagg@hdb;

alter trigger ref_agg_disagg_pk_trig disable;

insert into ref_agg_disagg 
select agg_disagg_id, source_datatype_id, b.interval_name, 
dest_datatype_unit_ind, dest_datatype_or_unit_id,
c.interval_name, 'M',
d.method_id, agg_disagg_function_name
from ref_agg_disagg@hdb a, hdb_interval b, hdb_interval c, hdb_method d
where a.source_observation_prd like b.interval_name
  and a.dest_observation_prd like c.interval_name
  and ((a.agg_disagg_operator = 'ave' and d.method_id = 2) OR
       (a.agg_disagg_operator = 'eop' and d.method_id = 7) OR
       (a.agg_disagg_operator = 'none' and d.method_id = 6) OR
       (a.agg_disagg_operator = 'sum' and d.method_id = 17) OR
       (a.agg_disagg_operator = 'div' and d.method_id = 20));

insert into ref_agg_disagg
select agg_disagg_id, source_datatype_id, b.interval_name,
dest_datatype_unit_ind, dest_datatype_or_unit_id,
c.interval_name, operator_or_function,
null, agg_disagg_function_name
from ref_agg_disagg@hdb a, hdb_interval b, hdb_interval c
where a.source_observation_prd like b.interval_name
  and a.dest_observation_prd like c.interval_name
  and a.agg_disagg_operator is null; 

alter trigger ref_agg_disagg_pk_trig enable;

commit;
