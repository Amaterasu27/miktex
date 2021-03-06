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

set(${cjklatex_dll_name}_sources
  cjklatex-version.h
  cjklatex.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cjklatex.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/cjklatex.rc
  )
  set(${cjklatex_dll_name}_sources ${${cjklatex_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/cjklatex.rc)
endif(NATIVE_WINDOWS)

add_library(${cjklatex_dll_name} SHARED ${${cjklatex_dll_name}_sources})

target_link_libraries(${cjklatex_dll_name}
  PRIVATE
    ${app_dll_name}
    ${core_dll_name}
    ${getopt_dll_name}
    ${mpm_dll_name}
    ${texmf_dll_name}
)

set(exes
  bg5pluslatex
  bg5pluspdflatex
  bg5latex
  bg5pdflatex
  bg5platex
  bg5ppdflatex
  cef5latex
  cef5pdflatex
  ceflatex
  cefpdflatex
  cefslatex
  cefspdflatex
  cjklatex
  gbklatex
  gbkpdflatex
  sjislatex
  sjispdflatex
)

foreach(t ${exes})
  create_dll_wrapper(${t} ${cjklatex_dll_name} cjklatexmain)
endforeach(t)

install(TARGETS ${cjklatex_dll_name} ${exes} DESTINATION ${bindir})
