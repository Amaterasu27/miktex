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

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

foreach(t cef5conv cefconv cefsconv)
  set(${t}_sources
    ${CMAKE_CURRENT_BINARY_DIR}/${t}.c
    ${t}-version.h
  )

  if(NATIVE_WINDOWS)
    set(${t}_sources ${${t}_sources}
      ${t}.rc
    )
  endif(NATIVE_WINDOWS)

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${t}.c
    COMMAND ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/${t}.w
    DEPENDS ${ctangle_exe} ${CMAKE_CURRENT_SOURCE_DIR}/${t}.w
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
  )

  add_executable(${t} ${CMAKE_CURRENT_BINARY_DIR}/${t}.c)

  merge_trustinfo_manifest(${t} asInvoker)

  install(TARGETS ${t} DESTINATION ${bindir})
endforeach(t)