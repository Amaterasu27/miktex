## CMakeLists.txt: top-level CMake file				 -*-
##
## Copyright (C) 2006-2015 Christian Schenk
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

cmake_minimum_required(VERSION 3.2.1 FATAL_ERROR)

set(CMAKE_COLOR_MAKEFILE FALSE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

option(
  MIKTEX_SUPPORT_WINDOWS_XP
  "Support Windows XP."
  ${WIN32}
)

if(WIN32 AND NOT UNIX)
  set(NATIVE_WINDOWS 1)
else()
  set(UNIX_ALIKE 1)
endif()

if(NATIVE_WINDOWS)
  project(miktex)
  set(PRODUCT_NAME "MiKTeX")
  set(BUILDING_MIKTEX TRUE)
  if(MIKTEX_SUPPORT_WINDOWS_XP)
    add_definitions(-D_USING_V110_SDK71_)
    if(CMAKE_CL_64)
        SET(CMAKE_CREATE_WIN32_EXE /SUBSYSTEM:WINDOWS,5.02)
        SET(CMAKE_CREATE_CONSOLE_EXE /SUBSYSTEM:CONSOLE,5.02)
    else()
        SET(CMAKE_CREATE_WIN32_EXE /SUBSYSTEM:WINDOWS,5.01)
        SET(CMAKE_CREATE_CONSOLE_EXE /SUBSYSTEM:CONSOLE,5.01)
    endif()
  endif()
else()
  project(miktex-tools)
  set(PRODUCT_NAME "MiKTeX Tools")
  set(BUILDING_MIKTEX_TOOLS TRUE)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "BSD/OS")
  set(system_name "openbsd")
elseif(CMAKE_SYSTEM_NAME MATCHES "HP-UX")
  set(system_name "hpux")
elseif(CMAKE_SYSTEM_NAME MATCHES "GNU/kFreeBSD")
  set(system_name "freebsd")
elseif(CMAKE_SYSTEM_NAME MATCHES "SCO_SV")
  set(system_name "scosv")
elseif(CMAKE_SYSTEM_NAME MATCHES "UNIX_SV")
  set(system_name "unixsv")
elseif(CMAKE_SYSTEM_NAME MATCHES "CYGWIN_NT-5.1")
  set(system_name "cygwin")
else()
  string(TOLOWER "${CMAKE_SYSTEM_NAME}" system_name)
endif()

if(NATIVE_WINDOWS)
  if(CMAKE_CL_64)
    set(system_cpu "x64")
    set(manifest_processor_architecture "amd64")
  else()
    set(system_cpu "x86")
    set(manifest_processor_architecture "x86")
  endif()
else()
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "i.86")
    set(system_cpu "x86")
  else()
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" system_cpu)
  endif()
endif()

#set(target_system_tag "${system_cpu}-${system_name}")
set(target_system_tag "${system_name}-${system_cpu}")

set(miktex_prefix "miktex-")
set(miktex_admin_suffix "_admin")

enable_testing()

option(
  USE_SYSTEM_BZIP2
  "Use the system bzip2 library."
  ${UNIX_ALIKE}
)

option(
  USE_SYSTEM_CURL
  "Use the system curl library."
  ${UNIX_ALIKE}
)

option(
  USE_SYSTEM_EXPAT
  "Use the system expat library."
  ${UNIX_ALIKE}
)

option(
  USE_SYSTEM_PNG
  "Use the system png library."
  ${UNIX_ALIKE}
)

option(
  USE_SYSTEM_URIPARSER
  "Use the system uriparser library."
  ${UNIX_ALIKE}
)

option(
  USE_SYSTEM_ZLIB
  "Use the system zlib library."
  ${UNIX_ALIKE}
)

option(
  INSTALL_PACKAGE_DATABASE
  "Install the package database."
  FALSE
)

option(
  WITH_UI_QT
  "Build Qt components."
  TRUE
)

option(
  WITH_UI_MFC
  "Build MFC components."
  ${WIN32}
)

option(
  WITH_MPC
  "Build mpc (MiKTeX Package Composer)?"
  ${NATIVE_WINDOWS}
)

