set serveroutput on size 32000
set long 32000
set line 120
spool cr_trigs.sql

execute gen_pk_trigs;
spool off;

