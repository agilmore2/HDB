Readme file for Pearson application
CADSWES
Rick Casey
May 2003
-----------------------------------

Description

The Pearson application retrieves data for a given site and datatype from the
month or day tables, either real or modelled, for a given span of years,
and produces the flows in acre feet for summer, peak and total in a form
suitable for input to a log-pearson distribution application.

Assumptions

The data for the datatype in HDB was stored in units of cfs.
(This conclusion is based on the conversion factor hard-coded in the 
application: 1.98347 applied to daily values.)

Usage

createLogPearsonFile <oracleUser> <psswd> d <site_id> <datatype_id> 0 
	<begin_year> <number_of years>

or

createLogPearsonFile <oracleUser> <psswd> m <site_id> <datatype_id> <model_run_id>
	<begin_year> <number_of years>

Output
------
Prints output to the screen. To capture output to a file, use output redirection, e.g.

createLogPearsonFile (parameters) >youroutputfile


