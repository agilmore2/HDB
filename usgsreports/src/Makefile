#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGETS = $(HDB_APP_BIN)/usgsdaily\
	$(HDB_APP_BIN)/usgsmonthly

BINS = $(SRCDIR)/usgsdaily\
	$(SRCDIR)/usgsmonthly

DAILYSCRIPT = $(SRCDIR)/usgsdaily.sql 
MONTHLYSCRIPT = $(SRCDIR)/usgsmonthly.sql
GENERALSCRIPTS = $(SRCDIR)/checkforsdi.sql $(SRCDIR)/reportsdi.sql

main: $(TARGETS)

clean:
	rm -f $(TARGETS)


$(HDB_APP_BIN)/usgsdaily: $(SRCDIR)/usgsdaily\
		$(DAILYSCRIPT) $(GENERALSCRIPTS)
	cd $(HDB_APP_BIN) ; \
	rm -f usgsdaily ; \
	ln -s $(APPDIR)/src/usgsdaily ; \
	cd $(APPDIR)


$(HDB_APP_BIN)/usgsmonthly: $(SRCDIR)/usgsmonthly\
		$(MONTHLYSCRIPT) $(GENERALSCRIPTS)
	cd $(HDB_APP_BIN) ; \
	rm -f usgsmonthly ; \
	ln -s $(APPDIR)/src/usgsmonthly ; \
	cd $(APPDIR)
