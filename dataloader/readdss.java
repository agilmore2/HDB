package dataloader;

import java.util.Vector;
import java.util.Hashtable;
import java.util.Enumeration;
import hec.heclib.dss.*;
import hec.heclib.util.doubleArrayContainer;
import hec.heclib.util.intArrayContainer;
import hec.heclib.util.HecTime;
import dbutils.DataObject;


public class readdss extends hec.heclib.dss.HecDSSFileAccess
{


    static public void main(String argv[])
    {

      readdss obj = new readdss();

      String DssFile = argv[0];
      String pathName = null;
      Vector pathnameList = new Vector();
      HecTimeSeries hts = null;
      DSSPathname dsp = null;
      doubleArrayContainer datarows = new doubleArrayContainer();
      intArrayContainer datatimes = new intArrayContainer();
      double dvalue = -9999.0;
     
      DataObject datatypes = new DataObject();
      //datatypes.addPropertyFile(System.getProperty("start.property"));
      //set_datatypes(datatypes);

      try {

        datatypes.addPropertyFile(System.getProperty("start.property"));
        hts = new HecTimeSeries();
        hts.setDSSFileName(DssFile);

        obj.setDSSFileName(DssFile);
        obj.getPathnameList("",pathnameList);

        Hashtable sites = new Hashtable();
        Hashtable sdis = new Hashtable();

        for ( int i=0; i < pathnameList.size(); i++)
        // for ( int i=0; i < 1; i++)  // for testing purposes
        {
          Integer path_cnt = new Integer(i+1);
          pathName = (String) pathnameList.get(i);
//          System.out.println(pathName);
          dsp = new DSSPathname(pathName);


          String sdi_key  = "|apart=" + dsp.getAPart() + "|"    
             + "bpart=" + dsp.getBPart() + "|"    
             + "cpart=" + dsp.getCPart() + "|"    
             + "epart=" + dsp.getEPart() + "|"    
             + "fpart=" + dsp.getFPart() + "|";
    
          sdi_key = sdi_key.toUpperCase();

          sites.put (dsp.getBPart(),path_cnt);
          sdis.put (sdi_key,dsp.getCPart());

/*
          //  now time to get all the data and times  for this path name
          hts.clear();
          hts.setPathname(pathName);
          hts.read();
          hts.getData(datarows);
          hts.getTimes(datatimes);
*/

//    System.out.println(temp_str);

          // System.out.println("Length: " + datarows.length);  // finds out how many datarows we have


/*
          // for every row go get the value and the timestamp for each record
          for ( int j=0; j < datarows.length; j++)
          {
            Double doubleValue = new Double (datarows.array[j]);
            dvalue = doubleValue.doubleValue();
            if (dvalue > -3.4028234663852886E+38)
            {
             HecTime hect = new HecTime("31 Dec 1899");
             hect.add(datatimes.array[j]);
            //System.out.println("value : " + dvalue  + "  time : " + hect.date() + " " + hect.time() + "   " + datatimes.array[j]);
            }
  
          }   // end of for to get all the rows values


 */

        }  // loop to handle each distinct pathname in the dss file
        hts.done();  // done with the file so close it
       

         int temp_cnt = 99999;
         for (Enumeration e = sites.keys() ; e.hasMoreElements() ;)
         {
            temp_cnt ++;
            String t_str = e.nextElement().toString();
            String temp_str = "insert into hdb_site (site_name,site_common_name,objecttype_id,db_site_code) values ('" 
            + t_str 
            + "','" + t_str
            + "',7,'ECO'"
            + ");";
            sites.put(t_str,new Integer(temp_cnt));

           System.out.println(temp_str);

         }
     
         temp_cnt = 99999;	
         for (Enumeration e2 = sdis.keys() ; e2.hasMoreElements() ;)
         {
            temp_cnt ++;
            String t_str = e2.nextElement().toString();
            String t_str2 = t_str.substring(t_str.indexOf("BPART=") + 6,t_str.indexOf("|",t_str.indexOf("BPART=")));
   //System.out.println(t_str2);
            String temp_str1 = "insert into hdb_site_datatype values ("
            + sites.get(t_str2) 
            + "," + datatypes.get((String)sdis.get(t_str))
            + ","+ temp_cnt 
            +" );"; 
            String temp_str = "insert into ref_dmi_data_map values (1,'" 
            + t_str 
            + "','" + DssFile.toUpperCase() + "', " + temp_cnt + ");";
           System.out.println(temp_str1);
           System.out.println(temp_str);

         }


       }

       catch (Exception e) {System.out.println(e.toString());}

    }

    private static void set_datatypes (Hashtable dt)
    {
     dt.put("FLOW","18");
     dt.put("STAGE","65");
     dt.put("ELEV","24");
     dt.put("STORAGE","16");
     dt.put("PRECIP-CUM","5");
     dt.put("TEMP","7");

    }  //end of method set_dadtatypes

}
