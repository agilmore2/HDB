/* command line arguments */
set verify off;
set termout off;
define site = &1;
define datatype = &2;

column sdi new_value sdi;

select count (distinct hdb_site_datatype_id) sdi from
ref_ext_site_data_map
where
UPPER(primary_site_code) = UPPER('&&site') and
UPPER(primary_data_code) = UPPER('&&datatype') and
hdb_site_datatype_id IS NOT NULL and
/*-- interval = 'day' and*/
rownum = 1;

exit &sdi;
