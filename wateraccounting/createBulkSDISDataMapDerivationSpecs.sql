alter trigger hdb_site_datatype_pk_trig disable;
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

insert into hdb_site_datatype
select a.site_id, b.datatype_id, temp_seq.nextval
 from ref_wa_dmi_site_map a,
      temp_wa_slot_datatype_map b,
      temp_wa_dmi_phys_obj_slot c
 where a.object_name = c.object_name
   and c.slot_name = b.slot_name;

alter trigger hdb_site_datatype_pk_trig enable;
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