#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGET = $(HDB_APP_BIN)/glenPowRelease
OBJS = \
		$(OBJDIR)/glenpowrelease.o \
		$(OBJDIR)/retrieve_avm_othrel.o \
		$(OBJDIR)/insertAVM.o

main: $(TARGET)

clean: 
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -luc -o $(TARGET)

$(OBJDIR)/retrieve_avm_othrel.o: retrieve_avm_othrel.pc
	proc iname=$? include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c retrieve_avm_othrel.c -o $@

$(OBJDIR)/insertAVM.o: \
					insertAVM.pc \
					$(HDB_APP_LIB)/libhdb.a \
					$(HDB_APP_LIB)/libutils.a
	proc user=$(DBA)/$(DBA_PASSWD) iname=insertAVM.pc include=$(HDB_APP_INCLUDE) sqlcheck=semantics
	$(CC) $(CFLAGS) -c insertAVM.c -o $@

$(OBJDIR)/glenpowrelease.o: glenpowrelease.c
	$(CC) $(CFLAGS) -c $? -o $@