option(
  WITH_PDFTEX
  "Build pdfTeX?"
  TRUE
)

option(
  WITH_SYNCTEX
  "Enable SyncTeX extensions."
  TRUE
)

option(
  LINK_EVERYTHING_STATICALLY
  "Don't use shared objects."
  ${UNIX_ALIKE}
)

option(
  INSTALL_MIKTEX_HEADERS
  "Install the MiKTeX header files."
  FALSE
)

option(
  INSTALL_STATIC_LIBRARIES
  "Install static libraries."
  FALSE
)

option(
  WITH_CONTEXT_SUPPORT
  "Enable ConTeXt support."
  FALSE
)

## check all dyn.sed files, if you alter C4P_VAR_NAME_PREFIX
set(C4P_VAR_NAME_PREFIX "g_")

set(outdir "${CMAKE_BINARY_DIR}/binlib")
set(EXECUTABLE_OUTPUT_PATH "${outdir}")
set(LIBRARY_OUTPUT_PATH "${outdir}")

## autotools compatibility names
set(PACKAGE_NAME ${PRODUCT_NAME})
set(top_srcdir "${CMAKE_SOURCE_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

include(miktex-version.cmake)
include(miktex-local-version.cmake OPTIONAL)

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
      "The MiKTeX version string."
      FORCE)
  set(MIKTEX_FULL_VERSION_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
      CACHE STRING
      "The full MiKTeX version tag string."
      FORCE)
  set(MIKTEX_VERSION_TAG_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
      CACHE STRING
      "The MiKTeX version tag string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 1)
  set(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR} RC ${MIKTEX_RELEASE_NUM}"
      CACHE STRING
      "The MiKTeX version string."
      FORCE)
  set(MIKTEX_VERSION_TAG_STR
      "${MIKTEX_SERIES_STR}-rc-${MIKTEX_RELEASE_NUM}"
      CACHE STRING
      "The MiKTeX version tag string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 2)
  set(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR} Beta ${MIKTEX_RELEASE_NUM}"
      CACHE STRING
      "The MiKTeX version string."
      FORCE)
  set(MIKTEX_VERSION_TAG_STR
      "${MIKTEX_SERIES_STR}-beta-${MIKTEX_RELEASE_NUM}"
      CACHE STRING
      "The MiKTeX version tag string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 3)
  set(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION} snapshot"
      CACHE STRING
      "The MiKTeX version string."
      FORCE)
  set(MIKTEX_VERSION_TAG_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}-snapshot"
      CACHE STRING
      "The MiKTeX version tag string."
      FORCE)
elseif(MIKTEX_RELEASE_STATE EQUAL 4)
  set(MIKTEX_VERSION_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION} debug"
      CACHE STRING
      "The MiKTeX version string."
      FORCE)
  set(MIKTEX_VERSION_TAG_STR
      "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}-debug"
      CACHE STRING
      "The MiKTeX version tag string."
      FORCE)
endif()

if(NOT MIKTEX_FULL_VERSION_STR)
  set(MIKTEX_FULL_VERSION_STR
      "${MIKTEX_VERSION_STR}"
      CACHE STRING
      "The full MiKTeX version string."
      FORCE)
endif()

include(Admin/COM/guids.cmake)

include(AddLinkFlags)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckFunctionExists)
include(CheckIncludeFileCXX)
include(CheckVariableExists)
include(CreateDllWrapper)
include(CreateExeAlias)
include(CreateWebApp)
include(DefaultCharTypeIsUnsigned)
include(DelayLoad)
include(DisableRuntimeChecks)
include(InstallPaths)
include(LibraryNames)
include(MergeCommonControlsManifest)
include(MergeManifests)
include(MergeTrustInfoManifest)
include(Rebase)
include(TestBigEndian)
include(SourcePaths)
include(UseStaticCRT)
include(UseStaticLibraries)

if(NATIVE_WINDOWS)
  set(dev_null "nul")
else()
  set(dev_null "/dev/null")
endif()

###############################################################################
## find/check stuff

