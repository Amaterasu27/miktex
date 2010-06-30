## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2010 Christian Schenk
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

set(${md5_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_md5_dir}/include/md5.h
  ${CMAKE_SOURCE_DIR}/${rel_md5_dir}/md5.c
)

add_library(${md5_lib_name} STATIC ${${md5_lib_name}_sources})
