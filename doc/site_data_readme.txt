This readme file will inform the reader how to import site specific data from their production hdb database
to their newly installed HDB2 instances.  Any questions or comments regarding this process can be directed 
via e-mail to hdb@cadswes.colorado.edu

EXPORTING SITE SPECIFIC PRODUCTION DATA:

To export the site specific production data, you will be utilizing the ORACLE export facility.  The exp utility
will create an export file that will be eventually imported to the HDB2 database.
To begin the process:

1.  set your default to the directory where this README.txt file resides.  ie : cd ..../HDB2release1.0/oracle_script/site_data
2.  Insure that your environment variable ORACLE_SID is set to the production database your copying data from
    ie. setenv ORACLE_SID lchdb
3.  execute the export using command:  exp <dba account>/<password> parfile=exportSiteData.parfile  
    note: use the dba account and password that typically maintains and owns the HDB data tables.
    note: a logfile called siteDataExp.log will be generated to review in case any problems were encountered

IMPORTING THE EXPORTED DATA TO THE NEW HDB2 INSTANCE:

To import production specific data into the new HDB2 instance, you will be utilizing the ORACLE import facility and 
 provide the .dmp file that was generated when you exported the data from your production database.
To start the import process:

1.  Insure the data export as explained above was performed and that no errors were encountered
2.  set your default to the directory where this README.txt file resides.  ie : cd ..../HDB2release1.0/oracle_script/site_data
3.  Reset the environment variable ORACLE_SID to the new HDB2 instance name.  ie: setenv ORACLE_SID lchdb2
4.  execute the loading shell script: ./sitedata.sh <HDB2DBA account> <password> 
5.  review the siteDataImp.log file and insure the import operation performed without any problems.

-NOTE:

-For Aggdissagg work under the HDB2 paradigm,  the table structure of ref_agg_dissagg was modified to allow a reference to
-the hdb_method table therefore  data entries in the old table must be carried over to the new database
-via other options besides export/import.  Use the supplied sql fixaggdissagg.sql for this.



