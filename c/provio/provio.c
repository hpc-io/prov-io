#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <uuid/uuid.h>
#include <string.h>
#include <unistd.h>


#include "provio.h"


#define DEFAULT_FUNCTION_PREFIX "H5VL_provenance_"
#define LEGACY_PREFIX "file"

/* Global variables */
// Process
int PROC_ID;

// Thread
int THREAD_ID;

// Flags
int MPI_RANK_TRACKED = 0;   
int PROC_NAME_TRACKED = 0;
int USER_TRACKED = 0;


/* Helper functions */
static void get_time_str(char *str_out);
static int get_mpi_rank(prov_fields* fields);
static void alloc_proc_uuid(prov_fields* fields);
static void alloc_api_uuid(prov_fields* fields);
// static char* add_prefix();
static void get_process_name_by_pid(prov_fields* fields, int pid);


static void get_time_str(char *str_out){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    *str_out = '\0';
    sprintf(str_out, "%d/%d/%d %d:%d:%d", timeinfo->tm_mon + 1, timeinfo->tm_mday, 
        timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}


/* Get RANK ID */
static int get_mpi_rank(prov_fields* fields) {
    // MPI rank 
    int MPI_RANK;
    int NUM_OF_RANK;
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_RANK); 
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_OF_RANK);
    sprintf(fields->mpi_rank,"%d", MPI_RANK); 
    return MPI_RANK;
}

/* Provenance helper methods */
static void alloc_proc_uuid(prov_fields* fields) {
    uuid_t uuid;
    uuid_generate_time_safe(uuid);
    char* uuid_ = malloc(37);
    uuid_unparse_lower(uuid, uuid_);
    strcat(fields->proc_name, "--");
    strcat(fields->proc_name, uuid_);
    strcpy(fields->proc_uuid, uuid_); 
}

static void alloc_api_uuid(prov_fields* fields) {
    uuid_t uuid;
    uuid_generate_time_safe(uuid);
    char* uuid_ = malloc(37);
    uuid_unparse_lower(uuid, uuid_);
    strcat(fields->io_api, "--");
    strcat(fields->io_api, uuid_);
}

// static char* add_prefix(char* node) {
//     char* tmp = malloc(64);
//     strcpy(tmp, "file:");
//     strcat(tmp, node);
//     return tmp;
// }

static void get_process_name_by_pid(prov_fields* fields, int pid)
{
    char* name = (char*)calloc(2048,sizeof(char));
    if(name){
        sprintf(name, "/proc/%d/cmdline",pid);
        FILE* f = fopen(name,"r");
        if(f){
            size_t size;
            size = fread(name, sizeof(char), 1024, f);
            if(size>0){
                if('\n'==name[size-1])
                    name[size-1]='\0';
            }
            fclose(f);
        }
    }
    strcpy(fields->proc_name, name);  
}


// static void free_fields(prov_fields* fields) {
//     free((char*)fields->proc_name);
//     free((char*)fields->proc_uuid);
// }

void prov_fill_data_object(prov_fields* fields, const char* obj_name, 
    const char* type) {
    strcpy(fields->data_object, obj_name);
    // if (fields->proc_uuid) {
    //     strcat(fields->data_object, "--");
    //     strcat(fields->data_object, fields->proc_uuid);
    // }
    strcpy(fields->type, type);      
}


void prov_fill_relation(prov_fields* fields, const char* relation) {
    strcpy(fields->relation, relation);
}


void prov_fill_io_api(prov_fields* fields, const char* io_api, unsigned long duration) {
    strcpy(fields->io_api, io_api);  
    fields->duration = duration; 
}


void func_stat(const char* func_name, unsigned long elapsed) {
    accumulate_duration(FUNCTION_FREQUENCY, func_name, elapsed);
}


