## Makefile.am for gtk+panda

if TESTCODE
TESTDIRS = test
endif

SUBDIRS = intl po 					\
	gtkpanda 				\
	$(TESTDIRS) \
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

