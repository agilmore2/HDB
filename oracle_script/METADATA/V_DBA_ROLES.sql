create view V_DBA_ROLES as select role from dba_roles
where password_required = 'YES';

grant select on v_dba_roles to public;
create public synonym v_dba_roles for v_dba_roles;
