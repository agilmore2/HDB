CREATE OR REPLACE VIEW V_VALID_INTERVAL_DATATYPE as
(SELECT 'instant' "INTERVAL",b.site_datatype_id
from hdb_datatype a, 
     hdb_site_datatype b 
where a.datatype_id = b.datatype_id 
and (a.datatype_type like '%simple%'
or a.method_class_id = 1
or (a.method_class_id in (11,12,13)
	and a.method_source_datatype_id in 
	(SELECT datatype_id 
	 from hdb_datatype 
	 where datatype_type like '%simple%'
	 or method_class_id =1)))
UNION
(SELECT 'noninstant' ,a.site_datatype_id
from hdb_site_datatype a
MINUS
SELECT 'noninstant' ,b.site_datatype_id
from hdb_datatype a, 
     hdb_site_datatype b 
where a.datatype_id = b.datatype_id 
and (a.datatype_type like '%simple%'
or a.method_class_id = 1
or (a.method_class_id in (11,12,13)
	and a.method_source_datatype_id in 
	(SELECT datatype_id 
	 from hdb_datatype 
	 where datatype_type like '%simple%'
	 or method_class_id =1)))));


