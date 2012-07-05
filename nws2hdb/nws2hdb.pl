#!/usr/bin/perl -w

use warnings;
use strict;
use LWP::Simple;
use DBI;
use Date::Calc qw(Add_Delta_DHMS);
my ($num,$line,$value,$load,$site_datatype_id_in,$start_date_time_in,$end_date_time);
my (@toks,$toks1,$toks2,$toks3,@toks2,$toks20,$toks21,$toks22,@toks3,$toks30,$toks31,$toks32);
my ($year2,$month2,$day2,$hour2);

#Create connection to database
my $name = 'dbi:Oracle:ecohdb';
my $user = 'ecodba';
my $pass = '<PASSWORD>';
my $dbh = DBI->connect($name,$user,$pass,{AutoCommit=>0}) || die "Cannot connect to $name\n";
$dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD HH24'")
  or $dbh->hdbdie("Unable to set NLS_DATE_FORMAT to ISO standard\n");
my $get_nws = "SELECT DESCRIPTION FROM HDB_EXT_DATA_SOURCE WHERE EXT_DATA_SOURCE_ID = 4";
my $arrayref = $dbh->selectall_arrayref($get_nws);
my @all_array=@$arrayref;
my @row_array=@{$all_array[0]};
my $url=$row_array[0];
my $ver = '&version=';
for $num (0 .. 1)
{
my $urltot = "$url$ver$num";
my $file1 = "$ENV{HDB_ENV}/nws2hdb/temp1.html";
my $file2 = "$ENV{HDB_ENV}/nws2hdb/temp2.shef";
my $get = getstore($urltot, $file1);
die "Error $get on $urltot" unless is_success($get);
open(HTML, "<$file1") || die "Can't open $file1\n";
open(SHEF, ">$file2") || die "Can't open $file2\n";
foreach $line (<HTML>)
{
   chomp($line);
   $line =~ s/<br>/\n/g;
   $line =~ s/^  *//g;
   $line =~ s/^  *//g;
   print SHEF "$line\n";
}
close(HTML);
close(SHEF);
my $var="$ENV{HDB_ENV}/nws2hdb/test.shef";
chdir "$ENV{HDB_ENV}/nws2hdb";
system("$ENV{HDB_ENV}/nws2hdb/shefit $file2 > $var");
open(DATA,"<$var") || die "Unable to open $var: $!";
my @var = <DATA>;
foreach (@var)
{
    chomp$_;
    @toks = split(/ /,$_);
    $toks1 = $toks[0];                        #Station ID
    if($toks1 eq 'ESSC2')
    {   
    #site_datatype_id_in, for estes lake inflow (ESSC2) = 100252
    $site_datatype_id_in = "100252";
    }   
    elsif($toks1 eq 'DKKC2')
    {   
    #site_datatype_id_in, for drake river gage (DKKC2) = 100441
    $site_datatype_id_in = "100441"
    }   
    $toks2 = $toks[5];                        #YYYY-MM-DD
    $toks3 = $toks[6];                        #Time HH:MM:SS 
    $value = $toks[21]*1000;                  #cfs
    @toks2 = split(/-/,$toks[5]);
    $toks20 = $toks2[0];                      #YYYY   
    $toks21 = $toks2[1];                      #MM  
    $toks22 = $toks2[2];                      #DD
    @toks3 = split(/:/,$toks[6]);
    $toks30 = $toks3[0];                      #Time HH
    $toks31 = $toks3[1];                      #Time MI
    $toks32 = $toks3[2];                      #Time SS
    ($year2,$month2,$day2,$hour2) = Add_Delta_DHMS($toks20,$toks21,$toks22,$toks30,$toks31,$toks32,0,-6,0,0);
    $start_date_time_in = $year2."-".$month2."-".$day2." ".$hour2; #YYYY-MM-DD HH MDT TZONE
    $end_date_time = $toks2." ".$toks30;
    $load = $dbh->prepare("BEGIN   
	      MODIFY_M_TABLE_RAW(?,?,?,?,?,?,?);  
	      END;");
    $load->bind_param(1,"1");
    $load->bind_param(2,$site_datatype_id_in);
    $load->bind_param(3,$start_date_time_in);
    $load->bind_param(4,$end_date_time);
    $load->bind_param(5,$value);
    $load->bind_param(6,"hour");
    $load->bind_param(7,"Y");
    $load->execute;
#   print "$toks1,$site_datatype_id_in,$start_date_time_in,$end_date_time,$value\n";
}
$dbh->commit;
close(DATA);
system("rm -f $file2; rm -f $file1; rm -f $var");
print "Done Loading NWSRFS Forecast for Station $toks1 on $toks2\n";
}
