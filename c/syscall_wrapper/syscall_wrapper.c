#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

#include "provio.h"


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


   const char* agent = program_name;

   typeof(&mkdir_) wrappee_mkdir = gotcha_get_wrappee(wrappee_mkdir_handle); // get my wrappee from Gotcha
   m1 = get_time_usec();
   ret = wrappee_mkdir(path, mode);
   m2 = get_time_usec();

    /* PROV-IO instrument start */
    const char* io_api = "mkdir";
    const char* relation = "prov:wasGeneratedBy";
    const char* type = "provio:Directory"; 
    prov_fill_data_object(&fields, path, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */

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
