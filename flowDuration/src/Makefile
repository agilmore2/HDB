# flowDuration makefile
# $Id$

include ../../Environ

OBJDIR = $(HDB_ENV)/flowDuration/obj
INCLUDES = -I$(HDB_APP_INCLUDE)
LIBS = $(HDB_LIBS) -lm -ldl
CFLAGS = -ansi -Wall -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_BIN)/flowDuration

OBJS = \
	$(OBJDIR)/FlowDuration.o \
	$(OBJDIR)/SqlGetFlowDuration.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean: 
	rm -f $(OBJS)

$(OBJDIR)/FlowDuration.o: \
	$(LOCAL_INCLUDE) \
	FlowDuration.c \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c FlowDuration.c -o $@

$(OBJDIR)/SqlGetFlowDuration.o: \
	$(LOCAL_INCLUDE) \
	SqlGetFlowDuration.pc \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c iname=SqlGetFlowDuration.pc  include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlGetFlowDuration.c -o $@


