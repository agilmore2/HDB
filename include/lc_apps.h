/* This file must be included in all report-generation code.      */
/* Contains site_datatype_ids used in reports                     */

#ifndef REPORT_INC
#define REPORT_INC

#include "defines.h"
#include "typedefs.h"

int ConnectedToUC; /* Set in reports that connect to UC -- allows non-fatal */
                          /*   failure when connecting to that DB site.            */

/* In the applications that produce model data, the user will now have the option
of supplying the model_run_id instead of deafaulting to RIVERWARE_BHOPS_MODEL_RUN_ID.
(i.e. model_run_id = 4. Therefore, an ew global variable is included here to
accomodate that new option
int user_defined_mri. */

int user_defined_mri;

extern char *month[];

#define MISSING_SDI 9998

#define RIVERWARE_BHOPS_MODEL_RUN_ID 4

#define POWELL_SITE_ID        919
#define POWELL_RELPWR_CFS_SDI 1862
#define POWELL_RELTOT_CFS_SDI 1872
#define POWELL_CONT_AF_SDI    1719
#define POWELL_ELEV_FT_SDI    1928
#define POWELL_INFLOW_CFS_SDI 1792
#define GDCANYON_FLOW_CFS_SDI 1346

#define PV_RETABVTF_FLOW_CFS_SDI  2220
#define PV_RETBLWTF_FLOW_CFS_SDI  2221
#define PAINTED_ELEV_FT_SDI       2198



#define MEAD_PUMP_CFS_SDI     2151 /* divave at site "pumping from lake mead" */
#define MEAD_PUMP_AF_SDI      2066 /* divave at site "pumping from lake mead" */

#define MEAD_AVG_TAILRACE_ELEV     2158

#define MEAD_SITE_ID          921
#define MEAD_RESV_REL_SDI     2215
#define MEAD_RELPWR_CFS_SDI   1863
#define MEAD_RELTOT_CFS_SDI   1874
#define MEAD_RELPWR_AF_SDI    2212
#define MEAD_ELEV_FT_SDI      1930
#define MEAD_CONT_AF_SDI      1721
#define MEAD_EVAP_AF_SDI      1776
#define MEAD_INFLOW_CFS_SDI   1794
#define MEAD_INFHYDAVE_CFS_SDI 2306
#define MEAD_SCHEDREL_CFS_SDI 2308
#define MEAD_FCREL_CFS_SDI    2310
#define MEAD_ELEVTWAVE_FT_SDI 2158
#define MEAD_SPLAVE_CFS_SDI   2156
#define HOOVER_EFFAVE_PCT_SDI 2152
#define HOOVER_EFF_KWHAF_SDI  2073
#define HOOVER_LC_EFF_SDI     2275
#define HOOVER_HEADAVE_FT_SDI 2154
#define HOOVER_NETGEN_KWH_SDI 2077 /* this is engynet at hoover. as of 1996, hoover has no 
                                      measuring devices on station service generators  */
#define HOOVER_STA_SVC_KWH_SDI 2281 /* so the operators manually enter the station service */
#define HOOVER_GRGEN_KWH_SDI  2070 /* this is engytot = engynet + station svc*/ 

#define MOHAVE_SITE_ID        922
#define MOHAVE_ELEV_FT_SDI    2100
#define MOHAVE_CONT_AF_SDI    2086
#define MOHAVE_RELPWR_CFS_SDI 2166
#define MOHAVE_RELTOT_CFS_SDI 2096
#define MOHAVE_RELPWR_AF_SDI  2213
#define MOHAVE_INFHYDAVE_CFS_SDI 2294
#define MOHAVE_ELEVTWAVE_FT_SDI 2169
#define DAVIS_GRGEN_KWH_SDI   2071
#define DAVIS_HEADAVE_FT_SDI  2164
#define DAVIS_PWRAVE_MW_SDI   2165
#define DAVIS_EFFAVE_PCT_SDI  2161
#define DAVIS_EFF_KWHAF_SDI   2277
#define DAVIS_LC_EFF_SDI      2274

