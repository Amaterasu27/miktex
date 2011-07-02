## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007-2011 Christian Schenk
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

add_definitions(
  -DHAVE_CONFIG_H
  -D_UNICODE -DUNICODE
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(fc-cat_sources
  ${CMAKE_CURRENT_BINARY_DIR}/../fc-arch/fcarch.h
  ${wrapper_cpp}
  fc-cat.c
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/../fc-arch/fcarch.h
  PROPERTIES GENERATED TRUE
)

if(NATIVE_WINDOWS)
  set(fc-cat_sources ${fc-cat_sources}
    fc-cat.rc
  )
endif(NATIVE_WINDOWS)

add_executable(fc-cat ${fc-cat_sources})

merge_trustinfo_manifest(fc-cat asInvoker)

target_link_libraries(fc-cat
  ${app_dll_name}
  ${core_dll_name}
  ${fontconfig_dll_name}
  ${getopt_dll_name}
  ${utf8wrap_dll_name}
)

install(TARGETS fc-cat DESTINATION ${bindir})
