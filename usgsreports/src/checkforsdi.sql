/* command line arguments */
set verify off;
set termout off;
define site = &1;
define datatype = &2;

column sdi new_value sdi;

select count (distinct site_datatype_id) sdi from
ref_hm_site_pcode
where
hm_site_code = UPPER('&&site') and
hm_pcode = UPPER('&&datatype') and
site_datatype_id IS NOT NULL
;

exit &sdi;