set(required_programs
  bison
  cat
  cp
  date
  diff
  flex
  lynx
  perl
  sed
  tr
  xsltproc
)

if(BUILDING_MIKTEX)
  set(required_programs ${required_programs}
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
  endif()
endif()

foreach(p ${required_programs})
  string(TOUPPER "${p}" p_u)
  find_program(${p_u}_EXE NAMES ${p})
  if(NOT ${p_u}_EXE)
    message(FATAL_ERROR "The program ${p} could not be found.")
  endif()
endforeach()

if(BUILDING_MIKTEX_TOOLS)
  set(MIKTEX_TEXMF "${texmfdir}")
endif()

find_package(ImageMagick REQUIRED)

if(WITH_UI_QT)
  find_package(Qt5 REQUIRED
    Core
    Gui
    PrintSupport
    Script
    ScriptTools
    UiTools
    Widgets
    WinExtras
    Xml
  )
endif()

find_package(Threads REQUIRED)

if(USE_SYSTEM_CURL)
  find_package(CURL REQUIRED)
endif()

if(USE_SYSTEM_BZIP2)
  find_package(BZip2 REQUIRED)
endif()

if(USE_SYSTEM_EXPAT)
  find_package(EXPAT REQUIRED)
endif()

if(USE_SYSTEM_PNG)
  find_package(PNG REQUIRED)
endif()

if(USE_SYSTEM_URIPARSER)
  find_package(URIPARSER REQUIRED)
endif()

if(USE_SYSTEM_ZLIB)
  find_package(ZLIB REQUIRED)
endif()

#find_package(ICU REQUIRED)

if(UNIX_ALIKE)
  set(CMAKE_REQUIRED_LIBRARIES "m")
endif()

if(NOT MSVC)
  check_c_compiler_flag(-fvisibility=hidden HAVE_C_VISIBILITY_OPTION)
  check_cxx_compiler_flag(-fvisibility=hidden HAVE_CXX_VISIBILITY_OPTION)
endif()

if(MSVC)
  set(HAVE_PRAGMA_ONCE TRUE)
endif()

test_big_endian(MIKTEX_WORDS_BIGENDIAN)

check_function_exists(canonicalize_file_name HAVE_CANONICALIZE_FILE_NAME)
check_function_exists(chown HAVE_CHOWN)
check_function_exists(close HAVE_CLOSE)
check_function_exists(confstr HAVE_CONFSTR)
check_function_exists(fork HAVE_FORK)
check_function_exists(fseeko64 HAVE_FSEEKO64)
check_function_exists(ftello64 HAVE_FTELLO64)
check_function_exists(getenv HAVE_GETENV)
check_function_exists(geteuid HAVE_GETEUID)
check_function_exists(getuid HAVE_GETUID)
check_function_exists(lsqrt HAVE_LSQRT)
check_function_exists(memcmp HAVE_MEMCMP)
check_function_exists(memmove HAVE_MEMMOVE)
check_function_exists(mmap HAVE_MMAP)
check_function_exists(resolvepath HAVE_RESOLVEPATH)
check_function_exists(round HAVE_ROUND)
check_function_exists(setenv HAVE_SETENV)
check_function_exists(setreuid HAVE_SETREUID)
check_function_exists(setuid HAVE_SETUID)
check_function_exists(statvfs HAVE_STATVFS)
check_function_exists(strerror HAVE_STRERROR)
check_function_exists(system HAVE_SYSTEM)
check_function_exists(trunc HAVE_TRUNC)
check_function_exists(uname HAVE_UNAME_SYSCALL)
check_function_exists(utime HAVE_UTIME)
check_function_exists(vfork HAVE_VFORK)

if(WIN32)
  ## ATL is mandatory
  #check_include_file_cxx(afxwin.h HAVE_AFXWIN_H)
  #check_include_file_cxx(atlbase.h HAVE_ATLBASE_H)
  set(HAVE_AFXWIN_H "YES")
  set(HAVE_ATLBASE_H "YES")
endif()

check_include_file_cxx(hash_map HAVE_HASH_MAP)
check_include_file_cxx(hash_set HAVE_HASH_SET)
check_include_file_cxx(unordered_map HAVE_UNORDERED_MAP)
check_include_file_cxx(unordered_set HAVE_UNORDERED_SET)

check_include_files(alloca.h HAVE_ALLOCA_H)
check_include_files(dirent.h HAVE_DIRENT_H)
check_include_files(errno.h HAVE_ERRNO_H)
check_include_files(float.h HAVE_FLOAT_H)
check_include_files(getopt.h HAVE_GETOPT_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(limits.h HAVE_LIMITS_H)
check_include_files(mcheck.h HAVE_MCHECK_H)
check_include_files(memory.h HAVE_MEMORY_H)
check_include_files(regex.h HAVE_REGEX_H)
check_include_files(stddef.h HAVE_STDDEF_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(sys/mman.h HAVE_SYS_MMAN_H)
check_include_files(sys/param.h HAVE_SYS_PARAM_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/statvfs.h HAVE_SYS_STATVFS_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(sys/utime.h HAVE_SYS_UTIME_H)
check_include_files(sys/utsname.h HAVE_SYS_UTSNAME_H)
check_include_files(sys/wait.h HAVE_SYS_WAIT_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(utime.h HAVE_UTIME_H)

check_variable_exists(timezone HAVE_TIMEZONE)

if(HAVE_LIMITS_H)
  set(STDC_HEADERS)
endif()

try_compile(HAVE_STRUCT_DIRENT_D_TYPE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/cmake/modules/HAVE_STRUCT_DIRENT_D_TYPE.c
)

## MiKTeX components
set(HAVE_FT2BUILD_H 1)
set(HAVE_LIBFONTCONFIG 1)
set(HAVE_LIBFREETYPE 1)
set(HAVE_LIBJPEG 1)
set(HAVE_LIBPNG 1)
set(HAVE_LIBZ 1)
set(HAVE_ZLIB 1)
set(HAVE_ZLIB_H 1)
set(HAVE_ZLIB_COMPRESS2 1)

if(MSVC)
  if(MSVC14)
    set(vc_runtime 140)
  elseif(MSVC12)
    set(vc_runtime 120)
  elseif(MSVC11)
    set(vc_runtime 110)
  elseif(MSVC10)
    set(vc_runtime 100)
  endif()
endif()

###############################################################################
## change default install prefix

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NATIVE_WINDOWS)
  SET(CMAKE_INSTALL_PREFIX
    "C:/MiKTeX-Builds/${MIKTEX_SERIES_STR}"
    CACHE PATH "MiKTeX install prefix." FORCE
  )
endif()

###############################################################################
## set date/time info

execute_process(
  COMMAND ${DATE_EXE}
  OUTPUT_VARIABLE MIKTEX_DATETIME_STR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

###############################################################################
## set the build type
##
## the build type is derived from the MiKTeX release state; see
## miktex-version.cmake

if(MIKTEX_RELEASE_STATE EQUAL 0)
  if(NATIVE_WINDOWS)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "The build type." FORCE)
  else()
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type." FORCE)
  endif()
elseif(MIKTEX_RELEASE_STATE EQUAL 1)
  if(NATIVE_WINDOWS)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "The build type." FORCE)
  else()
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type." FORCE)
  endif()
elseif(MIKTEX_RELEASE_STATE EQUAL 2)
  if(NATIVE_WINDOWS)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "The build type." FORCE)
    add_definitions(-DMIKTEX_DEBUG)
  else()
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type." FORCE)
  endif()
elseif(MIKTEX_RELEASE_STATE EQUAL 3)
  if(NATIVE_WINDOWS)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "The build type." FORCE)
    add_definitions(-DMIKTEX_DEBUG)
  else()
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type." FORCE)
  endif()
elseif(MIKTEX_RELEASE_STATE EQUAL 4)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type." FORCE)
  add_definitions(-DMIKTEX_DEBUG)
endif()

mark_as_advanced(CMAKE_BUILD_TYPE)

###############################################################################
## add global definitions

add_definitions(-DHAVE_CONFIG_H)

add_definitions(-DMIKTEX)

if(WITH_UI_QT)
  add_definitions(
    -DMIKTEX_QT
  )
endif()

if(NATIVE_WINDOWS)
  add_definitions(
    -DMIKTEX_WINDOWS
  )
  ## better bind to latest CRT/MFC DLLs if redistributing as private
  ## assemblies
  add_definitions(
    -D_BIND_TO_CURRENT_ATL_VERSION=1
    -D_BIND_TO_CURRENT_CRT_VERSION=1
    -D_BIND_TO_CURRENT_MFC_VERSION=1
  )
  if(WITH_UI_MFC)
    add_definitions(
      -DMIKTEX_ATLMFC
    )
    set(CMAKE_RC_FLAGS
      "${CMAKE_RC_FLAGS} -DMIKTEX_ATLMFC")
  endif()
  ## use overloads for unsecure functions
  #add_definitions(
  #  -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1
  #)
else()
  add_definitions(-DMIKTEX_UNIX)
endif()

###############################################################################
## 

if(USE_SYSTEM_BZIP2)
  set(bzip2_lib_name ${BZIP2_LIBRARIES})
endif()

if(USE_SYSTEM_EXPAT)
  set(expat_lib_name ${EXPAT_LIBRARIES})
endif()

if(USE_SYSTEM_PNG)
  set(png_lib_name ${PNG_LIBRARIES})
endif()

if(USE_SYSTEM_URIPARSER)
  set(uriparser_lib_name ${URIPARSER_LIBRARIES})
endif()

if(USE_SYSTEM_ZLIB)
  set(zlib_lib_name ${ZLIB_LIBRARIES})
endif()

###############################################################################
## add global include directories

include_directories(
  ${CMAKE_BINARY_DIR}

  # version number, general definitions
  ${CMAKE_BINARY_DIR}/${rel_core_dir}/include
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include
)

if(WITH_UI_MFC OR WITH_UI_QT)
  include_directories(
    ${CMAKE_SOURCE_DIR}/${rel_ui_dir}/include
  )
endif()

if(MSVC)
  include_directories(
    ${CMAKE_SOURCE_DIR}/${rel_msinttypes_dir}/include
  )
endif()

if(USE_SYSTEM_BZIP2)
  include_directories(${BZIP2_INCLUDE_DIR})
endif()

if(USE_SYSTEM_EXPAT)
  include_directories(${EXPAT_INCLUDE_DIRS})
endif()

if(USE_SYSTEM_PNG)
  include_directories(${PNG_INCLUDE_DIRS})
endif()

if(USE_SYSTEM_URIPARSER)
  include_directories(${URIPARSER_INCLUDE_DIR})
endif()

if(USE_SYSTEM_ZLIB)
  include_directories(${ZLIB_INCLUDE_DIR})
endif()

if(USE_SYSTEM_CURL)
  include_directories(${CURL_INCLUDE_DIRS})
endif()

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

configure_file(${CMAKE_SOURCE_DIR}/Admin/Scripts/putj2000version.cmd.in
	       ${CMAKE_BINARY_DIR}/Admin/Scripts/putj2000version.cmd)

configure_file(${CMAKE_SOURCE_DIR}/Resources/Manifests/Common-Controls.manifest.in
	       ${CMAKE_BINARY_DIR}/Resources/Manifests/Common-Controls.manifest)

configure_file(${CMAKE_SOURCE_DIR}/SDK/doc/mainpage.doxygen.in
	       ${CMAKE_BINARY_DIR}/SDK/doc/mainpage.doxygen)

configure_file(${CMAKE_SOURCE_DIR}/SDK/samples/c++/findfile/findfile.cpp.in
	       ${CMAKE_BINARY_DIR}/SDK/samples/c++/findfile/findfile.cpp)

configure_file(${CMAKE_SOURCE_DIR}/SDK/samples/c++/findpk/findpk.cpp.in
	       ${CMAKE_BINARY_DIR}/SDK/samples/c++/findpk/findpk.cpp)

if(NATIVE_WINDOWS AND MIKTEX_RELEASE_STATE EQUAL 4)
  configure_file(
    ${CMAKE_SOURCE_DIR}/miktexstartup.ini.debug.in
	${CMAKE_BINARY_DIR}/miktexstartup.ini.debug)
endif()


###############################################################################
## add sub-directories

add_subdirectory(${rel_tpm_dir})

if(NATIVE_WINDOWS)
  add_subdirectory(${rel_utf8wrap_dir})
  add_subdirectory(${rel_utf8wrap_dir}-static)
endif()

add_subdirectory(${rel_build_tools_etc_dir})

if(NOT USE_SYSTEM_URIPARSER)
  add_subdirectory(${rel_uriparser_dir}-static)
  if(NOT LINK_EVERYTHING_STATICALLY)
    add_subdirectory(${rel_uriparser_dir})
  endif()
endif()

if(NOT LINK_EVERYTHING_STATICALLY)
  add_subdirectory(${rel_lzmalib_dir})
endif()
add_subdirectory(${rel_lzmalib_dir}-static)

if(NOT USE_SYSTEM_BZIP2)
  add_subdirectory(${rel_bzip2_dir}-static)
  if(NOT LINK_EVERYTHING_STATICALLY)
    add_subdirectory(${rel_bzip2_dir})
  endif()
endif()

if(NOT USE_SYSTEM_CURL)
  add_subdirectory(${rel_curl_dir})
  if(NOT LINK_EVERYTHING_STATICALLY)
    add_subdirectory(${rel_curl_dir}-static)
  endif()
endif()

if(NOT USE_SYSTEM_EXPAT)
  add_subdirectory(${rel_expat_dir}-static)
  if(NOT LINK_EVERYTHING_STATICALLY)
    add_subdirectory(${rel_expat_dir})
  endif()
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_botan_dir}-static)
endif()

