alter table ref_derivation_destination add constraint
    ref_derivation_destination_fk1
    foreign key  (base_site_datatype_id)
    references   ref_derivation_source(site_datatype_id);
alter table ref_derivation_destination add constraint
    ref_derivation_destination_fk2
    foreign key  (dest_site_datatype_id)
    references   hdb_site_datatype(site_datatype_id);
alter table ref_derivation_destination add constraint
    ref_derivation_destination_fk3
    foreign key  (method_id)
    references   hdb_method(method_id);
alter table ref_derivation_destination add constraint
    ref_derivation_destination_fk4
    foreign key  (hr_window_unit)
    references   hdb_date_time_unit(date_time_unit);
alter table ref_derivation_destination add constraint
    ref_derivation_destination_fk5
    foreign key  (day_window_unit)
    references   hdb_date_time_unit(date_time_unit);
alter table ref_derivation_destination add constraint
    ref_derivation_destination_fk6
    foreign key  (mon_window_unit)
    references   hdb_date_time_unit(date_time_unit);
alter table ref_derivation_destination add constraint
    ref_derivation_destination_fk7
    foreign key  (yr_window_unit)
    references   hdb_date_time_unit(date_time_unit);
alter table ref_derivation_destination add constraint
    ref_derivation_destination_fk8
    foreign key  (wy_window_unit)
    references   hdb_date_time_unit(date_time_unit);
alter table ref_derivation_destination add constraint
    ref_derivation_destination_fk9
    foreign key  (compounding_source_sdi)
    references   hdb_site_datatype(site_datatype_id);
alter table ref_derivation_destination add constraint
    ref_derivation_destination_f10
    foreign key  (compounding_source_interval)
    references   hdb_interval(interval_name);

alter table ref_derivation_destination add constraint
    check_partial_calc
      check (partial_calc in ('Y','N'));

alter table ref_derivation_destination add constraint
    check_hr_des_eop_window
      check (hr_desired_eop_window >= 0 AND hr_desired_eop_window <= 60 );
alter table ref_derivation_destination add constraint
    check_hr_req_eop_window
      check (hr_required_eop_window>= 0 AND hr_required_eop_window <= 60 );
alter table ref_derivation_destination add constraint
    check_hr_des_bop_window
      check (hr_desired_bop_window >= 0 AND hr_desired_bop_window <= 60 );
alter table ref_derivation_destination add constraint
    check_hr_req_bop_window
      check (hr_required_bop_window>= 0 AND hr_required_bop_window <= 60 );
alter table ref_derivation_destination add constraint
    check_hr_des_number_source
      check (hr_desired_number_source >= 1 );
alter table ref_derivation_destination add constraint
    check_hr_req_number_source
      check (hr_required_number_source >= 1 );

alter table ref_derivation_destination add constraint
    check_day_des_eop_window
      check (day_desired_eop_window >= 0 AND day_desired_eop_window <= 24 );
alter table ref_derivation_destination add constraint
    check_day_req_eop_window
      check (day_required_eop_window>= 0 AND day_required_eop_window <= 24 );
alter table ref_derivation_destination add constraint
    check_day_des_bop_window
      check (day_desired_bop_window >= 0 AND day_desired_bop_window <= 24 );
alter table ref_derivation_destination add constraint
    check_day_req_bop_window
      check (day_required_bop_window>= 0 AND day_required_bop_window <= 24 );
alter table ref_derivation_destination add constraint
    check_day_des_number_source
      check (day_desired_number_source >= 1 );
alter table ref_derivation_destination add constraint
    check_day_req_number_source
      check (day_required_number_source >= 1 );

alter table ref_derivation_destination add constraint
    check_mon_des_eop_window
      check (mon_desired_eop_window >= 0 AND mon_desired_eop_window <= 31 );
alter table ref_derivation_destination add constraint
    check_mon_req_eop_window
      check (mon_required_eop_window>= 0 AND mon_required_eop_window <= 31 );
alter table ref_derivation_destination add constraint
    check_mon_des_bop_window
      check (mon_desired_bop_window >= 0 AND mon_desired_bop_window <= 31 );
alter table ref_derivation_destination add constraint
    check_mon_req_bop_window
      check (mon_required_bop_window>= 0 AND mon_required_bop_window <= 31 );
alter table ref_derivation_destination add constraint
    check_mon_des_number_source
      check (mon_desired_number_source >= -31  );
alter table ref_derivation_destination add constraint
    check_mon_req_number_source
      check (mon_required_number_source >= -31  );

alter table ref_derivation_destination add constraint
    check_yr_des_eop_window
      check (yr_desired_eop_window >= 0 AND yr_desired_eop_window <= 12 );
alter table ref_derivation_destination add constraint
    check_yr_req_eop_window
      check (yr_required_eop_window>= 0 AND yr_required_eop_window <= 12 );
alter table ref_derivation_destination add constraint
    check_yr_des_bop_window
      check (yr_desired_bop_window >= 0 AND yr_desired_bop_window <= 12 );
alter table ref_derivation_destination add constraint
    check_yr_req_bop_window
      check (yr_required_bop_window>= 0 AND yr_required_bop_window <= 12 );
alter table ref_derivation_destination add constraint
    check_yr_des_number_source
      check (yr_desired_number_source >= 1 );
alter table ref_derivation_destination add constraint
    check_yr_req_number_source
      check (yr_required_number_source >= 1 );

alter table ref_derivation_destination add constraint
    check_wy_des_eop_window
      check (wy_desired_eop_window >= 0 AND wy_desired_eop_window <= 12 );
alter table ref_derivation_destination add constraint
    check_wy_req_eop_window
      check (wy_required_eop_window>= 0 AND wy_required_eop_window <= 12 );
alter table ref_derivation_destination add constraint
    check_wy_des_bop_window
      check (wy_desired_bop_window >= 0 AND wy_desired_bop_window <= 12 );
alter table ref_derivation_destination add constraint
    check_wy_req_bop_window
      check (wy_required_bop_window>= 0 AND wy_required_bop_window <= 12 );
alter table ref_derivation_destination add constraint
    check_wy_des_number_source
      check (wy_desired_number_source >= 1 );
alter table ref_derivation_destination add constraint
    check_wy_req_number_source
      check (wy_required_number_source >= 1 );