#define HAVASU_SITE_ID        923
#define HAVASU_ELEV_FT_SDI    2101
#define HAVASU_CONT_AF_SDI    2087
#define HAVASU_RELPWR_CFS_SDI 2146
#define HAVASU_RELTOT_CFS_SDI 2097
#define HAVASU_RELPWR_AF_SDI  2214
#define HAVASU_INFLOW_CFS_SDI 2143
#define HAVASU_INFHYDAVE_CFS_SDI 2292
#define HAVASU_ELEVTWAVE_FT_SDI 2149
#define PARKER_GRGEN_SDI      2072
#define PARKER_HEADAVE_FT_SDI 2144
#define PARKER_PWRAVE_MW_SDI  2145
#define PARKER_EFFAVE_PCT_SDI 2141
#define PARKER_EFF_KWHAF_SDI  2278 
#define PARKER_LC_EFF_SDI     2276 

#define PARKER_STAGE_INST_SDI 2118
#define PARKER_STAGE_AVE_SDI  2055
#define PARKER_FLOW_CFS_SDI   2119
#define PARKER_SHIFT_SDI      2030

#define HEADGATE_ENGYTOT_SDI  2272     /* an energy estimate for the CRIR Headgate plant */

#define SENWASH_SITE_ID       1033
#define SENWASH_ELEV_FT_SDI   2207
#define SENWASH_CONT_AF_SDI   2206
#define SENWASH_PUMP_CFS_SDI  2205
#define SENWASH_RELPWR_CFS_SDI 2208

#define FLGOR_SITE_ID         917
#define FLGOR_RELTOT_AF_SDI   1919
#define FLGOR_RELTOT_CFS_SDI  1871
#define FLGOR_CONT_AF_SDI     1718
#define FLGOR_ELEV_FT_SDI     1927
#define FLGOR_RELPWR_CFS_SDI  1861
#define FLGOR_RELTOT_CFS_SDI  1871
#define FLGOR_UNRINFLOW_CFS_SDI   1850

#define NAVAJO_SITE_ID        920
#define NAVAJO_RELTOT_AF_SDI  1921
#define NAVAJO_RELTOT_CFS_SDI 1873
#define NAVAJO_CONT_AF_SDI    1720
#define NAVAJO_UNRINFLOW_CFS_SDI  1793
#define NAVAJO_ELEV_FT_SDI    1929
#define NAVAJO_RELTOT_CFS_SDI 1873 /* Navajo has power plant, but USBR doesn't*/
                                   /*   track its operations                  */

#define BLUEMESA_SITE_ID        913
#define BLUEMESA_UNRINFLOW_CFS_SDI 1847
#define BLUEMESA_RELTOT_AF_SDI  1915
#define BLUEMESA_RELTOT_CFS_SDI 1867
#define BLUEMESA_CONT_AF_SDI    1714
#define BLUEMESA_ELEV_FT_SDI    1923
#define BLUEMESA_RELPWR_CFS_SDI 1857

#define FONT_SITE_ID          916
#define FONT_RELTOT_AF_SDI    1918
#define FONT_ELEV_FT_SDI      1926
#define FONT_RELTOT_CFS_SDI   1870
#define FONT_CONT_AF_SDI      1717

#define CRYST_SITE_ID         915
#define CRYST_RELTOT_AF_SDI   1917
#define CRYST_ELEV_FT_SDI     1925
#define CRYST_RELTOT_CFS_SDI  1869
#define CRYST_CONT_AF_SDI     1716

#define MOOV_ELEV_FT_SDI      2102
#define CRIR_DIV_CFS_SDI      1982 /*This is for Headgate Rock Canal Flow sd*/

#define CRIR_DIVAVE_CFS_SDI   2209 /*This is REAL CRIR_DIV_CFS_SDI*/
#define POST_FLOW_CFS_SDI     2218

#define MORROW_SITE_ID        914
#define MORROW_RELTOT_AF_SDI  1916
#define MORROW_RELTOT_CFS_SDI 1868
#define MORROW_CONT_AF_SDI    1715
#define MORROW_ELEV_FT_SDI    1924
#define MORROW_RELPWR_CFS_SDI 1858

#define MWD_DIV_CFS_SDI       2064
#define MWD_DIV_SCHED_CFS_SDI 2137
#define MWD_DIVTOT_AF_SDI     2068
#define CAP_DIV_CFS_SDI       2065
#define CAP_DIV_SCHED_CFS_SDI 2134
#define CAP_DIVTOT_AF_SDI     2069

