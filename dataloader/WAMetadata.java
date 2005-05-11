package dataloader;

import java.io.*;
import java.lang.Integer;
import java.util.*;
import dbutils.*;
import java.sql.*;

public class WAMetadata {

  private Hashtable hash = null;
  private String file_name = null;
  private Logger log = null;
  private int exchanges = 0;
  private int supplies = 0;
  private int paybacks = 0;
  private int errors = 0;
  private DBAccess db = null;
  private boolean debug = false;
  private Connection cnn = null;
  private String comma = ",";
  private String apos = "'";
  private String db_site_code = "HDB";
  private int config_id = 0;
  private int model_id = 0;
  private DataObject pdobj = null;
//  variables to handle the account processing
  private int account_deletes = 0;
  private int account_modifies = 0;
  private int account_adds = 0;
  private String deleted_accts = null;
  private String del_accts_count = null;
  private  String new_accts = null;
  private String mod_accts = null;
//  variables to handle the supplies processing
  private int supply_deletes = 0;
  private int supply_modifies = 0;
  private int supply_adds = 0;
  private String deleted_supplies = null;
  private String del_supplies_count = null;
  private  String new_supplies = null;
  private String mod_supplies = null;

//  variables to handle the exchanges processing
  private int exchanges_deletes = 0;
  private int exchanges_modifies = 0;
  private int exchanges_adds = 0;
  private String deleted_exchanges = null;
  private String del_exchanges_count = null;
  private  String new_exchanges = null;
  private String mod_exchanges = null;

//  variables to handle the paybacks processing
  private int paybacks_deletes = 0;
  private int paybacks_modifies = 0;
  private int paybacks_adds = 0;
  private String deleted_paybacks = null;
  private String del_paybacks_count = null;
  private  String new_paybacks = null;
  private String mod_paybacks = null;

  //  boolean to indicate whether the object is an insert or a mod
  private boolean object_insert = false;

  public WAMetadata( String _file_name)
  {
    file_name = _file_name; 
    log = Logger.getInstance();
    db = new DBAccess(cnn);
    pdobj = new DataObject();

    try 
    {

      pdobj.addPropertyFile(System.getProperty("start.property"));
      if (!(pdobj.get("model_id") == null || ((String)pdobj.get("model_id")).length() == 0)) model_id = Integer.parseInt((String)pdobj.get("model_id"));
      if (!(pdobj.get("config_id") == null || ((String)pdobj.get("config_id")).length() == 0)) config_id = Integer.parseInt((String)pdobj.get("config_id"));
      if (!(pdobj.get("db_site_code") == null || ((String)pdobj.get("db_site_code")).length() == 0)) db_site_code = (String)pdobj.get("db_site_code");

      cnn = db.getConnection(pdobj);
    }
    catch (Exception e) {System.out.println(e.toString());}

  }


public void process()

 { // beginning of process method

   ArrayList records = null;
   DataObject temp_dobj = new DataObject();
   String status = null;

    //  first setup all the query strings necessary to perform the metadata update
    setup_queries();

//  handle all the accounts first

//  do the deletes first
    System.out.println("Processing DELETED Accounts...");
    status = db.performDML(deleted_accts,pdobj);

//  now go get the # of accounts logically deleted
    status = db.performQuery(del_accts_count,temp_dobj);
    account_deletes = Integer.parseInt((String)temp_dobj.get("acct_del_counts"));
    System.out.println("DELETED Accounts Processed: "+ account_deletes);
    System.out.println("");

//  now get the modified accounts 
//  loop through the list arraylist of modified accounts
//  and process the modified accounts
    System.out.println("Processing MODIFIED Accounts...");
    records = db.get_arraylist_query(mod_accts,pdobj);
    object_insert = false;
    for (int i = 0; i < records.size(); i++)
    {
      processAccount((DataObject)records.get(i));
      account_modifies ++;
    
    }
    System.out.println("MODIFIED Accounts Processed: "+ account_modifies);
    System.out.println("");

//  now get the new accounts 
//  loop through the list arraylist of new accounts
//  and process the new accounts
    System.out.println("Processing NEW Accounts...");
    object_insert = true;
    records = db.get_arraylist_query(new_accts,pdobj);
    for (int i = 0; i < records.size(); i++)
    {
      processAccount((DataObject)records.get(i));
      account_adds ++;

    } 
    System.out.println("NEW Accounts Processed: "+ account_adds);
    System.out.println("");
 
//  now handle all the supplies

//  do the supplies deletes first
    System.out.println("Processing Deleted Supplies...");
    status = db.performDML(deleted_supplies,pdobj);

//  now go get the # of supplies logically deleted
    status = db.performQuery(del_supplies_count,temp_dobj);
    supply_deletes = Integer.parseInt((String)temp_dobj.get("supply_del_counts"));
    System.out.println("Deleted Supplies Processed: " + supply_deletes);
    System.out.println("");


//  now get the modified supplies 
//  loop through the list arraylist of modified supplies
//  and process the modified supplies
    System.out.println("Processing MODIFIED Supplies...");
    records = db.get_arraylist_query(mod_supplies,pdobj);
    object_insert = false;
    for (int i = 0; i < records.size(); i++)
    {
      //System.out.println(records.get(i).toString());
      processSupply((DataObject)records.get(i));
      supply_modifies ++;
    
    }
    System.out.println("MODIFIED Supplies Processed: "+ supply_modifies);
    System.out.println("");


//  now get the new supplies 
//  loop through the list arraylist of new supplies
//  and process the new supplies
    System.out.println("Processing NEW Supplies...");
    object_insert = true;
    records = db.get_arraylist_query(new_supplies,pdobj);
    for (int i = 0; i < records.size(); i++)
    {
      //System.out.println(records.get(i).toString());
      processSupply((DataObject)records.get(i));
      supply_adds ++;

    } 
    System.out.println("NEW Supplies Processed: "+ supply_adds);
    System.out.println("");
 
//  now handle all the exchanges

//  do the exchanges deletes first
    System.out.println("Processing Deleted Exchanges...");
    status = db.performDML(deleted_exchanges,pdobj);

//  now go get the # of Exchanges logically deleted
    status = db.performQuery(del_exchanges_count,temp_dobj);
    exchanges_deletes = Integer.parseInt((String)temp_dobj.get("exchanges_del_counts"));
    System.out.println("Deleted Exchanges Processed: " + exchanges_deletes);
    System.out.println("");

//  now get the modified exchanges 
//  loop through the list arraylist of modified exchanges
//  and process the modified exchanges
    System.out.println("Processing MODIFIED Exchanges...");
    records = db.get_arraylist_query(mod_exchanges,pdobj);
    object_insert = false;
    for (int i = 0; i < records.size(); i++)
    {
      //System.out.println(records.get(i).toString());
      processExchange((DataObject)records.get(i));
      exchanges_modifies ++;
    
    }
    System.out.println("MODIFIED Exchanges Processed: "+ exchanges_modifies);
    System.out.println("");

//  now get the new exchanges 
//  loop through the list arraylist of new exchanges
//  and process the new exchanges
    System.out.println("Processing NEW Exchanges...");
    records = db.get_arraylist_query(new_exchanges,pdobj);
    object_insert = true;
    for (int i = 0; i < records.size(); i++)
    {
      //System.out.println(records.get(i).toString());
      processExchange((DataObject)records.get(i));
      exchanges_adds ++;
    
    }
    System.out.println("NEW Exchanges Processed: "+ exchanges_adds);
    System.out.println("");
 
//  now handle all the paybacks

//  do the paybacks deletes first
    System.out.println("Processing Deleted Paybacks...");
    status = db.performDML(deleted_paybacks,pdobj);

//  now go get the # of paybacks logically deleted
    status = db.performQuery(del_paybacks_count,temp_dobj);
    paybacks_deletes = Integer.parseInt((String)temp_dobj.get("paybacks_del_counts"));
    System.out.println("Deleted Paybacks Processed: " + paybacks_deletes);
    System.out.println("");

//  now get the modified paybacks 
//  loop through the list arraylist of modified paybacks
//  and process the modified paybacks
    System.out.println("Processing MODIFIED Paybacks...");
    records = db.get_arraylist_query(mod_paybacks,pdobj);
    object_insert = false;
    for (int i = 0; i < records.size(); i++)
    {
      //System.out.println(records.get(i).toString());
      processPayback((DataObject)records.get(i));
      paybacks_modifies ++;
    
    }
    System.out.println("MODIFIED Paybacks Processed: "+ paybacks_modifies);
    System.out.println("");

//  now get the new paybacks 
//  loop through the list arraylist of new paybacks
//  and process the new paybacks
    System.out.println("Processing NEW Paybacks...");
    records = db.get_arraylist_query(new_paybacks,pdobj);
    object_insert = true;
    for (int i = 0; i < records.size(); i++)
    {
      //System.out.println(records.get(i).toString());
      processPayback((DataObject)records.get(i));
      paybacks_adds ++;
    
    }
    System.out.println("NEW Paybacks Processed: "+ paybacks_adds);
    System.out.println("");

 }  // end of process method


