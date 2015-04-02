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

use_static_crt()
use_static_libraries()

set(${getopt_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_getopt_dir}/getopt.c
  ${CMAKE_SOURCE_DIR}/${rel_getopt_dir}/getopt1.c
)

add_library(${getopt_lib_name} STATIC ${${getopt_lib_name}_sources})

target_compile_definitions(${getopt_lib_name}
  PUBLIC
    -DMIKTEX_GETOPT_STATIC
)

target_include_directories(${getopt_lib_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_getopt_dir}/include
)