/* Initialize provenance helper */
provio_helper_t* provio_helper_init(prov_config* config, prov_fields* fields) {

    get_time_str(fields->proc_start_time);

    /* Load configuration */
    assert(config->prov_level);
    assert(config->enable_stat_file);
    assert(config->stat_file_path);
    assert(config->prov_line_format);
    // if (!config->legacy_graph_path) 
    //     assert(config->new_graph_path);
    // if (!config->new_graph_path) 
    //     assert(config->legacy_graph_path);


    provio_helper_t* new_helper = (provio_helper_t *)calloc(1, sizeof(provio_helper_t));

    if(config->prov_level >= 2) {//write to file
        if(!config->new_graph_path && !config->legacy_graph_path){
            printf("prov_helper_init() failed, provenance file path is not set.\n");
            return NULL;
        }
    }
    
    FUNCTION_FREQUENCY = stat_create(config->num_of_apis);

    if (fields->mpi_rank_int == 0) {
    /* Create stat file */
        if (config->enable_stat_file) 
            new_helper->stat_file_handle = fopen(config->stat_file_path, "a");
    }

    if(config->prov_level == File_only || config->prov_level == File_and_print) {
    
        if (!strcmp(config->prov_line_format, "rdf") || !strcmp(config->prov_line_format, "RDF")) {

            /* Open or create provenance file */
            if (config->legacy_graph_path && config->enable_legacy_graph) {
                new_helper->legacy_prov_file_handle = fopen(config->legacy_graph_path, "w");
            }
            if (config->new_graph_path) {
                if (!config->enable_legacy_graph) {
                    char tmp[32]; 
                    sprintf(tmp, "%d", fields->mpi_rank_int);
                    strcat(config->new_graph_path, ".RANK-");
                    strcat(config->new_graph_path, tmp);
                    new_helper->new_prov_file_handle = fopen(config->new_graph_path, "w");
                    printf("Created a new provenance file\n");
                }
            else 
                printf("NEW_GRAPH_PATH conflicts with ENABLE_LEGACY_GRAPH=T\n");                
            }            
        }
    }

    return new_helper;
}

void provio_init(prov_config* config, prov_fields* fields) {
    //Default settings
    load_config(config);

    if (!fields)
        fields = (prov_fields*) calloc(1, sizeof(prov_fields));
    else {
        memset(fields, 0, sizeof(prov_fields));
    }

    // Get RANK ID 
    fields->mpi_rank_int = get_mpi_rank(fields);

    char tmp_rank[128];
    if (fields->mpi_rank) {
        strcpy(tmp_rank, "MPI_rank_");
        strcat(tmp_rank, fields->mpi_rank);
        memcpy(fields->mpi_rank, tmp_rank, sizeof(fields->mpi_rank));
    }

    // Get process ID
    PROC_ID = getpid();
    sprintf(fields->pid,"%d", PROC_ID); 

    // Get thread ID
    THREAD_ID = pthread_self();
    sprintf(fields->tid,"%d", THREAD_ID); 

    // Get process name

    get_process_name_by_pid(fields, PROC_ID);

    alloc_proc_uuid(fields);

    getlogin_r(fields->user_name, 32);


#ifdef LIBRDF_H
    /* Initialise Redland environment */
    world = librdf_new_world();
    librdf_world_open(world);
    serializer = librdf_new_serializer(world, "turtle", NULL, NULL);

    /* Set up base uri and prefix */
    if (config->prov_base_uri)
        base_uri = librdf_new_uri(world, (const unsigned char *)config->prov_base_uri);
    else
        base_uri = NULL;
    
    librdf_serializer_set_namespace(serializer, base_uri, config->prov_prefix);

    provio_uri = librdf_new_uri(world, (const unsigned char *)"http://www.w3.org/ns/provio#");
    librdf_serializer_set_namespace(serializer, provio_uri, "provio");

    node_prefix = librdf_new_uri(world, (const unsigned char *)"/");
    librdf_serializer_set_namespace(serializer, node_prefix, LEGACY_PREFIX);

    // In-memory store 
    storage_prov = librdf_new_storage(world, "memory", NULL, NULL);
    model_prov = librdf_new_model(world, storage_prov, NULL); 

    // Store with BerkeleyDB 
    if (config->enable_bdb) {
        if(!(storage_prov = librdf_new_storage(world, "hashes", "prov",
                                 "hash-type='bdb',dir='.'"))) {
           storage_prov = librdf_new_storage(world, "hashes", "prov",
                                     "new='yes',hash-type='bdb',dir='.'");
        }
    }

    model_prov = librdf_new_model(world, storage_prov, NULL);

    // Parser and load legacy graph into model. 
    // We don't parse legacy graph in this version since it will need advance 
    // MPI thread coordiation mechanism to fully support cross-rank graph insertion.

    librdf_parser *parser = librdf_new_parser(world, "turtle", NULL, NULL);
    char legacy_uri_str[256] = "file:";
    if (config->legacy_graph_path && config->enable_legacy_graph) {
        FILE *legacy_path_handler;
        legacy_path_handler = fopen(config->legacy_graph_path, "r");
        if(legacy_path_handler == NULL) {
            printf("Old provenance file not found\n");
        }
        else {
            strcat(legacy_uri_str, config->legacy_graph_path);
            printf("Legacy graph: %s\n", config->legacy_graph_path);
            librdf_uri* legacy_uri=librdf_new_uri(world, (const unsigned char*)legacy_uri_str);
          
            if(librdf_parser_parse_into_model(parser,legacy_uri,legacy_uri,model_prov)) {
                fprintf(stderr, "Failed to parse old provenance file into model, check path %s\n", 
                    config->legacy_graph_path);
            }
            librdf_free_uri(legacy_uri);
            fclose(legacy_path_handler);
        }
        librdf_free_parser(parser);
    }
#endif

}