    private void get_col(DataObject do2, String table_names, String table_columns, String where_clause)  
     // method used to get any columns from any tables
    {

       String query = null;
       String result = null;
       String error_message = null;

       // get the  values
       query = "select " + table_columns+ " from " + table_names +  " where "  + where_clause;
       //System.out.println(query);
       result = db.performQuery(query,do2);
       if (result.startsWith("ERROR"))
       {
         error_message = "GET Columns Method FAILED" + result;
         log.debug( this,"  " + query + "  :" + error_message);
       }


       if (debug) log.debug( this,"  " + query + "  :" + " PASSED");

    } // end of get_col method


    private boolean tbl_insert( String table_name, String table_columns, String value_clause)  
     // method used to insert rows of data into passed in table
    {

       String dml = null;
       String result = null;
       String error_message = null;
       DataObject _do = null;

       // get the  values
       dml = "insert into " + table_name + " (" + table_columns +  ") values ( "  + value_clause + ")";
       //System.out.println(dml);
       result = db.performDML(dml,_do);
       if (result.startsWith("ERROR"))
       {
         error_message = "TBL_INSERT Method FAILED" + result;
         log.debug( this,"  " + dml + "  :" + error_message);
         return false;
       }

       if (debug) log.debug( this,"  " + dml + "  :" + " PASSED");
       return true;

    } // end of tbl_insert method


    private boolean get_parent(DataObject _dobj, String _add)  
     // method used to get any reservoir from the hdb_site table
    {

       String parent = (String) _dobj.get("ACCT_PARENT");
       String error_message = null;
       String obj_name = ((String) _dobj.get("ACCTOBJTYPE_NAME")).toLowerCase();
       boolean _cont = true;

       this.get_col(_dobj,"hdb_site","site_id parent_site_id, objecttype_id parent_objecttype_id","site_name ='" + parent + "'");
       if ((_dobj.get("parent_site_id") == null || ((String)_dobj.get("parent_site_id")).length() == 0) && _add.equals("ADD")) 
       {

         //  handle the Riverware unique object type name to correspond to database lookups
         if (obj_name.equals("streamgage")) obj_name = "stream gage";
         if (obj_name.indexOf("reservoir") != -1) obj_name = "reservoir";

         // now go get the object id for this object type
         this.get_col(_dobj,"hdb_objecttype","objecttype_id parent_objecttype_id","objecttype_name ='"+ obj_name +"'");
         _cont = this.tbl_insert("hdb_site","site_name,site_common_name,objecttype_id,db_site_code",
                 apos + parent + apos + comma + apos + parent + apos + comma + _dobj.get("parent_objecttype_id") + comma + apos +
                 db_site_code + apos); 
         this.get_col(_dobj,"hdb_site","site_id parent_site_id, objecttype_id parent_objecttype_id","site_name ='" + parent + "'");
         if (_dobj.get("parent_site_id") == null) _cont = false; 

         if( _cont) _cont = 
          this.tbl_insert("ref_wa_dmi_site_map","model_id,object_name,effective_start_date_time,objecttype_id,site_id",
          model_id + comma + apos + parent + apos + comma + "trunc(sysdate)"+ comma + 
          _dobj.get("parent_objecttype_id") + comma + _dobj.get("parent_site_id"));

       }

       if (! _cont)
       {
         error_message = parent + " GET Parent Method FAILED" ;
         log.debug( this,"  " + error_message);
       }

       if (debug) log.debug( this,"  " + parent + "  :" + " PASSED");
       return _cont;

    } // end of get_parent method


    private boolean get_water_owner(DataObject _dobj, String _add)  
     // method used to get any water_owner_id from the ref_wa_water_owner table
    {

       String error_message = null;
       String water_owner_name = (String) _dobj.get("water_owner_name");
       boolean _cont = true;

       this.get_col(_dobj,"ref_wa_water_owner","water_owner_id","water_owner_name ='" + water_owner_name + "'");
       if ((_dobj.get("water_owner_id") == null || ((String)_dobj.get("water_owner_id")).length() == 0) && _add.equals("ADD")) 
       {

         _cont = this.tbl_insert(
           "ref_wa_water_owner","water_owner_id,water_owner_name,water_owner_common_name,effective_start_date",
           "ref_wa_water_owner_seq.nextval" + comma + apos + water_owner_name + apos + comma + apos + water_owner_name + apos +
           comma + "trunc(sysdate)"  ); 
         this.get_col(_dobj,"ref_wa_water_owner","water_owner_id","water_owner_name ='" + water_owner_name + "'");
         if (_dobj.get("water_owner_id") == null) _cont = false; 
       }

       if (! _cont)
       {
         error_message = water_owner_name + " GETwaterowner Method FAILED" ;
         log.debug( this,"  " + error_message);
       }

       if (debug) log.debug( this,"  " + water_owner_name + "  :" + " PASSED");
       return _cont;

    } // end of get_water_owner method


