## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007 Christian Schenk
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
  ${CMAKE_CURRENT_BINARY_DIR}
)

file(WRITE
  ${CMAKE_CURRENT_BINARY_DIR}/fcaliastail.h
  "\n"
)

add_definitions(
  -DDLL_EXPORT
  -DHAVE_CONFIG_H
)

set(${fontconfig_dll_name}_sources
  fcatomic.c
  fcblanks.c
  fccache.c
  fccfg.c
  fccharset.c
  fcdbg.c
  fcdefault.c
  fcdir.c
  fcfreetype.c
  fcfs.c
  fcinit.c
  fclang.c
  fclist.c
  fcmatch.c
  fcmatrix.c
  fcname.c
  fcpat.c
  fcserialize.c
  fcstr.c
  fcxml.c
  ftglue.h
  ftglue.c
  fcmiktex.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/fontconfig.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/fontconfig.rc
  )
  set(${fontconfig_dll_name}_sources ${${fontconfig_dll_name}_sources}
    ${CMAKE_CURRENT_SOURCE_DIR}/fontconfig.def
    ${CMAKE_CURRENT_BINARY_DIR}/fontconfig.rc
  )
endif(NATIVE_WINDOWS)

add_library(${fontconfig_dll_name} SHARED ${${fontconfig_dll_name}_sources})

target_link_libraries(${fontconfig_dll_name}
  ${expat_dll_name}
  ${freetype2_dll_name}
  ${unxemu_dll_name}
)

install(TARGETS ${fontconfig_dll_name} DESTINATION "${bindir}")
