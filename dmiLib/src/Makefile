# dmiLib makefile
# $Id$: 

include ../../Environ

OBJDIR = $(HDB_ENV)/dmiLib/obj
INCLUDES = -I$(HDB_APP_INCLUDE) -I$(TK_INCLUDES) -I$(TK_HEADER) -I/usr/local/X11R5/include
PC_INCLUDES = include=$(HDB_APP_INCLUDE)
LIBS = $(HDB_LIBS) -lm -ldl
CFLAGS = -ansi -Wall -c -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_LIB)/libdmi.a

OBJS = \
	$(OBJDIR)/LookupSiteDatatype.o \
	$(OBJDIR)/HDBTkFiles.o \
	$(OBJDIR)/DmiUnits.o \
	$(OBJDIR)/DmiDateConversion.o \
	$(OBJDIR)/LookupTimestep.o \
	$(OBJDIR)/readControlFile.o \
	$(OBJDIR)/HeaderStructOperations.o \
	$(OBJDIR)/DmiLoadMappings.o \
	$(OBJDIR)/SqlSyncModelRuns.o \
	$(OBJDIR)/SqlCreateModelRunId.o \
	$(OBJDIR)/SqlCreateModelMap.o \
	$(OBJDIR)/SqlUpdateSys_date.o \
	$(OBJDIR)/GetOptionalArgs.o

GLOBAL_INCLUDE = \
	$(HDB_APP_INCLUDE)/dbutils.h \
	$(HDB_APP_INCLUDE)/dmi_utils.h \
	$(HDB_APP_INCLUDE)/dmiGlobals.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/typedefs.h

$(TARGET) : $(OBJS)
	rm -f $(HDB_APP_LIB)/libdmi.a
	ar rv $(HDB_APP_LIB)/libdmi.a $(OBJS)
	ranlib $(HDB_APP_LIB)/libdmi.a

clean:
	rm -f $(OBJS)

$(OBJDIR)/DmiDateConversion.o : \
	DmiDateConversion.pc \
	$(GLOBAL_INCLUDE) 
	proc code=ansi_c iname=DmiDateConversion.pc $(PC_INCLUDES)
	$(CC) $(CFLAGS) -c DmiDateConversion.c -o $@

$(OBJDIR)/DmiUnits.o : \
	DmiUnits.pc \
	$(GLOBAL_INCLUDE) 
	proc code=ansi_c iname=DmiUnits.pc $(PC_INCLUDES)
	$(CC) $(CFLAGS) -c DmiUnits.c -o $@

$(OBJDIR)/LookupSiteDatatype.o : \
	LookupSiteDatatype.c \
	$(GLOBAL_INCLUDE) 
	$(CC) $(CFLAGS) -c LookupSiteDatatype.c -o $@

$(OBJDIR)/LookupTimestep.o : \
	LookupTimestep.c \
	$(GLOBAL_INCLUDE) 
	$(CC) $(CFLAGS) -c LookupTimestep.c -o $@

$(OBJDIR)/readControlFile.o : \
	readControlFile.c \
	$(GLOBAL_INCLUDE) 
	$(CC) $(CFLAGS) -c readControlFile.c -o $@

$(OBJDIR)/HeaderStructOperations.o : \
	HeaderStructOperations.c \
	$(GLOBAL_INCLUDE) 
	$(CC) $(CFLAGS) -c HeaderStructOperations.c -o $@

$(OBJDIR)/DmiLoadMappings.o : \
	DmiLoadMappings.pc \
	$(GLOBAL_INCLUDE) 
	proc code=ansi_c iname=DmiLoadMappings.pc $(PC_INCLUDES)
	$(CC) $(CFLAGS) -c DmiLoadMappings.c -o $@

$(OBJDIR)/SqlCreateModelRunId.o : \
	SqlCreateModelRunId.pc \
	$(GLOBAL_INCLUDE) 
	proc code=ansi_c iname=SqlCreateModelRunId.pc $(PC_INCLUDES)
	$(CC) $(CFLAGS) -c SqlCreateModelRunId.c -o $@

$(OBJDIR)/SqlCreateModelMap.o : \
	SqlCreateModelMap.pc \
	$(GLOBAL_INCLUDE) 
	proc code=ansi_c iname=SqlCreateModelMap.pc $(PC_INCLUDES)
	$(CC) $(CFLAGS) -c SqlCreateModelMap.c -o $@

$(OBJDIR)/GetOptionalArgs.o : \
	GetOptionalArgs.c \
	$(GLOBAL_INCLUDE) 
	$(CC) $(CFLAGS) -c GetOptionalArgs.c -o $@

$(OBJDIR)/HDBTkFiles.o : \
	HDBTkFiles.pc \
	$(GLOBAL_INCLUDE) 
	proc code=ansi_c iname=HDBTkFiles.pc $(PC_INCLUDES)
	$(CC) $(CFLAGS) -c HDBTkFiles.c -o $@

$(OBJDIR)/SqlUpdateSys_date.o : \
	SqlUpdateSys_date.pc \
	$(GLOBAL_INCLUDE) 
	proc code=ansi_c iname=SqlUpdateSys_date.pc $(PC_INCLUDES)
	$(CC) $(CFLAGS) -c SqlUpdateSys_date.c -o $@

$(OBJDIR)/SqlSyncModelRuns.o : \
	SqlSyncModelRuns.pc \
	$(GLOBAL_INCLUDE) 
	proc code=ansi_c iname=SqlSyncModelRuns.pc $(PC_INCLUDES)
	$(CC) $(CFLAGS) -c SqlSyncModelRuns.c -o $@

