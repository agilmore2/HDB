-- install the triggers to:
--  -- set the effective_start_date_time in ref_source_priority
--  -- load a row to ref_source_priority_archive
-- 10/02/01
--

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
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.site_datatype_id          , 
    :old.agen_id    ,
    :old.priority_rank             ,
     'UPDATE',
     sysdate,
    NULL);
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
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.site_datatype_id          , 
    :old.agen_id    ,
    :old.priority_rank             ,
     'DELETE',
     sysdate,
    NULL);
end;
/
show errors trigger ref_src_priority_arch_delete;


