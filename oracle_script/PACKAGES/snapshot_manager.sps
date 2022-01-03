create or replace package SNAPSHOT_MANAGER as
/*  PACKAGE SNAPSHOT_MANAGER is the package designed to contain all
    the procedures and functions for general SNAPSHOT_MANAGER use.
    
    Created by M. Bogner April 2013  
    Added new CZAR procedures by Ismail O - Dec 2021
*/


/*  DECLARE ALL GLOBAL variables  */
   G_SNAPSHOT_MANAGER_KEY VARCHAR2(50) := 'SNAPSHOT_MANAGER';
   G_SNAPSHOT_CZAR_MANAGER_KEY VARCHAR2(50) := 'SNAPSHOT_CZAR_MANAGER';

-- This procedure performs all the business rules required when a master table has been modified
  PROCEDURE SNAPSHOT_MODIFIED(P_TABLE_NAME VARCHAR2);

-- This procedure performs all the required actions the snapshot manager must do during a refresh  
  PROCEDURE PERFORM_REFRESH(P_TABLE_NAME VARCHAR2);
  
  -- This procedure performs all the business rules required when a master table has been modified for CZAR tables
  PROCEDURE SNAPSHOT_CZAR_MODIFIED(P_TABLE_NAME VARCHAR2);

-- This procedure performs all the required actions the snapshot manager must do during a refresh  for CZAR tables
  PROCEDURE PERFORM_CZAR_REFRESH(P_TABLE_NAME VARCHAR2);
    
END SNAPSHOT_MANAGER;
.
/

create or replace public synonym SNAPSHOT_MANAGER for SNAPSHOT_MANAGER;
grant execute on SNAPSHOT_MANAGER to app_role;
grant execute on SNAPSHOT_MANAGER to savoir_faire;
