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

include_directories(${CMAKE_SOURCE_DIR}/${rel_gnu_dir})

use_static_crt()
use_static_libraries()

set(${gnu_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/basename.c
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/getopt.c
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/getopt.h
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/getopt1.c
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/getopt_int.h
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/gnu-miktex.h
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/gnu-version.h
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/internal.h
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/substring.c
)

set(${regex_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/regcomp.c
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/regex-version.h
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/regex.c
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/regex.h
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/regex_internal.c
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/regex_internal.h
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/regexec.c
)

set_source_files_properties(
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/regcomp.c
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/regex_internal.c
  ${CMAKE_SOURCE_DIR}/${rel_gnu_dir}/regexec.c
  PROPERTIES HEADER_FILE_ONLY TRUE
)

add_library(${gnu_lib_name} STATIC ${${gnu_lib_name}_sources})
add_library(${regex_lib_name} STATIC ${${regex_lib_name}_sources})
