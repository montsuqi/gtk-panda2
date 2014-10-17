## Makefile.am for gtk+panda

SUBDIRS = intl po 					\
	gtkpanda 				\
	docs


pkgconfigdir = 					\
	$(libdir)/pkgconfig

pkgconfig_DATA = 				\
	gtkpanda2.pc

EXTRA_DIST = config.rpath 					\
	gtkpanda2.pc.in

snapshot:
	$(MAKE) dist distdir=$(PACKAGE)-$(VERSION)~pre`date +"%y%m%d"`

ACLOCAL_AMFLAGS = -I m4

