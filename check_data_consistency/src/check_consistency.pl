#!/usr/bin/perl -w

use strict;

my $numdays = $ARGV[0];
my $status = 0;

my ($app,$sqlfile, $output);
my $subject = "Subject: $numdays Day Data Consistency Check Failed!\n";


if (!$numdays) {
  print "Usage: check_consistency.pl <number of days to look back>\n";
  exit 1;
}
elsif ($numdays =~ /\D/ ) {
  print "ERROR!: Argument for days to look back: $numdays is not a integer.\n";
  print "Usage: check_consistency.pl <number of days to look back>\n";
  exit 1;
}

# check each sql script
open INFILE, 'sqlscripts';

while (<INFILE>)
{
  chomp;
  ($app,$sqlfile) = split '=';

  my $sqlout = `sqlplus -S -L app_user/uchdb2 \@$sqlfile $numdays`;
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
  open MAIL, "|mail -t $ENV{HDB_NOTIFY_EMAIL}";
  print MAIL $subject, $output;

  close MAIL;
}


exit $status;
