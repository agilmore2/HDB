This version of the Teacup Generation software is for making teacup diagrams
for historic periods.  It is pretty much a hack but doesn't take much time.
These are the directions for use.

1.  First you must go to the src subdirectory and edit the readdata.pc code
to change the date in the sql curser.  Change the line in the code that looks
like this to reflect the date you want the Teacup generated for.

      AND B.START_DATE_TIME = TO_DATE('17-MAR-2005');

2.  You must then recompile the code with the new date.  To do this type
the command 'make' at the prompt while in the src directory.  This will create
and executable in the work directory called 'gentea_hist' that will get the 
data for the date you modified the code to.

3.  Now go to the work directory and type 'gentea_hist'  this will tell you 
that you need an input.png an output.png and a configuration file in order to run the program.  There are input.png files for each of the basins in the work
directory.  The output.png can be any name of your choosing.  The configuration
file tells gentea where to locate the teacup diagrams on the input file.  
There are serveral configuration files located in the work directory.  