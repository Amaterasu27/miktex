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

if(WIN32 AND NOT CYGWIN)
  set(NATIVE_WINDOWS 1)
else(WIN32 AND NOT CYGWIN)
  set(UNIX_ALIKE 1)
endif(WIN32 AND NOT CYGWIN)

if(NATIVE_WINDOWS)
  set(PRODUCT_NAME "MiKTeX")
  set(BUILDING_MIKTEX TRUE)
else(NATIVE_WINDOWS)
  set(PRODUCT_NAME "MiKTeX Tools")
  set(BUILDING_MIKTEX_TOOLS TRUE)
endif(NATIVE_WINDOWS)

project(miktex)

enable_testing()

set(outdir "${CMAKE_BINARY_DIR}/binlib")
set(EXECUTABLE_OUTPUT_PATH "${outdir}")
set(LIBRARY_OUTPUT_PATH "${outdir}")

## autotools compability names
set(PACKAGE_NAME ${PRODUCT_NAME})
set(TOP_SRCDIR "${CMAKE_SOURCE_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

include(miktex-version.cmake)

include(CheckFunctionExists)
include(CreateCppFromC)
include(CreateDllWrapper)
include(CreateExeAlias)
include(CreateWebApp)
include(DefaultCharTypeIsUnsigned)
include(DisableRuntimeChecks)
include(InstallPaths)
include(LibraryNames)
include(SourcePaths)
include(UseStaticCRT)
include(UseStaticLibraries)

if(NATIVE_WINDOWS)
  set(dev_null "nul")
else(NATIVE_WINDOWS)
  set(dev_null "/dev/null")
endif(NATIVE_WINDOWS)

###############################################################################
## find stuff

if(BUILDING_MIKTEX)
  set(required_programs
    bison
    cat
    cp
    date
    diff
    dos2unix
    flex
    perl
    sed
  )
else(BUILDING_MIKTEX)
  set(required_programs)
endif(BUILDING_MIKTEX)

foreach(p ${required_programs})
  string(TOUPPER "${p}" p_u)
  find_program(${p_u}_EXE NAMES ${p})
  if(NOT ${p_u}_EXE)
    message(FATAL_ERROR "The program ${p} could not be found.")
  endif(NOT ${p_u}_EXE)
endforeach(p)

find_package(MFC)
find_package(THREADS REQUIRED)

if(NATIVE_WINDOWS)
  find_package(CURL QUIET)
else(NATIVE_WINDOWS)
  find_package(CURL REQUIRED)
endif(NATIVE_WINDOWS)

check_function_exists(canonicalize_file_name HAVE_CANONICALIZE_FILE_NAME)
check_function_exists(chown HAVE_CHOWN)
check_function_exists(confstr HAVE_CONFSTR)
check_function_exists(fork HAVE_FORK)
check_function_exists(geteuid HAVE_GETEUID)
check_function_exists(getuid HAVE_GETUID)
check_function_exists(memcmp HAVE_MEMCMP)
check_function_exists(memmove HAVE_MEMMOVE)
check_function_exists(resolvepath HAVE_RESOLVEPATH)
check_function_exists(setenv HAVE_SETENV)
check_function_exists(setreuid HAVE_SETREUID)
check_function_exists(setuid HAVE_SETUID)
check_function_exists(statvfs HAVE_STATVFS)
check_function_exists(strerror HAVE_STRERROR)
check_function_exists(utime HAVE_UTIME)
check_function_exists(vfork HAVE_VFORK)

check_include_files(alloca.h HAVE_ALLOCA_H)
check_include_files(dirent.h HAVE_DIRENT_H)
check_include_files(float.h HAVE_FLOAT_H)
check_include_files(limits.h HAVE_LIMITS_H)
check_include_files(mcheck.h HAVE_MCHECK_H)
check_include_files(sys/mman.h HAVE_SYS_MMAN_H)
check_include_files(sys/param.h HAVE_SYS_PARAM_H)
check_include_files(sys/statvfs.h HAVE_SYS_STATVFS_H)
check_include_files(sys/utime.h HAVE_SYS_UTIME_H)
check_include_files(sys/utsname.h HAVE_SYS_UTSNAME_H)
check_include_files(sys/wait.h HAVE_SYS_WAIT_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(utime.h HAVE_UTIME_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(stdint.h HAVE_STDINT_H)

if(HAVE_LIMITS_H)
  set(STDC_HEADERS)
endif(HAVE_LIMITS_H)

try_compile(HAVE_STRUCT_DIRENT_D_TYPE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/cmake/modules/HAVE_STRUCT_DIRENT_D_TYPE.c
)

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
## change default install prefix

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NATIVE_WINDOWS)
  SET(CMAKE_INSTALL_PREFIX
    "C:/MiKTeX-Builds/${MIKTEX_SERIES_STR}"
    CACHE PATH "MiKTeX install prefix." FORCE
  )
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NATIVE_WINDOWS)

###############################################################################
## set date/time info

execute_process(
  COMMAND ${DATE_EXE}
  OUTPUT_VARIABLE MIKTEX_DATETIME_STR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

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

if(NATIVE_WINDOWS)
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
else(NATIVE_WINDOWS)
  add_definitions(-DMIKTEX_UNIX)
endif(NATIVE_WINDOWS)

###############################################################################
## add global include directories

include_directories(
  ${CMAKE_BINARY_DIR}/${rel_core_dir}/include
  ${CMAKE_BINARY_DIR}/${rel_texmf_dir}/include
  ${CMAKE_BINARY_DIR}/${rel_tiff_dir}
  ${CMAKE_SOURCE_DIR}/${rel_app_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_dib_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_dvi_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}
  ${CMAKE_SOURCE_DIR}/${rel_freetype_dir}/lib
  ${CMAKE_SOURCE_DIR}/${rel_gd_dir}
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}
  ${CMAKE_SOURCE_DIR}/${rel_jpeg_dir}
  ${CMAKE_SOURCE_DIR}/${rel_kpsemu_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_md5_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}
  ${CMAKE_SOURCE_DIR}/${rel_netpbm_dir}
  ${CMAKE_SOURCE_DIR}/${rel_netpbm_dir}/pbm
  ${CMAKE_SOURCE_DIR}/${rel_netpbm_dir}/pgm
  ${CMAKE_SOURCE_DIR}/${rel_netpbm_dir}/pnm
  ${CMAKE_SOURCE_DIR}/${rel_netpbm_dir}/ppm
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_tiff_dir}
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}
)

