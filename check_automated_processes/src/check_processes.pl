#!/usr/bin/perl -w

use strict;

# check on all the crontab applications
open INFILE, 'automatedapps';

my ($app,$errfile,$expectfile,$cmdname,$ps,$line,@dirent,$HDB_ENV);
while (<INFILE>)
{
  chomp;
  ($app,$errfile) = split '=';

  $HDB_ENV = $ENV{'HDB_ENV'};


  $errfile =~ s /\$HDB_ENV/$HDB_ENV/e;

  if (! (@dirent = stat($errfile))) {
    print "File not found, '$errfile'\n";
    next;
  }
  #  print "@dirent\n";

  if ($dirent[7] != 0) {
    $expectfile = $errfile . ".expected";
    if (stat($expectfile)) {
      if (`diff $errfile $expectfile`) {
        print "Application $app had an error! $errfile is not as expected\n";
        open ERRFILE, $errfile;
        print <ERRFILE>;
      }
    } else {
      print "Application $app had an error! $errfile is not empty\n";
      open ERRFILE, $errfile;
      print <ERRFILE>;
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
    print"Realtime application $app down!!!\n";
  }
}