#define WHEEL_STAGE_INST_SDI  2044
#define WHEEL_STAGE_AVE_SDI   2058
#define WHEEL_FLOW_CFS_SDI    2021
#define WHEEL_SHIFT_INST_SDI  2033

#define PV_RIV_FLOW_CFS_SDI   2019
#define PV_CAN_FLOW_CFS_SDI   1983
#define PV_RETABVTF_DIV_CFS_SDI 2220
#define PV_RETBLWTF_DIV_CFS_SDI 2221
#define PV_DIV_CFS_SDI        2204

#define TAYLOR_STAGE_INST_SDI 2043
#define TAYLOR_STAGE_AVE_SDI  2057
#define TAYLOR_FLOW_CFS_SDI   2020
#define TAYLOR_SHIFT_INST_SDI 2032
#define TAYLOR_CONT_AF_SDI    1713

#define CIBOLA_STAGE_INST_SDI 2036
#define CIBOLA_STAGE_AVE_SDI  2049
#define CIBOLA_FLOW_CFS_SDI   2013
#define CIBOLA_SHIFT_INST_SDI 2024

#define GARDNER_FLOW_CFS_SDI  2015

#define LOWMAIN_FLOW_CFS_SDI  2017
#define UPMAIN_FLOW_CFS_SDI   2219

#define LAGUNA_STAGE_INST_SDI 2039
#define LAGUNA_STAGE_AVE_SDI  2052
#define LAGUNA_FLOW_CFS_SDI   2016
#define LAGUNA_SHIFT_INST_SDI 2027

#define YUMA_STAGE_INST_SDI   2045
#define YUMA_STAGE_AVE_SDI    2059
#define YUMA_FLOW_CFS_SDI     2022
#define YUMA_SHIFT_INST_SDI   2034

#define NIB_STAGE_INST_SDI    2041
#define NIB_STAGE_AVE_SDI     2054
#define NIB_FLOW_CFS_SDI      2018
#define NIB_SHIFT_INST_SDI    2029

#define GILA_STAGE_INST_SDI   2048
#define GILA_STAGE_AVE_SDI    2061
#define GILA_FLOW_CFS_SDI     2023
#define GILA_SHIFT_INST_SDI   2035

#define PAINTED_REL_CFS_SDI   2197
#define PAINTED_ELET_FT_SDI   2198
#define PAINTED_CONT_AF_SDI   2199
#define PAINTED_INFLW_CFS_SDI 2196

#define ALAMO_REL_CFS_SDI     2189
#define ALAMO_ELEV_FT_SDI     2190
#define ALAMO_INFLW_CFS_SDI  2188
#define ALAMO_CONT_AF_SDI     2191

#define MORELOS_DIV_CFS_SDI   0   /* FIX THIS! */

#define DVS1_AVM_FLOW_AF_SDI  2004
#define DVS2_AVM_FLOW_AF_SDI  2005
#define DVS3_AVM_FLOW_AF_SDI  2006
#define DVS4_AVM_FLOW_AF_SDI  2007
#define DVS5_AVM_FLOW_AF_SDI  2008

#define PK1_AVM_FLOW_AF_SDI   2009
#define PK2_AVM_FLOW_AF_SDI   2010
#define PK3_AVM_FLOW_AF_SDI   2011
#define PK4_AVM_FLOW_AF_SDI   2012

#define CFS_TO_AF_FACTOR      1.983471

#define STATEMENT_LEN 1000
#define TABLE_NAME_LEN 24
#define NO_VALUE_AVAILABLE -191919.00

typedef char day_mo[10];

typedef struct two_dates {
  SQL_DATE sqlDate;
  char printDate[30];
} TWO_DATES;

#define MAX_NUM_REC 100
#define TWO_MONTH 2
 
typedef struct mead_data {
  float glen_rel[MAX_NUM_REC];
  float ggc_loss[MAX_NUM_REC];
  float gc_flow[MAX_NUM_REC];
  float gchv_loss[MAX_NUM_REC];
  float hv_rel[MAX_NUM_REC];
  float mead_cont[MAX_NUM_REC];
  float mead_elev[MAX_NUM_REC];
  float hv_tw_elev[MAX_NUM_REC];
  float hv_static_head[MAX_NUM_REC];
  float mead_spill[MAX_NUM_REC];
  float hv_energy[MAX_NUM_REC];
  float hv_teff[MAX_NUM_REC];
  float hv_eff[MAX_NUM_REC];
  float hv_hec[MAX_NUM_REC];
} MEAD_DATA;