if(NATIVE_WINDOWS)
  include_directories(
    ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/include
  )
endif(NATIVE_WINDOWS)

if(BUILDING_MIKTEX AND MFC_FOUND)
  include_directories(
    ${CMAKE_SOURCE_DIR}/${rel_ui_mfc_dir}/include
  )
endif(BUILDING_MIKTEX AND MFC_FOUND)

###############################################################################
## add global link direcories

link_directories(${LIBRARY_OUTPUT_PATH})

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
  configure_file(
    ${tpm}
    "${CMAKE_BINARY_DIR}/${rel_tpm_dir}/${fn}-${MIKTEX_SERIES_STR}.tpm"
  )
endforeach(tpm)

###############################################################################
## add sub-directories

add_subdirectory(${rel_build_tools_etc_dir})

add_subdirectory(${rel_bzip2_dir})
if(NATIVE_WINDOWS)
  add_subdirectory(${rel_bzip2_dir}-static)
endif(NATIVE_WINDOWS)

if(NATIVE_WINDOWS)
  add_subdirectory(${rel_curl_dir})
  add_subdirectory(${rel_curl_dir}-static)
endif(NATIVE_WINDOWS)

add_subdirectory(${rel_expat_dir})
if(NATIVE_WINDOWS)
  add_subdirectory(${rel_expat_dir}-static)
