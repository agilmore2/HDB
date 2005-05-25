# ucLib makefile
# $Id$

include ../../Environ

OBJDIR=	$(HDB_ENV)/ucLib/obj
INCLUDES = -I$(HDB_APP_INCLUDE)
LIBS = $(HDB_LIBS) -lm -ldl
CFLAGS = -ansi -Wall -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_LIB)/libuc.a

OBJS =  $(OBJDIR)/lookup.o \
	$(OBJDIR)/checkforModelrunid.o \
	$(OBJDIR)/insertModeldata.o

GLOBAL_INCLUDE = \
	$(HDB_APP_INCLUDE)/utils.h \
        $(HDB_APP_INCLUDE)/defines.h \
        $(HDB_APP_INCLUDE)/typedefs.h \
	$(HDB_APP_INCLUDE)/ucutils.h

$(TARGET) : $(OBJS)
	rm -f $(HDB_APP_LIB)/libuc.a
	ar rv $(HDB_APP_LIB)/libuc.a $(OBJS)
	ranlib $(HDB_APP_LIB)/libuc.a

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(HDB_APP_LIB)/libuc.a

$(OBJDIR)/lookup.o: \
	$(GLOBAL_INCLUDE) \
	lookup.pc 
	proc USER=app_user/$(HDB_LOCAL) iname=lookup.pc include=$(HDB_APP_INCLUDE) sqlcheck=semantics
	$(CC) $(CFLAGS) -c lookup.c -o $@

$(OBJDIR)/checkforModelrunid.o: \
			$(GLOBAL_INCLUDE) \
			checkforModelrunid.pc
	proc iname=checkforModelrunid.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c checkforModelrunid.c -o $@

$(OBJDIR)/insertModeldata.o: \
			$(GLOBAL_INCLUDE) \
			insertModeldata.pc
	proc iname=insertModeldata.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c insertModeldata.c -o $@

main: $(TARGET) Main.c
	$(CC) $(CFLAGS) Main.c $(LIBS) -luc -o main

