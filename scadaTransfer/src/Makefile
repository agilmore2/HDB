#  The version for use with C and Oracle follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGET = $(HDB_APP_BIN)/scadaTransfer
OBJS = \
		$(OBJDIR)/insertScada.o \
		$(OBJDIR)/scadaTransfer.o

main: $(TARGET)

clean: 
	rm -f $(TARGET) $(OBJS) edit_scada2hdb.pl

install:
	ln -s scada2hdb.pl edit_scada2hdb.pl

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -luc -o $(TARGET)

#Note, unless app_user has execute priviledge on modify_r_base_raw, this compile will fail.

$(OBJDIR)/insertScada.o: \
					insertScada.pc \
					$(HDB_APP_LIB)/libhdb.a \
					$(HDB_APP_LIB)/libutils.a
	proc user=$(DBA)/$(DBA_PASSWD) iname=insertScada.pc include=$(HDB_APP_INCLUDE) sqlcheck=semantics
	$(CC) $(CFLAGS) -c insertScada.c -o $@



$(OBJDIR)/scadaTransfer.o: \
					scadaTransfer.c \
					scadaTransfer.h \
					$(HDB_APP_LIB)/libhdb.a \
					$(HDB_APP_LIB)/libutils.a
	$(CC) $(CFLAGS) -c scadaTransfer.c -o $@
