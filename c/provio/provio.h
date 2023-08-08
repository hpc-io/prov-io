#ifndef _PROVIO_INCLUDE_PROVIO_H_
#define _PROVIO_INCLUDE_PROVIO_H_


#include <stdio.h>
#include <pthread.h>
// #ifdef PARALLEL
#include <mpi.h> 
// #endif


#include "config.h"
#include "stat.h"



typedef struct PROVIOHelper {
    FILE* legacy_prov_file_handle;
    FILE* new_prov_file_handle;
    FILE* stat_file_handle;
} provio_helper_t;


typedef struct prov_fields {
    char data_object[512];              // Name of the data object
    char io_api[512];                   // H5G/H5D/H5A/H5T
    char proc_name[1024];             // Name of the program
    char proc_uuid[512];
    // char* proc_name;             // Name of the program
    // char* proc_uuid;
    char proc_start_time[64];
    char proc_end_time[64];
    char pid[32];
    char tid[32];
// #ifdef MPI_INCLUDED
    int mpi_rank_int;
    char mpi_rank[128];                 // MPI rank ID
// #endif
    char user_name[32];                 // Current user
    unsigned long duration;             // I/O API duration
    char type[128];                     // Data object type: Group/Dataset/Attr/Datatype
    char relation[128];                 // relation between data object and I/O API
} prov_fields;


/* statistics */
Stat prov_stat;
duration_ht* FUNCTION_FREQUENCY;

/* User APIs */
void provio_init(prov_config* config, prov_fields* fields);
void provio_term(prov_config* config, prov_fields* fields);

provio_helper_t* provio_helper_init(prov_config* config, prov_fields* fields);
void provio_helper_teardown(prov_config* config, provio_helper_t* helper, prov_fields* fields);

// Fill in data object name and api name
void prov_fill_data_object(prov_fields* fields, const char* obj_name, const char* type);
void prov_fill_relation(prov_fields* fields, const char* relation);
void prov_fill_io_api(prov_fields* fields, const char* io_api, unsigned long duration);

int add_prov_record(prov_config* config, provio_helper_t* helper_in, prov_fields* fields);
int add_program_record(prov_config* config, prov_fields* fields);
// int add_user_record_Redland(prov_config* config, prov_fields* fields);
// int add_mpi_rank_record_Redland(prov_config* config, prov_fields* fields);
// int add_program_record_Redland(prov_config* config, prov_fields* fields);
// int add_thread_record_Redland(prov_config* config, prov_fields* fields);
// int add_io_api_record_Redland(prov_config* config, prov_fields* fields, char* time, char* duration_);
// int add_data_obj_record_Redland(prov_config* config, prov_fields* fields);

// function level stat helper
void func_stat(const char* func_name, unsigned long elapsed);

#endif 
