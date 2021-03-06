## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2001-2007 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${CMAKE_CURRENT_SOURCE_DIR}/../../extend
)

set(${freetype_dll_name}_sources
  ../../extend/ftxcmap.c
  ../../extend/ftxerr18.c
  ../../extend/ftxgasp.c
  ../../extend/ftxgdef.c
  ../../extend/ftxgpos.c
  ../../extend/ftxgsub.c
  ../../extend/ftxkern.c
  ../../extend/ftxopen.c
  ../../extend/ftxpost.c
  freetype.c
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/freetype.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/freetype.rc
  )
  set(${freetype_dll_name}_sources ${${freetype_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/freetype.rc
  )
endif(NATIVE_WINDOWS)

add_library(${freetype_dll_name} SHARED ${${freetype_dll_name}_sources})

install(TARGETS ${freetype_dll_name} DESTINATION "${bindir}")
