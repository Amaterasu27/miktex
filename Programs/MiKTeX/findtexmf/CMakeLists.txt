## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2014 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(${findtexmf_dll_name}_sources
  findtexmf-version.h
  findtexmf.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/findtexmf.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/findtexmf.rc
  )
  set(${findtexmf_dll_name}_sources ${${findtexmf_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/findtexmf.rc)
endif(NATIVE_WINDOWS)

add_library(${findtexmf_dll_name} SHARED ${${findtexmf_dll_name}_sources})

target_link_libraries(${findtexmf_dll_name}
  PRIVATE
    ${app_dll_name}
    ${core_dll_name}
    ${mpm_dll_name}
    ${popt_dll_name}
)

rebase(${findtexmf_dll_name})
  
set(exes findtexmf)

foreach(t ${exes})
  create_dll_wrapper(${t} ${findtexmf_dll_name} findtexmf)
endforeach(t)

install(TARGETS ${findtexmf_dll_name} ${exes} DESTINATION ${bindir})