    private boolean get_water_type(DataObject _dobj, String _add)  
     // method used to get any watertype from the ref_wa_watertype table
    {

       String error_message = null;
       String watertype_name = (String) _dobj.get("watertype_name");
       boolean _cont = true;

       this.get_col(_dobj,"ref_wa_watertype","watertype_id","watertype_name ='" + watertype_name + "'");
       if ((_dobj.get("watertype_id") == null || ((String)_dobj.get("watertype_id")).length() == 0) && _add.equals("ADD")) 
       {

         _cont = this.tbl_insert(
           "ref_wa_watertype","watertype_id,watertype_name,watertype_common_name,effective_start_date",
           "ref_wa_watertype_seq.nextval" + comma + apos + watertype_name + apos + comma + apos + watertype_name + apos +
           comma + "trunc(sysdate)"  ); 
         this.get_col(_dobj,"ref_wa_watertype","watertype_id","watertype_name ='" + watertype_name + "'");
         if (_dobj.get("watertype_id") == null || ((String)_dobj.get("watertype_id")).length() == 0)  _cont = false; 
       }

       if (! _cont)
       {
         error_message = watertype_name + " GETwatertype Method FAILED" ;
         log.debug( this,"  " + error_message);
       }

       if (debug) log.debug( this,"  " + watertype_name + "  :" + " PASSED");
       return _cont;

    } // end of get_watertype method



    private boolean get_accttype(DataObject _dobj, String _add)  
     // method used to get any accttype from the ref_wa_accttype table
    {

       String error_message = null;
       String accttype_name = (String) _dobj.get("accttype_name");
       boolean _cont = true;

       this.get_col(_dobj,"ref_wa_accttype","accttype_id","accttype_name ='" + accttype_name + "'");
       if ((_dobj.get("accttype_id") == null || ((String)_dobj.get("accttype_id")).length() == 0) && _add.equals("ADD")) 
       {

         _cont = this.tbl_insert(
           "ref_wa_accttype","accttype_id,accttype_name,is_legal_entity,accttype_common_name,effective_start_date",
           "ref_wa_accttype_seq.nextval" + comma + apos + accttype_name + apos + comma + "'N'"  + comma
           + apos + accttype_name + apos + comma + "trunc(sysdate)"  ); 
         this.get_col(_dobj,"ref_wa_accttype","accttype_id","accttype_name ='" + accttype_name + "'");
         if (_dobj.get("accttype_id") == null || ((String)_dobj.get("accttype_id")).length() == 0)  _cont = false;
       }

       if (! _cont)
       {
         error_message = accttype_name + " GETaccttype Method FAILED" ;
         log.debug( this,"  " + error_message);
       }

       if (debug) log.debug( this,"  " + accttype_name + "  :" + " PASSED");
       return _cont;

    } // end of get_accttype method


    private boolean get_release_type(DataObject _dobj, String _add)  
     // method used to get any releasetype from the ref_wa_releasetype table
    {

       String error_message = null;
       String releasetype_name = (String) _dobj.get("releasetype_name");
       boolean _cont = true;

       this.get_col(_dobj,"ref_wa_releasetype","releasetype_id","releasetype_name ='" + releasetype_name + "'");
       if ((_dobj.get("releasetype_id") == null || ((String)_dobj.get("releasetype_id")).length() == 0) && _add.equals("ADD")) 
       {

         _cont = this.tbl_insert(
           "ref_wa_releasetype","releasetype_id,releasetype_name,releasetype_common_name,effective_start_date",
           "ref_wa_releasetype_seq.nextval" + comma + apos + releasetype_name + apos + comma + apos + releasetype_name + apos +
           comma + "trunc(sysdate)"  ); 
         this.get_col(_dobj,"ref_wa_releasetype","releasetype_id","releasetype_name ='" + releasetype_name + "'");
         if (_dobj.get("releasetype_id") == null || ((String)_dobj.get("releasetype_id")).length() == 0)  _cont = false; 
       }

       if (! _cont)
       {
         error_message = releasetype_name + " GETreleasetype Method FAILED" ;
         log.debug( this,"  " + error_message);
       }

       if (debug) log.debug( this,"  " + releasetype_name + "  :" + " PASSED");
       return _cont;

    } // end of get_release_type method


    private boolean get_supply_type(DataObject _dobj, String _add)  
     // method used to get any supplytype from the ref_wa_supplytype table
    {

       String error_message = null;
       String supplytype_name = (String) _dobj.get("supplytype_name");
       boolean _cont = true;

       this.get_col(_dobj,"ref_wa_supplytype","supplytype_id","supplytype_name ='" + supplytype_name + "'");
       if ((_dobj.get("supplytype_id") == null || ((String)_dobj.get("supplytype_id")).length() == 0) && _add.equals("ADD")) 
       {

         _cont = this.tbl_insert(
           "ref_wa_supplytype","supplytype_id,supplytype_name,supplytype_common_name,effective_start_date",
           "ref_wa_supplytype_seq.nextval" + comma + apos + supplytype_name + apos + comma + apos + supplytype_name + apos +
           comma + "trunc(sysdate)"  ); 
         this.get_col(_dobj,"ref_wa_supplytype","supplytype_id","supplytype_name ='" + supplytype_name + "'");
         if (_dobj.get("supplytype_id") == null || ((String)_dobj.get("supplytype_id")).length() == 0)  _cont = false; 
       }

       if (! _cont)
       {
         error_message = supplytype_name + " GETsupplytype Method FAILED" ;
         log.debug( this,"  " + error_message);
       }

       if (debug) log.debug( this,"  " + supplytype_name + "  :" + " PASSED");
       return _cont;

    } // end of get_supply_type method


