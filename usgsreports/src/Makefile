#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

BINS = $(SRCDIR)/usgsdaily\
	$(SRCDIR)/usgsmonthly

SCRIPTS = $(SRCDIR)/usgsdaily.sql $(SRCDIR)/usgsmonthly.sql\
	$(SRCDIR)/checkforsdi.sql $(SRCDIR)/reportsdi.sql

main: $(BINS) $(SCRIPTS)
	cd $(HDB_APP_BIN) ; \
	rm -f usgsdaily ; \
	rm -f usgsmonthly ; \
	ln -s $(APPDIR)/src/usgsdaily ; \
	ln -s $(APPDIR)/src/usgsmonthly ; \
	cd $(APPDIR)
