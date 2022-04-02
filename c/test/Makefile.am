AM_CFLAGS = $(INTI_CFLAGS)


bin_PROGRAMS =
bin_PROGRAMS += example_provio
bin_PROGRAMS += example_dlopen

test_SOURCES =
test_SOURCES += example_provio.c
test_SOURCES += example_dlopen.c

test_LDADD = $(top_builddir)/provio/libprovio.la
test_LDADD = $(top_builddir)/syscall_wrapper/libwrapper.la




