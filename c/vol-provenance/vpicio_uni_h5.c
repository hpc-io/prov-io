/****** Copyright Notice ***
 *
 * PIOK - Parallel I/O Kernels - VPIC-IO, VORPAL-IO, and GCRM-IO, Copyright
 * (c) 2015, The Regents of the University of California, through Lawrence
 * Berkeley National Laboratory (subject to receipt of any required
 * approvals from the U.S. Dept. of Energy).  All rights reserved.
 *
 * If you have questions about your rights to use or distribute this
 * software, please contact Berkeley Lab's Innovation & Partnerships Office
 * at  IPO@lbl.gov.
 *
 * NOTICE.  This Software was developed under funding from the U.S.
 * Department of Energy and the U.S. Government consequently retains
 * certain rights. As such, the U.S. Government has been granted for itself
 * and others acting on its behalf a paid-up, nonexclusive, irrevocable,
 * worldwide license in the Software to reproduce, distribute copies to the
 * public, prepare derivative works, and perform publicly and display
 * publicly, and to permit other to do so.
 *
 ****************************/

/**
 *
 * Email questions to SByna@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
*/

// Description: This is a simple benchmark based on VPIC's I/O interface
//		Each process writes a specified number of particles into
//		a hdf5 output file using only HDF5 calls
// Author:	Suren Byna <SByna@lbl.gov>
//		Lawrence Berkeley National Laboratory, Berkeley, CA
// Created:	in 2011
// Modified:	01/06/2014 --> Removed all H5Part calls and using HDF5 calls
//          	02/19/2019 --> Add option to write multiple timesteps of data - Tang
//


#include "hdf5.h"
#include "H5VLprovnc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include <mpi.h>
#include <time.h>
// A simple timer based on gettimeofday

// static
// unsigned long get_time_usec(void) {
//     struct timeval tp;

//     gettimeofday(&tp, NULL);
//     return (unsigned long)((1000000 * tp.tv_sec) + tp.tv_usec);
// }

#define DTYPE float

extern struct timeval start_time[3];
extern float elapse[3];
#define timer_on(id) gettimeofday (&start_time[id], NULL)
#define timer_off(id) 	\
		{	\
		     struct timeval result, now; \
		     gettimeofday (&now, NULL);  \
		     timeval_subtract(&result, &now, &start_time[id]);	\
		     elapse[id] += result.tv_sec+ (DTYPE) (result.tv_usec)/1000000.;	\
		}

#define timer_msg(id, msg) \
	printf("%f seconds elapsed in %s\n", (DTYPE)(elapse[id]), msg);  \

#define timer_reset(id) elapse[id] = 0





//========================== For Provenance VOL ==========================
#define H5_MB (1024.0F * 1024.0F)
#define TRUE true
#define FALSE false
#define H5TEST_MULTI_FILENAME_LEN       1024

/* Temporary file for sending signal messages */
#define TMP_SIGNAL_FILE "tmp_signal_file"

/* The # of seconds to wait for the message file--used by h5_wait_message() */
#define MESSAGE_TIMEOUT         300             /* Timeout in seconds */
#define HDgetenv(S)    getenv(S)
#define HDstrcmp(X,Y)       strcmp(X,Y)
#define HDstrtok_r(X,Y,Z) strtok_r(X,Y,Z)
#define HDstrncpy(X,Y,Z)  strncpy(X,Y,Z)
//========================== For Provenance VOL ==========================



/* Subtract the `struct timeval' values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0.  */

//unsigned long get_time_usec(void) {
  //  struct timeval tp;
//    gettimeofday(&tp, NULL);
  //  return (unsigned long)((1000000 * tp.tv_sec) + tp.tv_usec);
//}

int
timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}


struct timeval start_time[3];
float elapse[3];

// HDF5 specific declerations
herr_t ierr;

// Variables and dimensions
long numparticles = 8388608;	// 8  meg particles per process
long long total_particles, offset;

float *x, *y, *z;
float *px, *py, *pz;
int *id1, *id2;
int x_dim = 64;
int y_dim = 64;
int z_dim = 64;

// Uniform random number
double uniform_random_number()
{
    return (((double)rand())/((double)(RAND_MAX)));
}

// Initialize particle data
void init_particles ()
{
    int i;
    for (i=0; i<numparticles; i++)
    {
        id1[i] = i;
        id2[i] = i*2;
        x[i] = uniform_random_number()*x_dim;
        y[i] = uniform_random_number()*y_dim;
        z[i] = ((double)id1[i]/numparticles)*z_dim;
        px[i] = uniform_random_number()*x_dim;
        py[i] = uniform_random_number()*y_dim;
        pz[i] = ((double)id2[i]/numparticles)*z_dim;
    }
}

