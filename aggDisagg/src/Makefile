# aggDisagg makefile
# $Id$

include ../../Environ

OBJDIR = $(HDB_ENV)/aggDisagg/obj
INCLUDES = -I$(HDB_APP_INCLUDE) -I$(TK_INCLUDES) -I$(TK_HEADER) -I/usr/local/X11R5/include
LIBS = $(HDB_LIBS) -lm -ldl
CFLAGS = -ansi -Wall -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_BIN)/aggDisagg

OBJS = \
	$(OBJDIR)/AggDisagg.o \
	$(OBJDIR)/RunControl.o \
	$(OBJDIR)/SqlLoadAggDisagg.o \
	$(OBJDIR)/SqlBoundaryDates.o \
	$(OBJDIR)/ProcessNonStats.o \
	$(OBJDIR)/ProcessStats.o \
	$(OBJDIR)/SqlRetrieveSourceValues.o \
	$(OBJDIR)/SqlFetchPreviousTimeStepValue.o \
	$(OBJDIR)/AggValues.o \
	$(OBJDIR)/DisaggValues.o \
	$(OBJDIR)/WriteValues.o \
	$(OBJDIR)/SqlWriteAggDisagg.o

INTERMED_PROC_FILES = \
	SqlBoundaryDates.c \
	SqlFetchPreviousTimeStepValue.c \
	SqlLoadAggDisagg.c \
	SqlRetrieveSourceValues.c \
	SqlWriteAggDisagg.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean: 
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -f $(INTERMED_PROC_FILES)
	rm -f *.lis

#        TARGET DEFINITIONS

$(OBJDIR)/AggDisagg.o: \
	AggDisagg.c \
	aggDisagg.h \
	aggDisaggDefines.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c AggDisagg.c -o $@

$(OBJDIR)/RunControl.o: \
	RunControl.c \
	aggDisagg.h \
	aggDisaggDefines.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c RunControl.c -o $@

$(OBJDIR)/SqlLoadAggDisagg.o: \
	SqlLoadAggDisagg.pc \
	aggDisagg.h \
	aggDisaggDefines.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c CHAR_MAP=string iname=SqlLoadAggDisagg.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlLoadAggDisagg.c -o $@

$(OBJDIR)/SqlBoundaryDates.o: \
	SqlBoundaryDates.pc \
	aggDisagg.h \
	aggDisaggDefines.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c iname=SqlBoundaryDates.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlBoundaryDates.c -o $@


$(OBJDIR)/ProcessNonStats.o: \
	ProcessNonStats.c \
	aggDisagg.h \
	aggDisaggDefines.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c ProcessNonStats.c -o $@

$(OBJDIR)/ProcessStats.o: \
	ProcessStats.c \
	aggDisagg.h \
	aggDisaggDefines.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c ProcessStats.c -o $@

$(OBJDIR)/SqlRetrieveSourceValues.o: \
	SqlRetrieveSourceValues.pc \
	aggDisagg.h \
	aggDisaggDefines.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c iname=SqlRetrieveSourceValues.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlRetrieveSourceValues.c -o $@

$(OBJDIR)/SqlFetchPreviousTimeStepValue.o: \
	SqlFetchPreviousTimeStepValue.pc \
	aggDisagg.h \
	aggDisaggDefines.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c iname=SqlFetchPreviousTimeStepValue.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlFetchPreviousTimeStepValue.c -o $@

$(OBJDIR)/AggValues.o: \
	AggValues.c \
	aggDisagg.h \
	aggDisaggDefines.h \
	userFunctions.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h 
	$(CC) $(CFLAGS) -c AggValues.c -o $@

$(OBJDIR)/DisaggValues.o: \
	DisaggValues.c \
	aggDisagg.h \
	aggDisaggDefines.h \
	userFunctions.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h 
	$(CC) $(CFLAGS) -c DisaggValues.c -o $@

$(OBJDIR)/WriteValues.o: \
	WriteValues.c \
	aggDisagg.h \
	aggDisaggDefines.h \
	userFunctions.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h 
	$(CC) $(CFLAGS) -c WriteValues.c -o $@

$(OBJDIR)/SqlWriteAggDisagg.o: \
	SqlWriteAggDisagg.pc \
	aggDisagg.h \
	aggDisaggDefines.h \
	aggDisaggTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c CHAR_MAP=string sqlcheck=semantics userid=$(DBA)/$(DBA_PASSWD) iname=SqlWriteAggDisagg.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlWriteAggDisagg.c -o $@
