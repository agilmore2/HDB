#!/usr/bin/perl -w

use strict;
use English;

my ($app,$errfile,$expectfile,$cmdname,$ps,$line,@dirent,$HDB_ENV);
my $DEBUG = 0;
my $status = 0;
my $subject = "Failure Detected in Automated $ENV{'HDB_LOCAL'} Process";
my ($output, $result);

# check on the realtime applications
open INFILE, 'realtimeapps' or die "can't open input file: $!";
while (<INFILE>)
{
  next if (/^#/);

  chomp;
  ($app,$cmdname) = split '=';

  if ($cmdname =~ /^\`/) { 
    #run command specified, any number in the output other than 0 is success
    $line = eval $cmdname;
    chomp $line;
    if ($line == 0) { # wc returned a zero
	$output .= "\nRealtime application down: $app\n";
	$status = 1;
    }
  } else {
    $ps = `ps -fu$ENV{'USER'} |grep $cmdname|grep -v grep`;
    if (!$ps) {
      $output .= "\nRealtime application down: $app\n";
      $status = 1;
    }
  }
}

close INFILE;

# check on all the crontab applications
open INFILE, 'automatedapps' or die "can't open input file: $!";

while (<INFILE>)
{
  next if (/^#/);

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
        $output .= "
Application error   : $app
File not as expected: $errfile
as of               : $errfiletime\n";
        $output .= $result;
      }
    } else {
      $status = 1;
      $output .= "
Application error   : $app
File is not empty   : $errfile
as of               : $errfiletime\n";
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

if ($status && !$DEBUG) {
  open MAIL, "|mail -s '$subject' $ENV{HDB_NOTIFY_EMAIL}";
  print MAIL $output;

  close MAIL;
}

if ($status && $DEBUG) {
  print $output;
}

exit $status;