if(NOT LINK_EVERYTHING_STATICALLY)
  add_subdirectory(${rel_botan_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_freetype2_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_graphite2_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_harfbuzz_dir}-static)
endif()

if(NOT HAVE_GETOPT_H)
  add_subdirectory(${rel_getopt_dir})
  add_subdirectory(${rel_getopt_dir}-static)
endif()

if(NOT HAVE_REGEX_H)
  add_subdirectory(${rel_regex_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_jpeg_dir})
endif()

if(NOT LINK_EVERYTHING_STATICALLY)
  add_subdirectory(${rel_mspack_dir})
endif()
add_subdirectory(${rel_mspack_dir}-static)

add_subdirectory(${rel_obsdcompat_dir})

if(NOT LINK_EVERYTHING_STATICALLY)
  add_subdirectory(${rel_popt_dir})
endif()
add_subdirectory(${rel_popt_dir}-static)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_tiff_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_netpbm_dir})
endif()

if(NOT USE_SYSTEM_ZLIB)
  add_subdirectory(${rel_zlib_dir}-static)
  if(NOT LINK_EVERYTHING_STATICALLY)
    add_subdirectory(${rel_zlib_dir})
  endif()
endif()

add_subdirectory(${rel_zzip_dir})

add_subdirectory(${rel_lua_dir}-static)
if(NOT LINK_EVERYTHING_STATICALLY)
  add_subdirectory(${rel_lua_dir})