typedef struct mohave_data {
  float hvdv_loss[MAX_NUM_REC];
  float dv_rel[MAX_NUM_REC];
  float moh_content[MAX_NUM_REC];
  float moh_elev[MAX_NUM_REC];
  float dv_tw_elev[MAX_NUM_REC];
  float dv_static_head[MAX_NUM_REC];
  float dv_cap[MAX_NUM_REC];
  float dv_energy[MAX_NUM_REC];
  float dv_teff[MAX_NUM_REC];
  float dv_eff[MAX_NUM_REC];
  int dv_units[MAX_NUM_REC];
} MOHAVE_DATA;

typedef struct havasu_data {
  float dvpk_loss[MAX_NUM_REC];
  float mwd_pumping[MAX_NUM_REC];
  float cap_pumping[MAX_NUM_REC];
  float pk_rel[MAX_NUM_REC];
  float hv_content[MAX_NUM_REC];
  float hv_elev[MAX_NUM_REC];
  float pk_tw_elev[MAX_NUM_REC];
  float pk_static_head[MAX_NUM_REC];
  float pk_cap[MAX_NUM_REC];
  float pk_energy[MAX_NUM_REC];
  float pk_teff[MAX_NUM_REC];
  float pk_eff[MAX_NUM_REC];
  float headgate_engy[MAX_NUM_REC];
  int pk_units[MAX_NUM_REC];
} HAVASU_DATA;

typedef struct total_data {
   float glen_flow[TWO_MONTH];
   float hoov_rel[TWO_MONTH];
   float hoov_energy[TWO_MONTH];
   float davis_rel[TWO_MONTH];
   float parker_rel[TWO_MONTH];
   float mwd_div[TWO_MONTH];
   float cap_div[TWO_MONTH];
   int  total_startmonth[4];
} TOTAL_DATA;

typedef struct {
    float glen_cfs;
    float glen_accum_af;
    float mead_elev;
    float hoo_rel_cfs;
    float hoo_rel_af;
    float hoo_accum;
    float hoo_ne_gen;
    float hoo_eff;
    float mo_elev;
    float davis_rel_cfs;
    float davis_rel_af;
    float davis_accum;
    float davis_gr_gen;
    float mwd_af;
    float mwd_accum;
    float hav_elev;
    float parker_rel_cfs;
    float parker_rel_af;
    float parker_accum;
    float parker_gr_gen;
    float cap_div_af;
    float cap_accum;
    float hoo_gr_gen;
} daily_accum_vals;

typedef struct {
    float tot_sys_stor;
    float tot_sys_space;
    float mead_swe;
    float mead_cont;
    float mead_rel;
    float mohave_swe;
    float mohave_cont;
    float mohave_rel;
    float havasu_swe;
    float havasu_cont;
    float havasu_rel;
    float senator_swe;
    float senator_cont;
    float senator_pump;
    float senator_rel;
} daily_elev_and_cont_vals;

typedef struct {
    float powell_inflow;
    float powell_elev;
    float powell_cont;
    float powell_rel;
    float flgor_elev;
    float flgor_cont;
    float flgor_rel;
    float navajo_elev;
    float navajo_cont;
    float navajo_rel;
    float blme_elev;
    float blme_cont;
    float blme_rel;
    float morrow_elev;
    float morrow_cont;
    float morrow_rel;
} daily_crsp_vals;

typedef struct {
    float powell_rel;
    float mead_rel;
    float mead_cont;
    float transit_loss_accum;
} daily_loss_values;

/*typedef struct {
    float powell_inflow;
    float powell_elev;
    float powell_cont;
    float powell_rel;
mead inflow -- calculated: mead_inflow = mead_rel + 
storage change + pump + evap + bank stor chg. 
    float mead_swe;
    float mead_cont;
    float mead_af_rel;
    float mead_cfs_rel;
    float hoo_eff;
    float hoo_ne_gen;
    float mohave_swe;
    float mohave_cont;
    float mohave_rel;
    float havasu_swe;
    float havasu_cont;
    float havasu_rel;
    float parker_flow;
    float wheel_flow;
    float pv_flow;
    float taylor_flow;
    float cibola_flow;
    float mwd_div_cfs;
    float mwd_div_af;
    float crir_div; 
    float crir_post_ret;
    float crir_gard_ret;
    float crir_lm_ret;
    float crir_um_ret;
    float crir_tot_ret;
    float pvid_div;
    float pvid_ab_ret;
    float pvid_bl_ret;
    float pvid_net_div;
    float alamo_elev;
    float alamo_cont;
    float alamo_rel;
    float alamo_inflow;
    float senator_swe;
    float senator_cont;
    float senator_pump;
    float senator_rel;
    float cap_div_cfs;
    float cap_div_af;
} long_form_values; */

