

create or replace trigger ref_ensemble_trace_upd
after update on REF_ENSEMBLE_TRACE for each row begin 
insert into REF_ENSEMBLE_TRACE_ARCHIVE (
ENSEMBLE_ID,
TRACE_ID,
TRACE_NUMERIC,
TRACE_NAME,
MODEL_RUN_ID,
ARCHIVE_REASON, DATE_TIME_ARCHIVED, ARCHIVE_CMMNT) values (
:old.ENSEMBLE_ID,
:old.TRACE_ID,
:old.TRACE_NUMERIC,
:old.TRACE_NAME,
:old.MODEL_RUN_ID,
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
show errors trigger ref_ensemble_trace_upd;

create or replace trigger ref_ensemble_trace_del
after delete on REF_ENSEMBLE_TRACE for each row begin
insert into REF_ENSEMBLE_TRACE_ARCHIVE(
ENSEMBLE_ID,
TRACE_ID,
TRACE_NUMERIC,
TRACE_NAME,
MODEL_RUN_ID,
ARCHIVE_REASON, DATE_TIME_ARCHIVED, ARCHIVE_CMMNT) values (
:old.ENSEMBLE_ID,
:old.TRACE_ID,
:old.TRACE_NUMERIC,
:old.TRACE_NAME,
:old.MODEL_RUN_ID,
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
show errors trigger ref_ensemble_trace_del;