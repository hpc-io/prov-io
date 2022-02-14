# Gotcha path
GOTCHA_DIR=/home/runzhou/Downloads/GOTCHA/build

# RDF libraries
SERD_DIR=/home/runzhou/Downloads/myhdfstuff/serd
REDLAND_DIR=/home/runzhou/Downloads/librdf

CC=gcc

DEBUG=-g -O0
LIBRDF_EXTERNAL_CPPFLAGS = -I/usr/local/include/raptor2 -I/usr/local/include/rasqal -I/usr/local/include/raptor2 
INCLUDES=-I$(HDF5_DIR)/include -I$(SERD_DIR)/include/serd $(LIBRDF_EXTERNAL_CPPFLAGS) -I/$(REDLAND_DIR)/src -I/$(GOTCHA_DIR)/include

CFLAGS = $(DEBUG) $(INCLUDES) -Wall
DYNCFLAGS = $(DEBUG) $(INCLUDES) -Wall -fPIC 

LIBRDF_DIRECT_LIBS =-L/usr/local/lib -lraptor2 -L/usr/local/lib -lrasqal -lraptor2 -L/$(REDLAND_DIR)/src -lrdf
LIBS=-L$(SERD_DIR)/build -lz -lserd-0 $(LIBRDF_DIRECT_LIBS) -L/$(GOTCHA_DIR)/lib -lgotcha
DYNLDFLAGS = $(DEBUG) -shared -fPIC 
LDFLAGS = $(DEBUG) $(LIBS)

# Shared library VOL connector
# DYNSRC = H5VLprovnc.c
# DYNOBJ = $(DYNSRC:.c=.o)
# DYNLIB = libh5prov.so
#DYNLIB = libh5prov.dylib
#DYNDBG = libh5prov.dylib.dSYM

# Testcase section
EXSRC = syscall_wrapper.c
EXOBJ = $(EXSRC:.c=.o)
EXEXE = $(EXSRC:.c=)

# GOTCHA example section
EXAMPLE = test_dlopen.c
EXAMOBJ = $(EXSRC:.c=.o)
EXAMEXE = $(EXSRC:.c=)

all: $(EXEXE) $(DYNLIB)
# all: $(DYNLIB)


$(EXEXE): $(EXSRC) $(STATLIB) $(DYNLIB)
		$(CC) $(CFLAGS) $^ -o $(EXEXE) $(LDFLAGS)

.PHONY: clean all
clean:
		rm -rf $(EXOBJ) $(EXAMOBJ) $(EXEXE) $(EXAMEXE) $(EXDBG)
