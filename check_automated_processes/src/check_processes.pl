#!/usr/bin/perl -w

use strict;
use English;

# check on all the crontab applications
open INFILE, 'automatedapps' or die "can't open input file: $!";

my ($app,$errfile,$expectfile,$cmdname,$ps,$line,@dirent,$HDB_ENV);
my $DEBUG = 1;
my $status = 0;
my $subject = "Subject: Failure Detected in Automated uchdb2 Process\n";
my ($output, $result);

while (<INFILE>)
{
  next if (~/^#/);

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
    my $errfiletime = localtime $dirent[9];
    $expectfile = $errfile . ".expected";
    if (stat($expectfile)) {
      $result = `diff -u $expectfile $errfile`;
      if ($?) {
        $status = 1;
        $output .= "\nApplication $app had an error! $errfile is not as expected as of $errfiletime\n";
        $output .= $result;
      }
    } else {
      $status = 1;
      $output .= "\nApplication $app had an error!\n$errfile is not empty as of $errfiletime\n";
      #Make this file get sucked in completely, don't break on newlines
      my $oldsep = $INPUT_RECORD_SEPARATOR;
      $INPUT_RECORD_SEPARATOR = undef;

      open ERRFILE, $errfile;
      $output .= <ERRFILE>;

      #reset the record separator
      $INPUT_RECORD_SEPARATOR = $oldsep;
    }
  }
}

close INFILE;

# check on the realtime applications
open INFILE, 'realtimeapps' or die "can't open input file: $!";
while (<INFILE>)
{
  next if (~/^#/);

  chomp;
  ($app,$cmdname) = split '=';
  $ps = `ps -fuuchdba|grep $cmdname|grep -v grep`;
  if (!$ps) {
    $output .= "\nRealtime application down: $app.\n";
    $status = 1;
  }
}

if ($status && !$DEBUG) {
  open MAIL, "|mail -t $ENV{HDB_NOTIFY_EMAIL}";
  print MAIL $subject, $output;

  close MAIL;
}

if ($status && $DEBUG) {
  print $output;
}

exit $status;
