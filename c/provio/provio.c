// #include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <uuid/uuid.h>
#include <string.h>
#include <unistd.h>

#include "provio.h"
#include "stat.h"
// #include ""


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


/* Global variables */
char* program_name;

/* Configuration global variables */
prov_params params;

/* Other provenance global variables */
// Name of the program
char* program_name;
char* program_uuid;
int program_name_tracked = 0;
// MPI rank ID
#ifdef H5_HAVE_PARALLEL
char mpi_rank[16];
int mpi_rank_int;
char num_ranks[16];
int num_ranks_int;
int mpi_rank_tracked = 0;
#endif
// User info
int user_tracked = 0;
// Host info

static void get_mpi_rank() {
    /* Get RANK ID */
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank_int); 
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks_int);
    sprintf(mpi_rank,"%d", mpi_rank_int); 
    sprintf(num_ranks,"%d", num_ranks_int);
}


/* Provenance helper methods */
static char* alloc_uuid(char* name) {
    uuid_t uuid;
    uuid_generate_time_safe(uuid);
    char* uuid_ = malloc(37);
    uuid_unparse_lower(uuid, uuid_);
    strcat(name, "--");
    strcat(name, uuid_);
    return uuid_;
}

static void prov_fill_data_object(struct prov_fields* fields, const char* name, 
    const char* relation, const char* type) {
    strcpy(fields->data_object, name);
    // if (*program_uuid) {
    //     strcat(fields->data_object, "--");
    //     strcat(fields->data_object, program_uuid);
    // }
    strcpy(fields->relation, relation);
    strcpy(fields->type, type);        
}


/* Initialize ProvenanceHelper */
provio_helper_t * provio_helper_init(char* file_path, Prov_level prov_level, char* prov_line_format)
{
    provio_helper_t* new_helper = (provio_helper_t *)calloc(1, sizeof(provio_helper_t));

    if(prov_level >= 2) {//write to file
        if(!file_path){
            printf("prov_helper_init() failed, provenance file path is not set.\n");
            return NULL;
        }
    }

    new_helper->prov_file_path = strdup(file_path);
    new_helper->prov_line_format = strdup(prov_line_format);
    new_helper->prov_level = prov_level;
    new_helper->pid = getpid();
    new_helper->tid = pthread_self();


    // new_helper->opened_files = NULL;
    // new_helper->opened_files_cnt = 0;

    getlogin_r(new_helper->user_name, 32);
    
    /* Don't use it */
    //if(new_helper->prov_level == File_only || new_helper->prov_level == File_and_print)
        // new_helper->prov_file_handle = fopen(new_helper->prov_file_path, "a");
        // new_helper->prov_file_handle = fopen(new_helper->prov_file_path, "w");

    // c1 = get_time_usec();
    if (mpi_rank_int == 0) {
        /* Create stat file */
        // if (enable_stat_file)
   new_helper->stat_file_handle = fopen(params.stat_file_path, "a");
    }
    // c2 = get_time_usec();

    if (!strcmp(new_helper->prov_line_format, "rdf")) {
        
   /* Open or create provenance file */
   strcat(params.new_graph_path, mpi_rank);
        // Append 
        librdf_prov_file_handler = fopen(params.new_graph_path, "a");
        // Overwrite
        // librdf_prov_file_handler = fopen(params.prov_file_path, "w");

        // In-memory store 
        storage_prov = librdf_new_storage(world, "memory", NULL, NULL);
        model_prov = librdf_new_model(world, storage_prov, NULL); 

        // Store with BerkeleyDB 
        //if(!(storage_prov = librdf_new_storage(world, "hashes", "prov",
        //                          "hash-type='bdb',dir='.'"))) {
        //    storage_prov = librdf_new_storage(world, "hashes", "prov",
        //                              "new='yes',hash-type='bdb',dir='.'");
        //}
        //model_prov = librdf_new_model(world, storage_prov, NULL);
   
   // Parser and load legacy graph into model. 
   // We don't parse legacy graph in this version since it will need advance 
        // MPI thread coordiation mechanism to fully support cross-rank graph insertion.
   /*
   
   librdf_parser *parser = librdf_new_parser(world, "turtle", NULL, NULL);
        char legacy_uri_str[256] = "file:";
        char legacy_path[256] = "prov_old.turtle";
   FILE *legacy_path_handler;
        legacy_path_handler = fopen(legacy_path, "r");
        if(legacy_path_handler == NULL) {
            printf("Old provenance file not found\n");
        }
        else {
            strcat(legacy_uri_str, legacy_path);
            librdf_uri* legacy_uri=librdf_new_uri(world, (const unsigned char*)legacy_uri_str);
          
            if(librdf_parser_parse_into_model(parser,legacy_uri,legacy_uri,model_prov)) {
                fprintf(stderr, "Failed to parse old provenance file into model, check path %s\n", legacy_path);
            }
            librdf_free_uri(legacy_uri);
        }
   fclose(legacy_path_handler);
        librdf_free_parser(parser);
   */
    }

    // c3 = get_time_usec();

    // _dic_init();
    _dic_init_int();

    return new_helper;
}


