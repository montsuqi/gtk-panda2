## Makefile.am for gtk+panda

AUTOMAKE_OPTIONS = 1.4

LTVERSION = 2:1:0

gtkpanda_public_h_sources = \
	debug.h 				\
	numeric.h 				\
	gtknumberentry.h		\
	gtkpanda.h 				\
	gtkpandaclist.h			\
	gtkpandacombo.h			\
	gtkpandafileentry.h		\
	gtkpandaentry.h 		\
	gtkpandahtml.h 			\
	gtkpandatext.h			\
	gtkpandatimer.h			\
	gtkpandadownload.h		\
	gtkpandadownload2.h		\
	gtkpandaprint.h			\
	imcontrol.h				\
	gtkpandapdf.h			\
	gtkpandapixmap.h		\
	gtkpandawindow.h		\
	gtkpandatable.h			\
	pandacellrenderertext.h	\
	pandamarshal.h

gtkpanda_c_sources = 		\
	numeric.c 				\
	gtkpandaclist.c			\
	gtkpandacombo.c			\
	gtkpandafileentry.c		\
	gtknumberentry.c 		\
	gtkpandaentry.c 		\
	gtkpandatext.c			\
	gtkpandatimer.c			\
	gtkpandadownload.c		\
	gtkpandadownload2.c		\
	gtkpandaprint.c			\
	imcontrol.c				\
	gtkpandapdf.c			\
	gtkpandapixmap.c		\
	gtkpandawindow.c		\
	gtkpandatable.c			\
	gtkpandamain.c			\
	gtkpandahtml.c			\
	pandacellrenderertext.c	\
	pandamarshal.c

gtkpanda_private_h_sources =

INCLUDES = 				\
	$(GTK_PANDA_CFLAGS) \
	-I$(top_srcdir)

AM_CPPFLAGS = -g -DLOCALEDIR=\"$(localedir)\"

libgtkpandaincludedir = $(includedir)/gtkpanda-2.0/gtkpanda
libgtkpandainclude_HEADERS = $(gtkpanda_public_h_sources)

lib_LTLIBRARIES = libgtkpanda-2.0.la
libgtkpanda_2_0_la_SOURCES =		\
	$(gtkpanda_c_sources)			\
	$(gtkpanda_public_h_sources)
libgtkpanda_2_0_la_LDFLAGS = 	\
	-version-info  $(LTVERSION)	\
	-export-dynamic 			\
	$(GTK_PANDA_LIBS)

BUILT_SOURCES = pandamarshal.h pandamarshal.c
gen_sources = pandamarshal.h pandamarshal.c
CLEANFILES = $(gen_sources)
pandamarshal.h: pandamarshal.list
	$(GLIB_GENMARSHAL) --prefix=panda_marshal \
		pandamarshal.list --header > $(@F)
pandamarshal.c: pandamarshal.list
	$(GLIB_GENMARSHAL) --prefix=panda_marshal \
		pandamarshal.list --body > $(@F)

EXTRA_HEADERS =
EXTRA_DIST = 				\
	$(gtkpanda_private_h_sources) 	\
	gtkpanda.def makefile.msc 	\
	tree_plus.xpm 			\
	tree_minus.xpm			\
	pandamarshal.h


# test programs, not to be installed

noinst_PROGRAMS = 	\
    testclist		\
	testcombo		\
	testfileentry	\
	testnumeric 	\
	testentry 		\
	testhtml		\
	testtext		\
	testtimer		\
	testpdf			\
	testpixmap		\
	testtable

DEPS = \
	libgtkpanda-2.0.la
LDADDS =	\
	./libgtkpanda-2.0.la	\
	$(GTK_PANDA_LIBS)

testclist_DEPENDENCIES = $(DEPS)
testclist_LDADD = $(LDADDS)

testcombo_DEPENDENCIES = $(DEPS)
testcombo_LDADD = $(LDADDS)

testfileentry_DEPENDENCIES = $(DEPS)
testfileentry_LDADD = $(LDADDS)

testnumeric_DEPENDENCIES = $(DEPS)
testnumeric_LDADD = $(LDADDS)

testentry_DEPENDENCIES = $(DEPS)
testentry_LDADD = $(LDADDS)

testhtml_DEPENDENCIES = $(DEPS)
testhtml_LDADD = $(LDADDS)

testtext_DEPENDENCIES = $(DEPS)
testtext_LDADD = $(LDADDS)

testtimer_DEPENDENCIES = $(DEPS)
testtimer_LDADD = $(LDADDS)

testpdf_DEPENDENCIES = $(DEPS)
testpdf_LDADD = $(LDADDS)

testpixmap_DEPENDENCIES = $(DEPS)
testpixmap_LDADD = $(LDADDS)

testtable_DEPENDENCIES = $(DEPS)
testtable_LDADD = $(LDADDS)
