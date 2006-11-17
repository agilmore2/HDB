#!/usr/bin/perl

use strict;
use warnings;

use English '-no_match_vars';

my $numdays = $ARGV[0];
my $status = 0;

my ($app,$sqlfile, $sqlout, @output, $mail);

my $to = 'frantz@wapa.gov,telschow@wapa.gov';
my $cc = 'agilmore@uc.usbr.gov,rclayton@uc.usbr.gov,pdavidson@uc.usbr.gov';

#handle bogus mail transfer programs. In Linux, mail is the more featureful,
# and in Solaris, mailx is.
if ($OSNAME eq 'linux') {
  $mail = "mail";
} elsif ($OSNAME eq 'solaris') {
  $mail = "mailx";
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

$sqlout = `sqlplus -S -L app_user/uchdb2\@uchdb2 \@wapa_scada_updates.sql $numdays`;
$status = $?;

die "sqlplus failed! $sqlout $!\n" if ($status > 0 and $status !=99);


#for testing
#$sqlout="20061116\n";
#$status=99;

print $sqlout;
chomp $sqlout;

#read all lines from daily file
open INFILE, "<wapa_$sqlout.dat" or die "unable to open wapa output file: wapa_$sqlout.dat $!\n";
@output = <INFILE>;
close INFILE;

my $subject = "\"WAPA SCADA Daily $sqlout\"";

#mail this data to WAPA
my $mailcmd = $mail ." -s ". $subject . " -c ".$cc." ".$to;
open MAIL, "|$mailcmd" or die "unable to open mail: $!\n";
#open MAIL, '>test.out' or warn "$!";;
print MAIL @output;
close MAIL;

unlink "wapa_$sqlout.dat";

#grep for GLEN, and ftp it to GCMRC
my @glen = grep {/GLEN/} @output;
open GLEN, "|ncftpput -f gcqaccount.dat -c glen_$sqlout.dat"
     or die "ftp failed! $!";

#open GLEN, ">glen_$sqlout.dat";
print GLEN @glen;
close GLEN;

#check if updates were found, if so, repeat the above with a different
# subject and filename
if ($status == 99) {
  open INFILE, "<wapa_update_$sqlout.dat" or die "unable to open wapa output file! $!\n";
  @output = <INFILE>;
  close INFILE;
  my $subject = "\"WAPA SCADA Update $sqlout\"";

  my $mailcmd = $mail ." -s ". $subject . " -c ".$cc." ".$to;
  open MAIL, "|$mailcmd" or die "unable to open mail: $!\n";
  #open MAIL, '>test_update.out' or warn "$!";;
  print MAIL @output;
  close MAIL;

  unlink "wapa_update_$sqlout.dat";

  @glen = grep {/GLEN/} @output;

  open GLEN, "|ncftpput -f gcqaccount.dat -c glen_update_$sqlout.dat" 
     or die "ftp failed! $!";

  #open GLEN, ">glen_update_$sqlout.dat";
  print GLEN @glen;
  close GLEN;
}

exit 0;
