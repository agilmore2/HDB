/*  this sql command file will do anything necessary to the imported tables  */

/*  update hdb_site for the new alias name column  */
update hdb_site set site_common_name = site_name;
commit;
quit
