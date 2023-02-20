--------------------------------------------------------------------------
-- This script add comments on HDB_ REF_ tables and columns
-- IMPORTANT!!! This script should be executed as main schema user that owns the tables.
-- created on 02/19/2023 by IsmailO
--------------------------------------------------------------------------


-- COMMENTS on HDB_ tables 

comment on table HDB_AGEN is 'This table contains all the different agencies that are potential sources of data.  The agency id is required in all r_base records to define the source agency of the data being stored.';
comment on table HDB_ATTR is 'This table contains attributes are used to describe characteristics of sites. ';
comment on table HDB_ATTR_FEATURE is 'This table links an attribute from hdb_attribute with a feature in hdb_feature. To date, this table has not been used';
comment on table HDB_COLLECTION_SYSTEM is 'Contains the system used by an agency to collect the source data.  This id must be in all r_base records.Since the collection system is not usually known, it is usually set to an id of 14 (see agency).';
comment on table HDB_DAMTYPE is 'This table contains the construction type of a dam. ';
comment on table HDB_DATA_SOURCE is 'This table is obsolete as of hdb2.  It is a precursor to the HDB_AGEN table. It is still required for referential integrity for pre-hdb2 records in the interval tables';
comment on table HDB_DATATYPE is 'Major hdb_ table that identifies and describes what data is being stored.The datatype in this table is half of the site and datatype combination which is used to identify all time series data by SDI.Every datatype must have a unit defined in hdb_unit and a physical quantity defined in hdb_physical_quantity. ';
	comment on column HDB_DATATYPE.DATATYPE_ID  IS 'Unique identifier of the datatype. No implicit meaning';
	comment on column HDB_DATATYPE.DATATYPE_NAME  IS 'Name of the datatype, in full';
	comment on column HDB_DATATYPE.DATATYPE_COMMON_NAME  IS 'Name of datatype, in shortened common name for display purpose';
	comment on column HDB_DATATYPE.PHYSICAL_QUANTITY_NAME  IS 'physical quantity name of the datatype';
	comment on column HDB_DATATYPE.UNIT_ID  IS 'Unit in which this datatype is stored.';
	comment on column HDB_DATATYPE.ALLOWABLE_INTERVALS  IS 'Allowable intervals of the datatype (non-instant,instant,either) ';
	comment on column HDB_DATATYPE.AGEN_ID  IS 'ID of Agency responsible for the datatype';
	comment on column HDB_DATATYPE.CMMNT  IS 'Comment';
