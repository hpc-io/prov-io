# PROV-IO

---
PROV-IO is an I/O-centric provenance management framework for scientific data. It provides an interface for data provenance tracking and stores provenance as RDF triples. [PROV-IO data model](https://github.com/hpc-io/prov-io/blob/master/doc/provio-latest.png) follows [W3C PROV-DM](https://www.w3.org/TR/prov-dm/) and is an extension of it. PROV-IO can be used to track provenance of HDF5 data by being integarted with [HDF5 vol-provenance connector](https://github.com/hpc-io/vol-provenance). PROV-IO has been tested on Ubuntu 18.04 and Cray Linux.

## Publications
Please cite the following paper if your project uses PROV-IO:  <br /> 
[PROV-IO: An I/O-Centric Provenance Framework for Scientific Data on HPC Systems](https://dl.acm.org/doi/10.1145/3502181.3531477) ([HPDC'22](https://www.hpdc.org/2022/)) [[Bibtex](https://github.com/hpc-io/prov-io/blob/master/doc/acm_3502181.3531477.bib)] <br /> 
Other pulications:  <br /> 
[Towards A Practical Provenance Framework for Scientific Data on HPC Systems](https://github.cunder development om/hpc-io/prov-io/blob/master/doc/FAST_22_WiP_PROV-IO.pdf) (poster@[FAST'22](https://www.usenix.org/conference/fast22)) <br />
[PROV-IO+: A Cross-Platform Provenance Framework for Scientific Data on HPC Systems](https://arxiv.org/abs/2308.00891) (preprint)<br />

## Docker
The easiest way of trying out PROV-IO is through ```docker```. PROV-IO ```docker``` image is available now at [rzhan/prov-io](https://hub.docker.com/repository/docker/rzhan/prov-io). The image is based on Debian 11 with Python 3.9 installed. Download the basic PROV-IO ```docker``` image:
```
docker pull rzhan/prov-io:1.0
```
We also publish the ```docker``` image of [Megatron-LM](https://github.com/NVIDIA/Megatron-LM) instrumented with PROV-IO as an example use case. Download the instrumented Megatron-LM docker image:
```
docker pull rzhan/prov-io:megatron-lm
```

## Build from Scratch
This is for building PROV-IO from scratch.

### Dependencies
PROV-IO library needs to be built with ```libtool```. Install it by: <br /> 
```
sudo apt-get install -y gcc make
sudo apt-get install -y autoconf automake libtool pkg-config gtk-doc-tools 
```
PROV-IO's RDF schema is based on Redland ```librdf``` (including ```raptor2-2.0.15```, ```rasqal-0.9.33```, ```librdf-1.0.17```) and its Python binding (```redland-bindings-1.0.17.1```). Install their dependencies first: <br />  
```
sudo apt-get install -y libltdl-dev libxml2 libxml2-dev flex bison swig uuid uuid-dev
```
We provide specific releases of ```librdf``` at: https://github.com/hpc-io/prov-io/tree/master/packages. Unzip and install them in the sequence of ```raptor2-2.0.15```->```rasqal-0.9.33```->```librdf-1.0.17```->```redland-bindings-1.0.17.1```. <br />

For example, install ```raptor2-2.0.15``` and export path:
```
cd raptor2-2.0.15
./autogen.sh
./configure --prefix=<your_prov_io_path>/lib/lib-raptor
make && make install
export PKG_CONFIG_PATH=<your_prov_io_path>/lib/lib-raptor/lib/pkgconfig:$PKG_CONFIG_PATH
```
Then, install ```rasqal-0.9.33``` and ```librdf-1.0.17``` with similar commands with correct path. <br />
Finally, install the Python binding (```redland-bindings-1.0.17.1```):
```
./autogen.sh
./configure --with-python
make && make install
```

## PROV-IO Python Library
PROV-IO Python library tracks provenance information defined in PROV-IO Extensible class.
Follow instructions in [python](https://github.com/hpc-io/prov-io/tree/master/python) to use it.


## PROV-IO C Library
PROV-IO C library tracks low level I/O information. 
Build PROV C library:
```
cd c/provio
```
Open ```Makefile```, set ```PROV_IO_PATH``` to ```<your_prov_io_path>```, save and quit, then:
```
make
export LD_LIBRARY_PATH=<your_prov_io_path>/c/provio
```
To run a basic PROV-IO test, in the same directory: 
```
export PROVIO_CONFIG=<your_prov_io_path>/doc/example_config/prov.cfg
./provio_test
```
Check out the provenance file (```prov.turtle```) and stat file (```prov.stat```) generated by PROV-IO.


### Tracking HDF5 Applications with HDF5 VOL Connector
PROV-IO HDF5 Lib Connector is used to track HDF5 I/O. Follow instructions to build it:
- Install HDF5 (provided in the repo: https://github.com/hpc-io/prov-io/tree/master/packages). Build and install hdf5:
```
cd hdf5
./autogen.sh
```

- Build HDF5 vol-connector instrumented with PROV-IO:
```
```

## PROV-IO Syscall Wrapper
PROV-IO Syscall Wrapper is used to track frequently used POSIX I/O APIs. It's based on [LLNL's GOTACH project](https://github.com/LLNL/GOTCHA). <br />
Syscall Wrapper has not been fully tested yet, stay tuned!