endif()

#add_subdirectory(${rel_luajit_dir}-static)
if(NOT LINK_EVERYTHING_STATICALLY)
  add_subdirectory(${rel_luajit_dir})
endif()

if(NATIVE_WINDOWS)
  add_subdirectory(${rel_teckit_dir})
endif()

add_subdirectory(${rel_pixman_dir}-static)
add_subdirectory(${rel_cairo_dir})

if(NOT USE_SYSTEM_PNG)
  add_subdirectory(${rel_png_dir}-static)
  if(NOT LINK_EVERYTHING_STATICALLY)
    add_subdirectory(${rel_png_dir})
  endif()
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_poppler_dir}-static)
endif()

if(BUILDING_MIKTEX AND WITH_UI_QT)
  add_subdirectory(${rel_poppler_dir}/qt5/src)
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_hunspell_dir})
endif()

add_subdirectory(${rel_c4p_dir})

add_subdirectory(${rel_inipool_dir})

if(NOT LINK_EVERYTHING_STATICALLY)
  add_subdirectory(${rel_md5_dir})
endif()
add_subdirectory(${rel_md5_dir}-static)

add_subdirectory(${rel_core_dir})
add_subdirectory(${rel_core_dir}-static)

add_subdirectory(${rel_cfg_dir})

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_touch_dir})
endif()

