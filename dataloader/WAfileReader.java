package dataloader;

import java.io.*;
import java.lang.Integer;
import java.util.*;
import dbutils.*;
import java.sql.*;

public class WAfileReader {

  private Hashtable hash = null;
  private String file_name = null;
  private Logger log = null;
  private int accounts = 0;
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

 

  public WAfileReader( String _file_name)
  {
    file_name = _file_name; 
    log = Logger.getInstance();
    db = new DBAccess(cnn);
    DataObject dobj = new DataObject();

    try 
    {

      dobj.addPropertyFile(System.getProperty("start.property"));
      if (!(dobj.get("model_id") == null || ((String)dobj.get("model_id")).length() == 0)) model_id = Integer.parseInt((String)dobj.get("model_id"));
      if (!(dobj.get("config_id") == null || ((String)dobj.get("config_id")).length() == 0)) config_id = Integer.parseInt((String)dobj.get("config_id"));
      cnn = db.getConnection(dobj);
    }
    catch (Exception e) {System.out.println(e.toString());}

  }


public void process()

 {
   BufferedReader input = null;

   try 
   {
     input = new BufferedReader(new FileReader(file_name));
   } 
   catch (FileNotFoundException e) {
     log.debug(this,e.getMessage()); 
     System.exit(-1);
   }


   boolean process;

   try 
   {

   // read the file line by line
   String inputLine = null;	
   int i = 0;
   int reads = 0; 
   int skips = 0; 
   DataObject dobj = null;


      inputLine = input.readLine();
      while (inputLine != null)  // read the file until and End of File EOF is returned
      {
          process = true;

         //System.out.println("RECORD: " + reads);

          if (inputLine != null)
          {
             reads ++; 
             //System.out.println("RECORD: " + reads + " " + inputLine);
 
             if (!inputLine.substring(0,1).equals("+"))
             {
               dobj = new DataObject();
               parse(dobj,inputLine);
             //System.out.println("RECORD: " + reads + " " + inputLine);
               String object_type = new String(((String)dobj.get("object_type")).toLowerCase());
               if (object_type.equals("account")) processAccount(dobj);
               if (object_type.equals("supply")) processSupply(dobj);
               if (object_type.equals("exchange")) processExchange(dobj);
               if (object_type.equals("payback")) processPayback(dobj);
             }
             else 
             {
               skips ++;
             }
          }

          inputLine = input.readLine();

//       if (reads ==2)  { System.out.println(dobj.toString()); inputLine = null; }

      }  /// end of big loop to read the whole file	

        input.close();
        log.debug(this,"RECORDS Processed: " + reads);
        log.debug(this, "SKIPS: " + skips + " ACCT: " + accounts  + " EX: " + exchanges + " Supp: " + supplies + " Paybacks: " + paybacks + "  Errors:  " + errors );

     } // end of big try
     catch (IOException e) 
     {
       log.debug(this,e.getMessage()); 
       System.exit(-1);
     }

    }  // end of process method

