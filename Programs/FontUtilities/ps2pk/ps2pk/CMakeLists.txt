## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006 Christian Schenk
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

add_definitions(
  -DKPATHSEA
)

include_directories(BEFOR
  ${CMAKE_CURRENT_SOURCE_DIR}/../common
  ${CMAKE_CURRENT_SOURCE_DIR}/../PSres
  ${CMAKE_CURRENT_SOURCE_DIR}/../type1
)

set(ps2pk_sources
  ${wrapper_cpp}
  encoding.c
  pkout.c
  ps2pk.c
)

if(NATIVE_WINDOWS)
  set(ps2pk_sources ${ps2pk_sources}
    ps2pk.rc
  )
endif(NATIVE_WINDOWS)

add_executable(ps2pk ${ps2pk_sources})

target_link_libraries(ps2pk
  ${app_dll_name}
  ${core_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
  ${zlib_dll_name}
  ${psres_dll_name}
  ps2pkcommon
  ps2pktype1
)

install(TARGETS ps2pk DESTINATION ${bindir})
