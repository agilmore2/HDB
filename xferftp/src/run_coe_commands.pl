#!/usr/bin/perl -w

use Expect;

#use a 15 second timeout
my $timeout = 15;

my $exp = Expect->spawn("ssh zippy -l hydromet");

# turn off echoing of stdout
#$exp->log_stdout(0);

#wait for prompt
$exp->expect($timeout,'>-Hydromet-> ');

#run hour.com from COE
$exp->send("\@hour\r");

# wait for "command prompt again" prompt
$exp->expect($timeout,">-Hydromet-> ");

# run data.com from COE
$exp->send("\@data\r");

# wait for "command prompt again" prompt
$exp->expect($timeout,">-Hydromet-> ");

$exp->send("lo\r");
$exp->soft_close();

exit(0);
