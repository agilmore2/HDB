#!/usr/bin/perl -w
# the line below was chnaged to the above by M. Bogner for the move to isis
#
##!/usr/local/bin/perl -w

use Date::Calc qw(:all);

my ($temp, $line, $value, $date, @fields);
my $old = 0;

my @current_date = Today(); #or (2001,8,1); for August 1, 2001

my $numdays = 10;

if (defined(@ARGV) and scalar(@ARGV) == 1 and $ARGV[0] =~ /\d+/) {
  $numdays = $ARGV[0];
} else {
  print 
"Usage: program (uses $numdays as default period) or
       program <number_of_periods>\n";
exit (1);
}


my @old_date = Add_Delta_Days(@current_date, -$numdays);

my $wy;
if ($old_date[1] < 10) {
  $wy = $old_date[0];
} else {
  $wy = $old_date[0] + 1;
}

while (Delta_Days(@old_date, @current_date) > 0)
{
  my $date = sprintf("%02d/%02d/%02d",$old_date[1],$old_date[2],
	  substr($old_date[0], -2));
  print "$wy $date\n";
  system("./get_load_hist_snotel.sh $wy $date");
  @old_date = Add_Delta_Days(@old_date, 1);

  if ($old_date[1] == 10 && $old_date[2] == 1) {$wy = $old_date[0] + 1;}
}

