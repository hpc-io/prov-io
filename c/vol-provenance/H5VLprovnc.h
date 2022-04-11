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
 * Purpose:	The public header file for the pass-through VOL connector.
 */

#ifndef _H5VLprovnc_H
#define _H5VLprovnc_H

#include "provio.h"

/* Identifier for the pass-through VOL connector */
#define H5VL_PROVNC	(H5VL_provenance_register())

/* Characteristics of the pass-through VOL connector */
#define H5VL_PROVNC_NAME        "provenance"
#define H5VL_PROVNC_VALUE       509           /* VOL connector ID */
#define H5VL_PROVNC_VERSION     0


/* Pass-through VOL connector info */
typedef struct H5VL_provenance_info_t {
    hid_t under_vol_id;         /* VOL ID for under VOL */
    void *under_vol_info;       /* VOL info for under VOL */
    char* prov_file_path;
    Prov_level prov_level;
    char* prov_line_format;
} H5VL_provenance_info_t;


#ifdef __cplusplus
extern "C" {
#endif

H5_DLL hid_t H5VL_provenance_register(void);

#ifdef __cplusplus
}
#endif

#endif /* _H5VLprovnc_H */

