/* in order to allow a view creation to a sys view need to grant select with grant option
  to the dba schema owner  */

grant select on dba_roles to hdb_dba_owner with grant option;

