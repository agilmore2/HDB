include ../../Environ
include make_include

TARGET = $(HDB_APP_BIN)/24MoReport
OBJS = \
		$(OBJDIR)/24MoReport.o \
		$(OBJDIR)/24MoPower.o \
		$(OBJDIR)/SqlGet24MoData.o

main: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)
	cd $(HDB_APP_BIN) ; \
	rm -f power.in ; \
	rm -f res_file.in ; \
	ln -s $(APPDIR)/src/power.in; \
	ln -s $(APPDIR)/src/res_file.in
	cd $(APPDIR)/src


obj: $(OBJS)

clean: 
	rm -f $(OBJS)

#        TARGET DEFINITIONS

$(OBJDIR)/24MoReport.o: 24MoReport.c \
			$(HDB_APP_INCLUDE)/lc_apps.h \
			$(HDB_APP_INCLUDE)/defines.h \
			$(HDB_APP_INCLUDE)/typedefs.h
	$(CC) $(CFLAGS) $(INCLUDES) -c 24MoReport.c -o $@

$(OBJDIR)/SqlGet24MoData.o: SqlGet24MoData.pc \
			$(HDB_APP_INCLUDE)/lc_apps.h \
			$(HDB_APP_INCLUDE)/defines.h \
			$(HDB_APP_INCLUDE)/typedefs.h
	proc code=ansi_c iname=SqlGet24MoData.pc  include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) $(INCLUDES) -c SqlGet24MoData.c -o $@

$(OBJDIR)/24MoPower.o: 24MoPower.c \
			$(HDB_APP_INCLUDE)/lc_apps.h \
			$(HDB_APP_INCLUDE)/defines.h
#	proc code=ansi_c iname=24MoPower.c  include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) $(INCLUDES) -c 24MoPower.c -o $@

