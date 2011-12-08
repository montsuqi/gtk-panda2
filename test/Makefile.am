noinst_PROGRAMS = \
	testpandaentry 

BASE_CFLAGS = \
	-I $(top_builddir)/gtkpanda \
	$(GTK_PANDA_CFLAGS)

BASE_LIBS = \
	-Wl,--no-as-needed\
	$(top_builddir)/gtkpanda/libgtkpanda-2.0.la \
	$(GTK_PANDA_LIBS)

BASE_DEPS = \
	$(top_builddir)/gtkpanda/libgtkpanda-2.0.la

testpandaentry_SOURCES = testpandaentry.c
testpandaentry_CFLAGS = $(BASE_CFLAGS)
testpandaentry_LDADD = $(BASE_LIBS)
testpandaentry_DEPENDENCIES = $(BASE_DEPS)
