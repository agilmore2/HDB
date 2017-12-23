# Environment for Oracle
# change these settings according to your installation

# You MUST uncomment the ORACLE_SID line after changing
# the value appropriately

ORACLE_BASE=/ora1/app
ORACLE_HOME=/ora1/app/oracle/product/12.2.0/db_1
#ORACLE_SID=hdb

# DBA should confirm that JDK is available, otherwise add it on the path
PATH=$PATH:$ORACLE_HOME/bin

TNS_ADMIN=$ORACLE_HOME/network/admin
ORA_NLS12=$ORACLE_HOME/nls/data
LD_LIBRARY_PATH=${LD_LIBRARY_PATH:+$LD_LIBRARY_PATH:}$ORACLE_HOME/lib
CLASSPATH=${CLASSPATH:+$CLASSPATH:}$ORACLE_HOME/JRE:$ORACLE_HOME/jlib:$ORACLE_HOME/rdbms/jlib:$ORACLE_HOME/network/jlib

export ORACLE_BASE ORACLE_HOME ORA_NLS12 ORACLE_SID PATH LD_LIBRARY_PATH CLASSPATH TNS_ADMIN

# ORACLE_TERM=xterm; export ORACLE_TERM
# NLS_LANG=AMERICAN-AMERICA.UTF8; export NLS_LANG

# Unnecessary now with Oracle supporting preinstall rpm that sets security limits for oracle directly
#if [ ${USER:-nobody} = "oracle" ]; then 
#      ulimit -u 16384 -n 63536
#fi
