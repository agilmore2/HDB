-- install the triggers to:
--  -- set the effective_start_date_time in ref_derivation_source
--  -- load a row to ref_derivation_source_archive
-- 10/02/01
--

create or replace trigger ref_derivation_src_st_date
before insert or update on ref_derivation_source
for each row
begin
  :new.effective_start_date_time := sysdate;
end;
/
show errors trigger ref_derivation_src_st_date;

create or replace trigger ref_derivation_src_arch_update
after update on ref_derivation_source
for each row
begin
-- archive the row that was changed
  insert into ref_derivation_source_archive
   (site_datatype_id           ,
    effective_start_date_time , 
    interval                  , 
    first_destination_interval, 
    min_value_expected        , 
    min_value_cutoff          , 
    max_value_expected        , 
    max_value_cutoff          , 
    time_offset_minutes       , 
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.site_datatype_id          , 
    :old.effective_start_date_time , 
    :old.interval                  , 
    :old.first_destination_interval, 
    :old.min_value_expected        , 
    :old.min_value_cutoff          , 
    :old.max_value_expected        , 
    :old.max_value_cutoff          , 
    :old.time_offset_minutes       , 
     'UPDATE',
     sysdate,
    NULL);
end;
/
show errors trigger ref_derivation_src_arch_update;

create or replace trigger ref_derivation_src_arch_delete
after delete on ref_derivation_source
for each row
begin
-- archive the row that was changed
  insert into ref_derivation_source_archive
   (site_datatype_id          , 
    effective_start_date_time , 
    interval                  , 
    first_destination_interval, 
    min_value_expected        , 
    min_value_cutoff          , 
    max_value_expected        , 
    max_value_cutoff          , 
    time_offset_minutes       , 
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.site_datatype_id     ,      
    :old.effective_start_date_time  ,
    :old.interval                   , 
    :old.first_destination_interval, 
    :old.min_value_expected        , 
    :old.min_value_cutoff          , 
    :old.max_value_expected        , 
    :old.max_value_cutoff          , 
    :old.time_offset_minutes       , 
     'DELETE',
     sysdate,
    NULL);
end;
/
show errors trigger ref_derivation_src_arch_delete;



