## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2008 Christian Schenk
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

set(CMAKE_COLOR_MAKEFILE FALSE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

cmake_minimum_required(VERSION 2.4.6 FATAL_ERROR)

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
set(top_srcdir "${CMAKE_SOURCE_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

include(miktex-version.cmake)

set(MIKTEX_SERIES_STR
    "${MIKTEX_MAJOR_VERSION}.${MIKTEX_MINOR_VERSION}")

set(MIKTEX_SERIES_ID_STR
    "${MIKTEX_MAJOR_VERSION}_${MIKTEX_MINOR_VERSION}")

math(
  EXPR
  MIKTEX_SERIES_INT
  "(100 * ${MIKTEX_MAJOR_VERSION}) + ${MIKTEX_MINOR_VERSION}"
)

if(MIKTEX_RELEASE_STATE EQUAL 0)
  set(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR}"
      CACHE STRING
      "The MikTeX version string."
      FORCE)
  set(MIKTEX_FULL_VERSION_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
      CACHE STRING
      "The full MikTeX version tag string."
      FORCE)
  set(MIKTEX_VERSION_TAG_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
      CACHE STRING
      "The MikTeX version tag string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 1)
  set(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR} RC ${MIKTEX_RELEASE_NUM}"
      CACHE STRING
      "The MikTeX version string."
      FORCE)
  set(MIKTEX_VERSION_TAG_STR
      "${MIKTEX_SERIES_STR}-rc-${MIKTEX_RELEASE_NUM}"
      CACHE STRING
      "The MikTeX version tag string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 2)
  set(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR} Beta ${MIKTEX_RELEASE_NUM}"
      CACHE STRING
      "The MikTeX version string."
      FORCE)
  set(MIKTEX_VERSION_TAG_STR
      "${MIKTEX_SERIES_STR}-beta-${MIKTEX_RELEASE_NUM}"
      CACHE STRING
      "The MikTeX version tag string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 3)
  SET(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION} snapshot"
      CACHE STRING
      "The MikTeX version string."
      FORCE)
  SET(MIKTEX_VERSION_TAG_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}-snapshot"
      CACHE STRING
      "The MikTeX version tag string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 4)
  SET(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION} debug"
      CACHE STRING
      "The MikTeX version string."
      FORCE)
  SET(MIKTEX_VERSION_TAG_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}-debug"
      CACHE STRING
      "The MikTeX version tag string."
      FORCE)
endif(MIKTEX_RELEASE_STATE EQUAL 0)

if(NOT MIKTEX_FULL_VERSION_STR)
  set(MIKTEX_FULL_VERSION_STR
      "${MIKTEX_VERSION_STR}"
      CACHE STRING
      "The full MikTeX version string."
      FORCE)
endif(NOT MIKTEX_FULL_VERSION_STR)

include(CPack)

include(Admin/COM/guids.cmake)

include(AddLinkFlags)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(CreateCppFromC)
include(CreateDllWrapper)
include(CreateExeAlias)
include(CreateWebApp)
include(DefaultCharTypeIsUnsigned)
include(DelayLoad)
include(DisableRuntimeChecks)
include(InstallPaths)
include(LibraryNames)
include(MergeTrustInfoManifest)
include(MergeManifests)
include(MergeCommonControlsManifest)
include(Rebase)
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

set(required_programs
  cat
  cp
  date
  lynx
  perl
  sed
  touch
  xsltproc
)

if(BUILDING_MIKTEX)
  set(required_programs ${required_programs}
    bison
    diff
    dos2unix
    flex
    texify
  )
  if(NATIVE_WINDOWS)
    set(required_programs ${required_programs}
      hhc
      makehm
      mc
      midl
      mt
    )
  endif(NATIVE_WINDOWS)
endif(BUILDING_MIKTEX)

foreach(p ${required_programs})
  string(TOUPPER "${p}" p_u)
  find_program(${p_u}_EXE NAMES ${p})
  if(NOT ${p_u}_EXE)
    message(FATAL_ERROR "The program ${p} could not be found.")
  endif(NOT ${p_u}_EXE)
endforeach(p)

