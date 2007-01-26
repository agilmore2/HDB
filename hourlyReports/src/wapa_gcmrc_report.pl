#!/usr/bin/perl

use strict;
use warnings;

use English '-no_match_vars';

my $numdays = $ARGV[0];
my $testing = $ARGV[1];
my $status = 0;

my ($app,$sqlfile, $sqlout, @output, $mail);

my $to = 'telschow@wapa.gov,patno@wapa.gov,loftin@wapa.gov';
my $cc = 'agilmore@uc.usbr.gov,rclayton@uc.usbr.gov,pdavidson@uc.usbr.gov';


#handle bogus mail transfer programs. In Linux, mail is the more featureful,
# and in Solaris, mailx is.
if ($OSNAME eq 'linux') {
  $mail = "mail";
} elsif ($OSNAME eq 'solaris') {
  $mail = "mailx";
}

if (defined ($testing) && $testing eq "-d") {
  $mail = 'cat';
} else {
  $testing=0;
}

if (!$numdays) {
  print "Usage: wapa_gcmrc_report.pl <number of days to look back>\n";
  exit 1;
}
elsif ($numdays =~ /\D/ ) {
  print "ERROR!: Argument for days to look back: $numdays is not a integer.\n";
  print "Usage: wapa_gcmrc_report.pl <number of days to look back>\n";
  exit 1;
}

# run the sql script
# it creates a wapa_<lookbackdate>.dat file and
# optionally a wapa_update_<lookbackdate>.dat file if there were any updates
# if there were updates, it exits with a exit code = 99;

$sqlout = `sqlplus -S -L app_user/uchdb2\@uchdb2 \@wapa_scada_daily.sql $numdays`;
$status = $?;

# taken from 'perldoc -f system'
if ($status == -1) {
  print "failed to execute sqlplus!: $!\n";
}
elsif ($status & 127) {
  printf "sqlplus died with signal %d\n",
  ($status & 127);
} else {
  $status = $status >>8; #shift by 8 bits to get to actual signal value
  if ($status > 0) {
    die "sqlplus failed! $status, $sqlout $!\n";
  } # else no error
}

#for testing
#$sqlout="20061116\n";
#$status=99;

print $sqlout;
chomp $sqlout;

#read all lines from daily file
open INFILE, "<wapa_$sqlout.dat" or die "unable to open wapa output file: wapa_$sqlout.dat $!\n";
@output = <INFILE>;
close INFILE;

#mail this data to WAPA
my $mailcmd = $mail;
if (!$testing) {
  #grep for GLEN, and ftp it to GCMRC
  my @glen = grep {/GLEN/} @output;
  open GLEN, "|ncftpput -f gcqaccount.dat -c glen_$sqlout.dat"
  or die "ftp failed! $!";

  #open GLEN, ">glen_$sqlout.dat";
  print GLEN @glen;
  close GLEN;

  #set up mail command
  my $subject = "\"WAPA SCADA Daily $sqlout\"";
  $mailcmd = $mailcmd ." -s ". $subject . " -c ".$cc." ".$to;
}

open MAIL, "|$mailcmd" or die "unable to open mail: $!\n";
print MAIL @output;
close MAIL;

unlink "wapa_$sqlout.dat";


#check if we're not testing, if so, repeat the above with a different
# SQL script, subject and filename
if (!$testing) {
  # run the sql script
  # if there were updates, it exits with a exit code = 99.
  # and creates a wapa_update_<lookbackdate>.dat file if there were any updates

  $sqlout = `sqlplus -S -L app_user/uchdb2\@uchdb2 \@wapa_scada_updates.sql $numdays`;
  $status = $?;

  chomp $sqlout;

  # taken from 'perldoc -f system'
  if ($status == -1) {
    print "failed to execute sqlplus!: $!\n";
  } elsif ($status & 127) {
    printf "sqlplus died with signal %d\n",
    ($status & 127);
  } else {
    $status = $status >>8;      #shift by 8 bits to get to actual signal value
    if ($status > 0 and $status !=99) {
      die "sqlplus failed! $status, $sqlout $!\n";
    }                           # else no error
  }
  exit 0 if $status == 0; # if no updates, done!

  open INFILE, "<wapa_update_$sqlout.dat" or die "unable to open wapa output file! $!\n";
  @output = <INFILE>;
  close INFILE;
  my $subject = "\"WAPA SCADA Update $sqlout\"";

  $mailcmd = $mail ." -s ". $subject . " -c ".$cc." ".$to;
  open MAIL, "|$mailcmd" or die "unable to open mail: $!\n";
  print MAIL @output;
  close MAIL;

  # do not want to delete the output of the updates.
  #  unlink "wapa_update_$sqlout.dat";

  my @glen = grep {/GLEN/} @output;

  open GLEN, "|ncftpput -f gcqaccount.dat -c glen_update_$sqlout.dat" 
  or die "ftp failed! $!";

  #open GLEN, ">glen_update_$sqlout.dat";
  print GLEN @glen;
  close GLEN;
}

exit 0;
