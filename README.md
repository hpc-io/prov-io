# PROV-IO

## **Note:** This repository is not ready to be used yet and is still under construction. The finalized README will come out soon!
---
PROV-IO is a provenance management framework for scientific data I/O libraries. It provides a library for provenance tracking based on an I/O-centric, [W3C PROV-DM](https://www.w3.org/TR/prov-dm/)-compliant [provenance model](https://github.com/hpc-io/prov-io/blob/master/provio_ontology.ttl) and provenance storage based on an RDF schema. PROV-IO has been tested on Ubuntu 18.04.1 and Cray Linux (Cori at NERSC/LBNL).

## Publications
Please cite the following paper if you use PROV-IO:  <br /> 
[PROV-IO: An I/O-Centric Provenance Framework for Scientific Data on HPC Systems](https://www.hpdc.org/2022/) ([HPDC'22](https://www.hpdc.org/2022/)) [[Bibtex]()] <br /> 
Other pulications:  <br /> 
[Towards A Practical Provenance Framework for Scientific Data on HPC Systems](https://github.com/hpc-io/prov-io/blob/master/doc/FAST_22_WiP_PROV-IO.pdf) (poster@[FAST'22](https://www.usenix.org/conference/fast22)) <br />

## Dependencies
sudo apt-get install gcc make
sudo apt-get install autoconf automake libtool pkg-config

librdf
librdf Python binding
openvirtuoso (optional)

- To use instrumented hdf5 vol provenance connector
  - hdf5 (provided in the repo)


- pip install graphviz
