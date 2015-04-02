## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2010-2015 Christian Schenk
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
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(
  -DZZIP_HAVE_STDINT_H=1
  -DZZIP_EXPORTS=1
)

set(${zzip_dll_name}_sources
  zzip/dir.c
  zzip/err.c
  zzip/fetch.c
  zzip/file.c
  zzip/info.c
  zzip/plugin.c
  zzip/stat.c
  zzip/write.c
  zzip/zip.c

  zzip/__debug.h
  zzip/__dirent.h
  zzip/__fnmatch.h
  zzip/__hints.h
  zzip/__mmap.h
  zzip/_config.h
  zzip/_msvc.h
  zzip/autoconf.h
  zzip/conf.h
  zzip/fetch.h
  zzip/file.h
  zzip/format.h
  zzip/fseeko.h
  zzip/info.h
  zzip/lib.h
  zzip/memdisk.h
  zzip/mmapped.h
  zzip/plugin.h
  zzip/stdint.h
  zzip/types.h
  zzip/write.h
  zzip/zzip.h
  zzip/zzip32.h

  zzip-version.h
)

foreach(hdr _msvc.h conf.h fetch.h file.h format.h fseeko.h
            info.h lib.h memdisk.h mmapped.h plugin.h
	    stdint.h types.h write.h zzip.h zzip32.h)
  configure_file(
     ${CMAKE_CURRENT_SOURCE_DIR}/zzip/${hdr}
     ${CMAKE_CURRENT_BINARY_DIR}/include/zzip/${hdr}
     COPYONLY
  )
endforeach()

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/zzip.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/zzip.rc
  )
  set(${zzip_dll_name}_sources ${${zzip_dll_name}_sources}
    zzip/_msvc.h
    ${CMAKE_CURRENT_BINARY_DIR}/zzip.rc
  )
endif()

add_library(${zzip_dll_name} SHARED ${${zzip_dll_name}_sources})

set_target_properties(${zzip_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_include_directories(${zzip_dll_name}
  PUBLIC
    ${CMAKE_BINARY_DIR}/${rel_zzip_dir}/include
)

target_link_libraries(${zzip_dll_name}
  PRIVATE
    ${zlib_dll_name}
)

rebase(${zzip_dll_name})

install(TARGETS ${zzip_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
