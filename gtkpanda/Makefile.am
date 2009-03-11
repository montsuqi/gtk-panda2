## Makefile.am for gtk+panda

AUTOMAKE_OPTIONS = 1.4

LTVERSION = 1:0:0

if PDF
gtkpandapdf_c = gtkpandapdf.c
gtkpandapdf_h = gtkpandapdf.h
testpdf = testpdf
else
gtkpandapdf_c =
gtkpandapdf_h = 
testpdf =
endif

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
	gsio.h					\
	gtkgs.h					\
	gtkpandaps.h			\
	imcontrol.h				\
	ps.h					\
	$(gtkpandapdf_h)

gtkpanda_c_sources = 		\
	numeric.c 				\
	gtkpandamain.c 			\
	gtkpandaclist.c			\
	gtkpandacombo.c			\
	gtkpandafileentry.c		\
	gtknumberentry.c 		\
	gtkpandaentry.c 		\
	gtkpandatext.c			\
	gtkpandatimer.c			\
	gsio.c					\
	gtkgs.c					\
	gtkpandaps.c			\
	imcontrol.c				\
	ps.c					\
	$(gtkpandapdf_c)

gtkpanda_cpp_sources = 		\
	gtkpandahtml.cpp

gtkpanda_private_h_sources =

INCLUDES = $(GTK_PANDA_CFLAGS) \
	$(POPPLER_CFLAGS) \
	-I$(top_srcdir)

AM_CPPFLAGS = -g -DLOCALEDIR=\"$(localedir)\"

libgtkpandaincludedir = $(includedir)/gtkpanda-2.0/gtkpanda
libgtkpandainclude_HEADERS = $(gtkpanda_public_h_sources)

lib_LTLIBRARIES = libgtkpanda-2.0.la
libgtkpanda_2_0_la_SOURCES =		\
	$(gtkpanda_c_sources)			\
	$(gtkpanda_cpp_sources)			\
	$(gtkpanda_public_h_sources)
libgtkpanda_2_0_la_LDFLAGS = 			\
	-version-info  $(LTVERSION) 		\
	-export-dynamic 					\
	$(GTK_PANDA_LIBS)					\
	$(POPPLER_LIBS)
EXTRA_HEADERS =
EXTRA_DIST = 						\
	$(gtkpanda_private_h_sources) 	\
	gtkpanda.def makefile.msc 		\
	tree_plus.xpm 					\
	tree_minus.xpm

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
	testps			\
	$(testpdf)

DEPS = \
	libgtkpanda-2.0.la
LDADDS =	\
	./libgtkpanda-2.0.la	\
	$(GTK_PANDA_LIBS)		\
	$(POPPLER_LIBS)

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

testps_DEPENDENCIES = $(DEPS)
testps_LDADD = $(LDADDS)

testtext_DEPENDENCIES = $(DEPS)
testtext_LDADD = $(LDADDS)

testtimer_DEPENDENCIES = $(DEPS)
testtimer_LDADD = $(LDADDS)

if PDF 
testpdf_DEPENDENCIES = $(DEPS)
testpdf_LDADD = $(LDADDS)
endif
