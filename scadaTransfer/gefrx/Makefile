#
###############################################################################
###############################################################################
###                                                                         ###
###          Contains confidential and proprietary information of           ###
###                   GE Energy Management Services, Inc.                   ###
###                                                                         ###
###           Copyright 2002 GE Energy Management Services, Inc.            ###
###                           All Rights Reserved                           ###
###                                                                         ###
###############################################################################
###############################################################################
#

# @(#) $RCSfile$ $Revision$
# @(#) : mkmk,v $ : 5.7.1.3 $
# Make file for standard HCCD hierarchy.
# Requires $(PROJ) to be set to the project directory.
#
# Stops make after first error.  The .IGNORE target is not used
# because it is difficult to override.  To continue in spite of
# errors use the -i option on the make command line.
#
# Choose SCCS interface program for project:
# (No selection defaults to environment value, if any)
#SCCSINTFC=???
#
#
# targets:
#   Makefile: reexecutes the command that built this file
#   all: does make on all objects (default)
#   clean: remove all objects.
#   gefrx.c: file name of main routine     
#   gefrx$(MACHINE): .o files linked in program
#   publics: no publics file specified
#   locals: no locals file specified
#   install: move gefrx$(MACHINE) to the executable directory, .
#
# Flags to C compiler on every compile
#
INCLUDES=
CFLAGS=  $(SBFLAGS) $(INCLUDES)  
FFLAGS=$(F77FLAGS) $(INCLUDES) 
FPPFLAGS=$(INCLUDES) -e 
INSTALL=$(INSTALLCMD) 
LOGFLAGS=-C -P
ESQL=esql
ESQLFLAGS=-e
SQL=cpre
SQLFLAGS=-m -l
PROC=proc
PROCFLAGS=define=OPROC include=$(INCDIR)
TNAME=gefrx

GET=$(SCCSINTFC) get

.SUFFIXES:      .cm     .i    .fi   .pc   .cp   .ec  .cc .C

%:: RCS/%,v
	@ test -w $@ || $(CO) $(COFLAGS) $< $@

%:: %,v
	@ test -w $@ || $(CO) $(COFLAGS) $< $@

.f.o:
	$(F77) $(FFLAGS) -c $<

.s.a:
	$(AS) $(ASFLAGS) -o $*.o $<

.c.a:
	$(CC) -c $(CFLAGS) $<

.f.a:
	$(F77) $(FFLAGS) -c $<

.cc.o:
	$(CPLUS) -c $(CFLAGS) $<

.C.o:
	$(CPLUS) -c $(CFLAGS) $<

.cc.a:
	$(CPLUS) -c $(CFLAGS) $<

.C.a:
	$(CPLUS) -c $(CFLAGS) $<

.c.i:
	$(CC) $(LOGFLAGS) $(CFLAGS) $<

.f.fi:
	fpp $(FPPFLAGS) $*.f -o $*.fi

.y.c:
	$(YACC) $(YFLAGS) $<
	mv -f y.tab.c $@

.y.o:
	$(YACC) $(YFLAGS) $<
	mv -f y.tab.c $*.c
	$(CC) $(CFLAGS) -c $*.c

.cp.o:
	$(SQL) $(SQLFLAGS) $<
	$(CC) $(CFLAGS) -c $*$(MACHINE).c

.cp.c:
	$(SQL) $(SQLFLAGS) $<

.pc.o:
	$(PROC) $(PROCFLAGS) iname=$<
	$(CC) $(CFLAGS) -c $*$(MACHINE).c

.pc.c:
	$(PROC) $(PROCFLAGS) iname=$<

.ec.o:
	$(ESQL) $(ESQLFLAGS) $<
	$(CC) $(CFLAGS) -c $.c

.ec.c:
	$(ESQL) -e $<

.l.c:
	$(LEX) $(LFLAGS) $<
	mv -f lex.yy.c $@

.l.o:
	$(LEX) $(LFLAGS) $<
	mv -f lex.yy.c $*.c
	$(CC) $(CFLAGS) -c $*.c

#
# No subdirectories specified
#
all: gefrx$(MACHINE)

 
 
#
# List of c sources to use for logiscope
#
LOGCOURCES= \
  .gefrx$(MACHINE).i
 
#
# Make target for generating c logiscope data
#
logiscopec: $(LOGCOURCES)
 
.PRECIOUS: Makefile .gefrx$(MACHINE).o  $(SUBDIR) $(OBJS)

#
# Main program without additional objects
#
gefrx$(MACHINE): .gefrx$(MACHINE).o
	$(CC) -o $@ $(CFLAGS) .gefrx$(MACHINE).o $(LOADFLAGS) $(LDFLAGS)


.gefrx$(MACHINE).o: .gefrx$(MACHINE).c

#
# update subsystem internal externs in subsystem file stored in
# subsystem directory from source in subsystem directory
#
locals:
	echo no locals file specified on make file build

#
# update publicly exported externs in subsystem file stored in
# subsystem directory from source in subsystem directory
#
publics:
	echo no publics file specified on make file build

#
# install target for this directory
#
.PHONY: install
install: all
	$(INSTALL) gefrx$(MACHINE) ./$(TNAME)

#
# clean target for this directory
#
.PHONY: clean
clean: 
	-rm -f .gefrx$(MACHINE).o

#
# love message
#
.PHONY: love
love: 
	@echo "\tNot War."

#
# lint target for this directory
#
.PHONY: lint
lint: 
		lint $(CFLAGS) gefrx.c 

.gefrx$(MACHINE).c: \
	gefrx.c
	rm -f .gefrx$(MACHINE).c
	$(SYMLINK) gefrx.c .gefrx$(MACHINE).c


CodeCenter:
	#setopt load_flags $(SBFLAGS)  
	#load  \
		$(LDFLAGS)
	#load  \
		gefrx.c

#
# re-execute the command that built this file
#
.PHONY: Remake
Remake: 
	@mkmk -v \
		-t gefrx$$\(MACHINE\) \
		-m gefrx.c \
		-x . 
