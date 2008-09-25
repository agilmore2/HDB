#!/usr/bin/perl

# Do the following
#store current time in a variable
#run daily report for yesterday 
#
#(grab all data for defined sites/datatypes 
#   for the last 3 hours
#   send via ftp to ftp.usbr.gov
#
#check to see if there are any updates to data for the period from 4 days ago
#  to the beginning of the lookback period (3 hours, defined in $hoursback)
#  by checking date_time_loaded against the timestamp stored in previous run.
# ftp these separately
#
#update a driver table to store the timestamp found in step 1.

#Data for this query is defined by an entry in hdb_ext_data_source
#with the name 'HDB Paradox Update' and a loading application id exists 
#with the same name. 
#Further, we'll use the site names
#and use the datatype_common_names to populate the ref_ext_site_data_map
#table. These site_data mappings will specify which site_datatype_ids to
#query for this application.
#See the end of this program for the details on metadata
#
#Assumptions: all queries are for instantaneous data

use strict;
use warnings;

use English '-no_match_vars';
#change this so that fields from printed arrays are separated by newlines
$OUTPUT_FIELD_SEPARATOR="\n";

#use libraries from HDB environment (Solaris only except for HDB.pm)
use lib "$ENV{HDB_ENV}/perlLib/lib";

use Hdb;

my $app_name = "HDB Paradox Update";
my $hoursback = 3;


if (not defined($ARGV[0]) 
     or $ARGV[0] ne '-a'
     or not defined($ARGV[1])) {
  usage();  
}

shift; #remove -a
my $filename = shift;

my $hdb=Hdb->new;
$hdb->connect_from_file($filename);

# look for -d flag for testing
my $testing = shift;

my ($mail);

if (defined ($testing) && $testing eq "-d") {
  $mail = 'cat';
} else {
  $testing=0;
  $mail = 'cat >/dev/null'
}
 
$hdb->dbh->do("alter session set NLS_DATE_FORMAT = 'YYYY-MM-DD HH24:MI:SS'");

#many of these HDB queries use the following syntax:
# my $var = @{$hdb->dbh->selectcol_arrayref("query")}[0]
# this is just a fancy way of getting the first item in the array that
# a reference is returned to by the query.
# since no column number is given to the selectcol_arrayref function,
# the first column is returned.
# since all of the queries in this script only return one column, this
# is quiet useful. 

# get the current time from the database
my $current_time = @{$hdb->dbh->selectcol_arrayref("
select sysdate current_time from dual
")}[0];

# get the date for yesterday and print it
my $hours = @{$hdb->dbh->selectcol_arrayref("
select to_char(trunc(sysdate - $hoursback/24,'HH24'), 'YYYYMMDDHH24') hours from dual
")}[0];

print $hours."\n";

#get data where the truncated to hour date is less than $hours hours ago
my $dailysql =  "select
 to_char(a.start_date_time,'YYYY-MM-DD,HH24:MI:SS')||','||
 b.primary_site_code||','||b.primary_data_code||','||
 round(a.value,2) as data
from
 r_instant a, ref_ext_site_data_map b, hdb_ext_data_source c
where
 trunc(a.start_date_time,'HH24') >= to_date('$hours','YYYYMMDDHH24') and
 b.hdb_site_datatype_id = a.site_datatype_id and
 b.ext_data_source_id = c.ext_data_source_id and
 c.ext_data_source_name = '$app_name' 

 order by b.primary_site_code, b.primary_data_code, a.start_date_time
";

# update the date loaded time
my $updatetimesql = "update ref_loading_application_prop
set prop_value = '$current_time'
where prop_name = 'last_retrieval' and
loading_application_id = 
(select loading_application_id from
hdb_loading_application 
where loading_application_name = '$app_name')";

# get the time that last application ran
my $lasttimesql = "select prop_value last_timestamp
from ref_loading_application_prop a, hdb_loading_application b
where prop_name = 'last_retrieval' and 
 a.loading_application_id = b.loading_application_id and
 b.loading_application_name = '$app_name'";
 
my $last_timestamp = @{$hdb->dbh->selectcol_arrayref("$lasttimesql")}[0];

  if (not defined($last_timestamp)) {
    $last_timestamp= @{$hdb->dbh->selectcol_arrayref("
    select sysdate - 1 last_timestamp from dual")}[0];
  }

#get data for the last 4 days until 3 hours ago
# that changed since one minute before the last time this application ran
my $updatessql = "select 
 to_char(a.start_date_time,'YYYY-MM-DD,HH24:MI:SS')||','||
 b.primary_site_code||','||b.primary_data_code||','||
 round(a.value,2) as data
from
 r_instant a, ref_ext_site_data_map b, hdb_ext_data_source c
where 
 trunc(a.start_date_time,'DD')
  between trunc(sysdate - 4,'DD')  and trunc(sysdate - $hoursback/24,'HH24') and
 a.date_time_loaded >= to_date('$last_timestamp')-(1/(24*60)) and
 b.hdb_site_datatype_id = a.site_datatype_id and
 b.ext_data_source_id = c.ext_data_source_id and
 c.ext_data_source_name = '$app_name' 

 order by b.primary_site_code, b.primary_data_code, a.start_date_time
";

my $data = $hdb->dbh->selectcol_arrayref("$dailysql");

# get the array that $data refers to
my @output = @$data;

#ftp this to ftp.usbr.gov
if (!$testing) {
  open FTP, "|ncftpput -o useCLNT=0 -f account.dat -c paradox/paradox_$hours.dat"
  or $hdb->hdbdie("ftp failed! $!");
  
  print FTP @output;
  print FTP "\n";
  close FTP;

}

if ($testing) {
print @output;
print "\n";
}

#check if we're not testing, if so, do the update data query

if (!$testing) {

 
#get all data changed between a minute before the last check ($last_timestamp)
# and two days ago
  my $data = $hdb->dbh->selectcol_arrayref("$updatessql");

  my @output = @$data;

  open FTP, "|ncftpput -o useCLNT=0 -f account.dat -c paradox/paradox_update_$hours.dat" 
  or $hdb->hdbdie("ftp failed! $!");

  print FTP @output;
  print FTP "\n";
  close FTP;

  $hdb->dbh->do("$updatetimesql");

  $hdb->dbh->commit;
}

exit 0;

sub usage {
  print "Usage: $0 -a hdbaccountfile <-d if testing>\n"; 
  exit 1; 
}


=item Example DDL/DML to generate metadata to drive this report:

Paul actually did this via the MetaData App.

insert into hdb_loading_application values(null, 'HDB Paradox Update',null,
'comment made by Paul');

Except for this one... :)
insert into ref_loading_application_prop values(73,'last_retrieval',sysdate);

insert into hdb_ext_data_source
values (
 9, 'HDB Paradox Update', 7,null,
7,null,null,null,
'another comment made by Paul',null
);



=cut
