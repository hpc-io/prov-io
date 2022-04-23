# Provenance Query
PROV-IO uses SPQRQL to query provenance stored in RDF triples. <br />
We currently provide two example querys using Python as the SPARQL endpoint ([field.py](https://github.com/hpc-io/prov-io/blob/master/user_engine/query/field.py) & [top_accuracy.py](https://github.com/hpc-io/prov-io/blob/master/user_engine/query/top_accuracy.py)). <br />


## Example provenance in turtle format
```
@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix prov: <http://www.w3.org/ns/prov#> .
@prefix provio: <http://www.w3.org/ns/provio#> .
@prefix file: </> .

<Bob>
    <prov:ofType> <prov:Agent> ;
    <prov:wasMemberOf> <provio:User> .
<MPI_rank_0>
    <prov:actedOnBehalfOf> <Bob> ;
    <prov:ofType> <prov:Agent> ;
    <prov:wasMemberOf> <provio:Thread> .
<vpicio_uni_h5.exe--a1>
    <prov:actedOnBehalfOf> <MPI_rank_0> ;
    <prov:ofType> <prov:Agent> ;
    <prov:wasMemberOf> <provio:Program> .
<H5Dcreate2--b1>
    <prov:ofType> <prov:Activity> ;
    <prov:wasMemberOf> <provio:IOAPI:Create> ;
    <prov:wasAssociatedWith> <vpicio_uni_h5.exe--a1> .
</Timestep_0/x>
    <prov:ofType> <prov:Entity> ;
    <prov:wasAttributedTo> <vpicio_uni_h5.exe--a1> ;
    <prov:wasCreatedBy> <H5Dcreate2--b1> ;
    <prov:wasMemberOf> <provio:DataObject:Dataset> .
```
## Example provenance visualization result
![alt text](https://github.com/hpc-io/prov-io/blob/master/user_engine/visualizer/example_prov.png)



