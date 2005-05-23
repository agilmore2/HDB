include ../../Environ
include make_include

TARGET = $(HDB_APP_BIN)/24moFld
OBJS = \
		$(OBJDIR)/FormatOutput.o \
		$(OBJDIR)/SqlFetch24moFldValues.o \
		$(OBJDIR)/24moFld.o

.PHONY: main
main: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

obj: $(OBJS)

clean: 
	rm -f $(OBJS)

#        TARGET DEFINITIONS

$(OBJDIR)/24moFld.o: 24moFld.c \
			$(HDB_APP_INCLUDE)/lc_apps.h \
			$(HDB_APP_INCLUDE)/globals.h \
			$(HDB_APP_INCLUDE)/defines.h \
			$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) $(INCLUDES) -c 24moFld.c -o $@

$(OBJDIR)/FormatOutput.o: FormatOutput.c \
			$(HDB_APP_INCLUDE)/lc_apps.h \
			$(HDB_APP_INCLUDE)/globals.h \
			$(HDB_APP_INCLUDE)/defines.h \
			$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) $(INCLUDES) -c FormatOutput.c -o $@

$(OBJDIR)/SqlFetch24moFldValues.o: SqlFetch24moFldValues.pc \
			$(HDB_APP_INCLUDE)/lc_apps.h \
			$(HDB_APP_INCLUDE)/globals.h \
			$(HDB_APP_INCLUDE)/defines.h
	proc code=ansi_c iname=SqlFetch24moFldValues.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) $(INCLUDES) -c SqlFetch24moFldValues.c -o $@