typedef struct {
    float powell_inflow;
    float powell_elev;
    float powell_cont;
    float powell_rel;
    /* mead inflow -- calculated: mead_inflow = mead_rel + storage change + pump + evap + bank stor chg. */
    float mead_swe;
    float mead_cont;
    float mead_af_rel;
    float mead_cfs_rel;
    float hoo_eff;
    float hoo_ne_gen;
    float hoo_gr_gen;
    float mohave_swe;
    float mohave_cont;
    float mohave_rel;
    float havasu_swe;
    float havasu_cont;
    float havasu_rel;
} long_form_p1;

typedef struct{
    float parker_flow;
    float wheel_flow;
    float pv_flow;
    float taylor_flow;
    float cibola_flow;
    float mwd_div_cfs;
    float mwd_div_af;
    float crir_div; /* main canal flow */
    float crir_post_ret;
    float crir_gard_ret;
    float crir_lm_ret;
    float crir_um_ret;
    float crir_tot_ret;
} long_form_p2;

typedef struct{
    float pvid_div;
    float pvid_ab_ret;
    float pvid_bl_ret;
    float pvid_net_div;
    float alamo_elev;
    float alamo_cont;
    float alamo_rel;
    float alamo_inflow;
    float painted_elev;
    float painted_cont;
    float painted_rel;
    float painted_inflow;
    float senator_swe;
    float senator_cont;
    float senator_pump;
    float senator_rel;
    float cap_div_cfs;
    float cap_div_af;
} long_form_p3;

typedef struct {
    float mead_elev;
    float hoo_rel;
    float mohave_elev;
    float mohave_cont;
    float mohave_rel;
    float havasu_elev;
    float havasu_cont;
    float havasu_rel;
    float moov_elev;
    float crir_div;
    float parker_stage;
    float parker_flow;
    float parker_shift;
    float wheel_stage;
    float wheel_flow;
    float wheel_shift;
    float pv_flow;
    float pv_canal;
    float taylor_stage;
    float taylor_flow;
    float taylor_shift;
    float cibola_stage;
    float cibola_flow;
    float cibola_shift;
    float laguna_stage;
    float laguna_flow;
    float laguna_shift;
    float yuma_stage;
    float yuma_flow;
    float yuma_shift;
    float nib_stage;
    float nib_flow;
    float nib_shift;
    float gila_stage;
    float gila_flow;
    float gila_shift;
    float senwash_elev;
    float senwash_cont;
} river_values;

typedef struct {
    char date[21];
    float font_conteop; 
    float font_reltot;
    float font_elev;
    float font_relave;
    float flgor_conteop;
    float flgor_reltot;
    float flgor_elev;
    float flgor_relave;
    float blme_conteop;
    float blme_reltot;
    float blme_elev;
    float blme_relave;
    float morrow_conteop;
    float morrow_reltot;
    float morrow_elev;
    float morrow_relave;
    float cryst_conteop;
    float cryst_reltot;
    float cryst_elev;
    float cryst_relave;
    float navajo_conteop;
    float navajo_reltot;
    float navajo_elev;
    float navajo_relave;
} upper_values;

typedef struct {
    char date[21];
    float mead_elev;
    float mead_cont;
    float mead_rel;
    float mohave_elev;
    float mohave_cont;
    float mohave_rel;
    float havasu_elev;
    float havasu_cont;
    float havasu_rel;
    float pg_flow;
    float ww_flow;
    float pv_flow;
    float tf_flow;
    float cb_flow;
    float gw_flow;
    float lm_flow;
    float parker_sched_avm1;
    float parker_actual_avm1;
    float parker_sched_avm2;
    float parker_actual_avm2;
    float parker_sched_avm3;
    float parker_actual_avm3;
    float parker_sched_avm4;
    float parker_actual_avm4;
    float parker_sched_plant;
    float davis_sched_avm1;
    float davis_actual_avm1;
    float davis_sched_avm2;
    float davis_actual_avm2;
    float davis_sched_avm3;
    float davis_actual_avm3;
    float davis_sched_avm4;
    float davis_actual_avm4;
    float davis_sched_avm5;
    float davis_actual_avm5;
    float davis_sched_plant;
} morning_values;

