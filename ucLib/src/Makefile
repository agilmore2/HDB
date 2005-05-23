# ucLib makefile
# $Id$

include ../../Environ

OBJDIR=	$(HDB_ENV)/ucLib/obj
INCLUDES = -I$(HDB_APP_INCLUDE)
LIBS = $(HDB_LIBS) -lm -ldl
CFLAGS = -ansi -Wall -g -DANSIPROTO $(INCLUDES)

TARGET = $(HDB_APP_LIB)/libuc.a

OBJS =  $(OBJDIR)/Lookup.o 

GLOBAL_INCLUDE = \
	$(HDB_APP_INCLUDE)/utils.h \
        $(HDB_APP_INCLUDE)/defines.h \
        $(HDB_APP_INCLUDE)/typedefs.h

$(TARGET) : $(OBJS)
	rm -f $(HDB_APP_LIB)/libuc.a
	ar rv $(HDB_APP_LIB)/libuc.a $(OBJS)
	ranlib $(HDB_APP_LIB)/libuc.a
	\cp Lookup.h $(HDB_APP_INCLUDE)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(HDB_APP_LIB)/libuc.a

$(OBJDIR)/Lookup.o: \
	$(GLOBAL_INCLUDE) \
	Lookup.pc 
	proc USER=app_user/$(HDB_LOCAL) iname=Lookup.pc include=$(HDB_APP_INCLUDE) sqlcheck=semantics
	$(CC) $(CFLAGS) -c Lookup.c -o $@

main: $(TARGET) Main.c
	$(CC) $(CFLAGS) Main.c $(LIBS) -luc -o main

