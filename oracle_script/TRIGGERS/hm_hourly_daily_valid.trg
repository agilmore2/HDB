create or replace trigger hm_hourly_daily_valid
after             insert or update of
                          hourly,
                          daily,
                          max_hourly_date,
                          max_daily_date
on                ref_hm_site_datatype
for   each row
declare
     text                varchar2(200);
begin
    if (:new.daily = 'Y' and :new.max_daily_date is NULL) then
        text := 'Integrity Failure: Max_daily_date is null in ref_hm_site_datatype when daily = Y';
        deny_action(text);
    end if;
    if (:new.hourly = 'Y' and :new.max_hourly_date is NULL) then
        text := 'Integrity Failure: Max_hourly_date is null in ref_hm_site_datatype when hourly = Y';
        deny_action(text);
    end if;
    if (:new.hourly = 'Y' and :new.cutoff_minute is NULL) then
        text := 'Integrity Failure:  Cutoff_minute is null in ref_hm_site_datatype when hourly = Y';
        deny_action(text);
    end if;
    if (:new.hourly = 'Y' and :new.hour_offset is NULL) then
        text := 'Integrity Failure: Hour_offset is null in ref_hm_site_datatype when hourly = Y';
        deny_action(text);
    end if;
end;
/
show errors trigger hm_hourly_daily_valid;
/
