create or replace trigger datatype_bef_stmt_all  
before insert or update or delete on hdb_datatype  
declare
 hold_dt_id      number(11);
 hold_dt_type    varchar2(32);
 hold_rowid      rowid;
 cursor datatype_cur is
  select rowid, datatype_id, datatype_type
  from hdb_datatype;
begin  
   /*  
   Remember to reset the pl/sql table before each statement  
   */  
   datatype_pkg.datatype_index := 0;  

   open datatype_cur;
   loop
    fetch datatype_cur into
      hold_rowid, hold_dt_id, hold_dt_type;
    exit when datatype_cur%notfound;
    datatype_pkg.datatype_index := datatype_pkg.datatype_index + 1;  
    datatype_pkg.rowid_tab(datatype_pkg.datatype_index) := hold_rowid;
    datatype_pkg.dt_id_tab(datatype_pkg.datatype_index) := hold_dt_id;
    datatype_pkg.dt_type_tab(datatype_pkg.datatype_index) := hold_dt_type;
   end loop;

   close datatype_cur;
end;
/
show error trigger datatype_bef_stmt_all  
/

