#define([PACKAGE], [provio])
#define([VERSION], [0.1.0])

AC_PREREQ([2.69])

AC_INIT([provio], [0.1.0])
AC_USE_SYSTEM_EXTENSIONS
#AC_CONFIG_AUX_DIR(autoconf)
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])
AM_PROG_AR
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
LT_PREREQ([2.4])
LT_INIT

AC_CONFIG_SRCDIR([provio/provio.c])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE

CFLAGS=${CFLAGS:-"-O2 -DNDEBUG"}


# checks for programs
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_MAKE_SET
#AX_MPI([mpi_detect=yes], [mpi_detect=false])

# checks for basic header files
AC_HEADER_STDC
AC_HEADER_TIME
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([stddef.h stdlib.h stdio.h string.h unistd.h sys/timeb.h])


# checks for types
AC_C_INLINE
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_PID_T
AC_TYPE_UID_T
AC_TYPE_SIZE_T

AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MMAP


# Checks for Operating System
AC_MSG_CHECKING([target os])
TARGET_OS=$(uname -s)
case "$TARGET_OS" in
  Linux)
    PLATFORM="-DOS_LINUX"
    ;;
  *)
    AC_MSG_ERROR([
---------------------------------------------------
Unsupported operating system:
  $TARGET_OS
Please switch to a LINUX OS.
---------------------------------------------------])
    ;;
esac
AC_SUBST([PLATFORM])
AC_MSG_RESULT([$TARGET_OS])



# Checks for C Flags
#LIBRAPTOR_HOME="-I$(top_srcdir)/lib/raptor"
#LIBRASQAL_HOME="-I$(top_srcdir)/lib/rasqal"
#LIBRDF_HOME="-I$(top_srcdir)/lib/librdf"
EXTRA_CFLAGS="-g -O0 -fPIC -Wall"
EXTRA_INCLUDES="-I$(top_srcdir) -I$(top_srcdir)/lib -I$(top_srcdir)/lib/librdf -I$(top_srcdir)/lib/librdf/src "
AC_SUBST([EXTRA_CFLAGS])
AC_SUBST([EXTRA_INCLUDES])
#EXTRA_LDFLAGS="-L/usr/local/lib -lraptor2 -L/usr/local/lib -lrasqal -L$(LIBRDF_HOME)/src -lrdf "
#AC_SUBST([EXTRA_LDFLAGS])

# checks for extra header files
AC_CHECK_HEADERS([librdf.h],
                [AC_DEFINE([HAVE_LIBRDF_H])],
                [AC_MSG_ERROR([librdf.h not found])])


# Checks for basic dependency libs
AC_CHECK_LIB([z], [gzopen], [],
             [AC_MSG_ERROR([
---------------------------------------------------
libz.so not found
which is required by libprovio
please install it and try again.
---------------------------------------------------])])
AC_CHECK_LIB([dl], [dlopen], [],
             [AC_MSG_ERROR([
---------------------------------------------------
libdl.so not found
which is required by libprovio
please install it and try again.
---------------------------------------------------])])
AC_CHECK_LIB([pthread], [pthread_create], [],
             [AC_MSG_ERROR([
---------------------------------------------------
libpthread.so not found
which is required by libprovio
please install it and try again.
---------------------------------------------------])])


# Checks for librdf
AC_CHECK_LIB([librdf], [librdf_new_world], [],
             [AC_MSG_ERROR([
---------------------------------------------------
librdf.so not found
which is required by libprovio
please install it and try again.
---------------------------------------------------])])
AC_CHECK_LIB([libhdf5], [H5Dcreate], [],
             [AC_MSG_ERROR([
---------------------------------------------------
librdf.so not found
which is required by libprovio
please install it and try again.
---------------------------------------------------])])



AC_CONFIG_FILES([Makefile
                 provio/Makefile])
AC_OUTPUT
