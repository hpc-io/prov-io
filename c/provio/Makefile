#Your HDF5 install path
HDF5_DIR=/home/runzhou/Downloads/myhdfstuff/hdf5-develop/build/hdf5
REDLAND_DIR=/home/runzhou/Downloads/librdf

CC=mpicc
# CC=gcc
AR=ar


DEBUG=-g -O0
LIBRDF_EXTERNAL_CPPFLAGS = -I/usr/local/include/raptor2 -I/usr/local/include/rasqal
INCLUDES=-I$(HDF5_DIR)/include $(LIBRDF_EXTERNAL_CPPFLAGS) -I/$(REDLAND_DIR)/src

CFLAGS = $(DEBUG) -fPIC $(INCLUDES) -Wall
DYNCFLAGS = $(DEBUG) $(INCLUDES) -Wall -fPIC 

#LIBS=-L$(HDF5_DIR)/lib -L$(MPI_DIR)/lib -lhdf5 -lz
LIBRDF_DIRECT_LIBS =  -L/usr/local/lib -lraptor2 -L/usr/local/lib -lrasqal -L$(REDLAND_DIR)/src -lrdf
LIBS=-L$(HDF5_DIR)/lib -lhdf5 -lz $(LIBRDF_DIRECT_LIBS)
DYNLDFLAGS = $(DEBUG) -shared -fPIC 
# DYNLDFLAGS = $(DEBUG) -dynamiclib -current_version 1.0 -fPIC $(LIBS)
LDFLAGS = $(DEBUG) $(LIBS)
ARFLAGS = rs


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
EXSRC = stat_test.c
EXOBJ = $(EXSRC:.c=.o)
EXEXE = $(EXSRC:.c=)
EXDBG = $(EXSRC:.c=.dSYM)

all: $(EXEXE) $(DYNLIB)
# all: $(DYNLIB)


$(EXEXE): $(EXSRC) $(STATLIB) $(DYNLIB)
		$(CC) $(CFLAGS) $^ -o $(EXEXE) $(LDFLAGS)

$(DYNLIB): $(DYNSRC)
		$(CC) $(DYNCFLAGS) $(STATSRC) -o $(STATOBJ) -c
		$(CC) $(DYNCFLAGS) $(CONFSRC) -o $(CONFOBJ) -c
		$(CC) $(DYNCFLAGS) $(DYNSRC) -o $(DYNOBJ) -c
		$(CC) $(STATOBJ) $(DYNOBJ) $(DYNLDFLAGS) $(LIBS) -o $(DYNLIB)

.PHONY: clean all
clean:
		rm -rf $(DYNOBJ) $(DYNLIB) $(DYNDBG) \
			$(EXOBJ) $(EXEXE) $(EXDBG) \
			$(DEPOBJ) 
# 		rm -rf $(DYNOBJ) $(DYNLIB) $(DYNDBG) 
# 			$(EXOBJ) $(EXEXE) $(EXDBG)
