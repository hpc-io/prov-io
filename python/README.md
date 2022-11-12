# PROV-IO Python Library

## Instruction
To use PROV-IO Python library, first set environmental variables with following commands:
```
# Export Redland library path
export LD_LIBRARY_PATH=<your_libraptor_path>/libraptor/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=<your_librasqal_path>/librasqal/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=<your_librdf_path>/librdf/lib:$LD_LIBRARY_PATH

# Export Redland binding path
export PYTHONPATH=<your_redland_binding_path>//redland-bindings/python:$PYTHONPATH

# Export PROV-IO path
export PYTHONPATH=<your_provio_path>/prov-io/python/src:$PYTHONPATH

# Export Extensible class directory
export SUBCLASS_PATH=<your_provio_path>/prov-io/python/extensible_class/
```

An example is provided in [test/test_provenance.py](https://github.com/hpc-io/prov-io/blob/master/python/test/test_provenance.py). This example shows how to use PROV-IO APIs. You can also use it test if you installed PROV-IO correctly.