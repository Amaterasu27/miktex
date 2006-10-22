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

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}
)

set(${mpm_dll_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}/stdsoap2.cpp
  ${CMAKE_SOURCE_DIR}/${rel_gsoap_dir}/stdsoap2.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/RepositorySoap.nsmap
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryC.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryH.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryRepositorySoapProxy.cpp
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryRepositorySoapProxy.h
  ${CMAKE_SOURCE_DIR}/${rel_mtrep_dir}/repositoryStub.h
  CabExtractor.cpp
  CabExtractor.h
  CurlWebFile.cpp
  CurlWebFile.h
  CurlWebSession.cpp
  CurlWebSession.h
  Extractor.cpp
  Extractor.h
  PackageInstaller.cpp
  PackageIterator.cpp
  StdAfx.h
  TarBzip2Extractor.cpp
  TarBzip2Extractor.h
  TpmParser.cpp
  TpmParser.h
  WebFile.h
  WebSession.cpp
  WebSession.h
  include/miktex/mpm.h
  internal.h
  mpm-version.h
  mpm.cpp
)

if(MIKTEX_WINDOWS)
  set(${mpm_dll_name}_sources ${${mpm_dll_name}_sources}
    mpm.rc
    win/winCabExtractor.cpp
    win/winCabExtractor.h
    win/winWebFile.cpp
    win/winWebFile.h
    win/winWebSession.cpp
    win/winWebSession.h
  )
endif(MIKTEX_WINDOWS)

add_library(${mpm_dll_name} SHARED ${${mpm_dll_name}_sources})

target_link_libraries(${mpm_dll_name}
  ${bzip2_dll_name}
  ${core_dll_name}
  ${expat_dll_name}
  ${md5_dll_name}
  ${mspack_dll_name}
)

if(MIKTEX_WINDOWS)
  target_link_libraries(${mpm_dll_name}
    ${curl_dll_name}
    Ws2_32
    fdi
    wininet
  )
else(MIKTEX_WINDOWS)
  target_link_libraries(${mpm_dll_name}
    ${CURL_LIBRARIES}
  )
endif(MIKTEX_WINDOWS)

install(TARGETS ${mpm_dll_name} DESTINATION "${bindir}")