if(NOT LINK_EVERYTHING_STATICALLY)
  add_subdirectory(${rel_extractor_dir})
endif()
add_subdirectory(${rel_extractor_dir}-static)

add_subdirectory(${rel_mpmlib_dir})
add_subdirectory(${rel_mpmlib_dir}-static)

add_subdirectory(${rel_setuplib_dir})
add_subdirectory(${rel_setuplib_dir}-static)

if(WITH_UI_QT)
  if(NOT LINK_EVERYTHING_STATICALLY)
    add_subdirectory(${rel_ui_qt_dir})
  endif()
## TODO
#  add_subdirectory(${rel_ui_qt_dir}-static)
endif()

if(BUILDING_MIKTEX AND WITH_UI_MFC)
  add_subdirectory(${rel_ui_mfc_dir})
endif()

if(NOT LINK_EVERYTHING_STATICALLY)
  add_subdirectory(${rel_app_dir})
endif()
add_subdirectory(${rel_app_dir}-static)

add_subdirectory(${rel_texmf_dir})
add_subdirectory(${rel_texmf_dir}-static)

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dib_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvi_dir})
endif()

add_subdirectory(${rel_kpsemu_dir})
add_subdirectory(${rel_kpsemu_dir}-static)

add_subdirectory(${rel_w2cemu_dir})