typedef struct {
    float powell_elev;
    float powell_inflow;
    float powell_rel;
    float powell_cont;
    float powell_livecap;
    float mead_elev;
    float mead_inflow;
    float mead_rel;
    float mead_cont;
    float mead_livecap;
    float havasu_elev;
    float havasu_inflow;
    float havasu_rel;
    float havasu_reltot;
    float havasu_cont;
    float havasu_livecap;
    float us_uses;
    float yuma_flow;
    float nib_flow;
} friday_values;

typedef struct {
    int day_of_month;
    int to_day;       /* used for the to-date of the weekly forecast values */
    int month_of_year;
    int to_month;     /* used for the to-date of the weekly forecast values */
    float havasu_rel;
    float havasu_elev;
    float mohave_rel;
    float mohave_elev;
    float mead_rel;
    float mead_elev;
    float powell_rel;
} memo_values;

typedef struct {
    int day_of_month;
    int month_of_year;
    float parker_rel;
    float havasu_change;
    float davis_parker_loss;
    float cap_mwd_diver;
    float davis_rel;
} rels_values;


typedef struct {
    int day_of_month;
    int month_of_year;
    float hoover_spill;       /* Release through bypass facilities (cfs). */
    float hoover_release;     /* Release from Hoover power plant (cfs). */
    float hoover_est_gnrtn;   /* Estimated rate of generation (kWh/af). */
    float hoover_gross_gnrtn; /* Gross Hoover generation (megawatt hours). */
    float hoover_loss;        /* Losses and station uses (MkWh). */
    float hoover_net_gnrtn;   /* Net Hoover generation (MkWh). */
} energy_values;

typedef struct {
    float powell_cont;
    float powell_capliv;
    float mead_cont;
    float mead_capliv;
    /* the following are for calculating the total system storage and capliv */
    /*   (in addition to the numbers for mead)                               */
    float mohave_cont;
    float mohave_capliv;
    float havasu_cont;
    float havasu_capliv;
    float senator_cont;
    float senator_capliv;

    float powell_elev;
    float forecast_river;
    float average_river;
    char forecast_title[100];
    int forecast_month;
    int forecast_day;
    int forecast_year;
    float yuma_flow;
    float nib_flow;
    float painted_rel;
    float alamo_rel;
    float morelos_div;
} press_footer_values;
    
typedef struct {
    float glen_rel;
    float mead_rel;
    float mead_elev;
    float mead_cont;
} corps_values;

typedef struct {
    float glen_rel;
    float hoover_rel;
    float mead_cont_1;
    float mead_cont_2;
    float davis_rel_1;
    float davis_rel_2;
    float davis_rel_3;
    float mohave_cont_1;
    float mohave_cont_2;
    float parker_rel;
    float havasu_cont_1;
    float havasu_cont_2;
    float mwd;
    float cap;
} GainLoss;

typedef struct
{
    float hoover_release;
    float hoover_energy;
    float hoover_head;
    float mead_elev;
    float mead_tw;
} Efficiency;

typedef struct 
{
    double flgorg_pred_space;
    double font_pred_space;
    double bluemesa_pred_space;
    double navajo_pred_space;
    double powell_pred_space;
    double upbasin_total;
    double mead_pred_space;
    double pred_total;
    double flgorg_ce_space; /*c-creditable, e-effective*/
    double font_ce_space;
    double bluemesa_ce_space;
    double navajo_ce_space;
    double tot_allow;
    double powell_ce_space;
    double mead_ce_space;
    double total;
    double bom_space;
    double mead_sched_rel;
    double mead_fc_rel;
    double sys_cont;
    double max_cont; /* not available */
} Fld24mon; 

int CreateAccumulationsReport(
#ifdef ANSIPROTO
  FILE *,
  char *,
  int,
  int,
  int
#endif
);

