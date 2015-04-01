## CMakeLists.txt					-*- CMake -*-
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

include_directories(BEFORE
  ${CMAKE_BINARY_DIR}/${rel_mpmlib_dir}
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}
)

add_definitions(-DHAVE_LIBCURL)

if(HAVE_ATLBASE_H)
  add_definitions(-DUSE_LOCAL_SERVER)
endif(HAVE_ATLBASE_H)

use_static_crt()
use_static_libraries()

set(${mpm_lib_name}_sources
  ${CMAKE_BINARY_DIR}/${rel_mpmlib_dir}/StdAfx.h
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}/stdsoap2.cpp
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}/stdsoap2.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/CurlWebFile.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/CurlWebFile.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/CurlWebSession.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/CurlWebSession.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/PackageInstaller.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/PackageIterator.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/PackageManager.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/TpmParser.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/TpmParser.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/WebFile.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/WebSession.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/WebSession.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/include/miktex/PackageManager/PackageManager
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/internal.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/mpm-version.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/RepositorySoap.nsmap
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryC.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryH.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryRepositorySoapProxy.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryRepositorySoapProxy.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryStub.h
)

if(NATIVE_WINDOWS)
  set(${mpm_lib_name}_sources ${${mpm_lib_name}_sources}
    ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/win/winWebFile.cpp
    ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/win/winWebFile.h
    ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/win/winWebSession.cpp
    ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/win/winWebSession.h
  )
endif(NATIVE_WINDOWS)

add_library(${mpm_lib_name} STATIC ${${mpm_lib_name}_sources})

target_compile_definitions(${mpm_lib_name}
  PUBLIC
    -DMIKTEX_MPM_STATIC
)

target_include_directories(${mpm_lib_name}
    PRIVATE
      ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}
    PUBLIC
      ${CMAKE_BINARY_DIR}/${rel_mpmlib_dir}/include
      ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/include
)

target_link_libraries(${mpm_lib_name}
  PUBLIC
    ${core_lib_name}
    ${expat_lib_name}
    ${extractor_lib_name}
    ${md5_lib_name}
  )

if(NATIVE_WINDOWS)
  target_link_libraries(${mpm_lib_name}
    PUBLIC
      ${curl_lib_name}
      Ws2_32
      wininet
  )
else()
  target_link_libraries(${mpm_lib_name}
    PUBLIC
    ${CURL_LIBRARIES}
  )
endif()