int add_user_record_Redland(prov_config* config, prov_fields* fields) {
    // User     
    if (config->enable_user_prov) {
        if (USER_TRACKED == 0) {
            statement=librdf_new_statement_from_nodes(world, 
               librdf_new_node_from_uri_string(world, (const unsigned char *)fields->user_name),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:type"),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:Agent")
               );
            librdf_model_add_statement(model_prov, statement);

            statement=librdf_new_statement_from_nodes(world, 
               librdf_new_node_from_uri_string(world, (const unsigned char *)fields->user_name),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasMemberOf"),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"provio:User")
               );
            librdf_model_add_statement(model_prov, statement);

            USER_TRACKED = 1;
        }
    }    
    return 0;
}

int add_mpi_rank_record_Redland(prov_config* config, prov_fields* fields) {
    // MPI rank ID
    if (config->enable_thread_prov && fields->mpi_rank) {
        if (MPI_RANK_TRACKED == 0) {
            statement=librdf_new_statement_from_nodes(world, 
               librdf_new_node_from_uri_string(world, (const unsigned char *)fields->mpi_rank),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:type"),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:Agent")
               );
            librdf_model_add_statement(model_prov, statement);

            statement=librdf_new_statement_from_nodes(world, 
               librdf_new_node_from_uri_string(world, (const unsigned char *)fields->mpi_rank),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasMemberOf"),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"provio:Thread")
               );
            librdf_model_add_statement(model_prov, statement);

            if (config->enable_user_prov) {
               statement=librdf_new_statement_from_nodes(world, 
                  librdf_new_node_from_uri_string(world, (const unsigned char *)fields->mpi_rank),
                  librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:actedOnBehalfOf"),
                  librdf_new_node_from_uri_string(world, (const unsigned char*)fields->user_name)
                                             );
               librdf_model_add_statement(model_prov, statement);
            }
            MPI_RANK_TRACKED = 1;
        }
    }
    return 0;
}

int add_program_record_Redland(prov_config* config, prov_fields* fields) {
    // program name
    if (config->enable_program_prov) {
        if (PROC_NAME_TRACKED == 0) {
            statement=librdf_new_statement_from_nodes(world,
               librdf_new_node_from_uri_string(world, (const unsigned char *)fields->proc_name),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:type"),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:Agent")
               );
            librdf_model_add_statement(model_prov, statement);

            statement=librdf_new_statement_from_nodes(world, 
               librdf_new_node_from_uri_string(world, (const unsigned char *)fields->proc_name),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasMemberOf"),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"provio:Program")
               );
            librdf_model_add_statement(model_prov, statement);

            if (config->enable_thread_prov && fields->mpi_rank) {
               statement=librdf_new_statement_from_nodes(world,
                  librdf_new_node_from_uri_string(world, (const unsigned char *)fields->proc_name),
                  librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:actedOnBehalfOf"),
                  librdf_new_node_from_uri_string(world, (const unsigned char*)fields->mpi_rank)
                  );
               librdf_model_add_statement(model_prov, statement);
            }

            statement=librdf_new_statement_from_nodes(world, 
                librdf_new_node_from_uri_string(world, (const unsigned char *)fields->proc_name),
                librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:startedAtTime"),
                librdf_new_node_from_literal(world, (const unsigned char*)fields->proc_start_time, NULL, 0)
            );
            librdf_model_add_statement(model_prov, statement);

            PROC_NAME_TRACKED = 1;
        }
        if (fields->proc_end_time) {
            statement=librdf_new_statement_from_nodes(world, 
                librdf_new_node_from_uri_string(world, (const unsigned char *)fields->proc_name),
                librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:endedAtTime"),
                librdf_new_node_from_literal(world, (const unsigned char*)fields->proc_end_time, NULL, 0)
            );
            librdf_model_add_statement(model_prov, statement);
        }
    }
    return 0;
}