int CreateElevationAndContentReport(
#ifdef ANSIPROTO
  FILE *,
  char *,
  int,
  int,
  int
#endif
);

int CreateDailyCrspReport(
#ifdef ANSIPROTO
  FILE *,
  char *,
  int,
  int,
  int
#endif
);

int CreateDailyLossesReport(
#ifdef ANSIPROTO
  FILE *,
  char *,
  int,
  int,
  int
#endif
);

int SqlFetchMaxContentAndMaxWseLC(
#ifdef ANSIPROTO
  float *,
  float *,
  float *,
  float *,
  float *,
  float *,
  float *,
  float *
#endif
);

int SqlFetchMaxContentAndMaxWseCRSP(
#ifdef ANSIPROTO
  float *,
  float *,
  float *,
  float *,
  float *,
  float *,
  float *,
  float *,
  float *,
  float *
#endif
);

float SqlFetchTotalSystemStorage(
#ifdef ANSIPROTO
  void
#endif
);

int SqlFetchDailyAccumulations(
#ifdef ANSIPROTO
  daily_accum_vals *,
  char *
#endif
);

int SqlFetchDailyElevationsAndContents(
#ifdef ANSIPROTO
  daily_elev_and_cont_vals *,
  char *
#endif
);

int SqlFetchDailyCrsp(
#ifdef ANSIPROTO
  daily_crsp_vals *,
  char *
#endif
);

int SqlGetNumDaysInMonth(
#ifdef ANSIPROTO
  char *
#endif
);

int SqlFetch24MonthStudy(
#ifdef ANSIPROTO
  float *,
  float *,
  char *
#endif
);

int SqlFetchLastMonthsValues(
#ifdef ANSIPROTO
  float *,
  float *,
  float *,
  char *
#endif
);

int SqlFetchDailyLosses(
#ifdef ANSIPROTO
  daily_loss_values *,
  char *
#endif
);

int SqlFetchHourlyRiverP1(
#ifdef ANSIPROTO
  river_values *,
  char *
#endif
);

int SqlFetchDailyRiverP1(
#ifdef ANSIPROTO
  river_values *,
  char *
#endif
);

int SqlFetchHourlyRiverP2(
#ifdef ANSIPROTO
  river_values *,
  char *
#endif
);

int SqlFetchDailyRiverP2(
#ifdef ANSIPROTO
  river_values *,
  char *
#endif
);

int SqlFetchHourlyRiverP3(
#ifdef ANSIPROTO
  river_values *,
  char *
#endif
);

int SqlFetchDailyRiverP3(
#ifdef ANSIPROTO
  river_values *,
  char *
#endif
);

int SqlFetchUpperValues(
#ifdef ANSIPROTO
  upper_values *,
  char *
#endif
);

int SqlFetchMorningValues(
#ifdef ANSIPROTO
  morning_values *,
  char *
#endif
);

int SqlFetchFridayValues(
#ifdef ANSIPROTO
  friday_values *,
  char *
#endif
);

int SqlFetchForecastValues(
#ifdef ANSIPROTO
  friday_values *,
  char *,
  char *,
  char *			  
#endif
);

int SqlFetchMemoValues(
#ifdef ANSIPROTO
  memo_values *,
  char *
#endif
);

int SqlFetchRelsValues(
#ifdef ANSIPROTO
  rels_values *,
  char *
#endif
);


int SqlFetchEnergyValues(
#ifdef ANSIPROTO
  energy_values *,
  char *
#endif
);

int FormatMemoAndPressSection(
#ifdef ANSIPROTO
  FILE *,
  memo_values *
#endif
);

int FormatRels(
#ifdef ANSIPROTO
  FILE *,
  rels_values *
#endif
);

int FormatEnergy(
#ifdef ANSIPROTO
  FILE *,
  energy_values *
#endif
);

int SqlFetchPressFooterValues(
#ifdef ANSIPROTO
  press_footer_values *,
  char *
#endif
);

int SqlFetchCorpsValues(
#ifdef ANSIPROTO
  corps_values *,
  char *
#endif
);

int CreateModelReport (
#ifdef ANSIPROTO
  FILE * fp,
  char * DateString,
  int DayOfMonth,
  int MonthOfYear,
  int Year
#endif
);

int CreateTape6Report (
#ifdef ANSIPROTO
  FILE *,
  int,
  SQL_DATE,		       
  SQL_DATE,		       
  SQL_DATE,		       
  SQL_DATE,		       
  int,
  int
#endif
);
		       
