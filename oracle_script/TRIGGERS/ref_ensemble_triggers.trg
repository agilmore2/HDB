CREATE OR REPLACE TRIGGER REF_ENSEMBLE_PK_TRIG 
BEFORE INSERT OR UPDATE
ON REF_ENSEMBLE
FOR EACH ROW 
BEGIN 
	IF inserting THEN 
	   IF populate_pk.pkval_pre_populated = FALSE THEN 
	      :new.ENSEMBLE_ID := populate_pk.get_pk_val( 'REF_ENSEMBLE', FALSE );  
	   END IF;

    ELSIF updating THEN 
     :new.ENSEMBLE_ID := :old.ENSEMBLE_ID; 
    END IF; 
END;         
                                                                                
.
/

create or replace trigger ref_ensemble_upd
after update on ref_ensemble for each row begin 
insert into ref_ensemble_archive (
ENSEMBLE_ID,
ENSEMBLE_NAME,
AGEN_ID,
TRACE_DOMAIN,
CMMNT,
ARCHIVE_REASON, DATE_TIME_ARCHIVED, ARCHIVE_CMMNT) values (
:old.ENSEMBLE_ID,
:old.ENSEMBLE_NAME,
:old.AGEN_ID,
:old.TRACE_DOMAIN,
:old.CMMNT,
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
show errors trigger ref_ensemble_upd;

create or replace trigger ref_ensemble_del
after delete on ref_ensemble for each row begin 
insert into ref_ensemble_archive (
ENSEMBLE_ID,
ENSEMBLE_NAME,
AGEN_ID,
TRACE_DOMAIN,
CMMNT,
ARCHIVE_REASON, DATE_TIME_ARCHIVED, ARCHIVE_CMMNT) values (
:old.ENSEMBLE_ID,
:old.ENSEMBLE_NAME,
:old.AGEN_ID,
:old.TRACE_DOMAIN,
:old.CMMNT,
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
show errors trigger ref_ensemble_del;