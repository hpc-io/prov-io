#Your HDF5 install path
HDF5_DIR=/home/runzhou/Downloads/myhdfstuff/hdf5-develop/build/hdf5
REDLAND_DIR=/home/runzhou/Downloads/librdf

CC=mpicc
# CC=gcc
AR=ar


DEBUG=-g -O0
LIBRDF_EXTERNAL_CPPFLAGS = -I/usr/local/include/raptor2 -I/usr/local/include/rasqal
INCLUDES=-I$(HDF5_DIR)/include $(LIBRDF_EXTERNAL_CPPFLAGS) -I/$(REDLAND_DIR)/src
DYNLIB_INCLUDE=-I/home/runzhou/PROV-IO/c/provio

CFLAGS = $(DEBUG) $(INCLUDES) -Wall
DYNCFLAGS = $(DEBUG) $(INCLUDES) -Wall -fPIC 
DYNLIB_CFLAGS = $(CFLAGS) $(DYNLIB_INCLUDE)

#LIBS=-L$(HDF5_DIR)/lib -L$(MPI_DIR)/lib -lhdf5 -lz
LIBRDF_DIRECT_LIBS =  -L/usr/local/lib -lraptor2 -L/usr/local/lib -lrasqal -L$(REDLAND_DIR)/src -lrdf
LIBS=-L$(HDF5_DIR)/lib -lhdf5 -lz $(LIBRDF_DIRECT_LIBS)
DYNLDFLAGS = $(DEBUG) -shared -fPIC 
# DYNLDFLAGS = $(DEBUG) -dynamiclib -current_version 1.0 -fPIC $(LIBS)
LDFLAGS = $(DEBUG) $(LIBS)
ARFLAGS = rs
DYNLIB_LDFLAGS=-L/home/runzhou/PROV-IO/c/provio -lprovio


STATSRC = stat.c
STATOBJ = $(STATSRC:.c=.o)
CONFSRC = config.c
CONFOBJ = $(CONFSRC:.c=.o)

# Shared library
DYNSRC = provio.c 
DYNOBJ = $(DYNSRC:.c=.o)
DYNLIB = libprovio.so

DEPOBJ = $(STATOBJ) $(CONFOBJ) 

#DYNLIB = libh5prov.dylib
#DYNDBG = libh5prov.dylib.dSYM

# Testcase section
STATTEST = stat_test.c
STATTEST_OBJ = $(STATTEST:.c=.o)
STATTEST_EXE = $(STATTEST:.c=)
STATTEST_DBUG = $(STATTEST:.c=.dSYM)
STATTEST_OUT = stat.txt

CONFIGTEST = config_test.c
CONFIGTEST_OBJ = $(CONFIGTEST:.c=.o)
CONFIGTEST_EXE = $(CONFIGTEST:.c=)
CONFIGTEST_DBUG = $(CONFIGTEST:.c=.dSYM)

LIBTEST = provio_test.c
LIBTEST_OBJ = $(LIBTEST:.c=.o)
LIBTEST_EXE = $(LIBTEST:.c=)
LIBTEST_DBUG = $(LIBTEST:.c=.dSYM)

all: $(STATTEST_EXE) $(CONFIGTEST_EXE) $(LIBTEST_EXE) $(DYNLIB) 
# all: $(DYNLIB)

$(STATTEST_EXE): $(STATTEST) $(STATOBJ) 
		$(CC) $(CFLAGS) $^ -o $(STATTEST_EXE)

$(CONFIGTEST_EXE): $(CONFIGTEST) $(CONFOBJ)
		$(CC) $(CFLAGS) $^ -o $(CONFIGTEST_EXE) 

$(DYNLIB): $(DYNSRC)
		$(CC) $(DYNCFLAGS) $(STATSRC) -o $(STATOBJ) -c
		$(CC) $(DYNCFLAGS) $(CONFSRC) -o $(CONFOBJ) -c
		$(CC) $(DYNCFLAGS) $(DYNSRC) -o $(DYNOBJ) -c
		$(CC) $(STATOBJ) $(CONFOBJ) $(DYNOBJ) $(DYNLDFLAGS) $(LIBS) -o $(DYNLIB)

$(LIBTEST_EXE): $(LIBTEST) $(DYNLIB)
		$(CC) $(DYNLIB_CFLAGS) $(DYNLIB_LDFLAGS) $^ -o $(LIBTEST_EXE) $(LDFLAGS)

.PHONY: clean all
clean:
		rm -rf $(DYNOBJ) $(DYNLIB) $(DYNDBG) \
			$(STATTEST_OBJ) $(STATTEST_EXE) $(STATTEST_DBUG) $(STATTEST_OUT)\
			$(DEPOBJ)
# 		rm -rf $(DYNOBJ) $(DYNLIB) $(DYNDBG) 
# 			$(STATTEST_OBJ) $(STATTEST_EXE) $(STATTEST_DBUG)
