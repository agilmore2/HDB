#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGETS = $(HDB_APP_BIN)/codwr2hdb.pl

BINS = $(SRCDIR)/codwr2hdb.pl

main: $(TARGETS)

$(TARGETS): $(BINS)
	cd $(HDB_APP_BIN) ; \
	rm -f $(TARGETS) ; \
	ln -s $(BINS) ; \
	cd $(APPDIR)

archive:
	cd $(HDB_ENV) ; \
	/usr/local/bin/tar czvf codwr2hdb.tar.gz codwr2hdb --exclude=RCS --exclude=logs
