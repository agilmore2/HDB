set verify off
spool wa_functions.out

@get_hdb_site_name.func  &1
@get_hdb_site_common_name.func &1
@cfs2acft.func &1

spool off


