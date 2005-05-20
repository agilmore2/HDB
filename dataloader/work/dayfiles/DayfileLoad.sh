i=6;
while [ $i -ge 1 ]
do
 echo $i
 ./hmDayJavaDate.sh `/usr/local/bin/date +%Y%h%d -d"$i days ago"|tr -s '[:upper:]' '[:lower:]'` 
 i=`expr $i - 1`
done

