-- install the triggers to load a row to r_base_update that
-- notifies derivation application of data to process
-- 10/02/01  
--
-- Modified 10-AUG-04 by M. Bogner to fix derivation  and overwrite bug discovered by A. Gilmore at U.C.

create or replace trigger r_base_before_insert_update
before insert or update 
on r_base
for each row
declare
  v_count            number;
  v_source_entry     number;
  v_invalid_interval number;
  e_bad_row          exception;
  text               varchar2(200);
begin

   if not (DBMS_SESSION.Is_Role_Enabled ('SAVOIR_FAIRE')) then
      check_sdi_auth (:new.site_datatype_id);
   end if;

   if (trunc(:new.start_date_time) > trunc(SYSDATE + 2/24)  ) then
      text := 'No future dates allowed in r_base tables';
      deny_action(text);
   end if;

--  test the method id if unknown don't do anything otherwise
--  see that the method_id has the same method class id as the
--  implied method_class_id as the datatype

    select count(*) into v_count from hdb_method
      where :new.method_id=method_id and
      upper(method_name) in ('UNKNOWN', 'COPY OF DATA', 'SIMPLE DIVISION');

    if v_count = 0 then
        select count(*) into v_count 
         from  hdb_site_datatype a, hdb_datatype b, hdb_method c	 
          where :new.method_id=c.method_id and
                :new.site_datatype_id = a.site_datatype_id and
                a.datatype_id = b.datatype_id  and
                b.method_class_id = c.method_class_id;
         if v_count = 0  then
           text := 'Method Class Integrity check violation against implied datatypes method class';
           deny_action(text);
         end if;
    end if;
-- make sure there is not an sdi/interval in ref_derivation_source 
  if :new.overwrite_flag='O' then
    select count(*) into v_count from ref_derivation_source
      where :new.site_datatype_id=site_datatype_id and
      :new.interval=interval;
    if v_count>0 then
      text := 'Invalid overwrite record because there is a derivation source specification for this SDI and interval';
      deny_action(text);
    end if;
  end if;
-- see if there's already one there
  select count(*) into v_count from r_base_update
    where :new.site_datatype_id=site_datatype_id and
    :new.interval=interval and
    :new.start_date_time=start_date_time and
    :new.end_date_time=end_date_time;
-- if there is a record then set ready fordelete to null for derivation app
-- decode statement added to update statement to address A. Gilmore discovered bug July 2004
  if v_count!=0 then
       update r_base_update set ready_for_delete = null,
       overwrite_flag = decode(:old.overwrite_flag,'O','O',:new.overwrite_flag)
       where :old.site_datatype_id=site_datatype_id and
       :old.interval=interval and
       :old.start_date_time=start_date_time and
       :old.end_date_time=end_date_time;
  end if;
-- if not, insert the new one
  if v_count=0 then
    select count(*) into v_count from ref_derivation_source
      where :new.site_datatype_id=site_datatype_id
        and :new.interval=interval;
-- only if there's a derivation spec or it's a forced 'O'verwrite
    if v_count!=0 or :new.overwrite_flag='O' then
      insert into r_base_update
        (site_datatype_id, interval, start_date_time, end_date_time,
         overwrite_flag, ready_for_delete)
      values
        (:new.site_datatype_id, :new.interval, :new.start_date_time,
         :new.end_date_time, :new.overwrite_flag, null);
    end if;
