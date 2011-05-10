## Makefile.am for gtk+panda

SUBDIRS = intl po 					\
	gtkpanda 				\
	docs


pkgconfigdir = 					\
	$(libdir)/pkgconfig

pkgconfig_DATA = 				\
	gtkpanda2.pc

EXTRA_DIST = config.rpath 					\
	config.h.win32				\
	gtkpanda2.pc.in

snapshot:
	$(MAKE) dist distdir=$(PACKAGE)`date +"%y%m%d"`

ACLOCAL_AMFLAGS = -I m4

