#!/usr/bin/perl

# Do the following
#store current time in a variable
#run daily report for yesterday 
#
#(grab all data for defined sites/datatypes 
#   from yesterday
#   and write them to a file named wapa_dateoflookback.dat)
#
#check to see if there are any updates to data for the period from 40 days ago
#  to the beginning of the lookback period (yesterday)
#  by checking date_time_loaded against the timestamp stored in previous run.
#write all these rows to a file named wapa_update_dateoflookback.dat
#
#update a driver table to store the timestamp found in step 1.

#Data for this query is defined by an entry in hdb_ext_data_source
#with the name 'HDB WAPA/GCMRC Update' and a loading application id exists 
#with the same name. 
#Further, we'll reuse the SCADA(?) site name mappings
#and use the datatype_common_names to populate the ref_ext_site_data_map
#table. These site_data mappings will specify which site_datatype_ids to
#query for this application.
#See the end of this program for the details on metadata
#
#Assumptions: all queries are for hourly data

use strict;
use warnings;

use English '-no_match_vars';
#change this so that fields from printed arrays are separated by newlines
$OUTPUT_FIELD_SEPARATOR="\n";

#use libraries from HDB environment (Solaris only except for HDB.pm)
use lib "$ENV{HDB_ENV}/perlLib/lib";

use Hdb;

my $app_name = "HDB WAPA/GCMRC Update";

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

#handle bogus mail transfer programs. In Linux, mail is the more featureful,
# and in Solaris, mailx is.
if ($OSNAME eq 'linux') {
  $mail = "mail";
} elsif ($OSNAME eq 'solaris') {
  $mail = "mailx";
}

