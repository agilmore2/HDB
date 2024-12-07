#!/bin/bash
#days=`/usr/local/bin/date +%d -d 'yesterday'`
#echo $days
# we create the new data, and then put it on the server.
#site_data 919 42 $days |\grep -v ORACLE > temp.out 
#echo "Powell Release	acre-ft" >powellrel
#due to the old geezer, we are using 1.9835 here to match Hydromet
#cat temp.out | awk '/-201/ {s += $2 ; printf "%s\t%d\n",$1,$2*1.9835} END {printf "Monthly Total is:\t%s\n",s*1.9835}' >>powellrel
#
#rm powellrel temp.out

# the whole shell script was changed by M.bogner on 31-MARCH-2011 to run a sql report
# the reasonm was that site data core dumped on ISIS and I didn't want to debug an
# old pro*c program; instead replaced it with a simple sql report

sqlplus -s $APP_USER/$HDB_LOCAL@$HDB_LOCAL @powell_releases.sql
EMAIL="-TTo:hpatno@usbr.gov,wilhite@wapa.gov,abrooks@wapa.gov,MTPreschedule@wapa.gov,rfellin@wapa.gov,apivarnik@usbr.gov,mcallahan@usbr.gov,ntodea@usbr.gov,charris@wapa.gov,thenriquez@wapa.gov,Mtr-Mrk@wapa.gov"
mail -s "Powell Release" "$EMAIL" < powell_releases.out