    private void parse(DataObject dobj, String input)
    {


/* the lines coming from the data source are the following format:
+Account,ObjType,Type,Name,WaterType,WaterOwner,Storage
+Supply,Name,SupplyAccount,DemandAccount,Role,ReleaseType,DestinationType,SupplyType
+Exchange,Name,BorrowSupply,DestinationSupply,Paybacks
+Payback,Name,LossCoeff
*/

         String field = null;
         int tcount = 0;
         int line_type = 0;

         dobj.put("INPUTLINE",input);
         StringTokenizer st = new StringTokenizer(input,",");
         while (st.hasMoreTokens()) {
            field = st.nextToken();
            //System.out.println("type=  " + line_type + "  token= " + tcount); 
            switch (tcount) 
            {

             case 0:   // first column and it will be object type

                 dobj.put("OBJECT_TYPE",field);
                 if (field.indexOf("Account") == 0)
                 {
                   accounts ++;
                   line_type = 0;
                 }
                 else if (field.indexOf("Supply") == 0)
                 {
                   supplies ++;
                   line_type = 20;
                 }
                 else if (field.indexOf("Exchange") == 0)
                 {
                   exchanges ++;
                   line_type = 40;
                 }
                 else if (field.indexOf("Payback") == 0)
                 {
                   paybacks ++;
                   line_type = 60;
                 }
                 break;
    
              case 1:  /// second column of the line

                switch ( line_type + tcount )
                {
                  case 1:   dobj.put("ACCTOBJTYPE_NAME",field);break;	
                  case 21:  dobj.put("SUPPLY_NAME",field);break;
                  case 41:  dobj.put("EXCHANGE_NAME",field);break;
                  case 61:  dobj.put("PAYBACK_NAME",field);break;
 
                }  // end of switch

    
              case 2:   // third column of the line

                switch ( line_type + tcount )
                {
                  case 2:   dobj.put("ACCTTYPE_NAME",field);break;
                  case 22:  dobj.put("SUPPLY_ACCOUNT_NAME",field);break;
                  case 42:  dobj.put("BORROW_NAME",field);break;
                  case 62:  dobj.put("PAYBACK_LEGAL_LOSS_AF",field);break;
 
                }  // end of switch
                break;

    
              case 3:  // fourth column of the line

                switch ( line_type + tcount )
                {
                  case 3:   dobj.put("ACCOUNT_NAME",field);break;
                  case 23:  dobj.put("DEMAND_ACCOUNT_NAME",field);break;
                  case 43:  dobj.put("DESTINATION_NAME",field);break;
                  //case 63:  no field presently 
 
                }  // end of switch
                break;

    
              case 4:  // fifth column of the line

                switch ( line_type + tcount )
                {
                  case 4:   dobj.put("WATERTYPE_NAME",field);break;
                  case 24:  dobj.put("SUPPLY_ROLE_NAME",field);break;
                  case 44:  dobj.put("PAYBACK_NAME",field);break;
                  //case 64:  no field presently 
 
                }  // end of switch
                break;

    
              case 5:  // sixth column of the line

                switch ( line_type + tcount )
                {
                  case 5:   dobj.put("WATER_OWNER_NAME",field);break;
                  case 25:  dobj.put("RELEASETYPE_NAME",field);break;
                  //case 45:  no field presently 
                  //case 65:  no field presently 
 
                }  // end of switch
                break;

    
              case 6:  // seventh column of the line

                switch ( line_type + tcount )
                {
                  case 6: dobj.put("ALLOW_STORAGE",field);break; 
                  case 26:  dobj.put("DESTINATION_NAME",field);break;
                  //case 46:  no field presently 
                  //case 66:  no field presently 
 
                }  // end of switch
                break;

    
              case 7:  // eighth column of the line

                switch ( line_type + tcount )
                {
                  //case 7:  no field presently 
                  case 27:  dobj.put("SUPPLYTYPE_NAME",field);break;
                  //case 47:  no field presently 
                  //case 67:  no field presently 
 
                }  // end of switch
                break;

            }  /// end of switch


     
            tcount ++;
         }  // end of token while


    } // end of method parse


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

     if( _cont) _cont = 
      this.tbl_insert("hdb_site","site_name,site_common_name,objecttype_id,parent_site_id,parent_objecttype_id,db_site_code",
      apos + account_name + apos + comma + apos + account_name.substring(account_name.indexOf("^")+1) + apos + comma + 
      _dobj.get("objecttype_id") + comma + 
      _dobj.get("parent_site_id") + comma + _dobj.get("parent_objecttype_id") + comma + apos +
      db_site_code + apos); 

     this.get_col(_dobj,"hdb_site","site_id account_site_id","site_name ='" + account_name + "'");
     if (_dobj.get("account_site_id") == null || ((String)_dobj.get("account_site_id")).length() == 0)  _cont = false;

     if( _cont) _cont = 
      this.tbl_insert("ref_wa_acct","site_id,config_id,accttype_id,watertype_id,water_owner_id,allow_storage",
      _dobj.get("account_site_id") + comma + config_id + comma + _dobj.get("accttype_id") + comma + 
      _dobj.get("watertype_id") + comma + _dobj.get("water_owner_id") + comma + apos + allow_storage + apos); 

     if( _cont) _cont = 
      this.tbl_insert("ref_wa_model_site_config","site_id,config_id,model_id,effective_start_date_time",
      _dobj.get("account_site_id") + comma + config_id + comma + model_id + comma + "trunc(sysdate)");

     if( _cont) _cont = 
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


     if( _cont) _cont = 
      this.tbl_insert("hdb_site","site_name,site_common_name,objecttype_id,parent_site_id,parent_objecttype_id,db_site_code",
      apos + supply_name + apos + comma + apos + supply_name + apos + comma + _dobj.get("objecttype_id") + comma + 
      _dobj.get("parent_site_id") + comma + _dobj.get("parent_objecttype_id") + comma + apos +
      db_site_code + apos); 

     this.get_col(_dobj,"hdb_site","site_id supply_site_id","site_name ='" + supply_name + "'");
     if (_dobj.get("supply_site_id") == null || ((String)_dobj.get("supply_site_id")).length() == 0)  _cont = false;

     if( _cont) _cont = 
      this.tbl_insert("ref_wa_supply","site_id,config_id,supplytype_id,supply_role_id,source_acct_name,source_acct_id,"
      +"downstream_acct_name,downstream_acct_id,releasetype_id,destination_id",
      _dobj.get("supply_site_id") + comma + config_id + comma + _dobj.get("supplytype_id") + comma + 
      _dobj.get("supply_role_id") + comma + apos +  _dobj.get("supply_account_name") + apos + comma + 
      _dobj.get("source_acct_id") + comma + 
      apos + _dobj.get("demand_account_name") + apos + comma + _dobj.get("downstream_acct_id") + comma + 
      _dobj.get("releasetype_id") + comma + _dobj.get("destination_id")); 

