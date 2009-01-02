## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007 Christian Schenk
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
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

set(${teckit_dll_name}_sources
  Engine.cpp
  teckit-version.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/teckit.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/teckit.rc
  )
  set(${teckit_dll_name}_sources ${${teckit_dll_name}_sources}
    teckit.def
    ${CMAKE_CURRENT_BINARY_DIR}/teckit.rc
  )
endif(NATIVE_WINDOWS)

add_library(${teckit_dll_name} SHARED ${${teckit_dll_name}_sources})

set_target_properties(${teckit_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_link_libraries(${teckit_dll_name}
  ${zlib_dll_name}
)

delay_load(${teckit_dll_name}
  ${zlib_dll_name}
)

rebase(${teckit_dll_name})

install(TARGETS ${teckit_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
