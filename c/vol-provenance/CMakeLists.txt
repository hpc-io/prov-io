project(HDF5_VOL_PROVENANCE C)

cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

if(APPLE)
  # We are doing a unix-style install i.e. everything will be installed in
  # CMAKE_INSTALL_PREFIX/bin and CMAKE_INSTALL_PREFIX/lib etc. as on other unix
  # platforms. We still need to setup CMAKE_INSTALL_NAME_DIR correctly so that
  # the binaries point to appropriate location for the libraries.

  # 1. Make CMAKE_INSTALL_PREFIX publicly accessible, if it was hidden in
  #    previous pass
  get_property(is_internal CACHE CMAKE_INSTALL_PREFIX PROPERTY TYPE)
  if(is_internal STREQUAL "INTERNAL")
    set(CMAKE_INSTALL_PREFIX ${CACHED_CMAKE_INSTALL_PREFIX} CACHE PATH "Install prefix" FORCE)
  else()
    set(CMAKE_INSTALL_PREFIX ${CACHED_CMAKE_INSTALL_PREFIX} CACHE PATH "Install prefix")
  endif()
  unset(MACOSX_APP_INSTALL_PREFIX CACHE)

  set(CMAKE_MACOSX_RPATH ON)
  set(CMAKE_INSTALL_NAME_DIR "@rpath")
  mark_as_advanced(
    CMAKE_OSX_ARCHITECTURES
    CMAKE_OSX_DEPLOYMENT_TARGET
    CMAKE_OSX_SYSROOT
    )
endif()

#------------------------------------------------------------------------------
# Include source and build directories
#------------------------------------------------------------------------------
set(HDF5_VOL_PROV_BUILD_INCLUDE_DEPENDENCIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

#------------------------------------------------------------------------------
# External dependencies
#------------------------------------------------------------------------------
#HDF5
find_package(HDF5 REQUIRED)
if(HDF5_FOUND)
  include_directories(${HDF5_INCLUDE_DIRS})
else()
  # Allow for HDF5 autotools builds
  find_package(HDF5 MODULE REQUIRED)
  if(HDF5_FOUND)
    include_directories(${HDF5_INCLUDE_DIRS})
  else()
    message(FATAL_ERROR "Could not find HDF5, please check HDF5_DIR.")
  endif()
endif()

# MPI
# Temporary workaround because HDF5 does not pull MPI dependency
find_package(MPI REQUIRED)
if(MPI_FOUND)
  include_directories(${MPI_INCLUDE_PATH})
else()
  message(FATAL_ERROR "Could not find MPI.")
endif()

#------------------------------------------------------------------------------
# Set sources
#------------------------------------------------------------------------------
set(HDF5_VOL_PROV_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/H5VLprovnc.c
)

#------------------------------------------------------------------------------
# Libraries
#------------------------------------------------------------------------------
# HDF5 PROV
add_library(hdf5_vol_prov ${HDF5_VOL_PROV_SRCS})
message("HDF5_LIBRARIES: ${HDF5_LIBRARIES}")
target_link_libraries(hdf5_vol_prov ${HDF5_LIBRARIES})

#------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------
add_executable(vpicio_uni_h5 vpicio_uni_h5.c)

target_link_libraries(vpicio_uni_h5 hdf5_vol_prov ${MPI_LIBRARIES})
