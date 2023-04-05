-- install the triggers to:
--  -- set the date_time_loaded in ref_source_priority
--  -- load a row to ref_source_priority_archive
-- 10/02/01
--

create or replace trigger ref_source_priority_dt_load
before insert or update on ref_source_priority
for each row
begin
  :new.date_time_loaded := sysdate;
end;
/
show errors trigger ref_source_priority_dt_load;


create or replace trigger ref_src_priority_arch_update
after update on ref_source_priority
REFERENCING NEW AS NEW OLD AS OLD
for each row
begin
-- archive the row that was changed
  insert into ref_source_priority_archive
   (site_datatype_id           ,
    agen_id     ,
    priority_rank              , 
    date_time_loaded          ,  
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.site_datatype_id          , 
    :old.agen_id    ,
    :old.priority_rank             ,
    :old.date_time_loaded          ,  
     'UPDATE',
     sysdate,
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
show errors trigger ref_src_priority_arch_update;

create or replace trigger ref_src_priority_arch_delete
after delete on ref_source_priority
for each row
begin
-- archive the row that was changed
  insert into ref_source_priority_archive
   (site_datatype_id           ,
    agen_id     ,
    priority_rank              ,   
    date_time_loaded          ,  
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.site_datatype_id          , 
    :old.agen_id    ,
    :old.priority_rank             ,
    :old.date_time_loaded          ,  
     'DELETE',
     sysdate,
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
show errors trigger ref_src_priority_arch_delete;


