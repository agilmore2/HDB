#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGETS = $(HDB_APP_BIN)/nmose2hdb.pl

BINS = $(SRCDIR)/nmose2hdb.pl

main: $(TARGETS)

$(TARGETS): $(BINS)
	cd $(HDB_APP_BIN) ; \
	rm -f $(TARGETS) ; \
	ln -s $(BINS) ; \
	cd $(APPDIR)

archive:
	cd $(HDB_ENV) ; \
	tar czvf nmose2hdb.tar.gz nmose2hdb --exclude=RCS --exclude=logs
