#!/usr/bin/perl -w

use strict;
my $file = "$ENV{HDB_ENV}/ratings/codwr/logs/get_codwr_ratings.err";
system("egrep \"ADATUNCO \|BTBLESCO \|BTABESCO \|OLYTUNCO \|HFCBBSCO \|HFCFLTCO \|BTCANYCO \|HFCWASCO \|CURVE: \" $file | mail -s \"CODWR SHIFTS\" $ENV{HDB_NOTIFY_CASPER_EMAIL}");
