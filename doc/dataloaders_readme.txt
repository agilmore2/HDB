
This file is to assist hdb users in the setup and use of the data loader utilities and the
Hydromet file loaders.   These programs are written in java so the system that you run
these on must have a valid JAVA JVM loaded on it.

In general,  the java routines utilize the environment variable CLASSPATH.  This classpath must be set in a 
shell script and either be sourced by default or explicitly sourced.  Included in this classpath
environment variable must be the following:

1.  The full path and the name of the ORACLE java zip file ie /local/oracle/product/816/jdbc/lib/classes12.zip
    Generally your DBA will have already loaded this zip file to your ORACLE installation but a current copy
    can also be found at your <HDB2 release Directory>/bin/lib

2.  The full path and name of the dataloader jar file ie .../HDB2release1.0/bin/lib/dataloader.jar

3.  The full path and name of the database pool jar file ie .../HDB2release1.0/bin/lib/jdbcpool-0.99.jar

Once you have set the classpath correctly you can test the operation of any of the java utilities
by executing the command java <utility name> -v  ie.  java dataloader.Server -v
This command should successfully execute and give you the name of the utility and its version number.
Usually if you get anything other than that you have set up your CLASSPATH incorrectly.

Each of the  java utilities uses a property file to pass along to the application all the information it needs 
to perform it's intended task.. The property file is utilized to pass along parameters and provides a simple way
to do this without the user having to know exacly what order the parameters go in.  A sample property file is
located in the <RELEASE DIRECTORY>/doc directory with the name of standard.property_file.  Each JAVA Utility should
have its own peculiar property file so that the application will specifically know what type of data it intends to handle
and how the utility should identify any data that it places into the database.  The communication between the JAVA
utility and the specific property file you want referenced is done by adding a -Dstart.property=<fully referenced filename>
after the java command   ie.  java -Dstart.property=/local/progs/dayfile.property dataloader.ProcessDayfile 20JUN2003.seq

PROCESSING Hydromet DAYFILES

The java utility ProcessDayfile will read a HYDROMET dayfile and place the data into r_base table.. This can be done either via a
cron job or run at the command level.  Be sure to set the loading application name to correspond to the name of the 
loading application in your database "Hydromet Dayfile Loader" and any other defaults information so that your data is 
correctly identified.  The comments in the standard property file should suffice to answer any questions.

the command: java -Dstart.property=/local/dayfile.property dataloader.ProcessDayfile 20JUN2003.seq   will process dayfile 20JUN2003.seq
optionally you can add a log file name as the second parameter if you do not wish to have the log file name as a standard
200JUN2003.log


PROCESSING Hydromet ARCHIVE FILES

The java utility ProcessArchfile will read a HYDROMET archive file and place the data into r_base table.. This can be 
done either via a  cron job or run at the command level.  Be sure to set the loading application name to correspond to 
the name of the loading application in your database. "Hydromet Archive File Loader"  and any other defaults information 
so that your data is  The comments in the standard property file should suffice to answer any questions.

the  command: java -Dstart.property=/local/archfile.property dataloader.ProcessArchfile filename=wy2003.seq   
will process archive file wy2003.seq   

optionally you can add a log file name as an additional  parameter if you do not wish to have the log file name as a standard wy2003.log  (ie logname=xxxx.log)

optionally to can add an additional parameter to specify wherether you want a particular set of site codes processed or only a subset of site_codes/pcodes.  THe sitecode/pcode combinations are user specified, comma separated.
(ie. site_codes=GLDA,FGRU/FB)


REAL Time Dataloader


The java application real time data loader will take data via a predetermined socket and place that data into the r-base table.

the  command: java -Dstart.property=/local/rtl.property dataloader.Server  will start the socket listener.  
This utility writes to the file rtl.log any errors it encounters.


STOPPING THE REAL TIME DATA LOADER

The java utility KillServer will halt the Server socket listener gracefully.

THe command: java dataloader.KillServer <hostname>  will attempt to connect to the socket that the Server process is 
listening to and will issue a command to halt processing.
