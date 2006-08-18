#!/usr/local/bin/perl -w

use strict;
use File::Basename;

my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my ($polldir, $pattern, @files, $file);

while (@ARGV)
{
  my $arg = shift(@ARGV);
  if ($arg =~ /-h/) {	        # print help
    usage();
  } elsif ($arg =~ /-v/) {	# print version
    version();
  } elsif ($arg =~ /-u/) {	# get hdb user
    $hdbuser=shift(@ARGV);
  } elsif ($arg =~ /-p/) {	# get hdb passwd
    $hdbpass=shift(@ARGV);
  } elsif ($arg =~ /-f/) {	# get filename pattern
    $pattern = shift(@ARGV);
  } elsif ($arg =~ /-d/) {	# get file to read from
    $polldir = shift(@ARGV);
    if (! -d $polldir) {
      print "directory not found: $polldir\n";
      usage();
      exit 1;
    }
  }
}

if (!defined($hdbuser) || !defined($hdbpass)) {
  print "Error! No user or password!\n";
  usage();
}

if (!defined($polldir)) {
  print "Must specify directory to watch!\n";
  usage();
}

if (!defined($pattern)) {
  print "Must specify filename pattern to watch for!\n";
  usage();
}


#run this loop forever
while (1) {
#check the directory specified for files beginning with $pattern
  opendir(DIR, $polldir) || die "opendir of $polldir failed: $!";
  @files = grep { /^$pattern/ && -f "$polldir/$_" } readdir(DIR);
  closedir DIR;

  #if any new files, run the loading script against them, and then
  #move them to the archives subdirectory
  if (@files) {
    foreach $file (@files) {
      my @program=("perl","../src/scada2hdb.pl","-u","app_user","-p","uchdbdev","-f","$polldir/$file");
      system (@program) == 0 or die "Failed to run scada2hdb.pl!\n $!";
      system ("mv","-f","$polldir/$file","$polldir/archives/$file") == 0 or
         die "Failed to move file: $file\n$!\n";
#      print "$file\n";
    }
  }
  sleep 60;
}

sub usage
{
  print STDERR <<"ENDHELP";
$progname [ -h | -v ] | [ options ]
Watches a specified directory for filenames beginning with a pattern every 60
seconds. Runs scada2hdb.pl on matching files, then moves the file to the
archives subdirectory.
Example: $progname -d <directory> -u <user_name> -p <hdbpassword> -f <pattern>

  -h               : This help
  -v               : Version
  -u <hdbusername> : HDB application user name (REQUIRED)
  -p <hdbpassword> : HDB password (REQUIRED)
  -f <filename pattern>    : file pattern to watch for (REQUIRED)
  -d <directory>   : directory to watch for files with right pattern <REQUIRED>
ENDHELP

  exit (1);
}

sub version
{
  print "$progname version $verstring\n";
  exit 1;
}
