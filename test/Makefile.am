noinst_PROGRAMS = \
	testpandaentry \
	testnumberentry

BASE_CFLAGS = \
	-I $(top_builddir)/gtkpanda \
	$(GTK_PANDA_CFLAGS)

BASE_LIBS = \
	$(top_builddir)/gtkpanda/libgtkpanda-2.0.la \
	$(GTK_PANDA_LIBS)

testpandaentry_SOURCES = testpandaentry.c
testpandaentry_CFLAGS = $(BASE_CFLAGS)
testpandaentry_LDADD = $(BASE_LIBS)

testnumberentry_SOURCES = testnumberentry.c
testnumberentry_CFLAGS = $(BASE_CFLAGS)
testnumberentry_LDADD = $(BASE_LIBS)
