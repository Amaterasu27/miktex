## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2006-2015 Christian Schenk
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

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

set(${regex_dll_name}_sources
  regex.c
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/regex.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/regex.rc
  )
  set(${regex_dll_name}_sources ${${regex_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/regex.rc
  )
endif()

add_library(${regex_dll_name} SHARED ${${regex_dll_name}_sources})

set_target_properties(${regex_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_compile_definitions(${regex_dll_name}
  PUBLIC
    -DMIKTEX_REGEX_SHARED
)

target_include_directories(${regex_dll_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_regex_dir}/include
)

rebase(${regex_dll_name})

install(TARGETS ${regex_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
