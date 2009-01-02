## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2008 Christian Schenk
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

set(${getopt_dll_name}_sources
  getopt.c
  getopt1.c
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/getopt.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/getopt.rc
  )
  set(${getopt_dll_name}_sources ${${getopt_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/getopt.rc
  )
endif(NATIVE_WINDOWS)

add_library(${getopt_dll_name} SHARED ${${getopt_dll_name}_sources})

set_target_properties(${getopt_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${getopt_dll_name})

install(TARGETS ${getopt_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