comment on table HDB_DATATYPE_ARCHIVE is 'Archive table of HDB_DATATYPE';
comment on table HDB_DATATYPE_FEATURE is 'This table contains data to describe features of datatypes. ';
comment on table HDB_DATE_TIME_UNIT is 'This table contains the date and time units used in hdb.';
comment on table HDB_DERIVATION_FLAG is 'This table is obsolete since we are no longer using the derivation application. A validation flag is now used for data that goes through validation.';
comment on table HDB_DIMENSION is 'The dimension_id is linked to the unit_id in hdb_unit and to the physical quantity in hdb_physical_quantity. Every hdb unit must have a dimension defined and every hdb physical quantity must also have a dimension defined.   ';
comment on table HDB_DIVTYPE is 'This table lists diversion types.';
comment on table HDB_DMI_UNIT_MAP is 'This table maps units between HDB and RiverWare DMI.';
comment on table HDB_EXT_DATA_CODE is 'This table links external data codes to hdb datatype ids.';
comment on table HDB_EXT_DATA_CODE_ARCHIVE is 'Archive table of HDB_EXT_DATA_CODE ';
comment on table HDB_EXT_DATA_CODE_SYS is 'This table defines external data code systems.It links the external systems to the agency id in hdb_agen.';
comment on table HDB_EXT_DATA_SOURCE is 'This table defines the data source for an  external data and external site combination.Mainly used to link an external data source name to a RiverWare model id.  The data source name is typically given the same name as the model name in hdb_model.If it is not used for a model, it links to an external source typically defined by a web address in the description field. ';
comment on table HDB_EXT_DATA_SOURCE_ARCHIVE is 'Archive table of HDB_EXT_DATA_SOURCE';
comment on table HDB_EXT_SITE_CODE is 'This table links external site codes to hdb site ids.';
comment on table HDB_EXT_SITE_CODE_ARCHIVE is 'Archive table of HDB_EXT_SITE_CODE';
comment on table HDB_EXT_SITE_CODE_SYS is 'This table defines external site code systems.It links the external systems to the agency id in hdb_agen.';
comment on table HDB_FEATURE is 'This table defines features, mainly dealing with crop and livestock in UCHDB2.';
comment on table HDB_FEATURE_CLASS is 'This table defines feature classes, mainly dealing with crop and livestock in UCHDB2.';
comment on table HDB_FEATURE_PROPERTY is 'This table links features with properties and assigns values to the properties, mainly dealing with crop and livestock in UCHDB2.';
comment on table HDB_GAGETYPE is 'This table lists gage types.';
comment on table HDB_INTERVAL is 'This table defines intervals used in hdb – instant, hour, day, etc.';
comment on table HDB_LOADING_APPLICATION is 'This table defines the valid loading applications.  One must be specified in all r_base records.';
comment on table HDB_METHOD is 'This table defines the method by which time series data is computed, if applicable. ';
comment on table HDB_METHOD_CLASS is 'Higher level of method, one must be assigned to each method in hdb_method.  ';
comment on table HDB_METHOD_CLASS_TYPE is 'Yet one level higher in the method chain, one must be assigned to each method class in hdb_method_class.';
comment on table HDB_MODEL is 'Every model is defined in this table and assigned an id.Typically this is a RiverWare model, but it is also used for water use forecasts and schedules.';
comment on table HDB_MODEL_COORD is 'This table lists the model id of coordinated models, and the site codes that share the model.';
comment on table HDB_MODELTYPE is 'Linked to ref_model_run M:mid-month P:preliminary F:Final ';
comment on table HDB_OBJECTTYPE is 'Linked to hdb_site, categorizes the site by type or function ';
comment on table HDB_OPERATOR is 'Arithmetic operators for addition, subtraction, multiplication, division';
comment on table HDB_OVERWRITE_FLAG is 'This table is used to indicate that this datum is an overwrite, and cannot be overwritten by any computation. ';
comment on table HDB_PHYSICAL_QUANTITY is 'This table is used to describe data in hdb_datatype. Each datatype must be assigned to a physical quantity name.';
comment on table HDB_PROPERTY is 'This table defines properties, mainly dealing with crop and livestock in UCHDB2. ';
comment on table HDB_RATING_ALGORITHM is 'Algorithm used for rating table lookups.';
comment on table HDB_RATING_TYPE is 'Types of rating tables are defined. ';
comment on table HDB_RIVER is 'This table defines rivers and assigns an id for each. ';
comment on table HDB_RIVER_REACH is 'This table defines river reaches and assigns an id for each. ';
comment on table HDB_SITE is 'Major hdb_ table that identifies and describes all sites that store data in hdb. The site_id in this table comprises half of the SDI which is used to identify all time series data.';
	comment on column HDB_SITE.SITE_ID  IS 'Unique identifier. No implicit meaning';
	comment on column HDB_SITE.SITE_NAME  IS 'Name of site, in full';
	comment on column HDB_SITE.SITE_COMMON_NAME  IS 'Name of site, in shortened common name for display purpose';
	comment on column HDB_SITE.OBJECTTYPE_ID  IS 'Indicator of objecttype that this site is. ';
	comment on column HDB_SITE.PARENT_SITE_ID  IS 'Identifier of site that sits at this same geographic location,if there is one. ';
	comment on column HDB_SITE.PARENT_OBJECTTYPE_ID  IS 'Identifier of objecttype that the parent site is.';
	comment on column HDB_SITE.STATE_ID  IS 'State in which this site resides.';
	comment on column HDB_SITE.BASIN_ID  IS 'Basin in which this site resides.';
	comment on column HDB_SITE.LAT  IS 'Latitude of site';
	comment on column HDB_SITE.LONGI  IS 'Longitude of site';
	comment on column HDB_SITE.HYDROLOGIC_UNIT  IS 'Hydrologic unit of site';
	comment on column HDB_SITE.SEGMENT_NO  IS 'Segment number of site';
	comment on column HDB_SITE.RIVER_MILE  IS 'River mile';
	comment on column HDB_SITE.ELEVATION  IS 'Elevation of site, in feet above sea level';
	comment on column HDB_SITE.DESCRIPTION  IS 'Site descriptor.';
	comment on column HDB_SITE.NWS_CODE  IS 'National Weather Service code for this site';
	comment on column HDB_SITE.SCS_ID  IS 'National Resource Conservation Service (formerly Soil Conservation Service) code for this site.';
	comment on column HDB_SITE.SHEF_CODE  IS 'SHEF code for this site.';
	comment on column HDB_SITE.USGS_ID  IS 'USGS identifier for this site';
	comment on column HDB_SITE.DB_SITE_CODE  IS 'Database site code for database which is responsible for data for this site. FK to REF_DB_List.';
