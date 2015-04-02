## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2007-2015 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(${extractor_dll_name}_sources
  CabExtractor.cpp
  CabExtractor.h
  Extractor.cpp
  StdAfx.h
  TarBzip2Extractor.cpp
  TarBzip2Extractor.h
  TarExtractor.cpp
  TarExtractor.h
  TarLzmaExtractor.cpp
  TarLzmaExtractor.h
  extractor-version.h
  include/miktex/Extractor/Extractor.h
  internal.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/extractor.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/extractor.rc
  )
  set(${extractor_dll_name}_sources ${${extractor_dll_name}_sources}
    win/winCabExtractor.cpp
    win/winCabExtractor.h
    ${CMAKE_CURRENT_BINARY_DIR}/extractor.rc
  )
endif()

add_library(${extractor_dll_name} SHARED ${${extractor_dll_name}_sources})

set_target_properties(${extractor_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_compile_definitions(${extractor_dll_name}
  PUBLIC
    -DMIKTEX_EXTRACTOR_SHARED
)

target_include_directories(${extractor_dll_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/include
)

target_link_libraries(${extractor_dll_name}
  PRIVATE
    ${core_dll_name}
    ${bzip2_dll_name}
    ${mspack_dll_name}
)

delay_load(${extractor_dll_name}
  ${bzip2_dll_name}
  ${mspack_dll_name}
)

rebase(${extractor_dll_name})

install(TARGETS ${extractor_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

install(
  FILES
    include/miktex/Extractor/Extractor
    include/miktex/Extractor/Extractor.h
  DESTINATION
    ${incdir}/miktex/Extractor
)
