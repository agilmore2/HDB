export DBA=
export DBA_PASSWD=

for dirs in `cat UCapps`
do 
    echo $dirs
    cd $dirs/src
    make clean
    make
    cd ../..
    sleep 5
done