if(BUILDING_MIKTEX_TOOLS)
  find_program(KPSEWHICH_EXE NAMES kpsewhich)
  if(KPSEWHICH_EXE AND NOT MIKTEX_ROOTS)
    execute_process(
      COMMAND kpsewhich --expand-path \$TEXMF
      OUTPUT_VARIABLE MIKTEX_ROOTS
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  endif(KPSEWHICH_EXE AND NOT MIKTEX_ROOTS)
  if(KPSEWHICH_EXE AND NOT MIKTEX_INSTALLROOT)
    execute_process(
      COMMAND kpsewhich --expand-path \$TEXMFLOCAL
      OUTPUT_VARIABLE MIKTEX_INSTALLROOT
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  endif(KPSEWHICH_EXE AND NOT MIKTEX_INSTALLROOT)
endif(BUILDING_MIKTEX_TOOLS)

find_package(ImageMagick REQUIRED)
find_package(MFC)

find_package(Qt4)
find_package(Threads REQUIRED)

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
check_function_exists(mmap HAVE_MMAP)
check_function_exists(resolvepath HAVE_RESOLVEPATH)
check_function_exists(setenv HAVE_SETENV)
check_function_exists(setreuid HAVE_SETREUID)
check_function_exists(setuid HAVE_SETUID)
check_function_exists(statvfs HAVE_STATVFS)
check_function_exists(strerror HAVE_STRERROR)
check_function_exists(uname HAVE_UNAME_SYSCALL)
check_function_exists(utime HAVE_UTIME)
check_function_exists(vfork HAVE_VFORK)

check_include_file_cxx(afxwin.h HAVE_AFXWIN_H "-D_AFXDLL")
check_include_file_cxx(atlbase.h HAVE_ATLBASE_H)

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

# work around CMake bug
if(MFC_FOUND AND NOT HAVE_AFXWIN_H)
  set(MFC_FOUND "NO")
endif(MFC_FOUND AND NOT HAVE_AFXWIN_H)

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
  add_definitions(-DMIKTEX_DEBUG)
elseif(MIKTEX_RELEASE_STATE EQUAL 3)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "The build type." FORCE)
  add_definitions(-DMIKTEX_DEBUG)
elseif(MIKTEX_RELEASE_STATE EQUAL 4)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type." FORCE)
  add_definitions(-DMIKTEX_DEBUG)
endif(MIKTEX_RELEASE_STATE EQUAL 0)

mark_as_advanced(CMAKE_BUILD_TYPE)

###############################################################################
## add global definitions

add_definitions(-DHAVE_CONFIG_H)

add_definitions(-DMIKTEX)

if(NATIVE_WINDOWS)
  add_definitions(
    -DMIKTEX_WINDOWS
  )
  if(MFC_FOUND AND HAVE_ATLBASE_H)
    add_definitions(
      -DMIKTEX_ATLMFC
    )
  endif(MFC_FOUND AND HAVE_ATLBASE_H)
  # DLL export enablers
  add_definitions(
    -DZLIB_DLL
  )
  # use overloads for unsecure functions
  #add_definitions(
  #  -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1
  #)
  # Windows version macros
#  add_definitions(
#    -DWINVER=0x0400
#    -D_WIN32_WINDOWS=0x0400
#    -D_WIN32_WINNT=0x0400
#    -D_WIN32_IE=0x0500
#  )
else(NATIVE_WINDOWS)
  add_definitions(-DMIKTEX_UNIX)
endif(NATIVE_WINDOWS)

set(CMAKE_RC_FLAGS
  "-I${CMAKE_SOURCE_DIR}/Resources/Graphics -I${CMAKE_SOURCE_DIR}/Resources/Videos")

###############################################################################
## add global include directories

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/${rel_core_dir}/include
  ${CMAKE_BINARY_DIR}/${rel_kpsemu_dir}/include
  ${CMAKE_BINARY_DIR}/${rel_md5_dir}/include
  ${CMAKE_BINARY_DIR}/${rel_mpmlib_dir}/include
  ${CMAKE_BINARY_DIR}/${rel_texmf_dir}/include
  ${CMAKE_BINARY_DIR}/${rel_tiff_dir}
  ${CMAKE_BINARY_DIR}/${rel_unxemu_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}
  ${CMAKE_SOURCE_DIR}/${rel_app_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_dib_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_dvi_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_fontconfig_dir}/fontconfig
  ${CMAKE_SOURCE_DIR}/${rel_fontconfig_dir}
  ${CMAKE_SOURCE_DIR}/${rel_freetype2_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_gd_dir}
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}
  ${CMAKE_SOURCE_DIR}/${rel_graphite_engine_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_icu_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_jpeg_dir}
  ${CMAKE_SOURCE_DIR}/${rel_kpsemu_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_lzmalib_dir}/include
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
  ${CMAKE_SOURCE_DIR}/${rel_teckit_dir}/source/Public-headers
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_tiff_dir}
  ${CMAKE_SOURCE_DIR}/${rel_unxemu_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}
)

if(NATIVE_WINDOWS)
  include_directories(
    ${CMAKE_SOURCE_DIR}/${rel_curl_dir}/include
    ${CMAKE_SOURCE_DIR}/${rel_unxemu_dir}/include
  )
endif(NATIVE_WINDOWS)

if(QT4_FOUND)
  include_directories(
    ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/include
  )
endif(QT4_FOUND)

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

file(TO_NATIVE_PATH ${CMAKE_SOURCE_DIR} CMAKE_SOURCE_DIR_NATIVE)
file(TO_NATIVE_PATH ${CMAKE_BINARY_DIR} CMAKE_BINARY_DIR_NATIVE)

configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake
               ${CMAKE_BINARY_DIR}/config.h)

configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in
               ${CMAKE_BINARY_DIR}/DoxyFile)

configure_file(${CMAKE_SOURCE_DIR}/Admin/Scripts/Doxyfile.sdk.in
               ${CMAKE_BINARY_DIR}/Admin/Scripts/DoxyFile.sdk)

configure_file(${CMAKE_SOURCE_DIR}/Documentation/Styles/db2latex-common.xsl.in
               ${CMAKE_BINARY_DIR}/Documentation/Styles/db2latex-common.xsl)

configure_file(${CMAKE_SOURCE_DIR}/BuildUtilities/docbook/catalog.in
               ${CMAKE_BINARY_DIR}/BuildUtilities/docbook/catalog)

configure_file(${CMAKE_SOURCE_DIR}/Admin/Scripts/makesdk.cmd.in
	       ${CMAKE_BINARY_DIR}/Admin/Scripts/makesdk.cmd)

configure_file(${CMAKE_SOURCE_DIR}/Admin/Scripts/makelocfile.cmd.in
	       ${CMAKE_BINARY_DIR}/Admin/Scripts/makelocfile.cmd)

configure_file(${CMAKE_SOURCE_DIR}/Admin/Scripts/makesrc.cmd.in
	       ${CMAKE_BINARY_DIR}/Admin/Scripts/makesrc.cmd)

configure_file(${CMAKE_SOURCE_DIR}/Admin/Scripts/produce.cmd.in
	       ${CMAKE_BINARY_DIR}/Admin/Scripts/produce.cmd)

configure_file(${CMAKE_SOURCE_DIR}/SDK/doc/mainpage.doxygen.in
	       ${CMAKE_BINARY_DIR}/SDK/doc/mainpage.doxygen)

configure_file(${CMAKE_SOURCE_DIR}/SDK/samples/c++/findfile/findfile.cpp.in
	       ${CMAKE_BINARY_DIR}/SDK/samples/c++/findfile/findfile.cpp)

configure_file(${CMAKE_SOURCE_DIR}/SDK/samples/c++/findpk/findpk.cpp.in
	       ${CMAKE_BINARY_DIR}/SDK/samples/c++/findpk/findpk.cpp)

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

if(NATIVE_WINDOWS)
  add_subdirectory(${rel_icu_dir})
endif(NATIVE_WINDOWS)

add_subdirectory(${rel_lzmalib_dir})

if(NATIVE_WINDOWS)
  add_subdirectory(${rel_lzmalib_dir}-static)
endif(NATIVE_WINDOWS)

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

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_freetype2_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_graphite_engine_dir}-static)
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_graphite_engine_dir})
endif(BUILDING_MIKTEX)

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

if(NATIVE_WINDOWS)
  add_subdirectory(${rel_teckit_dir})
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

add_subdirectory(${rel_extractor_dir})

if(NATIVE_WINDOWS)
  add_subdirectory(${rel_extractor_dir}-static)
endif(NATIVE_WINDOWS)

add_subdirectory(${rel_mpmlib_dir})

if(NATIVE_WINDOWS)
  add_subdirectory(${rel_mpmlib_dir}-static)
endif(NATIVE_WINDOWS)

if(QT4_FOUND)
  add_subdirectory(${rel_ui_qt_dir})
endif(QT4_FOUND)

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

if(BUILDING_MIKTEX AND NATIVE_WINDOWS)
  add_subdirectory(${rel_unxemu_dir})
endif(BUILDING_MIKTEX AND NATIVE_WINDOWS)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_fontconfig_dir})
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

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_findtexmf_dir})
endif(BUILDING_MIKTEX)

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
  add_subdirectory(${rel_copystart_dir})
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
  add_subdirectory(${rel_xetex_dir})
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
  add_subdirectory(${rel_xdvipdfmx_dir})
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

add_subdirectory(${rel_md5walk_dir})

add_subdirectory(${rel_mthelp_dir})

add_subdirectory(${rel_mpc_dir})

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_padout_dir})
endif(BUILDING_MIKTEX)

if(BUILDING_MIKTEX AND HAVE_ATLBASE_H)
  add_subdirectory(${rel_migrate_dir})
endif(BUILDING_MIKTEX AND HAVE_ATLBASE_H)

if(BUILDING_MIKTEX AND MFC_FOUND)
  add_subdirectory(${rel_setup_mfc_dir})
endif(BUILDING_MIKTEX AND MFC_FOUND)

if(BUILDING_MIKTEX AND MFC_FOUND)
  add_subdirectory(${rel_update_mfc_dir})
endif(BUILDING_MIKTEX AND MFC_FOUND)

if(BUILDING_MIKTEX AND MFC_FOUND)
  add_subdirectory(${rel_remove_mfc_dir})
endif(BUILDING_MIKTEX AND MFC_FOUND)

add_subdirectory(${rel_doc_dir})

add_subdirectory(${rel_mtrep_dir} EXCLUDE_FROM_ALL)
