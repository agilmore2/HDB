set echo off
spool wa_prelims.out

drop role wa_role;
create role wa_role;

drop user wa_user cascade;

create user wa_user identified by wa_user
default tablespace HDB_data quota unlimited on HDB_data
temporary tablespace HDB_temp quota unlimited on HDB_temp;

grant wa_role to wa_user;
alter user wa_user default role wa_role;
grant wa_role to ref_meta_role;

grant create session to wa_user;
grant resource to wa_user;
grant connect  to wa_user;


spool off
