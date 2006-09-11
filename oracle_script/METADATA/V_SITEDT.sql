CREATE OR REPLACE VIEW V_HDB_SITE_DATATYPE_NAME AS
SELECT site_datatype_id, site_name||'---'||datatype_name s_d_name               
FROM hdb_site_datatype, hdb_site, hdb_datatype                                  
WHERE                                                                           
     hdb_site_datatype.site_id = hdb_site.site_id                               
AND  hdb_site_datatype.datatype_id = hdb_datatype.datatype_id                   
/

grant select on v_hdb_site_datatype_name to public;
create public synonym v_hdb_site_datatype_name for v_hdb_site_datatype_name; 


