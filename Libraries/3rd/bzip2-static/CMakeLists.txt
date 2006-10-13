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

use_static_crt()

add_library(${bzip2_lib_name} STATIC
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/blocksort.c
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/bzlib.c
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/bzlib.h
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/bzlib.h
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/bzlib_private.h
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/bzlib_private.h
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/compress.c
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/crctable.c
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/decompress.c
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/huffman.c
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/libbz2-version.h
  ${CMAKE_SOURCE_DIR}/${rel_bzip2_dir}/randtable.c
)