if (defined ($testing) && $testing eq "-d") {
  $mail = 'cat';
} else {
  $testing=0;
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
my $yesterday = @{$hdb->dbh->selectcol_arrayref("
select to_char(trunc(sysdate - 1,'DD'), 'YYYYMMDD') day from dual
")}[0];

print $yesterday."\n";

#define SQL queries. 
my $mailtosql =  "select description from hdb_ext_data_source where
ext_data_source_name = '$app_name'";

#get data where the truncated to day date is yesterday
my $dailysql =  "select
 to_char(a.start_date_time,'YYYY-MM-DD,HH24:MI:SS')||','||
 b.primary_site_code||','||b.primary_data_code||','||
 round(a.value,2) as data
from
 r_hour a, ref_ext_site_data_map b, hdb_ext_data_source c
where
 trunc(a.start_date_time,'DD') = to_date('$yesterday','YYYYMMDD') and
 b.hdb_site_datatype_id = a.site_datatype_id and
 b.ext_data_source_id = c.ext_data_source_id and
 c.ext_data_source_name = '$app_name' 

 order by b.primary_site_code, b.primary_data_code, a.start_date_time
";

# update the date loaded time
my $updatetimesql = "update ref_loading_application_data
set last_retrieval = '$current_time'
where loading_application_id = 
(select loading_application_id from
hdb_loading_application 
where loading_application_name = '$app_name')";

# get the time that last application ran
my $lasttimesql = "select last_retrieval last_timestamp
from ref_loading_application_data a, hdb_loading_application b
where a.loading_application_id = b.loading_application_id and
 b.loading_application_name = '$app_name'";
 
my $last_timestamp = @{$hdb->dbh->selectcol_arrayref("$lasttimesql")}[0];

  if (not defined($last_timestamp)) {
    $last_timestamp= @{$hdb->dbh->selectcol_arrayref("
    select sysdate - 1 last_timestamp from dual")}[0];
  }

#get data for the last 40 days until the day before yesterday
# that changed since one minute before the last time this application ran
my $updatessql = "select 
 to_char(a.start_date_time,'YYYY-MM-DD,HH24:MI:SS')||','||
 b.primary_site_code||','||b.primary_data_code||','||
 round(a.value,2) as data
from
 r_hour a, ref_ext_site_data_map b, hdb_ext_data_source c
where 
 trunc(a.start_date_time,'DD')
  between trunc(sysdate - 40,'DD')  and trunc(sysdate - 2,'DD') and
 a.date_time_loaded >= to_date('$last_timestamp')-(1/(24*60)) and
 b.hdb_site_datatype_id = a.site_datatype_id and
 b.ext_data_source_id = c.ext_data_source_id and
 c.ext_data_source_name = '$app_name' 

 order by b.primary_site_code, b.primary_data_code, a.start_date_time
";

#get the email addresses to write to
# first line of this reponse is
# mailto: <emailaddresses>
my @mailto = @{$hdb->dbh->selectcol_arrayref("$mailtosql")};

my ($to) = split /\n/,$mailto[0];
$to =~ s/^mailto: //;
chomp $to;

my $cc = $ENV{HDB_NOTIFY_EMAIL};

my $data = $hdb->dbh->selectcol_arrayref("$dailysql");

# get the array that $data refers to
my @output = @$data;

#mail this data to WAPA
my $mailcmd = $mail;
if (!$testing) {
  #grep for GLEN, and ftp it to GCMRC
  my @glen = grep {/GLEN/} @output;
  open GLEN, "|ncftpput -f gcqaccount.dat -c glen_$yesterday.dat"
  or $hdb->hdbdie("ftp failed! $!");
  
#  open GLEN, ">glen_$yesterday.dat";
  print GLEN @glen;
  print GLEN "\n";
  close GLEN;

  #set up mail command
  my $subject = "\"WAPA SCADA Daily $yesterday\"";
  $mailcmd = $mailcmd ." -s ". $subject . " -c ".$cc." ".$to;
}

open MAIL, "|$mailcmd" or $hdb->hdbdie("unable to open mail: $!\n");
print MAIL @output;
print MAIL "\n";
close MAIL;

#check if we're not testing, if so, do the update data query
# and grep out GLEN for use by GCMRC

if (!$testing) {

 
#get all data changed between a minute before the last check ($last_timestamp)
# and two days ago
  my $data = $hdb->dbh->selectcol_arrayref("$updatessql");

  my @output = @$data;

  my $subject = "\"WAPA SCADA Update $yesterday\"";

  $mailcmd = $mail ." -s ". $subject . " -c ".$cc." ".$to;
  open MAIL, "|$mailcmd" or $hdb->hdbdie("unable to open mail: $!\n");
  print MAIL @output;
  print MAIL "\n";
  close MAIL;

  # do not want to delete the output of the updates.
  #  unlink "wapa_update_$sqlout.dat";

  my @glen = grep {/GLEN/} @output;

  open GLEN, "|ncftpput -f gcqaccount.dat -c glen_update_$yesterday.dat" 
  or $hdb->hdbdie("ftp failed! $!");

# open GLEN, ">glen_update_$yesterday.dat";
  print GLEN @glen;
  print GLEN "\n";
  close GLEN;

  $hdb->dbh->do("$updatetimesql");

  $hdb->dbh->commit;
}

exit 0;

sub usage {
  print "Usage: $0 -a hdbaccountfile <-d if testing>\n"; 
  exit 1; 
}


=item DDL/DML to generate metadata to drive this report:


insert into hdb_loading_application values(null, 'HDB WAPA/GCMRC Update',null,
'Reporting application for data from r_hour for WAPA and GCMRC');

CREATE TABLE ref_loading_application_data
(
        LOADING_APPLICATION_ID NUMBER NOT NULL,
        LAST_RETRIEVAL DATE  -- Null means no retrieval yet.
)
tablespace HDB_data
;

-- primary key for table cp_comp_proc_lock
alter table ref_loading_application_data  add constraint rlad_loading_appl_id_pk 
primary key (loading_application_id) 
using index tablespace hdb_idx;

--  foreign key loading_application_id
alter table ref_loading_application_data add constraint rlad_loading_appl_id_pk 
foreign key (loading_application_id) 
references hdb_loading_application (loading_application_id);

grant select,update,insert on ref_loading_application_data to app_user;
create public synonym ref_loading_application_data for ref_loading_application_data;

insert into ref_loading_application_data values(53,sysdate);

insert into hdb_ext_data_source
values (
 9, 'HDB WAPA/GCMRC Update', 7,null,
7,null,null,null,
'Report definition for querying data from r_hour for WAPA and GCMRC, data codes use datatype_common_names from hdb_datatype',null
);

select 'exec create_site_data_map ('||
a.ext_data_source_id||','||b.primary_site_code||','||
d.datatype_common_name||',0,'||e.site_datatype_id||
',''hour'',null,null,7,''Y'',null,null);'
from hdb_ext_data_source a, hdb_ext_site_code b,
hdb_ext_site_code_sys c, hdb_datatype d, hdb_site_datatype e
where 
a.ext_data_source_name = 'HDB WAPA/GCMRC Update' and
b.ext_site_code_sys_id = c.ext_site_code_sys_id and
c.ext_site_code_sys_name = 'XA21 SCADA' and
e.site_id = b.hdb_site_id and
e.datatype_id = d.datatype_id and
d.datatype_id in (39,49,46,1197,73,14);

--Other datatypes?

	exec create_site_data_map (9,BLUE,power release,0,1857,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,BLUE,spill,0,4147,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,BLUE,pool elevation,0,1923,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,BLUE,ave bypass release,0,4149,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,BLUE,ave head,0,1970,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,BLUE,power,0,1976,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,power release,0,1859,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,spill,0,4155,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,pool elevation,0,1925,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,ave bypass release,0,4157,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,ave head,0,1972,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,power,0,1978,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,power release,0,1861,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,spill,0,2416,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,pool elevation,0,1927,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,ave bypass release,0,4164,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,ave head,0,1974,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,power,0,1980,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,power release,0,1860,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,spill,0,4159,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,pool elevation,0,1926,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,ave bypass release,0,4161,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,ave head,0,1973,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,power,0,1979,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,power release,0,1862,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,spill,0,4166,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,pool elevation,0,1928,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,ave bypass release,0,4167,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,ave head,0,1975,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,power,0,1981,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,power release,0,1858,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,spill,0,4151,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,pool elevation,0,1924,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,ave bypass release,0,4153,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,ave head,0,1971,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,power,0,1977,'hour',null,null,7,'Y',null,null);



=cut
