#!/usr/bin/perl

use strict;
use warnings;
use English '-no_match_vars';

$OUTPUT_FIELD_SEPARATOR="\n";

#use libraries from HDB environment (Solaris only except for HDB.pm)
use lib "$ENV{PERL_ENV}/lib";
# the previous line was modified by M. Bogner to allow program to compile and run on ISIS


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

my $date = shift;
chomp $date;

if (not defined($date) or not ($date =~ /^\d{8}$/)) {
  usage();
  exit 1;
}

# run the sql script
# it creates a wapa_<lookbackdate>.dat file and
# optionally a wapa_update_<lookbackdate>.dat file if there were any updates
# if there were updates, it exits with a exit code = 99;
my $data = $hdb->dbh->selectcol_arrayref("
select
 to_char(a.start_date_time,'YYYY-MM-DD,HH24:MI:SS')||','||
 b.primary_site_code||','||b.primary_data_code||','||
 round(a.value,2) as data
from
 r_hour a, ref_ext_site_data_map b, hdb_ext_data_source c
where
 trunc(a.start_date_time,'DD') = to_date($date,'YYYYMMDD') and
 b.hdb_site_datatype_id = a.site_datatype_id and
 b.ext_data_source_id = c.ext_data_source_id and
 c.ext_data_source_name = '$app_name' 

 order by b.primary_site_code, b.primary_data_code, a.start_date_time");

open OUTPUT, ">wapa_$date.dat";
print OUTPUT @$data;
close OUTPUT;
 
exit 0;

sub usage {
  print "Usage: $0 -a hdbaccountfile YYYYMMDD\n"; 
  exit 1; 
}