    private boolean get_supply_role(DataObject _dobj, String _add)  
     // method used to get any supply_role from the ref_wa_supply_role table
    {

       String error_message = null;
       String supply_role_name = (String) _dobj.get("supply_role_name");
       boolean _cont = true;

       this.get_col(_dobj,"ref_wa_supply_role","supply_role_id","supply_role_name ='" + supply_role_name + "'");
       if ((_dobj.get("supply_role_id") == null || ((String)_dobj.get("supply_role_id")).length() == 0) && _add.equals("ADD")) 
       {

         _cont = this.tbl_insert(
           "ref_wa_supply_role","supply_role_id,supply_role_name,supply_role_common_name,effective_start_date",
           "ref_wa_supply_role_seq.nextval" + comma + apos + supply_role_name + apos + comma + apos + supply_role_name + apos +
           comma + "trunc(sysdate)"  ); 
         this.get_col(_dobj,"ref_wa_supply_role","supply_role_id","supply_role_name ='" + supply_role_name + "'");
         if (_dobj.get("supply_role_id") == null || ((String)_dobj.get("supply_role_id")).length() == 0)  _cont = false; 
       }

       if (! _cont)
       {
         error_message = supply_role_name + " GETsupplyrole Method FAILED" ;
         log.debug( this,"  " + error_message);
       }

       if (debug) log.debug( this,"  " + supply_role_name + "  :" + " PASSED");
       return _cont;

    } // end of get_supply_role method




    private boolean get_destination(DataObject _dobj, String _add)  
     // method used to get any destination from the ref_wa_destination table
    {

       String error_message = null;
       String destination_name = (String) _dobj.get("destination_name");
       boolean _cont = true;

       this.get_col(_dobj,"ref_wa_destination","destination_id","destination_name ='" + destination_name + "'");
       if ((_dobj.get("destination_id") == null || ((String)_dobj.get("destination_id")).length() == 0) && _add.equals("ADD")) 
       {

         _cont = this.tbl_insert(
           "ref_wa_destination","destination_id,destination_name,destination_common_name,effective_start_date",
           "ref_wa_destination_seq.nextval" + comma + apos + destination_name + apos + comma + apos + destination_name + apos +
           comma + "trunc(sysdate)"  ); 
         this.get_col(_dobj,"ref_wa_destination","destination_id","destination_name ='" + destination_name + "'");
         if (_dobj.get("destination_id") == null || ((String)_dobj.get("destination_id")).length() == 0)  _cont = false; 
       }

       if (! _cont)
       {
         error_message = destination_name + " GETdestination Method FAILED" ;
         log.debug( this,"  " + error_message);
       }

       if (debug) log.debug( this,"  " + destination_name + "  :" + " PASSED");
       return _cont;

    } // end of get_destination method



    private void processAccount(DataObject _dobj)
    {
  
     boolean _cont = true;

     String allow_storage = (String)_dobj.get("allow_storage");
     if (allow_storage.equals("1") || ((String)_dobj.get("ACCTTYPE_NAME")).equalsIgnoreCase("storageaccount")) allow_storage = "Y";
     else allow_storage = "N";

     String account_name = (String)_dobj.get("account_name");
     String acct_parent =  account_name.substring(0,account_name.indexOf("^"));
     System.out.println("processing Account " + account_name +  "   Res:  " + acct_parent);
     _dobj.put("ACCT_PARENT",acct_parent);
     _cont = this.get_parent(_dobj,"ADD");
     if (_cont) _cont = this.get_water_owner(_dobj,"ADD");
     if (_cont) _cont = this.get_water_type(_dobj,"ADD");
     if (_cont) _cont = this.get_accttype(_dobj,"ADD");
     this.get_col(_dobj,"hdb_objecttype","objecttype_id","objecttype_name ='wa account'");
     if (_dobj.get("objecttype_id") == null || ((String)_dobj.get("objecttype_id")).length() == 0)  _cont = false;

//   if the this is an add then insert the record into the hdb_site table  otherwise is a modify 
//   so don't do anything to hdb_site
     if( _cont && object_insert) 
     {
       _cont = 
         //  insert account into hdb_site table
         this.tbl_insert("hdb_site","site_name,site_common_name,objecttype_id,parent_site_id,parent_objecttype_id,db_site_code",
        apos + account_name + apos + comma + apos + account_name.substring(account_name.indexOf("^")+1) + apos + comma + 
        _dobj.get("objecttype_id") + comma + 
        _dobj.get("parent_site_id") + comma + _dobj.get("parent_objecttype_id") + comma + apos +
        db_site_code + apos); 

       //  get the site_id for this new account object
       this.get_col(_dobj,"hdb_site","site_id account_site_id","site_name ='" + account_name + "'");
       if (_dobj.get("account_site_id") == null || ((String)_dobj.get("account_site_id")).length() == 0)  _cont = false;

     }  // end of if its an account insert

//  insert the account into the water accounting account table
     if( _cont) _cont = 
       this.tbl_insert("ref_wa_acct","site_id,config_id,accttype_id,watertype_id,water_owner_id,allow_storage",
       _dobj.get("account_site_id") + comma + _dobj.get("config_id") + comma + _dobj.get("accttype_id") + comma + 
       _dobj.get("watertype_id") + comma + _dobj.get("water_owner_id") + comma + apos + allow_storage + apos); 


//  if an update then  modify the last entry for this account in the ref_wa_model_site_config table
//  this dml string has to be generated dynamically for each account modified
    if (!object_insert)
    {
       String dml = " update ref_wa_model_site_config set effective_end_date_time = trunc(sysdate) " +
      " where site_id = " + _dobj.get("account_site_id") +
      " and model_id = " + _dobj.get("model_id") +
      " and config_id = " + _dobj.get("config_id") + "-1" +
      " and trunc(sysdate) >= effective_start_date_time and effective_end_date_time is null"; 

      //System.out.println ("DOING UPDATE!!!! " + dml);
      String status = db.performQuery(dml,_dobj);

    }  // end of if its an updated account check
 

//  insert a record into the model config table for both inserts and updates
     if( _cont) _cont = 
      this.tbl_insert("ref_wa_model_site_config","site_id,config_id,model_id,effective_start_date_time",
      _dobj.get("account_site_id") + comma + _dobj.get("config_id") + comma + _dobj.get("model_id") + comma + "trunc(sysdate)");

//  only do an insert into ref_wa_dmi_site_map  if its a newly inserted account
     if( _cont && object_insert) _cont = 
      this.tbl_insert("ref_wa_dmi_site_map","model_id,object_name,effective_start_date_time,objecttype_id,site_id",
      model_id + comma + apos + account_name + apos + comma + "trunc(sysdate)"+ comma + _dobj.get("objecttype_id") + comma + 
      _dobj.get("account_site_id"));

      if (_cont) log.debug( this," Account:  " + account_name + " processed correctly");
      if (! _cont)
      {
        log.debug( this," Account:  " + account_name + " ERROR in processing: " + _dobj.toString());
        errors ++;
      }

    }   //end of processAccount


