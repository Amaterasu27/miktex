## CMakeLists.txt						-*- CMake -*-
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

set(${bzip2_dll_name}_sources
  blocksort.c
  bzlib.c
  bzlib.h
  bzlib_private.h
  compress.c
  crctable.c
  decompress.c
  huffman.c
  randtable.c

  libbz2-version.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libbz2.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libbz2.rc
  )
  set(${bzip2_dll_name}_sources ${${bzip2_dll_name}_sources}
    libbz2.def
    ${CMAKE_CURRENT_BINARY_DIR}/libbz2.rc
  )
endif()

add_library(${bzip2_dll_name} SHARED ${${bzip2_dll_name}_sources})

set_target_properties(${bzip2_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_include_directories(${bzip2_dll_name}
  PRIVATE
    ${CMAKE_BINARY_DIR}/${rel_bzip2_dir}
    ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}
  PUBLIC
    ${CMAKE_BINARY_DIR}/${rel_bzip2_dir}/include
)

target_link_libraries(${bzip2_dll_name}
  PRIVATE
    ${utf8wrap_dll_name}
)

rebase(${bzip2_dll_name})

install(TARGETS ${bzip2_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

set(public_headers
  bzlib.h
)

foreach(h ${public_headers})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${h}
    ${CMAKE_BINARY_DIR}/${rel_bzip2_dir}/include/${h}
    COPYONLY
  )
endforeach()
