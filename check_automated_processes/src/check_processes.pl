#!/usr/bin/perl -w
use strict;

open INFILE, 'automatedapps';

my ($app,$errfile,$line,@dirent,$HDB_ENV);
while (<INFILE>)
{
  chomp;
  ($app,$errfile) = split '=';

  $HDB_ENV = $ENV{'HDB_ENV'};


  $errfile =~ s /\$HDB_ENV/$HDB_ENV/e;

#  print "$app, $errfile\n";

  if (! (@dirent = stat($errfile))) {
    print "File not found, '$errfile'\n";
    next;
  }
#  print "@dirent\n";


  if ($dirent[7] != 0) {
   print "Application $app had an error! $errfile is not empty\n";
 }
}
