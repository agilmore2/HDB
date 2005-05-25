export DBA=
export DBA_PASSWD=

for dirs in `cat CADSWES_APPS`
do 
    echo $dirs
    cd $dirs/src
    make clean
    make
    cd ../..
    sleep 5
done