    private void processSupply(DataObject _dobj)
    {
     boolean _cont = true;
     String supply_name = (String) _dobj.get("supply_name");
 
     System.out.println("processing Supply " + supply_name);
     this.get_col(_dobj,"hdb_objecttype","objecttype_id","objecttype_name ='wa supply'");
     if (_dobj.get("objecttype_id") == null || ((String)_dobj.get("objecttype_id")).length() == 0)  _cont = false;

     if (_cont) _cont = this.get_supply_role(_dobj,"ADD");
     if (_cont) _cont = this.get_supply_type(_dobj,"ADD");
     if (_cont) _cont = this.get_release_type(_dobj,"ADD");
     if (_cont) _cont = this.get_destination(_dobj,"ADD");

     this.get_col(_dobj,"hdb_site","site_id source_acct_id","site_name ='" + _dobj.get("supply_account_name") + "'");
     this.get_col(_dobj,"hdb_site","site_id downstream_acct_id","site_name ='" + _dobj.get("demand_account_name") + "'");
     //  currently assumes all supplies will have an upstream and downstream account
     if (_dobj.get("source_acct_id") == null || ((String)_dobj.get("source_acct_id")).length() == 0)  _cont = false;
     if (_dobj.get("downstream_acct_id") == null || ((String)_dobj.get("downstream_acct_id")).length() == 0)  _cont = false;

//   if the this is an add then insert the record into the hdb_site table  otherwise is a modify
//   so don't do anything to hdb_site

     if( _cont && object_insert)
     {

        this.get_col(_dobj,"hdb_site","site_id supply_site_id","site_name ='" + supply_name + "'");
        if (_dobj.get("supply_site_id") == null || ((String)_dobj.get("supply_site_id")).length() == 0) _cont = 
         this.tbl_insert("hdb_site","site_name,site_common_name,objecttype_id,db_site_code",
         apos + supply_name + apos + comma + apos + supply_name + apos + comma + _dobj.get("objecttype_id") + comma + 
         apos + db_site_code + apos); 
//         this.tbl_insert("hdb_site","site_name,site_common_name,objecttype_id,parent_site_id,parent_objecttype_id,db_site_code",
//         apos + supply_name + apos + comma + apos + supply_name + apos + comma + _dobj.get("objecttype_id") + comma + 
//         _dobj.get("parent_site_id") + comma + _dobj.get("parent_objecttype_id") + comma + apos +
//         db_site_code + apos); 

        this.get_col(_dobj,"hdb_site","site_id supply_site_id","site_name ='" + supply_name + "'");
        if (_dobj.get("supply_site_id") == null || ((String)_dobj.get("supply_site_id")).length() == 0)  _cont = false;
      }

//  now for either case insert the supply record

     if( _cont) _cont = 
      this.tbl_insert("ref_wa_supply","site_id,config_id,supplytype_id,supply_role_id,source_acct_name,source_acct_id,"
      +"downstream_acct_name,downstream_acct_id,releasetype_id,destination_id",
      _dobj.get("supply_site_id") + comma + _dobj.get("config_id") + comma + _dobj.get("supplytype_id") + comma + 
      _dobj.get("supply_role_id") + comma + apos +  _dobj.get("supply_account_name") + apos + comma + 
      _dobj.get("source_acct_id") + comma + 
      apos + _dobj.get("demand_account_name") + apos + comma + _dobj.get("downstream_acct_id") + comma + 
      _dobj.get("releasetype_id") + comma + _dobj.get("destination_id")); 

//  if an update then  modify the last entry for this supply in the ref_wa_model_site_config table
//  this dml string has to be generated dynamically for each supply modified
    if (!object_insert)
    {
       String dml = " update ref_wa_model_site_config set effective_end_date_time = trunc(sysdate) " +
      " where site_id = " + _dobj.get("supply_site_id") +
      " and model_id = " + _dobj.get("model_id") +
      " and config_id = " + _dobj.get("config_id") + "-1" +
      " and trunc(sysdate) >= effective_start_date_time and effective_end_date_time is null";

      String status = db.performQuery(dml,_dobj);

    }  // end of if its an updated supply check


//  insert a record into the model config table for both inserts and updates
     if( _cont) _cont = 
      this.tbl_insert("ref_wa_model_site_config","site_id,config_id,model_id,effective_start_date_time",
      _dobj.get("supply_site_id") + comma + _dobj.get("config_id") + comma + _dobj.get("model_id") + comma + 
      "trunc(sysdate)");

//  only do an insert into ref_wa_dmi_site_map  if its a newly inserted supply
     if( _cont && object_insert) _cont = 
      this.tbl_insert("ref_wa_dmi_site_map","model_id,object_name,effective_start_date_time,objecttype_id,site_id",
      _dobj.get("model_id") + comma + apos + supply_name + apos + comma + "trunc(sysdate)"+ comma + 
      _dobj.get("objecttype_id") + comma + 
      _dobj.get("supply_site_id"));

     if (_cont) log.debug( this," Supply:  " + supply_name + " processed correctly");
     if (! _cont)
     {
       log.debug( this," Supply:  " + supply_name + " ERROR in processing: " + _dobj.toString());
       errors ++;
     }

    }   //end of processSupply 


