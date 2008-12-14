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

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/expat_config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/expat_config.h
  )

include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DHAVE_EXPAT_CONFIG_H)

if(NATIVE_WINDOWS)
  add_definitions(-DCOMPILED_FROM_DSP)
endif(NATIVE_WINDOWS)

use_static_crt()
use_static_libraries()

set(${expat_lib_name}_sources
  ${CMAKE_BINARY_DIR}/${rel_expat_dir}/expat_config.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/ascii.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/asciitab.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/expat-version.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/expat.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/expat_external.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/iasciitab.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/internal.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/latin1tab.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/nametab.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/utf8tab.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/xmlparse.c
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/xmlrole.c
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/xmlrole.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/xmltok.c
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/xmltok.h
  ${CMAKE_SOURCE_DIR}/${rel_expat_dir}/xmltok_impl.h
)

set_source_files_properties(
  ${CMAKE_BINARY_DIR}/${rel_expat_dir}/expat_config.h
  PROPERTIES GENERATED TRUE
)

add_library(${expat_lib_name} STATIC ${${expat_lib_name}_sources})
