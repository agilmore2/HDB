
This file is to assist hdb users in the setup and use of the Water Accounting META-data loader utility.
This program is written in java so the system that you run this application on must have a valid 
JAVA JVM loaded on it.

In general,  the java routines utilize the environment variable CLASSPATH.  This classpath must be set in a 
shell script and either be sourced by default or explicitly sourced.  Included in this classpath
environment variable must be the following:

1.  The full path and the name of the ORACLE java zip file ie /local/oracle/product/816/jdbc/lib/classes12.zip
    Generally your DBA will have already loaded this zip file to your ORACLE installation but a current copy
    can also be found at your <HDB2 release Directory>/bin/lib

2.  The full path and name of the dataloader jar file ie .../HDB2release1.0/bin/lib/dataloader.jar

3.  The full path and name of the database pool jar file ie .../HDB2release1.0/bin/lib/jdbcpool-0.99.jar

Once you have set the classpath correctly you can test the operation of the java Metadata loading utility
by executing the command:  java dataloader.ProcessMetadata -v
This command should successfully execute and give you the name of the utility and its version number.
Usually if you get anything other than that you have set up your CLASSPATH incorrectly.

The java Metatadata loading utility uses a property file to pass along to the application all the information it needs 
to perform it's intended task.. The property file is utilized to pass along parameters and provides a simple way
to do this without the user having to know exacly what order the parameters go in.  A sample property file is
located in the <RELEASE DIRECTORY>/doc directory with the name of standard.property_file.  
However the Metadata loaders' property file should look like the following:

DRIVER=oracle.jdbc.driver.OracleDriver
URL=jdbc:oracle:thin:@THE_NAME_OF_YOUR_MACHINE:1521:THE_DB_INSTANCE_NAME
USERNAME=dba_username
PASSWORD=db_password
# modelid will be equal to the model_id value in hdb_model for the model that generated the metadata file
MODEL_ID=1
# config_id is the default config_id that the user wants for any new records that are inserted into the database
# a default of zero should be adequate
CONFIG_ID=0
# this application loads site information into the hdb_site table.  A required 
# data element is the db_site_code found in your ref_db_list table
# it a a three character field usually a representation of your database 
#  the program default is HDB but will probably not work for most database
# intallations
DB_SITE_CODE=ALB

The communication between the JAVA Water Accounting Metadata loader utility and the specific property file you 
want referenced is done by adding a -Dstart.property=<fully referenced filename>
after the java command:

 ie. java -Dstart.property=/local/progs/wa_model_1.property dataloader.ProcessWAMetadata model_1.log 

GENERATION OF THE WATER ACCOUNTING METADATA FILE:

The generation of the metadata file necessary to run this application is performed by running the 
RiverWare software application on the Water Accounting Model you either wish to have update or 
inserted into the Water Accounting Meta Data tables.

The command:  riverware --acctdata model_1.data  will generate the required data file model_1.data


MOVING THE METADATA FILE TO THE PROPER ORACLE DIRECTORY:

The Metadata loading utility utilizes the ORACLE external table facility.  The metadata file must reside where ORACLE 
expects the file to be and must be the exact file name that the external table DDL specifies.  Your ORACLE DBA will 
assist in the identification and the file name specification for this application to work correctly.
 
