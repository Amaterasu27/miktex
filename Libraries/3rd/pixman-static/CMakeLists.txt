## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2013 Christian Schenk
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

##use_static_crt()
##use_static_libraries()

add_definitions(-DUSE_SSE2 -DPACKAGE=pixman-1 -DPACKAGE_VERSION="" -DPACKAGE_BUGREPORT="")

if(NOT CMAKE_CL_64)
  add_definitions(-DUSE_X86_MMX)
endif(NOT CMAKE_CL_64)

include_directories(BEFORE
  ${CMAKE_SOURCE_DIR}/${rel_pixman_include_dir}
)

set(${pixman_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-access.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-access-accessors.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-bits-image.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-combine32.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-combine-float.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-conical-gradient.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-filter.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-x86.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-mips.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-arm.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-ppc.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-edge.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-edge-accessors.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-fast-path.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-glyph.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-general.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-gradient-walker.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-image.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-implementation.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-linear-gradient.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-matrix.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-noop.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-radial-gradient.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-region16.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-region32.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-solid-fill.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-timer.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-trap.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-utils.c

  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-mmx.c
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-sse2.c

  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman.h
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-accessor.h
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-combine32.h
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-compiler.h
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-edge-imp.h
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-inlines.h
  ${CMAKE_SOURCE_DIR}/${rel_pixman_source_dir}/pixman-private.h
)

add_library(${pixman_lib_name} STATIC ${${pixman_lib_name}_sources})