comment on table HDB_SITE_ARCHIVE is 'Archive table of HDB_SITE ';
comment on table HDB_SITE_DATATYPE is 'Major hdb_ table that identifies all time series data by combining the site_id and datatype_id to create a site_datatype_id (SDI). All SDIs are unique and any data that is stored must have an SDI.';
	comment on column HDB_SITE_DATATYPE.SITE_ID  IS 'Site to which this site-datatype applies';
	comment on column HDB_SITE_DATATYPE.DATATYPE_ID  IS 'Datatype to which this site-datatype belongs';
	comment on column HDB_SITE_DATATYPE.SITE_DATATYPE_ID  IS 'Unique identifier of site-datatype combination. No implicit meaning.';
comment on table HDB_SITE_DATATYPE_ARCHIVE is 'Archive table of HDB_SITE_DATATYPE ';
comment on table HDB_STATE is 'This table identifies all states with a state id.State_id is a part of the hdb_site table. ';
comment on table HDB_UNIT is 'Identifies all units with a unit id. Unit_id is a required field of the hdb_datatype table. ';
comment on table HDB_USBR_OFF is 'This table contains regional and field office names within the Bureau ';
comment on table HDB_VALIDATION is 'This table contains the allowable validation flags. When data is written to r_base a validation flag (or a NULL) must be passed. P=provisional data  A=approved data (official)  F=failed validation(written by the validation procedure) L=low limit exceeded(written by the validation procedure)  H=high limit exceeded(written by the validation procedure) ';


-- COMMENTS  on REF_ tables 

comment on table REF_APP_DATA_SOURCE is 'This table contains the names of loading applications and assigns IDs';
comment on table REF_AUTH_SITE is 'This table contains  site IDs with roles';
comment on table REF_AUTH_SITE_DATATYPE is 'This table contains  site-datatype-ids with roles';
comment on table REF_DB_LIST is 'This table contains database names, database site codes and the minimum and maximum coordinated model run id  for each site. This table is referenced in model run management and also when refreshing snapshots (materialized views) of the hdb_ tables';
	comment on table REF_DB_PARAMETER is ' ';
comment on table REF_DIV is 'This table is empty. May be obsolete.';
comment on table REF_ENSEMBLE is 'Main ensemble table that identifies and describes all ensembles';
comment on table REF_ENSEMBLE_ARCHIVE is 'Archive table of REF_ENSEMBLE';
comment on table REF_ENSEMBLE_KEYVAL is 'Lookup table for key_name and key_vale for the ensemble ';
comment on table REF_ENSEMBLE_KEYVAL_ARCHIVE is 'Archive table of REF_ENSEMBLE_KEYVAL ';
comment on table REF_ENSEMBLE_TRACE is 'Lookup table for ensemble trace ';
comment on table REF_ENSEMBLE_TRACE_ARCHIVE is 'Archive table of REF_ENSEMBLE_TRACE ';
comment on table REF_EXT_SITE_DATA_MAP is 'This table maps the RiverWare model slot and object names to SDIs.This table is also used by the PERL program that brings in USGS data daily, run in the cron. Can be used to map any external data codes to SDIs';
comment on table REF_EXT_SITE_DATA_MAP_ARCHIVE is 'Archive table of REF_EXT_SITE_DATA_MAP';
comment on table REF_EXT_SITE_DATA_MAP_KEYVAL is 'This table contains additional information which may be needed by the mappings in table ref_ext_site_data_map';
comment on table REF_EXT_SITE_DATA_MAP_KEY_ARCH is 'Archive table of REF_EXT_SITE_DATA_MAP_KEYVAL';
	comment on table REF_HDB_INSTALLATION is ' ';
