# PROV-IO

---
PROV-IO is a provenance management framework for scientific data I/O libraries. It provides a library for provenance tracking based on an I/O-centric, [W3C PROV-DM](https://www.w3.org/TR/prov-dm/)-compliant [provenance model](https://github.com/hpc-io/prov-io/blob/master/provio_ontology.ttl) ([figure](https://github.com/hpc-io/prov-io/blob/master/doc/provio-latest.png)) and provenance storage based on an RDF schema. PROV-IO can be used by instrumenting it into C/C++ I/O libraries. In this repository we provide an instrumented HDF5 vol-provenance connector and an instrumented POSIX I/O-related Syscall wrapper. PROV-IO has been tested on Ubuntu 18.04.1 and Cray Linux (Cori at NERSC/LBNL). 

## Publications
Please cite the following paper if you use PROV-IO:  <br /> 
[PROV-IO: An I/O-Centric Provenance Framework for Scientific Data on HPC Systems](https://www.hpdc.org/2022/) ([HPDC'22](https://www.hpdc.org/2022/)) [[Bibtex]()] <br /> 
Other pulications:  <br /> 
[Towards A Practical Provenance Framework for Scientific Data on HPC Systems](https://github.com/hpc-io/prov-io/blob/master/doc/FAST_22_WiP_PROV-IO.pdf) (poster@[FAST'22](https://www.usenix.org/conference/fast22)) <br />

## Dependencies
PROV-IO needs to be built with ```libtool```. Install it by: <br /> 
```
sudo apt-get install gcc make
sudo apt-get install autoconf automake libtool pkg-config
sudo apt-get install python python-dev
(Python extensible class:)
python3
```
PROV-IO's RDF schema is currently based on Redland ```librdf``` and its Python binding. <br /> 
[Please see the instruction on installing librdf here](https://librdf.org/INSTALL.html) <br /> 
[Please see the instruction on installing librdf here](https://librdf.org/bindings/) <br /> 
It will suppurt other RDF backend in the future: <br /> 
```openvirtuoso``` (optional) <br /> 

MPI <br /> 

- To use instrumented hdf5 vol provenance connector
  - hdf5 (provided in the repo). Build and install hdf5:
```
cd hdf5
./autogen.sh
```

- To use instrumented POSIX Syscall wrapper
  - GOTCHA. Build and install GOTCHA:
```
```

Redland RDF library <br />

- Raptor
 - https://librdf.org/raptor/INSTALL.html
- Rasqal
 - https://librdf.org/rasqal/INSTALL.html
- librdf
 - https://librdf.org/INSTALL.html
- librdf bindings
 - https://librdf.org/INSTALL.html
 - Only build Python part

To use Python extensible class and also a guide for docker file: <br />  

export PKG_CONFIG_PATH <br /> 
export PYTHON_PATH <br /> 
export LD_LIBRARY_PATH (add libraptor librasqal librdf)<br /> 
