#!/usr/bin/perl

use strict;
use warnings;

use Date::Calc qw (Add_N_Delta_YMDHMS);

my @date = (12,3,1,00);

my ($strdate, $realstrdate);
while ($date[0] <15) {
my @realdate = @date;
$realdate[0] = "20".$date[0];

#date format is YYMMDDHH

#subtract one hour to get to start_date_time
my ($yr,$mm,$dd,$hr) = Add_N_Delta_YMDHMS(@date,0,0,0,0,0,-1,0,0);
my ($ryr,$rmm,$rdd,$rhr) = Add_N_Delta_YMDHMS(@realdate,0,0,0,0,0,-1,0,0);

#make the date back with zero padding
$strdate=sprintf("%02s%02s%02s%02s",$yr,$mm,$dd,$hr);
$ryr = substr($ryr,2,2);
$realstrdate=sprintf("%02s%02s%02s%02s",$ryr,$rmm,$rdd,$rhr);

if ($realstrdate ne $strdate) {
    print "$realstrdate $strdate\n";
}
#add an hour
($yr,$mm,$dd,$hr) = Add_N_Delta_YMDHMS(@date,0,0,0,0,0,1,0,0);
@date = ($yr,$mm,$dd,$hr);

}
print "$realstrdate $strdate\n";