if(BUILDING_MIKTEX AND NATIVE_WINDOWS)
  add_subdirectory(${rel_unxemu_dir})
endif()

add_subdirectory(${rel_cweb_dir})

add_subdirectory(${rel_tie_dir})

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_fontconfig_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_gd_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_bmeps_dir})
endif()

add_subdirectory(${rel_web_dir})

if(BUILDING_MIKTEX AND WITH_UI_MFC)
  add_subdirectory(${rel_mpm_mfc_dir})
endif()

if(WITH_UI_QT)
  add_subdirectory(${rel_mpm_qt_dir})
endif()

add_subdirectory(${rel_mpm_dir})

add_subdirectory(${rel_miktexsetup_dir})

add_subdirectory(${rel_makex_dir})

add_subdirectory(${rel_initexmf_dir})

add_subdirectory(${rel_config_dir})

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_findtexmf_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_kpsewhich_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mkfntmap_dir})
endif()

if(BUILDING_MIKTEX AND WITH_UI_MFC)
  add_subdirectory(${rel_mo_mfc_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_programs_etc_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_copystart_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_tex_etc_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_texware_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_tex_dir})
endif()

if(WITH_PDFTEX)
  add_subdirectory(${rel_pdftex_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_xetex_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_omegaware_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_otps_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_omega_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mfware_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mf_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mp_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_luatex_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_makeindex_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_bibtex_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_bibtexx_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_texify_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_texinfo_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_tex4ht_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_musixflx_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_mtprint_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_epstopdf_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_psutils_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_cjkutils_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_freetype_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_gsf2pk_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_ps2pk_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvicopy_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvipdfmx_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvipng_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_dvips_dir})
endif()

if(BUILDING_MIKTEX AND NATIVE_WINDOWS)
  add_subdirectory(${rel_arctrl_win_dir})
endif()

if(BUILDING_MIKTEX AND NATIVE_WINDOWS)
  add_subdirectory(${rel_taskbar_icon_win_dir})
endif()

if(BUILDING_MIKTEX AND WITH_UI_QT)
  add_subdirectory(${rel_texworks_dir})
endif()

if(BUILDING_MIKTEX AND WITH_UI_MFC)
  add_subdirectory(${rel_yap_mfc_dir})
endif()

add_subdirectory(${rel_md5walk_dir})

add_subdirectory(${rel_mthelp_dir})

if(WITH_MPC)
  add_subdirectory(${rel_mpc_dir})