comment on table REF_HM_FILETYPE is 'used in the Hydromet applications';
comment on table REF_HM_PCODE is 'used in the Hydromet applications';
comment on table REF_HM_PCODE_OBJECTTYPE is 'used in the Hydromet applications';
comment on table REF_HM_SITE is ' lookup table for all Hydromet site codes used in the Hydromet applications ';
comment on table REF_HM_SITE_DATATYPE is 'lookup table for all Hydromet site datatype ids used in the Hydromet applications ';
comment on table REF_HM_SITE_HDBID is 'used in the Hydromet applications';
comment on table REF_HM_SITE_PCODE is 'used in the Hydromet applications';
comment on table REF_INSTALLATION is 'This table Identifies the database instance as a snapshot or a master installation';
comment on table REF_INTERVAL_COPY_LIMITS is 'This table contains the limits for data that is checked for validation by the validation procedure.It is checked by SDI and interval after it enters r_base. ';
comment on table REF_INTER_COPY_LIMITS_ARCHIVE is 'Archive table of REF_INTERVAL_COPY_LIMITS';
comment on table REF_INTERVAL_REDEFINITION is 'This table helps how the water year interval is defined ';
comment on table REF_INTERVAL_REDEF_ARCHIVE is 'Archive table of REF_INTERVAL_REDEFINITION ';
comment on table REF_LOADING_APPLICATION_PROP is 'This table is for loading applications that are identified in the computation processor as computation processes.';
comment on table REF_MODEL_RUN is 'This is the table that contains all the model run information.';
	comment on column REF_MODEL_RUN.MODEL_RUN_ID  IS 'Unique identifier of the model run. No implicit meaning';
	comment on column REF_MODEL_RUN.MODEL_RUN_NAME  IS 'Name-descriptor of model run.';
	comment on column REF_MODEL_RUN.MODEL_ID  IS 'Model used to generate this model run. FK to HDB_Model.';
	comment on column REF_MODEL_RUN.DATE_TIME_LOADED  IS ' date and time on which this model run was loaded';
	comment on column REF_MODEL_RUN.USER_NAME  IS 'Audit information indicating the db user name who last modified the model run or its time series data';
	comment on column REF_MODEL_RUN.EXTRA_KEYS_Y_N  IS 'Y or N to indicate if the model run has any extra keys (key value pair data is stored in ref_model_run_keyval)';
	comment on column REF_MODEL_RUN.RUN_DATE  IS 'Forecast date of the model run (only really applies to 24-month study)';
	comment on column REF_MODEL_RUN.START_DATE  IS 'Start date of the forecast period for the model run';
	comment on column REF_MODEL_RUN.END_DATE  IS 'End date of the forecast period for the model run.';
	comment on column REF_MODEL_RUN.HYDROLOGIC_INDICATOR  IS 'Text indicating hydrology used ';
	comment on column REF_MODEL_RUN.MODELTYPE  IS 'Valid model type from hdb_modeltype ';
	comment on column REF_MODEL_RUN.TIME_STEP_DESCRIPTOR  IS 'Text describing time step used in model run ';	
	comment on column REF_MODEL_RUN.CMMNT  IS 'Comment.';		