int add_io_api_record_Redland(prov_config* config, prov_fields* fields, char* duration_) {
    // I/O API
    if (config->enable_api_prov) {
        /* Allocate UUID to io_api */
        alloc_api_uuid(fields);
        statement=librdf_new_statement_from_nodes(world, 
            librdf_new_node_from_uri_string(world, (const unsigned char *)fields->io_api),
            librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:type"),
            librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:Activity")
            );
        librdf_model_add_statement(model_prov, statement);

        if (config->enable_program_prov) {
            statement=librdf_new_statement_from_nodes(world,
               librdf_new_node_from_uri_string(world, (const unsigned char *)fields->io_api),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasAssociatedWith"),
               librdf_new_node_from_uri_string(world, (const unsigned char*)fields->proc_name)
               );
            librdf_model_add_statement(model_prov, statement);    
        }      

        if (config->enable_duration_prov) {
            statement=librdf_new_statement_from_nodes(world, 
               librdf_new_node_from_uri_string(world, (const unsigned char *)fields->io_api),
               librdf_new_node_from_uri_string(world, (const unsigned char*)"provio:elapsed"),
               librdf_new_node_from_literal(world, (const unsigned char*)duration_, NULL, 0)
               );
            librdf_model_add_statement(model_prov, statement);
        }
    }
    return 0;
}

int add_data_obj_record_Redland(prov_config* config, prov_fields* fields) {
    // Data object
    if ((config->enable_file_prov && (!strcmp(fields->type, "provio:File"))) ||
        (config->enable_group_prov && (!strcmp(fields->type, "provio:Group"))) ||
        (config->enable_dataset_prov && (!strcmp(fields->type, "provio:Dataset"))) ||
        (config->enable_attr_prov && (!strcmp(fields->type, "provio:Attr"))) ||
        (config->enable_dtype_prov && (!strcmp(fields->type, "provio:Datatype")))) {

    statement=librdf_new_statement_from_nodes(world, 
        librdf_new_node_from_uri_string(world, (const unsigned char *)fields->data_object),
        librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:type"),
        librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:Entity")
        );
    librdf_model_add_statement(model_prov, statement);

    statement=librdf_new_statement_from_nodes(world, 
        librdf_new_node_from_uri_string(world, (const unsigned char*)fields->data_object),
        librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasMemberOf"),
        librdf_new_node_from_uri_string(world, (const unsigned char *)fields->type)
        );
    librdf_model_add_statement(model_prov, statement);

    if (config->enable_api_prov) {
        statement=librdf_new_statement_from_nodes(world, 
            librdf_new_node_from_uri_string(world, (const unsigned char *)fields->data_object),
            librdf_new_node_from_uri_string(world, (const unsigned char *)fields->relation),
            librdf_new_node_from_uri_string(world, (const unsigned char *)fields->io_api)
            );
        librdf_model_add_statement(model_prov, statement);
    }

      if (config->enable_program_prov) {
        statement=librdf_new_statement_from_nodes(world, 
            librdf_new_node_from_uri_string(world, (const unsigned char*)fields->data_object),
            librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasAttributedTo"),
            librdf_new_node_from_uri_string(world, (const unsigned char *)fields->proc_name)
            );
        librdf_model_add_statement(model_prov, statement);  
        }
    }
    return 0;
}

int add_program_record(prov_config* config, prov_fields* fields) {
    int ret = add_program_record_Redland(config, fields);
    return ret;
}

/* Add Redland provenance statement */
int add_prov_record_Redland(prov_config* config, prov_fields* fields, char* duration_) {
    int ret;
    ret = add_user_record_Redland(config, fields);
    ret = add_mpi_rank_record_Redland(config, fields);
    ret = add_io_api_record_Redland(config, fields, duration_);
    ret = add_data_obj_record_Redland(config, fields);
    return ret;
}


