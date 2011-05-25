#!/usr/bin/perl

# Do the following
#store current time in a variable
#run daily report for yesterday 
#
#(grab all data for defined sites/datatypes 
#   from yesterday
#   send in an email to the defined parties)
#
#check to see if there are any updates to data for the period from 10 days ago
#  to the beginning of the lookback period (yesterday)
#  by checking date_time_loaded against the timestamp stored in previous run.
#email these in a separate email. Send blank?
#
#update a driver table to store the timestamp found in step 1.

#Data for this query is defined by an entry in hdb_ext_data_source
#with the name 'HDB CODWR Update' and a loading application id exists 
#with the same name. 
#We're using fairly generic names and codes for ref_ext_site_data_map
#These site_data mappings will specify which site_datatype_ids to
#query for this application.
#See the end of this program for the details on metadata
#
#Assumptions: all queries are for daily data

use strict;
use warnings;

use English '-no_match_vars';
#change this so that fields from printed arrays are separated by newlines
$OUTPUT_FIELD_SEPARATOR="\n";

#use libraries from HDB environment (Solaris only except for HDB.pm)
use lib "$ENV{PERL_ENV}/lib";

use Hdb;

my $app_name = "HDB CODWR Update";

if (not defined($ARGV[0]) 
     or $ARGV[0] ne '-a'
     or not defined($ARGV[1])) {
  usage();  
}

shift; #remove -a
my $filename = shift;

my $hdb=Hdb->new;
$hdb->connect_from_file($filename);
$hdb->set_approle();
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
 to_char(a.start_date_time,'YYYY-MM-DD')||','||
 b.primary_site_code||','||b.primary_data_code||','||
 round(a.value,2) as data
from
 r_day a, ref_ext_site_data_map b, hdb_ext_data_source c
where
 trunc(a.start_date_time,'DD') = to_date('$yesterday','YYYYMMDD') and
 b.hdb_site_datatype_id = a.site_datatype_id and
 b.ext_data_source_id = c.ext_data_source_id and
 c.ext_data_source_name = '$app_name'  and
 b.is_active_y_n = 'Y' -- SJ disabled per Erik Knight 8/18
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

#get data for the last 40 days until the day before yesterday
# that changed since one minute before the last time this application ran
my $updatessql = "select 
 to_char(a.start_date_time,'YYYY-MM-DD')||','||
 b.primary_site_code||','||b.primary_data_code||','||
 round(a.value,2) as data
from
 r_day a, ref_ext_site_data_map b, hdb_ext_data_source c
where 
 trunc(a.start_date_time,'DD')
  between trunc(sysdate - 10,'DD')  and trunc(sysdate - 2,'DD') and
 a.date_time_loaded >= to_date('$last_timestamp')-(1/(24*60)) and
 b.hdb_site_datatype_id = a.site_datatype_id and
 b.ext_data_source_id = c.ext_data_source_id and
 c.ext_data_source_name = '$app_name' and
 b.is_active_y_n = 'Y' -- SJ disabled per Erik Knight 8/18

 order by b.primary_site_code, b.primary_data_code, a.start_date_time
";

#get the email addresses to write to
# first line of this reponse is
# mailto: <emailaddresses>
my @mailto = @{$hdb->dbh->selectcol_arrayref("$mailtosql")};

my ($to) = split /\n/,$mailto[0];
$to =~ s/^mailto: //;
chomp $to;

my $cc = $ENV{HDB_XFER_EMAIL};

do_mail ($dailysql, "\"Gunnison Daily $yesterday\"");
do_mail ($updatessql, "\"Gunnison Update $yesterday\"");

# update timestamp in database
if (!$testing) {
  $hdb->dbh->do("$updatetimesql");

  $hdb->dbh->commit;
}

exit 0;

sub usage {
  print "Usage: $0 -a hdbaccountfile <-d if testing>\n"; 
  exit 1; 
}


sub do_mail {
  my $sql  = shift;
  my $subject = shift;

  my $data = $hdb->dbh->selectcol_arrayref("$sql");

  # get the array that $data refers to
  my @output = @$data;

  #mail this data to CODWR
  my $mailcmd = $mail;

  if ( !$testing ) {
    #set up mail command
    $mailcmd = $mailcmd . " -s " . $subject . " -c " . $cc . " \'" . $to . "\'";
  }

  if ( $#output > 0 ) {
    open MAIL, "|$mailcmd" or $hdb->hdbdie("unable to open mail: $!\n");
    print MAIL @output;
    print MAIL "\n";
    close MAIL;
  }
}


=item DDL/DML to generate metadata to drive this report:


insert into hdb_loading_application values(null, 'HDB CODWR Update',null,
'Reporting application for data from r_day for the State of Colorado (CODWR)');

insert into ref_loading_application_prop values(70,'last_retrieval',sysdate);

insert into hdb_ext_data_source
values (
 36, 'HDB CODWR Update', 7,null,
7,null,null,null,
'mailto: Dan Crabtree <dcrabtree@usbr.gov>, Erik Knight <eknight@usbr.gov>, Bob.hurford@state.co.us, Dan Crabtree <dan.crabtree@bresnan.net>,Robert Hurford <bhurford@montrose.net>,Gina Dearcos <gina.dearcos@state.co.us>',
null);

--Other datatypes?

begin
create_site_data_map(36,'Taylor Park','FB',0,1922,'day',null,null,null,'Y',null,null);
create_site_data_map(36,'Taylor Park','AF',0,1713,'day',null,null,null,'Y',null,null);
create_site_data_map(36,'Blue Mesa','FB',0,1923,'day',null,null,null,'Y',null,null);
create_site_data_map(36,'Blue Mesa','AF',0,1714,'day',null,null,null,'Y',null,null);
create_site_data_map(36,'Morrow Point','FB',0,1924,'day',null,null,null,'Y',null,null);
create_site_data_map(36,'Morrow Point','AF',0,1715,'day',null,null,null,'Y',null,null);
create_site_data_map(36,'Silver Jack','FB',0,1944,'day',null,null,null,'Y',null,null);
create_site_data_map(36,'Silver Jack','AF',0,1737,'day',null,null,null,'Y',null,null);
create_site_data_map(36,'Crystal','FB',0,1925,'day',null,null,null,'Y',null,null);
create_site_data_map(36,'Crystal','AF',0,1716,'day',null,null,null,'Y',null,null);
create_site_data_map(36,'Crystal','REL',0,1869,'day',null,null,null,'Y',null,null);
end;
=cut