endif(NATIVE_WINDOWS)

if(NATIVE_WINDOWS)
  add_subdirectory(${rel_gnu_dir})
  add_subdirectory(${rel_gnu_dir}-static)
endif(NATIVE_WINDOWS)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_jpeg_dir})
endif(BUILDING_MIKTEX)

add_subdirectory(${rel_mspack_dir})
if(NATIVE_WINDOWS)
  add_subdirectory(${rel_mspack_dir}-static)
endif(NATIVE_WINDOWS)

add_subdirectory(${rel_popt_dir})
if(NATIVE_WINDOWS)
  add_subdirectory(${rel_popt_dir}-static)
endif(NATIVE_WINDOWS)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_tiff_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_netpbm_dir})
endif(BUILDING_MIKTEX)

add_subdirectory(${rel_zlib_dir})
if(NATIVE_WINDOWS)
  add_subdirectory(${rel_zlib_dir}-static)
endif(NATIVE_WINDOWS)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_png_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_bmeps_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_gd_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_xpdf_goo_dir})
  add_subdirectory(${rel_xpdf_fofi_dir})
  add_subdirectory(${rel_xpdf_xpdf_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_c4p_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_cweb_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_tie_dir})
endif(BUILDING_MIKTEX)

add_subdirectory(${rel_md5_dir})
if(NATIVE_WINDOWS)
  add_subdirectory(${rel_md5_dir}-static)
endif(NATIVE_WINDOWS)

add_subdirectory(${rel_core_dir})
if(NATIVE_WINDOWS)
  add_subdirectory(${rel_core_dir}-static)
endif(NATIVE_WINDOWS)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_cfg_dir})
endif(BUILDING_MIKTEX)

add_subdirectory(${rel_mpmlib_dir})
if(NATIVE_WINDOWS)
  add_subdirectory(${rel_mpmlib_dir}-static)
endif(NATIVE_WINDOWS)

if(BUILDING_MIKTEX AND MFC_FOUND)
  add_subdirectory(${rel_ui_mfc_dir})
endif(BUILDING_MIKTEX AND MFC_FOUND)

add_subdirectory(${rel_app_dir})

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_texmf_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dib_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvi_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_kpsemu_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_web_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX AND MFC_FOUND)
  add_subdirectory(${rel_mpm_mfc_dir})
endif(BUILDING_MIKTEX AND MFC_FOUND)

add_subdirectory(${rel_mpm_dir})

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_makex_dir})
endif(BUILDING_MIKTEX)

add_subdirectory(${rel_initexmf_dir})

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_config_dir})
endif(BUILDING_MIKTEX)

add_subdirectory(${rel_findtexmf_dir})

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mkfntmap_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX AND MFC_FOUND)
  add_subdirectory(${rel_mo_mfc_dir})
endif(BUILDING_MIKTEX AND MFC_FOUND)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_programs_etc_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_tex_etc_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_texware_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_tex_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_pdftex_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_pdfetex_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_omegaware_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_otps_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_omega_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mfware_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mf_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mpware_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mp_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_makeindex_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_bibtex_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_bibtex8_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_texify_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_texinfo_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_tex4ht_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mtprint_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_epstopdf_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_pnmtopng_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_psutils_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_cjkutils_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_freetype_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_gsf2pk_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_ps2pk_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvicopy_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvipdfm_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvipdfmx_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvipng_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvips_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX AND NATIVE_WINDOWS)
  add_subdirectory(${rel_arctrl_win_dir})
endif(BUILDING_MIKTEX AND NATIVE_WINDOWS)

if(BUILDING_MIKTEX AND MFC_FOUND)
  add_subdirectory(${rel_yap_mfc_dir})
endif(BUILDING_MIKTEX AND MFC_FOUND)
