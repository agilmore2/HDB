set echo on
set feedback on
spool hdb_syns.out

create public synonym hdb_agen                    for DBA.hdb_agen;
create public synonym hdb_attr                  for DBA.hdb_attr;
create public synonym hdb_attr_feature for DBA.hdb_attr_feature;
create public synonym hdb_collection_system      for DBA.hdb_collection_system;
/* create public synonym hdb_computed_datatype      for DBA.hdb_computed_datatype; */
/* create public synonym hdb_computed_datatype_component      	      for DBA.hdb_computed_datatype_component;
*/
create public synonym hdb_damtype                  for DBA.hdb_damtype;
create public synonym hdb_data_source                   for DBA.hdb_data_source;
create public synonym hdb_datatype      for DBA.hdb_datatype;
create public synonym hdb_datatype_feature for DBA.hdb_datatype_feature;
create public synonym hdb_date_time_unit for DBA.hdb_date_time_unit;
-- create public synonym hdb_derivation_flag    for DBA.hdb_derivation_flag;
create public synonym hdb_dimension     for DBA.hdb_dimension;
create public synonym hdb_divtype                     for DBA.hdb_divtype;
create public synonym hdb_dmi_unit_map                    for DBA.hdb_dmi_unit_map;
create public synonym hdb_ext_site_code_sys for DBA.hdb_ext_site_code_sys;
create public synonym hdb_ext_site_code for DBA.hdb_ext_site_code;
create public synonym hdb_ext_site_code_archive for DBA.hdb_ext_site_code_archive;
create public synonym hdb_ext_data_code_sys for DBA.hdb_ext_data_code_sys;
create public synonym hdb_ext_data_code for DBA.hdb_ext_data_code;
create public synonym hdb_ext_data_code_archive for DBA.hdb_ext_data_code_archive;
create public synonym hdb_ext_data_source for DBA.hdb_ext_data_source;
create public synonym hdb_ext_data_source_archive for DBA.hdb_ext_data_source_archive;
create public synonym hdb_feature for DBA.hdb_feature;
create public synonym hdb_feature_class for DBA.hdb_feature_class;
create public synonym hdb_feature_property for DBA.hdb_feature_property;
create public synonym hdb_gagetype                    for DBA.hdb_gagetype;
create public synonym hdb_interval      for DBA.hdb_interval;
create public synonym hdb_loading_application      for DBA.hdb_loading_application;
create public synonym hdb_method        for DBA.hdb_method;
create public synonym hdb_method_class        for DBA.hdb_method_class;
create public synonym hdb_method_class_type        for DBA.hdb_method_class_type;
create public synonym hdb_model                  for DBA.hdb_model;
create public synonym hdb_model_coord            for DBA.hdb_model_coord;
create public synonym hdb_modeltype                       for DBA.hdb_modeltype;
create public synonym hdb_objecttype    for DBA.hdb_objecttype;
create public synonym hdb_operator        for DBA.hdb_operator;
create public synonym hdb_overwrite_flag        for DBA.hdb_overwrite_flag;
create public synonym hdb_physical_quantity for DBA.hdb_physical_quantity;
create public synonym hdb_property for DBA.hdb_property;
create public synonym hdb_rating_algorithm for DBA.hdb_rating_algorithm;
create public synonym hdb_rating_type for DBA.hdb_rating_type;
create public synonym hdb_river                       for DBA.hdb_river;
create public synonym hdb_river_reach                   for DBA.hdb_river_reach;
create public synonym hdb_site          for DBA.hdb_site;
create public synonym hdb_site_datatype for DBA.hdb_site_datatype;
create public synonym hdb_state         for DBA.hdb_state;
create public synonym hdb_usbr_off      for DBA.hdb_usbr_off;
create public synonym hdb_validation                    for DBA.hdb_validation;
create public synonym hdb_unit                  for DBA.hdb_unit;
create public synonym hm_temp_data                        for DBA.hm_temp_data;
create public synonym m_day          for DBA.m_day;
create public synonym m_hour         for DBA.m_hour;
create public synonym m_month        for DBA.m_month;
create public synonym m_monthrange                        for DBA.m_monthrange;
create public synonym m_monthstat                            for DBA.m_monthstat;
create public synonym m_wy           for DBA.m_wy;
create public synonym m_year         for DBA.m_year;
create public synonym ratings for DBA.ratings;
-- create public synonym ref_agg_disagg                      for DBA.ref_agg_disagg; deprecated and removed by IsmailO 10/2022
create public synonym ref_app_data_source                    for DBA.ref_app_data_source;
create public synonym ref_auth_site                   for DBA.ref_auth_site;
create public synonym ref_auth_site_datatype                  for DBA.ref_auth_site_datatype;
create or replace public synonym REF_DB_GENERIC_LIST for DBA.REF_DB_GENERIC_LIST;
create or replace public synonym REF_CZAR_DB_GENERIC_LIST for DBA.REF_CZAR_DB_GENERIC_LIST;
create public synonym ref_db_list                  for DBA.ref_db_list;
-- create public synonym ref_derivation_source        for DBA.ref_derivation_source;
-- create public synonym ref_derivation_destination        for DBA.ref_derivation_destination;
create public synonym ref_div                     for DBA.ref_div;
create or replace public synonym REF_ENSEMBLE for DBA.REF_ENSEMBLE;
create or replace public synonym REF_ENSEMBLE_KEYVAL for DBA.REF_ENSEMBLE_KEYVAL;
create or replace public synonym REF_ENSEMBLE_TRACE for DBA.REF_ENSEMBLE_TRACE;
create or replace public synonym REF_ENSEMBLE_ARCHIVE for DBA.REF_ENSEMBLE_ARCHIVE;
create or replace public synonym REF_ENSEMBLE_KEYVAL_ARCHIVE for DBA.REF_ENSEMBLE_KEYVAL_ARCHIVE;
create or replace public synonym REF_ENSEMBLE_TRACE_ARCHIVE for DBA.REF_ENSEMBLE_TRACE_ARCHIVE;
-- create public synonym ref_dmi_data_map                  for DBA.ref_dmi_data_map; deprecated and removed by IsmailO 10/2022
create public synonym ref_ext_site_data_map  for DBA.ref_ext_site_data_map;
create public synonym ref_ext_site_data_map_archive  for DBA.ref_ext_site_data_map_archive;
create public synonym ref_ext_site_data_map_keyval  for DBA.ref_ext_site_data_map_keyval;
create public synonym ref_ext_site_data_map_key_arch  for DBA.ref_ext_site_data_map_key_arch;
create public synonym ref_hm_filetype                  for DBA.ref_hm_filetype;
create public synonym ref_hm_pcode                      for DBA.ref_hm_pcode;
create public synonym ref_hm_pcode_objecttype                  for DBA.ref_hm_pcode_objecttype;
create public synonym ref_hm_site                              for DBA.ref_hm_site;
create public synonym ref_hm_site_datatype                  for DBA.ref_hm_site_datatype;
create public synonym ref_hm_site_hdbid                   for DBA.ref_hm_site_hdbid;
create public synonym ref_hm_site_pcode                   for DBA.ref_hm_site_pcode;
create public synonym ref_interval_redefinition      for DBA.ref_interval_redefinition;
create public synonym ref_loading_application_prop     for DBA.ref_loading_application_prop;
create public synonym ref_model      for DBA.ref_model;
create public synonym ref_model_run                       for DBA.ref_model_run;
create public synonym ref_model_run_keyval for DBA.ref_model_run_keyval;
create public synonym ref_model_run_archive for DBA.ref_model_run_archive;
create public synonym ref_model_run_keyval_archive for DBA.ref_model_run_keyval_archive;
create public synonym ref_rating for DBA.ref_rating;
create public synonym ref_rating_archive for DBA.ref_rating_archive;
create public synonym ref_res        for DBA.ref_res;
create public synonym ref_res_flowlu                  for DBA.ref_res_flowlu;
create public synonym ref_res_wselu                    for DBA.ref_res_wselu;
create public synonym ref_site_attr                  for DBA.ref_site_attr;
create public synonym ref_site_attr_archive       for DBA.ref_site_attr_archive;
create public synonym ref_site_coef                  for DBA.ref_site_coef;
create public synonym ref_site_coeflu   for DBA.ref_site_coeflu;
create public synonym ref_site_coef_day                  for DBA.ref_site_coef_day;
create public synonym ref_site_coef_month                  for DBA.ref_site_coef_month;
create public synonym ref_site_rating for DBA.ref_site_rating;
create public synonym ref_site_rating_archive for DBA.ref_site_rating_archive;
create public synonym ref_str           for DBA.ref_str;
create public synonym ref_source_priority           for DBA.ref_source_priority;
create public synonym ref_source_priority_archive   for DBA.ref_source_priority_archive;
create public synonym ref_user_groups   for DBA.ref_user_groups;
create public synonym ref_spatial_relation   for DBA.ref_spatial_relation;
create public synonym r_base            for DBA.r_base;
create public synonym r_base_archive            for DBA.r_base_archive;
-- create public synonym r_base_update            for DBA.r_base_update; removed for CP project
create public synonym r_day             for DBA.r_day;
create public synonym r_daystat         for DBA.r_daystat;
create public synonym r_hour            for DBA.r_hour;
create public synonym r_hourstat        for DBA.r_hourstat;
create public synonym r_instant         for DBA.r_instant;
create public synonym r_month           for DBA.r_month;
create public synonym r_monthstat                  for DBA.r_monthstat;
create public synonym r_monthstatrange                    for DBA.r_monthstatrange;
create public synonym r_other             for DBA.r_other;
create public synonym r_wy              for DBA.r_wy;
create public synonym r_wystat          for DBA.r_wystat;
create public synonym r_year            for DBA.r_year;
create public synonym r_yearstat        for DBA.r_yearstat ;
create public synonym rm_year_v                   for DBA.rm_year_v;
create public synonym rm_wy_v                     for DBA.rm_wy_v; 
create public synonym rm_month_v                  for DBA.rm_month_v;
create public synonym rm_hour_v                   for DBA.rm_hour_v;
create public synonym rm_day_v                    for DBA.rm_day_v;

