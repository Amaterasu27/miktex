## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2011 Christian Schenk
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

## cannot compile as C++: too many implicit casts
set(BIBTEX8_COMPILE_AS_CXX 0)

default_char_type_is_unsigned()

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

add_definitions(
  -DHAVE_CONFIG_H
  -DKPATHSEA
  -DSUPPORT_8BIT
  -D_UNICODE -DUNICODE
)

if(BIBTEX8_COMPILE_AS_CXX)
  set_source_files_properties(
    bibtex-1.c
    bibtex-2.c
    bibtex-3.c
    bibtex-4.c
    bibtex.c
    utils.c
    PROPERTIES LANGUAGE CXX
  )
  add_definitions(
    -DCPLUSPLUSMAIN
  )
endif(BIBTEX8_COMPILE_AS_CXX)

set(bibtex8_sources
  ${wrapper_cpp}
  bibtex-1.c
  bibtex-2.c
  bibtex-3.c
  bibtex-4.c
  bibtex.c
  bibtex.h
  bibtex8-version.h
  datatype.h
  gblprocs.h
  gblvars.h
  sysdep.h
  utils.c
  utils.h
  version.h
)

if(NATIVE_WINDOWS)
  set(bibtex8_sources ${bibtex8_sources} bibtex8.rc)
endif(NATIVE_WINDOWS)

add_executable(bibtex8 ${bibtex8_sources})

merge_trustinfo_manifest(bibtex8 asInvoker)

target_link_libraries(bibtex8
  ${app_dll_name}
  ${core_dll_name}
  ${getopt_dll_name}
  ${kpsemu_dll_name}
  ${utf8wrap_dll_name}
)

install(TARGETS bibtex8 DESTINATION ${bindir})
