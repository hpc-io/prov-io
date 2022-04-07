#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"


#define CFG_LINE_LEN_MAX 510
#define INITIAL_CAPACITY 62  // 62 H5VL_provenance methods in total


/* Configuration parser */
void prov_params_init(prov_config* params_out){
    if(!params_out)
        params_out = (prov_config*) calloc(1, sizeof(prov_config));
    (*params_out).prov_base_uri = NULL;
    (*params_out).prov_prefix = NULL;
    (*params_out).stat_file_path = NULL;
    (*params_out).new_graph_path = NULL;
    (*params_out).legacy_graph_path = NULL;
    (*params_out).prov_line_format = NULL;
    (*params_out).enable_stat_file = 0;
    (*params_out).enable_legacy_graph = 0;
    (*params_out).enable_file_prov = 0;
    (*params_out).enable_group_prov = 0;
    (*params_out).enable_dataset_prov = 0;
    (*params_out).enable_attr_prov = 0;
    (*params_out).enable_dtype_prov = 0;
    (*params_out).enable_api_prov = 0;
    (*params_out).enable_duration_prov = 0;
    (*params_out).enable_program_prov = 0;
    (*params_out).enable_thread_prov = 0;
    (*params_out).enable_user_prov = 0;
    (*params_out).enable_bdb = 0;
    (*params_out).num_of_apis = INITIAL_CAPACITY;
    (*params_out).prov_level = Default;
}

#define CFG_DELIMS "=\n"

char* _parse_val(char* val_in){
    char* val_str = strdup(val_in);
    char *tok = strtok(val_str, "*");
    char* val = NULL;
    val = strdup(tok);
//    printf("_parse_val: val_in = [%s], val = [%s]\n", val_in, val);
    if(val_str)
        free(val_str);
    return val;
}

int _set_params(char *key, char *val_in, prov_config* params_in_out) {
    if (!params_in_out)
        return 0;
    char* val = _parse_val(val_in);

    if (strcmp(key, "BASE_URI") == 0){
        if (strcmp(val, "") == 0){
            printf("Blank base URI!\n");
            // return -1;
        } else {
            (*params_in_out).prov_base_uri = strdup(val);
        }
    } else if (strcmp(key, "PREFIX") == 0){
        if (strcmp(val, "") == 0){
            printf("Blank PREFIX!\n");
            // return -1;
        } else {
            (*params_in_out).prov_prefix = strdup(val);
        }
    } else if(strcmp(key, "STAT_FILE_PATH") == 0) {
        if (strcmp(val, "") == 0){
            printf("Blank stat file path!\n");
            return -1;
        } else {
            (*params_in_out).stat_file_path = strdup(val);
        }
    } else if(strcmp(key, "NEW_GRAPH_PATH") == 0) {
        if (strcmp(val, "") == 0){
            printf("Blank provenance file path!\n");
            return -1;
        } else {
            (*params_in_out).new_graph_path = strdup(val);
        }
    } else if(strcmp(key, "LEGACY_GRAPH_PATH") == 0) {
        if (strcmp(val, "") == 0){
            printf("Blank provenance file path!\n");
            return -1;
        } else {
            (*params_in_out).legacy_graph_path = strdup(val);
        }
    } else if(strcmp(key, "FORMAT") == 0) {
        if (strcmp(val, "") == 0){
            printf("Blank provenace format!\n");
            return -1;
        } else {
            (*params_in_out).prov_line_format = strdup(val);
        }
    } 
    else if(strcmp(key, "PROV_LEVEL") == 0) {
        if (strcmp(val, "") == 0){
            printf("Blank provenace level!\n");
            return -1;
        } else {
            (*params_in_out).prov_level = atoi(val);
        }
    } 
    else if (strcmp(key, "ENABLE_STAT_FILE") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_stat_file = 1;
        else
            (*params_in_out).enable_stat_file = 0;
    } else if (strcmp(key, "ENALBE_LEGACY_GRAPH") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_legacy_graph = 1;
        else
            (*params_in_out).enable_legacy_graph = 0;
    }  else if (strcmp(key, "ENABLE_FILE") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_file_prov = 1;
        else
            (*params_in_out).enable_file_prov = 0;
    }  else if (strcmp(key, "ENABLE_GROUP") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_group_prov = 1;
        else
            (*params_in_out).enable_group_prov = 0;
    }  else if (strcmp(key, "ENABLE_DATASET") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_dataset_prov = 1;
        else
            (*params_in_out).enable_dataset_prov = 0;
    }  else if (strcmp(key, "ENABLE_ATTR") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_attr_prov = 1;
        else
            (*params_in_out).enable_attr_prov = 0;
    } else if (strcmp(key, "ENABLE_DTYPE") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_dtype_prov = 1;
        else
            (*params_in_out).enable_dtype_prov = 0;
    } else if (strcmp(key, "ENABLE_DURATION") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_duration_prov = 1;
        else
            (*params_in_out).enable_duration_prov = 0;
    } else if (strcmp(key, "ENABLE_THREAD") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_thread_prov = 1;
        else
            (*params_in_out).enable_thread_prov = 0;
    } else if (strcmp(key, "ENABLE_USER") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_user_prov = 1;
        else
            (*params_in_out).enable_user_prov = 0;
    } else if (strcmp(key, "ENABLE_API") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_api_prov = 1;
        else
            (*params_in_out).enable_api_prov = 0;
    } else if (strcmp(key, "ENABLE_PROGRAM") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_program_prov = 1;
        else
            (*params_in_out).enable_program_prov = 0;
    } else if (strcmp(key, "NUM_OF_APIS") == 0) {
        if (val[0] > 0)
            (*params_in_out).enable_program_prov = atoi(val);
    } else if (strcmp(key, "ENABLE_BDB") == 0) {
        if (val[0] == 'T' || val[0] == 't')
            (*params_in_out).enable_bdb = 1;
        else
            (*params_in_out).enable_bdb = 0;
    }

    if(val)
        free(val);
    return 1;
}

int read_config(const char *file_path, prov_config *params_out) {
    char cfg_line[CFG_LINE_LEN_MAX] = "";

    if (!params_out)
        params_out = (prov_config*) calloc(1, sizeof(prov_config));
    else
        memset(params_out, 0, sizeof(prov_config));
    //Default settings
    prov_params_init(params_out);
    (*params_out).stat_file_path = strdup(file_path);

    FILE *file = fopen(file_path, "r");

    int parsed = 1;

    while (fgets(cfg_line, CFG_LINE_LEN_MAX, file) && (parsed == 1)) {
        if (cfg_line[0] == '*') {
            continue;
        }
        char *tokens[2];
        char *tok = strtok(cfg_line, CFG_DELIMS);
        if (tok) {
            tokens[0] = tok;
            tok = strtok(NULL, CFG_DELIMS);
            if (tok){
                tokens[1] = tok;
            }
            else
                return -1;
        } else 
            return -1; 
        parsed = _set_params(tokens[0], tokens[1], params_out);
    }
    if (parsed < 0) 
        return -1;
    return 0;
}

/* Load configuration based on environmental variables */
void load_config(prov_config* config) {
    const char *path = getenv("PROVIO_CONFIG");
    if (path) {
        read_config(path, config);
    }
}
