
RELEASE/.cshrc_hdb_app exists to consolidate and effectively manage the 
environment variables required to successfully run any HDB application. 

Edit and use this file as indicated below:

*Oracle variables:

- Set ORACLE_HOME to the directory where your current version of Oracle
  is installed.
- Set ORACLE_BASE to Oracle's base directory
- Set ORACLE_LPPROG, ORACLE_LPSTAT and ORACLE_PAGER to point to the
  appropriate executables for printing, getting printer status, and paging.
- Set ORACLE_TERM to your terminal type.
- Set ORACLE_SID to the name of your HDB installation.

*HDB Variables

- Set HDB_LOCAL to the name of your HDB installation.
- Set HDB_ENV to the name of your current release directory.

*Oracle Permissions

Do nothing; all values are set appropriately.

*Hydromet Data Loading

Do nothing; all values are set appropriately or no longer needed.

*Hydromet HDB/VAX

While the Hydromet apps no longer run for HDB2, these variables may
need to be set to facilitate the movement of Hydromet data files from
the Hydromet machine to the database server; it depends on how you 
choose to implement this process at your installation.

- Set DAYDATA_DIR to the directory where the Dayfiles will be seen on
  the Database Server.
- Set ARCDATA_DIR to the directory where the Dayfiles will be seen on
  the Database Server.
- Set MOUNTDIR to the directory crossmounted from the Hydromet machine
  to the Database Server, if there is one.
- Set HYD_HOST to the name of your Hydromet machine.
- Set HYD_USER to the name of the user which an ssh or rsh into the Hydromet
  machine would be executed. (Note: it is recommended that rsh'ing into
  the Hydromet machine be stopped for security reasons.)


This file should be sourced in every HDB user's .cshrc file to guarantee that 
all environment variables are properly set. Additionally, this file should be 
sourced in the DBA's .cshrc file, and also before any invocation of an HDB 
application on the crontab; sourcing .cshrc_hdb_app will guarantee that the 
cron environment is properly set to run any HDB applications.