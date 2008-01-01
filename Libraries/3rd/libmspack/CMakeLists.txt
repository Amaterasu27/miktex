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

if(NOT NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
  )
  add_definitions(-DHAVE_CONFIG_H)
  include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
endif(NOT NATIVE_WINDOWS)

set(${mspack_dll_name}_sources
  cab.h
  cabc.c
  cabd.c
  chm.h
  chmc.c
  chmd.c
  des.h
  hlp.h
  hlpc.c
  hlpd.c
  kwaj.h
  kwajc.c
  kwajd.c
  lit.h
  litc.c
  litd.c
  lzss.h
  lzx.h
  lzxc.c
  lzxd.c
  mspack-version.h
  mspack.h
  mszip.h
  mszipc.c
  mszipd.c
  qtm.h
  qtmc.c
  qtmd.c
  sha.h
  system.c
  system.h
  szdd.h
  szddc.c
  szddd.c
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/mspack.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/mspack.rc
  )
  set(${mspack_dll_name}_sources ${${mspack_dll_name}_sources}
    mspack.def
    ${CMAKE_CURRENT_BINARY_DIR}/mspack.rc
  )
else(NATIVE_WINDOWS)
  set(${mspack_dll_name}_sources ${${mspack_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
  )
endif(NATIVE_WINDOWS)

add_library(${mspack_dll_name} SHARED ${${mspack_dll_name}_sources})

set_target_properties(${mspack_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
    SOVERSION "1"
)

rebase(${mspack_dll_name})

install(TARGETS ${mspack_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
