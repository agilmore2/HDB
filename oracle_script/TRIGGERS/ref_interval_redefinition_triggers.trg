-- install the triggers to:
--  -- set the effective_start_date_time in ref_interval_redefinition
--  -- load a row to ref_interval_redef_archive
-- 10/02/01
--

create or replace trigger ref_interval_redef_st_date
before insert or update on ref_interval_redefinition
for each row
begin
  :new.effective_start_date_time := sysdate;
end;
/
show errors trigger ref_interval_redef_st_date;

create or replace trigger ref_interval_redef_arch_update
after update on ref_interval_redefinition
for each row
begin
-- archive the row that was changed
  insert into ref_interval_redef_archive
   (interval                  ,
    effective_start_date_time , 
    time_offset               ,
    offset_units              ,
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.interval                  ,
    :old.effective_start_date_time , 
    :old.time_offset               ,
    :old.offset_units              ,
     'UPDATE',
     sysdate,
    NULL);
end;
/
show errors trigger ref_interval_redef_arch_update;

create or replace trigger ref_interval_redef_arch_delete
after delete on ref_interval_redefinition
for each row
begin
-- archive the row that was changed
  insert into ref_interval_redef_archive
   (interval                  ,
    effective_start_date_time , 
    time_offset               ,
    offset_units              ,
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.interval                  ,
    :old.effective_start_date_time , 
    :old.time_offset               ,
    :old.offset_units              ,
     'DELETE',
     sysdate,
    NULL);
end;
/
show errors trigger ref_interval_redef_arch_delete;



