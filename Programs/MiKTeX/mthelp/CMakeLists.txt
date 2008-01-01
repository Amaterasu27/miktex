## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2008 Christian Schenk
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
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(${mthelp_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/template.html.h
  mthelp.cpp
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/template.html.h
  COMMAND ${bintoc_exe} templateHtml
	< ${CMAKE_CURRENT_SOURCE_DIR}/template.html
	> ${CMAKE_CURRENT_BINARY_DIR}/template.html.h
  VERBATIM
)

if(NATIVE_WINDOWS)
  set(${mthelp_dll_name}_sources ${${mthelp_dll_name}_sources}
    mthelp.rc)
endif(NATIVE_WINDOWS)

add_library(${mthelp_dll_name} SHARED ${${mthelp_dll_name}_sources})

target_link_libraries(${mthelp_dll_name}
  ${app_dll_name}
  ${core_dll_name}
  ${mpm_dll_name}
  ${popt_dll_name}
)

rebase(${mthelp_dll_name})
  
set(exes mthelp)

if(NATIVE_WINDOWS)
  set (exes ${exes} texdoc)
endif(NATIVE_WINDOWS)

foreach(t ${exes})
  create_dll_wrapper(${t} ${mthelp_dll_name} mthelp)
endforeach(t)

install(TARGETS ${mthelp_dll_name} ${exes}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
