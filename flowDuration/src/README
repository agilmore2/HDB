Readme file for flowDuration
CADSWES
Rick Casey
May 2003
-----------------------------
usage: 

	flowDuration <oracleUser> <psswd> <siteId> <datatypeId> <begDate> <endDate> r 

or

	flowDuration <oracleUser> <psswd> <siteId> <datatypeId> <begDate> <endDate>  m <model_run_id>

where

 oracleUser    =  HDB logon
 psswd         =  password for oracleUser
 siteId        =  a site id
 datatypeId    =  a datatype id
 begDate       =  beginning date
 endDate       =  ending date from model day interval table 
 m             = the letter "m" (for Model)
 model_run_id  = id number of the model run from Riverware

Description

The flow duration applition produces a report on the percentage
breakdown of flow duration by cfs between two dates for a given site and
datatype. The report has the sample format:

	"Percent of values over 8 cfs is 26.17 percent"

The number of lines printed is dependent on the number of values retrieved.
A subset of the retrieved values is printed, incrementing over the total
value range sorted by values.

Assumptions

- values retrieved are in printed as cfs (cubic feet per second), though
  any numeric value would work.
- the site and datatype id's have been set up in the HDB metadata.
- the date parameters are in DD-MON-YYYY hh24:mi:ss format.

Output

The report is printed to the screen. To capture the output to a file, 
use redirection, e.g. 

	flowDuration (parameters) > yourfilename

