#ifndef _PROVIO_INCLUDE_CONFIG_H_
#define _PROVIO_INCLUDE_CONFIG_H_


/* Provenance parameters */
typedef struct prov_params {
    char* prov_base_uri;
    char* prov_prefix;
    char* stat_file_path;
    char* new_graph_path;
    char* legacy_graph_path;
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
} prov_params;


void _load_config(prov_params* params)

#endif
