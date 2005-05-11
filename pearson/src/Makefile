# pearson makefile
# $Id$

include ../../Environ

OBJDIR = $(HDB_ENV)/pearson/obj
INCLUDES = -I$(HDB_APP_INCLUDE)
LIBS = $(HDB_LIBS) -lm -ldl
CFLAGS = -ansi -Wall -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_BIN)/createLogPearsonFile

OBJS = \
	$(OBJDIR)/CreateLogPearsonFile.o \
	$(OBJDIR)/SqlSelects.o \
	$(OBJDIR)/SqlProcessValues.o

LOCAL_INCLUDE = \
        pearson.h

INTERMED_PROC_FILES = \
	SqlProcessValues.c \
	SqlSelects.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean: 
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -f $(INTERMED_PROC_FILES)
	rm -f *.lis

$(OBJDIR)/CreateLogPearsonFile.o: \
	$(LOCAL_INCLUDE) \
	CreateLogPearsonFile.c \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c CreateLogPearsonFile.c -o $@

$(OBJDIR)/SqlSelects.o: \
	$(LOCAL_INCLUDE) \
	SqlSelects.pc \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c iname=SqlSelects.pc  include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlSelects.c -o $@

$(OBJDIR)/SqlProcessValues.o: \
	$(LOCAL_INCLUDE) \
	SqlProcessValues.pc \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c iname=SqlProcessValues.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlProcessValues.c -o $@

