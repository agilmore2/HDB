--Run below grants as SYS
GRANT execute ON utl_mail TO YAOHDBA;
grant EXECUTE on IS_ROLE_GRANTED to PUBLIC ;
CREATE OR REPLACE PUBLIC SYNONYM IS_ROLE_GRANTED FOR IS_ROLE_GRANTED;
grant select on sys.dba_role_privs to SAVOIR_FAIRE;
grant select on sys.dba_role_privs to YAOHDBA;
@?/rdbms/admin/utlrp.sql
@?/rdbms/admin/utlrp.sql 