--
-- only if there's not a derivation spec or it's a previously forced 'O'verwrite and the overwrite flag is
-- being set to null
-- bug discovered by A. Gilmore July 2004
    if v_count=0 and :old.overwrite_flag='O' and :new.overwrite_flag is null then
      insert into r_base_update
        (site_datatype_id, interval, start_date_time, end_date_time,
         overwrite_flag, ready_for_delete)
      values
        (:new.site_datatype_id, :new.interval, :new.start_date_time,
         :new.end_date_time, :old.overwrite_flag, null);
    end if;
  end if;

  :new.date_time_loaded:=sysdate;

  if (:new.interval = 'instant') then
     if ( :new.start_date_time <> :new.end_date_time) then
        text := 'Instant interval start and end date times must be equal';
        deny_action(text);
     end if;

     select count(*) into v_count from v_valid_interval_datatype
     where interval = 'instant' and site_datatype_id = :new.site_datatype_id;
     if (v_count = 0) then
        text := 'Invalid Interval for this datatype';
        deny_action(text);
     end if;

  end if;

  if (:new.interval <> 'instant') then
     if ( :new.start_date_time > :new.end_date_time) then
        text := 'Non-instant interval start date time must be less than the end date time';
        deny_action(text);
     end if;
     if ( :new.start_date_time = :new.end_date_time) then
        text := 'Non-instant interval start and end date times cannot be equal';
        deny_action(text);
     end if;

     select count(*) into v_count from v_valid_interval_datatype
     where interval = 'noninstant' and site_datatype_id = :new.site_datatype_id;
     if (v_count = 0) then
        text := 'Invalid Interval for this datatype';
        deny_action(text);
     end if;

  end if;

end;
/
show errors trigger r_base_before_insert_update;
/



create or replace trigger r_base_after_update
after update on r_base
for each row
declare
  v_count            number;
begin

-- archive the row that was changed
  insert into r_base_archive
    (site_datatype_id,
     interval,
     start_date_time,
     end_date_time,
     value,
     agen_id,
     overwrite_flag,
     date_time_loaded,
     validation,
     collection_system_id,
     loading_application_id,
     method_id,
     computation_id,
     archive_reason,
     date_time_archived)
  values
    (:old.site_datatype_id,
     :old.interval,
     :old.start_date_time,
     :old.end_date_time,
     :old.value,
     :old.agen_id,
     :old.overwrite_flag,
     :old.date_time_loaded,
     :old.validation,
     :old.collection_system_id,
     :old.loading_application_id,
     :old.method_id,
     :old.computation_id,
     'UPDATE',
     sysdate);
end;
/
show errors trigger r_base_after_update;

create or replace trigger r_base_before_delete
before delete on r_base
for each row
declare
  v_count            number;
begin

   if not (DBMS_SESSION.Is_Role_Enabled ('SAVOIR_FAIRE')) then
      check_sdi_auth (:old.site_datatype_id);
   end if;

end;
/
show errors trigger r_base_before_delete;


create or replace trigger r_base_after_delete
after delete on r_base
for each row
declare
  v_count            number;
begin

-- see if there's already one there
  select count(*) into v_count from r_base_update
    where :old.site_datatype_id=site_datatype_id and
    :old.interval=interval and
    :old.start_date_time=start_date_time and
    :old.end_date_time=end_date_time;
-- if there is a record then set ready fordelete to null for derivation app
  if v_count!=0 then
       update r_base_update set ready_for_delete = null
       where :old.site_datatype_id=site_datatype_id and
       :old.interval=interval and
       :old.start_date_time=start_date_time and
       :old.end_date_time=end_date_time;
  end if;
-- if not, insert the new one
  if v_count=0 then
    select count(*) into v_count from ref_derivation_source
      where :old.site_datatype_id=site_datatype_id
        and :old.interval=interval;
-- only if there's a derivation spec or it's a forced 'O'verwrite
    if v_count!=0 or :old.overwrite_flag='O' then
      insert into r_base_update
        (site_datatype_id, interval, start_date_time, end_date_time,
         overwrite_flag, ready_for_delete)
      values
        (:old.site_datatype_id, :old.interval, :old.start_date_time,
         :old.end_date_time, :old.overwrite_flag, null);
    end if;
  end if;
-- archive the row that was deleted
  insert into r_base_archive
    (site_datatype_id,
     interval,
     start_date_time,
     end_date_time,
     value,
     agen_id,
     overwrite_flag,
     date_time_loaded,
     validation,
     collection_system_id,
     loading_application_id,
     method_id,
     computation_id,
     archive_reason,
     date_time_archived)
  values
    (:old.site_datatype_id,
     :old.interval,
     :old.start_date_time,
     :old.end_date_time,
     :old.value,
     :old.agen_id,
     :old.overwrite_flag,
     :old.date_time_loaded,
     :old.validation,
     :old.collection_system_id,
     :old.loading_application_id,
     :old.method_id,
     :old.computation_id,
     'DELETE',
     sysdate);
end;
/
show errors trigger r_base_after_delete;
