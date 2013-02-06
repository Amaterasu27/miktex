## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2013 Christian Schenk
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

include_directories(
  ${rel_graphite2_dir}/include
)

set(GRAPHITE2_VM_TYPE auto CACHE STRING "Choose the type of vm machine: Auto, Direct or Call.")
option(GRAPHITE2_NSEGCACHE "Compile out the gr_*_with_seg_cache APIs" TRUE)
option(GRAPHITE2_NFILEFACE "Compile out the gr_make_file_face* APIs" FALSE)
option(GRAPHITE2_NTRACING "Compile out log segment tracing capability" TRUE)

string(TOLOWER ${GRAPHITE2_VM_TYPE} GRAPHITE2_VM_TYPE)
if (NOT GRAPHITE2_VM_TYPE MATCHES "auto|direct|call")
    message(SEND_ERROR "unrecognised vm machine type: ${GRAPHITE2_VM_TYPE}. Only Auto, Direct or Call are available")
endif (NOT GRAPHITE2_VM_TYPE MATCHES "auto|direct|call")
if (GRAPHITE2_VM_TYPE STREQUAL "auto")
    if (CMAKE_BUILD_TYPE MATCHES "[Rr]el(ease|[Ww]ith[Dd]eb[Ii]nfo)")
        set(GRAPHITE2_VM_TYPE "direct")
    else(CMAKE_BUILD_TYPE MATCHES "[Rr]el(ease|[Ww]ith[Dd]eb[Ii]nfo)")
        set(GRAPHITE2_VM_TYPE "call")
    endif(CMAKE_BUILD_TYPE MATCHES "[Rr]el(ease|[Ww]ith[Dd]eb[Ii]nfo)")
endif (GRAPHITE2_VM_TYPE STREQUAL "auto")
if (GRAPHITE2_VM_TYPE STREQUAL "direct" AND NOT (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_BASE_NAME STREQUAL "clang++"))
    #message(WARNING "vm machine type direct can only be built using GCC")
    set(GRAPHITE2_VM_TYPE "call")
endif (GRAPHITE2_VM_TYPE STREQUAL "direct" AND NOT (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_BASE_NAME STREQUAL "clang++"))

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_SOURCE_DIR}/${rel_graphite2_dir}/graphite2.rc.in
    ${CMAKE_BINARY_DIR}/${rel_graphite2_dir}/graphite2.rc
  )
endif(NATIVE_WINDOWS)

add_subdirectory(src)
