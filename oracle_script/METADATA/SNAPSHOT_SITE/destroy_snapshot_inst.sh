echo Enter dba name at local installation:
read dba_name

echo Enter dba Oracle password:
read psswd

echo "Destroy snapshot installation? (y or n)"
read answer

if test $answer != y; then
  echo Exiting...
  exit
fi

echo Running snap_de.sql...
sqlplus $1/$2 < snap_de.sql > snap_de.out

echo "**********************"
echo "Check output in snap_de.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running gen_drop.sql...
sqlplus $1/$2 < gen_drop.sql > gen_drop.out

echo Running gen_enbl.sql...
sqlplus  $1/$2 < gen_enbl.sql > gen_enbl.out

echo Now you MUST edit drop_snap_fk.sql and enbl_con.sql.
echo Remove all non-SQL statements from beginning and end of files.
echo When these files have been edited, press any key to continue.
read continue

echo Running drop_snap_fk.sql...
sqlplus $1/$2 < drop_snap_fk.sql > drop_snap_fk.out

echo "**********************"
echo "Check output in drop_snap_fk.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running enbl_con.sql...
sqlplus $1/$2 < enbl_con.sql > enbl_con.out

echo "**********************"
echo "Check output in enbl_con.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi


echo "** Snapshot installation destroyed. **"
exit
