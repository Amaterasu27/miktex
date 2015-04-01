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

disable_runtime_checks()

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

hide_symbols()

set(${zlib_dll_name}_sources
  adler32.c
  compress.c
  crc32.c
  crc32.h
  deflate.c
  deflate.h
  gzclose.c
  gzguts.h
  gzlib.c
  gzread.c
  gzwrite.c
  infback.c
  inffast.c
  inffast.h
  inffixed.h
  inflate.c
  inflate.h
  inftrees.c
  inftrees.h
  trees.c
  trees.h
  uncompr.c
  zutil.c
  zutil.h
  
  zlib-version.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/zlib.rc
  )
  set(${zlib_dll_name}_sources ${${zlib_dll_name}_sources}
    zlib.def
    ${CMAKE_CURRENT_BINARY_DIR}/zlib.rc
  )
endif()

add_library(${zlib_dll_name} SHARED ${${zlib_dll_name}_sources})

set_target_properties(${zlib_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_compile_definitions(${zlib_dll_name}
  PRIVATE
    -DHAVE_SYS_TYPES_H
    -DHAVE_STDINT_H
    -DHAVE_STDDEF_H
    -D_LARGEFILE64_SOURCE=1
  PUBLIC
    -DZLIB_DLL
)

target_include_directories(${zlib_dll_name}
  PRIVATE
    ${CMAKE_BINARY_DIR}/${rel_zlib_dir}
    ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}
  PUBLIC
    ${CMAKE_BINARY_DIR}/${rel_zlib_dir}/include
)

target_link_libraries(${zlib_dll_name}
  PRIVATE
    ${utf8wrap_dll_name}
)

rebase(${zlib_dll_name})

install(TARGETS ${zlib_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

set(public_headers
  zlib.h
  zconf.h
)

foreach(h ${public_headers})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${h}
    ${CMAKE_BINARY_DIR}/${rel_zlib_dir}/include/${h}
    COPYONLY
  )
endforeach()
