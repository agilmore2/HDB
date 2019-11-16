#!/bin/bash

function usage () {
cat << EOH
Usage: ecohdb2hdb.sh Loads data from ECOHDB via database procedure
  -h             Help
  -n [number]    Number of days back to load data
  -a [loginfile] File (like .app_user_login) to use for credentials
  -u [username]  Username to use for $HDB_LOCAL database login
  -p [password]  Password to use for \$HDB_LOCAL database login. Not recommended.
                 Exposing passwords on the command line is not secure.
EOH
}

while getopts ":hn:a:u:p:" opt; do
  case ${opt} in
    "h" )
      usage
      exit 1
      ;;
    "n" )
      if [ "$OPTARG" -eq "$OPTARG" ] 2>/dev/null
      then
        numdays=$OPTARG
      fi
      ;; 
    "u" )
      if [ -n "$OPTARG" ] 2>/dev/null
      then
        app=$OPTARG
        db=$HDB_LOCAL
      fi
      ;;
    "p" )
      if [ -n "$OPTARG" ] 2>/dev/null
      then
        pass=$OPTARG
      fi
      ;;
    "a" )
      if [ -r $OPTARG ] #file is readable
      then
        if [ -z `find $OPTARG -perm /go+w` ] #check that group and others cannot write 
        then 
          app=`sed -ne '/username/ s/username //p' $OPTARG`
          pass=`sed -ne '/password/ s/password //p' $OPTARG`
          db=`sed -ne '/database/ s/database //p' $OPTARG`
        else
          echo "Permissions on $OPTARG are wrong. group and others may not write"
          usage
          exit 1
        fi
      fi
  esac
done



#if [ -z "$app" -o -z "$pass" -o -z "$db" -o -z "$numdays" ] ; then
if [ -z "$db" -o -z "$numdays" ] ; then
  usage
  exit 1
fi

sqlplus -S /nolog <<EOS
--connect $app/$pass@$db
connect /@$db
@ecohdb2hdb.sql $numdays
EOS
