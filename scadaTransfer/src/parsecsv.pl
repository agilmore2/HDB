#!/usr/local/bin/perl

use Date::Calc qw(Month_to_Text);
use File::Basename;
use FileHandle;
use Data::Dumper;
use strict;
use warnings;

my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my $insertflag = 1;
my $overwrite = 'null';

my (@value_date, @file_date, @data_date);
my ($debug,$readfile, $newdate, $newmonth, $readdir);
my %datefiles;

#======================================================================
#parse arguments
while (@ARGV)
{
  my $arg = shift(@ARGV);
  if ($arg =~ /-h/) {	        # print help
    &usage;
  } elsif ($arg =~ /-v/) {	# print version
    &version;
  } elsif ($arg =~ /-f/) {	# get file to read from
    $readfile = shift(@ARGV);
    if (! -e $readfile) {
      print "file not found: $readfile";
      exit 1;
    }
  } elsif ($arg =~ /-.*/) {	# Unrecognized flag, print help.
    print STDERR "Unrecognized flag: $arg\n";
    &usage;
  } else {
    print STDERR "Unrecognized command line argument: $arg\n";
    &usage;
  }
}

if (!defined($readfile)) {
  print "Error! No file was specified!\n";
  usage();
}


=FORMAT

The filename is expected to be csv_YYYY_MM_DD_HH_MI_SS, where the date is the
date the file was created.
The data is expected in the following format, one value per line, 6000 lines:

YYYY,MM,DD,HH,MI,SITE,datatatype_name,value,somevalidationcode

which is 9 fields.

The file also contains data on individual units:

YYYY,MM,DD,HH,MI,SITE,UNIT,datatype_name,value,somevalidationcode

which is 10 fields.

We will make individual files, one per day of data with the
filename reflecting the date of the data in the file, not the date the file was
created.

These files will be named "crsp_YYYMONDD.dat"

The twist is that each file must contain data from 01..23 hours plus the data
for 00 hours the next day.

=cut

my ($line, $datestr, @fields, @prevdate, $sitecode);
my ($value, $head, $rel, $seenspill);

# get date on input file
open (INFILE, "$readfile") || die "Error: couldn't open input file $ARGV[0]";

$readdir = dirname($readfile);

# get date of file
$datestr = substr $readfile, index($readfile,"csv_")+4;

(@file_date = split(/_/,$datestr)) ||
  die "There was an error parsing the date in the file name!.\n";

print "@file_date\n";

READ: while ($line = <INFILE>)
{
#  print $line if defined($debug);
  
  chomp($line);
  @fields=split(/,/,$line);

  if ($fields[5] eq "UPPER") { # do not read Upper and Lower Molina data
    # individual unit data follows these, don't want that either
     last READ;
  }

  unless ($fields[6] =~ /elev/ or
          $fields[6] =~ /power/ or
          $fields[6] =~ /turbine/ or
          $fields[6] =~ /spillway/ or
          $fields[6] =~ /hollow_jet/) {
    next READ;
  }

#value_date is an array, year, month, day, hour, minute, second
  @value_date=@fields[0..4]; #get first 5 fields from line and
  $value_date[5]=0; #zero seconds field

  if (!defined($data_date[0]) # if this is the first time through
       or ($value_date[3] >= 1 and # or hour is 1 or more and date is different
           ($value_date[0] != $data_date[0] or
           $value_date[1] != $data_date[1] or
           $value_date[2] != $data_date[2]))) {
    #make upper case version of first three letters of month name
    $newmonth=uc substr Month_to_Text($value_date[1]),0,3;

    $newdate=join('',$value_date[0],$newmonth, $value_date[2]);
    if (!defined($datefiles{$newdate})) {
      my $newfile = "$readdir/crsp_$newdate.dat";
      unlink ($newfile);
      $datefiles{$newdate} =  new FileHandle "> $newfile" or
        die "unable to open $newfile";
    }
    @data_date = @value_date;
#    print $newdate . "\n";
  }

  $datefiles{$newdate}->print(join(',',@fields)."\n");
}

foreach (values(%datefiles)) {
  close;
}

sub usage
{
  print STDERR <<"ENDHELP";
$progname [ -h | -v ] | [ options ]
Parses csv file and writes files containing only interesting data
with a filename reflecting the date of data in the file
Example: $progname -f <csvfile>

  -h               : This help
  -v               : Version
  -f <filename>    : file to read from (REQUIRED)
ENDHELP

  exit (1);
}

sub version
{
  print "$progname version $verstring\n";
  exit 1;
}
