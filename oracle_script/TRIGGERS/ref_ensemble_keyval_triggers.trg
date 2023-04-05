
create or replace trigger ref_ensemble_keyval_upd
after update on ref_ensemble_keyval for each row begin
insert into ref_ensemble_keyval_archive (
ENSEMBLE_ID,
KEY_NAME,
KEY_VALUE,
DATE_TIME_LOADED,
ARCHIVE_REASON, DATE_TIME_ARCHIVED, ARCHIVE_CMMNT) values (
:old.ENSEMBLE_ID,
:old.KEY_NAME,
:old.KEY_VALUE,
:old.DATE_TIME_LOADED,
'UPDATE', sysdate, 
coalesce(
          sys_context('APEX$SESSION','app_user')
         ,regexp_substr(sys_context('userenv','client_identifier'),'^[^:]*')
         ,sys_context('userenv','session_user')
         ) || ':' || sys_context('userenv','os_user') 
         || ':' || sys_context('userenv','HOST') 
         || ':' || sys_context('userenv','CLIENT_PROGRAM_NAME')
);
end;
/
show errors trigger ref_ensemble_keyval_upd;

create or replace trigger ref_ensemble_keyval_del
after delete on ref_ensemble_keyval for each row begin
insert into ref_ensemble_keyval_archive (
ENSEMBLE_ID,
KEY_NAME,
KEY_VALUE,
DATE_TIME_LOADED,
ARCHIVE_REASON, DATE_TIME_ARCHIVED, ARCHIVE_CMMNT) values (
:old.ENSEMBLE_ID,
:old.KEY_NAME,
:old.KEY_VALUE,
:old.DATE_TIME_LOADED,
'DELETE', sysdate, 
coalesce(
          sys_context('APEX$SESSION','app_user')
         ,regexp_substr(sys_context('userenv','client_identifier'),'^[^:]*')
         ,sys_context('userenv','session_user')
         ) || ':' || sys_context('userenv','os_user') 
         || ':' || sys_context('userenv','HOST') 
         || ':' || sys_context('userenv','CLIENT_PROGRAM_NAME')
);
end;
/
show errors trigger ref_ensemble_keyval_del;