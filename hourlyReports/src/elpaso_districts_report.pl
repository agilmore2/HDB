#!/usr/bin/perl

# Do the following
#ata for this query is defined by an entry in hdb_ext_data_source
#with the name 'HDB El Paso Update' 
# The site_data mappings will specify which site_datatype_ids to
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
use Net::FTP;

my $app_name = "HDB El Paso Update";

if (not defined($ARGV[0]) 
     or $ARGV[0] ne '-a'
     or not defined($ARGV[1])) {
  usage();
}

shift; #remove -a
my $filename = shift;

my $hdb=Hdb->new;
$hdb->connect_from_file($filename);
$hdb->dbh->{FetchHashKeyName} = "NAME_lc";

$hdb->dbh->do("alter session set NLS_DATE_FORMAT = 'YYYY-MM-DD HH24:MI:SS'");

# look for -d flag for testing
my $testing = shift;
my $ftp;

if (defined ($testing) && $testing eq "-d") {
  $testing=1;
} else {
  $testing=0;
  $ftp = Net::FTP->new("d5ftp.usbr.gov")
    or die "Cannot connect to ftp site: $@";
  $ftp->login("agilmore","march28")
    or die "Cannot login ", $ftp->message;
  $ftp->cwd("elpaso")
    or die "Cannot change working directory ", $ftp->message;
}

#many of these HDB queries use the following syntax:
# my $var = @{$hdb->dbh->selectcol_arrayref("query")}[0]
# this is just a fancy way of getting the first item in the array that
# a reference is returned to by the query.
# since no column number is given to the selectcol_arrayref function,
# the first column is returned.
# since all of the queries in this script only return one column, this
# is quite useful. 

#design for data query control via generic mapping
#ext_data_source_name is report name
#rows in ref_ext_site_data_map control which timeseries
#key value in the _keyval table control which column the timeseries occurs in
#each specific primary_site_code is own report file, and site_code is filename
#primary_data_code is column name

my $sitessql = "
select distinct primary_site_code from hdb_ext_data_source a,
ref_ext_site_data_map b 
where ext_data_source_name = 'HDB El Paso Update' and
b.EXT_DATA_SOURCE_ID = a.EXT_DATA_SOURCE_ID";

my $sites = $hdb->dbh->selectcol_arrayref("$sitessql");

foreach my $site (@$sites) {

my $outfile = $site.".dat";
$outfile =~ s/ //g;

open OUTPUT, ">$outfile";

my $headersql = "
select primary_data_code code ,unit_common_name unit
from ref_ext_site_data_map b, ref_ext_site_data_map_keyval c,
 hdb_ext_data_source d,hdb_site_datatype e, hdb_datatype f,hdb_unit g
where
b.mapping_id = c.mapping_id and
c.key_name = 'Column Order' and 
b.ext_data_source_id = d.ext_data_source_id and
d.ext_data_source_name = '$app_name' and
e.site_datatype_id = b.hdb_site_datatype_id and
e.datatype_id = f.datatype_id and
g.unit_id = f.unit_id and
b.primary_site_code = ?
order by key_value";

my $codes = $hdb->dbh->selectall_arrayref("$headersql",{ Slice=>{}},$site);

#want header to have two tabs, then data codes
#header setup data
my $sp = 10;

my $siteheader = " " x (34-length($site)/2) . $site;
my $provoheader .=   "            DATA IS PROVISIONAL AND SUBJECT TO REVISION";
my $codeline ="Date       Time     ";
my $unitline ="                    ";

foreach my $code (@$codes) {
  $codeline .= sprintf("%*.*s",$sp,$sp,"$code->{code}");
  $code->{unit} =~ s/feet/ft/;
  $unitline .= sprintf("%*.*s",$sp,$sp,"($code->{unit})");
}

print OUTPUT $siteheader ."\n";
print OUTPUT $provoheader ."\n";
print OUTPUT $codeline ."\n";
print OUTPUT $unitline . "\n\n";

#get data from last 7 days
# uses a with statement to get all data for the report, then queries it\
# multiple times to get the data in the proper columns
my $datasql =  "
with data as (select start_date_time,primary_site_code,primary_data_code,
key_value, round(value,2) as value
from
 r_instant a, ref_ext_site_data_map b, ref_ext_site_data_map_keyval c,
 hdb_ext_data_source d
where
primary_site_code = ? and
site_datatype_id=hdb_site_datatype_id and
b.mapping_id = c.mapping_id and
c.key_name = 'Column Order' and 
b.ext_data_source_id = d.ext_data_source_id and
d.ext_data_source_name = '$app_name' and
start_date_time between trunc(sysdate - 7,'DD') and sysdate
order by start_date_time)
select
date_time,
lpad(to_char(round(a.value,2)),22) val1,b.value val2,c.value val3,d.value val4
from data a, data b, data c, data d,
table(instants_between(trunc(sysdate - 7,'DD'),sysdate,15)) dates
where
 dates.date_time = a.start_date_time(+) and
 dates.date_time = b.start_date_time(+) and
 dates.date_time = c.start_date_time(+) and
 dates.date_time = d.start_date_time(+) and
 a.key_value(+) = 1 and
 b.key_value(+) = 2 and
 c.key_value(+) = 3 and
 d.key_value(+) = 4
 order by dates.date_time";

#get the email addresses to write to
# first line of this reponse is
# mailto: <emailaddresses>

my $data = $hdb->dbh->selectall_arrayref("$datasql",{},$site);

my @output;

foreach my $row (@$data) {
  my $line = pack("A20",$row->[0]);
  shift (@$row);
  foreach my $field (@$row) {
    if (!defined($field)) {
      $field = sprintf("%*s", $sp,"");
    } elsif ($field < 10000) {
      $field = sprintf("%*.2f", $sp,$field);
    } else {
       $field = sprintf("%*d", $sp,$field);
    }
    $line .=$field;
  }
  $line =~ s/\s+$//;
  push @output, $line;

}

print OUTPUT @output;
print OUTPUT "\n";
close OUTPUT;

  unless ($testing) {
    $ftp->put($outfile)
      or die "put failed ", $ftp->message;
  }
}

