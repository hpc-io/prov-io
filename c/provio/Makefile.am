# Headers
noinst_HEADERS = 
noinst_HEADERS += rdf.h
noinst_HEADERS += config.h
noinst_HEADERS += stat.h
noinst_HEADERS += provio.h

# Static Lib
noinst_LTLIBRARIES =
noinst_LTLIBRARIES += libprovio.la


# Tests
nobase_bin_PROGRAMS =
nobase_bin_PROGRAMS += stat_test
nobase_bin_PROGRAMS += config_test
nobase_bin_PROGRAMS += provio_test

# Sources
libprovio_la_SOURCES =
libprovio_la_SOURCES += config.c
libprovio_la_SOURCES += stat.c
libprovio_la_SOURCES += provio.c

libprovio_la_LIBADD = $(top_builddir)/lib/librdf/src/librdf.la
