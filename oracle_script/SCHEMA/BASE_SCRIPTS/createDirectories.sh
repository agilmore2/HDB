# use this script as the template to create your database directory
# structure.  substitute the db_name with the actual instance name as well
# as substitute the oracle_dir with the actual mount point name 
# in vi : %s/oracle_dir/"actual drive name"/g
# make sure ORACLE_BASE environment variable has been set

mkdir $ORACLE_BASE/admin/db_name
mkdir $ORACLE_BASE/admin/db_name/arch	
mkdir $ORACLE_BASE/admin/db_name/bdump	
mkdir $ORACLE_BASE/admin/db_name/cdump	
mkdir $ORACLE_BASE/admin/db_name/udump	
mkdir $ORACLE_BASE/admin/db_name/create
mkdir $ORACLE_BASE/admin/db_name/pfile

mkdir /oracle_dir/oradata
mkdir /oracle_dir/oradata/db_name

mkdir /oracle_dir/oradata/db_name/db_name_redoa
mkdir /oracle_dir/oradata/db_name/db_name_redob
mkdir /oracle_dir/oradata/db_name/db_name_redoc

mkdir /oracle_dir/oradata/db_name/db_name_undots
mkdir /oracle_dir/oradata/db_name/db_name_users
mkdir /oracle_dir/oradata/db_name/db_name_temp

mkdir /oracle_dir/oradata/db_name/db_name_index
mkdir /oracle_dir/oradata/db_name/db_name_data
mkdir /oracle_dir/oradata/db_name/db_name_sys