    private void processExchange(DataObject _dobj)
    {
  
     boolean _cont = true;
     String exchange_name = (String) _dobj.get("exchange_name");
     System.out.println("processing Exchange " + exchange_name);

     this.get_col(_dobj,"hdb_objecttype","objecttype_id","objecttype_name ='wa exchange'");
     if (_dobj.get("objecttype_id") == null || ((String)_dobj.get("objecttype_id")).length() == 0)  _cont = false;

//   if the this is an add then insert the record into the hdb_site table  otherwise is a modify
//   so don't do anything to hdb_site
     if( _cont && object_insert)
     {  _cont = 
        this.tbl_insert("hdb_site","site_name,site_common_name,objecttype_id,parent_site_id,parent_objecttype_id,db_site_code",
        apos + exchange_name + apos + comma + apos + exchange_name + apos + comma + _dobj.get("objecttype_id") + comma + 
        _dobj.get("parent_site_id") + comma + _dobj.get("parent_objecttype_id") + comma + apos +
        db_site_code + apos); 

       this.get_col(_dobj,"hdb_site","site_id exchange_site_id","site_name ='" + exchange_name + "'");
       if (_dobj.get("exchange_site_id") == null || ((String)_dobj.get("exchange_site_id")).length() == 0)  _cont = false;

     }  //  end of the hdb_site insert block

     //  now go get the fks for the exchanges supply ids..
     this.get_col(_dobj,"hdb_site","site_id borrow_supply_id","site_name ='" + _dobj.get("borrow_name") + "'");
     if ((_dobj.get("borrow_supply_id") == null || ((String)_dobj.get("borrow_supply_id")).length() == 0) & !((String)_dobj.get("borrow_name")).equals("NONE"))  _cont = false;
     
     this.get_col(_dobj,"hdb_site","site_id payback_destination_supply_id","site_name ='" + _dobj.get("destination_name") + "'");
     if ((_dobj.get("payback_destination_supply_id") == null || ((String)_dobj.get("payback_destination_supply_id")).length() == 0) & !((String)_dobj.get("destination_name")).equals("NONE"))  _cont = false;

     //  insert a exchange record whether a new one or a modified
     if( _cont)
     {
        String borrow_id =  (String) _dobj.get("borrow_supply_id");
        if (borrow_id.length() == 0)  borrow_id = null; 
        String dest_id =  (String) _dobj.get("payback_destination_supply_id");
        if (dest_id.length() == 0)  dest_id = null; 
        _cont = this.tbl_insert("ref_wa_exchange","site_id,config_id,borrow_supply_id,payback_destination_supply_id",
      _dobj.get("exchange_site_id") + comma + _dobj.get("config_id") + comma + borrow_id + comma + dest_id); 
     }

    //  if an update then  modify the last entry for this exchange in the ref_wa_model_site_config table
    //  this dml string has to be generated dynamically for each exchange modified
    if (!object_insert)
    {
       String dml = " update ref_wa_model_site_config set effective_end_date_time = trunc(sysdate) " +
      " where site_id = " + _dobj.get("exchange_site_id") +
      " and model_id = " + _dobj.get("model_id") +
      " and config_id = " + _dobj.get("config_id") + "-1" +
      " and trunc(sysdate) >= effective_start_date_time and effective_end_date_time is null";

      String status = db.performQuery(dml,_dobj);

    }  // end of if its an updated supply check

     //  insert a model config record whether a new one or a modified
     if( _cont) _cont = 
      this.tbl_insert("ref_wa_model_site_config","site_id,config_id,model_id,effective_start_date_time",
      _dobj.get("exchange_site_id") + comma +_dobj.get("config_id") + comma + _dobj.get("model_id") + comma + "trunc(sysdate)");

     //  only do an insert into the dmi table if its an insert
     if( _cont && object_insert) _cont = 
      this.tbl_insert("ref_wa_dmi_site_map","model_id,object_name,effective_start_date_time,objecttype_id,site_id",
      model_id + comma + apos + exchange_name + apos + comma + "trunc(sysdate)"+ comma + _dobj.get("objecttype_id") + comma + 
      _dobj.get("exchange_site_id"));

     if (_cont) log.debug( this," Exchange:  " + exchange_name + " processed correctly");
     if (! _cont)
     { log.debug( this," Exchange:  " + exchange_name + " ERROR in processing: " + _dobj.toString());
       errors ++;
     }

    }   //end of processExchange


    private void processPayback(DataObject _dobj)
    {
  
     boolean _cont = true;
     String payback_name =  (String) _dobj.get("Payback_name");
     System.out.println("processing Payback " + payback_name);
     String exchange_name =  payback_name.substring(0,payback_name.indexOf("."));
     String supply_name =  payback_name.substring(payback_name.indexOf(".")+1);


     this.get_col(_dobj,"hdb_objecttype","objecttype_id","objecttype_name ='wa exchange payback'");
     if (_dobj.get("objecttype_id") == null || ((String)_dobj.get("objecttype_id")).length() == 0)  _cont = false;

     if( _cont && object_insert) 
     {
       _cont = 
          this.tbl_insert("hdb_site","site_name,site_common_name,objecttype_id,parent_site_id,parent_objecttype_id,db_site_code",
          apos + payback_name + apos + comma + apos + payback_name + apos + comma + _dobj.get("objecttype_id") + comma + 
          _dobj.get("parent_site_id") + comma + _dobj.get("parent_objecttype_id") + comma + apos +
          db_site_code + apos); 

         this.get_col(_dobj,"hdb_site","site_id payback_site_id","site_name ='" + payback_name + "'");
         if (_dobj.get("payback_site_id") == null || ((String)_dobj.get("payback_site_id")).length() == 0)  _cont = false;
     }


     // now go get the exchange and the supply for this payback     
         this.get_col(_dobj,"hdb_site","site_id exchange_site_id","site_name ='" + exchange_name + "'");
         if (_dobj.get("exchange_site_id") == null || ((String)_dobj.get("exchange_site_id")).length() == 0)  _cont = false;

         this.get_col(_dobj,"hdb_site","site_id payback_supply_id","site_name ='" + supply_name + "'");
         if (_dobj.get("payback_supply_id") == null || ((String)_dobj.get("payback_supply_id")).length() == 0)  _cont = false;

     //  insert a record into the payback table regardless of whether an insert or mod
     if( _cont) _cont = 
      this.tbl_insert("ref_wa_exchange_payback","site_id,config_id,exchange_site_id,payback_supply_id,payback_legal_loss_af",
      _dobj.get("payback_site_id") + comma + _dobj.get("config_id") + comma + _dobj.get("exchange_site_id") + comma + 
      _dobj.get("payback_supply_id") + comma + 
      _dobj.get("payback_legal_loss_af"));


    //  if an update then  modify the last entry for this payback in the ref_wa_model_site_config table
    //  this dml string has to be generated dynamically for each payback modified
    if (!object_insert)
    {
       String dml = " update ref_wa_model_site_config set effective_end_date_time = trunc(sysdate) " +
      " where site_id = " + _dobj.get("payback_site_id") +
      " and model_id = " + _dobj.get("model_id") +
      " and config_id = " + _dobj.get("config_id") + "-1" +
      " and trunc(sysdate) >= effective_start_date_time and effective_end_date_time is null";

      String status = db.performQuery(dml,_dobj);

    }  // end of if its an updated supply check


     //  insert a record into the model config  table regardless of whether an insert or mod
     if( _cont) _cont = 
      this.tbl_insert("ref_wa_model_site_config","site_id,config_id,model_id,effective_start_date_time",
      _dobj.get("payback_site_id") + comma + _dobj.get("config_id") + comma + _dobj.get("model_id") + 
      comma + "trunc(sysdate)");

     //  only do insert into dmi mapping table if its a nbew record
     if( _cont && object_insert) _cont = 
      this.tbl_insert("ref_wa_dmi_site_map","model_id,object_name,effective_start_date_time,objecttype_id,site_id",
      model_id + comma + apos + payback_name + apos + comma + "trunc(sysdate)"+ comma + _dobj.get("objecttype_id") + comma + 
      _dobj.get("payback_site_id"));


     if (_cont) log.debug( this," Payback:  " + payback_name + " processed correctly");
     if (! _cont) 
     { 
       log.debug( this," Payback:  " + payback_name + " ERROR in processing: "+ _dobj.toString());
       errors ++;
     }

    }   //end of processPayback 

 
   private void setup_queries()
   {

/// here are all the query strings necessary to perform the needed queries

  //  here are the queries for the accounts
  //  deleted_accounts will logically delete any account that is currently active in ref_wa_model_site_config
  //  that is not currently in the new water accounting metadata file (external table)
  deleted_accts = " update ref_wa_model_site_config set effective_end_date_time = trunc(sysdate) " +
    " where (site_id,config_id,model_id) in (select s.site_id,sc.config_id,sc.model_id" +
    " from hdb_site s, ref_wa_acct rwa, ref_wa_model_site_config sc, ref_wa_metadata_input rmi" +
    " where s.site_id = rwa.site_id and s.site_id = sc.site_id and rwa.config_id = sc.config_id" +
    " and rmi.column_1(+) = 'Account' and s.site_name = rmi.column_4(+)" +
    " and sc.model_id = " + pdobj.get("model_id") +
    " and trunc(sysdate) between sc.effective_start_date_time and nvl(sc.effective_end_date_time,sysdate)" +
    " and rmi.column_4 is null)";

  //  gets a count of the records logically deleted from the previously executed sql
  del_accts_count = "select count(*) acct_del_counts from  ref_wa_acct rwa,ref_wa_model_site_config sc " +
  "where rwa.site_id = sc.site_id and sc.effective_end_date_time = trunc(sysdate)" +
  " and rwa.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id");

