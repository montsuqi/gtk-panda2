noinst_PROGRAMS = \
	testentry \
	testnumeric	\
	testtext \
	testcombo \
	testpixmap \
	testpdf \
	testhtml \
	testtimer \
	testtable

BASE_CFLAGS = \
	-I $(top_builddir)/gtkpanda \
	$(GTK_PANDA_CFLAGS)

BASE_LIBS = \
	$(top_builddir)/gtkpanda/libgtkpanda-2.0.la \
	$(GTK_PANDA_LIBS)
BASE_DEPS = \
	$(top_builddir)/gtkpanda/libgtkpanda-2.0.la

testentry_SOURCES = testentry.c
testentry_CFLAGS = $(BASE_CFLAGS)
testentry_LDADD = $(BASE_LIBS)
testentry_DEPENDENCIES = $(BASE_DEPS)

testnumeric_SOURCES = testnumeric.c
testnumeric_CFLAGS = $(BASE_CFLAGS)
testnumeric_LDADD = $(BASE_LIBS)
testnumeric_DEPENDENCIES = $(BASE_DEPS)

testtext_SOURCES = testtext.c
testtext_CFLAGS = $(BASE_CFLAGS)
testtext_LDADD = $(BASE_LIBS)
testtext_DEPENDENCIES = $(BASE_DEPS)

testcombo_SOURCES = testcombo.c
testcombo_CFLAGS = $(BASE_CFLAGS)
testcombo_LDADD = $(BASE_LIBS)
testcombo_DEPENDENCIES = $(BASE_DEPS)

##testpandaclist_SOURCES = testpandaclist.c
##testpandaclist_CFLAGS = $(BASE_CFLAGS)
##testpandaclist_LDADD = $(BASE_LIBS)
##testpandaclist_DEPENDENCIES = $(BASE_DEPS)

testpixmap_SOURCES = testpixmap.c
testpixmap_CFLAGS = $(BASE_CFLAGS)
testpixmap_LDADD = $(BASE_LIBS)
testpixmap_DEPENDENCIES = $(BASE_DEPS)

testpdf_SOURCES = testpdf.c
testpdf_CFLAGS = $(BASE_CFLAGS)
testpdf_LDADD = $(BASE_LIBS)
testpdf_DEPENDENCIES = $(BASE_DEPS)

testhtml_SOURCES = testhtml.c
testhtml_CFLAGS = $(BASE_CFLAGS)
testhtml_LDADD = $(BASE_LIBS)
testhtml_DEPENDENCIES = $(BASE_DEPS)

testtimer_SOURCES = testtimer.c
testtimer_CFLAGS = $(BASE_CFLAGS)
testtimer_LDADD = $(BASE_LIBS)
testtimer_DEPENDENCIES = $(BASE_DEPS)

testtable_SOURCES = testtable.c
testtable_CFLAGS = $(BASE_CFLAGS)
testtable_LDADD = $(BASE_LIBS)
testtable_DEPENDENCIES = $(BASE_DEPS)
