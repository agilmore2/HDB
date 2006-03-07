-- install the triggers to:
--  -- set the date_time_loaded in ref_interval_redefinition
--  -- load a row to ref_interval_redef_archive
-- 10/02/01
--

create or replace trigger ref_interval_redef_dt_load
before insert or update on ref_interval_redefinition
for each row
begin
  :new.date_time_loaded := sysdate;
end;
/
show errors trigger ref_interval_redef_dt_load;

create or replace trigger ref_interval_redef_arch_update
after update on ref_interval_redefinition
for each row
begin
-- archive the row that was changed
  insert into ref_interval_redef_archive
   (interval                  ,
    time_offset               ,
    offset_units              ,
    date_time_loaded          , 
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.interval                  ,
    :old.time_offset               ,
    :old.offset_units              ,
    :old.date_time_loaded          ,
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
    time_offset               ,
    offset_units              ,
    date_time_loaded          ,
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.interval                  ,
    :old.time_offset               ,
    :old.offset_units              ,
    :old.date_time_loaded          ,
     'DELETE',
     sysdate,
    NULL);
end;
/
show errors trigger ref_interval_redef_arch_delete;



