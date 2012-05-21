## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2012 Christian Schenk
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

use_static_crt()
use_static_libraries()

add_definitions(
  -DHAVE_SYS_TYPES_H
  -DHAVE_STDINT_H
  -DHAVE_STDDEF_H
  -D_LARGEFILE64_SOURCE=1
)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

set(${zlib_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/adler32.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/compress.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/crc32.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/crc32.h
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/deflate.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/deflate.h
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/gzclose.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/gzguts.h
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/gzlib.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/gzread.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/gzwrite.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/infback.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/inffast.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/inffast.h
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/inffixed.h
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/inflate.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/inflate.h
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/inftrees.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/inftrees.h
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/trees.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/trees.h
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/uncompr.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/zutil.c
  ${CMAKE_SOURCE_DIR}/${rel_zlib_dir}/zutil.h
)

add_library(${zlib_lib_name} STATIC ${${zlib_lib_name}_sources})
