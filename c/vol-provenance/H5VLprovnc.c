/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Purpose:     This is a "PROVENANCE" VOL connector, which forwards each
 *              VOL callback to an underlying connector.
 *
 *              It is designed as an example VOL connector for developers to
 *              use when creating new connectors, especially connectors that
 *              are outside of the HDF5 library.  As such, it should _NOT_
 *              include _any_ private HDF5 header files.  This connector should
 *              therefore only make public HDF5 API calls and use standard C /
 *              POSIX calls.
 */


/* Header files needed */
/* (Public HDF5 and standard C / POSIX only) */

#include <assert.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "hdf5.h"
#include "H5VLprovnc.h"

/**********/
/* Macros */
/**********/

/* Whether to display log messge when callback is invoked */
/* (Uncomment to enable) */
/* #define ENABLE_PROVNC_LOGGING */

/* Hack for missing va_copy() in old Visual Studio editions
 * (from H5win2_defs.h - used on VS2012 and earlier)
 */
#if defined(_WIN32) && defined(_MSC_VER) && (_MSC_VER < 1800)
#define va_copy(D,S)      ((D) = (S))
#endif

#define STAT_FUNC_MOD 733//a reasonably big size to avoid expensive collision handling, make sure it works with 62 function names.

//H5PL_type_t H5PLget_plugin_type(void) {return H5PL_TYPE_FILTER;}
//const void *H5PLget_plugin_info(void) {return &H5VL_provenance_cls;}

/************/
/* Typedefs */
/************/
typedef struct H5VL_prov_dataset_info_t dataset_prov_info_t;
typedef struct H5VL_prov_group_info_t group_prov_info_t;
typedef struct H5VL_prov_datatype_info_t datatype_prov_info_t;
typedef struct H5VL_prov_attribute_info_t attribute_prov_info_t;
typedef struct H5VL_prov_file_info_t file_prov_info_t;

typedef struct ProvenanceHelper {
    /* Provenance properties */
    char* prov_file_path;
    FILE* prov_file_handle;
    Prov_level prov_level;
    char* prov_line_format;
    char user_name[32];
    int pid;
    pthread_t tid;
    char proc_name[64];
    int ptr_cnt;
    int opened_files_cnt;
    file_prov_info_t* opened_files;//linkedlist,
} prov_helper_t;

typedef struct H5VL_provenance_t {
    hid_t  under_vol_id;        /* ID for underlying VOL connector */
    void  *under_object;        /* Info object for underlying VOL connector */
    H5I_type_t my_type;         /* obj type, dataset, datatype, etc. */
    prov_helper_t *prov_helper; /* pointer shared among all layers, one per process. */
    void *generic_prov_info;    /* Pointer to a class-specific prov info struct. */
                                /* Should be cast to layer-specific type before use, */
                                /* such as file_prov_info, dataset_prov_info. */
} H5VL_provenance_t;

/* The PROVENANCE VOL wrapper context */
typedef struct H5VL_provenance_wrap_ctx_t {
    prov_helper_t *prov_helper; /* shared pointer */
    hid_t under_vol_id;         /* VOL ID for under VOL */
    void *under_wrap_ctx;       /* Object wrapping context for under VOL */
    file_prov_info_t *file_info;
    unsigned long file_no;
    hid_t dtype_id;             /* only used by datatype */
} H5VL_provenance_wrap_ctx_t;

//======================================= statistics =======================================
//typedef struct H5VL_prov_t {
//    void   *under_object;
//    char* func_name;
//    int func_cnt;//stats
//} H5VL_prov_t;

struct H5VL_prov_file_info_t {//assigned when a file is closed, serves to store stats (copied from shared_file_info)
    prov_helper_t* prov_helper;  //pointer shared among all layers, one per process.
    char* file_name;
    unsigned long file_no;
#ifdef H5_HAVE_PARALLEL
    // Only present for parallel HDF5 builds
    MPI_Comm mpi_comm;           // Copy of MPI communicator for file
    MPI_Info mpi_info;           // Copy of MPI info for file
    hbool_t mpi_comm_info_valid; // Indicate that MPI Comm & Info are valid
#endif /* H5_HAVE_PARALLEL */
    int ref_cnt;

    /* Currently open objects */
    int opened_datasets_cnt;
    dataset_prov_info_t *opened_datasets;
    int opened_grps_cnt;
    group_prov_info_t *opened_grps;
    int opened_dtypes_cnt;
    datatype_prov_info_t *opened_dtypes;
    int opened_attrs_cnt;
    attribute_prov_info_t *opened_attrs;

    /* Statistics */
    int ds_created;
    int ds_accessed;
    int grp_created;
    int grp_accessed;
    int dtypes_created;
    int dtypes_accessed;

    file_prov_info_t *next;
};

// Common provenance information, for all objects
typedef struct H5VL_prov_object_info_t {
    prov_helper_t *prov_helper;         //pointer shared among all layers, one per process.
    file_prov_info_t *file_info;        // Pointer to file info for object's file
    H5O_token_t token;                  // Unique ID within file for object
    char *name;                         // Name of object within file
                                        // (possibly NULL and / or non-unique)
    int ref_cnt;                        // # of references to this prov info
} object_prov_info_t;

struct H5VL_prov_dataset_info_t {
    object_prov_info_t obj_info;        // Generic prov. info
                                        // Must be first field in struct, for
                                        // generic upcasts to work

    H5T_class_t dt_class;               //data type class
    H5S_class_t ds_class;               //data space class
    H5D_layout_t layout;
    unsigned int dimension_cnt;
    hsize_t dimensions[H5S_MAX_RANK];
    size_t dset_type_size;
    hsize_t dset_space_size;            //unsigned long long

    hsize_t total_bytes_read;
    hsize_t total_bytes_written;
    hsize_t total_read_time;
    hsize_t total_write_time;
    int dataset_read_cnt;
    int dataset_write_cnt;
#ifdef H5_HAVE_PARALLEL
    int ind_dataset_read_cnt;
    int ind_dataset_write_cnt;
    int coll_dataset_read_cnt;
    int coll_dataset_write_cnt;
    int broken_coll_dataset_read_cnt;
    int broken_coll_dataset_write_cnt;
#endif /* H5_HAVE_PARALLEL */
    int access_cnt;

    dataset_prov_info_t *next;
};

struct H5VL_prov_group_info_t {
    object_prov_info_t obj_info;        // Generic prov. info
                                        // Must be first field in struct, for
                                        // generic upcasts to work

    int func_cnt;//stats
//    int group_get_cnt;
//    int group_specific_cnt;

    group_prov_info_t *next;
};

typedef struct H5VL_prov_link_info_t {
    int link_get_cnt;
    int link_specific_cnt;
} link_prov_info_t;

struct H5VL_prov_datatype_info_t {
    object_prov_info_t obj_info;        // Generic prov. info
                                        // Must be first field in struct, for
                                        // generic upcasts to work

    hid_t dtype_id;
    int datatype_commit_cnt;
    int datatype_get_cnt;

    datatype_prov_info_t *next;
};

struct H5VL_prov_attribute_info_t {
    object_prov_info_t obj_info;        // Generic prov. info
                                        // Must be first field in struct, for
                                        // generic upcasts to work

    int func_cnt;//stats

    attribute_prov_info_t *next;
};

unsigned long TOTAL_PROV_OVERHEAD;
unsigned long TOTAL_NATIVE_H5_TIME;
unsigned long PROV_WRITE_TOTAL_TIME;
unsigned long FILE_LL_TOTAL_TIME;       //record file linked list overhead
unsigned long DS_LL_TOTAL_TIME;         //dataset
unsigned long GRP_LL_TOTAL_TIME;        //group
unsigned long DT_LL_TOTAL_TIME;         //datatype
unsigned long ATTR_LL_TOTAL_TIME;       //attribute
static prov_helper_t* PROV_HELPER = NULL;


/* PROV-IO instrument start */
prov_fields fields;
provio_helper_t* provio_helper;
prov_config config;

static ssize_t object_get_name(void *under_obj, hid_t under_vol_id, 
    const H5VL_loc_params_t *loc_params, hid_t dxpl_id, size_t buf_size, void *buf) {
    struct H5VL_object_get_args_t vol_cb_args; /* Set up VOL callback arguments */
    int obj_name_len = 0;  /* Length of obj name */
    vol_cb_args.op_type                = H5VL_OBJECT_GET_NAME;
    vol_cb_args.args.get_name.buf_size = buf_size;
    vol_cb_args.args.get_name.buf      = buf;
    vol_cb_args.args.get_name.name_len = &obj_name_len;
    /* Retrieve object's name */
    if (H5VLobject_get(under_obj, loc_params, under_vol_id, &vol_cb_args, 
        H5P_DATASET_XFER_DEFAULT, NULL) < 0)
        return -1;

    return (ssize_t) obj_name_len;
}
/* PROV-IO instrument end */

//======================================= statistics =======================================

/********************* */
/* Function prototypes */
/********************* */

/* Helper routines  */
static H5VL_provenance_t *H5VL_provenance_new_obj(void *under_obj,
    hid_t under_vol_id, prov_helper_t* helper);
static herr_t H5VL_provenance_free_obj(H5VL_provenance_t *obj);

/* "Management" callbacks */
static herr_t H5VL_provenance_init(hid_t vipl_id);
static herr_t H5VL_provenance_term(void);
static void *H5VL_provenance_info_copy(const void *info);
static herr_t H5VL_provenance_info_cmp(int *cmp_value, const void *info1, const void *info2);
static herr_t H5VL_provenance_info_free(void *info);
static herr_t H5VL_provenance_info_to_str(const void *info, char **str);
static herr_t H5VL_provenance_str_to_info(const char *str, void **info);
static void *H5VL_provenance_get_object(const void *obj);
static herr_t H5VL_provenance_get_wrap_ctx(const void *obj, void **wrap_ctx);
static void *H5VL_provenance_wrap_object(void *under_under_in, H5I_type_t obj_type, void *wrap_ctx);
static void *H5VL_provenance_unwrap_object(void *under);
static herr_t H5VL_provenance_free_wrap_ctx(void *obj);

/* Attribute callbacks */
static void *H5VL_provenance_attr_create(void *obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t type_id, hid_t space_id, hid_t acpl_id,
    hid_t aapl_id, hid_t dxpl_id, void **req);
