#  The version for use with C and Oracle follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGET = $(HDB_APP_BIN)/scadaTransfer
OBJS = \
		$(OBJDIR)/insertScada.o \
		$(OBJDIR)/scadaTransfer.o

main: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

$(OBJDIR)/insertScada.o: \
					insertScada.pc \
					$(HDB_APP_LIB)/libhdb.a \
					$(HDB_APP_LIB)/libutils.a
	proc iname=insertScada.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c insertScada.c -o $@

$(OBJDIR)/scadaTransfer.o: \
					scadaTransfer.c \
					scadaTransfer.h \
					$(HDB_APP_LIB)/libhdb.a \
					$(HDB_APP_LIB)/libutils.a
	$(CC) $(CFLAGS) -c scadaTransfer.c -o $@
