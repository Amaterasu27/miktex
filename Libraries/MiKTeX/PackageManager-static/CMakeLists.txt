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

add_definitions(-DHAVE_LIBCURL)

use_static_crt()
use_static_libraries()

include_directories(BEFORE
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}
)

set(${mpm_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}/stdsoap2.cpp
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}/stdsoap2.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/RepositorySoap.nsmap
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryC.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryH.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryRepositorySoapProxy.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryRepositorySoapProxy.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryStub.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/CabExtractor.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/CabExtractor.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/CurlWebFile.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/CurlWebFile.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/CurlWebSession.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/CurlWebSession.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/Extractor.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/Extractor.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/PackageInstaller.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/PackageIterator.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/StdAfx.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/TarBzip2Extractor.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/TarBzip2Extractor.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/TpmParser.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/TpmParser.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/WebFile.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/WebSession.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/WebSession.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/include/miktex/mpm.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/internal.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/mpm-version.h
  ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/mpm.cpp
)

if(MIKTEX_WINDOWS)
  set(${mpm_lib_name}_sources ${${mpm_lib_name}_sources}
    ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/win/winCabExtractor.cpp
    ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/win/winCabExtractor.h
    ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/win/winWebFile.cpp
    ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/win/winWebFile.h
    ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/win/winWebSession.cpp
    ${CMAKE_SOURCE_DIR}/${rel_mpmlib_dir}/win/winWebSession.h
  )
endif(MIKTEX_WINDOWS)

add_library(${mpm_lib_name} STATIC ${${mpm_lib_name}_sources})
