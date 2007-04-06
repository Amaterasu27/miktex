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

set(${mcd_dll_name}_sources
  mcd-version.h
  mcd.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/mcd.rc).in
    ${CMAKE_CURRENT_BINARY_DIR}/mcd.rc)
  )
  set(${mcd_dll_name}_sources ${${mcd_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/mcd.rc)
endif(NATIVE_WINDOWS)

add_library(${mcd_dll_name} SHARED ${${mcd_dll_name}_sources})

target_link_libraries(${mcd_dll_name}
  ${app_dll_name}
  ${core_dll_name}
  ${popt_dll_name}
  ${regex_dll_name}
  ${texmf_dll_name}
)
  
create_dll_wrapper(texify ${mcd_dll_name} mcdmain)
create_dll_wrapper(texi2dvi ${mcd_dll_name} mcdmain)

install(TARGETS ${mcd_dll_name} texify texi2dvi DESTINATION ${bindir})
