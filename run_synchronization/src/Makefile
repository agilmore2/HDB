# run_synchronization makefile
# $Id$

include ../../Environ

OBJDIR = $(HDB_ENV)/run_synchronization/obj
INCLUDES = -I$(HDB_APP_INCLUDE) -I/usr/local/X11R5/include
LIBS = $(HDB_LIBS) -ldl -lX11 -lm 
CFLAGS = -ansi -Wall -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_BIN)/SYNC

OBJS = $(OBJDIR)/syncModelRuns.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(OBJS)	

#        TARGET DEFINITIONS

$(OBJDIR)/syncModelRuns.o: syncModelRuns.c\
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/dbutils.h 
	$(CC) $(CFLAGS) -c syncModelRuns.c -o $@
