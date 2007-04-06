## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2007 Christian Schenk
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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(${md5_dll_name}_sources
  global.h
  internal.h
  md5-version.h
  md5c.cpp
  include/miktex/md5.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/md5.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/md5.rc
  )
  set(${md5_dll_name}_sources ${${md5_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/md5.rc
  )
endif(NATIVE_WINDOWS)

add_library(${md5_dll_name} SHARED ${${md5_dll_name}_sources})

install(TARGETS ${md5_dll_name} DESTINATION "${bindir}")
