set echo off
set heading off
set feedback off
set pagesize 0
set linesize 180
set verify off
spool ControlFilePhase2
REM non passthrough accts & supplies, passthrough accts, nonNambe series, Nambe series
SELECT a.object || '.' || a.slot || ': file=~/%o.%s units=' || b.rw_unit_name || ' scale=1.0 !destination=r'
FROM ref_wa_slot_data a, temp_wa_slot_datatype_map b
WHERE a.section in ( ' Account', ' Supply')
      and a.slot = b.slot_name
      and a.object_type <> 'PassThroughAccount'
UNION
SELECT a.object || '.' || a.slot || ': file=~/%o.%s units=' || b.rw_unit_name || ' scale=1.0 !destination=r'
FROM ref_wa_slot_data a, temp_wa_slot_datatype_map b
WHERE a.section in (' Account')
      and a.slot = b.slot_name
      and a.object_type = 'PassThroughAccount'
UNION
SELECT a.object_name || '.' || a.slot_name || ': file=~/%o.%s units=' || b.rw_unit_name || ' scale=1.0 !destination=r'
FROM temp_wa_dmi_phys_obj_slot a, temp_wa_slot_datatype_map b
WHERE a.slot_name = b.slot_name
  and instr (a.slot_name, '.') = 0
  and object_name <> 'Nambe'
UNION
SELECT a.object_name || '.' || a.slot_name || ': file=~/%o.%s units=' || b.rw_unit_name || ' scale=1.0 !destination=r'
FROM temp_wa_dmi_phys_obj_slot a, temp_wa_slot_datatype_map b
WHERE a.slot_name = b.slot_name
  and instr (a.slot_name, '.') = 0
  and object_name = 'Nambe'
ORDER BY 1;
spool off