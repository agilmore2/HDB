#!/usr/bin/perl -w

use strict;

my $number_of_days = $ARGV[0];
my $status = 0;

my ($app,$sqlfile, $output);
my $subject = "Subject: Data Consistency Check Failed!\n";


if (!$ARGV[0]) {
  print "Usage: check_consistency.pl <number of days to look back>\n";
  exit 1;
}
elsif ($ARGV[0] =~ /\D/ ) {
  print "ERROR!: Argument for days to look back was not a integer.\n";
  print "Usage: check_consistency.pl <number of days to look back>\n";
  exit 1;
}

# check each sql script
open INFILE, 'sqlscripts';

while (<INFILE>)
{
  chomp;
  ($app,$sqlfile) = split '=';

  my $sqlout = `sqlplus -S -L app_user/uchdb2 \@$sqlfile $number_of_days`;
  my $result = $?;

  if ($result) {
# delete annoying leading newline in sqlplus output
    $sqlout = substr $sqlout,1;
    $output .= "\n$app consistency check failed:\n";
    $output .= $sqlout;
    $status = $result;
  }
}

if ($status) {
  open MAIL, "|mail $ENV{HDB_XFER_EMAIL},rclayton\@uc.usbr.gov";
  print MAIL $subject, $output;

  close MAIL;
}


exit $status;