comment on table REF_MODEL_RUN_ARCHIVE is 'Archive table of REF_MODEL_RUN ';
comment on table REF_MODEL_RUN_KEYVAL is 'This is the table that contains additional model run information if extra_keys_y_n in ref_model_run is set to ‘y’.';
comment on table REF_MODEL_RUN_KEYVAL_ARCHIVE is 'Archive table of REF_MODEL_RUN_KEYVAL ';
comment on table REF_PHYS_QUAN_REFRESH_MONITOR is 'Table that shows status of a refresh for czar tables';
comment on table REF_RATING is 'This table contains the actual rating table information for a rating_id ';
comment on table REF_RATING_ARCHIVE is 'Archive table of REF_RATING';
comment on table REF_REFRESH_MONITOR is 'Table that shows status of a refresh for non-czar tables';
comment on table REF_RATING_TABLE is 'Table that describes the rating. Description for the rating table defined by all rows in ref_rating with that rating_id. ';
comment on table REF_RES is 'This table contains reservoir and dam information.Table is mostly empty – use table ref_site_attr instead';
comment on table REF_RES_FLOWLU is 'Lookup table for dam release vs. tail water. Use rating tables instead ??????';
comment on table REF_RES_WSELU is 'Lookup table for reservoir elevation vs. area, content, release, spill.Use rating tables instead ?????? ';
comment on table REF_SITE_ATTR is 'This table contains values for attributes defined in hdb_attr.';
comment on table REF_SITE_ATTR_ARCHIVE is 'Archive table of REF_SITE_ATTR';
comment on table REF_SITE_COEF is 'This table contains values for attributes defined in hdb_attr that are coefficients.This is where shifts for gaging sites are now stored. ';
comment on table REF_SITE_COEF_DAY is 'This table is for coefficients that change depending on the day of the month.';
comment on table REF_SITE_COEF_MONTH is 'This table is for coefficients that change depending on the calendar month.';
comment on table REF_SITE_COEFLU is 'This table is for coefficients that change depending on the value of another attribute.';
comment on table REF_SITE_RATING is 'This table contains the rating type (from hdb_rating_type) for sites that have rating table information in ref_rating. Same rating_id can be used for multiple sites';
comment on table REF_SITE_RATING_ARCHIVE is 'Archive table of REF_SITE_RATING';
comment on table REF_SOURCE_PRIORITY is 'Table is used to determine priority of data if the same site has more than one source for its data.';
comment on table REF_SOURCE_PRIORITY_ARCHIVE is 'Archive table of REF_SOURCE_PRIORITY ';
comment on table REF_SPATIAL_RELATION is 'This table is used only at UC, for their crop data in the CUL program. Defines one site’s relation to another, spatially.';
comment on table REF_STR is 'This table contains information on stream gages.Since the table only allows a limited amount of information – area, gage type and owner – it would be better to define attributes common to gages (in hdb_attr) and then populate the ref_attr table. ';
comment on table REF_USER_GROUPS is 'This table contains database user information with assigned group name. ';
comment on table REF_WA_ACCT is 'Lookup table for all accounts and account-specific information.';
comment on table REF_WA_ACCTTYPE is 'Lookup table for water account types ';
comment on table REF_WA_ANNOTATION is 'Lookup table for all annotations on releases.';
comment on table REF_WA_ANNOTATION_BORROW is 'Lookup table for all details on borrow annotations.';
comment on table REF_WA_ANNOTATION_LEASE is 'Lookup table for lease annotation details.';
comment on table REF_WA_ANNOTATION_LETTER is 'Lookup table for all details on letter annotations.';
comment on table REF_WA_ANNOTATIONTYPE is 'Lookup table for annotation types';
comment on table REF_WA_BORROWTYPE is ' Lookup table for types of borrows.';
comment on table REF_WA_DESTINATION is ' Lookup table for release destinations.';
comment on table REF_WA_DMI_SITE_MAP is 'Mapping of modeling objects onto HDB site IDs. Mapping can vary by model,object type, and date.';
comment on table REF_WA_EXCHANGE is ' Lookup table for all exchange-specific information.';
comment on table REF_WA_EXCHANGE_PAYBACK is 'Lookup for exchange payback definitions.';
comment on table REF_WA_LEASETYPE is ' Lookup table for lease types.';
comment on table REF_WA_MODEL_SITE_CONFIG is 'For each model in HDB, indicates sites and site configurations which are valid at a point in time. Initially intended for use with logical (account, supply, exchange) sites and subbasins, not physical sites. ';
comment on table REF_WA_RELEASETYPE is 'Lookup table for all release types. ';
comment on table REF_WA_REPORT is 'Lookup table for reports';
comment on table REF_WA_REPORT_ANNOTATION is 'Lookup table for report annotation ';
comment on table REF_WA_SLOT_DATA is 'Lookup table for slot data ';
comment on table REF_WA_SUBBASIN_MEMBERS is ' Lookup table for all subbasin member relations; can vary by model and date.';
comment on table REF_WA_SUPPLY is 'Lookup table for all supplies and supply-specific information.';
comment on table REF_WA_SUPPLY_ROLE is 'Lookup table for all supply roles as defined in Riverware';
comment on table REF_WA_SUPPLYTYPE is 'Lookup table for all supply types';
comment on table REF_WA_WATER_OWNER is 'Lookup table for all water owners, or contractors.';
comment on table REF_WA_WATERTYPE is 'Lookup table for water types.';