     if( _cont) _cont = 
      this.tbl_insert("ref_wa_model_site_config","site_id,config_id,model_id,effective_start_date_time",
      _dobj.get("supply_site_id") + comma + config_id + comma + model_id + comma + "trunc(sysdate)");

     if( _cont) _cont = 
      this.tbl_insert("ref_wa_dmi_site_map","model_id,object_name,effective_start_date_time,objecttype_id,site_id",
      model_id + comma + apos + supply_name + apos + comma + "trunc(sysdate)"+ comma + _dobj.get("objecttype_id") + comma + 
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

     if( _cont) _cont = 
      this.tbl_insert("hdb_site","site_name,site_common_name,objecttype_id,parent_site_id,parent_objecttype_id,db_site_code",
      apos + exchange_name + apos + comma + apos + exchange_name + apos + comma + _dobj.get("objecttype_id") + comma + 
      _dobj.get("parent_site_id") + comma + _dobj.get("parent_objecttype_id") + comma + apos +
      db_site_code + apos); 

     this.get_col(_dobj,"hdb_site","site_id exchange_site_id","site_name ='" + exchange_name + "'");
     if (_dobj.get("exchange_site_id") == null || ((String)_dobj.get("exchange_site_id")).length() == 0)  _cont = false;

     this.get_col(_dobj,"hdb_site","site_id borrow_supply_id","site_name ='" + _dobj.get("borrow_name") + "'");
     if ((_dobj.get("borrow_supply_id") == null || ((String)_dobj.get("borrow_supply_id")).length() == 0) & !((String)_dobj.get("borrow_name")).equals("NONE"))  _cont = false;
     
     this.get_col(_dobj,"hdb_site","site_id payback_destination_supply_id","site_name ='" + _dobj.get("destination_name") + "'");
     if ((_dobj.get("payback_destination_supply_id") == null || ((String)_dobj.get("payback_destination_supply_id")).length() == 0) & !((String)_dobj.get("destination_name")).equals("NONE"))  _cont = false;

     if( _cont)
     {
        String borrow_id =  (String) _dobj.get("borrow_supply_id");
        if (borrow_id.length() == 0)  borrow_id = null; 
        String dest_id =  (String) _dobj.get("payback_destination_supply_id");
        if (dest_id.length() == 0)  dest_id = null; 
        _cont = this.tbl_insert("ref_wa_exchange","site_id,config_id,borrow_supply_id,payback_destination_supply_id",
      _dobj.get("exchange_site_id") + comma + config_id + comma + borrow_id + comma + dest_id); 
     }

     if( _cont) _cont = 
      this.tbl_insert("ref_wa_model_site_config","site_id,config_id,model_id,effective_start_date_time",
      _dobj.get("exchange_site_id") + comma + config_id + comma + model_id + comma + "trunc(sysdate)");

     if( _cont) _cont = 
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

     if( _cont) _cont = 
      this.tbl_insert("hdb_site","site_name,site_common_name,objecttype_id,parent_site_id,parent_objecttype_id,db_site_code",
      apos + payback_name + apos + comma + apos + payback_name + apos + comma + _dobj.get("objecttype_id") + comma + 
      _dobj.get("parent_site_id") + comma + _dobj.get("parent_objecttype_id") + comma + apos +
      db_site_code + apos); 

     this.get_col(_dobj,"hdb_site","site_id payback_site_id","site_name ='" + payback_name + "'");
     if (_dobj.get("payback_site_id") == null || ((String)_dobj.get("payback_site_id")).length() == 0)  _cont = false;

     this.get_col(_dobj,"hdb_site","site_id exchange_site_id","site_name ='" + exchange_name + "'");
     if (_dobj.get("exchange_site_id") == null || ((String)_dobj.get("exchange_site_id")).length() == 0)  _cont = false;

     this.get_col(_dobj,"hdb_site","site_id payback_supply_id","site_name ='" + supply_name + "'");
     if (_dobj.get("payback_supply_id") == null || ((String)_dobj.get("payback_supply_id")).length() == 0)  _cont = false;

     if( _cont) _cont = 
      this.tbl_insert("ref_wa_exchange_payback","site_id,config_id,exchange_site_id,payback_supply_id,payback_legal_loss_af",
      _dobj.get("payback_site_id") + comma + config_id + comma + _dobj.get("exchange_site_id") + comma + 
      _dobj.get("payback_supply_id") + comma + 
      _dobj.get("payback_legal_loss_af"));


     if( _cont) _cont = 
      this.tbl_insert("ref_wa_model_site_config","site_id,config_id,model_id,effective_start_date_time",
      _dobj.get("payback_site_id") + comma + config_id + comma + model_id + comma + "trunc(sysdate)");

     if( _cont) _cont = 
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


}
