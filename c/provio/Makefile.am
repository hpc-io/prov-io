AM_CFLAGS = $(INTI_CFLAGS)


lib_LTLIBRARIES=libtest.la
libtest_la_SOURCES=test.cpp
libtest_la_CFLAGS=-fPIC
libtest_la_CPPFLAGS=-fPIC
libtest_la_CXXFLAGS=-fPIC
libtest_la_LDFLAGS= -shared -fPIC

noinst_HEADERS = 
noinst_HEADERS += rdf.h
noinst_HEADERS += stat.h
noinst_HEADERS += provio.h


provio_SOURCES =
provio_SOURCES += stat.c
provio_SOURCES += provio.c

provio_LDADD = $(top_builddir)/lib/librdf


