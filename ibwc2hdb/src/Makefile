#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGETS = $(HDB_APP_BIN)/usgs2hdb.pl

BINS = $(SRCDIR)/usgs2hdb.pl

main: $(TARGETS)

clean:
	rm -f $(TARGETS) 

$(TARGETS): $(BINS)
	cd $(HDB_APP_BIN) ; \
	rm -f $(TARGETS) ; \
	ln -s $(BINS) ; \
	cd $(APPDIR)

archive:
	cd $(HDB_ENV) ; \
	/usr/local/bin/tar czvf usgs2hdb.tar.gz usgs2hdb --exclude=RCS --exclude=logs
