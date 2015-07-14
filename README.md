# README #

This repository contains some historical applications, as well as many current applications running in HDB. The data and rating loaders are under constant use by ECAO, UC, and LC. There may be some bugs fixed in these versions which are not running in production.

The historical apps are almost all Pro*C based. The only one of these still running is the 'basins' application, which pulls Snotel data and basin averages from the NRCS. 

### What is this repository for? ###

* Files were pulled from ECAO filesystem on July 6, 2015. The CVS system was pulled to ECAO from UC a few years before that. There may have been additional commits at UC that need to be merged from /wrg/cvsrepos at UC.
* Consider this a point in time marker, which may be 1-5 years out of date from production. 
* The loading apps are up to date for ECAO
* Gentea in Python may be first HDB Python application

### How do I get set up? ###

* Need correct set of Perl, python, Pro*C libraries to build it all.
* Perl loader documentation is a current HDB task to be completed this year
* This repository also contains build scripts for HDB database that are out of date. We will be figuring out how to update them.
* Regression tests have not yet been added from the few that do exist at ECAO
* This is not the OpenDCS repository, that will be added separately.

### Who do I talk to? ###

* andrew@precisionwre.com