int add_prov_record(prov_config* config, provio_helper_t* helper_in, prov_fields* fields){
    unsigned long start = get_time_usec();
    const char* base = DEFAULT_FUNCTION_PREFIX; //to be replace by H5
    size_t base_len;
    size_t io_api_len;
    char pline[1024];
    char duration_[256];

    assert(helper_in);
    assert(fields);


    /* Trimming long VOL function names */
    base_len = strlen(base);
    io_api_len = strlen(fields->io_api);

    if(io_api_len > base_len) {//strlen(H5VL_provenance_) == 16.
        size_t i = 0;
        for(; i < base_len; i++)
            if(base[i] != fields->io_api[i])
                break;
    }

    if(!strcmp(config->prov_line_format, "rdf") || !strcmp(config->prov_line_format, "RDF")) {
        sprintf(duration_, "%lu", fields->duration);
    }
    else
        sprintf(pline, "%s %luus\n", fields->io_api, fields->duration);//assume less than 64 functions
    
    switch(config->prov_level){
        case File_only:
            if(!strcmp(config->prov_line_format, "rdf") || !strcmp(config->prov_line_format, "RDF")) {
#ifdef LIBRDF_H
                add_prov_record_Redland(config, fields, duration_);
#endif      
            }
            else {
                if (config->enable_legacy_graph)
                    fputs(pline, helper_in->legacy_prov_file_handle);
                fputs(pline, helper_in->new_prov_file_handle);
            }
            break;

        case File_and_print:
            if(!strcmp(config->prov_line_format, "rdf") || !strcmp(config->prov_line_format, "RDF")) {
#ifdef LIBRDF_H
                add_prov_record_Redland(config, fields, duration_);
#endif                 
            }
            else {
                if (config->enable_legacy_graph)
                    fputs(pline, helper_in->legacy_prov_file_handle);
                fputs(pline, helper_in->new_prov_file_handle);
            }
            printf("%s", pline);
            break;

        case Print_only:
            printf("%s", pline);
            break;

        case Level3:
        case Level4:
        case Disabled:
        case Default:
        default:
            break;
    }

//    unsigned tmp = PROV_WRITE_TOTAL_TIME;

    prov_stat.PROV_WRITE_TOTAL_TIME += (get_time_usec() - start);

    return 0;
}


void provio_helper_teardown(prov_config* config, provio_helper_t* helper, prov_fields* fields){
    get_time_str(fields->proc_end_time);
    add_program_record(config, fields);

    if (helper->legacy_prov_file_handle || helper->new_prov_file_handle) {
        /* Redland: serialize to file */
        unsigned long start = get_time_usec();
        if (config->enable_legacy_graph) {
            if(!strcmp(config->prov_line_format, "rdf") || !strcmp(config->prov_line_format, "RDF")) {
#ifdef LIBRDF_H
        librdf_serializer_serialize_model_to_file_handle(serializer, 
            helper->legacy_prov_file_handle, NULL, model_prov);
#endif      
            }
        }
        else {
            if(!strcmp(config->prov_line_format, "rdf") || !strcmp(config->prov_line_format, "RDF")) {
    #ifdef LIBRDF_H
            librdf_serializer_serialize_model_to_file_handle(serializer, 
                helper->new_prov_file_handle, NULL, model_prov);
    #endif      
            }
        }
        if (helper->legacy_prov_file_handle)
            fclose(helper->legacy_prov_file_handle);
        if (helper->new_prov_file_handle)
            fclose(helper->new_prov_file_handle);
        prov_stat.PROV_SERIALIZE_TIME += (get_time_usec() - start);
    }

    char pline[2048];
    if (fields->mpi_rank_int == 0) {
        if (helper->stat_file_handle != NULL) {
            stat_print_(0, &prov_stat, FUNCTION_FREQUENCY, helper->stat_file_handle);
        }
        else {
            printf("%s", pline);
        }
        
        stat_destroy(FUNCTION_FREQUENCY);
    }
        
    if (fields->mpi_rank_int == 0 && helper->stat_file_handle != NULL) {
         fflush(helper->stat_file_handle);
         fclose(helper->stat_file_handle);
    }
    /* Free provenacne helper */
    free(helper);
}

void provio_term(prov_config* config, prov_fields* fields) {
#ifdef LIBRDF_H
    /* Free Redland pointers */    
    librdf_free_statement(statement);
    librdf_free_serializer(serializer);
    librdf_free_memory(base_uri);
    librdf_free_memory(provio_uri);    
    librdf_free_model(model_prov);
    librdf_free_storage(storage_prov);
    librdf_free_world(world);
#endif
    /* Free provenance fields */
    // free_fields(fields);
    /* Free provenance config */
    free_config(config);
}