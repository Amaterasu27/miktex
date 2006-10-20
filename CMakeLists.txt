## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006 Christian Schenk
## 
## This file is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published
## by the Free Software Foundation; either version 2, or (at your
## option) any later version.
## 
## This file is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this file; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

cmake_minimum_required(VERSION 2.5 FATAL_ERROR)

project(miktex)

## autotools compability names
set(PACKAGE_NAME "MiKTeX")
set(TOP_SRCDIR "${CMAKE_SOURCE_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

include(miktex-version.cmake)

include(DisableRuntimeChecks)
include(InstallPaths)
include(LibraryNames)
include(SourcePaths)
include(UseStaticCRT)
include(UseStaticLibraries)

###############################################################################
## set MiKTeX version info

set(MIKTEX_SERIES_STR
    "${MIKTEX_MAJOR_VERSION}.${MIKTEX_MINOR_VERSION}")

if(MIKTEX_RELEASE_STATE EQUAL 0)
  set(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR}"
      CACHE STRING
      "The MikTeX version string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 1)
  set(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR} RC ${MIKTEX_RELEASE_NUM}"
      CACHE STRING
      "The MikTeX version string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 2)
  set(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR} Beta ${MIKTEX_RELEASE_NUM}"
      CACHE STRING
      "The MikTeX version string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 4)
  SET(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION} snapshot"
      CACHE STRING
      "The MikTeX version string."
      FORCE)
endif(MIKTEX_RELEASE_STATE EQUAL 0)

###############################################################################
## set date/time info

# fixme: use perl
execute_process(COMMAND "bash" "-c" "date" OUTPUT_VARIABLE d)
string(REGEX REPLACE ".$" "" MIKTEX_DATETIME_STR ${d})

###############################################################################
## set the build type

if(MIKTEX_RELEASE_STATE EQUAL 0)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "The build type." FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 1)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "The build type." FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 2)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "The build type." FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 4)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type." FORCE)
endif(MIKTEX_RELEASE_STATE EQUAL 0)

mark_as_advanced(CMAKE_BUILD_TYPE)

###############################################################################
## add global definitions

add_definitions(-DMIKTEX)

if(WIN32)
  add_definitions(
    -DMIKTEX_WINDOWS
  )
  # DLL export enablers
  add_definitions(
    -DZLIB_DLL
  )
  # Windows version macros
  add_definitions(
    -DWINVER=0x0400
    -D_WIN32_WINDOWS=0x0400
    -D_WIN32_WINNT=0x0400
    -D_WIN32_IE=0x0500
  )
else(WIN32)
  add_definitions(-DMIKTEX_UNIX)
endif(WIN32)

###############################################################################
## add global include directories

include_directories(
  ${CMAKE_BINARY_DIR}/${rel_core_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}
  ${CMAKE_SOURCE_DIR}/${rel_gd_dir}
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}
  ${CMAKE_SOURCE_DIR}/${rel_jpeg_dir}
  ${CMAKE_SOURCE_DIR}/${rel_md5_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}
  ${CMAKE_SOURCE_DIR}/${rel_netpbm_dir}
  ${CMAKE_SOURCE_DIR}/${rel_netpbm_dir}/pbm
  ${CMAKE_SOURCE_DIR}/${rel_netpbm_dir}/pgm
  ${CMAKE_SOURCE_DIR}/${rel_netpbm_dir}/pnm
  ${CMAKE_SOURCE_DIR}/${rel_netpbm_dir}/ppm
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}
  ${CMAKE_SOURCE_DIR}/${rel_tiff_dir}
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}
)

if(WIN32)
  include_directories(
    ${CMAKE_SOURCE_DIR}/${rel_ui_mfc_dir}/include
  )
endif(WIN32)

###############################################################################
## add global link direcories

