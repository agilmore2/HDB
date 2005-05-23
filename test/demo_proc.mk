include $(ORACLE_HOME)/precomp/lib/env_precomp.mk

# This Makefile builds the sample programs in 
# $(ORACLE_HOME)/precomp/demo/proc, and can serve as a template for linking
# customer applications.
#
# SAMPLES is a list of the c proc sample programs.
# CPPSAMPLES is a list of the c++ proc sample programs.
# OBJECT_SAMPLES is a list of the proc sample programs using the new
# type features.  You must install ott in addition to proc to run some
# of those samples.
#
SAMPLES=sample1 sample2 sample3 sample4 sample6 sample7 sample8 \
	sample9 sample10 sample11 sample12 oraca sqlvcp cv_demo 
CPPSAMPLES=cppdemo1 cppdemo2 cppdemo3
OBJECT_SAMPLES=objdemo1 lobdemo1 coldemo1 navdemo1
#
# These targets are build all of a class of samples in one call to make.
#
samples: $(SAMPLES)
cppsamples: $(CPPSAMPLES)
object_samples: $(OBJECT_SAMPLES) 
#
# The target 'build' puts together an executable $(EXE) from the .o files
# in $(OBJS) and the libraries in $(PROLDLIBS).  It is used to build the
# c sample programs.  The rules to make .o files from .c and .pc files are
# later in this file.
# ($(PROLDLIBS) includes the client shared library, and $(STATICPROLDLIBS) does
# not.)
#
build: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) -L$(LIBHOME) $(PROLDLIBS)
build_static: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) -L$(LIBHOME) $(STATICPROLDLIBS)
$(SAMPLES) $(OBJECT_SAMPLES):
	$(MAKE) -f $(MAKEFILE) OBJS=$@.o EXE=$@ build
#
# The c++ samples are built using the 'cppbuild' target.  It precompiles to
# get a .c file, compiles to get a .o file and then builds the executable.
#
cppbuild: 
	$(PROC) $(PROCPPFLAGS) iname=$(EXE)
	CC -c $(INCLUDE) $(EXE).c
	CC -o $(EXE) $(OBJS) -L$(LIBHOME) $(CPPLDLIBS)
cppbuild_static: 
	$(PROC) $(PROCPPFLAGS) iname=$(EXE)
	CC -c $(INCLUDE) $(EXE).c
	CC -o $(EXE) $(OBJS) -L$(LIBHOME) $(STATICCPPLDLIBS)
$(CPPSAMPLES):
	$(MAKE) -f $(MAKEFILE) OBJS=$@.o EXE=$@ cppbuild
#
# sample5.pc illustrates the precompiler part of building a precompiler-forms
# application.  Building the actual executable would require the FORMS bundle.
#
sample5:
	@echo 'sample5 is a user-exit demo; use a forms makefile to build it.'
#
# Some of the samples require that .sql scripts be run before precompilation.
# If you set RUNSQL=run in your call to make, then make will use sqlplus or
# svrmgrl, as appropriate, to run the .sql scripts.
# If you leave RUNSQL unset, then make will print out a reminder to run the
# scripts.
# If you have already run the scripts, then RUNSQL=done will omit the reminder.
#
sqlplus_run:
	($(CD) ../sql; $(BINHOME)sqlplus $(USER) @$(SCRIPT) < /dev/null)
svrmgrl_run:
	($(CD) ../sql; $(BINHOME)svrmgrl < $(SCRIPT).sql)
sqlplus_ svrmgrl_:
	@$(ECHO) "# You must run the .sql script, " $(SCRIPT),
	@$(ECHO) "# before precomping this sample."
sqlplus_done svrmgrl_done:
#
calldemo-sql:
	$(MAKE) -f $(MAKEFILE) USER=scott/tiger SCRIPT=calldemo sqlplus_$(RUNSQL)
sample11-sql:
	$(MAKE) -f $(MAKEFILE) USER=scott/tiger SCRIPT=sample11 sqlplus_$(RUNSQL)
cv_demo-sql:
	$(MAKE) -f $(MAKEFILE) USER=scott/tiger SCRIPT=cv_demo sqlplus_$(RUNSQL)
lobdemo1-sql:
	$(MAKE) -f $(MAKEFILE) SCRIPT=lobdemo1c svrmgrl_$(RUNSQL)
objdemo1-sql:
	$(MAKE) -f $(MAKEFILE) SCRIPT=objdemo1 svrmgrl_$(RUNSQL)
coldemo1-sql:
	$(MAKE) -f $(MAKEFILE) SCRIPT=coldemo1 svrmgrl_$(RUNSQL)
navdemo1-sql:
	$(MAKE) -f $(MAKEFILE) SCRIPT=navdemo1 svrmgrl_$(RUNSQL)
