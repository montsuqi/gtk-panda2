## Makefile.am for gtk+panda

AUTOMAKE_OPTIONS = 1.4

LTVERSION = 2:1:0

gtkpanda_public_h_sources = \
	debug.h 			\
	numeric.h 			\
	gtknumberentry.h		\
	gtkpanda.h 			\
	gtkpandaclist.h			\
	gtkpandacombo.h			\
	gtkpandafileentry.h		\
	gtkpandaentry.h 		\
	gtkpandahtml.h 			\
	gtkpandatext.h			\
	gtkpandatimer.h			\
	gtkpandadownload.h		\
	gtkpandaprint.h			\
	imcontrol.h			\
	gtkpandapdf.h			\
	gtkpandapixmap.h		\
	gtkpandatable.h			\
	pandacellrenderertext.h		\
	pandamarshal.h

gtkpanda_c_sources = 			\
	numeric.c 			\
	gtkpandaclist.c			\
	gtkpandacombo.c			\
	gtkpandafileentry.c		\
	gtknumberentry.c 		\
	gtkpandaentry.c 		\
	gtkpandatext.c			\
	gtkpandatimer.c			\
	gtkpandadownload.c		\
	gtkpandaprint.c			\
	imcontrol.h			\
	imcontrol.c			\
	gtkpandapdf.c			\
	gtkpandapixmap.c		\
	gtkpandatable.c			\
	gtkpandamain.c			\
	gtkpandahtml.c			\
	pandacellrenderertext.c		\
	pandamarshal.c

gtkpanda_private_h_sources =

INCLUDES = $(GTK_PANDA_CFLAGS) \
	-I$(top_srcdir)

AM_CPPFLAGS = -g -DLOCALEDIR=\"$(localedir)\"

libgtkpandaincludedir = $(includedir)/gtkpanda-2.0/gtkpanda
libgtkpandainclude_HEADERS = $(gtkpanda_public_h_sources)

lib_LTLIBRARIES = libgtkpanda-2.0.la
libgtkpanda_2_0_la_SOURCES =		\
	$(gtkpanda_c_sources)			\
	$(gtkpanda_public_h_sources)
libgtkpanda_2_0_la_LDFLAGS = 			\
	-version-info  $(LTVERSION) 		\
	-export-dynamic 					\
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
