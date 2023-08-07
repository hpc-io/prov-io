# PROV-IO

---
PROV-IO is an I/O-centric provenance management framework for scientific data. It provides an interface for data provenance tracking and stores provenance as RDF triples. [PROV-IO data model](https://github.com/hpc-io/prov-io/blob/master/doc/provio-latest.png) follows [W3C PROV-DM](https://www.w3.org/TR/prov-dm/) and is an extension of it. PROV-IO can be used to track HDF5 application's provenance by utilizing [HDF5 vol-provenance connector](https://github.com/hpc-io/vol-provenance). We have tested PROV-IO on Ubuntu 18.04, Redhat 8 and Cray Linux.

## Publications
Please cite the following paper if your project uses PROV-IO:  <br /> 
[PROV-IO: An I/O-Centric Provenance Framework for Scientific Data on HPC Systems](https://dl.acm.org/doi/10.1145/3502181.3531477) ([HPDC'22](https://www.hpdc.org/2022/)) [[Bibtex](https://github.com/hpc-io/prov-io/blob/master/doc/acm_3502181.3531477.bib)] <br /> 
Other pulications:  <br /> 
[Towards A Practical Provenance Framework for Scientific Data on HPC Systems](https://github.com/hpc-io/prov-io/blob/master/doc/FAST_22_WiP_PROV-IO.pdf) (poster@[FAST'22](https://www.usenix.org/conference/fast22)) <br />
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

## Dependencies
This is for building PROV-IO from scratch.
PROV-IO library needs to be built with ```libtool```. Install it by: <br /> 
```
sudo apt-get install gcc make
sudo apt-get install autoconf automake libtool pkg-config
```
PROV-IO's RDF schema is based on Redland ```librdf``` (including ```raptor2-2.0.15```, ```rasqal-0.9.33```, ```librdf-1.0.17```) and its Python binding (```redland-bindings-1.0.17.1```). Install the dependencies first: <br />  
```
sudo apt-get install libltdl-dev libxml2
```
We provide specific releases of ```librdf``` at: https://github.com/hpc-io/prov-io/tree/master/c/lib. Unzip and install them. <br />
First, install ```raptor2-2.0.15```:
```
cd raptor2-2.0.15
./autogen.sh
./configure --prefix=<your_install_path>/lib-raptor
make && make install
```
Export paths for ```lib-raptor```:
```
export CFLAGS=-I<your_install_path>/lib-raptor/include/raptor2
export LDFLAGS=-L<your_install_path>/lib-raptor/lib
export LIBS='-lraptor2'
export PKG_CONFIG_PATH=<your_install_path>/lib-raptor/lib/pkgconfig:$PKG_CONFIG_PATH
```
Next, install ```rasqal-0.9.33```:
```
cd rasqal-0.9.33
./autogen.sh
./configure --prefix=<your_install_path>/lib-rasqal
make && make install
```
Similarly, export paths for ```lib-rasqal```:
```
export CFLAGS=$CFLAGS' -I<your_install_path>/lib-rasqal/include/rasqal'
export LDFLAGS=$LDFLAGS' -L<your_install_path>/lib-rasqal/lib'
export LIBS=$LIBS' -lrasqal'
export PKG_CONFIG_PATH=<your_install_path>/lib-rasqal/lib/pkgconfig:$PKG_CONFIG_PATH
```
Install ```librdf-1.0.17```:
```
./autogen.sh
./configure --prefix=<your_install_path>/librdf [--enable-bdb]
make && make install
```
Finally, install the Python binding (```redland-bindings-1.0.17.1```):
```
./autogen.sh
./configure --with-python
make && make install
```

## PROV-IO Python Library
PROV-IO Python Library is to track workflow information defined in PROV-IO Extensible class.
Follow instructions in [python](https://github.com/hpc-io/prov-io/tree/master/python) to use it.

## PROV-IO HDF5 Lib Connector
PROV-IO HDF5 Lib Connector is used to track HDF5 I/O. Follow instructions to build it:
- Install HDF5 (provided in the repo). Build and install hdf5:
```
cd hdf5
./autogen.sh
```

- Build HDF5 vol-connector instrumented with PROV-IO:
```
```

## PROV-IO Syscall Wrapper
PROV-IO Syscall Wrapper is used to track frequently used POSIX I/O APIs. Follow instructions to build it:
- Build and install GOTCHA:
```
```
