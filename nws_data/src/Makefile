#  The version for use with C and Oracle follows:

include ../../Environ

######################### Directives ###############################

main: links

links:
	cd $(HDB_APP_BIN) ; \
	rm -f nwsdata2hdb.pl ; \
	ln -s ../nws_data/src/nwsdata2hdb.pl nwsdata2hdb.pl ; \
	cd $(HDB_ENV)/nws_data/src
