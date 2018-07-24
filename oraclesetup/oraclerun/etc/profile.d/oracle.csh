# Environment for Oracle
# change these settings according to your installation

# You MUST uncomment the ORACLE_SID line after changing
# the value appropriately

setenv ORACLE_BASE /ora1/app
setenv ORACLE_HOME /ora1/app/oracle/product/12.2.0/db_1
#setenv ORACLE_SID hdb

# DBA should confirm the path to the JVM, may need to add it here
setenv PATH ${PATH}:${ORACLE_HOME}/bin

setenv TNS_ADMIN ${ORACLE_HOME}/network/admin

setenv ORA_NLS12 ${ORACLE_HOME}/nls/data

if ( ${?CLASSPATH} ) then
    setenv CLASSPATH ${CLASSPATH}:${ORACLE_HOME}/JRE:${ORACLE_HOME}/jlib:${ORACLE_HOME}/rdbms/jlib:${ORACLE_HOME}/network/jlib
else
    setenv CLASSPATH ${ORACLE_HOME}/JRE:${ORACLE_HOME}/jlib:${ORACLE_HOME}/rdbms/jlib:${ORACLE_HOME}/network/jlib
endif
if ( ${?LD_LIBRARY_PATH} ) then
    setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${ORACLE_HOME}/lib
else
    setenv LD_LIBRARY_PATH ${ORACLE_HOME}/lib
endif

# setenv ORACLE_TERM xterm
# setenv NLS_LANG AMERICAN-AMERICA.UTF8
