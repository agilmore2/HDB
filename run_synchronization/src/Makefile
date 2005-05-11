# run_synchronization makefile
# $Id$

include ../../Environ

OBJDIR = $(HDB_ENV)/run_synchronization/obj
INCLUDES = -I$(HDB_APP_INCLUDE) 
LIBS = $(HDB_LIBS) -ldl -lm 
CFLAGS = -ansi -Wall -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_BIN)/SYNC

OBJS = $(OBJDIR)/syncModelRuns.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(OBJS)	
	rm -f $(TARGET)

#        TARGET DEFINITIONS

$(OBJDIR)/syncModelRuns.o: syncModelRuns.c\
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/dbutils.h 
	$(CC) $(CFLAGS) -c syncModelRuns.c -o $@
