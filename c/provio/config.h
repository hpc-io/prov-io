#ifndef _PROVIO_INCLUDE_CONFIG_H_
#define _PROVIO_INCLUDE_CONFIG_H_


typedef enum ProvInfoLevel {
    Base, 
    Performance,
    Region
} ProvInfo_Level;

typedef enum ProvLevel {
    Default, //no file write, only screen print
    Print_only,
    File_only,
    File_and_print,
    Level3,
    Level4,
    Disabled
}Prov_level;


/* Provenance parameters */
typedef struct prov_config {
    char* prov_base_uri;
    char* prov_prefix;
    char* stat_file_path;
    char* new_graph_path;
    char* legacy_graph_path;
    char* prov_line_format;
    int enable_stat_file;
    int enable_legacy_graph;
    int enable_file_prov;
    int enable_group_prov;
    int enable_dataset_prov;
    int enable_attr_prov;
    int enable_dtype_prov;
    int enable_api_prov;
    int enable_duration_prov;
    int enable_program_prov;
    int enable_thread_prov;
    int enable_user_prov;
    int enable_bdb;
    int num_of_apis;      
    Prov_level prov_level;      
} prov_config;

void load_config(prov_config* config);

#endif