int prov_write(provio_helper_t* helper_in, struct prov_fields* fields){
    unsigned long start = get_time_usec();
    const char* base = "H5VL_provenance_"; //to be replace by H5
    size_t base_len;
    size_t io_api_len;
    char time[64];
    char pline[1024];
    char duration_[256];

    assert(helper_in);

    get_time_str(time);


    // printf("MPI RANK: %d\n", mpi_rank_int);

    /* Trimming long VOL function names */
    base_len = strlen(base);
    io_api_len = strlen(fields->io_api);

    if(io_api_len > base_len) {//strlen(H5VL_provenance_) == 16.
        size_t i = 0;

        for(; i < base_len; i++)
            if(base[i] != fields->io_api[i])
                break;
    }

    if(!strcmp(PROV_HELPER->prov_line_format, "rdf")) {
        sprintf(duration_, "%lu", fields->duration);
    }
    else
        sprintf(pline, "%s %luus\n", fields->io_api, fields->duration);//assume less than 64 functions

    /* Allocate UUID to io_api */
    // alloc_uuid(fields->data_object);
    alloc_uuid(fields->io_api);
    
    // Copy agent info to pro_fields structure
    strcpy(fields->program_name, program_name);
#ifdef H5_HAVE_PARALLEL
    strcpy(fields->mpi_rank, "MPI_rank_");
    strcat(fields->mpi_rank, mpi_rank);
#endif

    //printf("Func name:[%s], hash index = [%u], overhead = [%lu]\n",  fields->io_api, genHash(fields->io_api), duration);
    switch(helper_in->prov_level){
        case File_only:
            if(!strcmp(PROV_HELPER->prov_line_format, "rdf")) {
                /* Provenance statement */
                // User     

                if (params.enable_user_prov) {
                    if (user_tracked == 0) {
                        statement=librdf_new_statement_from_nodes(world, 
                           librdf_new_node_from_uri_string(world, (const unsigned char *)helper_in->user_name),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:type"),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:Agent")
                           );
                        librdf_model_add_statement(model_prov, statement);

                        statement=librdf_new_statement_from_nodes(world, 
                           librdf_new_node_from_uri_string(world, (const unsigned char *)helper_in->user_name),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasMemberOf"),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)"provio:User")
                           );
                        librdf_model_add_statement(model_prov, statement);

                        user_tracked = 1;
                    }
                }

                // MPI rank ID
                if (params.enable_thread_prov) {
                    if (mpi_rank_tracked == 0) {
#ifdef H5_HAVE_PARALLEL
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

                        if (params.enable_user_prov) {
                           statement=librdf_new_statement_from_nodes(world, 
                              librdf_new_node_from_uri_string(world, (const unsigned char *)fields->mpi_rank),
                              librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:actedOnBehalfOf"),
                              librdf_new_node_from_uri_string(world, (const unsigned char*)helper_in->user_name)
                                                         );
                           librdf_model_add_statement(model_prov, statement);
                        }
#endif
                        mpi_rank_tracked = 1;
                    }
                }

                // program name
                if (params.enable_program_prov) {
                    if (program_name_tracked == 0) {
                        statement=librdf_new_statement_from_nodes(world,
                           librdf_new_node_from_uri_string(world, (const unsigned char *)fields->program_name),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:type"),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:Agent")
                           );
                        librdf_model_add_statement(model_prov, statement);

                        statement=librdf_new_statement_from_nodes(world, 
                           librdf_new_node_from_uri_string(world, (const unsigned char *)fields->program_name),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasMemberOf"),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)"provio:Program")
                           );
                        librdf_model_add_statement(model_prov, statement);

                        if (params.enable_thread_prov) {
#ifdef H5_HAVE_PARALLEL
                           statement=librdf_new_statement_from_nodes(world,
                              librdf_new_node_from_uri_string(world, (const unsigned char *)fields->program_name),
                              librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:actedOnBehalfOf"),
                              librdf_new_node_from_uri_string(world, (const unsigned char*)fields->mpi_rank)
                              );
                           librdf_model_add_statement(model_prov, statement);
#endif
                        }
                        program_name_tracked = 1;
                    }
                }

                // io_api
                if (params.enable_api_prov) {
                    statement=librdf_new_statement_from_nodes(world, 
                        librdf_new_node_from_uri_string(world, (const unsigned char *)fields->io_api),
                        librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:type"),
                        librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:Activity")
                        );
                    librdf_model_add_statement(model_prov, statement);

                    if (params.enable_program_prov) {
                        statement=librdf_new_statement_from_nodes(world,
                           librdf_new_node_from_uri_string(world, (const unsigned char *)fields->io_api),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasAssociatedWith"),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)fields->program_name)
                           );
                        librdf_model_add_statement(model_prov, statement);    
                    }      

                    statement=librdf_new_statement_from_nodes(world, 
                        librdf_new_node_from_uri_string(world, (const unsigned char *)fields->io_api),
                        librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:startedAtTime"),
                        librdf_new_node_from_literal(world, (const unsigned char*)time, NULL, 0)
                        );
                    librdf_model_add_statement(model_prov, statement);

                    if (params.enable_duration_prov) {
                        statement=librdf_new_statement_from_nodes(world, 
                           librdf_new_node_from_uri_string(world, (const unsigned char *)fields->io_api),
                           librdf_new_node_from_uri_string(world, (const unsigned char*)"provio:elapsed"),
                           librdf_new_node_from_literal(world, (const unsigned char*)duration_, NULL, 0)
                           );
                        librdf_model_add_statement(model_prov, statement);
                    }
                }

               // data object
               if ((params.enable_file_prov && (!strcmp(fields->type, "provio:File"))) ||
                    (params.enable_group_prov && (!strcmp(fields->type, "provio:Group"))) ||
                    (params.enable_dataset_prov && (!strcmp(fields->type, "provio:Dataset"))) ||
                    (params.enable_attr_prov && (!strcmp(fields->type, "provio:Attr"))) ||
                    (params.enable_dtype_prov && (!strcmp(fields->type, "provio:Datatype")))) {

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

                  if (params.enable_api_prov) {
                     statement=librdf_new_statement_from_nodes(world, 
                        librdf_new_node_from_uri_string(world, (const unsigned char *)fields->data_object),
                        librdf_new_node_from_uri_string(world, (const unsigned char *)fields->relation),
                        librdf_new_node_from_uri_string(world, (const unsigned char *)fields->io_api)
                        );
                     librdf_model_add_statement(model_prov, statement);
                  }

                  if (params.enable_program_prov) {
                     statement=librdf_new_statement_from_nodes(world, 
                        librdf_new_node_from_uri_string(world, (const unsigned char*)fields->data_object),
                        librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasAttributedTo"),
                        librdf_new_node_from_uri_string(world, (const unsigned char *)fields->program_name)
                        );
                     librdf_model_add_statement(model_prov, statement);  
                  }
               }     
            }

            else
                fputs(pline, helper_in->prov_file_handle);               
            break;

        case File_and_print:
            if(!strcmp(PROV_HELPER->prov_line_format, "rdf")) {
                ;
            }

            else
                fputs(pline, helper_in->prov_file_handle);                
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

    if(helper_in->prov_level == (File_only | File_and_print)){
        fputs(pline, helper_in->prov_file_handle);
    }
