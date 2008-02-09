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

disable_runtime_checks()

set(${zlib_dll_name}_sources
  adler32.c
  compress.c
  crc32.c
  crc32.h
  deflate.c
  deflate.h
  gzio.c
  infback.c
  inffast.c
  inffast.h
  inffixed.h
  inflate.c
  inflate.h
  inftrees.c
  inftrees.h
  trees.c
  trees.h
  uncompr.c
  zconf.h
  zlib-version.h
  zlib.h
  zutil.c
  zutil.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/zlib.rc
  )
  set(${zlib_dll_name}_sources ${${zlib_dll_name}_sources}
    zlib.def
    ${CMAKE_CURRENT_BINARY_DIR}/zlib.rc
  )
endif(NATIVE_WINDOWS)

add_library(${zlib_dll_name} SHARED ${${zlib_dll_name}_sources})

rebase(${zlib_dll_name})

install(TARGETS ${zlib_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