$ftp->quit unless $testing;

exit;

sub usage {
  print "Usage: $0 -a hdbaccountfile <-d if testing>\n"; 
  exit 1; 
}


=item DDL/DML to generate metadata to drive this report:

Do we want last query tracking on this one?

insert into hdb_loading_application values(null, 'HDB El Paso Update',null,
'Reporting application for data from r_instant to the ftp site');

insert into ref_loading_application_prop values(??,'Last Run Time',sysdate);

insert into hdb_ext_data_source
values (
 null, 'HDB El Paso Update', 7,null,
null,null,null,null,
'Report definition for querying data from r_instant for ftp site',null
);

select 'exec create_site_data_map ('||
a.ext_data_source_id||','||b.site_common_name||','||
d.datatype_common_name||',1,'||e.site_datatype_id||
',''instant'',null,null,7,''Y'',null,''Column Order=1'');'
from hdb_ext_data_source a, hdb_site b,
hdb_datatype d, hdb_site_datatype e
where 
a.ext_data_source_name = 'HDB El Paso Update' and
e.site_id = b.site_id and
e.datatype_id = d.datatype_id and
d.datatype_id in (16,18,24,63,65) and
e.site_id in (1094,1113,1115,1119);

-- must alter data codes and column order
--Other datatypes?

begin
create_site_data_map (25,'CABALLO','Storage',1,10100,'instant',null,null,7,'Y',null,'Column Order=1');
create_site_data_map (25,'CABALLO','Pool Elevation',1,10101,'instant',null,null,7,'Y',null,'Column Order=2');
create_site_data_map (25,'RIO GRANDE BELOW CABALLO DAM','Flow',1,10106,'instant',null,null,7,'Y',null,'Column Order=3');
create_site_data_map (25,'RIO GRANDE BELOW CABALLO DAM','Shift',1,10105,'instant',null,null,7,'Y',null,'Column Order=2');
create_site_data_map (25,'RIO GRANDE BELOW CABALLO DAM','Gage Height',1,10104,'instant',null,null,7,'Y',null,'Column Order=1');
create_site_data_map (25,'RIO GRANDE BELOW ELEPHANT BUTTE DAM','Flow',1,2527,'instant',null,null,7,'Y',null,'Column Order=3');
create_site_data_map (25,'RIO GRANDE BELOW ELEPHANT BUTTE DAM','Shift',1,10108,'instant',null,null,7,'Y',null,'Column Order=2');
create_site_data_map (25,'RIO GRANDE BELOW ELEPHANT BUTTE DAM','Gage Height',1,10107,'instant',null,null,7,'Y',null,'Column Order=1');
create_site_data_map (25,'ELEPHANT BUTTE DAM','Storage',1,10102,'instant',null,null,7,'Y',null,'Column Order=1');
create_site_data_map (25,'ELEPHANT BUTTE DAM','Pool Elevation',1,10103,'instant',null,null,7,'Y',null,'Column Order=2');
end;

=cut
