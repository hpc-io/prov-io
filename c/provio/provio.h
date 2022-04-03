#ifndef _PROVIO_INCLUDE_PROVIO_H_
#define _PROVIO_INCLUDE_PROVIO_H_


#include <stdio.h>
#include <pthread.h>
#include "rdf.h"


typedef enum ProvLevel {
    Default, //no file write, only screen print
    Print_only,
    File_only,
    File_and_print,
    Level3,
    Level4,
    Disabled
}Prov_level;

typedef enum ProvInfoLevel {
    Base, 
    Performance,
    Region
}ProvInfo_Level;

typedef struct ProvenanceHelper {
    /* Provenance properties */
    char* prov_file_path;
    FILE* prov_file_handle;
    FILE* stat_file_handle;
    Prov_level prov_level;
    char* prov_line_format;
    char user_name[32];
    int pid;
    pthread_t tid;
    char proc_name[64];
    int ptr_cnt;
    // int opened_files_cnt;
    // file_prov_info_t* opened_files;//linkedlist,
} provio_helper_t;


struct prov_fields {
    char data_object[512];              // Name of the data object
    char io_api[512];                   // H5G/H5D/H5A/H5T
    char program_name[512];             // Name of the program
#ifdef H5_HAVE_PARALLEL
    char mpi_rank[128];                 // MPI rank ID
#endif
    char user[512];                     // Current user
    unsigned long duration;             // I/O API duration
    char type[128];                     // Data object type: Group/Dataset/Attr/Datatype
    char relation[128];                 // relation between data object and I/O API
} prov_fields;

int prov_write(provio_helper_t* helper_in, struct prov_fields* fields);

void load_config(prov_params* params) {
	
}

#endif



