## Process this file with automake to produce Makefile.in

bin_PROGRAMS = urlview

urlview_SOURCES = urlview.c enter.c dokey.c enter.h quote.c quote.h
urlview_LDADD = @LIBOBJS@

EXTRA_DIST = text urlview.man urlview.sgml sample.urlview \
	url_handler.sh url_handler.sh.suse urlview.conf.suse

CLEANFILES = *~ *.orig *.rej

install-data-local:
	$(INSTALL_DATA) urlview.man $(mandir)/man1/urlview.1

# how to create a target for uninstall-data-local???
