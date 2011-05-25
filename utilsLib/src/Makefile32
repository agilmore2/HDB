# utilsLib makefile
# $Id$

include ../../Environ

OBJDIR=	$(HDB_ENV)/utilsLib/obj
INCLUDES = -I$(HDB_APP_INCLUDE)
LIBS = $(HDB_LIBS) -lm -ldl
CFLAGS = -ansi -Wall -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_LIB)/libutils.a

OBJS =  $(OBJDIR)/Error.o \
        $(OBJDIR)/RoundValue.o \
        $(OBJDIR)/CalcExpression.o \
        $(OBJDIR)/TableLookup.o \
        $(OBJDIR)/Date.o \
        $(OBJDIR)/Convert.o \
        $(OBJDIR)/Interval.o \
	$(OBJDIR)/Schema.o \
	$(OBJDIR)/signalHandler.o \
	$(OBJDIR)/Misc.o

GLOBAL_INCLUDE = \
	$(HDB_APP_INCLUDE)/utils.h \
        $(HDB_APP_INCLUDE)/defines.h \
        $(HDB_APP_INCLUDE)/typedefs.h

$(TARGET) : $(OBJS)
	rm -f $(HDB_APP_LIB)/libutils.a
	ar rv $(HDB_APP_LIB)/libutils.a $(OBJS)
	ranlib $(HDB_APP_LIB)/libutils.a

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

$(OBJDIR)/Error.o: \
	Error.c \
	$(HDB_APP_INCLUDE)/utils.h \
	$(HDB_APP_INCLUDE)/defines.h
	$(CC) $(CFLAGS) -c Error.c -o $@

$(OBJDIR)/RoundValue.o: \
	$(GLOBAL_INCLUDE) \
	RoundValue.c 
	$(CC) $(CFLAGS) -c RoundValue.c -o $@

$(OBJDIR)/CalcExpression.o: \
	$(GLOBAL_INCLUDE) \
	CalcExpression.c
	$(CC) $(CFLAGS) -c CalcExpression.c -o $@

$(OBJDIR)/TableLookup.o: \
	TableLookup.c \
	$(HDB_APP_INCLUDE)/defines.h
	$(CC) $(CFLAGS) -c TableLookup.c -o $@

$(OBJDIR)/Date.o: \
	$(GLOBAL_INCLUDE) \
	Date.c \
	$(HDB_APP_INCLUDE)/dbutils.h 
	$(CC) $(CFLAGS) -c Date.c -o $@

$(OBJDIR)/Convert.o: \
	$(GLOBAL_INCLUDE) \
	Convert.c 
	$(CC) $(CFLAGS) -c Convert.c -o $@

$(OBJDIR)/Interval.o: \
	$(GLOBAL_INCLUDE) \
	Interval.c 
	$(CC) $(CFLAGS) -c Interval.c -o $@

$(OBJDIR)/Schema.o: \
	$(GLOBAL_INCLUDE) \
	Schema.c 
	$(CC) $(CFLAGS) -c Schema.c -o $@

$(OBJDIR)/signalHandler.o: \
	signalHandler.c \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_INCLUDE)/typedefs.h \
	$(HDB_APP_INCLUDE)/utils.h
	$(CC) $(CFLAGS) -c signalHandler.c -o $@

$(OBJDIR)/Misc.o: \
	$(GLOBAL_INCLUDE) \
	Misc.c 
	$(CC) $(CFLAGS) -c Misc.c -o $@

