/* Recreate views invalidated by dropping and recreating tables */
CREATE OR REPLACE VIEW HDB_DATATYPE_UNIT AS
select 'D' unit_ind, datatype_id dest_id, datatype_name dest_name,
  dimension_name dimension_name               
from hdb_datatype, hdb_dimension, hdb_unit
where hdb_datatype.unit_id = hdb_unit.unit_id
  and hdb_unit.dimension_id = hdb_dimension.dimension_id
union   
select 'U' unit_ind, unit_id dest_id, unit_name dest_name, null 
from hdb_unit                                                                
/

grant select on hdb_datatype_unit to public;
create public synonym hdb_datatype_unit for hdb_datatype_unit;