-- COMMENTS on interval, timeseries, CP and OTHER tables 

comment on table ALARM_CURRENT is 'This table holds currently-asserted alarms';
comment on table ALARM_EVENT is ' ';
comment on table ALARM_GROUP is 'Table contains a list of EMAIL_ADDR records. It is also associated with PROCESS_MONITOR and FILE_MONITOR records described elsewhere.';
comment on table ALARM_HISTORY is 'This table holds alarms that have been asserted and resolved in the past. ';
comment on table ALARM_LIMIT_SET is 'Table contains the limits used to generate the alarm. ';
comment on table ALARM_SCREENING is 'This table is associated with a DATATYPE and optionally a SITE. Records without SITE assignment can define a kind of default for a data type. For example, you might define a screening with datatype=Incremental Precip and no site assignment. ';
comment on table CG_REF_CODES is ' ';
comment on table CP_ALGO_PROPERTY is 'Table defines the properties associated with the algorithm. The properties apply to all computations using that algorithm.The property value in this table is the default value for any computation using the algorithm.';
comment on table CP_ALGO_PROPERTY_ARCHIVE is 'Archive table of  CP_ALGO_PROPERTY';
comment on table CP_ALGO_SCRIPT is ' ';
comment on table CP_ALGO_TS_PARM is 'Table defines the time series parameters associated with the algorithm. Defines the number of input and output that the algorithm will accept.';
comment on table CP_ALGO_TS_PARM_ARCHIVE is 'Archive table of  CP_ALGO_TS_PARM ';
comment on table CP_ALGORITHM is 'Table contains all defined algorithms.Every computation must be assigned to an algorithm.';
comment on table CP_ALGORITHM_ARCHIVE is 'Archive table of  CP_ALGORITHM ';
comment on table CP_COMP_DEPENDS is ' ';
comment on table CP_COMP_DEPENDS_SCRATCHPAD is ' ';
comment on table CP_COMP_PROC_LOCK is ' ';
comment on table CP_COMP_PROPERTY is 'Table contains the property name and value for a computation_id. The property names are dependent on the assigned algorithm. ';
comment on table CP_COMP_PROPERTY_ARCHIVE is 'Archive table of  CP_COMP_PROPERTY';
comment on table CP_COMP_TASKLIST is 'Table contains records to be processed by the Computation Processor-CP. Records deleted after processed by the CP. Data are written to this table by database procedures.';
comment on table CP_COMP_TS_PARM is 'Table contains the SDI associated with the input and output parameters for each computation.  Specifies interval so the CP knows which interval table to retrieve the data from.Specifies model id (-1 if an r_ table) ';
comment on table CP_COMP_TS_PARM_ARCHIVE is 'Archive table of CP_COMP_TS_PARM ';
comment on table CP_COMPUTATION is 'Table contains all defined computations.';
comment on table CP_COMPUTATION_ARCHIVE is 'Archive table of CP_COMPUTATION ';
comment on table CP_DEPENDS_NOTIFY is ' ';
comment on table CP_HISTORIC_COMPUTATIONS is ' ';
comment on table CP_REMOTE_TRIGGERING is ' ';
comment on table CP_TS_ID is ' ';
comment on table DECODES_SITE_EXT is ' ';
comment on table EMAIL_ADDR is ' ';
comment on table FILE_MONITOR is 'Table that describes and define file_monitor. A File Monitor allows you to watch a directory or normal file for abnormal conditions, and then to send email notifications when they are detected. ';
comment on table M_DAY is 'holds model-generated data associated with daily intervals  ';
comment on table M_HOUR is 'holds model-generated data associated with hourly intervals  ';
comment on table M_MONTH is 'holds model-generated data associated with monthly intervals ';
comment on table M_MONTHRANGE is 'Table that defines month range for model_run_id and SITE_DATATYPE_ID ';
comment on table M_MONTHSTAT is 'holds statistical values computed for every month of the year of model data ';
comment on table M_WY is 'holds model-generated data associated with wateryear intervals  ';
comment on table M_YEAR is 'holds model-generated data associated with yearly intervals  ';
comment on table PROCESS_MONITOR is 'Table that describes and define process_monitor.For Process Monitors, the alarm monitor task will connect to one of the supported OpenDCS processes and collect events as they happen. ';
comment on table R_BASE is 'This table is the main entry-point for all data coming into HDB ';
	comment on column R_BASE.SITE_DATATYPE_ID  IS 'The identifier for this site and datatype';
	comment on column R_BASE.INTERVAL  IS 'The interval which the piece of data represents ';
	comment on column R_BASE.START_DATE_TIME  IS 'The beginning of the interval represented';
	comment on column R_BASE.END_DATE_TIME  IS 'The end of the interval represented';
	comment on column R_BASE.VALUE  IS 'Value';
	comment on column R_BASE.AGEN_ID  IS 'ID of Agency responsible for data';
	comment on column R_BASE.OVERWRITE_FLAG  IS 'Overwrite flag, used to tell the Derivation Application that this value overrides other values already-derived at the same interval (e.g., USGS final data). The overwrite values is always at a larger interval than the provisional value.';
	comment on column R_BASE.DATE_TIME_LOADED  IS 'Date and time the value was loaded into HDB';
	comment on column R_BASE.VALIDATION  IS 'Validation flag';
	comment on column R_BASE.COLLECTION_SYSTEM_ID  IS 'Identifier representing the system from which the data came';
	comment on column R_BASE.LOADING_APPLICATION_ID  IS 'Identifier representing the application which loaded the data.';
	comment on column R_BASE.METHOD_ID  IS 'External method used to arrive at the data';
	comment on column R_BASE.COMPUTATION_ID  IS 'External computation used to arrive at the data';
	comment on column R_BASE.DATA_FLAGS  IS 'Data Flag';
