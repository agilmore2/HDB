#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

PERLSITECONFIG = PREFIX=$(HDB_ENV)/perlLib \
		 LIB=$(HDB_ENV)/perlLib/lib \
		 INSTALLDIRS=site \
		 INSTALLSITELIB=$(HDB_ENV)/perlLib/lib \
		 INSTALLARCHLIB=$(HDB_ENV)/perlLib/lib/sun4-solaris \
		 INSTALLMAN1DIR=$(HDB_ENV)/perlLib/man/man1 \
		 INSTALLMAN3DIR=$(HDB_ENV)/perlLib/man/man3

LIB=$(HDB_ENV)/perlLib/lib

HDBTARGET = $(LIB)/Hdb.pm

HDBLIB = $(SRCDIR)/Hdb.pm

#PERLGCC = Solaris-PerlGcc-1.3

MODULES = MIME-Base64-2.18 URI-1.23 HTML-Parser-3.27 Digest-MD5-2.24 libwww-perl-5.69 DBI-1.35 Compress-Zlib-1.20 Date-Calc-5.3 HTML-TableExtract-1.08

LIBNET = libnet-1.13

main: .junk libnet modules DBD-Oracle-1.14 $(HDBTARGET)

.PHONY : $(MODULES) modules libnet

.junk:
	touch .junk

$(HDBTARGET): $(HDBLIB)
	cd $(LIB) ; \
	rm -f $(HDBTARGET) ; \
	cp $(HDBLIB) . ;  \
	cd $(APPDIR)

modules: $(MODULES) 
 
$(MODULES): .junk
	cd $@ ; \
	perl Makefile.PL $(PERLSITECONFIG) ; \
	make ; \
	make install ; \
	cd $(APPDIR)

DBD-Oracle-1.14: DBI-1.35 .junk
	cd $@ ; \
	perl Makefile.PL -p $(PERLSITECONFIG) ; \
	make ; \
	make install ; \
	cd $(APPDIR)

perlgcc: $(PERLGCC)
	cd $(PERLGCC) ; \
	perl Makefile.PL $(PERLSITECONFIG) ; \
	make ; \
	make install ; \
	cd $(APPDIR)

libnet: $(LIBNET)
	cd $(LIBNET) ; \
	./Configure -d ; \
	 perl Makefile.PL $(PERLSITECONFIG) ; \
	make ; \
	make install ; \
	cd $(APPDIR)

libwww-perl-5.69: MIME-Base64-2.18 URI-1.23 HTML-Parser-3.27 Digest-MD5-2.24 libnet

URI-1.23: MIME-Base64-2.18

