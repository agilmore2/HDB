-- install the triggers to:
--  -- set the effective_start_date_time in ref_derivation_destination
--  -- load a row to ref_derivation_dest_archive
-- 10/02/01
--

create or replace trigger ref_derivation_dest_st_date
before insert or update on ref_derivation_destination
for each row
begin
  :new.effective_start_date_time := sysdate;
end;
/
show errors trigger ref_derivation_dest_st_date;

create or replace trigger ref_derivation_dest_arch_upd
after update on ref_derivation_destination
for each row
begin
-- archive the row that was changed
  insert into ref_derivation_dest_archive
   (base_site_datatype_id           ,
    dest_site_datatype_id,
    effective_start_date_time , 
    method_id                       ,
    partial_calc                    ,
    compounding_source_sdi          ,
    compounding_source_interval     ,
    hr_desired_eop_window           ,
    hr_required_eop_window          ,
    hr_desired_bop_window           ,
    hr_required_bop_window          ,
    hr_desired_number_source        ,
    hr_required_number_source       ,
    hr_window_unit                  ,
    day_desired_eop_window          ,
    day_required_eop_window         ,
    day_desired_bop_window          ,
    day_required_bop_window         ,
    day_desired_number_source       ,
    day_required_number_source      ,
    day_window_unit                 ,
    mon_desired_eop_window          ,
    mon_required_eop_window         ,
    mon_desired_bop_window          ,
    mon_required_bop_window         ,
    mon_desired_number_source       ,
    mon_required_number_source      ,
    mon_window_unit                 ,
    yr_desired_eop_window           ,
    yr_required_eop_window          ,
    yr_desired_bop_window           ,
    yr_required_bop_window          ,
    yr_desired_number_source        ,
    yr_required_number_source       ,
    yr_window_unit                  ,
    wy_desired_eop_window           ,
    wy_required_eop_window          ,
    wy_desired_bop_window           ,
    wy_required_bop_window          ,
    wy_desired_number_source        ,
    wy_required_number_source       ,
    wy_window_unit                  ,
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.base_site_datatype_id           ,
    :old.dest_site_datatype_id,
    :old.effective_start_date_time , 
    :old.method_id                       ,
    :old.partial_calc                    ,
    :old.compounding_source_sdi          ,
    :old.compounding_source_interval     ,
    :old.hr_desired_eop_window           ,
    :old.hr_required_eop_window          ,
    :old.hr_desired_bop_window           ,
    :old.hr_required_bop_window          ,
    :old.hr_desired_number_source        ,
    :old.hr_required_number_source       ,
    :old.hr_window_unit                  ,
    :old.day_desired_eop_window          ,
    :old.day_required_eop_window         ,
    :old.day_desired_bop_window          ,
    :old.day_required_bop_window         ,
    :old.day_desired_number_source       ,
    :old.day_required_number_source      ,
    :old.day_window_unit                 ,
    :old.mon_desired_eop_window          ,
    :old.mon_required_eop_window         ,
    :old.mon_desired_bop_window          ,
    :old.mon_required_bop_window         ,
    :old.mon_desired_number_source       ,
    :old.mon_required_number_source      ,
    :old.mon_window_unit                 ,
    :old.yr_desired_eop_window           ,
    :old.yr_required_eop_window          ,
    :old.yr_desired_bop_window           ,
    :old.yr_required_bop_window          ,
    :old.yr_desired_number_source        ,
    :old.yr_required_number_source       ,
    :old.yr_window_unit                  ,
    :old.wy_desired_eop_window           ,
    :old.wy_required_eop_window          ,
    :old.wy_desired_bop_window           ,
    :old.wy_required_bop_window          ,
    :old.wy_desired_number_source        ,
    :old.wy_required_number_source       ,
    :old.wy_window_unit                  ,
     'UPDATE',
     sysdate,
    NULL);
end;
/
show errors trigger ref_derivation_dest_arch_upd;

create or replace trigger ref_derivation_dest_arch_del
after delete on ref_derivation_destination
for each row
begin
-- archive the row that was changed
  insert into ref_derivation_dest_archive
   (base_site_datatype_id           ,
    dest_site_datatype_id,
    effective_start_date_time , 
    method_id                       ,
    partial_calc                    ,
    compounding_source_sdi          ,
    compounding_source_interval     ,
    hr_desired_eop_window           ,
    hr_required_eop_window          ,
    hr_desired_bop_window           ,
    hr_required_bop_window          ,
    hr_desired_number_source        ,
    hr_required_number_source       ,
    hr_window_unit                  ,
    day_desired_eop_window          ,
    day_required_eop_window         ,
    day_desired_bop_window          ,
    day_required_bop_window         ,
    day_desired_number_source       ,
    day_required_number_source      ,
    day_window_unit                 ,
    mon_desired_eop_window          ,
    mon_required_eop_window         ,
    mon_desired_bop_window          ,
    mon_required_bop_window         ,
    mon_desired_number_source       ,
    mon_required_number_source      ,
    mon_window_unit                 ,
    yr_desired_eop_window           ,
    yr_required_eop_window          ,
    yr_desired_bop_window           ,
    yr_required_bop_window          ,
    yr_desired_number_source        ,
    yr_required_number_source       ,
    yr_window_unit                  ,
    wy_desired_eop_window           ,
    wy_required_eop_window          ,
    wy_desired_bop_window           ,
    wy_required_bop_window          ,
    wy_desired_number_source        ,
    wy_required_number_source       ,
    wy_window_unit                  ,
    archive_reason            , 
    date_time_archived        , 
    archive_cmmnt              )
  values
   (:old.base_site_datatype_id           ,
    :old.dest_site_datatype_id,
    :old.effective_start_date_time , 
    :old.method_id                       ,
    :old.partial_calc                    ,
    :old.compounding_source_sdi          ,
    :old.compounding_source_interval     ,
    :old.hr_desired_eop_window           ,
    :old.hr_required_eop_window          ,
    :old.hr_desired_bop_window           ,
    :old.hr_required_bop_window          ,
    :old.hr_desired_number_source        ,
    :old.hr_required_number_source       ,
    :old.hr_window_unit                  ,
    :old.day_desired_eop_window          ,
    :old.day_required_eop_window         ,
    :old.day_desired_bop_window          ,
    :old.day_required_bop_window         ,
    :old.day_desired_number_source       ,
    :old.day_required_number_source      ,
    :old.day_window_unit                 ,
    :old.mon_desired_eop_window          ,
    :old.mon_required_eop_window         ,
    :old.mon_desired_bop_window          ,
    :old.mon_required_bop_window         ,
    :old.mon_desired_number_source       ,
    :old.mon_required_number_source      ,
    :old.mon_window_unit                 ,
    :old.yr_desired_eop_window           ,
    :old.yr_required_eop_window          ,
    :old.yr_desired_bop_window           ,
    :old.yr_required_bop_window          ,
    :old.yr_desired_number_source        ,
    :old.yr_required_number_source       ,
    :old.yr_window_unit                  ,
    :old.wy_desired_eop_window           ,
    :old.wy_required_eop_window          ,
    :old.wy_desired_bop_window           ,
    :old.wy_required_bop_window          ,
    :old.wy_desired_number_source        ,
    :old.wy_required_number_source       ,
    :old.wy_window_unit                  ,
     'DELETE',
     sysdate,
    NULL);
end;
/
show errors trigger ref_derivation_dest_arch_del;



