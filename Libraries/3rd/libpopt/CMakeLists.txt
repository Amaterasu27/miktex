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

if(NOT NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
  )
  add_definitions(-DHAVE_CONFIG_H)
  include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
endif(NOT NATIVE_WINDOWS)

set(${popt_dll_name}_sources
  findme.c
  findme.h
  popt-miktex.h
  popt-version.h
  popt.c
  popt.h
  poptconfig.c
  popthelp.c
  poptint.h
  poptparse.c
  system.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/popt.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/popt.rc
  )
  set(${popt_dll_name}_sources ${${popt_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/popt.rc
  )
else(NATIVE_WINDOWS)
  set(${mspack_dll_name}_sources ${${mspack_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
  )
endif(NATIVE_WINDOWS)

add_library(${popt_dll_name} SHARED ${${popt_dll_name}_sources})

install(TARGETS ${popt_dll_name} DESTINATION "${bindir}")
