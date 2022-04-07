#ifndef _PROVIO_INCLUDE_RDF_H_
#define _PROVIO_INCLUDE_RDF_H_

// #ifdef LIBRDF_H
#include <redland.h>
#include "librdf.h"
// #endif /* Redland RDF */

/* This structure contains provenance information */
// struct prov_fields {
//     char data_object[512];              // Name of the data object
//     char io_api[512];                   // H5G/H5D/H5A/H5T
//     char program_name[512];             // Name of the program
// #ifdef H5_HAVE_PARALLEL
//     char mpi_rank[128];                 // MPI rank ID
// #endif
//     char user[512];                     // Current user
//     unsigned long duration;             // I/O API duration
//     char type[128];                     // Data object type: Group/Dataset/Attr/Datatype
//     char relation[128];                 // relation between data object and I/O API
// } prov_fields;


#ifdef LIBRDF_H
/* Redland global variables */
librdf_world* world;
librdf_storage* storage_prov;
librdf_model* model_prov;
librdf_statement *statement;
librdf_serializer* serializer;
librdf_uri* base_uri;
librdf_uri* provio_uri;
librdf_uri* node_prefix;
#endif /* Redland RDF */

#ifdef LIBRDF_H
/* Wrapper layer for corresponding RDF backend */
#endif /* Redland RDF */

#ifdef ENABLE_VIRTUOSO
/* Wrapper layer for corresponding RDF backend */
#endif /* Redland RDF */

#endif /* _PROVIO_INCLUDE_RDF_H_ */