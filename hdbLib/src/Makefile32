# hdbLib makefile
# $Id$:

include ../../Environ

OBJDIR = $(HDB_ENV)/hdbLib/obj
INCLUDES = -I$(HDB_APP_INCLUDE)
LIBS = $(HDB_LIBS) -lm -ldl
CFLAGS = -ansi -Wall -c -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_LIB)/libhdb.a

OBJS = \
	$(OBJDIR)/SqlError.o \
	$(OBJDIR)/SqlGetDBName.o \
	$(OBJDIR)/SqlConnect.o \
	$(OBJDIR)/SqlDate.o \
	$(OBJDIR)/SqlDataSource.o \
	$(OBJDIR)/SqlValidation.o \
	$(OBJDIR)/SqlUnits.o \
	$(OBJDIR)/SqlInterval.o \
	$(OBJDIR)/SqlObjects.o \
	$(OBJDIR)/SqlModel.o \
	$(OBJDIR)/SqlGetDBName.o \
	$(OBJDIR)/SqlCount.o \
	$(OBJDIR)/SqlLookup.o \
	$(OBJDIR)/SqlBaseKeys.o \
	$(OBJDIR)/SqlChkModelRunID.o \
	$(OBJDIR)/SqlGetSiteDataTypeIdWithName.o

GLOBAL_INCLUDE = \
	$(HDB_APP_INCLUDE)/dbutils.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/typedefs.h

INTERMED_PROC_FILES = \
	SqlBaseKeys.c \
	SqlChkModelRunID.c \
	SqlConnect.c \
	SqlCount.c \
	SqlDataSource.c \
	SqlDate.c \
	SqlError.c \
	SqlGetDBName.c \
	SqlGetSiteDataTypeIdWithName.c \
	SqlInterval.c \
	SqlLookup.c \
	SqlModel.c \
	SqlObjects.c \
	SqlUnits.c \
	SqlValidation.c

$(TARGET) : $(OBJS)
	rm -f $(HDB_APP_LIB)/libhdb.a
	ar rv $(HDB_APP_LIB)/libhdb.a $(OBJS)
	ranlib $(HDB_APP_LIB)/libhdb.a

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -f $(INTERMED_PROC_FILES)
	rm -f *.lis

$(OBJDIR)/SqlError.o : \
	$(GLOBAL_INCLUDE) \
	SqlError.pc
	proc code=ansi_c iname=SqlError.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlError.c -o $@

$(OBJDIR)/SqlGetDBName.o : \
	$(GLOBAL_INCLUDE) \
	SqlGetDBName.pc
	proc code=ansi_c iname=SqlGetDBName.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlGetDBName.c -o $@

$(OBJDIR)/SqlConnect.o : \
	$(GLOBAL_INCLUDE) \
	SqlConnect.pc
	proc code=ansi_c iname=SqlConnect.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlConnect.c -o $@

$(OBJDIR)/SqlDate.o : \
	$(GLOBAL_INCLUDE) \
	SqlDate.pc
	proc code=ansi_c iname=SqlDate.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlDate.c -o $@

$(OBJDIR)/SqlDataSource.o : \
	$(GLOBAL_INCLUDE) \
	SqlDataSource.pc
	proc code=ansi_c iname=SqlDataSource.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlDataSource.c -o $@

$(OBJDIR)/SqlValidation.o : \
	$(GLOBAL_INCLUDE) \
	SqlValidation.pc
	proc code=ansi_c iname=SqlValidation.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlValidation.c -o $@

$(OBJDIR)/SqlUnits.o: \
	$(GLOBAL_INCLUDE) \
	SqlUnits.pc
	proc code=ansi_c iname=SqlUnits.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlUnits.c -o $@

$(OBJDIR)/SqlInterval.o: \
	$(GLOBAL_INCLUDE) \
	SqlInterval.pc
	proc code=ansi_c char_map=string iname=SqlInterval.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlInterval.c -o $@

$(OBJDIR)/SqlObjects.o: \
	$(GLOBAL_INCLUDE) \
	SqlObjects.pc \
	$(HDB_APP_INCLUDE)/utils.h
	proc code=ansi_c iname=SqlObjects.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlObjects.c -o $@

$(OBJDIR)/SqlModel.o: \
	$(GLOBAL_INCLUDE) \
	SqlModel.pc \
	$(HDB_APP_INCLUDE)/utils.h
	proc code=ansi_c CHAR_MAP=string sqlcheck=semantics userid=$(DBA)/$(DBA_PASSWD) iname=SqlModel.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlModel.c -o $@

$(OBJDIR)/SqlCount.o: \
	$(GLOBAL_INCLUDE) \
	SqlCount.pc \
	$(HDB_APP_INCLUDE)/utils.h
	proc code=ansi_c iname=SqlCount.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlCount.c -o $@

$(OBJDIR)/SqlLookup.o: \
	$(GLOBAL_INCLUDE) \
	SqlLookup.pc \
	$(HDB_APP_INCLUDE)/utils.h
	proc code=ansi_c iname=SqlLookup.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlLookup.c -o $@

$(OBJDIR)/SqlBaseKeys.o: \
	$(GLOBAL_INCLUDE) \
	SqlBaseKeys.pc \
	$(HDB_APP_INCLUDE)/utils.h
	proc code=ansi_c iname=SqlBaseKeys.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlBaseKeys.c -o $@

$(OBJDIR)/SqlChkModelRunID.o: \
	$(GLOBAL_INCLUDE) \
	SqlChkModelRunID.pc
	proc code=ansi_c iname=SqlChkModelRunID.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlChkModelRunID.c -o $@

$(OBJDIR)/SqlGetSiteDataTypeIdWithName.o: \
	$(GLOBAL_INCLUDE) \
	SqlGetSiteDataTypeIdWithName.pc 
	proc code=ansi_c iname=SqlGetSiteDataTypeIdWithName.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS)  SqlGetSiteDataTypeIdWithName.c -o $@
