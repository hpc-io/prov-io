#Your HDF5 install path
HDF5_DIR=/home/runzhou/Downloads/myhdfstuff/hdf5-develop/build/hdf5
REDLAND_DIR=/home/runzhou/Downloads/librdf
PROVIO_DIR=/home/runzhou/PROV-IO/c/provio

CC=mpicc
# CC=gcc
AR=ar


DEBUG=-g -O0
LIBRDF_EXTERNAL_CPPFLAGS = -I/usr/local/include/raptor2 -I/usr/local/include/rasqal
LIRPROVIO_CFALGS=-I/home/runzhou/PROV-IO/c/provio
INCLUDES=-I$(HDF5_DIR)/include $(LIBRDF_EXTERNAL_CPPFLAGS) -I/$(REDLAND_DIR)/src $(LIRPROVIO_CFALGS)

CFLAGS = $(DEBUG) -fPIC $(INCLUDES) -Wall
DYNCFLAGS = $(DEBUG) $(INCLUDES) -Wall -fPIC 

#LIBS=-L$(HDF5_DIR)/lib -L$(MPI_DIR)/lib -lhdf5 -lz
LIBRDF_DIRECT_LIBS =  -L/usr/local/lib -lraptor2 -L/usr/local/lib -lrasqal -L/$(REDLAND_DIR)/src -lrdf -L/$(PROVIO_DIR) -lprovio
LIBS=-L$(HDF5_DIR)/lib -lhdf5 -lz $(LIBRDF_DIRECT_LIBS)
DYNLDFLAGS = $(DEBUG) -shared -fPIC 
# DYNLDFLAGS = $(DEBUG) -dynamiclib -current_version 1.0 -fPIC $(LIBS)
LDFLAGS = $(DEBUG) $(LIBS)
ARFLAGS = rs

# Shared library VOL connector
DYNSRC = H5VLprovnc.c
DYNOBJ = $(DYNSRC:.c=.o)
DYNLIB = libh5prov.so
#DYNLIB = libh5prov.dylib
#DYNDBG = libh5prov.dylib.dSYM

# Testcase section
EXSRC = vpicio_uni_h5.c
EXOBJ = $(EXSRC:.c=.o)
EXEXE = $(EXSRC:.c=.exe)
EXDBG = $(EXSRC:.c=.exe.dSYM)

all: $(EXEXE) $(DYNLIB)
# all: $(DYNLIB)


$(EXEXE): $(EXSRC) $(STATLIB) $(DYNLIB)
		$(CC) $(CFLAGS) $^ -o $(EXEXE) $(LDFLAGS)

$(DYNLIB): $(DYNSRC)
		$(CC) $(DYNCFLAGS) $(DYNSRC) -c -o $(DYNOBJ)
		$(CC) $(DYNOBJ) $(DYNLDFLAGS) $(LIBS) -o $(DYNLIB)

.PHONY: clean all
clean:
		rm -rf $(DYNOBJ) $(DYNLIB) $(DYNDBG) \
			$(EXOBJ) $(EXEXE) $(EXDBG)