endif()

if(BUILDING_MIKTEX)
  add_subdirectory(${rel_padout_dir})
endif()

if(BUILDING_MIKTEX AND HAVE_ATLBASE_H)
  #add_subdirectory(${rel_migrate_dir})
endif()

if(BUILDING_MIKTEX AND WITH_UI_MFC)
  add_subdirectory(${rel_setup_mfc_dir})
endif()

if(BUILDING_MIKTEX AND WITH_UI_MFC)
  add_subdirectory(${rel_update_mfc_dir})
endif()

if(BUILDING_MIKTEX AND WITH_UI_MFC)
  add_subdirectory(${rel_remove_mfc_dir})
endif()

add_subdirectory(${rel_doc_dir})

add_subdirectory(${rel_mtrep_dir} EXCLUDE_FROM_ALL)

###############################################################################
## package database

if(INSTALL_PACKAGE_DATABASE)
  set(remote_repository
    "http://ctan.miktex.org/systems/win32/miktex/tm/packages/")

  execute_process(
    COMMAND
      wget ${remote_repository}/miktex-zzdb1-2.8.tar.lzma
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}
  )
  
  execute_process(
    COMMAND
      lzma --decompress --stdout miktex-zzdb1-2.8.tar.lzma
    COMMAND
      tar -xvf -
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}
  )

  execute_process(
    COMMAND
      wget ${remote_repository}/miktex-zzdb2-2.8.tar.lzma
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}
  )
  
  execute_process(
    COMMAND
      lzma --decompress --stdout miktex-zzdb2-2.8.tar.lzma
    COMMAND
      tar -xvf -
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}
  )

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/mpm.ini
    DESTINATION ${texmfdir}/miktex/config
  )

  install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/texmf/tpm/packages/
    DESTINATION ${texmfdir}/tpm/packages
  )
endif()

###############################################################################
## packaging

find_program(RPMBUILD_EXE NAMES rpmbuild)

set(CPACK_GENERATOR "DEB;TGZ")

if(RPMBUILD_EXE)
  set(CPACK_GENERATOR "${CPACK_GENERATOR};RPM")
endif()

set(miktex_tools_slogan "experimental TeX additions")

set(CPACK_PACKAGE_DESCRIPTION_FILE
  "${CMAKE_SOURCE_DIR}/Documentation/miktex-tools.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MiKTeX Tools: ${miktex_tools_slogan}")
set(CPACK_PACKAGE_CONTACT "Christian Schenk <cs@miktex.org>")
set(CPACK_PACKAGE_VENDOR "MiKTeX.org")
set(CPACK_PACKAGE_VERSION_MAJOR ${MIKTEX_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${MIKTEX_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${MIKTEX_J2000_VERSION})
string(REPLACE "-" "_" CPACK_PACKAGE_VERSION "${MIKTEX_VERSION_TAG_STR}")

set(CPACK_PACKAGE_FILE_NAME
  "miktex-tools-${CPACK_PACKAGE_VERSION}-${MIKTEX_PACKAGE_REVISION}-${target_system_tag}"
  )

set(
  CPACK_DEBIAN_PACKAGE_VERSION
  "${MIKTEX_VERSION_TAG_STR}-${MIKTEX_PACKAGE_REVISION}")
set(
  CPACK_DEBIAN_PACKAGE_DEPENDS
  "libqt4-core (>=4.2.0) | libqtcore4 (>= 4.2.0), libqt4-gui (>= 4.2.0) | libqtgui4 (>= 4.2.0), libcurl3 (>= 7.15.0), libexpat1, libbz2-1.0, liburiparser1, zlib1g, libpng12-0"
  )
set(CPACK_DEBIAN_PACKAGE_SECTION "TeX Authoring")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
  "${CMAKE_SOURCE_DIR}/Packaging/deb/postinst")

set(CPACK_RPM_PACKAGE_RELEASE ${MIKTEX_PACKAGE_REVISION})
set(CPACK_RPM_SPEC_INSTALL_POST "initexmf --admin --configure")

set(CPACK_STRIP_SYMBOLS TRUE)

include(CPack)
