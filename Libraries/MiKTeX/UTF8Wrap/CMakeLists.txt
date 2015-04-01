## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2011-2015 Christian Schenk
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

set(${utf8wrap_dll_name}_sources
  include/miktex/utf8wrap.h
  internal.h
  utf8wrap-version.h
  utf8wrap.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/utf8wrap.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/utf8wrap.rc
  )
  set(${utf8wrap_dll_name}_sources ${${utf8wrap_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/utf8wrap.rc
  )
endif()

add_library(${utf8wrap_dll_name} SHARED ${${utf8wrap_dll_name}_sources})

set_target_properties(${utf8wrap_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_include_directories(${utf8wrap_dll_name}
  PRIVATE
    ${CMAKE_BINARY_DIR}/${rel_utf8wrap_dir}
    ${CMAKE_SOURCE_DIR}/${rel_utf8wrap_dir}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_utf8wrap_dir}/include
)

rebase(${utf8wrap_dll_name})

install(TARGETS ${utf8wrap_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

install(
  FILES
    include/miktex/utf8wrap.h
  DESTINATION
    ${incdir}/miktex
)
