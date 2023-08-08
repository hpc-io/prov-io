# Redland install path
PROV_IO_PATH=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../../)
REDLAND_DIR=$(PROV_IO_PATH)/c/lib
RAPTOR_DIR=$(REDLAND_DIR)/lib-raptor
RASQAL_DIR=$(REDLAND_DIR)/lib-rasqal
LIBRDF_DIR=$(REDLAND_DIR)/librdf

# Compiler
CC=mpicc

# Debug flags
DEBUG=-g -O0

# Redland header files
INCLUDES=-I$(RAPTOR_DIR)/include/raptor2 -I$(RASQAL_DIR)/include/rasqal -I$(LIBRDF_DIR)/include

# CFLAGS
CFLAGS=$(DEBUG) $(INCLUDES) -Wall

# Redland libray path
LIBS=-L$(RAPTOR_DIR)/lib -lraptor2 -L$(RASQAL_DIR)/lib -lrasqal -L$(LIBRDF_DIR)/lib -lrdf

# PROV-IO header file
DYNLIB_INCLUDE=-I$(PROV_IO_PATH)/c/provio


DYNCFLAGS = $(DEBUG) $(INCLUDES) -Wall -fPIC 
DYNLIB_CFLAGS = $(CFLAGS) $(DYNLIB_INCLUDE)
DYNLDFLAGS = $(DEBUG) -shared -fPIC 
# DYNLDFLAGS = $(DEBUG) -dynamiclib -current_version 1.0 -fPIC $(LIBS)
LDFLAGS = $(DEBUG) $(LIBS)
DYNLIB_LDFLAGS=-L$(PROV_IO_PATH)/c/provio -lprovio


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

# Tests
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
			$(STATTEST_OBJ) $(STATTEST_EXE) $(STATTEST_DBUG) $(STATTEST_OUT) \
			$(CONFIGTEST_OBJ) $(CONFIGTEST_EXE) $(CONFIGTEST_DBUG) \
			$(LIBTEST_OBJ) $(LIBTEST_EXE) $(LIBTEST_DBUG) \
			$(DEPOBJ)

