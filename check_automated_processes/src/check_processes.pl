#!/usr/bin/perl -w

use strict;

# check on all the crontab applications
open INFILE, 'automatedapps';

my ($app,$errfile,$expectfile,$cmdname,$ps,$line,@dirent,$HDB_ENV);

my $status = 0;
my $subject = "Subject: Failure Detected in Automated uchdb2 Process\n";
my $output;

while (<INFILE>)
{
  chomp;
  ($app,$errfile) = split '=';

  $HDB_ENV = $ENV{'HDB_ENV'};

  $errfile =~ s /\$HDB_ENV/$HDB_ENV/e;

  if (! (@dirent = stat($errfile))) {
    $output .= "\nFile not found, '$errfile'\n";
    $status = 1;

    next;
  }
  #  print "@dirent\n";

  if ($dirent[7] != 0) {
    $expectfile = $errfile . ".expected";
    if (stat($expectfile)) {
      if (`diff $errfile $expectfile`) {
        $status = 1;
        $output .= "\nApplication $app had an error! $errfile is not as expected\n";
        open ERRFILE, $errfile;
        $output .= <ERRFILE>;
      }
    } else {
      $status = 1;
      $output .= "\nApplication $app had an error! $errfile is not empty\n";
      open ERRFILE, $errfile;
      $output .= <ERRFILE>;
    }
  }
}

close INFILE;

# check on the realtime applications
open INFILE, 'realtimeapps';
while (<INFILE>)
{
  chomp;
  ($app,$cmdname) = split '=';
  $ps = `ps -fuuchdba|grep $cmdname|grep -v grep`;
  if (!$ps) {
    $output .= "\nRealtime application down: $app.\n";
    $status = 1;
  }
}

if ($status) {
  open MAIL, "|mail $ENV{HDB_XFER_EMAIL}";
  print MAIL $subject, $output;

  close MAIL;
}

exit $status;