// Create HDF5 file and write data
void create_and_write_synthetic_h5_data(int rank, hid_t loc, hid_t *dset_ids, hid_t filespace, hid_t memspace, hid_t plist_id)
{
    int i;
    // Note: printf statements are inserted basically
    // to check the progress. Other than that they can be removed
    dset_ids[0] = H5Dcreate(loc, "x", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    ierr = H5Dwrite(dset_ids[0], H5T_NATIVE_FLOAT, memspace, filespace, plist_id, x);
    /* if (rank == 0) printf ("Written variable 1 \n"); */

    dset_ids[1] = H5Dcreate(loc, "y", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    ierr = H5Dwrite(dset_ids[1], H5T_NATIVE_FLOAT, memspace, filespace, plist_id, y);
    /* if (rank == 0) printf ("Written variable 2 \n"); */

    dset_ids[2] = H5Dcreate(loc, "z", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    ierr = H5Dwrite(dset_ids[2], H5T_NATIVE_FLOAT, memspace, filespace, plist_id, z);
    /* if (rank == 0) printf ("Written variable 3 \n"); */

    dset_ids[3] = H5Dcreate(loc, "id1", H5T_NATIVE_INT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    ierr = H5Dwrite(dset_ids[3], H5T_NATIVE_INT, memspace, filespace, plist_id, id1);
    /* if (rank == 0) printf ("Written variable 4 \n"); */

    dset_ids[4] = H5Dcreate(loc, "id2", H5T_NATIVE_INT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    ierr = H5Dwrite(dset_ids[4], H5T_NATIVE_INT, memspace, filespace, plist_id, id2);
    /* if (rank == 0) printf ("Written variable 5 \n"); */

    dset_ids[5] = H5Dcreate(loc, "px", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    ierr = H5Dwrite(dset_ids[5], H5T_NATIVE_FLOAT, memspace, filespace, plist_id, px);
    /* if (rank == 0) printf ("Written variable 6 \n"); */

    dset_ids[6] = H5Dcreate(loc, "py", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    ierr = H5Dwrite(dset_ids[6], H5T_NATIVE_FLOAT, memspace, filespace, plist_id, py);
    /* if (rank == 0) printf ("Written variable 7 \n"); */

    dset_ids[7] = H5Dcreate(loc, "pz", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    ierr = H5Dwrite(dset_ids[7], H5T_NATIVE_FLOAT, memspace, filespace, plist_id, pz);
    /* if (rank == 0) printf ("Written variable 8 \n"); */
    if (rank == 0) printf ("  Finished written 8 variables \n");

}

void print_usage(char *name)
{
    printf("Usage: %s /path/to/file #timestep sleep_sec enable_prov_logging /path/to/prov_log_file\n", name);
}

hid_t fileaccess_mod(const char* log_file_path){
    hid_t fapl_id = H5I_INVALID_HID;
    htri_t connector_is_registered;    
    H5VL_provenance_info_t prov_vol_info;
    //void *vol_info = NULL;

    hid_t under_vol_id;
    void *under_vol_info;
    herr_t status;

    fapl_id = H5Pcreate(H5P_FILE_ACCESS);
//    if(fapl_id == H5P_DEFAULT);
       // printf("fapl_id == H5P_DEFAULT = %lld \n", fapl_id);

    status = H5Pget_vol_id(fapl_id, &under_vol_id);
    assert(status >= 0);
    assert(under_vol_id > 0);

    status = H5Pget_vol_info(fapl_id, &under_vol_info);
    assert(status >= 0);

    prov_vol_info.under_vol_id = under_vol_id;
    prov_vol_info.under_vol_info = under_vol_info;

    hid_t connector_id = -1;

    
    if((connector_is_registered = H5VLis_connector_registered_by_name("provenance")) < 0){
        //printf("(connector is registered) < 0 \n");
        goto done;
    }
    else if(connector_is_registered) {
        //printf("connector is registered \n");
        /* Retrieve the ID of the already-registered VOL connector */
         if((connector_id = H5VLget_connector_id_by_name("provenance")) < 0){
            printf("can not get vol id of provenance \n");
             goto error;
         }
    } 
    else {//not registed        
        //use built-in connector
        //printf("use built-in connector \n");
        connector_id = H5VL_PROVNC;
        if(H5Iinc_ref(connector_id) < 0){
            printf("H5Iinc_ref failed \n");
         goto error;      
        }
    } 

    prov_vol_info.prov_file_path = log_file_path;//"my_prov_file";
    prov_vol_info.prov_level = 2; //0/1 for print, 2 for file only, 3 for both.
    prov_vol_info.prov_line_format = "";

    //printf("provenance data path: %s\n", prov_vol_info.prov_file_path);
    if(H5Pset_vol(fapl_id, connector_id, &prov_vol_info) < 0){
        printf("H5Pset_vol failed \n");
        goto error;
    }

done:
    return fapl_id;

error:
    printf("ERROR!\n");
    // if(vol_info)
    //     H5VLfree_connector_info(connector_id, vol_info);
    // if(connector_id >= 0)
    //     H5VLunregister_connector(connector_id);

    return -1;
}

int main (int argc, char* argv[])
{
    char *file_name = argv[1];

    MPI_Init(&argc,&argv);
    int my_rank, num_procs, nts, i, j, nthread, sleep_time = 0;
    MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size (MPI_COMM_WORLD, &num_procs);

    MPI_Comm comm  = MPI_COMM_WORLD;
    MPI_Info info  = MPI_INFO_NULL;

    hid_t file_id, filespace, memspace, plist_id, *grp_ids, fapl, **dset_ids;
    char grp_name[128];

    if (argc < 5) {
        print_usage(argv[0]);
        return 0;
    }

    nts = atoi(argv[2]);
    if (nts <= 0) {
        print_usage(argv[0]);
        return 0;
    }

    sleep_time = atoi(argv[3]);
    if (sleep_time < 0) {
        print_usage(argv[0]);
        return 0;
    }

    int prov_enable = atoi(argv[4]);
    const char* log_file_path = argv[5];
    if (argc == 7) {
        numparticles = (atoi (argv[6]))*1024*1024;
    }
    else {
        numparticles = 8*1024*1024;
    }

    if (my_rank == 0) {
        printf ("Number of paritcles: %ld \n", numparticles);
    }

    x=(float*)malloc(numparticles*sizeof(double));
    y=(float*)malloc(numparticles*sizeof(double));
    z=(float*)malloc(numparticles*sizeof(double));

    px=(float*)malloc(numparticles*sizeof(double));
    py=(float*)malloc(numparticles*sizeof(double));
    pz=(float*)malloc(numparticles*sizeof(double));

    id1=(int*)malloc(numparticles*sizeof(int));
    id2=(int*)malloc(numparticles*sizeof(int));

    init_particles ();

    if (my_rank == 0)
        printf ("Finished initializeing particles \n");

    MPI_Barrier (MPI_COMM_WORLD);
    unsigned long start = get_time_usec();
    timer_on (0);

    MPI_Allreduce(&numparticles, &total_particles, 1, MPI_LONG_LONG, MPI_SUM, comm);
    MPI_Scan(&numparticles, &offset, 1, MPI_LONG_LONG, MPI_SUM, comm);
    offset -= numparticles;

    nthread  = 1;

    //printf("Reading env = %s\n", HDgetenv("HDF5_VOL_CONNECTOR"));
    //fapl = H5Pcreate(H5P_FILE_ACCESS);
    fapl = fileaccess_mod(log_file_path);
    // if(prov_enable){
    //     fapl = fileaccess_mod();
    // }else
    //     fapl = H5Pcreate(H5P_FILE_ACCESS);

        H5Pset_fapl_mpio(fapl, comm, info);

    file_id = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, fapl);
    H5Pclose(fapl);

    //if (my_rank == 0)
        //printf ("Opened HDF5 file... \n");

    filespace = H5Screate_simple(1, (hsize_t *) &total_particles, NULL);
    memspace =  H5Screate_simple(1, (hsize_t *) &numparticles, NULL);

    plist_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);

    H5Sselect_hyperslab(filespace, H5S_SELECT_SET, (hsize_t *) &offset, NULL, (hsize_t *) &numparticles, NULL);

    dset_ids = (hid_t**)calloc(nts, sizeof(hid_t*));
    grp_ids  = (hid_t*)calloc(nts, sizeof(hid_t));

    MPI_Barrier (MPI_COMM_WORLD);
    timer_on (1);

    for (i = 0; i < nts; i++) {
        sprintf(grp_name, "Timestep_%d", i);
        grp_ids[i] = H5Gcreate2(file_id, grp_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        if (my_rank == 0)
            printf ("Writing %s ... \n", grp_name);

        dset_ids[i] = (hid_t*)calloc(8, sizeof(hid_t));
        create_and_write_synthetic_h5_data(my_rank, grp_ids[i], dset_ids[i], filespace, memspace, plist_id);

        if (i != nts - 1) {
            if (my_rank == 0) printf ("  sleep for %ds\n", sleep_time);
            sleep(sleep_time);
        }
    }


    MPI_Barrier (MPI_COMM_WORLD);

    timer_off (1);

    for (i = 0; i < nts; i++) {
        for (j = 0; j < 8; j++) {
            H5Dclose(dset_ids[i][j]);
        }
        H5Gclose(grp_ids[i]);
    }

    H5Sclose(memspace);
    H5Sclose(filespace);
    H5Pclose(plist_id);
    H5Fclose(file_id);
    /* if (my_rank == 0) printf ("After closing HDF5 file \n"); */
    MPI_Barrier (MPI_COMM_WORLD);

    timer_off (0);

    if (my_rank == 0) {
        //printf ("\nTiming results\n");
        //printf("Total sleep time %ds\n", sleep_time*(nts-1));
        //timer_msg (1, "just writing data");
        //timer_msg (0, "total running");//opening, writing, closing file
        //printf ("\n");
	printf("Total running time: %lu\n", get_time_usec() - start);
    }

    free(x);
    free(y);
    free(z);
    free(px);
    free(py);
    free(pz);
    free(id1);
    free(id2);


    MPI_Finalize();
    return 0;
}
