## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2010-2015 Christian Schenk
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
)

set(HAVE_INTXX_T 1)
set(HAVE_UINTXX_T 1)
set(HAVE_SNPRINTF 1)
set(HAVE_VSNPRINTF 1)
set(HAVE_SIZE_T 1)
set(HAVE_STDINT_T 1)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/obsdcompat/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/obsdcompat/config.h
)

set(${obsdcompat_dll_name}_sources
  bsd-asprintf.c
  defines.h
  includes.h
  obsdcompat-version.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/obsdcompat.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/obsdcompat.rc
  )
  set(${obsdcompat_dll_name}_sources ${${obsdcompat_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/obsdcompat.rc
  )
endif(NATIVE_WINDOWS)

add_library(${obsdcompat_dll_name} SHARED ${${obsdcompat_dll_name}_sources})

set_target_properties(${obsdcompat_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_compile_definitions(${obsdcompat_dll_name}
  PUBLIC
    -DMIKTEX_OBSDCOMPAT_SHARED
)
  
target_include_directories(${obsdcompat_dll_name}
  PRIVATE
    ${CMAKE_BINARY_DIR}/${rel_obsdcompat_dir}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_obsdcompat_dir}
)
    
rebase(${obsdcompat_dll_name})

install(TARGETS ${obsdcompat_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
