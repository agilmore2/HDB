/* command line arguments */
set verify off;
set termout off;
define site = &1;
define datatype = &2;

column sdi new_value sdi;

select count (distinct site_datatype_id) sdi from
ref_hm_site_pcode
where
hm_site_code like '%&&site%' and
hm_pcode like '%&&datatype%'
order by site_datatype_id desc;

exit &sdi;