int CalcDates (
#ifdef ANSIPROTO
  int,
  int,
  int,
  SQL_DATE,		       
  SQL_DATE,		       
  SQL_DATE,		       
  SQL_DATE,		       
  int,
  int *, 
  int *
#endif
);

int SqlGetMaxModelDayDate (
#ifdef ANSIPROTO
  int,
  char *
#endif
);

int SetDates (
#ifdef ANSIPROTO
  TWO_DATES *,
  SQL_DATE,		       
  int
#endif
);

int PrintHeaders (
#ifdef ANSIPROTO
  FILE *,
  char *,
  char *,
  char *,
  char *,
  char *,
  int
#endif
);

int SqlFetchMeadModelData (
#ifdef ANSIPROTO
  MEAD_DATA *,
  TWO_DATES *,
  int,
  int		   
#endif
);

int SqlFetchTotalData (
#ifdef ANSIPROTO
  TOTAL_DATA *,
  SQL_DATE,
  SQL_DATE,
  SQL_DATE
#endif
);

int PrintTotalData (
#ifdef ANSIPROTO
  FILE *,
  TOTAL_DATA
#endif
);

int PrintMeadData (
#ifdef ANSIPROTO
  FILE *,
  MEAD_DATA,
  TWO_DATES *,
  int,
  int,
  int   
#endif
);

int GenMeadWeeklyValues(
#ifdef ANSIPROTO
  FILE *,
  MEAD_DATA,
  int		   
#endif
);

int SqlFetchMohaveModelData (
#ifdef ANSIPROTO
  MOHAVE_DATA *,
  TWO_DATES *,
  int,
  int		   
#endif
);

int PrintMohaveData (
#ifdef ANSIPROTO
  FILE *,
  MOHAVE_DATA,
  TWO_DATES *,
  int,
  int,
  int   
#endif
);

int GenMohaveWeeklyValues(
#ifdef ANSIPROTO
  FILE *,
  MOHAVE_DATA,
  int		   
#endif
);

int SqlFetchSingleValue(
#ifdef ANSIPROTO
  int,
  char *,
  float *
#endif
);

int SqlFetchSingleTape6Value(
#ifdef ANSIPROTO
  char *,
  int,
  char *,
  float *
#endif
);

int SqlFetchLongFormPoneValues(
#ifdef ANSIPROTO
  long_form_p1 *,
  char *
#endif
);

int SqlFetchLongFormPtwoValues(
#ifdef ANSIPROTO
  long_form_p2 *,
  char *
#endif
);

int SqlFetchLongFormPthreeValues(
#ifdef ANSIPROTO
  long_form_p3 *,
  char *
#endif
);

int CreateLongFromPone(
#ifdef ANSIPROTO
  FILE *,
  char *,
  int,
  int,
  int
#endif
);

int CreateLongFromPtwo(
#ifdef ANSIPROTO
  FILE *,
  char *,
  int,
  int,
  int
#endif
);

int CreateLongFromPthree(
#ifdef ANSIPROTO
  FILE *,
  char *,
  int,
  int,
  int
#endif
);

int SqlValidModelRunId(
#ifdef ANSIPROTO
   int
#endif
);

int SqlFetch24monFldValues(
#ifdef ANSIPROTO
   Fld24mon *, 
   int, 
   int, 
   int,
   int
#endif
);

int Create24moFldReport(
#ifdef  ANSIPROTO
   FILE *,
   char *,
   int,
   int,
   int,
   int
#endif
);

int SqlFetchDateByModelRunId(
#ifdef ANSIPROTO
   int model_run_run, 
   int * MonthOfYear, 
   int * Year
#endif
);

int SqlInsertNewGainValue(
#ifdef ANSIPROTO
   char *
#endif
);

int SqlInsertEfficiencyValue(
#ifdef ANSIPROTO
   char *
#endif
);

int SqlCalGain(
#ifdef ANSIPROTO
   char * , 
   float *
#endif
);

int SqlFetchGainLossData(
#ifdef ANSIPROTO
   GainLoss *, 
   char *
#endif
);

int SqlFetchEfficiencyData(
#ifdef ANSIPROTO
   Efficiency *, 
   char *
#endif
);

#endif
