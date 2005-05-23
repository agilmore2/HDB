#!/usr/bin/perl -w

use Expect;

#use a 15 second timeout
my $timeout = 15;

my $exp = Expect->spawn("ssh zippy -i /wrg/home/agilmore/.ssh/hydrometid");

# turn off echoing of stdout
$exp->log_stdout(0);

#wait for prompt
$exp->expect($timeout,'$ ');

#enter dayfile program, login.com on vms side has definition
$exp->send("dayfile\r");

# wait for "DAYFILES >" prompt
$exp->expect($timeout,"DAYFILES >");

# send query
$exp->send("get $ARGV[0]/$ARGV[1]\r");

# wait for result
# will be either data, or no data found
my $result = $exp->expect($timeout,
             uc($ARGV[0]).'    |',
             '%W-Dayfile, no data found for get request: '
            );

$exp->send("exit\r");
$exp->send("lo\r");
$exp->soft_close();

if ($result == 1) {
  print "$ARGV[0]/$ARGV[1] Data Found!\n";
  exit(0);
}
elsif ($result == 2) {
  print "$ARGV[0]/$ARGV[1] Data Not Found!\n";
  exit (1);
}
else {
  print "$ARGV[0]/$ARGV[1] Information Unclear!\n";
  exit (2);
}
