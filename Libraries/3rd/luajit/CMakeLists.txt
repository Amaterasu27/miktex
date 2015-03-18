## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2015 Christian Schenk
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

add_definitions(-DLUA_BUILD_AS_DLL)

set(${luajit_dll_name}_sources
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libluajit.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libluajit.rc
  )
  set(${luajit_dll_name}_sources ${${luajit_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/libluajit.rc
  )
endif(NATIVE_WINDOWS)

add_library(${luajit_dll_name} SHARED ${${luajit_dll_name}_sources})

target_link_libraries(${luajit_dll_name}
  ${utf8wrap_dll_name}
)

set_target_properties(${luajit_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${luajit_dll_name})

install(TARGETS ${luajit_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

set(luajit_sources
  src/luajit.c
  luajit-version.h
)

if(NATIVE_WINDOWS)
  set(luajit_sources ${luajit_sources}
    luajit.rc
  )
endif(NATIVE_WINDOWS)

add_executable(${miktex_prefix}luajit ${luajit_sources})

merge_trustinfo_manifest(${miktex_prefix}luajit asInvoker)

target_link_libraries(${miktex_prefix}luajit ${luajit_dll_name})

install(TARGETS ${miktex_prefix}luajit DESTINATION ${bindir})