  //  gets the new metadata that doesn't exist currently in the database
  new_accts = "select column_1 object_type,column_2 acctobjtype_name,column_3 accttype_name,column_4 account_name,column_5 watertype_name, " +
    "column_6 water_owner_name, column_7 allow_storage , " + pdobj.get("config_id") + " config_id" +
    "," + pdobj.get("model_id") + " model_id" +
    " from ref_wa_metadata_input where column_1 = 'Account' and " +
    "(column_4) not in (select s.site_name" +
    " from hdb_site s, ref_wa_acct rwa, ref_wa_model_site_config sc " +
    " where s.site_id = rwa.site_id and s.site_id = sc.site_id and " +
    " rwa.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null)";

   //   gets the metadata records that have been modified when compared to what's in the database
   mod_accts = " select column_1 object_type,column_2 acctobjtype_name,column_3 accttype_name,column_4 account_name,column_5 watertype_name, " +
    " column_6 water_owner_name, column_7 allow_storage , sc.config_id + 1 config_id " +
    "," + pdobj.get("model_id") + " model_id, sc.site_id account_site_id" +
    " from ref_wa_metadata_input, ref_wa_model_site_config sc" +
    " where column_1 = 'Account' and sc.effective_end_date_time is null and (column_4,sc.site_id) in" +
    " (select s.site_name,s.site_id from hdb_site s, ref_wa_acct rwa, ref_wa_model_site_config sc" +
    " where s.site_id = rwa.site_id and s.site_id = sc.site_id" +
    " and rwa.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null) and" +
    " (column_4,column_3,column_5,column_6,decode(column_7,'1','1',decode(column_3,'StorageAccount','1','0'))) not in" +
    " (select s.site_name,a.accttype_name,wt.watertype_name,wo.water_owner_name,decode(rwa.allow_storage,'Y',1,0)" +
    " from hdb_site s, ref_wa_acct rwa, ref_wa_water_owner wo, ref_wa_watertype wt, ref_wa_accttype a, ref_wa_model_site_config sc" +
    " where s.site_id = rwa.site_id and s.site_id = sc.site_id" +
    " and rwa.accttype_id = a.accttype_id and rwa.watertype_id = wt.watertype_id" +
    " and rwa.water_owner_id = wo.water_owner_id and rwa.config_id = sc.config_id" +
    " and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null)";

    //  now the queries for the supplies....

  //  deleted_supplies will logically delete any supplies that is currently active in ref_wa_model_site_config
  //  that is not currently in the new water accounting metadata file (external table)
  deleted_supplies = " update ref_wa_model_site_config set effective_end_date_time = trunc(sysdate) " +
    " where (site_id,config_id,model_id) in (select s.site_id,sc.config_id,sc.model_id" +
    " from hdb_site s, ref_wa_supply rws, ref_wa_model_site_config sc, ref_wa_metadata_input rmi" +
    " where s.site_id = rws.site_id and s.site_id = sc.site_id and rws.config_id = sc.config_id" +
    " and rmi.column_1(+) = 'Supply' and s.site_name = rmi.column_2(+)" +
    " and sc.model_id = " + pdobj.get("model_id") +
    " and trunc(sysdate) between sc.effective_start_date_time and nvl(sc.effective_end_date_time,sysdate)" +
    " and rmi.column_2 is null)";

  //  gets a count of the records logically deleted from the previously executed sql
  del_supplies_count = "select count(*) supply_del_counts from  ref_wa_supply rws,ref_wa_model_site_config sc " +
  "where rws.site_id = sc.site_id and sc.effective_end_date_time = trunc(sysdate)" +
  " and rws.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id");

  //  gets the new metadata that doesn't exist currently in the database


  new_supplies = "select column_1 object_type,column_2 supply_name,column_3 supply_account_name,column_4 demand_account_name, " +
    " column_5 supply_role_name,column_6 releasetype_name, column_7 destination_name, column_8 supplytype_name, " +
    pdobj.get("config_id") + " config_id," + pdobj.get("model_id") + " model_id" +
    " from ref_wa_metadata_input where column_1 = 'Supply' and " +
    "(column_2) not in (select s.site_name" +
    " from hdb_site s, ref_wa_supply rws, ref_wa_model_site_config sc " +
    " where s.site_id = rws.site_id and s.site_id = sc.site_id and " +
    " rws.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null)";


   //   gets the metadata records that have been modified when compared to what's in the database
   mod_supplies = " select column_1 object_type,column_2 supply_name,column_3 supply_account_name,column_4 demand_account_name, "
