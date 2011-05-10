# basins makefile
# $Id$

include ../../Environ

OBJDIR = $(HDB_ENV)/basins/obj
INCLUDES= -I$(HDB_APP_INCLUDE)
LIBS= $(HDB_LIBS) -lm -ldl
CFLAGS = -ansi -Wall -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_BIN)/basins

OBJS = \
	$(OBJDIR)/SqlFetchSiteNames.o \
	$(OBJDIR)/Basins.o \
	$(OBJDIR)/ParseDate.o \
	$(OBJDIR)/UpdateSnotel.o \
	$(OBJDIR)/SqlUpdateSnotel.o \
	$(OBJDIR)/UpdateBasins.o \
	$(OBJDIR)/SqlUpdateBasins.o

INTERMED_PROC_FILES = \
	SqlFetchSiteNames.c \
	SqlUpdateBasins.c \
	SqlUpdateSnotel.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean: 
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -f $(INTERMED_PROC_FILES)
	rm -f *.lis

#        TARGET DEFINITIONS

$(OBJDIR)/SqlFetchSiteNames.o: \
	SqlFetchSiteNames.pc \
	basins.h \
	basinTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/typedefs.h \
	$(HDB_APP_INCLUDE)/dbutils.h \
	$(HDB_APP_INCLUDE)/utils.h \
	$(HDB_APP_LIB)/libhdb.a
	proc code=ansi_c char_map=string iname=SqlFetchSiteNames.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlFetchSiteNames.c -o $@

$(OBJDIR)/SqlUpdateSnotel.o: \
	SqlUpdateSnotel.pc \
	basins.h \
	basinTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/typedefs.h \
	$(HDB_APP_INCLUDE)/dbutils.h \
	$(HDB_APP_INCLUDE)/utils.h \
	$(HDB_APP_LIB)/libhdb.a
	proc code=ansi_c char_map=string sqlcheck=semantics userid=$(DBA)/$(DBA_PASSWD) iname=SqlUpdateSnotel.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlUpdateSnotel.c -o $@

$(OBJDIR)/Basins.o: \
	Basins.c \
	basins.h \
	basinTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/typedefs.h \
	$(HDB_APP_INCLUDE)/dbutils.h \
	$(HDB_APP_INCLUDE)/utils.h
	$(CC) $(CFLAGS) -c Basins.c -o $@

$(OBJDIR)/ParseDate.o: \
	ParseDate.c \
	basins.h
	$(CC) $(CFLAGS) -c ParseDate.c -o $@

$(OBJDIR)/UpdateSnotel.o: \
	UpdateSnotel.c \
	basins.h \
	basinTypedefs.h \
	$(HDB_APP_INCLUDE)/dbutils.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/utils.h \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c UpdateSnotel.c -o $@

$(OBJDIR)/SqlUpdateBasins.o: \
	SqlUpdateBasins.pc \
	basins.h \
	basinTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/typedefs.h \
	$(HDB_APP_INCLUDE)/dbutils.h \
	$(HDB_APP_INCLUDE)/utils.h \
	$(HDB_APP_LIB)/libhdb.a
	proc code=ansi_c char_map=string sqlcheck=semantics userid=$(DBA)/$(DBA_PASSWD) iname=SqlUpdateBasins.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlUpdateBasins.c -o $@

$(OBJDIR)/UpdateBasins.o: \
	UpdateBasins.c \
	basins.h \
	basinTypedefs.h \
	$(HDB_APP_INCLUDE)/dbutils.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/utils.h \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c UpdateBasins.c -o $@