//    unsigned tmp = PROV_WRITE_TOTAL_TIME;

    PROV_WRITE_TOTAL_TIME += (get_time_usec() - start);

    return 0;
}


void prov_helper_teardown(prov_helper_t* helper){
    
    if (librdf_prov_file_handler != NULL) {
       /* Redland: serialize to file */
       unsigned long start = get_time_usec();
       librdf_serializer_serialize_model_to_file_handle(serializer, librdf_prov_file_handler, NULL, model_prov);
       fclose(librdf_prov_file_handler);
       PROV_SERIALIZE_TIME += (get_time_usec() - start);
    }

    if(helper){// not null
   char pline[2048];
   if (mpi_rank_int == 0) {
      sprintf(pline,
         "+ MPI RANK %d\n"
         "TOTAL_PROV_OVERHEAD %lu us\n"
         "TOTAL_NATIVE_H5_TIME %lu us\n"
         "PROV_WRITE_TOTAL_TIME %lu us\n"
         "FILE_LINKED_LIST_TOTAL_TIME %lu us\n"
         "DS_LINKED_LIST_TOTAL_TIME %lu us\n"
         "GRP_LINKED_LIST_TOTAL_TIME %lu us\n"
         "DT_LINKED_LIST_TOTAL_TIME %lu us\n"
         "ATTR_LINKED_LIST_TOTAL_TIME %lu us\n"
         "PROV_SERIALIZATION_TIME %lu us\n",
         mpi_rank_int,
         TOTAL_PROV_OVERHEAD,
         TOTAL_NATIVE_H5_TIME,
         PROV_WRITE_TOTAL_TIME,
         FILE_LL_TOTAL_TIME,
         DS_LL_TOTAL_TIME,
         GRP_LL_TOTAL_TIME,
         DT_LL_TOTAL_TIME,
         ATTR_LL_TOTAL_TIME,
         PROV_SERIALIZE_TIME);
                if (helper->stat_file_handle != NULL) {
                        fputs(pline, helper->stat_file_handle);
                }
                else {
                        printf("%s", pline);
      }
      // Iteratively print out accumulated duration hash table, freeing values as we go.
      hti it = ht_iterator(counts);
      while (ht_next(&it)) {
                    sprintf(pline,
                        "%s %d us\n", it.key, *(int*)it.value);
                    free(it.value);
          if (helper->stat_file_handle != NULL) {
                        fputs(pline, helper->stat_file_handle);
                    }
                      else {
                        printf("%s", pline);
          }
      }
                ht_destroy(counts);
   }
    }
        
        // if(helper->prov_level == File_only || helper->prov_level ==File_and_print){//no file
            // fflush(helper->prov_file_handle);
            // fclose(helper->prov_file_handle);
        // }

        // _dic_free();
   if (mpi_rank_int == 0 && helper->stat_file_handle != NULL) {
         fflush(helper->stat_file_handle);
         fclose(helper->stat_file_handle);
   }

        if(helper->prov_file_path)
            free(helper->prov_file_path);
        if(helper->prov_line_format)
            free(helper->prov_line_format);
        free(helper);
}