#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/stat.h>
#include <serd.h>
#include <redland.h>

#include "librdf.h"
#include "gotcha/gotcha.h"


/* Local level one wrappers */

static int mknod_(const char* path, mode_t mode, dev_t dev);

static int mkdir_(const char* path, mode_t mode);

static int creat_(const char *path, mode_t mode);

static int open_(const char *path, mode_t mode);

static ssize_t read_(int fd, void *buf, size_t count);

static ssize_t write_(int fd, const void *buf, size_t count);

static int close_(int fd);

static int rmdir_(const char *path);

static int rename_(const char *path);

static int unlink_(const char *path);

static int fsync_(int fd);

static gotcha_wrappee_handle_t wrappee_mknod_handle;
static gotcha_wrappee_handle_t wrappee_mkdir_handle;

struct gotcha_binding_t funcs[] = {
   { "mknod", mknod_, &wrappee_mknod_handle },
   { "mkdir", mkdir_, &wrappee_mkdir_handle }
};

struct prov_fields {
    char entity[64];
    char activity[64];
    char agent[64];
    char type[64];
    char relation[64];
    int entity_id;
    int activity_id;
    int agent_id;
    int relation_id;
    unsigned long duration;
} prov_fields;

static void prov_fill(struct prov_fields* fields, const char* name, 
    const char* relation, const char* type, const char* agent) {
    strcpy(fields->entity, name);
    strcpy(fields->relation, relation);
    strcpy(fields->type, type);        
    strcpy(fields->agent, agent);
}

int prov_write(struct prov_fields* fields){
   unsigned long start = get_time_usec();
   size_t base_len;
   size_t activity_len;
   char time[64];
   char pline[512];
   char duration_[256];

   get_time_str(time);

   sprintf(duration_, "%lu", fields->duration);

   /* Redland */
   statement=librdf_new_statement_from_nodes(world, librdf_new_node_from_uri_string(world, fields->entity),
                                 librdf_new_node_from_uri_string(world, (const unsigned char*)"provio:ofType"),
                                 librdf_new_node_from_literal(world, fields->type, NULL, 0)
                                 );
   librdf_model_add_statement(model, statement);
   statement=librdf_new_statement_from_nodes(world, librdf_new_node_from_uri_string(world, fields->entity),
                                 librdf_new_node_from_uri_string(world, fields->relation),
                                 librdf_new_node_from_literal(world, fields->activity, NULL, 0)
                                 );
   librdf_model_add_statement(model, statement);
   statement=librdf_new_statement_from_nodes(world, librdf_new_node_from_uri_string(world, fields->entity),
                                 librdf_new_node_from_uri_string(world, (const unsigned char*)"prov:wasAttributedTo"),
                                 librdf_new_node_from_literal(world, fields->agent, NULL, 0)
                                 );
   librdf_model_add_statement(model, statement);                     
   return 0;
}


/* Gotcha wrappers */

int mknod_wrapper(const char* path, mode_t mode, dev_t dev) {
   typeof(&mknod_) wrappee_mknod = gotcha_get_wrappee(wrappee_mknod_handle); // get my wrappee from Gotcha
   return wrappee_mknod(path, mode, dev);
}

int mkdir_wrapper(const char* path, mode_t mode) {
   unsigned long start = get_time_usec();
   unsigned long m1, m2;
   int ret;
   struct prov_fields fields;

   const char* name = "./newdir";
   const char* relation = "provio:wasWrittenBy";
   const char* type = "provio:Directory";
   const char* agent = program_name;

   typeof(&mkdir_) wrappee_mkdir = gotcha_get_wrappee(wrappee_mkdir_handle); // get my wrappee from Gotcha
   m1 = get_time_usec();
   ret = wrappee_mkdir(path, mode);
   m2 = get_time_usec();
   if(!ret) {
      prov_fill(&fields, name, relation, type, agent);
      fields.duration = get_time_usec() - start;
      strcpy(fields.activity, "mkdir");
      prov_write(&fields);  
   } 
   return ret;
}

int init_mytool(){
   gotcha_wrap(funcs, sizeof(funcs)/sizeof(struct gotcha_binding_t), "POSIX wrapper");
}

int main()
{
   size_t ret;
   /* Get program name */
   pid_t self = getpid();
   program_name = get_process_name_by_pid(self);
   
   world=librdf_new_world();
   librdf_world_open(world);
   serializer=librdf_new_serializer(world, "turtle", NULL, NULL);
   storage=librdf_new_storage(world, "file", "prov.log", NULL);
   model=librdf_new_model(world, storage, NULL); 

   ret = init_mytool();

   ret = mkdir_wrapper("./newdir", 0777);
   // ret = mkdir_wrapper("newdir", 0777);

   librdf_uri* base_uri=librdf_new_uri(world, (const unsigned char*)"http://exampe.org/base.rdf");
   FILE *turtle = fopen("prov.turtle", "a");
   librdf_serializer_serialize_model_to_file_handle(serializer, turtle, base_uri, model);

   /* Free Redland pointers */
   librdf_free_statement(statement);
   librdf_free_serializer(serializer);
   librdf_free_model(model);
   librdf_free_storage(storage);
   librdf_free_world(world);

   return ret;
}