link_directories(
  ${CMAKE_BINARY_DIR}/${rel_bzip2_dir}
  ${CMAKE_BINARY_DIR}/${rel_core_dir}
  ${CMAKE_BINARY_DIR}/${rel_curl_dir}
  ${CMAKE_BINARY_DIR}/${rel_expat_dir}
  ${CMAKE_BINARY_DIR}/${rel_gnu_dir}
  ${CMAKE_BINARY_DIR}/${rel_jpeg_dir}
  ${CMAKE_BINARY_DIR}/${rel_md5_dir}
  ${CMAKE_BINARY_DIR}/${rel_mspack_dir}
  ${CMAKE_BINARY_DIR}/${rel_netpbm_dir}/pbm
  ${CMAKE_BINARY_DIR}/${rel_netpbm_dir}/pgm
  ${CMAKE_BINARY_DIR}/${rel_netpbm_dir}/pnm
  ${CMAKE_BINARY_DIR}/${rel_netpbm_dir}/ppm
  ${CMAKE_BINARY_DIR}/${rel_png_dir}
  ${CMAKE_BINARY_DIR}/${rel_tiff_dir}
  ${CMAKE_BINARY_DIR}/${rel_ui_dir}/include
  ${CMAKE_BINARY_DIR}/${rel_zlib_dir}
)

if(WIN32)
  link_directories(
    ${CMAKE_BINARY_DIR}/${rel_bzip2_dir}-static
    ${CMAKE_BINARY_DIR}/${rel_core_dir}-static
    ${CMAKE_BINARY_DIR}/${rel_curl_dir}-static
    ${CMAKE_BINARY_DIR}/${rel_expat_dir}-static
    ${CMAKE_BINARY_DIR}/${rel_gnu_dir}-static
    ${CMAKE_BINARY_DIR}/${rel_md5_dir}-static
    ${CMAKE_BINARY_DIR}/${rel_mspack_dir}-static
    ${CMAKE_BINARY_DIR}/${rel_zlib_dir}-static
  )
endif(WIN32)

###############################################################################
## configure files

configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in
               ${CMAKE_BINARY_DIR}/DoxyFile)

configure_file(${CMAKE_SOURCE_DIR}/Documentation/version.ent.in
               ${CMAKE_BINARY_DIR}/Documentation/version.ent)

configure_file(${CMAKE_SOURCE_DIR}/Documentation/Styles/db2latex-common.xsl.in
               ${CMAKE_BINARY_DIR}/Documentation/Styles/db2latex-common.xsl)

configure_file(${CMAKE_SOURCE_DIR}/BuildUtilities/docbook/catalog.in
               ${CMAKE_BINARY_DIR}/BuildUtilities/docbook/catalog)

file(GLOB tpms "${CMAKE_SOURCE_DIR}/${rel_tpm_dir}/*.tpm.in")
foreach(tpm ${tpms})
  get_filename_component(fn ${tpm} NAME_WE)
  configure_file(${tpm}
	"${CMAKE_BINARY_DIR}/${rel_tpm_dir}/${fn}-${MIKTEX_SERIES_STR}.tpm")
endforeach(tpm)

###############################################################################
## add sub-directories

add_subdirectory(${rel_build_tools_etc_dir})

add_subdirectory(${rel_bzip2_dir})
if(WIN32)
  add_subdirectory(${rel_bzip2_dir}-static)
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_curl_dir})
  add_subdirectory(${rel_curl_dir}-static)
endif(WIN32)

add_subdirectory(${rel_expat_dir})
if(WIN32)
  add_subdirectory(${rel_expat_dir}-static)
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_gnu_dir})
  add_subdirectory(${rel_gnu_dir}-static)
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_jpeg_dir})
endif(WIN32)

add_subdirectory(${rel_mspack_dir})
if(WIN32)
  add_subdirectory(${rel_mspack_dir}-static)
endif(WIN32)

add_subdirectory(${rel_popt_dir})
if(WIN32)
  add_subdirectory(${rel_popt_dir}-static)
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_tiff_dir})
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_netpbm_dir})
endif(WIN32)

add_subdirectory(${rel_zlib_dir})
if(WIN32)
  add_subdirectory(${rel_zlib_dir}-static)
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_png_dir})
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_bmeps_dir})
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_gd_dir})
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_goo_dir})
  add_subdirectory(${rel_fofi_dir})
  add_subdirectory(${rel_xpdf_dir})
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_c4p_dir})
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_cweb_dir})
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_tie_dir})
endif(WIN32)

add_subdirectory(${rel_md5_dir})
if(WIN32)
  add_subdirectory(${rel_md5_dir}-static)
endif(WIN32)

add_subdirectory(${rel_core_dir})
if(WIN32)
  add_subdirectory(${rel_core_dir}-static)
endif(WIN32)

add_subdirectory(${rel_mpmlib_dir})
if(WIN32)
  add_subdirectory(${rel_mpmlib_dir}-static)
endif(WIN32)

if(WIN32)
  add_subdirectory(${rel_ui_mfc_dir})
endif(WIN32)