#
# Here are some rules for converting .pc -> .c -> .o and for .typ -> .h.
#
# If proc needs to find .h files, it should find the same .h files that the 
# c compiler finds.  We use a macro named INCLUDE to hadle that.  The general 
# format of the INCLUDE macro is 
# INCLUDE= $(I_SYM)dir1 $(I_SYM)dir2 ...
#
# Normally, I_SYM=-I, for the c compiler.  However, we have a special target,
# pc1, which calls $(PROC) with various arguments, include $(INCLUDE).  It
# is used like this:
#	$(MAKE) -f $(MAKEFILE) <more args to make> I_SYM=include= pc1
# This is used for some of $(SAMPLES) and for $(OBJECT_SAMPLE).
.SUFFIXES: .pc .c .o .typ .h

pc1:
	$(PROC) $(PROCFLAGS) iname=$(PCCSRC) $(INCLUDE)

.pc.c:
	$(PROC) $(PROCFLAGS) iname=$*

.pc.o:
	$(PROC) $(PROCFLAGS) iname=$*
	$(CC) $(CFLAGS) -c $*.c

.c.o:
	$(CC) $(CFLAGS) -c $*.c

.typ.h:
	$(OTT) intype=$*.typ hfile=$*.h outtype=$*o.typ $(OTTFLAGS) code=c user=scott/tiger
#
# These are specific targets to make the .o files for samples that require
# more careful handling.
#
sample9.o: sample9.pc calldemo-sql
	$(PROC) $(PROCPLSFLAGS) iname=$*
	$(CC) $(CFLAGS) -c $*.c

cv_demo.o: cv_demo.pc cv_demo-sql
	$(PROC) $(PROCPLSFLAGS) iname=$*
	$(CC) $(CFLAGS) -c $*.c

sample11.o: sample11.pc sample11-sql
	$(PROC) $(PROCPLSFLAGS) iname=$*
	$(CC) $(CFLAGS) -c $*.c

lobdemo1.o: lobdemo1.pc lobdemo1-sql
	$(MAKE) -f $(MAKEFILE) PROCFLAGS="$(PROCPLSFLAGS)" PCCSRC=$* I_SYM=include= pc1
	$(CC) $(CFLAGS) -c $*.c

objdemo1.o: objdemo1.pc objdemo1.typ objdemo1-sql
	$(OTT) intype=$*.typ hfile=$*.h outtype=$*o.typ $(OTTFLAGS) code=c user=scott/tiger
	$(MAKE) -f $(MAKEFILE) PROCFLAGS=intype=$*o.typ PCCSRC=$* I_SYM=include= pc1
	$(CC) $(CFLAGS) -c $*.c

coldemo1.o: coldemo1.pc coldemo1.typ coldemo1-sql
	$(OTT) intype=$*.typ hfile=$*.h outtype=$*o.typ $(OTTFLAGS) code=c user=scott/tiger
	$(MAKE) -f $(MAKEFILE) PROCFLAGS=intype=$*o.typ PCCSRC=$* I_SYM=include= pc1
	$(CC) $(CFLAGS) -c $*.c

navdemo1.o: navdemo1.pc navdemo1.typ navdemo1-sql
	$(OTT) intype=$*.typ hfile=$*.h outtype=$*o.typ $(OTTFLAGS) code=c user=scott/tiger
	$(MAKE) -f $(MAKEFILE) PROCFLAGS=intype=$*o.typ PCCSRC=$* I_SYM=include= pc1
	$(CC) $(CFLAGS) -c $*.c
#
# The macro definition fill in some details or override some defaults from 
# other files.
#
OTTFLAGS=$(PCCFLAGS)
CLIBS= $(TTLIBS_QA) $(LDLIBS)
PRODUCT_LIBHOME=
MAKEFILE=$(ORACLE_HOME)/precomp/demo/proc/demo_proc.mk
PROCPLSFLAGS= sqlcheck=full userid=$(USERID) 
PROCPPFLAGS= code=cpp $(CCPSYSINCLUDE)
USERID=scott/tiger
NETWORKHOME=$(ORACLE_HOME)/network/
PLSQLHOME=$(ORACLE_HOME)/plsql/
INCLUDE=$(I_SYM). $(I_SYM)$(PRECOMPHOME)public $(I_SYM)$(RDBMSHOME)public $(I_SYM)$(RDBMSHOME)demo $(I_SYM)$(PLSQLHOME)public $(I_SYM)$(NETWORKHOME)public
I_SYM=-I
STATICPROLDLIBS=$(SCOREPT) $(SSCOREED) $(DEF_ON) $(LLIBCLIENT) $(LLIBSQL) $(TTLIBS)
PROLDLIBS=$(LLIBCLNTSH) $(STATICPROLDLIBS)
STATICCPPLDLIBS=$(SCOREPT) $(SSCOREED) $(DEF_ON) $(LLIBCLIENT) $(LLIBSQL) $(DEVTTLIBS)
CPPLDLIBS=$(LLIBCLNTSH) $(STATICCPPLDLIBS)
