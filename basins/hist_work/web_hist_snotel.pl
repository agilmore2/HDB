#!/usr/bin/perl -w
#
# This perl program modified from original by Mark Bogner December 2013
# modification was to change the processing to utilize the web pages for historical data
# rather than the FTP site which is suppose to go away
# Fixed year value over year boundries, by Ismail Ozdemir - 01/04/2018

use Date::Calc qw(:all);

my ($temp, $line, $value, $date, @fields);
my $old = 0;

my @current_date = Today(); #or (2001,8,1); for August 1, 2001

my $numdays = 10;

if (@ARGV and scalar(@ARGV) == 1 and $ARGV[0] =~ /\d+/) {
  $numdays = $ARGV[0];
} else {
  print 
"Usage: program (uses $numdays as default period) or
       program <number_of_periods>\n";
exit (1);
}


my @old_date = Add_Delta_Days(@current_date, -$numdays);

#my $wy;


# M. Bogner Dec 2013 -- modified the loop to also do todays retrieval so that a 0 can be passed into this script
while (Delta_Days(@old_date, @current_date) >= 0)
{
# new_get_load_hist_snotel.sh requires the month name, day and year for every historical retrieval request
  my $date = sprintf("%02d/%02d/%02d",$old_date[1],$old_date[2],
             substr($old_date[0], -2));
# get the day and month from the old_date
  my $day = $old_date[2];
  my $month = Month_to_Text($old_date[1]);
  my  $year = $old_date[0];  
  
  print "$month $day $year $date\n";
  system("./web_get_load_hist_snotel.sh $month $day $year");
  @old_date = Add_Delta_Days(@old_date, 1);

}

