drop table ref_installation;

create table ref_installation (                     
meta_data_installation_type    varchar2(32) NOT NULL
)                                                      
pctfree 10
pctused 40
tablespace HDB_data
storage (initial 50k
         next 50k
         pctincrease 0);

alter table ref_installation
add constraint
check_meta_data_install_type
check (meta_data_installation_type in ('master', 'snapshot', 'island'));

grant select on ref_installation to public;
create public synonym ref_installation for ref_installation;

insert into ref_installation (meta_data_installation_type)
values ('&1');

quit;

