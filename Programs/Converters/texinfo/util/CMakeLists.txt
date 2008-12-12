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
  ${CMAKE_SOURCE_DIR}/${rel_texinfo_dir}
)

add_definitions(
  -DLOCALEDIR=0
  -D_POSIX_VERSION
  -Dmain=Main
  -Dxexit=exit
  -Dxmalloc=malloc
  -Dxrealloc=realloc
)

set(texindex_sources
  ${CMAKE_SOURCE_DIR}/${rel_texinfo_dir}/config.h
  ${CMAKE_SOURCE_DIR}/${rel_texinfo_dir}/system.h
  ${wrapper_cpp}
  texindex-version.h
  texindex.c
)

if(NATIVE_WINDOWS)
  set(texindex_sources ${texindex_sources}
    texindex.rc
  )
endif(NATIVE_WINDOWS)

add_executable(texindex ${texindex_sources})

merge_trustinfo_manifest(texindex asInvoker)

target_link_libraries(texindex
  ${app_dll_name}
  ${core_dll_name}
  ${getopt_dll_name}
  ${texmf_dll_name}
)

install(TARGETS texindex DESTINATION ${bindir})
