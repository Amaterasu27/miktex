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

include_directories(BEFORE ${CMAKE_BINARY_DIR}/${rel_app_dir})
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/${rel_app_dir})

use_static_crt()
use_static_libraries()

set(${app_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_app_dir}/StdAfx.cpp
  ${CMAKE_SOURCE_DIR}/${rel_app_dir}/StdAfx.h
  ${CMAKE_SOURCE_DIR}/${rel_app_dir}/app-version.h
  ${CMAKE_SOURCE_DIR}/${rel_app_dir}/app.cpp
  ${CMAKE_SOURCE_DIR}/${rel_app_dir}/include/miktex/App/Application.h
  ${CMAKE_SOURCE_DIR}/${rel_app_dir}/internal.h
)

add_library(${app_lib_name} STATIC ${${app_lib_name}_sources})

target_compile_definitions(${app_lib_name}
  PUBLIC
    -DMIKTEX_APP_STATIC
)

target_include_directories(${app_lib_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_app_dir}/include
)

target_link_libraries(${app_lib_name}
  PUBLIC
    ${core_lib_name}
    ${mpm_lib_name}
)

if(INSTALL_STATIC_LIBRARIES)
  install(TARGETS ${app_lib_name}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
endif(INSTALL_STATIC_LIBRARIES)
