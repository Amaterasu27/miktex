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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(${makex_dll_name}_sources
  common.cpp
  internal.h
  makebase.cpp
  makefmt.cpp
  makemem.cpp
  makemf.cpp
  makepk.cpp
  maketfm.cpp
  makex-version.h
  makex.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/makex.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/makex.rc
  )
  set(${makex_dll_name}_sources ${${makex_dll_name}_sources} ${CMAKE_CURRENT_BINARY_DIR}/makex.rc)
endif(NATIVE_WINDOWS)

add_library(${makex_dll_name} SHARED ${${makex_dll_name}_sources})

target_link_libraries(${makex_dll_name}
  ${app_dll_name}
  ${core_dll_name}
  ${texmf_dll_name}
)

if(NATIVE_WINDOWS)
target_link_libraries(${makex_dll_name}
  ${getopt_dll_name}
)
endif(NATIVE_WINDOWS)

rebase(${makex_dll_name})

set(exes
  makebase
  makefmt
  makemem
  makemf
  makepk
  maketfm
)

install(TARGETS ${makex_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

foreach(t ${exes})
  create_dll_wrapper(${miktex_prefix}${t} ${makex_dll_name} ${t})
  install(TARGETS ${miktex_prefix}${t}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
  if(NATIVE_WINDOWS)
    create_dll_wrapper(${t} ${makex_dll_name})
    install(TARGETS ${t}
      RUNTIME DESTINATION "${bindir}"
      LIBRARY DESTINATION "${libdir}"
      ARCHIVE DESTINATION "${libdir}"
    )
  endif(NATIVE_WINDOWS)
endforeach(t)