+
    " column_5 supply_role_name,column_6 releasetype_name, column_7 destination_name, column_8 supplytype_name, " +
    " sc.config_id + 1 config_id " +
    "," + pdobj.get("model_id") + " model_id, sc.site_id supply_site_id" +
    " from ref_wa_metadata_input, ref_wa_model_site_config sc" +
    " where column_1 = 'Supply' and sc.effective_end_date_time is null and (column_2,sc.site_id) in" +
    " (select s.site_name,s.site_id from hdb_site s, ref_wa_supply rws, ref_wa_model_site_config sc" +
    " where s.site_id = rws.site_id and s.site_id = sc.site_id" +
    " and rws.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null) and" +
    " (column_2,column_3,column_4,column_5,column_6,column_7,column_8) not in" +
    " (select s.site_name, upa.site_name, dem.site_name, sr.supply_role_name, rel.releasetype_name, " +
    " des.destination_name, st.supplytype_name " +
    " from hdb_site s, hdb_site upa, ref_wa_releasetype rel, hdb_site dem," +
    " ref_wa_supply rws, ref_wa_supplytype st, ref_wa_destination des, ref_wa_supply_role sr, ref_wa_model_site_config sc" +
    " where rws.site_id = s.site_id and rws.site_id = sc.site_id and rws.supplytype_id = st.supplytype_id" +
    " and rws.supply_role_id = sr.supply_role_id and rws.source_acct_id = upa.site_id and rws.downstream_acct_id = dem.site_id" +
    " and rws.releasetype_id = rel.releasetype_id and rws.destination_id = des.destination_id" +
    " and rws.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null)";

  deleted_exchanges = " update ref_wa_model_site_config set effective_end_date_time = trunc(sysdate) " +
    " where (site_id,config_id,model_id) in (select s.site_id,sc.config_id,sc.model_id" +
    " from hdb_site s, ref_wa_exchange rwe, ref_wa_model_site_config sc, ref_wa_metadata_input rmi" +
    " where s.site_id = rwe.site_id and s.site_id = sc.site_id and rwe.config_id = sc.config_id" +
    " and rmi.column_1(+) = 'Exchange' and s.site_name = rmi.column_2(+)" +
    " and sc.model_id = " + pdobj.get("model_id") +
    " and trunc(sysdate) between sc.effective_start_date_time and nvl(sc.effective_end_date_time,sysdate)" +
    " and rmi.column_2 is null)";

  //  gets a count of the records logically deleted from the previously executed sql
  del_exchanges_count = "select count(*) exchanges_del_counts from  ref_wa_exchange rwe,ref_wa_model_site_config sc " +
  "where rwe.site_id = sc.site_id and sc.effective_end_date_time = trunc(sysdate)" +
  " and rwe.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id");


   // gets all the records from the new metadata file that have exchange records that have been modified
   mod_exchanges = " select column_1 object_type,column_2 exchange_name,column_3 borrow_name,column_4 destination_name, " +
    " column_5 payback_name, " +
    " sc.config_id + 1 config_id " +
    "," + pdobj.get("model_id") + " model_id, sc.site_id exchange_site_id" +
    " from ref_wa_metadata_input, ref_wa_model_site_config sc" +
    " where column_1 = 'Exchange' and sc.effective_end_date_time is null and (column_2,sc.site_id) in" +
    " (select s.site_name,s.site_id from hdb_site s, ref_wa_exchange rwe, ref_wa_model_site_config sc" +
    " where s.site_id = rwe.site_id and rwe.site_id = sc.site_id" +
    " and rwe.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null) and" +
    " (column_2,column_3,column_4) not in" +
    " (select get_hdb_site_name(rwe.site_id), nvl(get_hdb_site_name(rwe.borrow_supply_id),'NONE')," +
    " nvl(get_hdb_site_name(rwe.payback_destination_supply_id),'NONE')" +
    " from ref_wa_exchange rwe, ref_wa_model_site_config sc" +
    " where rwe.site_id = sc.site_id" +
    " and rwe.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null)";

   new_exchanges = " select column_1 object_type,column_2 exchange_name,column_3 borrow_name,column_4 destination_name, " +
    " column_5 payback_name, " +
    pdobj.get("config_id") + " config_id," + pdobj.get("model_id") + " model_id" +
    " from ref_wa_metadata_input where column_1 = 'Exchange' and " +
    "(column_2) not in (select s.site_name" +
    " from hdb_site s, ref_wa_exchange rwe, ref_wa_model_site_config sc " +
    " where s.site_id = rwe.site_id and s.site_id = sc.site_id and " +
    " rwe.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null)";


  deleted_paybacks = " update ref_wa_model_site_config set effective_end_date_time = trunc(sysdate) " +
    " where (site_id,config_id,model_id) in (select s.site_id,sc.config_id,sc.model_id" +
    " from hdb_site s, ref_wa_exchange_payback rwp, ref_wa_model_site_config sc, ref_wa_metadata_input rmi" +
    " where s.site_id = rwp.site_id and s.site_id = sc.site_id and rwp.config_id = sc.config_id" +
    " and rmi.column_1(+) = 'Payback' and s.site_name = rmi.column_2(+)" +
    " and sc.model_id = " + pdobj.get("model_id") +
    " and trunc(sysdate) between sc.effective_start_date_time and nvl(sc.effective_end_date_time,sysdate)" +
    " and rmi.column_2 is null)";

  //  gets a count of the records logically deleted from the previously executed sql
  del_paybacks_count = "select count(*) paybacks_del_counts " +
    " from ref_wa_exchange_payback rwp,ref_wa_model_site_config sc " +
    " where rwp.site_id = sc.site_id and sc.effective_end_date_time= trunc(sysdate) " +
    " and rwp.config_id = sc.config_id and sc.model_id = "+ pdobj.get("model_id");

   // gets all the records from the new metadata file that have exchange_payback records that have been modified
   mod_paybacks = " select column_1 object_type,column_2 payback_name,column_3 payback_legal_loss_af," +
    " sc.config_id + 1 config_id " +
    "," + pdobj.get("model_id") + " model_id, sc.site_id payback_site_id" +
    " from ref_wa_metadata_input, ref_wa_model_site_config sc" +
    " where column_1 = 'Payback' and sc.effective_end_date_time is null and (column_2,sc.site_id) in" +
    " (select s.site_name,s.site_id from hdb_site s, ref_wa_exchange_payback rwp, ref_wa_model_site_config sc" +
    " where s.site_id = rwp.site_id and rwp.site_id = sc.site_id" +
    " and rwp.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null) and" +
    " (column_2,to_number(column_3)) not in" +
    " (select get_hdb_site_name(rwp.site_id), rwp.payback_legal_loss_af" +
    " from ref_wa_exchange_payback rwp, ref_wa_model_site_config sc" +
    " where rwp.site_id = sc.site_id" +
    " and rwp.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null)";

   //  gets all the new exchange paybacks in the metadata file that are not in the database
   new_paybacks = " select column_1 object_type,column_2 payback_name,column_3 payback_legal_loss_af," +
    pdobj.get("config_id") + " config_id," + pdobj.get("model_id") + " model_id" +
    " from ref_wa_metadata_input where column_1 = 'Payback' and " +
    "(column_2) not in (select s.site_name" +
    " from hdb_site s, ref_wa_exchange_payback rwp, ref_wa_model_site_config sc " +
    " where s.site_id = rwp.site_id and s.site_id = sc.site_id and " +
    " rwp.config_id = sc.config_id and sc.model_id = " + pdobj.get("model_id") +
    " and sc.effective_end_date_time is null)";


 }  // end of method setup_queries
 

}  //   end of class WAMetadata
