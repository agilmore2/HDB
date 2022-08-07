#!/usr/bin/perl

use warnings;
use strict;
use File::Basename;
use File::Copy "mv";
use Date::Calc qw(Today Delta_Days);

my $verstring = '$Revision$ ';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

my $appdir = $ENV{"HDB_ENV"};

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my ($polldir, $archivedir, $dubiousdir, $pattern, $dubdays, @files, $file, $accountfile);
my (@crspfiles, $crspfile, $newcrspfile, @program, @problems);

while (@ARGV)
{
  my $arg = shift(@ARGV);
  if ($arg =~ /-h/) {	        # print help
    usage();
  } elsif ($arg =~ /-v/) {	# print version
    version();
  } elsif ($arg =~ /-a/) {	# get hdb login file
    $accountfile=shift(@ARGV);
  } elsif ($arg =~ /-f/) {	# get csv filename pattern
    $pattern = shift(@ARGV);
  } elsif ($arg =~ /-n/) {	# get number of days before data considered dubious
    $dubdays = shift(@ARGV);
  } elsif ($arg =~ /-d/) {	# get directory to check
    $polldir = shift(@ARGV);
    if (! -d $polldir) {
      print "polling directory not found: $polldir\n";
      usage();
    }
  }
}

if (!defined($accountfile)) {
  print "Error! No HDB login file!\n";
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

if (!defined($dubdays)) {
  $dubdays = 35;
}

$archivedir=$appdir . "/scadaTransfer/work/old_csv";
$dubiousdir=$appdir . "/scadaTransfer/work/dubious";

chdir $polldir;

mkdir $dubiousdir if (! -d $dubiousdir);

#run this loop forever
while (1) {
#check the directory specified for files beginning with $pattern
  opendir(DIR, $polldir) || die "opendir of $polldir failed: $!";
  @files = grep { /^$pattern/ && -f "$polldir/$_" } readdir(DIR);
  closedir DIR;

  undef @problems;

  #if any new files, 
  # run parsecsv.pl to create separate crsp_ files for each day
  # run the loading script against them, and then
  # move the crsp_ files to the old_files subdir and
  # move the csv file to old_csv subdir
  if (@files) {
    sleep 10; # wait for gefrx to finish checking the file before moving it!

    my @today=Today();

    foreach $file (@files) {
      #compare dates for files to current date, ignore and complain if it's more than X days old
      my @date = $file =~ /csv_(20\d\d)_(\d\d)_(\d\d)/;
      if (Delta_Days(@date,@today) > $dubdays ) {
        push(@problems,$file);
        mv "$polldir/$file","$dubiousdir" or
          die "Failed to move file: $file\n$!\n";
        next;
      }

      @program=("perl","$appdir/scadaTransfer/src/parsecsv.pl","-f","$polldir/$file");
      system (@program) == 0 or die "Failed to run parsecsv.pl!\n $!";

      #read the directory again to find all crsp_20* files 
      # have to do this since the old scada files are crsp_ as well, they
      # just use a two digit year instead of four.
      # watch out for the Y2.1K bug!
      opendir(DIR, $polldir) || die "opendir of $polldir failed: $!";
      @crspfiles = grep { /^crsp_20/ && -f "$polldir/$_" } readdir(DIR);
      closedir DIR;

      # now for each crsp file created by parsecsv.pl, run the loading script
      foreach $crspfile (@crspfiles) {
        @program=("perl","$appdir/scadaTransfer/src/scada2hdb.pl","-a",$accountfile,"-f","$polldir/$crspfile");
        system (@program) == 0 or die "Failed to run scada2hdb.pl!\n $!";
      # move processed crspfile to old_files
        $newcrspfile=$crspfile;
        while (-e "$appdir/scadaTransfer/work/old_files/$newcrspfile") {
          $newcrspfile =~ s/dat$/update.dat/;
        }
        mv "$polldir/$crspfile", "$appdir/scadaTransfer/work/old_files/$newcrspfile" or
           die "Failed to move file: $crspfile\n$!\n";
      }

      #now move processed csv file to command line specified directory
      mv "$polldir/$file","$archivedir/$file" or
         die "Failed to move file: $file\n$!\n";
    }

    if (@problems) {
      @problems = map {"$dubiousdir/" . $_ } @problems;
      @problems = map { ("-a", $_) } @problems;

      @program=("mailx","-s","Transfer Warning: $progname found dubious files more than $dubdays day(s) old",
          @problems,"--",$ENV{HDB_XFER_EMAIL});
      system(@program) == 0 or warn ("email about dubious files failed!");
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
Example: $progname -d <directory> -a <accountfile> -f <pattern>

  -h               : This help
  -v               : Version
  -n <days>        : Number of days before new files considered dubious (default 35)
  -a <accountfile> : HDB account login file (REQUIRED)
  -f <filename pattern>    : file pattern to watch for csv files(REQUIRED)
  -d <directory>   : directory to watch for files with right pattern (REQUIRED)
ENDHELP

  exit (1);
}

sub version
{
  print "$progname version $verstring\n";
  exit 1;
}
