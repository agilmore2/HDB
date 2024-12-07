create or replace TRIGGER HDB_DIMENSION_SNAP_CZAR_MODIFIED
after             insert OR update OR delete
on                HDB_DIMENSION
for   each row
declare
begin

     /* populate the ref_czar_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_czar_modified('HDB_DIMENSION');

end;
/

create or replace TRIGGER HDB_UNIT_SNAP_CZAR_MODIFIED
after             insert OR update OR delete
on                HDB_UNIT
for   each row
declare
begin

     /* populate the ref_czar_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_czar_modified('HDB_UNIT');

end;
/

create or replace TRIGGER HDB_PHYSICAL_QUANTITY_SNAP_CZAR_MODIFIED
after             insert OR update OR delete
on                HDB_PHYSICAL_QUANTITY
for   each row
declare
begin

     /* populate the ref_czar_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_czar_modified('HDB_PHYSICAL_QUANTITY');

end;
/