static void *H5VL_provenance_attr_open(void *obj, const H5VL_loc_params_t *loc_params, const char *name, hid_t aapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_attr_read(void *attr, hid_t mem_type_id, void *buf, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_attr_write(void *attr, hid_t mem_type_id, const void *buf, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_attr_get(void *obj, H5VL_attr_get_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_attr_specific(void *obj, const H5VL_loc_params_t *loc_params, H5VL_attr_specific_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_attr_optional(void *obj, H5VL_optional_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_attr_close(void *attr, hid_t dxpl_id, void **req);

/* Dataset callbacks */
static void *H5VL_provenance_dataset_create(void *obj, const H5VL_loc_params_t *loc_params,
    const char *ds_name, hid_t lcpl_id, hid_t type_id, hid_t space_id,
    hid_t dcpl_id, hid_t dapl_id, hid_t dxpl_id, void **req);
static void *H5VL_provenance_dataset_open(void *obj, const H5VL_loc_params_t *loc_params, const char *ds_name, hid_t dapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_dataset_read(void *dset, hid_t mem_type_id, hid_t mem_space_id,
                                    hid_t file_space_id, hid_t plist_id, void *buf, void **req);
static herr_t H5VL_provenance_dataset_write(void *dset, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t plist_id, const void *buf, void **req);
static herr_t H5VL_provenance_dataset_get(void *dset, H5VL_dataset_get_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_dataset_specific(void *obj, H5VL_dataset_specific_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_dataset_optional(void *obj, H5VL_optional_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_dataset_close(void *dset, hid_t dxpl_id, void **req);

/* Datatype callbacks */
static void *H5VL_provenance_datatype_commit(void *obj, const H5VL_loc_params_t *loc_params, const char *name, hid_t type_id, hid_t lcpl_id, hid_t tcpl_id, hid_t tapl_id, hid_t dxpl_id, void **req);
static void *H5VL_provenance_datatype_open(void *obj, const H5VL_loc_params_t *loc_params, const char *name, hid_t tapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_datatype_get(void *dt, H5VL_datatype_get_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_datatype_specific(void *obj, H5VL_datatype_specific_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_datatype_optional(void *obj, H5VL_optional_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_datatype_close(void *dt, hid_t dxpl_id, void **req);

/* File callbacks */
static void *H5VL_provenance_file_create(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id, hid_t dxpl_id, void **req);
static void *H5VL_provenance_file_open(const char *name, unsigned flags, hid_t fapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_file_get(void *file, H5VL_file_get_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_file_specific(void *file, H5VL_file_specific_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_file_optional(void *file, H5VL_optional_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_file_close(void *file, hid_t dxpl_id, void **req);

/* Group callbacks */
static void *H5VL_provenance_group_create(void *obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t lcpl_id, hid_t gcpl_id, hid_t gapl_id, hid_t dxpl_id, void **req);
static void *H5VL_provenance_group_open(void *obj, const H5VL_loc_params_t *loc_params, const char *name, hid_t gapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_group_get(void *obj, H5VL_group_get_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_group_specific(void *obj, H5VL_group_specific_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_group_optional(void *obj, H5VL_optional_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_group_close(void *grp, hid_t dxpl_id, void **req);

/* Link callbacks */
static herr_t H5VL_provenance_link_create(H5VL_link_create_args_t *args,
    void *obj, const H5VL_loc_params_t *loc_params, hid_t lcpl_id, hid_t lapl_id,
    hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_link_copy(void *src_obj, const H5VL_loc_params_t *loc_params1, void *dst_obj, const H5VL_loc_params_t *loc_params2, hid_t lcpl_id, hid_t lapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_link_move(void *src_obj, const H5VL_loc_params_t *loc_params1, void *dst_obj, const H5VL_loc_params_t *loc_params2, hid_t lcpl_id, hid_t lapl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_link_get(void *obj, const H5VL_loc_params_t *loc_params, H5VL_link_get_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_link_specific(void *obj, const H5VL_loc_params_t *loc_params, H5VL_link_specific_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_link_optional(void *obj, const H5VL_loc_params_t *loc_params, H5VL_optional_args_t *args, hid_t dxpl_id, void **req);

/* Object callbacks */
static void *H5VL_provenance_object_open(void *obj, const H5VL_loc_params_t *loc_params, H5I_type_t *obj_to_open_type, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_object_copy(void *src_obj, const H5VL_loc_params_t *src_loc_params, const char *src_name, void *dst_obj, const H5VL_loc_params_t *dst_loc_params, const char *dst_name, hid_t ocpypl_id, hid_t lcpl_id, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_object_get(void *obj, const H5VL_loc_params_t *loc_params, H5VL_object_get_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_object_specific(void *obj, const H5VL_loc_params_t *loc_params, H5VL_object_specific_args_t *args, hid_t dxpl_id, void **req);
static herr_t H5VL_provenance_object_optional(void *obj, const H5VL_loc_params_t *loc_params, H5VL_optional_args_t *args, hid_t dxpl_id, void **req);

/* Container/connector introspection callbacks */
static herr_t H5VL_provenance_introspect_get_cap_flags(const void *info, unsigned *cap_flags);
static herr_t H5VL_provenance_introspect_opt_query(void *obj, H5VL_subclass_t cls, int opt_type, uint64_t *flags);

/* Async request callbacks */
static herr_t H5VL_provenance_request_wait(void *req, uint64_t timeout, H5VL_request_status_t *status);
static herr_t H5VL_provenance_request_notify(void *obj, H5VL_request_notify_t cb, void *ctx);
static herr_t H5VL_provenance_request_cancel(void *req, H5VL_request_status_t *status);
static herr_t H5VL_provenance_request_specific(void *req, H5VL_request_specific_args_t *args);
static herr_t H5VL_provenance_request_optional(void *req, H5VL_optional_args_t *args);
static herr_t H5VL_provenance_request_free(void *req);

/* Blob callbacks */
static herr_t H5VL_provenance_blob_put(void *obj, const void *buf, size_t size, void *blob_id, void *ctx);
static herr_t H5VL_provenance_blob_get(void *obj, const void *blob_id, void *buf, size_t size, void *ctx);
static herr_t H5VL_provenance_blob_specific(void *obj, void *blob_id, H5VL_blob_specific_args_t *args);
static herr_t H5VL_provenance_blob_optional(void *obj, void *blob_id, H5VL_optional_args_t *args);

/* Token callbacks */
static herr_t H5VL_provenance_token_cmp(void *obj, const H5O_token_t *token1, const H5O_token_t *token2, int *cmp_value);
static herr_t H5VL_provenance_token_to_str(void *obj, H5I_type_t obj_type, const H5O_token_t *token, char **token_str);
static herr_t H5VL_provenance_token_from_str(void *obj, H5I_type_t obj_type, const char *token_str, H5O_token_t *token);

/* Catch-all optional callback */
static herr_t H5VL_provenance_optional(void *obj, H5VL_optional_args_t *args, hid_t dxpl_id, void **req);

/*******************/
/* Local variables */
/*******************/

/* PROVENANCE VOL connector class struct */
static const H5VL_class_t H5VL_provenance_cls = {
    H5VL_VERSION,                            /* VOL class struct version */
    (H5VL_class_value_t)H5VL_PROVNC_VALUE,          /* value        */
    H5VL_PROVNC_NAME,                               /* name         */
    H5VL_PROVNC_VERSION,                            /* version      */
    0,                                              /* capability flags */
    H5VL_provenance_init,                           /* initialize   */
    H5VL_provenance_term,                           /* terminate    */
    {                                           /* info_cls */
        sizeof(H5VL_provenance_info_t),             /* info size    */
        H5VL_provenance_info_copy,                  /* info copy    */
        H5VL_provenance_info_cmp,                   /* info compare */
        H5VL_provenance_info_free,                  /* info free    */
        H5VL_provenance_info_to_str,                /* info to str  */
        H5VL_provenance_str_to_info,                /* str to info  */
    },
    {                                           /* wrap_cls */
        H5VL_provenance_get_object,                 /* get_object */
        H5VL_provenance_get_wrap_ctx,               /* get_wrap_ctx */
        H5VL_provenance_wrap_object,                /* wrap_object */
        H5VL_provenance_unwrap_object,              /* unwrap_object */
        H5VL_provenance_free_wrap_ctx,              /* free_wrap_ctx */
    },
    {                                           /* attribute_cls */
        H5VL_provenance_attr_create,                /* create */
        H5VL_provenance_attr_open,                  /* open */
        H5VL_provenance_attr_read,                  /* read */
        H5VL_provenance_attr_write,                 /* write */
        H5VL_provenance_attr_get,                   /* get */
        H5VL_provenance_attr_specific,              /* specific */
        H5VL_provenance_attr_optional,              /* optional */
        H5VL_provenance_attr_close                  /* close */
    },
    {                                           /* dataset_cls */
        H5VL_provenance_dataset_create,             /* create */
        H5VL_provenance_dataset_open,               /* open */
        H5VL_provenance_dataset_read,               /* read */
        H5VL_provenance_dataset_write,              /* write */
        H5VL_provenance_dataset_get,                /* get */
        H5VL_provenance_dataset_specific,           /* specific */
        H5VL_provenance_dataset_optional,           /* optional */
        H5VL_provenance_dataset_close               /* close */
    },
    {                                           /* datatype_cls */
        H5VL_provenance_datatype_commit,            /* commit */
        H5VL_provenance_datatype_open,              /* open */
        H5VL_provenance_datatype_get,               /* get_size */
        H5VL_provenance_datatype_specific,          /* specific */
        H5VL_provenance_datatype_optional,          /* optional */
        H5VL_provenance_datatype_close              /* close */
    },
    {                                           /* file_cls */
        H5VL_provenance_file_create,                /* create */
        H5VL_provenance_file_open,                  /* open */
        H5VL_provenance_file_get,                   /* get */
        H5VL_provenance_file_specific,              /* specific */
        H5VL_provenance_file_optional,              /* optional */
        H5VL_provenance_file_close                  /* close */
    },
    {                                           /* group_cls */
        H5VL_provenance_group_create,               /* create */
        H5VL_provenance_group_open,                 /* open */
        H5VL_provenance_group_get,                  /* get */
        H5VL_provenance_group_specific,             /* specific */
        H5VL_provenance_group_optional,             /* optional */
        H5VL_provenance_group_close                 /* close */
    },
    {                                           /* link_cls */
        H5VL_provenance_link_create,                /* create */
        H5VL_provenance_link_copy,                  /* copy */
        H5VL_provenance_link_move,                  /* move */
        H5VL_provenance_link_get,                   /* get */
        H5VL_provenance_link_specific,              /* specific */
        H5VL_provenance_link_optional,              /* optional */
    },
    {                                           /* object_cls */
        H5VL_provenance_object_open,                /* open */
        H5VL_provenance_object_copy,                /* copy */
        H5VL_provenance_object_get,                 /* get */
        H5VL_provenance_object_specific,            /* specific */
        H5VL_provenance_object_optional,            /* optional */
    },
    {                                           /* introspect_cls */
        NULL,                                       /* get_conn_cls */
        H5VL_provenance_introspect_get_cap_flags,   /* get_cap_flags */
        H5VL_provenance_introspect_opt_query,       /* opt_query */
    },
    {                                           /* request_cls */
        H5VL_provenance_request_wait,               /* wait */
        H5VL_provenance_request_notify,             /* notify */
        H5VL_provenance_request_cancel,             /* cancel */
        H5VL_provenance_request_specific,           /* specific */
        H5VL_provenance_request_optional,           /* optional */
        H5VL_provenance_request_free                /* free */
    },
    {                                           /* blobs_cls */
        H5VL_provenance_blob_put,                   /* put */
        H5VL_provenance_blob_get,                   /* get */
        H5VL_provenance_blob_specific,              /* specific */
        H5VL_provenance_blob_optional               /* optional */
    },
    {                                           /* token_cls */
        H5VL_provenance_token_cmp,                  /* cmp */
        H5VL_provenance_token_to_str,               /* to_str */
        H5VL_provenance_token_from_str              /* from_str */
    },
    H5VL_provenance_optional                    /* optional */
};

H5PL_type_t H5PLget_plugin_type(void) {return H5PL_TYPE_VOL;}
const void *H5PLget_plugin_info(void) {return &H5VL_provenance_cls;}

/* The connector identification number, initialized at runtime */
static hid_t prov_connector_id_global = H5I_INVALID_HID;

/* Local routine prototypes */
static hid_t dataset_get_type(void *under_dset, hid_t under_vol_id, hid_t dxpl_id);
static hid_t dataset_get_space(void *under_dset, hid_t under_vol_id, hid_t dxpl_id);
static hid_t dataset_get_dcpl(void *under_dset, hid_t under_vol_id, hid_t dxpl_id);
static ssize_t attr_get_name(void *under_obj, hid_t under_vol_id, hid_t dxpl_id,
    size_t buf_size, void *buf);
datatype_prov_info_t *new_dtype_info(file_prov_info_t* root_file,
    const char *name, H5O_token_t token);
dataset_prov_info_t *new_dataset_info(file_prov_info_t *root_file,
    const char *name, H5O_token_t token);
group_prov_info_t *new_group_info(file_prov_info_t *root_file,
    const char *name, H5O_token_t token);
attribute_prov_info_t *new_attribute_info(file_prov_info_t *root_file,
    const char *name, H5O_token_t token);
file_prov_info_t *new_file_info(const char* fname, unsigned long file_no);
void dtype_info_free(datatype_prov_info_t* info);
void file_info_free(file_prov_info_t* info);
void group_info_free(group_prov_info_t* info);
void dataset_info_free(dataset_prov_info_t* info);
void attribute_info_free(attribute_prov_info_t *info);
void dataset_stats_prov_write(const dataset_prov_info_t* ds_info);
void file_stats_prov_write(const file_prov_info_t* file_info);
void datatype_stats_prov_write(const datatype_prov_info_t* dt_info);
void group_stats_prov_write(const group_prov_info_t* grp_info);
void attribute_stats_prov_write(const attribute_prov_info_t *attr_info);
void prov_helper_teardown(prov_helper_t* helper);
void file_ds_created(file_prov_info_t* info);
void file_ds_accessed(file_prov_info_t* info);
datatype_prov_info_t *add_dtype_node(file_prov_info_t *file_info,
    H5VL_provenance_t *dtype, const char *obj_name, H5O_token_t token);
int rm_dtype_node(prov_helper_t *helper, void *under, hid_t under_vol_id, datatype_prov_info_t *dtype_info);
group_prov_info_t *add_grp_node(file_prov_info_t *root_file,
    H5VL_provenance_t *upper_o, const char *obj_name, H5O_token_t token);
int rm_grp_node(prov_helper_t *helper, void *under_obj, hid_t under_vol_id, group_prov_info_t *grp_info);
attribute_prov_info_t *add_attr_node(file_prov_info_t *root_file,
    H5VL_provenance_t *attr, const char *obj_name, H5O_token_t token);
int rm_attr_node(prov_helper_t *helper, void *under_obj, hid_t under_vol_id, attribute_prov_info_t *attr_info);
file_prov_info_t* add_file_node(prov_helper_t* helper, const char* file_name, unsigned long file_no);
int rm_file_node(prov_helper_t* helper, unsigned long file_no);
file_prov_info_t* _search_home_file(unsigned long obj_file_no);
dataset_prov_info_t* add_dataset_node(unsigned long obj_file_no, H5VL_provenance_t *dset, H5O_token_t token,
        file_prov_info_t* file_info_in, const char* ds_name, hid_t dxpl_id, void** req);
int rm_dataset_node(prov_helper_t *helper, void *under_obj, hid_t under_vol_id, dataset_prov_info_t *dset_info);
void ptr_cnt_increment(prov_helper_t* helper);
void ptr_cnt_decrement(prov_helper_t* helper);
void get_time_str(char *str_out);
dataset_prov_info_t* new_ds_prov_info(void* under_object, hid_t vol_id, H5O_token_t token,
        file_prov_info_t* file_info, const char* ds_name, hid_t dxpl_id, void **req);
void _new_loc_pram(H5I_type_t type, H5VL_loc_params_t *lparam);
static int get_native_info(void *obj, H5I_type_t target_obj_type, hid_t connector_id,
                       hid_t dxpl_id, H5O_info2_t *oinfo);
static int get_native_file_no(const H5VL_provenance_t *file_obj, unsigned long *file_num_out);
herr_t provenance_file_setup(const char* str_in, char* file_path_out, Prov_level* level_out, char* format_out);
H5VL_provenance_t* _fake_obj_new(file_prov_info_t* root_file, hid_t under_vol_id);
void _fake_obj_free(H5VL_provenance_t* obj);
H5VL_provenance_t* _obj_wrap_under(void* under, H5VL_provenance_t* upper_o,
        const char *name, H5I_type_t type, hid_t dxpl_id, void** req);
H5VL_provenance_t* _file_open_common(void* under, hid_t vol_id, const char* name);
unsigned int genHash(const char *msg);
void _dic_init(void);
void _dic_print(void);
void _dic_free(void);
void _preset_dic_print(void);

// static
// unsigned long get_time_usec(void) {
//     struct timeval tp;

//     gettimeofday(&tp, NULL);
//     return (unsigned long)((1000000 * tp.tv_sec) + tp.tv_usec);
// }

datatype_prov_info_t *new_dtype_info(file_prov_info_t* root_file,
    const char *name, H5O_token_t token)
{
    datatype_prov_info_t *info;

    info = (datatype_prov_info_t *)calloc(1, sizeof(datatype_prov_info_t));
    info->obj_info.prov_helper = PROV_HELPER;
    info->obj_info.file_info = root_file;
    info->obj_info.name = name ? strdup(name) : NULL;
    info->obj_info.token = token;

    return info;
}

dataset_prov_info_t *new_dataset_info(file_prov_info_t *root_file,
    const char *name, H5O_token_t token)
{
    dataset_prov_info_t *info;

    info = (dataset_prov_info_t *)calloc(1, sizeof(dataset_prov_info_t));
    info->obj_info.prov_helper = PROV_HELPER;
    info->obj_info.file_info = root_file;
    info->obj_info.name = name ? strdup(name) : NULL;
    info->obj_info.token = token;

    return info;
}

group_prov_info_t *new_group_info(file_prov_info_t *root_file,
    const char *name, H5O_token_t token)
{
    group_prov_info_t *info;

    info = (group_prov_info_t *)calloc(1, sizeof(group_prov_info_t));
    info->obj_info.prov_helper = PROV_HELPER;
    info->obj_info.file_info = root_file;
    info->obj_info.name = name ? strdup(name) : NULL;
    info->obj_info.token = token;

    return info;
}

attribute_prov_info_t *new_attribute_info(file_prov_info_t *root_file,
    const char *name, H5O_token_t token)
{
    attribute_prov_info_t *info;

    info = (attribute_prov_info_t *)calloc(1, sizeof(attribute_prov_info_t));
    info->obj_info.prov_helper = PROV_HELPER;
    info->obj_info.file_info = root_file;
    info->obj_info.name = name ? strdup(name) : NULL;
    info->obj_info.token = token;

    return info;
}

file_prov_info_t* new_file_info(const char* fname, unsigned long file_no)
{
    file_prov_info_t *info;

    info = (file_prov_info_t *)calloc(1, sizeof(file_prov_info_t));
    info->file_name = fname ? strdup(fname) : NULL;
    info->prov_helper = PROV_HELPER;
    info->file_no = file_no;

    return info;
}

void dtype_info_free(datatype_prov_info_t* info)
{
    if(info->obj_info.name)
        free(info->obj_info.name);
    free(info);
}

void file_info_free(file_prov_info_t* info)
{
#ifdef H5_HAVE_PARALLEL
    // Release MPI Comm & Info, if they are valid
    if(info->mpi_comm_info_valid) {
	if(MPI_COMM_NULL != info->mpi_comm)
	    MPI_Comm_free(&info->mpi_comm);
	if(MPI_INFO_NULL != info->mpi_info)
	    MPI_Info_free(&info->mpi_info);
    }
#endif /* H5_HAVE_PARALLEL */
    if(info->file_name)
        free(info->file_name);
    free(info);
}

void group_info_free(group_prov_info_t* info)
{
    if(info->obj_info.name)
        free(info->obj_info.name);
    free(info);
}

void dataset_info_free(dataset_prov_info_t* info)
{
    if(info->obj_info.name)
        free(info->obj_info.name);
    free(info);
}

void attribute_info_free(attribute_prov_info_t* info)
{
    if(info->obj_info.name)
        free(info->obj_info.name);
    free(info);
}

void dataset_stats_prov_write(const dataset_prov_info_t* ds_info){
    if(!ds_info){
//        printf("dataset_stats_prov_write(): ds_info is NULL.\n");
        return;
    }
//    printf("Dataset name = %s,\ndata type class = %d, data space class = %d, data space size = %llu, data type size =%zu.\n",
//            ds_info->dset_name, ds_info->dt_class, ds_info->ds_class,  (unsigned long long)ds_info->dset_space_size, ds_info->dset_type_size);
//    printf("Dataset is %u dimensions.\n", ds_info->dimension_cnt);
//    printf("Dataset is read %d time, %llu bytes in total, costs %llu us.\n", ds_info->dataset_read_cnt, ds_info->total_bytes_read, ds_info->total_read_time);
//    printf("Dataset is written %d time, %llu bytes in total, costs %llu us.\n", ds_info->dataset_write_cnt, ds_info->total_bytes_written, ds_info->total_write_time);
}

//not file_prov_info_t!
void file_stats_prov_write(const file_prov_info_t* file_info) {
    if(!file_info){
 //       printf("file_stats_prov_write(): ds_info is NULL.\n");
        return;
    }

    //printf("H5 file closed, %d datasets are created, %d datasets are accessed.\n", file_info->ds_created, file_info->ds_accessed);

}

void datatype_stats_prov_write(const datatype_prov_info_t* dt_info) {
    if(!dt_info){
        //printf("datatype_stats_prov_write(): ds_info is NULL.\n");
        return;
    }
    //printf("Datatype name = %s, commited %d times, datatype get is called %d times.\n", dt_info->dtype_name, dt_info->datatype_commit_cnt, dt_info->datatype_get_cnt);
}

void group_stats_prov_write(const group_prov_info_t* grp_info) {
    if(!grp_info){
        //printf("group_stats_prov_write(): grp_info is NULL.\n");
        return;
    }
    //printf("group_stats_prov_write() is yet to be implemented.\n");
}

void attribute_stats_prov_write(const attribute_prov_info_t *attr_info) {
    if(!attr_info){
        //printf("attribute_stats_prov_write(): attr_info is NULL.\n");
        return;
    }
    //printf("attribute_stats_prov_write() is yet to be implemented.\n");
}

void prov_verify_open_things(int open_files, int open_dsets)
{
    if(PROV_HELPER) {
        assert(open_files == PROV_HELPER->opened_files_cnt);

        /* Check opened datasets */
        if(open_files > 0) {
            file_prov_info_t* opened_file;
            int total_open_dsets = 0;

            opened_file = PROV_HELPER->opened_files;
            while(opened_file) {
                total_open_dsets += opened_file->opened_datasets_cnt;
                opened_file = opened_file->next;
            }
            assert(open_dsets == total_open_dsets);
        }
    }
}

// need to be fixed if the function got called
void prov_dump_open_things(FILE *f)
{
    if(PROV_HELPER) {
        file_prov_info_t *opened_file;
        unsigned file_count = 0;

        fprintf(f, "# of open files: %d\n", PROV_HELPER->opened_files_cnt);

        /* Print opened files */
        opened_file = PROV_HELPER->opened_files;
        while(opened_file) {
            dataset_prov_info_t *opened_dataset;
            unsigned dset_count = 0;

            fprintf(f, "file #%u: info ptr = %p, name = '%s', fileno = %lu\n", file_count, (void *)opened_file, opened_file->file_name, opened_file->file_no);
            fprintf(f, "\tref_cnt = %d\n", opened_file->ref_cnt);

            /* Print opened datasets */
            fprintf(f, "\topened_datasets_cnt = %d\n", opened_file->opened_datasets_cnt);
            opened_dataset = opened_file->opened_datasets;
            while(opened_dataset) {
                // need to be fixed if the function got called
                // fprintf(f, "\t\tdataset #%u: name = '%s', objno = %llu\n", dset_count, opened_dataset->obj_info.name, (unsigned long long)opened_dataset->obj_info.objno);
                fprintf(f, "\t\t\tfile_info ptr = %p\n", (void *)opened_dataset->obj_info.file_info);
                fprintf(f, "\t\t\tref_cnt = %d\n", opened_dataset->obj_info.ref_cnt);

                dset_count++;
                opened_dataset = opened_dataset->next;
            }

            fprintf(f, "\topened_grps_cnt = %d\n", opened_file->opened_grps_cnt);
            fprintf(f, "\topened_dtypes_cnt = %d\n", opened_file->opened_dtypes_cnt);
            fprintf(f, "\topened_attrs_cnt = %d\n", opened_file->opened_attrs_cnt);

            file_count++;
            opened_file = opened_file->next;
        }
    }
    else
        fprintf(f, "PROV_HELPER not initialized\n");
}

prov_helper_t * prov_helper_init( char* file_path, Prov_level prov_level, char* prov_line_format)
{
    prov_helper_t* new_helper = (prov_helper_t *)calloc(1, sizeof(prov_helper_t));

    if(prov_level >= 2) {//write to file
        if(!file_path){
            printf("prov_helper_init() failed, provenance file path is not set.\n");
            return NULL;
        }
    }

    /* PROV-IO instrument point */
    provio_helper = provio_helper_init(&config, &fields);

    new_helper->prov_file_path = strdup(file_path);
    new_helper->prov_line_format = strdup(prov_line_format);
    new_helper->prov_level = prov_level;
    new_helper->pid = getpid();
    new_helper->tid = pthread_self();

    new_helper->opened_files = NULL;
    new_helper->opened_files_cnt = 0;

    getlogin_r(new_helper->user_name, 32);

    // if(new_helper->prov_level == File_only || new_helper->prov_level == File_and_print)
    //     new_helper->prov_file_handle = fopen(new_helper->prov_file_path, "a");

    _dic_init();
    return new_helper;
}

void prov_helper_teardown(prov_helper_t* helper){
    if(helper){// not null
        char pline[512];
        sprintf(pline,
                "TOTAL_PROV_OVERHEAD %lu\n"
                "TOTAL_NATIVE_H5_TIME %lu\n"
                "PROV_WRITE_TOTAL_TIME %lu\n"
                "FILE_LL_TOTAL_TIME %lu\n"
                "DS_LL_TOTAL_TIME %lu\n"
                "GRP_LL_TOTAL_TIME %lu\n"
                "DT_LL_TOTAL_TIME %lu\n"
                "ATTR_LL_TOTAL_TIME %lu\n",
                TOTAL_PROV_OVERHEAD,
                TOTAL_NATIVE_H5_TIME,
                PROV_WRITE_TOTAL_TIME,
                FILE_LL_TOTAL_TIME,
                DS_LL_TOTAL_TIME,
                GRP_LL_TOTAL_TIME,
                DT_LL_TOTAL_TIME,
                ATTR_LL_TOTAL_TIME);

        switch(helper->prov_level){
            case File_only:
                // fputs(pline, helper->prov_file_handle);
                break;

            case File_and_print:
                // fputs(pline, helper->prov_file_handle);
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


        // if(helper->prov_level == File_only || helper->prov_level ==File_and_print){//no file
        //     fflush(helper->prov_file_handle);
        //     fclose(helper->prov_file_handle);
        // }
        if(helper->prov_file_path)
            free(helper->prov_file_path);
        if(helper->prov_line_format)
            free(helper->prov_line_format);

        /* PROV-IO instrument point */
        if(provio_helper)
            provio_helper_teardown(&config, provio_helper, &fields);

        free(helper);
        _dic_free();
    }
}

void file_ds_created(file_prov_info_t *info)
{
    assert(info);
    if(info)
        info->ds_created++;
}

//counting how many times datasets are opened in a file.
//Called by a DS
void file_ds_accessed(file_prov_info_t* info)
{
    assert(info);
    if(info)
        info->ds_accessed++;
}

datatype_prov_info_t * add_dtype_node(file_prov_info_t *file_info,
    H5VL_provenance_t *dtype, const char *obj_name, H5O_token_t token)
{
    unsigned long start = get_time_usec();
    datatype_prov_info_t *cur;
    int cmp_value;

    assert(file_info);

    // Find datatype in linked list of opened datatypes
    cur = file_info->opened_dtypes;
    while (cur) {
        if (H5VLtoken_cmp(dtype->under_object, dtype->under_vol_id,
		          &(cur->obj_info.token), &token, &cmp_value) < 0)
	    fprintf(stderr, "H5VLtoken_cmp error");
        if (cmp_value == 0)
            break;
        cur = cur->next;
    }

    if(!cur) {
        // Allocate and initialize new datatype node
        cur = new_dtype_info(file_info, obj_name, token);

        // Increment refcount on file info
        file_info->ref_cnt++;

        // Add to linked list
        cur->next = file_info->opened_dtypes;
        file_info->opened_dtypes = cur;
        file_info->opened_dtypes_cnt++;
    }

    // Increment refcount on datatype
    cur->obj_info.ref_cnt++;

    DT_LL_TOTAL_TIME += (get_time_usec() - start);
    return cur;
}

int rm_dtype_node(prov_helper_t *helper, void *under, hid_t under_vol_id, datatype_prov_info_t *dtype_info)
{
    unsigned long start = get_time_usec();
    file_prov_info_t *file_info;
    datatype_prov_info_t *cur;
    datatype_prov_info_t *last;
    int cmp_value;

    // Decrement refcount
    dtype_info->obj_info.ref_cnt--;

    // If refcount still >0, leave now
    if(dtype_info->obj_info.ref_cnt > 0)
        return dtype_info->obj_info.ref_cnt;

    // Refcount == 0, remove datatype from file info

    file_info = dtype_info->obj_info.file_info;
    assert(file_info);
    assert(file_info->opened_dtypes);

    cur = file_info->opened_dtypes;
    last = cur;
    while(cur) {
        if (H5VLtoken_cmp(under, under_vol_id, &(cur->obj_info.token),
                          &(dtype_info->obj_info.token), &cmp_value) < 0)
	    fprintf(stderr, "H5VLtoken_cmp error");
        if (cmp_value == 0) {
            //special case: first node is the target, ==cur
            if(cur == file_info->opened_dtypes)
                file_info->opened_dtypes = file_info->opened_dtypes->next;
            else
                last->next = cur->next;

            dtype_info_free(cur);

            file_info->opened_dtypes_cnt--;
            if(file_info->opened_dtypes_cnt == 0)
                assert(file_info->opened_dtypes == NULL);

            // Decrement refcount on file info
            DT_LL_TOTAL_TIME += (get_time_usec() - start);
            rm_file_node(helper, file_info->file_no);

            return 0;
        }

        last = cur;
        cur = cur->next;
    }

    DT_LL_TOTAL_TIME += (get_time_usec() - start);
    //node not found.
    return -1;
}

group_prov_info_t *add_grp_node(file_prov_info_t *file_info,
    H5VL_provenance_t *upper_o, const char *obj_name, H5O_token_t token)
{
    group_prov_info_t *cur;
    unsigned long start = get_time_usec();
    assert(file_info);
    int cmp_value;

    // Find group in linked list of opened groups
    cur = file_info->opened_grps;
    while (cur) {
        if (H5VLtoken_cmp(upper_o->under_object, upper_o->under_vol_id,
                          &(cur->obj_info.token), &token, &cmp_value) < 0)
	    fprintf(stderr, "H5VLtoken_cmp error");
        if (cmp_value == 0)
            break;
        cur = cur->next;
    }

    if(!cur) {
        // Allocate and initialize new group node
        cur = new_group_info(file_info, obj_name, token);

        // Increment refcount on file info
        file_info->ref_cnt++;

        // Add to linked list
        cur->next = file_info->opened_grps;
        file_info->opened_grps = cur;
        file_info->opened_grps_cnt++;
    }

    // Increment refcount on group
    cur->obj_info.ref_cnt++;

    GRP_LL_TOTAL_TIME += (get_time_usec() - start);
    return cur;
}

int rm_grp_node(prov_helper_t *helper, void *under_obj, hid_t under_vol_id, group_prov_info_t *grp_info)
{   unsigned long start = get_time_usec();
    file_prov_info_t *file_info;
    group_prov_info_t *cur;
    group_prov_info_t *last;
    int cmp_value;

    // Decrement refcount
    grp_info->obj_info.ref_cnt--;

    // If refcount still >0, leave now
    if(grp_info->obj_info.ref_cnt > 0)
        return grp_info->obj_info.ref_cnt;

    // Refcount == 0, remove group from file info

    file_info = grp_info->obj_info.file_info;
    assert(file_info);
    assert(file_info->opened_grps);

    cur = file_info->opened_grps;
    last = cur;
    while(cur) {
        if (H5VLtoken_cmp(under_obj, under_vol_id, &(cur->obj_info.token),
                          &(grp_info->obj_info.token), &cmp_value) < 0)
	    fprintf(stderr, "H5VLtoken_cmp error");
        if (cmp_value == 0) { //node found
            //special case: first node is the target, ==cur
            if (cur == file_info->opened_grps)
                file_info->opened_grps = file_info->opened_grps->next;
            else
                last->next = cur->next;

            group_info_free(cur);

            file_info->opened_grps_cnt--;
            if (file_info->opened_grps_cnt == 0)
                assert(file_info->opened_grps == NULL);

            // Decrement refcount on file info
            GRP_LL_TOTAL_TIME += (get_time_usec() - start);
            rm_file_node(helper, file_info->file_no);

            return 0;
        }

        last = cur;
        cur = cur->next;
    }

    GRP_LL_TOTAL_TIME += (get_time_usec() - start);
    //node not found.
    return -1;
}

attribute_prov_info_t *add_attr_node(file_prov_info_t *file_info,
    H5VL_provenance_t *attr, const char *obj_name, H5O_token_t token)
{   unsigned long start = get_time_usec();
    attribute_prov_info_t *cur;
    int cmp_value;

    assert(file_info);

    // Find attribute in linked list of opened attributes
    cur = file_info->opened_attrs;
    while (cur) {
        if (H5VLtoken_cmp(attr->under_object, attr->under_vol_id,
                          &(cur->obj_info.token), &token, &cmp_value) < 0)
	    fprintf(stderr, "H5VLtoken_cmp error");
        if (cmp_value == 0)
            break;
        cur = cur->next;
    }

    if(!cur) {
        // Allocate and initialize new attribute node
        cur = new_attribute_info(file_info, obj_name, token);

        // Increment refcount on file info
        file_info->ref_cnt++;

        // Add to linked list
        cur->next = file_info->opened_attrs;
        file_info->opened_attrs = cur;
        file_info->opened_attrs_cnt++;
    }

    // Increment refcount on attribute
    cur->obj_info.ref_cnt++;

    ATTR_LL_TOTAL_TIME += (get_time_usec() - start);
    return cur;
}

int rm_attr_node(prov_helper_t *helper, void *under_obj, hid_t under_vol_id, attribute_prov_info_t *attr_info)
{   unsigned long start = get_time_usec();
    file_prov_info_t *file_info;
    attribute_prov_info_t *cur;
    attribute_prov_info_t *last;
    int cmp_value;

    // Decrement refcount
    attr_info->obj_info.ref_cnt--;

    // If refcount still >0, leave now
    if(attr_info->obj_info.ref_cnt > 0)
        return attr_info->obj_info.ref_cnt;

    // Refcount == 0, remove attribute from file info

    file_info = attr_info->obj_info.file_info;
    assert(file_info);
    assert(file_info->opened_attrs);

    cur = file_info->opened_attrs;
    last = cur;
    while(cur) {
	if (H5VLtoken_cmp(under_obj, under_vol_id, &(cur->obj_info.token),
                          &(attr_info->obj_info.token), &cmp_value) < 0)
	    fprintf(stderr, "H5VLtoken_cmp error");
	if (cmp_value == 0) { //node found
            //special case: first node is the target, ==cur
            if(cur == file_info->opened_attrs)
                file_info->opened_attrs = file_info->opened_attrs->next;
            else
                last->next = cur->next;

            attribute_info_free(cur);

            file_info->opened_attrs_cnt--;
            if(file_info->opened_attrs_cnt == 0)
                assert(file_info->opened_attrs == NULL);

            ATTR_LL_TOTAL_TIME += (get_time_usec() - start);

            // Decrement refcount on file info
            rm_file_node(helper, file_info->file_no);

            return 0;
        }

        last = cur;
        cur = cur->next;
    }

    ATTR_LL_TOTAL_TIME += (get_time_usec() - start);
    //node not found.
    return -1;
}

file_prov_info_t* add_file_node(prov_helper_t* helper, const char* file_name,
    unsigned long file_no)
{
    unsigned long start = get_time_usec();
    file_prov_info_t* cur;

    assert(helper);

    if(!helper->opened_files) //empty linked list, no opened file.
        assert(helper->opened_files_cnt == 0);

    // Search for file in list of currently opened ones
    cur = helper->opened_files;
    while (cur) {
        assert(cur->file_no);

        if (cur->file_no == file_no)
            break;

        cur = cur->next;
    }

    if(!cur) {
        // Allocate and initialize new file node
        cur = new_file_info(file_name, file_no);

        // Add to linked list
        cur->next = helper->opened_files;
        helper->opened_files = cur;
        helper->opened_files_cnt++;
    }

    // Increment refcount on file node
    cur->ref_cnt++;

    FILE_LL_TOTAL_TIME += (get_time_usec() - start);
    return cur;
}

//need a dumy node to make it simpler
int rm_file_node(prov_helper_t* helper, unsigned long file_no)
{
    unsigned long start = get_time_usec();
    file_prov_info_t* cur;
    file_prov_info_t* last;

    assert(helper);
    assert(helper->opened_files);
    assert(helper->opened_files_cnt);
    assert(file_no);

    cur = helper->opened_files;
    last = cur;
    while(cur) {
        // Node found
        if(cur->file_no == file_no) {
            // Decrement file node's refcount
            cur->ref_cnt--;

            // If refcount == 0, remove file node & maybe print file stats
            if(cur->ref_cnt == 0) {
                // Sanity checks
                assert(0 == cur->opened_datasets_cnt);
                assert(0 == cur->opened_grps_cnt);
                assert(0 == cur->opened_dtypes_cnt);
                assert(0 == cur->opened_attrs_cnt);

                // Unlink from list of opened files
                if(cur == helper->opened_files) //first node is the target
                    helper->opened_files = helper->opened_files->next;
                else
                    last->next = cur->next;

                // Free file info
                file_info_free(cur);

                // Update connector info
                helper->opened_files_cnt--;
                if(helper->opened_files_cnt == 0)
                    assert(helper->opened_files == NULL);
            }

            break;
        }

        // Advance to next file node
        last = cur;
        cur = cur->next;
    }

    FILE_LL_TOTAL_TIME += (get_time_usec() - start);
    return helper->opened_files_cnt;
}

file_prov_info_t* _search_home_file(unsigned long obj_file_no){
    file_prov_info_t* cur;

    if(PROV_HELPER->opened_files_cnt < 1)
        return NULL;

    cur = PROV_HELPER->opened_files;
    while (cur) {
        if (cur->file_no == obj_file_no) {//file found
            cur->ref_cnt++;
            return cur;
        }

        cur = cur->next;
    }

    return NULL;
}

dataset_prov_info_t * add_dataset_node(unsigned long obj_file_no,
    H5VL_provenance_t *dset, H5O_token_t token,
    file_prov_info_t *file_info_in, const char* ds_name,
    hid_t dxpl_id, void** req)
{
    unsigned long start = get_time_usec();
    file_prov_info_t* file_info;
    dataset_prov_info_t* cur;
    int cmp_value;

    assert(dset);
    assert(dset->under_object);
    assert(file_info_in);
	
    if (obj_file_no != file_info_in->file_no) {//creating a dataset from an external place
        file_prov_info_t* external_home_file;

        external_home_file = _search_home_file(obj_file_no);
        if(external_home_file){//use extern home
            file_info = external_home_file;
        }else{//extern home not exist, fake one
            file_info = new_file_info("dummy", obj_file_no);
        }
    }else{//local
        file_info = file_info_in;
    }

    // Find dataset in linked list of opened datasets
    cur = file_info->opened_datasets;
    while (cur) {
        if (H5VLtoken_cmp(dset->under_object, dset->under_vol_id,
                          &(cur->obj_info.token), &token, &cmp_value) < 0)
	    fprintf(stderr, "H5VLtoken_cmp error");
        if (cmp_value == 0)
	    break;

        cur = cur->next;
    }

    if(!cur) {
        cur = new_ds_prov_info(dset->under_object, dset->under_vol_id, token, file_info, ds_name, dxpl_id, req);

        // Increment refcount on file info
        file_info->ref_cnt++;

        // Add to linked list of opened datasets
        cur->next = file_info->opened_datasets;
        file_info->opened_datasets = cur;
        file_info->opened_datasets_cnt++;
    }

    // Increment refcount on dataset
    cur->obj_info.ref_cnt++;

    DS_LL_TOTAL_TIME += (get_time_usec() - start);
    return cur;
}

//need a dumy node to make it simpler
int rm_dataset_node(prov_helper_t *helper, void *under_obj, hid_t under_vol_id, dataset_prov_info_t *dset_info)
{
    unsigned long start = get_time_usec();
    file_prov_info_t *file_info;
    dataset_prov_info_t *cur;
    dataset_prov_info_t *last;
    int cmp_value;

    // Decrement refcount
    dset_info->obj_info.ref_cnt--;

    // If refcount still >0, leave now
    if(dset_info->obj_info.ref_cnt > 0)
        return dset_info->obj_info.ref_cnt;

    // Refcount == 0, remove dataset from file info
    file_info = dset_info->obj_info.file_info;
    assert(file_info);
    assert(file_info->opened_datasets);

    cur = file_info->opened_datasets;
    last = cur;
    while(cur){
        if (H5VLtoken_cmp(under_obj, under_vol_id, &(cur->obj_info.token),
                          &(dset_info->obj_info.token), &cmp_value) < 0)
	    fprintf(stderr, "H5VLtoken_cmp error");
	if (cmp_value == 0) {//node found
            //special case: first node is the target, ==cur
            if(cur == file_info->opened_datasets)
                file_info->opened_datasets = file_info->opened_datasets->next;
            else
                last->next = cur->next;

            dataset_info_free(cur);

            file_info->opened_datasets_cnt--;
            if(file_info->opened_datasets_cnt == 0)
                assert(file_info->opened_datasets == NULL);

            // Decrement refcount on file info
            DS_LL_TOTAL_TIME += (get_time_usec() - start);
            rm_file_node(helper, file_info->file_no);

            return 0;
        }

        last = cur;
        cur = cur->next;
    }

    DS_LL_TOTAL_TIME += (get_time_usec() - start);
    //node not found.
    return -1;
}

//This function makes up a fake upper layer obj used as a parameter in _obj_wrap_under(..., H5VL_provenance_t* upper_o,... ),
//Use this in H5VL_provenance_wrap_object() ONLY!!!
H5VL_provenance_t* _fake_obj_new(file_prov_info_t *root_file, hid_t under_vol_id)
{
    H5VL_provenance_t* obj;

    obj = H5VL_provenance_new_obj(NULL, under_vol_id, PROV_HELPER);
    obj->my_type = H5I_FILE;  // FILE should work fine as a parent obj for all.
    obj->generic_prov_info = (void*)root_file;

    return obj;
}

void _fake_obj_free(H5VL_provenance_t *obj)
{
    H5VL_provenance_free_obj(obj);
}

/* under: obj need to be wrapped
 * upper_o: holder or upper layer object. Mostly used to pass root_file_info, vol_id, etc,.
 *      - it's a fake obj if called by H5VL_provenance_wrap_object().
 * target_obj_type:
 *      - for H5VL_provenance_wrap_object(obj_type): the obj should be wrapped into this type
 *      - for H5VL_provenance_object_open(): it's the obj need to be opened as this type
 *
 */
H5VL_provenance_t * _obj_wrap_under(void *under, H5VL_provenance_t *upper_o,
                                    const char *target_obj_name,
                                    H5I_type_t target_obj_type,
                                    hid_t dxpl_id, void **req)
{
    H5VL_provenance_t *obj;
    file_prov_info_t *file_info = NULL;

    if (under) {
        H5O_info2_t oinfo;
        H5O_token_t token;
        unsigned long file_no;

        //open from types
        switch(upper_o->my_type) {
            case H5I_DATASET:
            case H5I_GROUP:
            case H5I_DATATYPE:
            case H5I_ATTR:
                file_info = ((object_prov_info_t *)(upper_o->generic_prov_info))->file_info;
                break;

            case H5I_FILE:
                file_info = (file_prov_info_t*)upper_o->generic_prov_info;
                break;

            case H5I_UNINIT:
            case H5I_BADID:
            case H5I_DATASPACE:
            case H5I_VFL:
            case H5I_VOL:
            case H5I_GENPROP_CLS:
            case H5I_GENPROP_LST:
            case H5I_ERROR_CLASS:
            case H5I_ERROR_MSG:
            case H5I_ERROR_STACK:
            case H5I_NTYPES:
            default:
                file_info = NULL;  // Error
                break;
        }
        assert(file_info);

        obj = H5VL_provenance_new_obj(under, upper_o->under_vol_id, upper_o->prov_helper);

        /* Check for async request */
        if (req && *req)
            *req = H5VL_provenance_new_obj(*req, upper_o->under_vol_id, upper_o->prov_helper);

        //obj types
        if(target_obj_type != H5I_FILE) {
            // Sanity check
            assert(target_obj_type == H5I_DATASET || target_obj_type == H5I_GROUP ||
                    target_obj_type == H5I_DATATYPE || target_obj_type == H5I_ATTR);

            get_native_info(under, target_obj_type, upper_o->under_vol_id,
                            dxpl_id, &oinfo);
            token = oinfo.token;
            file_no = oinfo.fileno;
        }
        else
            get_native_file_no(obj, &file_no);

        switch (target_obj_type) {
            case H5I_DATASET:
                obj->generic_prov_info = add_dataset_node(file_no, obj, token, file_info, target_obj_name, dxpl_id, req);
                obj->my_type = H5I_DATASET;

                file_ds_accessed(file_info);
                break;

            case H5I_GROUP:
                obj->generic_prov_info = add_grp_node(file_info, obj, target_obj_name, token);
                obj->my_type = H5I_GROUP;
                break;

            case H5I_FILE: //newly added. if target_obj_name == NULL: it's a fake upper_o
                obj->generic_prov_info = add_file_node(PROV_HELPER, target_obj_name, file_no);
                obj->my_type = H5I_FILE;
                break;

            case H5I_DATATYPE:
                obj->generic_prov_info = add_dtype_node(file_info, obj, target_obj_name, token);
                obj->my_type = H5I_DATATYPE;
                break;

            case H5I_ATTR:
                obj->generic_prov_info = add_attr_node(file_info, obj, target_obj_name, token);
                obj->my_type = H5I_ATTR;
                break;

            case H5I_UNINIT:
            case H5I_BADID:
            case H5I_DATASPACE:
            case H5I_VFL:
            case H5I_VOL:
            case H5I_GENPROP_CLS:
            case H5I_GENPROP_LST:
            case H5I_ERROR_CLASS:
            case H5I_ERROR_MSG:
            case H5I_ERROR_STACK:
            case H5I_NTYPES:
            default:
                break;
        }
    } /* end if */
    else
        obj = NULL;

    return obj;
}

void ptr_cnt_increment(prov_helper_t* helper){
    assert(helper);

    //mutex lock

    if(helper){
        (helper->ptr_cnt)++;
    }

    //mutex unlock
}

void ptr_cnt_decrement(prov_helper_t* helper){
    assert(helper);

    //mutex lock

    helper->ptr_cnt--;

    //mutex unlock

    if(helper->ptr_cnt == 0){
        // do nothing for now.
        //prov_helper_teardown(helper);loggin is not decided yet.
    }
}


void get_time_str(char *str_out){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    *str_out = '\0';
    sprintf(str_out, "%d/%d/%d %d:%d:%d", timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

dataset_prov_info_t * new_ds_prov_info(void* under_object, hid_t vol_id, H5O_token_t token,
        file_prov_info_t* file_info, const char* ds_name, hid_t dxpl_id, void **req){
    hid_t dcpl_id = -1;
    hid_t dt_id = -1;
    hid_t ds_id = -1;
    dataset_prov_info_t* ds_info;

    assert(under_object);
    assert(file_info);

    ds_info = new_dataset_info(file_info, ds_name, token);

    dt_id = dataset_get_type(under_object, vol_id, dxpl_id);
    ds_info->dt_class = H5Tget_class(dt_id);
    ds_info->dset_type_size = H5Tget_size(dt_id);
    H5Tclose(dt_id);

    ds_id = dataset_get_space(under_object, vol_id, dxpl_id);
    ds_info->ds_class = H5Sget_simple_extent_type(ds_id);
    if (ds_info->ds_class == H5S_SIMPLE) {
        ds_info->dimension_cnt = (unsigned)H5Sget_simple_extent_ndims(ds_id);
        H5Sget_simple_extent_dims(ds_id, ds_info->dimensions, NULL);
        ds_info->dset_space_size = (hsize_t)H5Sget_simple_extent_npoints(ds_id);
    }
    H5Sclose(ds_id);

    dcpl_id = dataset_get_dcpl(under_object, vol_id, dxpl_id);
    ds_info->layout = H5Pget_layout(dcpl_id);
    H5Pclose(dcpl_id);

    return ds_info;
}

void _new_loc_pram(H5I_type_t type, H5VL_loc_params_t *lparam)
{
    assert(lparam);

    lparam->type = H5VL_OBJECT_BY_SELF;
    lparam->obj_type = type;
    return;
}

static int get_native_info(void *obj, H5I_type_t target_obj_type, hid_t connector_id,
                       hid_t dxpl_id, H5O_info2_t *oinfo)
{
    H5VL_object_get_args_t vol_cb_args; /* Arguments to VOL callback */
    H5VL_loc_params_t loc_params;

    /* Set up location parameter */
    _new_loc_pram(target_obj_type, &loc_params);

    /* Set up VOL callback arguments */
    vol_cb_args.op_type              = H5VL_OBJECT_GET_INFO;
    vol_cb_args.args.get_info.oinfo  = oinfo;
    vol_cb_args.args.get_info.fields = H5O_INFO_BASIC;

    if(H5VLobject_get(obj, &loc_params, connector_id, &vol_cb_args, dxpl_id, NULL) < 0)
        return -1;

    return 0;
}

static int get_native_file_no(const H5VL_provenance_t *file_obj, unsigned long *fileno)
{
    H5VL_file_get_args_t vol_cb_args; /* Arguments to VOL callback */

    /* Set up VOL callback arguments */
    vol_cb_args.op_type              = H5VL_FILE_GET_FILENO;
    vol_cb_args.args.get_fileno.fileno = fileno;

    if(H5VLfile_get(file_obj->under_object, file_obj->under_vol_id, &vol_cb_args, H5P_DEFAULT, NULL) < 0)
        return -1;

    return 0;
}

H5VL_provenance_t *_file_open_common(void *under, hid_t vol_id,
    const char *name)
{
    H5VL_provenance_t *file;
    unsigned long file_no = 0;

    file = H5VL_provenance_new_obj(under, vol_id, PROV_HELPER);
    file->my_type = H5I_FILE;
    get_native_file_no(file, &file_no);
    file->generic_prov_info = add_file_node(PROV_HELPER, name, file_no);

    return file;
}

static hid_t dataset_get_type(void *under_dset, hid_t under_vol_id, hid_t dxpl_id)
{
    H5VL_dataset_get_args_t vol_cb_args; /* Arguments to VOL callback */

    /* Set up VOL callback arguments */
    vol_cb_args.op_type              = H5VL_DATASET_GET_TYPE;
    vol_cb_args.args.get_type.type_id = H5I_INVALID_HID;

    if(H5VLdataset_get(under_dset, under_vol_id, &vol_cb_args, dxpl_id, NULL) < 0)
        return H5I_INVALID_HID;

    return vol_cb_args.args.get_type.type_id;
}

static hid_t dataset_get_space(void *under_dset, hid_t under_vol_id, hid_t dxpl_id)
{
    H5VL_dataset_get_args_t vol_cb_args; /* Arguments to VOL callback */

    /* Set up VOL callback arguments */
    vol_cb_args.op_type              = H5VL_DATASET_GET_SPACE;
    vol_cb_args.args.get_space.space_id = H5I_INVALID_HID;

    if(H5VLdataset_get(under_dset, under_vol_id, &vol_cb_args, dxpl_id, NULL) < 0)
        return H5I_INVALID_HID;

    return vol_cb_args.args.get_space.space_id;
}

static hid_t dataset_get_dcpl(void *under_dset, hid_t under_vol_id, hid_t dxpl_id)
{
    H5VL_dataset_get_args_t vol_cb_args; /* Arguments to VOL callback */

    /* Set up VOL callback arguments */
    vol_cb_args.op_type              = H5VL_DATASET_GET_DCPL;
    vol_cb_args.args.get_dcpl.dcpl_id = H5I_INVALID_HID;

    if(H5VLdataset_get(under_dset, under_vol_id, &vol_cb_args, dxpl_id, NULL) < 0)
        return H5I_INVALID_HID;

    return vol_cb_args.args.get_dcpl.dcpl_id;
}

static ssize_t attr_get_name(void *under_obj, hid_t under_vol_id, hid_t dxpl_id,
    size_t buf_size, void *buf)
{
    H5VL_attr_get_args_t vol_cb_args;        /* Arguments to VOL callback */
    size_t attr_name_len = 0;  /* Length of attribute name */

    /* Set up VOL callback arguments */
    vol_cb_args.op_type                           = H5VL_ATTR_GET_NAME;
    vol_cb_args.args.get_name.loc_params.type     = H5VL_OBJECT_BY_SELF;
    vol_cb_args.args.get_name.loc_params.obj_type = H5I_ATTR;
    vol_cb_args.args.get_name.buf_size            = buf_size;
    vol_cb_args.args.get_name.buf                 = buf;
    vol_cb_args.args.get_name.attr_name_len       = &attr_name_len;

    if (H5VLattr_get(under_obj, under_vol_id, &vol_cb_args, dxpl_id, NULL) < 0)
        return -1;

    return (ssize_t)attr_name_len;
}

//shorten function id: use hash value
static char* FUNC_DIC[STAT_FUNC_MOD];

void _dic_init(void){
    for(int i = 0; i < STAT_FUNC_MOD; i++){
        FUNC_DIC[i] = NULL;
    }
}

unsigned int genHash(const char *msg) {
    unsigned long hash = 0;
    unsigned long c;
    unsigned int func_index;
    const char* tmp = msg;

    while (0 != (c = (unsigned long)(*msg++))) {//SDBM hash
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    msg = tmp;//restore string head address
    func_index = (unsigned int)(hash % STAT_FUNC_MOD);
    if(!FUNC_DIC[func_index]) {
        FUNC_DIC[func_index] = strdup(msg);
        //printf("received msg = %s, hash index = %d, result msg = %s\n", msg, func_index, FUNC_DIC[func_index]);
    }

    return func_index;
}

void _dic_free(void){
    for(int i = 0; i < STAT_FUNC_MOD; i++){
        if(FUNC_DIC[i]){
            free(FUNC_DIC[i]);
        }
    }
}

void _dic_print(void){
    for(int i = 0; i < STAT_FUNC_MOD; i++){
        if(FUNC_DIC[i]){
            printf("%d %s\n", i, FUNC_DIC[i]);
        }
    }
}
void _preset_dic_print(void){
    const char* preset_dic[] = {
            "H5VL_provenance_init",                         /* initialize   */
            "H5VL_provenance_term",                         /* terminate    */
            "H5VL_provenance_info_copy",                /* info copy    */
            "H5VL_provenance_info_cmp",                 /* info compare */
            "H5VL_provenance_info_free",                /* info free    */
            "H5VL_provenance_info_to_str",              /* info to str  */
            "H5VL_provenance_str_to_info",              /* str to info  */
            "H5VL_provenance_get_object",               /* get_object   */
            "H5VL_provenance_get_wrap_ctx",             /* get_wrap_ctx */
            "H5VL_provenance_wrap_object",              /* wrap_object  */
            "H5VL_provenance_unwrap_object",            /* unwrap_object  */
            "H5VL_provenance_free_wrap_ctx",            /* free_wrap_ctx */
            "H5VL_provenance_attr_create",                       /* create */
            "H5VL_provenance_attr_open",                         /* open */
            "H5VL_provenance_attr_read",                         /* read */
            "H5VL_provenance_attr_write",                        /* write */
            "H5VL_provenance_attr_get",                          /* get */
            "H5VL_provenance_attr_specific",                     /* specific */
            "H5VL_provenance_attr_optional",                     /* optional */
            "H5VL_provenance_attr_close",                         /* close */
            "H5VL_provenance_dataset_create",                    /* create */
            "H5VL_provenance_dataset_open",                      /* open */
            "H5VL_provenance_dataset_read",                      /* read */
            "H5VL_provenance_dataset_write",                     /* write */
            "H5VL_provenance_dataset_get",                       /* get */
            "H5VL_provenance_dataset_specific",                  /* specific */
            "H5VL_provenance_dataset_optional",                  /* optional */
            "H5VL_provenance_dataset_close",                      /* close */
            "H5VL_provenance_datatype_commit",                   /* commit */
            "H5VL_provenance_datatype_open",                     /* open */
            "H5VL_provenance_datatype_get",                      /* get_size */
            "H5VL_provenance_datatype_specific",                 /* specific */
            "H5VL_provenance_datatype_optional",                 /* optional */
            "H5VL_provenance_datatype_close",                     /* close */
            "H5VL_provenance_file_create",                       /* create */
            "H5VL_provenance_file_open",                         /* open */
            "H5VL_provenance_file_get",                          /* get */
            "H5VL_provenance_file_specific",                     /* specific */
            "H5VL_provenance_file_optional",                     /* optional */
            "H5VL_provenance_file_close",                         /* close */
            "H5VL_provenance_group_create",                      /* create */
            "H5VL_provenance_group_open",                        /* open */
            "H5VL_provenance_group_get",                         /* get */
            "H5VL_provenance_group_specific",                    /* specific */
            "H5VL_provenance_group_optional",                    /* optional */
            "H5VL_provenance_group_close",                        /* close */
            "H5VL_provenance_link_create",                       /* create */
            "H5VL_provenance_link_copy",                         /* copy */
            "H5VL_provenance_link_move",                         /* move */
            "H5VL_provenance_link_get",                          /* get */
            "H5VL_provenance_link_specific",                     /* specific */
            "H5VL_provenance_link_optional",                     /* optional */
            "H5VL_provenance_object_open",                       /* open */
            "H5VL_provenance_object_copy",                       /* copy */
            "H5VL_provenance_object_get",                        /* get */
            "H5VL_provenance_object_specific",                   /* specific */
            "H5VL_provenance_object_optional",                   /* optional */
            "H5VL_provenance_request_wait",                      /* wait */
            "H5VL_provenance_request_notify",
            "H5VL_provenance_request_cancel",
            "H5VL_provenance_request_specific",
            "H5VL_provenance_request_optional",
            "H5VL_provenance_request_free",
    };
    int size = sizeof(preset_dic) / sizeof(const char*);
    int key_space[1000];

    for(int i = 0; i < 1000; i++){
        key_space[i] = -1;
    }

    for(int i = 0; i < size; i++){
        printf("%d %s\n", genHash(preset_dic[i]), preset_dic[i]);
        if(key_space[genHash(preset_dic[i])] == -1){
            key_space[genHash(preset_dic[i])] = (int)genHash(preset_dic[i]);
        }else
            printf("Collision found: key = %d, hash index = %d\n", key_space[genHash(preset_dic[i])], genHash(preset_dic[i]));
    }
}

int prov_write(prov_helper_t* helper_in, const char* msg, unsigned long duration){
//    assert(strcmp(msg, "root_file_info"));
    unsigned long start = get_time_usec();
    const char* base = "H5VL_provenance_";
    size_t base_len;
    size_t msg_len;
    char time[64];
    char pline[512];

    assert(helper_in);

    get_time_str(time);

    /* Trimming long VOL function names */
    base_len = strlen(base);
    msg_len = strlen(msg);
    if(msg_len > base_len) {//strlen(H5VL_provenance_) == 16.
        size_t i = 0;

        for(; i < base_len; i++)
            if(base[i] != msg[i])
                break;
    }

    sprintf(pline, "%s %lu\n",  msg, duration);//assume less than 64 functions
    //printf("Func name:[%s], hash index = [%u], overhead = [%lu]\n",  msg, genHash(msg), duration);
    switch(helper_in->prov_level){
        case File_only:
            fputs(pline, helper_in->prov_file_handle);
            break;

        case File_and_print:
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

/*-------------------------------------------------------------------------
 * Function:    H5VL__provenance_new_obj
 *
 * Purpose:     Create a new PROVENANCE object for an underlying object
 *
 * Return:      Success:    Pointer to the new PROVENANCE object
 *              Failure:    NULL
 *
 * Programmer:  Quincey Koziol
 *              Monday, December 3, 2018
 *
 *-------------------------------------------------------------------------
 */
static H5VL_provenance_t *
H5VL_provenance_new_obj(void *under_obj, hid_t under_vol_id, prov_helper_t* helper)
{
//    unsigned long start = get_time_usec();
    H5VL_provenance_t *new_obj;

    assert(under_vol_id);
    assert(helper);

    new_obj = (H5VL_provenance_t *)calloc(1, sizeof(H5VL_provenance_t));
    new_obj->under_object = under_obj;
    new_obj->under_vol_id = under_vol_id;
    new_obj->prov_helper = helper;
    ptr_cnt_increment(new_obj->prov_helper);
    H5Iinc_ref(new_obj->under_vol_id);
    //TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
    return new_obj;
} /* end H5VL__provenance_new_obj() */


/*-------------------------------------------------------------------------
 * Function:    H5VL__provenance_free_obj
 *
 * Purpose:     Release a PROVENANCE object
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 * Programmer:  Quincey Koziol
 *              Monday, December 3, 2018
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_free_obj(H5VL_provenance_t *obj)
{
    //unsigned long start = get_time_usec();
    hid_t err_id;

    assert(obj);

    ptr_cnt_decrement(PROV_HELPER);

    err_id = H5Eget_current_stack();

    H5Idec_ref(obj->under_vol_id);

    H5Eset_current_stack(err_id);

    free(obj);
    //TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
    return 0;
} /* end H5VL__provenance_free_obj() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_register
 *
 * Purpose:     Register the provenance VOL connector and retrieve an ID
 *              for it.
 *
 * Return:      Success:    The ID for the provenance VOL connector
 *              Failure:    -1
 *
 * Programmer:  Quincey Koziol
 *              Wednesday, November 28, 2018
 *
 *-------------------------------------------------------------------------
 */
hid_t
H5VL_provenance_register(void)
{
    unsigned long start = get_time_usec();

    /* Clear the error stack */
    H5Eclear2(H5E_DEFAULT);

    /* Singleton register the provenance VOL connector ID */
    if(H5I_VOL != H5Iget_type(prov_connector_id_global))
        prov_connector_id_global = H5VLregister_connector(&H5VL_provenance_cls, H5P_DEFAULT);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
    return prov_connector_id_global;
} /* end H5VL_provenance_register() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_init
 *
 * Purpose:     Initialize this VOL connector, performing any necessary
 *              operations for the connector that will apply to all containers
 *              accessed with the connector.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_init(hid_t vipl_id)
{

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL INIT\n");
#endif
    TOTAL_PROV_OVERHEAD = 0;
    TOTAL_NATIVE_H5_TIME = 0;
    PROV_WRITE_TOTAL_TIME = 0;
    FILE_LL_TOTAL_TIME = 0;
    DS_LL_TOTAL_TIME = 0;
    GRP_LL_TOTAL_TIME = 0;
    DT_LL_TOTAL_TIME = 0;
    ATTR_LL_TOTAL_TIME = 0;

    /* Shut compiler up about unused parameter */
    (void)vipl_id;

    /* PROV-IO instrument point */
    provio_init(&config, &fields);

    return 0;
} /* end H5VL_provenance_init() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_term
 *
 * Purpose:     Terminate this VOL connector, performing any necessary
 *              operations for the connector that release connector-wide
 *              resources (usually created / initialized with the 'init'
 *              callback).
 *
 * Return:      Success:    0
 *              Failure:    (Can't fail)
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_term(void)
{

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL TERM\n");
#endif
    // Release resources, etc.
    prov_helper_teardown(PROV_HELPER);
    PROV_HELPER = NULL;

    /* Reset VOL ID */
    prov_connector_id_global = H5I_INVALID_HID;

    /* PROV-IO instrument point */
    provio_term(&config, &fields);

    return 0;
} /* end H5VL_provenance_term() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_info_copy
 *
 * Purpose:     Duplicate the connector's info object.
 *
 * Returns:     Success:    New connector info object
 *              Failure:    NULL
 *
 *---------------------------------------------------------------------------
 */
static void *
H5VL_provenance_info_copy(const void *_info)
{
    unsigned long start = get_time_usec();

    const H5VL_provenance_info_t *info = (const H5VL_provenance_info_t *)_info;
    H5VL_provenance_info_t *new_info;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL INFO Copy\n");
#endif

    /* Allocate new VOL info struct for the PROVENANCE connector */
    new_info = (H5VL_provenance_info_t *)calloc(1, sizeof(H5VL_provenance_info_t));

    /* Increment reference count on underlying VOL ID, and copy the VOL info */
    new_info->under_vol_id = info->under_vol_id;
    H5Iinc_ref(new_info->under_vol_id);
    if(info->under_vol_info)
        H5VLcopy_connector_info(new_info->under_vol_id, &(new_info->under_vol_info), info->under_vol_info);

    if(info->prov_file_path)
        new_info->prov_file_path = strdup(info->prov_file_path);
    if(info->prov_line_format)
        new_info->prov_line_format = strdup(info->prov_line_format);
    new_info->prov_level = info->prov_level;

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
    return new_info;
} /* end H5VL_provenance_info_copy() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_info_cmp
 *
 * Purpose:     Compare two of the connector's info objects, setting *cmp_value,
 *              following the same rules as strcmp().
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_info_cmp(int *cmp_value, const void *_info1, const void *_info2)
{
    unsigned long start = get_time_usec();

    const H5VL_provenance_info_t *info1 = (const H5VL_provenance_info_t *)_info1;
    const H5VL_provenance_info_t *info2 = (const H5VL_provenance_info_t *)_info2;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL INFO Compare\n");
#endif

    /* Sanity checks */
    assert(info1);
    assert(info2);

    /* Initialize comparison value */
    *cmp_value = 0;

    /* Compare under VOL connector classes */
    H5VLcmp_connector_cls(cmp_value, info1->under_vol_id, info2->under_vol_id);
    if(*cmp_value != 0){
        TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
        return 0;
    }

    /* Compare under VOL connector info objects */
    H5VLcmp_connector_info(cmp_value, info1->under_vol_id, info1->under_vol_info, info2->under_vol_info);
    if(*cmp_value != 0){
        TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
        return 0;
    }

    *cmp_value = strcmp(info1->prov_file_path, info2->prov_file_path);
    if(*cmp_value != 0){
        TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
        return 0;
    }

    *cmp_value = strcmp(info1->prov_line_format, info2->prov_line_format);
    if(*cmp_value != 0){
        TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
        return 0;
    }

    *cmp_value = (int)info1->prov_level - (int)info2->prov_level;
    if(*cmp_value != 0){
        TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
        return 0;
    }

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start);

    return 0;
} /* end H5VL_provenance_info_cmp() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_info_free
 *
 * Purpose:     Release an info object for the connector.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_info_free(void *_info)
{
    unsigned long start = get_time_usec();

    H5VL_provenance_info_t *info = (H5VL_provenance_info_t *)_info;
    hid_t err_id;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL INFO Free\n");
#endif

    /* Release underlying VOL ID and info */
    if(info->under_vol_info)
        H5VLfree_connector_info(info->under_vol_id, info->under_vol_info);

    err_id = H5Eget_current_stack();

    H5Idec_ref(info->under_vol_id);

    H5Eset_current_stack(err_id);

    /* Free PROVENANCE info object itself */
    free(info->prov_file_path);
    free(info->prov_line_format);
    free(info);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
    return 0;
} /* end H5VL_provenance_info_free() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_info_to_str
 *
 * Purpose:     Serialize an info object for this connector into a string
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_info_to_str(const void *_info, char **str)
{
    const H5VL_provenance_info_t *info = (const H5VL_provenance_info_t *)_info;
    H5VL_class_value_t under_value = (H5VL_class_value_t)-1;
    char *under_vol_string = NULL;
    size_t under_vol_str_len = 0;
    size_t path_len = 0;
    size_t format_len = 0;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL INFO To String\n");
#endif

    /* Get value and string for underlying VOL connector */
    H5VLget_value(info->under_vol_id, &under_value);
    H5VLconnector_info_to_str(info->under_vol_info, info->under_vol_id, &under_vol_string);

    /* Determine length of underlying VOL info string */
    if(under_vol_string)
        under_vol_str_len = strlen(under_vol_string);

    if(info->prov_file_path)
        path_len = strlen(info->prov_file_path);

    if(info->prov_line_format)
        format_len = strlen(info->prov_line_format);

    /* Allocate space for our info */
    *str = (char *)H5allocate_memory(64 + under_vol_str_len + path_len + format_len, (hbool_t)0);
    assert(*str);

    /* Encode our info
     * Normally we'd use snprintf() here for a little extra safety, but that
     * call had problems on Windows until recently. So, to be as platform-independent
     * as we can, we're using sprintf() instead.
     */
    sprintf(*str, "under_vol=%u;under_info={%s};path=%s;level=%d;format=%s",
            (unsigned)under_value, (under_vol_string ? under_vol_string : ""), info->prov_file_path, info->prov_level, info->prov_line_format);

    return 0;
} /* end H5VL_provenance_info_to_str() */

herr_t provenance_file_setup(const char* str_in, char* file_path_out, Prov_level* level_out, char* format_out){
    //acceptable format: path=$path_str;level=$level_int;format=$format_str
    char tmp_str[100] = {'\0'};
    char* toklist[4] = {NULL};
    int i;
    char *p;

    memcpy(tmp_str, str_in, strlen(str_in)+1);

    i = 0;
    p = strtok(tmp_str, ";");
    while(p != NULL) {
        toklist[i] = strdup(p);
        p = strtok(NULL, ";");
        i++;
    }

    sscanf(toklist[1], "path=%s", file_path_out);
    sscanf(toklist[2], "level=%d", (int *)level_out);
    sscanf(toklist[3], "format=%s", format_out);

    for(i = 0; i<=3; i++)
        if(toklist[i])
            free(toklist[i]);

    return 0;
}


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_str_to_info
 *
 * Purpose:     Deserialize a string into an info object for this connector.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_str_to_info(const char *str, void **_info)
{
    H5VL_provenance_info_t *info;
    unsigned under_vol_value;
    const char *under_vol_info_start, *under_vol_info_end;
    hid_t under_vol_id;
    void *under_vol_info = NULL;
    char *under_vol_info_str = NULL;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL INFO String To Info\n");
#endif

    /* Retrieve the underlying VOL connector value and info */
    sscanf(str, "under_vol=%u;", &under_vol_value);
    under_vol_id = H5VLregister_connector_by_value((H5VL_class_value_t)under_vol_value, H5P_DEFAULT);
    under_vol_info_start = strchr(str, '{');
    under_vol_info_end = strrchr(str, '}');
    assert(under_vol_info_end > under_vol_info_start);

    if(under_vol_info_end != (under_vol_info_start + 1)) {
        under_vol_info_str = (char *)malloc((size_t)(under_vol_info_end - under_vol_info_start));
        memcpy(under_vol_info_str, under_vol_info_start + 1, (size_t)((under_vol_info_end - under_vol_info_start) - 1));
        *(under_vol_info_str + (under_vol_info_end - under_vol_info_start)) = '\0';

        H5VLconnector_str_to_info(under_vol_info_str, under_vol_id, &under_vol_info);//generate under_vol_info obj.

    } /* end else */

    /* Allocate new provenance VOL connector info and set its fields */
    info = (H5VL_provenance_info_t *)calloc(1, sizeof(H5VL_provenance_info_t));
    info->under_vol_id = under_vol_id;
    info->under_vol_info = under_vol_info;

    info->prov_file_path = (char *)calloc(64, sizeof(char));
    info->prov_line_format = (char *)calloc(64, sizeof(char));

    if(provenance_file_setup(under_vol_info_end, info->prov_file_path, &(info->prov_level), info->prov_line_format) != 0){
        free(info->prov_file_path);
        free(info->prov_line_format);
        info->prov_line_format = NULL;
        info->prov_file_path = NULL;
        info->prov_level = File_only;
    }

    /* Set return value */
    *_info = info;

    if(under_vol_info_str)
        free(under_vol_info_str);

    return 0;
} /* end H5VL_provenance_str_to_info() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_get_object
 *
 * Purpose:     Retrieve the 'data' for a VOL object.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
static void *
H5VL_provenance_get_object(const void *obj)
{
    const H5VL_provenance_t *o = (const H5VL_provenance_t *)obj;
    void* ret;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL Get object\n");
#endif

    ret = H5VLget_object(o->under_object, o->under_vol_id);

    return ret;

} /* end H5VL_provenance_get_object() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_get_wrap_ctx
 *
 * Purpose:     Retrieve a "wrapper context" for an object
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_get_wrap_ctx(const void *obj, void **wrap_ctx)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    const H5VL_provenance_t *o = (const H5VL_provenance_t *)obj;
    H5VL_provenance_wrap_ctx_t *new_wrap_ctx;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL WRAP CTX Get\n");
#endif

    assert(o->my_type != 0);

    /* Allocate new VOL object wrapping context for the PROVENANCE connector */
    new_wrap_ctx = (H5VL_provenance_wrap_ctx_t *)calloc(1, sizeof(H5VL_provenance_wrap_ctx_t));
    switch(o->my_type){
        case H5I_DATASET:
        case H5I_GROUP:
        case H5I_DATATYPE:
        case H5I_ATTR:
            new_wrap_ctx->file_info = ((object_prov_info_t *)(o->generic_prov_info))->file_info;
            break;

        case H5I_FILE:
            new_wrap_ctx->file_info = (file_prov_info_t*)(o->generic_prov_info);
            break;

        case H5I_UNINIT:
        case H5I_BADID:
        case H5I_DATASPACE:
        case H5I_VFL:
        case H5I_VOL:
        case H5I_GENPROP_CLS:
        case H5I_GENPROP_LST:
        case H5I_ERROR_CLASS:
        case H5I_ERROR_MSG:
        case H5I_ERROR_STACK:
        case H5I_NTYPES:
        default:
            printf("%s:%d: unexpected type: my_type = %d\n", __func__, __LINE__, (int)o->my_type);
            break;
    }

    // Increment reference count on file info, so it doesn't get freed while
    // we're wrapping objects with it.
    new_wrap_ctx->file_info->ref_cnt++;

    /* Increment reference count on underlying VOL ID, and copy the VOL info */
    m1 = get_time_usec();
    new_wrap_ctx->under_vol_id = o->under_vol_id;
    H5Iinc_ref(new_wrap_ctx->under_vol_id);
    H5VLget_wrap_ctx(o->under_object, o->under_vol_id, &new_wrap_ctx->under_wrap_ctx);
    m2 = get_time_usec();

    /* Set wrap context to return */
    *wrap_ctx = new_wrap_ctx;

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return 0;
} /* end H5VL_provenance_get_wrap_ctx() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_wrap_object
 *
 * Purpose:     Use a "wrapper context" to wrap a data object
 *
 * Return:      Success:    Pointer to wrapped object
 *              Failure:    NULL
 *
 *---------------------------------------------------------------------------
 */
static void *
H5VL_provenance_wrap_object(void *under_under_in, H5I_type_t obj_type, void *_wrap_ctx_in)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    /* Generic object wrapping, make ctx based on types */
    H5VL_provenance_wrap_ctx_t *wrap_ctx = (H5VL_provenance_wrap_ctx_t *)_wrap_ctx_in;
    void *under;
    H5VL_provenance_t* new_obj;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL WRAP Object\n");
#endif

    /* Wrap the object with the underlying VOL */
    m1 = get_time_usec();
    under = H5VLwrap_object(under_under_in, obj_type, wrap_ctx->under_vol_id, wrap_ctx->under_wrap_ctx);

    m2 = get_time_usec();

    if(under) {
        H5VL_provenance_t* fake_upper_o;

        fake_upper_o = _fake_obj_new(wrap_ctx->file_info, wrap_ctx->under_vol_id);

        new_obj = _obj_wrap_under(under, fake_upper_o, NULL, obj_type, H5P_DEFAULT, NULL);

        _fake_obj_free(fake_upper_o);
    }
    else
        new_obj = NULL;

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return (void*)new_obj;
} /* end H5VL_provenance_wrap_object() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_unwrap_object
 *
 * Purpose:     Unwrap a wrapped data object
 *
 * Return:      Success:    Pointer to unwrapped object
 *              Failure:    NULL
 *
 *---------------------------------------------------------------------------
 */
static void *
H5VL_provenance_unwrap_object(void *obj)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    /* Generic object unwrapping, make ctx based on types */
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL UNWRAP Object\n");
#endif

    /* Unwrap the object with the underlying VOL */
    m1 = get_time_usec();
    under = H5VLunwrap_object(o->under_object, o->under_vol_id);
    m2 = get_time_usec();

    if(under) {
        // Free the class-specific info
        switch(o->my_type) {
            case H5I_DATASET:
                rm_dataset_node(o->prov_helper, o->under_object, o->under_vol_id, (dataset_prov_info_t *)(o->generic_prov_info));
                break;

            case H5I_GROUP:
                rm_grp_node(o->prov_helper, o->under_object, o->under_vol_id, (group_prov_info_t *)(o->generic_prov_info));
                break;

            case H5I_DATATYPE:
                rm_dtype_node(o->prov_helper, o->under_object, o->under_vol_id, (datatype_prov_info_t *)(o->generic_prov_info));
                break;

            case H5I_ATTR:
                rm_attr_node(o->prov_helper, o->under_object, o->under_vol_id, (attribute_prov_info_t *)(o->generic_prov_info));
                break;

            case H5I_FILE:
                rm_file_node(o->prov_helper, ((file_prov_info_t *)o->generic_prov_info)->file_no);
                break;

            case H5I_UNINIT:
            case H5I_BADID:
            case H5I_DATASPACE:
            case H5I_VFL:
            case H5I_VOL:
            case H5I_GENPROP_CLS:
            case H5I_GENPROP_LST:
            case H5I_ERROR_CLASS:
            case H5I_ERROR_MSG:
            case H5I_ERROR_STACK:
            case H5I_NTYPES:
            default:
                break;
        }

        // Free the wrapper object
        H5VL_provenance_free_obj(o);
    }

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return under;
} /* end H5VL_provenance_unwrap_object() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_free_wrap_ctx
 *
 * Purpose:     Release a "wrapper context" for an object
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_free_wrap_ctx(void *_wrap_ctx)
{
    unsigned long start = get_time_usec();

    H5VL_provenance_wrap_ctx_t *wrap_ctx = (H5VL_provenance_wrap_ctx_t *)_wrap_ctx;
    hid_t err_id;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL WRAP CTX Free\n");
#endif

    err_id = H5Eget_current_stack();

    // Release hold on underlying file_info
    rm_file_node(PROV_HELPER, wrap_ctx->file_info->file_no);

    /* Release underlying VOL ID and wrap context */
    if(wrap_ctx->under_wrap_ctx)
        H5VLfree_wrap_ctx(wrap_ctx->under_wrap_ctx, wrap_ctx->under_vol_id);
    H5Idec_ref(wrap_ctx->under_vol_id);

    H5Eset_current_stack(err_id);

    /* Free PROVENANCE wrap context object itself */
    free(wrap_ctx);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start);
    return 0;
} /* end H5VL_provenance_free_wrap_ctx() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_attr_create
 *
 * Purpose:     Creates an attribute on an object.
 *
 * Return:      Success:    Pointer to attribute object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_attr_create(void *obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t type_id, hid_t space_id, hid_t acpl_id,
    hid_t aapl_id, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *attr;
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL ATTRIBUTE Create\n");
#endif

    m1 = get_time_usec();
    under = H5VLattr_create(o->under_object, loc_params, o->under_vol_id, name, type_id, space_id, acpl_id, aapl_id, dxpl_id, req);
    m2 = get_time_usec();

    if(under)
        attr = _obj_wrap_under(under, o, name, H5I_ATTR, dxpl_id, req);
    else
        attr = NULL;

    if(o) 
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    /* PROV-IO instrument start */
    const char* io_api = "H5Acreate2";
    const char* io_api_async = "H5Acreate_async";
    const char* relation = "prov:wasGeneratedBy";
    const char* type = "provio:Attr";
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */   

    return (void*)attr;
} /* end H5VL_provenance_attr_create() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_attr_open
 *
 * Purpose:     Opens an attribute on an object.
 *
 * Return:      Success:    Pointer to attribute object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_attr_open(void *obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t aapl_id, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *attr;
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL ATTRIBUTE Open\n");
#endif

    m1 = get_time_usec();
    under = H5VLattr_open(o->under_object, loc_params, o->under_vol_id, name, aapl_id, dxpl_id, req);
    m2 = get_time_usec();

    if(under) {
        char *attr_name = NULL;

        if(NULL == name) {
            ssize_t size_ret = 0;

            size_ret = attr_get_name(under, o->under_vol_id, dxpl_id, 0, NULL);
            if(size_ret > 0) {
                size_t buf_len = (size_t)(size_ret + 1);

                attr_name = (char *)malloc(buf_len);
                size_ret = attr_get_name(under, o->under_vol_id, dxpl_id, buf_len, attr_name);
                if(size_ret >= 0)
                    name = attr_name;
            }
        }

        attr = _obj_wrap_under(under, o, name, H5I_ATTR, dxpl_id, req);

        if(attr_name)
            free(attr_name);
    }
    else
        attr = NULL;

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    /* PROV-IO instrument start */
    const char* io_api = "H5Aopen";
    const char* io_api_async = "H5Aopen_async";
    const char* relation = "provio:wasOpenedBy";
    const char* type = "provio:Attr";
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */  

    return (void *)attr;
} /* end H5VL_provenance_attr_open() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_attr_read
 *
 * Purpose:     Reads data from attribute.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_attr_read(void *attr, hid_t mem_type_id, void *buf,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)attr;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL ATTRIBUTE Read\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLattr_read(o->under_object, o->under_vol_id, mem_type_id, buf, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    /* PROV-IO instrument start */
    char* name = NULL;
    char *attr_name = NULL;
    ssize_t size_ret = 0;
    const char* io_api = "H5Aread";
    const char* io_api_async = "H5Aread_async";
    const char* relation = "provio:wasReadBy";
    const char* type = "provio:Attr";
    size_ret = attr_get_name(o->under_object, o->under_vol_id, dxpl_id, 0, NULL);
    if(size_ret > 0) {
        size_t buf_len = (size_t)(size_ret + 1);

        attr_name = (char *)malloc(buf_len);
        size_ret = attr_get_name(o->under_object, o->under_vol_id, dxpl_id, buf_len, attr_name);
        if(size_ret >= 0)
            name = attr_name;
    }
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */  

    return ret_value;
} /* end H5VL_provenance_attr_read() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_attr_write
 *
 * Purpose:     Writes data to attribute.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_attr_write(void *attr, hid_t mem_type_id, const void *buf,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)attr;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL ATTRIBUTE Write\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLattr_write(o->under_object, o->under_vol_id, mem_type_id, buf, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    /* PROV-IO instrument start */
    char* name = NULL;
    char *attr_name = NULL;
    ssize_t size_ret = 0;
    const char* io_api = "H5Awrite";
    const char* io_api_async = "H5Awrite_async";
    const char* relation = "provio:wasWrittenBy";
    const char* type = "provio:Attr";
    size_ret = attr_get_name(o->under_object, o->under_vol_id, dxpl_id, 0, NULL);
    if(size_ret > 0) {
        size_t buf_len = (size_t)(size_ret + 1);

        attr_name = (char *)malloc(buf_len);
        size_ret = attr_get_name(o->under_object, o->under_vol_id, dxpl_id, buf_len, attr_name);
        if(size_ret >= 0)
            name = attr_name;
    }
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */  

    return ret_value;
} /* end H5VL_provenance_attr_write() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_attr_get
 *
 * Purpose:     Gets information about an attribute
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_attr_get(void *obj, H5VL_attr_get_args_t *args, hid_t dxpl_id,
    void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL ATTRIBUTE Get\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLattr_get(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_attr_get() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_attr_specific
 *
 * Purpose:     Specific operation on attribute
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_attr_specific(void *obj, const H5VL_loc_params_t *loc_params,
    H5VL_attr_specific_args_t *args, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL ATTRIBUTE Specific\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLattr_specific(o->under_object, loc_params, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_attr_specific() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_attr_optional
 *
 * Purpose:     Perform a connector-specific operation on an attribute
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_attr_optional(void *obj, H5VL_optional_args_t *args, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL ATTRIBUTE Optional\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLattr_optional(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_attr_optional() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_attr_close
 *
 * Purpose:     Closes an attribute.
 *
 * Return:      Success:    0
 *              Failure:    -1, attr not closed.
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_attr_close(void *attr, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)attr;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL ATTRIBUTE Close\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLattr_close(o->under_object, o->under_vol_id, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    /* Release our wrapper, if underlying attribute was closed */
    if(ret_value >= 0) {
        attribute_prov_info_t *attr_info;

        attr_info = (attribute_prov_info_t *)o->generic_prov_info;

        // prov_write(o->prov_helper, __func__, get_time_usec() - start);
        attribute_stats_prov_write(attr_info);

        rm_attr_node(o->prov_helper, o->under_object, o->under_vol_id, attr_info);
        H5VL_provenance_free_obj(o);
    }

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_attr_close() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_dataset_create
 *
 * Purpose:     Creates a dataset in a container
 *
 * Return:      Success:    Pointer to a dataset object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_dataset_create(void *obj, const H5VL_loc_params_t *loc_params,
    const char *ds_name, hid_t lcpl_id, hid_t type_id, hid_t space_id,
    hid_t dcpl_id, hid_t dapl_id, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *dset;
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATASET Create\n");
#endif

    m1 = get_time_usec();
    under = H5VLdataset_create(o->under_object, loc_params, o->under_vol_id, ds_name, lcpl_id, type_id, space_id, dcpl_id,  dapl_id, dxpl_id, req);
    m2 = get_time_usec();

    if(under)
        dset = _obj_wrap_under(under, o, ds_name, H5I_DATASET, dxpl_id, req);
    else
        dset = NULL;

    if(o) 
        ;
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    /* PROV-IO instrument start */
    const char* io_api = "H5Dcreate2";
    const char* io_api_async = "H5Dcreate_async";
    const char* relation = "prov:wasGeneratedBy";
    const char* type = "provio:Dataset"; 
    prov_fill_data_object(&fields, ds_name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return (void *)dset;
} /* end H5VL_provenance_dataset_create() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_dataset_open
 *
 * Purpose:     Opens a dataset in a container
 *
 * Return:      Success:    Pointer to a dataset object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_dataset_open(void *obj, const H5VL_loc_params_t *loc_params,
    const char *ds_name, hid_t dapl_id, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    void *under;
    H5VL_provenance_t *dset;
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;



#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATASET Open\n");
#endif

    m1 = get_time_usec();
    under = H5VLdataset_open(o->under_object, loc_params, o->under_vol_id, ds_name, dapl_id, dxpl_id, req);
    m2 = get_time_usec();

    if(under)
        dset = _obj_wrap_under(under, o, ds_name, H5I_DATASET, dxpl_id, req);
    else
        dset = NULL;

    if(dset) 
        ;
        // prov_write(dset->prov_helper, __func__, get_time_usec() - start);

    /* PROV-IO instrument start */
    const char* io_api = "H5Dopen2";
    const char* io_api_async = "H5Dopen_async";
    const char* relation = "provio:wasOpenedBy";
    const char* type = "provio:Dataset"; 
    prov_fill_data_object(&fields, ds_name, type);
    prov_fill_relation(&fields, relation);
    char name[64];
    object_get_name(o->under_object, o->under_vol_id, loc_params, H5P_DATASET_XFER_DEFAULT, 64, name);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return (void *)dset;
} /* end H5VL_provenance_dataset_open() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_dataset_read
 *
 * Purpose:     Reads data elements from a dataset into a buffer.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_dataset_read(void *dset, hid_t mem_type_id, hid_t mem_space_id,
    hid_t file_space_id, hid_t plist_id, void *buf, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)dset;
#ifdef H5_HAVE_PARALLEL
    H5FD_mpio_xfer_t xfer_mode = H5FD_MPIO_INDEPENDENT;
#endif /* H5_HAVE_PARALLEL */
    herr_t ret_value;

    /* PROV-IO instrument start */
    const char* io_api = "H5Dread";
    const char* io_api_async = "H5Dread_async";
    const char* relation = "provio:wasReadBy";
    const char* type = "provio:Dataset"; 
    char name[64];
    H5VL_loc_params_t loc_params; 
    loc_params.type     = H5VL_OBJECT_BY_SELF;
    loc_params.obj_type = H5I_DATASET;
    object_get_name(o->under_object, o->under_vol_id, &loc_params, H5P_DATASET_XFER_DEFAULT, 64, name);
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    /* PROV-IO instrument end */

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATASET Read\n");
#endif

#ifdef H5_HAVE_PARALLEL
    // Retrieve MPI-IO transfer option
    H5Pget_dxpl_mpio(plist_id, &xfer_mode);
#endif /* H5_HAVE_PARALLEL */

    m1 = get_time_usec();
    ret_value = H5VLdataset_read(o->under_object, o->under_vol_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(ret_value >= 0) {
        dataset_prov_info_t * dset_info = (dataset_prov_info_t*)o->generic_prov_info;
        hsize_t r_size;

#ifdef H5_HAVE_PARALLEL
        // Increment appropriate parallel I/O counters
        if(xfer_mode == H5FD_MPIO_INDEPENDENT)
            // Increment counter for independent reads
            dset_info->ind_dataset_read_cnt++;
        else {
            H5D_mpio_actual_io_mode_t actual_io_mode;

            // Increment counter for collective reads
            dset_info->coll_dataset_read_cnt++;

            // Check for actually completing a collective I/O
            H5Pget_mpio_actual_io_mode(plist_id, &actual_io_mode);
            if(!actual_io_mode)
                dset_info->broken_coll_dataset_read_cnt++;
        } /* end else */
#endif /* H5_HAVE_PARALLEL */

        if(H5S_ALL == mem_space_id)
            r_size = dset_info->dset_type_size * dset_info->dset_space_size;
        else
            r_size = dset_info->dset_type_size * (hsize_t)H5Sget_select_npoints(mem_space_id);

        dset_info->total_bytes_read += r_size;
        dset_info->dataset_read_cnt++;
        dset_info->total_read_time += (m2 - m1);
    }

    // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    /* PROV-IO instrument start */
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_dataset_read() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_dataset_write
 *
 * Purpose:     Writes data elements from a buffer into a dataset.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_dataset_write(void *dset, hid_t mem_type_id, hid_t mem_space_id,
    hid_t file_space_id, hid_t plist_id, const void *buf, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;
//H5VL_provenance_t: A envelop
    H5VL_provenance_t *o = (H5VL_provenance_t *)dset;
#ifdef H5_HAVE_PARALLEL
    H5FD_mpio_xfer_t xfer_mode = H5FD_MPIO_INDEPENDENT;
#endif /* H5_HAVE_PARALLEL */
    herr_t ret_value;

    assert(dset);

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATASET Write\n");
#endif

#ifdef H5_HAVE_PARALLEL
    // Retrieve MPI-IO transfer option
    H5Pget_dxpl_mpio(plist_id, &xfer_mode);
#endif /* H5_HAVE_PARALLEL */

//H5VLdataset_write: framework
// VOL B do IO, so A ask B to write.    o->under_object is a B envelop.
    // reuse A envelop
    m1 = get_time_usec();
    ret_value = H5VLdataset_write(o->under_object, o->under_vol_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(ret_value >= 0) {
        dataset_prov_info_t * dset_info = (dataset_prov_info_t*)o->generic_prov_info;
        hsize_t w_size;

#ifdef H5_HAVE_PARALLEL
        // Increment appropriate parallel I/O counters
        if(xfer_mode == H5FD_MPIO_INDEPENDENT)
            // Increment counter for independent writes
            dset_info->ind_dataset_write_cnt++;
        else {
            H5D_mpio_actual_io_mode_t actual_io_mode;

            // Increment counter for collective writes
            dset_info->coll_dataset_write_cnt++;

            // Check for actually completing a collective I/O
            H5Pget_mpio_actual_io_mode(plist_id, &actual_io_mode);
            if(!actual_io_mode)
                dset_info->broken_coll_dataset_write_cnt++;
        } /* end else */
#endif /* H5_HAVE_PARALLEL */

        if(H5S_ALL == mem_space_id)
            w_size = dset_info->dset_type_size * dset_info->dset_space_size;
        else
            w_size = dset_info->dset_type_size * (hsize_t)H5Sget_select_npoints(mem_space_id);

        dset_info->total_bytes_written += w_size;
        dset_info->dataset_write_cnt++;
        dset_info->total_write_time += (m2 - m1);
    }

    // prov_write(o->prov_helper, __func__, get_time_usec() - start);
    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    /* PROV-IO instrument start */
    const char* io_api = "H5Dwrite";
    const char* io_api_async = "H5Dwrite_async";
    const char* relation = "provio:wasWrittenBy";
    const char* type = "provio:Dataset";  
    char name[64];
    H5VL_loc_params_t loc_params; 
    loc_params.type     = H5VL_OBJECT_BY_SELF;
    loc_params.obj_type = H5I_DATASET;
    object_get_name(o->under_object, o->under_vol_id, &loc_params, H5P_DATASET_XFER_DEFAULT, 64, name);
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */

    return ret_value;
} /* end H5VL_provenance_dataset_write() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_dataset_get
 *
 * Purpose:     Gets information about a dataset
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_dataset_get(void *dset, H5VL_dataset_get_args_t *args,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)dset;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATASET Get\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLdataset_get(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_dataset_get() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_dataset_specific
 *
 * Purpose:     Specific operation on a dataset
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *------------------------------------`-------------------------------------
 */
static herr_t
H5VL_provenance_dataset_specific(void *obj, H5VL_dataset_specific_args_t *args,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under_obj = NULL;
    hid_t under_vol_id = -1;
    prov_helper_t *helper = NULL;
    dataset_prov_info_t *my_dataset_info;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL H5Dspecific\n");
#endif

    // Sanity check
    assert(o->my_type == H5I_DATASET);

    // Check if refreshing
    if(args->op_type == H5VL_DATASET_REFRESH) {
        // Save dataset prov info for later, and increment the refcount on it,
        // so that the stats aren't lost when the object is closed and reopened
        // during the underlying refresh operation
        my_dataset_info = (dataset_prov_info_t *)o->generic_prov_info;
        my_dataset_info->obj_info.ref_cnt++;
    }

    // Save copy of underlying VOL connector ID and prov helper, in case of
    // refresh destroying the current object
    under_obj = o->under_object;
    under_vol_id = o->under_vol_id;
    helper = o->prov_helper;

    m1 = get_time_usec();
    ret_value = H5VLdataset_specific(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    // Update dataset dimensions for 'set extent' operations
    if(args->op_type == H5VL_DATASET_SET_EXTENT) {
        if(ret_value >= 0) {
            dataset_prov_info_t *ds_info;

            ds_info = (dataset_prov_info_t *)o->generic_prov_info;
            assert(ds_info);

            // Update dimension sizes, if simple dataspace
            if(H5S_SIMPLE == ds_info->ds_class) {
                const hsize_t *new_size = args->args.set_extent.size;
                unsigned u;

                // Update the dataset's dimensions & element count
                ds_info->dset_space_size = 1;
                for(u = 0; u < ds_info->dimension_cnt; u++) {
                    ds_info->dimensions[u] = new_size[u];
                    ds_info->dset_space_size *= new_size[u];
                }
            }
        }
    }
    // Get new dataset info, after refresh
    else if(args->op_type == H5VL_DATASET_REFRESH) {
        // Sanity check
        assert(my_dataset_info);

        if(ret_value >= 0) {
            hid_t dataset_id;
            hid_t space_id;

            // Sanity check - make certain dataset info wasn't freed
            assert(my_dataset_info->obj_info.ref_cnt > 0);

            // Set object pointers to NULL, to avoid programming errors
            o = NULL;
            obj = NULL;

            // Get dataset ID from arg list
            dataset_id = args->args.refresh.dset_id;

            // Update dataspace dimensions & element count (which could have changed)
            space_id = H5Dget_space(dataset_id);
            H5Sget_simple_extent_dims(space_id, my_dataset_info->dimensions, NULL);
            my_dataset_info->dset_space_size = (hsize_t)H5Sget_simple_extent_npoints(space_id);
            H5Sclose(space_id);

            // Don't close dataset ID, it's owned by the application
        }

        // Decrement refcount on dataset info
        rm_dataset_node(helper, under_obj, under_vol_id, my_dataset_info);
    }

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, under_vol_id, helper);

    // prov_write(helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_dataset_specific() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_dataset_optional
 *
 * Purpose:     Perform a connector-specific operation on a dataset
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_dataset_optional(void *obj, H5VL_optional_args_t *args,
                                 hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATASET Optional\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLdataset_optional(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_dataset_optional() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_dataset_close
 *
 * Purpose:     Closes a dataset.
 *
 * Return:      Success:    0
 *              Failure:    -1, dataset not closed.
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_dataset_close(void *dset, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)dset;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATASET Close\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLdataset_close(o->under_object, o->under_vol_id, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    /* Release our wrapper, if underlying dataset was closed */
    if(ret_value >= 0){
        dataset_prov_info_t* dset_info;

        dset_info = (dataset_prov_info_t*)o->generic_prov_info;
        assert(dset_info);

        dataset_stats_prov_write(dset_info);//output stats
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

        rm_dataset_node(o->prov_helper, o->under_object, o->under_vol_id, dset_info);

        H5VL_provenance_free_obj(o);
    }

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_dataset_close() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_datatype_commit
 *
 * Purpose:     Commits a datatype inside a container.
 *
 * Return:      Success:    Pointer to datatype object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_datatype_commit(void *obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t type_id, hid_t lcpl_id, hid_t tcpl_id, hid_t tapl_id,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *dt;
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATATYPE Commit\n");
#endif

    m1 = get_time_usec();
    under = H5VLdatatype_commit(o->under_object, loc_params, o->under_vol_id, name, type_id, lcpl_id, tcpl_id, tapl_id, dxpl_id, req);
    m2 = get_time_usec();

    if(under)
        dt = _obj_wrap_under(under, o, name, H5I_DATATYPE, dxpl_id, req);
    else
        dt = NULL;

    if(dt)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    /* PROV-IO instrument start */
    const char* io_api = "H5Tcommit2";
    const char* io_api_async = "H5Tcommit_async";
    const char* relation = "prov:wasCommittedBy";
    const char* type = "provio:Datatype";
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */

    return (void *)dt;
} /* end H5VL_provenance_datatype_commit() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_datatype_open
 *
 * Purpose:     Opens a named datatype inside a container.
 *
 * Return:      Success:    Pointer to datatype object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_datatype_open(void *obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t tapl_id, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *dt;
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATATYPE Open\n");
#endif

    m1 = get_time_usec();
    under = H5VLdatatype_open(o->under_object, loc_params, o->under_vol_id, name, tapl_id, dxpl_id, req);
    m2 = get_time_usec();

    if(under)
        dt = _obj_wrap_under(under, o, name, H5I_DATATYPE, dxpl_id, req);
    else
        dt = NULL;

    if(dt)
        // prov_write(dt->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    /* PROV-IO instrument start */
    const char* io_api = "H5Topen2";
    const char* io_api_async = "H5Topen_async";
    const char* relation = "provio:wasOpenedBy";
    const char* type = "provio:Datatype";
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */

    return (void *)dt;
} /* end H5VL_provenance_datatype_open() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_datatype_get
 *
 * Purpose:     Get information about a datatype
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_datatype_get(void *dt, H5VL_datatype_get_args_t *args,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)dt;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATATYPE Get\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLdatatype_get(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_datatype_get() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_datatype_specific
 *
 * Purpose:     Specific operations for datatypes
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_datatype_specific(void *obj, H5VL_datatype_specific_args_t *args,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under_obj = NULL;
    hid_t under_vol_id = -1;
    prov_helper_t *helper = NULL;
    datatype_prov_info_t *my_dtype_info = NULL;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATATYPE Specific\n");
#endif

    // Check if refreshing
    if(args->op_type == H5VL_DATATYPE_REFRESH) {
        // Save datatype prov info for later, and increment the refcount on it,
        // so that the stats aren't lost when the object is closed and reopened
        // during the underlying refresh operation
        my_dtype_info = (datatype_prov_info_t *)o->generic_prov_info;
        my_dtype_info->obj_info.ref_cnt++;
    }

    // Save copy of underlying VOL connector ID and prov helper, in case of
    // refresh destroying the current object
    under_obj = o->under_object;
    under_vol_id = o->under_vol_id;
    helper = o->prov_helper;

    m1 = get_time_usec();
    ret_value = H5VLdatatype_specific(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    if(args->op_type == H5VL_DATATYPE_REFRESH) {
        // Sanity check
        assert(my_dtype_info);

        // Get new datatype info, after refresh
        if(ret_value >= 0) {
            // Sanity check - make certain datatype info wasn't freed
            assert(my_dtype_info->obj_info.ref_cnt > 0);

            // Set object pointers to NULL, to avoid programming errors
            o = NULL;
            obj = NULL;

            // Update datatype info (nothing to update, currently)

            // Don't close datatype ID, it's owned by the application
        }

        // Decrement refcount on datatype info
        rm_dtype_node(helper, o->under_object, under_vol_id, my_dtype_info);
    }

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, under_vol_id, helper);

    // prov_write(helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_datatype_specific() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_datatype_optional
 *
 * Purpose:     Perform a connector-specific operation on a datatype
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_datatype_optional(void *obj, H5VL_optional_args_t *args,
                                  hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATATYPE Optional\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLdatatype_optional(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_datatype_optional() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_datatype_close
 *
 * Purpose:     Closes a datatype.
 *
 * Return:      Success:    0
 *              Failure:    -1, datatype not closed.
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_datatype_close(void *dt, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)dt;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL DATATYPE Close\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLdatatype_close(o->under_object, o->under_vol_id, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    /* Release our wrapper, if underlying datatype was closed */
    if(ret_value >= 0){
        datatype_prov_info_t* info;

        info = (datatype_prov_info_t*)(o->generic_prov_info);

        datatype_stats_prov_write(info);
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

        rm_dtype_node(PROV_HELPER, o->under_object, o->under_vol_id , info);

        H5VL_provenance_free_obj(o);
    }

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_datatype_close() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_file_create
 *
 * Purpose:     Creates a container using this connector
 *
 * Return:      Success:    Pointer to a file object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_file_create(const char *name, unsigned flags, hid_t fcpl_id,
    hid_t fapl_id, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_info_t *info = NULL;
    H5VL_provenance_t *file;
    hid_t under_fapl_id = -1;
    void *under;
#ifdef H5_HAVE_PARALLEL
    hid_t driver_id;            // VFD driver for file
    MPI_Comm mpi_comm = MPI_COMM_NULL;  // MPI Comm from FAPL
    MPI_Info mpi_info = MPI_INFO_NULL;  // MPI Info from FAPL
    hbool_t have_mpi_comm_info = false;     // Whether the MPI Comm & Info are retrieved
#endif /* H5_HAVE_PARALLEL */

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL FILE Create\n");
#endif

    /* Get copy of our VOL info from FAPL */
    H5Pget_vol_info(fapl_id, (void **)&info);

    /* Copy the FAPL */
    under_fapl_id = H5Pcopy(fapl_id);

    /* Set the VOL ID and info for the underlying FAPL */
    H5Pset_vol(under_fapl_id, info->under_vol_id, info->under_vol_info);

#ifdef H5_HAVE_PARALLEL
    // Determine if the file is accessed with the parallel VFD (MPI-IO)
    // and copy the MPI comm & info objects for our use
    if((driver_id = H5Pget_driver(under_fapl_id)) > 0 && driver_id == H5FD_MPIO) {
        // Retrieve the MPI comm & info objects
        H5Pget_fapl_mpio(under_fapl_id, &mpi_comm, &mpi_info);

        // Indicate that the Comm & Info are available
        have_mpi_comm_info = true;
    }
#endif /* H5_HAVE_PARALLEL */

    /* Open the file with the underlying VOL connector */
    m1 = get_time_usec();
    under = H5VLfile_create(name, flags, fcpl_id, under_fapl_id, dxpl_id, req);
    m2 = get_time_usec();

    if(under) {
        if(!PROV_HELPER)
            PROV_HELPER = prov_helper_init(info->prov_file_path, info->prov_level, info->prov_line_format);;

        file = _file_open_common(under, info->under_vol_id, name);

#ifdef H5_HAVE_PARALLEL
        if(have_mpi_comm_info) {
            file_prov_info_t *file_info = file->generic_prov_info;

            // Take ownership of MPI Comm & Info
            file_info->mpi_comm = mpi_comm;
            file_info->mpi_info = mpi_info;
            file_info->mpi_comm_info_valid = true;

            // Reset flag, so Comm & Info aren't freed
            have_mpi_comm_info = false;
        }
#endif /* H5_HAVE_PARALLEL */

        /* Check for async request */
        if(req && *req)
            *req = H5VL_provenance_new_obj(*req, info->under_vol_id, PROV_HELPER);
    } /* end if */
    else
        file = NULL;

    if(file)
        ;
        // prov_write(file->prov_helper, __func__, get_time_usec() - start);


    /* PROV-IO instrument start */
    const char* io_api = "H5Topen2";
    const char* io_api_async = "H5Topen_async";
    const char* relation = "provio:wasOpenedBy";
    const char* type = "provio:Datatype";
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    if (fields.mpi_rank_int == 0) {
        // Create a Group for provenance validation in the H5 file
        H5VL_loc_params_t loc_params; 
        loc_params.type     = H5VL_OBJECT_BY_SELF;
        loc_params.obj_type = H5I_FILE;

        H5VL_provenance_t *group;

        void *group_under = H5VLgroup_create(file->under_object, &loc_params, file->under_vol_id, "Provenance", 
            H5P_LINK_CREATE_DEFAULT, H5P_GROUP_CREATE_DEFAULT, H5P_GROUP_ACCESS_DEFAULT, H5P_DATASET_XFER_DEFAULT, req);

        if(group_under)
            group = _obj_wrap_under(group_under, file, name, H5I_GROUP, dxpl_id, req);
        else
            group = NULL;
        
        // Add provenance file path to H5 file as a file attribute synchronously 
        loc_params.type     = H5VL_OBJECT_BY_SELF;
        loc_params.obj_type = H5I_GROUP;

        H5VL_provenance_t *attr_path;

        // hsize_t dims = MAX_NUM_PROV_FILE;
        hsize_t dims = 1;
        hid_t aspace = H5Screate_simple(1, &dims, NULL);
        hid_t memtype = H5Tcopy (H5T_C_S1);

        H5Tset_size (memtype, strlen(info->prov_file_path)+1);

        void *attr_under = H5VLattr_create(group->under_object, &loc_params, group->under_vol_id, "Path",
           memtype, aspace, H5P_ATTRIBUTE_CREATE_DEFAULT, H5P_ATTRIBUTE_ACCESS_DEFAULT, H5P_DATASET_XFER_DEFAULT, req);

        // Wrap provenance path attribute
        if(attr_under)
           attr_path = _obj_wrap_under(attr_under, group, name, H5I_ATTR, dxpl_id, req);
        else
           attr_path = NULL;

        // Write and close provenance path attribute
        H5VLattr_write(attr_path->under_object, attr_path->under_vol_id, memtype,
           info->prov_file_path, H5P_DATASET_XFER_DEFAULT, req);
        H5VLattr_close(attr_path->under_object, attr_path->under_vol_id, H5P_DATASET_XFER_DEFAULT, req);
        
        H5VL_provenance_t *attr_time;

        attr_under = H5VLattr_create(group->under_object, &loc_params, group->under_vol_id, "Creation time",
           memtype, aspace, H5P_ATTRIBUTE_CREATE_DEFAULT, H5P_ATTRIBUTE_ACCESS_DEFAULT, H5P_DATASET_XFER_DEFAULT, req);

        // Wrap provenance file creation time attribute
        if(attr_under)
           attr_time = _obj_wrap_under(attr_under, group, name, H5I_ATTR, dxpl_id, req);
        else
           attr_time = NULL;
        
        // Write and close provenance file creation time attribute
        char time[64];
        get_time_str(time);
        H5VLattr_write(attr_time->under_object, attr_time->under_vol_id, memtype,
           time, H5P_DATASET_XFER_DEFAULT, req);
        H5VLattr_close(attr_time->under_object, attr_time->under_vol_id, H5P_DATASET_XFER_DEFAULT, req);

        H5Sclose(aspace);
        H5Tclose(memtype);
        // Add provenance file path to H5 file as a file attribute synchronously

        // Close group
        H5VLgroup_close(group->under_object, group->under_vol_id, dxpl_id, req);
        H5VL_provenance_free_obj(group);
    }
    /* PROV-IO instrument end */

    /* Close underlying FAPL */
    if(under_fapl_id > 0)
        H5Pclose(under_fapl_id);

    /* Release copy of our VOL info */
    if(info)
        H5VL_provenance_info_free(info);

#ifdef H5_HAVE_PARALLEL
    // Release MPI Comm & Info, if they weren't taken over
    if(have_mpi_comm_info) {
	if(MPI_COMM_NULL != mpi_comm)
	    MPI_Comm_free(&mpi_comm);
	if(MPI_INFO_NULL != mpi_info)
	    MPI_Info_free(&mpi_info);
    }
#endif /* H5_HAVE_PARALLEL */

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    return (void *)file;
} /* end H5VL_provenance_file_create() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_file_open
 *
 * Purpose:     Opens a container created with this connector
 *
 * Return:      Success:    Pointer to a file object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_file_open(const char *name, unsigned flags, hid_t fapl_id,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_info_t *info = NULL;
    H5VL_provenance_t *file;
    hid_t under_fapl_id = -1;
    void *under;
#ifdef H5_HAVE_PARALLEL
    hid_t driver_id;            // VFD driver for file
    MPI_Comm mpi_comm = MPI_COMM_NULL;  // MPI Comm from FAPL
    MPI_Info mpi_info = MPI_INFO_NULL;  // MPI Info from FAPL
    hbool_t have_mpi_comm_info = false;     // Whether the MPI Comm & Info are retrieved
#endif /* H5_HAVE_PARALLEL */

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL FILE Open\n");
#endif

    /* Get copy of our VOL info from FAPL */
    H5Pget_vol_info(fapl_id, (void **)&info);

    /* Copy the FAPL */
    under_fapl_id = H5Pcopy(fapl_id);

    /* Set the VOL ID and info for the underlying FAPL */
    H5Pset_vol(under_fapl_id, info->under_vol_id, info->under_vol_info);

#ifdef H5_HAVE_PARALLEL
    // Determine if the file is accessed with the parallel VFD (MPI-IO)
    // and copy the MPI comm & info objects for our use
    if((driver_id = H5Pget_driver(under_fapl_id)) > 0 && driver_id == H5FD_MPIO) {
        // Retrieve the MPI comm & info objects
        H5Pget_fapl_mpio(under_fapl_id, &mpi_comm, &mpi_info);

        // Indicate that the Comm & Info are available
        have_mpi_comm_info = true;
    }
#endif /* H5_HAVE_PARALLEL */

    /* Open the file with the underlying VOL connector */
    m1 = get_time_usec();
    under = H5VLfile_open(name, flags, under_fapl_id, dxpl_id, req);
    m2 = get_time_usec();

    //setup global
    if(under) {
        if(!PROV_HELPER)
            PROV_HELPER = prov_helper_init(info->prov_file_path, info->prov_level, info->prov_line_format);
        file = _file_open_common(under, info->under_vol_id, name);

#ifdef H5_HAVE_PARALLEL
        if(have_mpi_comm_info) {
            file_prov_info_t *file_info = file->generic_prov_info;

            // Take ownership of MPI Comm & Info
            file_info->mpi_comm = mpi_comm;
            file_info->mpi_info = mpi_info;
            file_info->mpi_comm_info_valid = true;

            // Reset flag, so Comm & Info aren't freed
            have_mpi_comm_info = false;
        }
#endif /* H5_HAVE_PARALLEL */

        /* Check for async request */
        if(req && *req)
            *req = H5VL_provenance_new_obj(*req, info->under_vol_id, file->prov_helper);
    } /* end if */
    else
        file = NULL;

    if(file)
        ;
        // prov_write(file->prov_helper, __func__, get_time_usec() - start);

    /* PROV-IO instrument start */
    const char* io_api = "H5Topen2";
    const char* io_api_async = "H5Topen_async";
    const char* relation = "provio:wasOpenedBy";
    const char* type = "provio:Datatype";
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    if (fields.mpi_rank_int == 0) {
        // Create a Group for provenance validation in the H5 file
        H5VL_loc_params_t loc_params; 
        loc_params.type     = H5VL_OBJECT_BY_SELF;
        loc_params.obj_type = H5I_FILE;

        H5VL_provenance_t *group;

        void *group_under = H5VLgroup_create(file->under_object, &loc_params, file->under_vol_id, "Provenance", 
            H5P_LINK_CREATE_DEFAULT, H5P_GROUP_CREATE_DEFAULT, H5P_GROUP_ACCESS_DEFAULT, H5P_DATASET_XFER_DEFAULT, req);

        if(group_under)
            group = _obj_wrap_under(group_under, file, name, H5I_GROUP, dxpl_id, req);
        else
            group = NULL;
        
        // Add provenance file path to H5 file as a file attribute synchronously 
        loc_params.type     = H5VL_OBJECT_BY_SELF;
        loc_params.obj_type = H5I_GROUP;

        H5VL_provenance_t *attr_path;

        // hsize_t dims = MAX_NUM_PROV_FILE;
        hsize_t dims = 1;
        hid_t aspace = H5Screate_simple(1, &dims, NULL);
        hid_t memtype = H5Tcopy (H5T_C_S1);

        H5Tset_size (memtype, strlen(info->prov_file_path)+1);

        void *attr_under = H5VLattr_create(group->under_object, &loc_params, group->under_vol_id, "Path",
           memtype, aspace, H5P_ATTRIBUTE_CREATE_DEFAULT, H5P_ATTRIBUTE_ACCESS_DEFAULT, H5P_DATASET_XFER_DEFAULT, req);

        // Wrap provenance path attribute
        if(attr_under)
           attr_path = _obj_wrap_under(attr_under, group, name, H5I_ATTR, dxpl_id, req);
        else
           attr_path = NULL;

        // Write and close provenance path attribute
        H5VLattr_write(attr_path->under_object, attr_path->under_vol_id, memtype,
           info->prov_file_path, H5P_DATASET_XFER_DEFAULT, req);
        H5VLattr_close(attr_path->under_object, attr_path->under_vol_id, H5P_DATASET_XFER_DEFAULT, req);
        
        H5VL_provenance_t *attr_time;

        attr_under = H5VLattr_create(group->under_object, &loc_params, group->under_vol_id, "Creation time",
           memtype, aspace, H5P_ATTRIBUTE_CREATE_DEFAULT, H5P_ATTRIBUTE_ACCESS_DEFAULT, H5P_DATASET_XFER_DEFAULT, req);

        // Wrap provenance file creation time attribute
        if(attr_under)
           attr_time = _obj_wrap_under(attr_under, group, name, H5I_ATTR, dxpl_id, req);
        else
           attr_time = NULL;
        
        // Write and close provenance file creation time attribute
        char time[64];
        get_time_str(time);
        H5VLattr_write(attr_time->under_object, attr_time->under_vol_id, memtype,
           time, H5P_DATASET_XFER_DEFAULT, req);
        H5VLattr_close(attr_time->under_object, attr_time->under_vol_id, H5P_DATASET_XFER_DEFAULT, req);

        H5Sclose(aspace);
        H5Tclose(memtype);
        // Add provenance file path to H5 file as a file attribute synchronously

        // Close group
        H5VLgroup_close(group->under_object, group->under_vol_id, dxpl_id, req);
        H5VL_provenance_free_obj(group);
    }
    /* PROV-IO instrument end */

    /* Close underlying FAPL */
    if(under_fapl_id > 0)
        H5Pclose(under_fapl_id);

    /* Release copy of our VOL info */
    if(info)
        H5VL_provenance_info_free(info);

#ifdef H5_HAVE_PARALLEL
    // Release MPI Comm & Info, if they weren't taken over
    if(have_mpi_comm_info) {
	if(MPI_COMM_NULL != mpi_comm)
	    MPI_Comm_free(&mpi_comm);
	if(MPI_INFO_NULL != mpi_info)
	    MPI_Info_free(&mpi_info);
    }
#endif /* H5_HAVE_PARALLEL */

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return (void *)file;
} /* end H5VL_provenance_file_open() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_file_get
 *
 * Purpose:     Get info about a file
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_file_get(void *file, H5VL_file_get_args_t *args, hid_t dxpl_id,
    void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)file;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL FILE Get\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLfile_get(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_file_get() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_file_specific
 *
 * Purpose:     Specific operation on file
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_file_specific(void *file, H5VL_file_specific_args_t *args,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)file;
    H5VL_provenance_t *new_o;
    H5VL_file_specific_args_t my_args;
    H5VL_file_specific_args_t *new_args;
    H5VL_provenance_info_t *info;
    hid_t under_vol_id = -1;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL FILE Specific\n");
#endif

    /* Check for 'is accessible' operation */
    if(args->op_type == H5VL_FILE_IS_ACCESSIBLE) {
        /* Make a (shallow) copy of the arguments */
        memcpy(&my_args, args, sizeof(my_args));

        /* Set up the new FAPL for the updated arguments */

        /* Get copy of our VOL info from FAPL */
        H5Pget_vol_info(args->args.is_accessible.fapl_id, (void **)&info);

        /* Make sure we have info about the underlying VOL to be used */
        if (!info)
            return (-1);

        /* Keep the correct underlying VOL ID for later */
        under_vol_id = info->under_vol_id;

        /* Copy the FAPL */
        my_args.args.is_accessible.fapl_id = H5Pcopy(args->args.is_accessible.fapl_id);

        /* Set the VOL ID and info for the underlying FAPL */
        H5Pset_vol(my_args.args.is_accessible.fapl_id, info->under_vol_id, info->under_vol_info);

        /* Set argument pointer to new arguments */
        new_args = &my_args;

        /* Set object pointer for operation */
        new_o = NULL;
    } /* end else-if */
    /* Check for 'delete' operation */
    else if(args->op_type == H5VL_FILE_DELETE) {
        /* Make a (shallow) copy of the arguments */
        memcpy(&my_args, args, sizeof(my_args));

        /* Set up the new FAPL for the updated arguments */

        /* Get copy of our VOL info from FAPL */
        H5Pget_vol_info(args->args.del.fapl_id, (void **)&info);

        /* Make sure we have info about the underlying VOL to be used */
        if (!info)
            return (-1);

        /* Keep the correct underlying VOL ID for later */
        under_vol_id = info->under_vol_id;

        /* Copy the FAPL */
        my_args.args.del.fapl_id = H5Pcopy(args->args.del.fapl_id);

        /* Set the VOL ID and info for the underlying FAPL */
        H5Pset_vol(my_args.args.del.fapl_id, info->under_vol_id, info->under_vol_info);

        /* Set argument pointer to new arguments */
        new_args = &my_args;

        /* Set object pointer for operation */
        new_o = NULL;
    } /* end else-if */
    else {
        /* Keep the correct underlying VOL ID for later */
        under_vol_id = o->under_vol_id;

        /* Set argument pointer to current arguments */
        new_args = args;

        /* Set object pointer for operation */
        new_o = o->under_object;
    } /* end else */

    m1 = get_time_usec();
    ret_value = H5VLfile_specific(new_o, under_vol_id, new_args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, under_vol_id, o->prov_helper);

    /* Check for 'is accessible' operation */
    if(args->op_type == H5VL_FILE_IS_ACCESSIBLE) {
        /* Close underlying FAPL */
        H5Pclose(my_args.args.is_accessible.fapl_id);

        /* Release copy of our VOL info */
        H5VL_provenance_info_free(info);
    } /* end else-if */
    /* Check for 'delete' operation */
    else if(args->op_type == H5VL_FILE_DELETE) {
        /* Close underlying FAPL */
        H5Pclose(my_args.args.del.fapl_id);

        /* Release copy of our VOL info */
        H5VL_provenance_info_free(info);
    } /* end else-if */
    else if(args->op_type == H5VL_FILE_REOPEN) {
        /* Wrap reopened file struct pointer, if we reopened one */
        if(ret_value >= 0 && args->args.reopen.file) {
            char *file_name = ((file_prov_info_t*)(o->generic_prov_info))->file_name;

            *args->args.reopen.file = _file_open_common(*args->args.reopen.file, under_vol_id, file_name);

            // Shouldn't need to duplicate MPI Comm & Info
            // since the file_info should be the same
        } /* end if */
    } /* end else */

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_file_specific() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_file_optional
 *
 * Purpose:     Perform a connector-specific operation on a file
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_file_optional(void *file, H5VL_optional_args_t *args,
                              hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)file;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL File Optional\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLfile_optional(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_file_optional() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_file_close
 *
 * Purpose:     Closes a file.
 *
 * Return:      Success:    0
 *              Failure:    -1, file not closed.
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_file_close(void *file, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)file;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL FILE Close\n");
#endif

    if(o){
        assert(o->generic_prov_info);

        file_stats_prov_write((file_prov_info_t*)(o->generic_prov_info));

        // prov_write(o->prov_helper, __func__, get_time_usec() - start);
    }

    m1 = get_time_usec();
    ret_value = H5VLfile_close(o->under_object, o->under_vol_id, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    /* Release our wrapper, if underlying file was closed */
    if(ret_value >= 0){
        rm_file_node(PROV_HELPER, ((file_prov_info_t*)(o->generic_prov_info))->file_no);

        H5VL_provenance_free_obj(o);
    }

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_file_close() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_group_create
 *
 * Purpose:     Creates a group inside a container
 *
 * Return:      Success:    Pointer to a group object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_group_create(void *obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t lcpl_id, hid_t gcpl_id, hid_t gapl_id, hid_t dxpl_id,
    void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *group;
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL GROUP Create\n");
#endif

    m1 = get_time_usec();
    under = H5VLgroup_create(o->under_object, loc_params, o->under_vol_id, name, lcpl_id, gcpl_id,  gapl_id, dxpl_id, req);
    m2 = get_time_usec();

    if(under)
        group = _obj_wrap_under(under, o, name, H5I_GROUP, dxpl_id, req);
    else
        group = NULL;

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    /* PROV-IO instrument start */
    const char* io_api = "H5Gcreate2";
    const char* io_api_async = "H5Gcreate2_async";
    const char* relation = "prov:wasGeneratedBy";
    const char* type = "provio:Group";    
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */

    return (void *)group;
} /* end H5VL_provenance_group_create() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_group_open
 *
 * Purpose:     Opens a group inside a container
 *
 * Return:      Success:    Pointer to a group object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_group_open(void *obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t gapl_id, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *group;
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL GROUP Open\n");
#endif

    m1 = get_time_usec();
    under = H5VLgroup_open(o->under_object, loc_params, o->under_vol_id, name, gapl_id, dxpl_id, req);
    m2 = get_time_usec();

    if(under)
        group = _obj_wrap_under(under, o, name, H5I_GROUP, dxpl_id, req);
    else
        group = NULL;

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));

    /* PROV-IO instrument start */
    const char* io_api = "H5Gopen2";
    const char* io_api_async = "H5Gopen_async";
    const char* relation = "provio:wasOpenedBy";
    const char* type = "provio:Group"; 
    prov_fill_data_object(&fields, name, type);
    prov_fill_relation(&fields, relation);
    prov_fill_io_api(&fields, io_api, get_time_usec() - start);
    add_prov_record(&config, provio_helper, &fields);
    func_stat(__func__, (get_time_usec() - start - (m2 - m1)));
    prov_stat.TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    /* PROV-IO instrument end */

    return (void *)group;
} /* end H5VL_provenance_group_open() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_group_get
 *
 * Purpose:     Get info about a group
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_group_get(void *obj, H5VL_group_get_args_t *args, hid_t dxpl_id,
    void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL GROUP Get\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLgroup_get(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_group_get() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_group_specific
 *
 * Purpose:     Specific operation on a group
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_group_specific(void *obj, H5VL_group_specific_args_t *args,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    H5VL_group_specific_args_t my_args;
    H5VL_group_specific_args_t *new_args;
    hid_t under_vol_id = -1;
    prov_helper_t *helper = NULL;
    group_prov_info_t *my_group_info;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL GROUP Specific\n");
#endif

    /* Unpack arguments to get at the child file pointer when mounting a file */
    if(args->op_type == H5VL_GROUP_MOUNT) {

        /* Make a (shallow) copy of the arguments */
        memcpy(&my_args, args, sizeof(my_args));

        /* Set the object for the child file */
        my_args.args.mount.child_file = ((H5VL_provenance_t *)args->args.mount.child_file)->under_object;

        /* Point to modified arguments */
        new_args = &my_args;
    } /* end if */
    else
        new_args = args;

    // Check if refreshing
    if(args->op_type == H5VL_GROUP_REFRESH) {
        // Save group prov info for later, and increment the refcount on it,
        // so that the stats aren't lost when the object is closed and reopened
        // during the underlying refresh operation
        my_group_info = (group_prov_info_t *)o->generic_prov_info;
        my_group_info->obj_info.ref_cnt++;
    }

    // Save copy of underlying VOL connector ID and prov helper, in case of
    // refresh destroying the current object
    under_vol_id = o->under_vol_id;
    helper = o->prov_helper;

    m1 = get_time_usec();
    ret_value = H5VLgroup_specific(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    if(args->op_type == H5VL_GROUP_REFRESH) {
        // Sanity check
        assert(my_group_info);

        // Get new group info, after refresh
        if(ret_value >= 0) {
            // Sanity check - make certain group info wasn't freed
            assert(my_group_info->obj_info.ref_cnt > 0);

            // Set object pointers to NULL, to avoid programming errors
            o = NULL;
            obj = NULL;

            // Update group info (nothing to update, currently)

            // Don't close group ID, it's owned by the application
        }

        // Decrement refcount on group info
        rm_grp_node(helper, o->under_object, o->under_vol_id, my_group_info);
    }

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, under_vol_id, helper);

    // prov_write(helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_group_specific() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_group_optional
 *
 * Purpose:     Perform a connector-specific operation on a group
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_group_optional(void *obj, H5VL_optional_args_t *args,
                               hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL GROUP Optional\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLgroup_optional(o->under_object, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_group_optional() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_group_close
 *
 * Purpose:     Closes a group.
 *
 * Return:      Success:    0
 *              Failure:    -1, group not closed.
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_group_close(void *grp, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)grp;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL H5Gclose\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLgroup_close(o->under_object, o->under_vol_id, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    /* Release our wrapper, if underlying group was closed */
    if(ret_value >= 0){
        group_prov_info_t* grp_info;

        grp_info = (group_prov_info_t*)o->generic_prov_info;

        // prov_write(o->prov_helper, __func__, get_time_usec() - start);
        group_stats_prov_write(grp_info);

        rm_grp_node(o->prov_helper, o->under_object, o->under_vol_id, grp_info);

        H5VL_provenance_free_obj(o);
    }

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_group_close() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_link_create
 *
 * Purpose:     Creates a hard / soft / UD / external link.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_link_create(H5VL_link_create_args_t *args, void *obj,
    const H5VL_loc_params_t *loc_params, hid_t lcpl_id, hid_t lapl_id,
    hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_link_create_args_t my_args;
    H5VL_link_create_args_t *new_args;
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    hid_t under_vol_id = -1;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL LINK Create\n");
#endif

    /* Try to retrieve the "under" VOL id */
    if(o)
        under_vol_id = o->under_vol_id;

    /* Fix up the link target object for hard link creation */
    if(H5VL_LINK_CREATE_HARD == args->op_type) {
        /* If it's a non-NULL pointer, find the 'under object' and re-set the args */
        if(args->args.hard.curr_obj) {
            /* Make a (shallow) copy of the arguments */
            memcpy(&my_args, args, sizeof(my_args));

            /* Check if we still need the "under" VOL ID */
            if(under_vol_id < 0)
                under_vol_id = ((H5VL_provenance_t *)args->args.hard.curr_obj)->under_vol_id;

            /* Set the object for the link target */
            my_args.args.hard.curr_obj = ((H5VL_provenance_t *)args->args.hard.curr_obj)->under_object;

            /* Set argument pointer to modified parameters */
            new_args = &my_args;
        } /* end if */
        else
            new_args = args;
    } /* end if */
    else
        new_args = args;

    /* Re-issue 'link create' call, possibly using the unwrapped pieces */
    m1 = get_time_usec();
    ret_value = H5VLlink_create(new_args, (o ? o->under_object : NULL), loc_params, under_vol_id, lcpl_id, lapl_id, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_link_create() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_link_copy
 *
 * Purpose:     Renames an object within an HDF5 container and copies it to a new
 *              group.  The original name SRC is unlinked from the group graph
 *              and then inserted with the new name DST (which can specify a
 *              new path for the object) as an atomic operation. The names
 *              are interpreted relative to SRC_LOC_ID and
 *              DST_LOC_ID, which are either file IDs or group ID.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_link_copy(void *src_obj, const H5VL_loc_params_t *loc_params1,
    void *dst_obj, const H5VL_loc_params_t *loc_params2, hid_t lcpl_id,
    hid_t lapl_id, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o_src = (H5VL_provenance_t *)src_obj;
    H5VL_provenance_t *o_dst = (H5VL_provenance_t *)dst_obj;
    hid_t under_vol_id = -1;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL LINK Copy\n");
#endif

    /* Retrieve the "under" VOL id */
    if(o_src)
        under_vol_id = o_src->under_vol_id;
    else if(o_dst)
        under_vol_id = o_dst->under_vol_id;
    assert(under_vol_id > 0);

    m1 = get_time_usec();
    ret_value = H5VLlink_copy((o_src ? o_src->under_object : NULL), loc_params1, (o_dst ? o_dst->under_object : NULL), loc_params2, under_vol_id, lcpl_id, lapl_id, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, under_vol_id, o_dst->prov_helper);

    if(o_dst)
        // prov_write(o_dst->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_link_copy() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_link_move
 *
 * Purpose:     Moves a link within an HDF5 file to a new group.  The original
 *              name SRC is unlinked from the group graph
 *              and then inserted with the new name DST (which can specify a
 *              new path for the object) as an atomic operation. The names
 *              are interpreted relative to SRC_LOC_ID and
 *              DST_LOC_ID, which are either file IDs or group ID.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_link_move(void *src_obj, const H5VL_loc_params_t *loc_params1,
    void *dst_obj, const H5VL_loc_params_t *loc_params2, hid_t lcpl_id,
    hid_t lapl_id, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o_src = (H5VL_provenance_t *)src_obj;
    H5VL_provenance_t *o_dst = (H5VL_provenance_t *)dst_obj;
    hid_t under_vol_id = -1;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL LINK Move\n");
#endif

    /* Retrieve the "under" VOL id */
    if(o_src)
        under_vol_id = o_src->under_vol_id;
    else if(o_dst)
        under_vol_id = o_dst->under_vol_id;
    assert(under_vol_id > 0);

    m1 = get_time_usec();
    ret_value = H5VLlink_move((o_src ? o_src->under_object : NULL), loc_params1, (o_dst ? o_dst->under_object : NULL), loc_params2, under_vol_id, lcpl_id, lapl_id, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, under_vol_id, o_dst->prov_helper);

    if(o_dst)
        // prov_write(o_dst->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_link_move() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_link_get
 *
 * Purpose:     Get info about a link
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_link_get(void *obj, const H5VL_loc_params_t *loc_params,
    H5VL_link_get_args_t *args, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL LINK Get\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLlink_get(o->under_object, loc_params, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_link_get() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_link_specific
 *
 * Purpose:     Specific operation on a link
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_link_specific(void *obj, const H5VL_loc_params_t *loc_params,
    H5VL_link_specific_args_t *args, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL LINK Specific\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLlink_specific(o->under_object, loc_params, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_link_specific() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_link_optional
 *
 * Purpose:     Perform a connector-specific operation on a link
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_link_optional(void *obj, const H5VL_loc_params_t *loc_params,
                H5VL_optional_args_t *args, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL LINK Optional\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLlink_optional(o->under_object, loc_params, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_link_optional() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_object_open
 *
 * Purpose:     Opens an object inside a container.
 *
 * Return:      Success:    Pointer to object
 *              Failure:    NULL
 *
 *-------------------------------------------------------------------------
 */
static void *
H5VL_provenance_object_open(void *obj, const H5VL_loc_params_t *loc_params,
    H5I_type_t *obj_to_open_type, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *new_obj;
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL OBJECT Open\n");
#endif

    m1 = get_time_usec();
    under = H5VLobject_open(o->under_object, loc_params, o->under_vol_id,
            obj_to_open_type, dxpl_id, req);
    m2 = get_time_usec();

    if(under) {
        const char* obj_name = NULL;

        if(loc_params->type == H5VL_OBJECT_BY_NAME)
            obj_name = loc_params->loc_data.loc_by_name.name;

        new_obj = _obj_wrap_under(under, o, obj_name, *obj_to_open_type, dxpl_id, req);
    } /* end if */
    else
        new_obj = NULL;

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return (void *)new_obj;
} /* end H5VL_provenance_object_open() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_object_copy
 *
 * Purpose:     Copies an object inside a container.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_object_copy(void *src_obj, const H5VL_loc_params_t *src_loc_params,
    const char *src_name, void *dst_obj, const H5VL_loc_params_t *dst_loc_params,
    const char *dst_name, hid_t ocpypl_id, hid_t lcpl_id, hid_t dxpl_id,
    void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o_src = (H5VL_provenance_t *)src_obj;
    H5VL_provenance_t *o_dst = (H5VL_provenance_t *)dst_obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL OBJECT Copy\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLobject_copy(o_src->under_object, src_loc_params, src_name, o_dst->under_object, dst_loc_params, dst_name, o_src->under_vol_id, ocpypl_id, lcpl_id, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o_src->under_vol_id, o_dst->prov_helper);

    if(o_dst)
        // prov_write(o_dst->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_object_copy() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_object_get
 *
 * Purpose:     Get info about an object
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_object_get(void *obj, const H5VL_loc_params_t *loc_params, H5VL_object_get_args_t *args, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL OBJECT Get\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLobject_get(o->under_object, loc_params, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_object_get() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_object_specific
 *
 * Purpose:     Specific operation on an object
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_object_specific(void *obj, const H5VL_loc_params_t *loc_params,
    H5VL_object_specific_args_t *args, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    void *under_obj = NULL;
    hid_t under_vol_id = -1;
    prov_helper_t *helper = NULL;
    object_prov_info_t *my_prov_info = NULL;
    H5I_type_t my_type;         //obj type, dataset, datatype, etc.,
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL OBJECT Specific\n");
#endif

    // Check if refreshing
    if(args->op_type == H5VL_OBJECT_REFRESH) {
        // Save prov info for later, and increment the refcount on it,
        // so that the stats aren't lost when the object is closed and reopened
        // during the underlying refresh operation
        my_prov_info = (object_prov_info_t *)o->generic_prov_info;
        my_prov_info->ref_cnt++;
    }

    // Save copy of underlying VOL connector ID and prov helper, in case of
    // refresh destroying the current object
    under_obj = o->under_object;
    under_vol_id = o->under_vol_id;
    helper = o->prov_helper;
    my_type = o->my_type;

    m1 = get_time_usec();
    ret_value = H5VLobject_specific(o->under_object, loc_params, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    if(args->op_type == H5VL_OBJECT_REFRESH) {
        // Sanity check
        assert(my_prov_info);

        // Get new object info, after refresh
        if(ret_value >= 0) {
            // Sanity check - make certain info wasn't freed
            assert(my_prov_info->ref_cnt > 0);

            // Set object pointers to NULL, to avoid programming errors
            o = NULL;
            obj = NULL;

            if(my_type == H5I_DATASET) {
                dataset_prov_info_t *my_dataset_info;
                hid_t dataset_id;
                hid_t space_id;

                // Get dataset ID from arg list
                dataset_id = args->args.refresh.obj_id;

                // Cast object prov info into a dataset prov info
                my_dataset_info = (dataset_prov_info_t *)my_prov_info;

                // Update dataspace dimensions & element count (which could have changed)
                space_id = H5Dget_space(dataset_id);
                H5Sget_simple_extent_dims(space_id, my_dataset_info->dimensions, NULL);
                my_dataset_info->dset_space_size = (hsize_t)H5Sget_simple_extent_npoints(space_id);
                H5Sclose(space_id);

                // Don't close dataset ID, it's owned by the application
            }
        }

        // Decrement refcount on object info
        if(my_type == H5I_DATASET)
            rm_dataset_node(helper, under_obj, under_vol_id, (dataset_prov_info_t *)my_prov_info);
        else if(my_type == H5I_GROUP)
            rm_grp_node(helper, under_obj, under_vol_id, (group_prov_info_t *)my_prov_info);
        else if(my_type == H5I_DATATYPE)
            rm_dtype_node(helper, under_obj, under_vol_id, (datatype_prov_info_t *)my_prov_info);
        else if(my_type == H5I_ATTR)
            rm_attr_node(helper, under_obj, under_vol_id, (attribute_prov_info_t *)my_prov_info);
        else
            assert(0 && "Unknown / unsupported object type");
    }

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, under_vol_id, helper);

    // prov_write(helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_object_specific() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_object_optional
 *
 * Purpose:     Perform a connector-specific operation for an object
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_object_optional(void *obj, const H5VL_loc_params_t *loc_params,
                                H5VL_optional_args_t *args, hid_t dxpl_id, void **req)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL OBJECT Optional\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLobject_optional(o->under_object, loc_params, o->under_vol_id, args, dxpl_id, req);
    m2 = get_time_usec();

    /* Check for async request */
    if(req && *req)
        *req = H5VL_provenance_new_obj(*req, o->under_vol_id, o->prov_helper);

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_object_optional() */

/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_introspect_get_cap_flags
 *
 * Purpose:     Query the capability flags for this connector and any
 *              underlying connector(s).
 *
 * Return:      SUCCEED / FAIL
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5VL_provenance_introspect_get_cap_flags(const void *_info, unsigned *cap_flags)
{
    const H5VL_provenance_info_t *info = (const H5VL_provenance_info_t *)_info;
    herr_t                          ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL INTROSPECT GetCapFlags\n");
#endif

    /* Invoke the query on the underlying VOL connector */
    ret_value = H5VLintrospect_get_cap_flags(info->under_vol_info, info->under_vol_id, cap_flags);

    /* Bitwise OR our capability flags in */
    if (ret_value >= 0)
        *cap_flags |= H5VL_provenance_cls.cap_flags;

    return ret_value;
} /* end H5VL_provenance_introspect_get_cap_flags() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_introspect_opt_query
 *
 * Purpose:     Query if an optional operation is supported by this connector
 *
 * Return:      SUCCEED / FAIL
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5VL_provenance_introspect_opt_query(void *obj, H5VL_subclass_t cls,
                                     int opt_type, uint64_t *flags)
{
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL INTROSPECT OptQuery\n");
#endif

    ret_value = H5VLintrospect_opt_query(o->under_object, o->under_vol_id,
                                         cls, opt_type, flags);

    return ret_value;
} /* end H5VL_provenance_introspect_opt_query() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_request_wait
 *
 * Purpose:     Wait (with a timeout) for an async operation to complete
 *
 * Note:        Releases the request if the operation has completed and the
 *              connector callback succeeds
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_request_wait(void *obj, uint64_t timeout,
    H5VL_request_status_t *status)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL REQUEST Wait\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLrequest_wait(o->under_object, o->under_vol_id, timeout, status);
    m2 = get_time_usec();

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    if(ret_value >= 0 && *status != H5ES_STATUS_IN_PROGRESS)
        H5VL_provenance_free_obj(o);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_request_wait() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_request_notify
 *
 * Purpose:     Registers a user callback to be invoked when an asynchronous
 *              operation completes
 *
 * Note:        Releases the request, if connector callback succeeds
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_request_notify(void *obj, H5VL_request_notify_t cb, void *ctx)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL REQUEST Wait\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLrequest_notify(o->under_object, o->under_vol_id, cb, ctx);
    m2 = get_time_usec();

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    if(ret_value >= 0)
        H5VL_provenance_free_obj(o);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_request_notify() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_request_cancel
 *
 * Purpose:     Cancels an asynchronous operation
 *
 * Note:        Releases the request, if connector callback succeeds
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_request_cancel(void *obj, H5VL_request_status_t *status)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL REQUEST Cancel\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLrequest_cancel(o->under_object, o->under_vol_id, status);
    m2 = get_time_usec();

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    if(ret_value >= 0)
        H5VL_provenance_free_obj(o);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_request_cancel() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_request_specific
 *
 * Purpose:     Specific operation on a request
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_request_specific(void *obj, H5VL_request_specific_args_t *args)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value = -1;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL REQUEST Specific\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLrequest_specific(o->under_object, o->under_vol_id, args);
    m2 = get_time_usec();

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_request_specific() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_request_optional
 *
 * Purpose:     Perform a connector-specific operation for a request
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_request_optional(void *obj, H5VL_optional_args_t *args)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL REQUEST Optional\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLrequest_optional(o->under_object, o->under_vol_id, args);
    m2 = get_time_usec();

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_request_optional() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_request_free
 *
 * Purpose:     Releases a request, allowing the operation to complete without
 *              application tracking
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_request_free(void *obj)
{
    unsigned long start = get_time_usec();
    unsigned long m1, m2;

    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL REQUEST Free\n");
#endif

    m1 = get_time_usec();
    ret_value = H5VLrequest_free(o->under_object, o->under_vol_id);
    m2 = get_time_usec();

    if(o)
        // prov_write(o->prov_helper, __func__, get_time_usec() - start);

    if(ret_value >= 0)
        H5VL_provenance_free_obj(o);

    TOTAL_PROV_OVERHEAD += (get_time_usec() - start - (m2 - m1));
    return ret_value;
} /* end H5VL_provenance_request_free() */

/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_blob_put
 *
 * Purpose:     Handles the blob 'put' callback
 *
 * Return:      SUCCEED / FAIL
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5VL_provenance_blob_put(void *obj, const void *buf, size_t size,
    void *blob_id, void *ctx)
{
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL BLOB Put\n");
#endif

    ret_value = H5VLblob_put(o->under_object, o->under_vol_id, buf, size,
        blob_id, ctx);

    return ret_value;
} /* end H5VL_provenance_blob_put() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_blob_get
 *
 * Purpose:     Handles the blob 'get' callback
 *
 * Return:      SUCCEED / FAIL
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5VL_provenance_blob_get(void *obj, const void *blob_id, void *buf,
    size_t size, void *ctx)
{
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL BLOB Get\n");
#endif

    ret_value = H5VLblob_get(o->under_object, o->under_vol_id, blob_id, buf,
        size, ctx);

    return ret_value;
} /* end H5VL_provenance_blob_get() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_blob_specific
 *
 * Purpose:     Handles the blob 'specific' callback
 *
 * Return:      SUCCEED / FAIL
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5VL_provenance_blob_specific(void *obj, void *blob_id,
    H5VL_blob_specific_args_t *args)
{
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL BLOB Specific\n");
#endif

    ret_value = H5VLblob_specific(o->under_object, o->under_vol_id, blob_id, args);

    return ret_value;
} /* end H5VL_provenance_blob_specific() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_blob_optional
 *
 * Purpose:     Handles the blob 'optional' callback
 *
 * Return:      SUCCEED / FAIL
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5VL_provenance_blob_optional(void *obj, void *blob_id, H5VL_optional_args_t *args)
{
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL BLOB Optional\n");
#endif

    ret_value = H5VLblob_optional(o->under_object, o->under_vol_id, blob_id, args);

    return ret_value;
} /* end H5VL_provenance_blob_optional() */

/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_token_cmp
 *
 * Purpose:     Compare two of the connector's object tokens, setting
 *              *cmp_value, following the same rules as strcmp().
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_token_cmp(void *obj, const H5O_token_t *token1,
    const H5O_token_t *token2, int *cmp_value)
{
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL TOKEN Compare\n");
#endif

    /* Sanity checks */
    assert(obj);
    assert(token1);
    assert(token2);
    assert(cmp_value);

    ret_value = H5VLtoken_cmp(o->under_object, o->under_vol_id, token1, token2, cmp_value);

    return ret_value;
} /* end H5VL_provenance_token_cmp() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_token_to_str
 *
 * Purpose:     Serialize the connector's object token into a string.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_token_to_str(void *obj, H5I_type_t obj_type,
    const H5O_token_t *token, char **token_str)
{
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL TOKEN To string\n");
#endif

    /* Sanity checks */
    assert(obj);
    assert(token);
    assert(token_str);

    ret_value = H5VLtoken_to_str(o->under_object, obj_type, o->under_vol_id, token, token_str);

    return ret_value;
} /* end H5VL_provenance_token_to_str() */


/*---------------------------------------------------------------------------
 * Function:    H5VL_provenance_token_from_str
 *
 * Purpose:     Deserialize the connector's object token from a string.
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_token_from_str(void *obj, H5I_type_t obj_type,
    const char *token_str, H5O_token_t *token)
{
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL TOKEN From string\n");
#endif

    /* Sanity checks */
    assert(obj);
    assert(token);
    assert(token_str);

    ret_value = H5VLtoken_from_str(o->under_object, obj_type, o->under_vol_id, token_str, token);

    return ret_value;
} /* end H5VL_provenance_token_from_str() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_provenance_optional
 *
 * Purpose:     Handles the generic 'optional' callback
 *
 * Return:      SUCCEED / FAIL
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5VL_provenance_optional(void *obj, H5VL_optional_args_t *args, hid_t dxpl_id, void **req)
{
    H5VL_provenance_t *o = (H5VL_provenance_t *)obj;
    herr_t ret_value;

#ifdef ENABLE_PROVNC_LOGGING
    printf("------- PROVENANCE VOL generic Optional\n");
#endif

    ret_value = H5VLoptional(o->under_object, o->under_vol_id, args, dxpl_id, req);

    return ret_value;
} /* end H5VL_provenance_optional() */
