## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2009-2015 Christian Schenk
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

set(${kpsemu_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_kpsemu_dir}/StdAfx.cpp
  ${CMAKE_SOURCE_DIR}/${rel_kpsemu_dir}/StdAfx.h
  ${CMAKE_SOURCE_DIR}/${rel_kpsemu_dir}/include/miktex/KPSE/Emulation.h
  ${CMAKE_SOURCE_DIR}/${rel_kpsemu_dir}/internal.h
  ${CMAKE_SOURCE_DIR}/${rel_kpsemu_dir}/kpsemu-version.h
  ${CMAKE_SOURCE_DIR}/${rel_kpsemu_dir}/kpsemu.cpp
)

add_library(${kpsemu_lib_name} STATIC ${${kpsemu_lib_name}_sources})

target_include_directories(${kpsemu_lib_name}
  PRIVATE
    ${CMAKE_BINARY_DIR}/${rel_kpsemu_dir}
    ${CMAKE_SOURCE_DIR}/${rel_kpsemu_dir}
  PUBLIC
    ${CMAKE_BINARY_DIR}/${rel_kpsemu_dir}/include
    ${CMAKE_SOURCE_DIR}/${rel_kpsemu_dir}/include
)

target_link_libraries(${kpsemu_lib_name}
  PUBLIC
    ${core_lib_name}
    ${texmf_lib_name}
  INTERFACE
    ${getopt_lib_name}
)

if(INSTALL_STATIC_LIBRARIES)
  install(TARGETS ${kpsemu_lib_name}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
endif(INSTALL_STATIC_LIBRARIES)
