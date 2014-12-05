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

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(kpathsea_includes
  absolute.h
  c-auto.h
  c-ctype.h
  c-dir.h
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
  concatn.h
  config.h
  debug.h
  dirent.h
  expand.h
  getopt.h
  kpathsea.h
  lib.h
  line.h
  magstep.h
  paths.h
  pathsearch.h
  proginit.h
  progname.h
  readable.h
  recorder.h
  str-list.h
  systypes.h
  tex-file.h
  tex-glyph.h
  tex-hush.h
  tex-make.h
  types.h
  variable.h
  version.h
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
  include/miktex/KPSE/Emulation.h
  internal.h
  kpsemu-version.h
  kpsemu.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/kpsemu.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/kpsemu.rc
  )
  set(${kpsemu_dll_name}_sources ${${kpsemu_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/kpsemu.rc
  )
endif(NATIVE_WINDOWS)

if(NOT LINK_EVERYTHING_STATICALLY)
  add_library(${kpsemu_dll_name} SHARED ${${kpsemu_dll_name}_sources})

  set_target_properties(${kpsemu_dll_name}
    PROPERTIES
      VERSION "${MIKTEX_SERIES_STR}"
      SOVERSION "1"
  )

  target_link_libraries(${kpsemu_dll_name}
    PRIVATE 
      ${core_dll_name}
      ${texmf_dll_name}
  )

  rebase(${kpsemu_dll_name})

  install(TARGETS ${kpsemu_dll_name}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
endif(NOT LINK_EVERYTHING_STATICALLY)

if(INSTALL_MIKTEX_HEADERS)
  install(
    FILES
      include/miktex/KPSE/Emulation
      include/miktex/KPSE/Emulation.h
    DESTINATION
      ${incdir}/miktex/KPSE
  )
endif(INSTALL_MIKTEX_HEADERS)
