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

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(unx_includes
  unistd.h
  dirent.h
)

foreach(inc ${unx_includes})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/unx.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/${inc}
  )
endforeach()

set(${unxemu_dll_name}_sources
  internal.h
  unxemu-version.h
  unxemu.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/unxemu.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/unxemu.rc
  )
  set(${unxemu_dll_name}_sources ${${unxemu_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/unxemu.rc
  )
endif()

add_library(${unxemu_dll_name} SHARED ${${unxemu_dll_name}_sources})

set_target_properties(${unxemu_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_compile_definitions(${unxemu_dll_name}
  PUBLIC
    -DMIKTEX_UNXEMU_SHARED
)

target_include_directories(${unxemu_dll_name}
  PUBLIC
    ${CMAKE_BINARY_DIR}/${rel_unxemu_dir}/include
    ${CMAKE_SOURCE_DIR}/${rel_unxemu_dir}/include
)

target_link_libraries(${unxemu_dll_name}
  PRIVATE
    ${core_dll_name}
)

rebase(${unxemu_dll_name})

install(TARGETS ${unxemu_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

install(
  FILES
    include/miktex/unxemu.h
  DESTINATION
    ${incdir}/miktex
)

foreach(inc ${unx_includes})
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/include/${inc}
  DESTINATION
    ${incdir}
  )
endforeach(inc ${unx_includes})
