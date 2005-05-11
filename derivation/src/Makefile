# derivation makefile
# $Id$

include ../../Environ

OBJDIR = $(HDB_ENV)/derivation/obj
INCLUDES = -I$(HDB_APP_INCLUDE) -I/usr/local/X11R5/include
LIBS = $(HDB_LIBS) -lm -ldl
CFLAGS = -ansi -Wall -g -DANSIPROTO -DDEBUG $(INCLUDES)

TARGET = $(HDB_APP_BIN)/derivation

OBJS = \
	$(OBJDIR)/Derivation.o \
	$(OBJDIR)/SqlDerivation.o \
	$(OBJDIR)/CheckData.o \
	$(OBJDIR)/DeriveValues.o \
	$(OBJDIR)/SqlMethods.o \
	$(OBJDIR)/Methods.o \
	$(OBJDIR)/SqlOverwrite.o \
	$(OBJDIR)/SqlSDIList.o \

INTERMED_PROC_FILES = \
	SqlDerivation.c \
	SqlMethods.c \
	SqlOverwrite.c \
	SqlSDIList.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

#	purify $(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean: 
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -f $(INTERMED_PROC_FILES)
	rm -f *.lis

#        TARGET DEFINITIONS

$(OBJDIR)/Derivation.o: \
	Derivation.c \
	derivation.h \
	derivationTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c Derivation.c -o $@

$(OBJDIR)/SqlDerivation.o: \
	SqlDerivation.pc \
	derivation.h \
	derivationTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c char_map=string iname=SqlDerivation.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlDerivation.c -o $@

$(OBJDIR)/CheckData.o: \
	CheckData.c \
	derivation.h \
	derivationTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c CheckData.c -o $@

$(OBJDIR)/DeriveValues.o: \
	DeriveValues.c \
	derivation.h \
	derivationTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c DeriveValues.c -o $@

$(OBJDIR)/SqlMethods.o: \
	SqlMethods.pc \
	derivation.h \
	derivationTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c char_map=string iname=SqlMethods.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlMethods.c -o $@

$(OBJDIR)/Methods.o: \
	Methods.c \
	derivation.h \
	derivationTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_LIB)/libhdb.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) -c Methods.c -o $@

$(OBJDIR)/SqlOverwrite.o: \
	SqlOverwrite.pc \
	derivation.h \
	derivationTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c char_map=string iname=SqlOverwrite.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlOverwrite.c -o $@

$(OBJDIR)/SqlSDIList.o: \
	SqlSDIList.pc \
	derivation.h \
	derivationTypedefs.h \
	$(HDB_APP_INCLUDE)/defines.h \
	$(HDB_APP_LIB)/libutils.a \
	$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c char_map=string iname=SqlSDIList.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c SqlSDIList.c -o $@
