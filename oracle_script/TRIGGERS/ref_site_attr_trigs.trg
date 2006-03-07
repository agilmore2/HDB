-- install the triggers to:
-- set the effective_start_date_time and check attr value type
-- load a row to ref_site_attr_archive
-- 10/02/01
--

create or replace trigger ref_site_attr_dt_load_chk_val
before             insert or update
on                 ref_site_attr
for   each row
begin
     check_valid_attr_value(:new.attr_id, :new.value, :new.string_value, :new.date_value);
     :new.date_time_loaded := SYSDATE;
end;
/
show errors trigger ref_site_attr_dt_load_chk_val;


create or replace trigger ref_site_attr_arch_update
after update on ref_site_attr
REFERENCING NEW AS NEW OLD AS OLD
for each row
begin
  insert into ref_site_attr_archive
   (site_id,
    attr_id,
    effective_start_date_time,
    effective_end_date_time,
    value,
    string_value,
    date_value,
    date_time_loaded,
    archive_reason,
    date_time_archived,
    archive_cmmnt)
  values
   (:old.site_id,
    :old.attr_id,
    :old.effective_start_date_time,
    :old.effective_end_date_time,
    :old.value,
    :old.string_value,
    :old.date_value,
    :old.date_time_loaded,
     'UPDATE',
     sysdate,
    NULL);
end;
/
show errors trigger ref_site_attr_arch_update;

create or replace trigger ref_site_attr_arch_delete
after delete on ref_site_attr
for each row
begin
  insert into ref_site_attr_archive
   (site_id,
    attr_id,
    effective_start_date_time,
    effective_end_date_time,
    value,
    string_value,
    date_value,
    date_time_loaded,
    archive_reason,
    date_time_archived,
    archive_cmmnt)
  values
   (:old.site_id,
    :old.attr_id,
    :old.effective_start_date_time,
    :old.effective_end_date_time,
    :old.value,
    :old.string_value,
    :old.date_value,
    :old.date_time_loaded,
    'DELETE',
     sysdate,
    NULL);
end;
/
show errors trigger ref_site_attr_arch_delete;


