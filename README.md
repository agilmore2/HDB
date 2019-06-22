# README #

[![Apache License][li]][ll]

[li]: https://img.shields.io/badge/license-ASL-blue.svg
[ll]: https://www.apache.org/licenses/LICENSE-2.0-

Welcome to the Hydrologic Database (HDB) Project!

HDB is a water management data storage solution currently used in
production by several Bureau of Reclamation offices and one external
entity. The core data storage is within an Oracle Database. 

HDB has unique advantages in its tight integration with RiverWare and the
emphasis on flexibility, integrity, archival, and consistency. The
Computation Processor (a unit of the OpenDCS package) provides for
extremely flexible computational ability and is a core tool of HDB.

While the design of HDB is stable, significant new activity continues in
building additional alarm abilities, a web API, and ongoing data
loader improvements. 

While some of HDB was written or altered from work done by federal
employees, a significant amount has been built under specific
government grants and contracts for research, development, and support.
As a whole, Reclamation has concluded that the Apache 2.0 License will
be used for this project going forward. Affected contractors have been
notified and have agreed with this choice. 

## Contents ##

This repository contains some historical applications, as well as many
current applications running in HDB. The data and rating loaders are
under constant use by ECAO, UC, and LC. There may be some bugs fixed
in these versions which are not running in production.

The historical apps are almost all Pro*C based. The only one of these
still running is the 'basins' application, built in 1996, which pulls
SNOTEL data and basin averages from the NRCS.

The "oracle_script" directory includes the Oracle Database build
scripts. Most of the rest of these directories are loaders or
reporting tools. Building this software is mostly about getting the
database installed then configuring the rest of these tools with the
appropriate metadata. 

### Licensing ###

Per Section 105 of the Copyright Act of 1976, portions written by
employees of the US Federal Government as part of their official duties
are not entitled to domestic copyright protection under US Federal law.

The United States Government retains the right to pursue copyright
protections outside of the United States.

The United States Government has unlimited rights in this software and all
derivatives thereof, pursuant to the contracts under which it was developed and
the License under which it falls.

---

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
    or in this repository in the LICENSE file

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


### What is this repository for? ###

* Centralized location for HDB development
* Public reference for HDB support to point to explain issues
* Hosting location for documentation and releases

### What is in this repository? ###
* Files were pulled from the host CVS system on July 6, 2015.
* Gentea in Python may be first HDB Python application

### How do I get set up? ###

* Need correct set of Perl, python, Pro*C libraries to build it all
* Generic and Perl loader documentation to be uploaded to this repository. Currently available at hydrodb.net and on the admin@hydrodb.net Google Drive
* This repository also contains build scripts for the HDB4 database.
* Regression tests have not yet been added.
* This is not the OpenDCS repository, that will be added separately.

### Who do I talk to? ###

* TBD
