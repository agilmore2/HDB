CREATE DIRECTORY utl as '/ora901/temp';
GRANT READ ON DIRECTORY utl to PUBLIC;


create table ref_wa_metadata_input
(column_1  varchar2(240),
 column_2  varchar2(240),
 column_3  varchar2(240),
 column_4  varchar2(240),
 column_5  varchar2(240),
 column_6  varchar2(240),
 column_7  varchar2(240),
 column_8  varchar2(240),
 column_9  varchar2(240),
 column_10 varchar2(240))
organization external
(type oracle_loader
 default directory utl
 Access parameters
 (
  records delimited by newline
  fields terminated by ','
  missing field values are null
 )
  location (utl:'WAMetadata.out')
)
reject limit unlimited;

grant select on ref_wa_metadata_input to public;

create public synonym ref_wa_metadata_input for ref_wa_metadata_input;

