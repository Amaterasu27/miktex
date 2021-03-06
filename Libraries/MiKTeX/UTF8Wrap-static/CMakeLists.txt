## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2011-2015 Christian Schenk
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

use_static_crt()
use_static_libraries()

set(${utf8wrap_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_utf8wrap_dir}/include/miktex/utf8wrap.h
  ${CMAKE_SOURCE_DIR}/${rel_utf8wrap_dir}/internal.h
  ${CMAKE_SOURCE_DIR}/${rel_utf8wrap_dir}/utf8wrap-version.h
  ${CMAKE_SOURCE_DIR}/${rel_utf8wrap_dir}/utf8wrap.cpp
)

add_library(${utf8wrap_lib_name} STATIC ${${utf8wrap_lib_name}_sources})

target_compile_definitions(${utf8wrap_lib_name}
  PUBLIC
    -DMIKTEX_UTF8WRAP_STATIC
)

target_include_directories(${utf8wrap_lib_name}
  PRIVATE
    ${CMAKE_BINARY_DIR}/${rel_utf8wrap_dir}
    ${CMAKE_SOURCE_DIR}/${rel_utf8wrap_dir}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_utf8wrap_dir}/include
)

add_executable(utf8wrap
  ${CMAKE_SOURCE_DIR}/${rel_utf8wrap_dir}/include/miktex/utf8wrap.h
  utf8wrap.cpp
)

merge_trustinfo_manifest(utf8wrap asInvoker)

target_link_libraries(utf8wrap
  ${utf8wrap_lib_name}
)

install(TARGETS utf8wrap DESTINATION ${bindir})