comment on table R_BASE_ARCHIVE is 'Archive of data that previously resided in R_Base but was modified. This archive can be used to re-generate the data set that was active at any point in time ';
comment on table R_DAY is 'holds real, observed data associated with daily intervals';
comment on table R_DAYSTAT is 'holds statistical values computed for every day of the month of real data';
comment on table R_HOUR is 'holds real, observed data associated with hourly intervals ';
comment on table R_HOURSTAT is 'holds statistical values computed for every hour of the day arrived at from examining several days, months, or years of real data ';
comment on table R_INSTANT is 'holds real, observed data where each value is associated with an instant in time ';
comment on table R_MONTH is 'holds real, observed data associated with monthly intervals';
comment on table R_MONTHSTAT is 'holds statistical values computed for every month of the year of real data ';
comment on table R_MONTHSTATRANGE is 'holds statistical monthly range data arrived at from examining several years of monthly range data ';
comment on table R_OTHER is 'This table contains OTHER interval time series data, including irregular or non-business intervals.';
comment on table R_WY is 'holds real, observed data associated with wateryearly intervals ';
comment on table R_WYSTAT is 'holds statistical values computed for wateryear ';
comment on table R_YEAR is 'holds real, observed data associated with yearly intervals ';
comment on table R_YEARSTAT is 'holds statistical values computed for year  ';
comment on table TSDB_DATABASE_VERSION is ' ';
comment on table TSDB_GROUP is ' ';
comment on table TSDB_GROUP_MEMBER_DT is ' ';
comment on table TSDB_GROUP_MEMBER_GROUP is ' ';
comment on table TSDB_GROUP_MEMBER_OTHER is ' ';
comment on table TSDB_GROUP_MEMBER_SITE is ' ';
comment on table TSDB_GROUP_MEMBER_TS is ' ';
comment on table TSDB_PROPERTY is 'OPENDCS Global properties on the database components.';




--update version table
INSERT INTO TSDB_PROPERTY (PROP_NAME, PROP_VALUE) VALUES ('4.15.4', 'comments on HDB_ REF_ tables and columns have been added. ');


--Commit
commit;

