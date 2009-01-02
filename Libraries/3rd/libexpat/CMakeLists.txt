## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2001-2007 Christian Schenk
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

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/expat_config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/expat_config.h
  )

add_definitions(-DHAVE_EXPAT_CONFIG_H)

include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

if(NATIVE_WINDOWS)
  add_definitions(-DCOMPILED_FROM_DSP)
endif(NATIVE_WINDOWS)

set(${expat_dll_name}_sources
  ascii.h
  asciitab.h
  expat-version.h
  expat.h
  expat_external.h
  iasciitab.h
  internal.h
  latin1tab.h
  nametab.h
  utf8tab.h
  xmlparse.c
  xmlrole.c
  xmlrole.h
  xmltok.c
  xmltok.h
  xmltok_impl.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/expat.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/expat.rc
  )
  set(${expat_dll_name}_sources ${${expat_dll_name}_sources}
    expat.def
    ${CMAKE_CURRENT_BINARY_DIR}/expat.rc
  )
else(NATIVE_WINDOWS)
  set(${expat_dll_name}_sources ${${expat_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/expat_config.h
  )
endif(NATIVE_WINDOWS)

add_library(${expat_dll_name} SHARED ${${expat_dll_name}_sources})

set_target_properties(${expat_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${expat_dll_name})

install(TARGETS ${expat_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
