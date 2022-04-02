#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/stat.h>
#include <serd.h>
#include <redland.h>

#include "librdf.h"
#include "gotcha/gotcha.h"


/* Global variables */
char* program_name;

/* Redland global variables */
librdf_world* world;
librdf_storage* storage;
librdf_model* model;
librdf_statement *statement;
librdf_serializer* serializer;


/* Helper functions */

static
unsigned long get_time_usec(void) {
    struct timeval tp;

    gettimeofday(&tp, NULL);
    return (unsigned long)((1000000 * tp.tv_sec) + tp.tv_usec);
}

void get_time_str(char *str_out){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    *str_out = '\0';
    sprintf(str_out, "%d/%d/%d %d:%d:%d", timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

static const char* get_process_name_by_pid(const int pid)
{
    char* name = (char*)calloc(1024,sizeof(char));
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
    return name;
}


/* Local level one wrappers */

static int mknod_(const char* path, mode_t mode, dev_t dev) {
   int ret;
   // ret = syscall(SYS_mknod, path, mode, dev);
   mknod(const char *pathname, mode_t mode, dev_t dev);
   return ret;
}

static int mkdir_(const char* path, mode_t mode) {
   int ret;
   // ret = syscall(SYS_mkdir, mode);
   ret = mkdir("./newdir", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   return ret;
}

static int creat_(const char *path, mode_t mode) {
   int ret;
   ret = syscall(SYS_creat, path, mode);
   return ret;
}

static int open_(const char *path, mode_t mode) {
   int ret;
   ret = syscall(SYS_open, path, mode);
   return ret;
}

static ssize_t read_(int fd, void *buf, size_t count) {
   ssize_t ret;
   ret = syscall(SYS_read, fd, buf, count);
   return ret;
}

static ssize_t write_(int fd, const void *buf, size_t count) {
   ssize_t ret;
   ret = syscall(SYS_mknod, fd, buf, count);
   return ret;
}

static int close_(int fd) {
   int ret;
   ret = syscall(SYS_close, fd);
   return ret;
}

static int rmdir_(const char *path) {
   int ret;
   ret = syscall(SYS_rmdir, path);
   return ret;
}

static int rename_(const char *path) {
   int ret;
   ret = syscall(SYS_rename, path);
   return ret;
}

static int unlink_(const char *path) {
   int ret;
   ret = syscall(SYS_unlink, path);
   return ret;
}

static int fsync_(int fd) {
   int ret;
   ret = syscall(SYS_unlink, fd);
   return ret;
}

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
