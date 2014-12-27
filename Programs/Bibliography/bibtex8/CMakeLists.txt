## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2014 Christian Schenk
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
set(BIBTEX8_COMPILE_AS_CXX 1)

default_char_type_is_unsigned()

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_icu_dir}/include
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

add_definitions(
  -DHAVE_CONFIG_H
  -DKPATHSEA
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

set(common_sources
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

set(bibtex8_sources ${common_sources})

if(NATIVE_WINDOWS)
  set(bibtex8_sources ${bibtex8_sources} bibtex8.rc)
endif(NATIVE_WINDOWS)

add_executable(bibtex8 ${bibtex8_sources})

set_target_properties(bibtex8 PROPERTIES COMPILE_DEFINITIONS "SUPPORT_8BIT")

merge_trustinfo_manifest(bibtex8 asInvoker)

target_link_libraries(bibtex8
  ${app_dll_name}
  ${core_dll_name}
  ${getopt_dll_name}
  ${kpsemu_dll_name}
  ${utf8wrap_dll_name}
)

install(TARGETS bibtex8 DESTINATION ${bindir})

set(bibtexu_sources ${common_sources})

if(NATIVE_WINDOWS)
  set(bibtexu_sources ${bibtexu_sources} bibtexu.rc)
endif(NATIVE_WINDOWS)

add_executable(bibtexu ${bibtexu_sources})

set_target_properties(bibtexu PROPERTIES COMPILE_DEFINITIONS "UTF_8")

merge_trustinfo_manifest(bibtexu asInvoker)

target_link_libraries(bibtexu
  ${app_dll_name}
  ${core_dll_name}
  ${getopt_dll_name}
  ${icuin_dll_name}
  ${icuuc_dll_name}
  ${kpsemu_dll_name}
  ${utf8wrap_dll_name}
)

install(TARGETS bibtexu DESTINATION ${bindir})
