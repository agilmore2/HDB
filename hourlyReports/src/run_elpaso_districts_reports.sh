#!/bin/bash
# This shell script created by M. Bogner, Sutron Corporation 27-May-2011
# this shell script was created to run the elpaso_district reports perl script and
# then ftp the resultant dat files to the FTP server.
# this shell script is intended to be a temporary fix to why the ftp command
# in the perl program doesn't work

# remove all the .dat files to make sure we are working with fresh files
 rm *.dat

#  now generate the report files
 ./elpaso_districts_report2.pl -a $HOME/.app_user_login

# now ftp the files over to the USBR FTP file server
# ncftpput -u agilmore -p march28 ftp.usbr.gov /elpaso *.dat

#this sftp realtime (every 15 minutes) data to secure.usbr.gov
sftp -oBatchMode=yes jzyu@usbr.gov@secure.usbr.gov<<EOF
cd elpaso
put *.dat
bye
EOF

#this copy realtime (every 15 minutes) data to c5stagesvr_uc_water
yes | cp -rf *.dat /wrg/exec/pub/c5stagesvr_uc_water/elpaso_water
