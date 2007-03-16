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

set(kpathsea_includes
  absolute.h
  c-auto.h
  c-ctype.h
  c-errno.h
  c-fopen.h
  c-limits.h
  c-memstr.h
  c-minmax.h
  c-pathch.h
  c-pathmx.h
  c-proto.h
  c-stat.h
  c-std.h
  c-unistd.h
  c-vararg.h
  config.h
  debug.h
  expand.h
  getopt.h
  kpathsea.h
  lib.h
  magstep.h
  pathsearch.h
  proginit.h
  progname.h
  systypes.h
  tex-file.h
  tex-glyph.h
  tex-hush.h
  tex-make.h
  types.h
  variable.h
)

foreach(inc ${kpathsea_includes})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kpathsea/kpathsea.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/kpathsea/${inc}
  )
endforeach(inc ${kpathsea_includes})

set(${kpsemu_dll_name}_sources
  StdAfx.cpp
  StdAfx.h
  internal.h
  kpsemu-version.h
  kpsemu.cpp
)

if(NATIVE_WINDOWS)
  set(${kpsemu_dll_name}_sources ${${kpsemu_dll_name}_sources}
    kpsemu.rc
  )
endif(NATIVE_WINDOWS)

add_library(${kpsemu_dll_name} SHARED ${${kpsemu_dll_name}_sources})

target_link_libraries(${kpsemu_dll_name}
  ${core_dll_name}
  ${texmf_dll_name}
)

install(TARGETS ${kpsemu_dll_name} DESTINATION "${bindir}")
