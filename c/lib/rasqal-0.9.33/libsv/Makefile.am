# -*- Mode: Makefile -*-
#
# Makefile for libsv convienience library
#

ANALYZE = clang
ANALYZE_FLAGS = "--analyze"
# Based on COMPILE target
ANALYZE_COMMAND = $(ANALYZE) \
	$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) \
	$(ANALYZE_FLAGS)

noinst_LTLIBRARIES = libsv.la
AM_CPPFLAGS = -DSV_CONFIG -I$(top_srcdir)/src

libsv_la_SOURCES = \
sv.c \
sv.h

EXTRA_DIST = \
test1.csv \
zero.tsv \
one.tsv \
test1.tsv


TESTS=svtest$(EXEEXT)

check_PROGRAMS=svtest$(EXEEXT)

EXTRA_PROGRAMS=example$(EXEEXT) sv2c$(EXEEXT)

CLEANFILES=$(EXTRA_PROGRAMS) \
*.plist

svtest_SOURCES = svtest.c
svtest_LDADD = $(builddir)/libsv.la

example_SOURCES = example.c
example_LDADD = $(builddir)/libsv.la

sv2c_SOURCES = sv2c.c
sv2c_LDADD = $(builddir)/libsv.la



if MAINTAINER_MODE
# Run Clang static analyzer over sources.
analyze: $(SOURCES)
	@list='$(SOURCES)'; \
	result=0; \
	for file in $$list; do \
	  if echo $$file | grep '\.c$$' >/dev/null 2>&1; then \
	    $(RECHO) "Analyzing $$file"; \
	    $(ANALYZE_COMMAND) $(srcdir)/$$file; \
	    status=$$?; \
	    if test $$status != 0; then \
	      result=1; \
	    fi; \
	  fi; \
	done; \
	set -e; exit $$result
endif

# Some people need a little help ;-)
test: check

if MAINTAINER_MODE

.PHONY: update-ChangeLog

# Requires gitlog-to-changelog from
# http://git.savannah.gnu.org/gitweb/?p=gnulib.git;a=blob_plain;f=build-aux/gitlog-to-changelog
update-ChangeLog:
	if test -d $(srcdir)/.git; then                         \
	   gitlog-to-changelog                        \
	      --format='* %s%n%b%n'                             \
	      --strip-tab --strip-cherry-pick                   \
	      -- $$(cat $(srcdir)/.last-cl-gen)..               \
	    >ChangeLog.tmp                                      \
	  && git rev-list -n 1 HEAD >.last-cl-gen.tmp           \
	  && (echo; cat $(srcdir)/ChangeLog) >>ChangeLog.tmp    \
	  && mv -f ChangeLog.tmp $(srcdir)/ChangeLog            \
	  && mv -f .last-cl-gen.tmp $(srcdir)/.last-cl-gen      \
	  && rm -f ChangeLog.tmp;                               \
	fi
endif

EXTRA_DIST += .last-cl-gen
