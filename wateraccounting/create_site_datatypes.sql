set echo on
spool create_site_datatype.out

alter trigger HDB_SITE_DATATYPE_PK_TRIG disable;

column max_num NEW_VALUE sdi_seq_start_number noprint
select max(site_datatype_id)+1 max_num from hdb_site_datatype;
create sequence temp_seq start with &&sdi_seq_start_number nocache;

insert into hdb_site_datatype (site_id,datatype_id,site_datatype_id)
select rwa.site_id,hd.datatype_id,temp_seq.nextval from ref_wa_acct rwa, ref_wa_accttype acct, hdb_datatype hd 
where 
    rwa.accttype_id = acct.accttype_id
and ACCT.ACCTTYPE_NAME = 'StorageAccount'
and hd.datatype_id in (29,1117);
commit;

insert into ref_derivation_source(site_datatype_id,effective_start_date_time,interval)
select hsd.site_datatype_id,sysdate,'day' from hdb_site_datatype hsd, ref_wa_acct rwa, ref_wa_accttype acct
where
    rwa.accttype_id = acct.accttype_id
and acct.ACCTTYPE_NAME = 'StorageAccount'
and hsd.datatype_id in (29,1117)
and hsd.site_id = rwa.site_id;
commit;


insert into hdb_site_datatype (site_id,datatype_id,site_datatype_id)
select rws.site_id,hd.datatype_id,temp_seq.nextval from ref_wa_supply rws, hdb_datatype hd 
where  hd.datatype_id in (19);
commit;

insert into ref_derivation_source(site_datatype_id,effective_start_date_time,interval)
select hsd.site_datatype_id,sysdate,'day' from hdb_site_datatype hsd, ref_wa_supply rws
where
    hsd.site_id = rws.site_id
and hsd.datatype_id in (19);
commit;


insert into hdb_site_datatype
(select a.site_id, b.datatype_id, temp_seq.nextval
 from ref_wa_dmi_site_map a, 
      temp_wa_slot_datatype_map b,
      ref_wa_slot_data c
 where c.section in (' Account', ' Exchange', ' Payback')
   and not (c.object_type = 'StorageAccount' AND c.slot in ('Inflow', 'Outflow'))
   and c.object = a.object_name
   and c.slot = b.slot_name);
commit;

insert into hdb_site_datatype
select a.site_id, b.datatype_id, temp_seq.nextval
 from ref_wa_dmi_site_map a,
      temp_wa_slot_datatype_map b,
      temp_wa_dmi_phys_obj_slot c
 where a.object_name = c.object_name
   and c.slot_name = b.slot_name;

commit;

alter trigger HDB_SITE_DATATYPE_PK_TRIG enable;
drop sequence temp_seq;

insert into ref_dmi_data_map (model_id,object_name,data_name,site_datatype_id)
select 17,get_hdb_site_name(hsd.site_id),decode(hd.datatype_id,29,'Inflow','Outflow'),hsd.site_datatype_id
 from hdb_site_datatype hsd, ref_wa_acct rwa, hdb_datatype hd 
where 
    hsd.site_id = rwa.site_id
and hsd.datatype_id = hd.datatype_id; 
commit;

insert into ref_dmi_data_map (model_id,object_name,data_name,site_datatype_id)
select 17,get_hdb_site_name(hsd.site_id),'Supply',hsd.site_datatype_id
 from hdb_site_datatype hsd, ref_wa_supply rws
where
    hsd.site_id = rws.site_id;

commit;

REM accting union'd w/ phys
insert into ref_dmi_data_map
(select c.model_id, a.object, a.slot, b.site_datatype_id
 from ref_wa_slot_data a, hdb_site_datatype b,
      ref_wa_dmi_site_map c, temp_wa_slot_datatype_map d
 where a.object = c.object_name
   and a.slot = d.slot_name
   and c.site_id = b.site_id
   and d.datatype_id = b.datatype_id
   and a.section in (' Account', ' Exchange', ' Payback')
   and not (a.object_type = 'StorageAccount' AND a.slot in ('Inflow', 'Outflow'))
 UNION
 select c.model_id, a.object_name, a.slot_name, b.site_datatype_id
 from temp_wa_dmi_phys_obj_slot a, hdb_site_datatype b,
      ref_wa_dmi_site_map c, temp_wa_slot_datatype_map d
 where a.object_name = c.object_name
   and a.slot_name = d.slot_name
   and c.site_id = b.site_id
   and d.datatype_id = b.datatype_id);
commit;

REM accting union'd w/ phys
insert into ref_derivation_source
(select b.site_datatype_id, sysdate, 'day', null, null, null, null, null, null
 from ref_wa_slot_data a, hdb_site_datatype b,
      ref_wa_dmi_site_map c, temp_wa_slot_datatype_map d
 where a.object = c.object_name
   and a.slot = d.slot_name
   and c.site_id = b.site_id
   and d.datatype_id = b.datatype_id
   and a.section in (' Account', ' Exchange', ' Payback')
   and not (a.object_type = 'StorageAccount' AND a.slot in ('Inflow', 'Outflow'))
 UNION
 select b.site_datatype_id, sysdate, 'day', null, null, null, null, null, null
 from temp_wa_dmi_phys_obj_slot a, hdb_site_datatype b,
      ref_wa_dmi_site_map c, temp_wa_slot_datatype_map d
 where a.object_name = c.object_name
   and a.slot_name = d.slot_name
   and c.site_id = b.site_id
   and d.datatype_id = b.datatype_id);
commit;

spool off
