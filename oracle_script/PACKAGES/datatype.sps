/* Spec only, as there are no procedures or functions in this package,
   only variables accessed by other triggers and procedures. */
create or replace package datatype_pkg as  
   type      rowid_tab_type is table of rowid index by binary_integer;  
   type      dt_id_tab_type is table of number(11) index by binary_integer; 
   type      dt_type_tab_type is table of varchar2(32) index by binary_integer; 

   rowid_tab    rowid_tab_type;
   dt_id_tab    dt_id_tab_type;  
   dt_type_tab  dt_type_tab_type;  
   datatype_index binary_integer;  
end datatype_pkg;  
/  
show error package datatype_pkg  
/
exit