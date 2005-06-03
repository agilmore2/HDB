#!/usr/local/bin/perl -w

use Date::Calc qw(:all);

my ($temp, $line, $value, $date, @fields);
my $old = 0;

my @current_date = (1997,9,30); #or (2001,8,1); for August 1, 2001

my @old_date = Add_Delta_Days(@current_date, -364);

my $wy;
if ($current_date[1] < 10) {
  $wy = $current_date[0];
} else {
  $wy = $current_date[0] + 1;
}

while (Delta_Days(@old_date, @current_date) >= 0)
{
  my $date = sprintf("%02d/%02d/%02d",$old_date[1],$old_date[2],
	  substr($old_date[0], -2));
  print "$wy $date\n";
#  system("./get_load_hist_snotel.sh $wy $date");
  @old_date = Add_Delta_Days(@old_date, 1);
}

