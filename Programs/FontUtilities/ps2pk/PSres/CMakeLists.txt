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

set(${psres_dll_name}_sources
  PSres-version.h
  PSres.c
  PSres.h
  defs.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/PSres.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/PSres.rc
  )
  set(${psres_dll_name}_sources ${${psres_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/PSres.rc
    psres.def
  )
endif(NATIVE_WINDOWS)

add_library(${psres_dll_name} SHARED ${${psres_dll_name}_sources})

target_link_libraries(${psres_dll_name}
  ${core_dll_name}
  ps2pkcommon
)

install(TARGETS ${psres_dll_name} DESTINATION ${bindir})
