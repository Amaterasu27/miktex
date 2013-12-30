## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 1013 Christian Schenk
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

include_directories(BEFORE
  ${CMAKE_BINARY_DIR}/${rel_setuplib_dir}
  ${CMAKE_SOURCE_DIR}/${rel_setuplib_dir}
  ${CMAKE_SOURCE_DIR}/${rel_setuplib_dir}/include
)

set(${setup_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_setuplib_dir}/SetupService.cpp
  ${CMAKE_SOURCE_DIR}/${rel_setuplib_dir}/include/miktex/Setup/SetupService.h
  ${CMAKE_SOURCE_DIR}/${rel_setuplib_dir}/internal.h
  ${CMAKE_SOURCE_DIR}/${rel_setuplib_dir}/setup-version.h
  ${CMAKE_SOURCE_DIR}/${rel_setuplib_dir}/win/winSetupService.cpp
)

if(NATIVE_WINDOWS)
  set(${setup_lib_name}_sources ${${setup_lib_name}_sources}
  )
endif(NATIVE_WINDOWS)

add_library(${setup_lib_name} STATIC ${${setup_lib_name}_sources})