create public synonym v_valid_interval_datatype   for DBA.v_valid_interval_datatype;

--Missing Synonymes added on 09/2015  
CREATE OR REPLACE PUBLIC SYNONYM GET_PK_VAL_WRAP FOR DBA.GET_PK_VAL_WRAP;
CREATE OR REPLACE PUBLIC SYNONYM CFS2ACFT FOR DBA.CFS2ACFT;
CREATE OR REPLACE PUBLIC SYNONYM GET_HDB_SITE_COMMON_NAME FOR DBA.GET_HDB_SITE_COMMON_NAME;
CREATE OR REPLACE PUBLIC SYNONYM GET_HDB_SITE_NAME FOR DBA.GET_HDB_SITE_NAME;
CREATE OR REPLACE PUBLIC SYNONYM GSNA FOR DBA.GSNA;
create or replace public synonym V_HDB_SITE_DATATYPE_NAME for DBA.V_HDB_SITE_DATATYPE_NAME;
create or replace public synonym V_DBA_ROLES for DBA.V_DBA_ROLES;
create or replace public synonym HDB_DATATYPE_UNIT for DBA.HDB_DATATYPE_UNIT;
create or replace public synonym DAYS for DBA.DAYS;
CREATE OR REPLACE PUBLIC SYNONYM POPULATE_PK_HDB FOR DBA.POPULATE_PK;
CREATE OR REPLACE PUBLIC SYNONYM POPULATE_PK_REF FOR DBA.POPULATE_PK;

--Synonyms for new archive tables added on 09/2019 by IsmailO
CREATE OR REPLACE PUBLIC SYNONYM HDB_SITE_ARCHIVE FOR DBA.HDB_SITE_ARCHIVE;
CREATE OR REPLACE PUBLIC SYNONYM HDB_DATATYPE_ARCHIVE FOR DBA.HDB_DATATYPE_ARCHIVE;
CREATE OR REPLACE PUBLIC SYNONYM HDB_SITE_DATATYPE_ARCHIVE FOR DBA.HDB_SITE_DATATYPE_ARCHIVE;


spool off
exit;
