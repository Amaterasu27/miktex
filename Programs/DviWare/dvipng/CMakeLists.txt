## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2011 Christian Schenk
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

## cannot compile as C++: too many implicit casts
set(DVIPNG_COMPILE_AS_CXX 0)

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(dvipng_c_sources
  color.c
  draw.c
  dvi.c
  dvipng.c
  enc.c
  font.c
  fontmap.c
  ft.c
  misc.c
  papersiz.c
  pk.c
  ppagelist.c
  set.c
  sfd.c
  special.c
  tfm.c
  vf.c
)

set(dvipng_sources
  ${dvipng_c_sources}
  ${wrapper_cpp}
  commands.h
  config.h
  dvipng-version.h
  dvipng.h
)

add_definitions(-D_UNICODE -DUNICODE)

if(NATIVE_WINDOWS)
  set(dvipng_sources ${dvipng_sources}
    dvipng.rc
  )
endif(NATIVE_WINDOWS)

if(DVIPNG_COMPILE_AS_CXX)
  set_source_files_properties(${dvipng_c_sources}
    PROPERTIES
      LANGUAGE CXX
  )
  set_source_files_properties(${wrapper_cpp}
    PROPERTIES
      COMPILE_FLAGS "-DCPLUSPLUSMAIN -DBEQUIET"
  )
else(DVIPNG_COMPILE_AS_CXX)
  set_source_files_properties(${wrapper_cpp}
    PROPERTIES
      COMPILE_FLAGS "-DBEQUIET"
  )
endif(DVIPNG_COMPILE_AS_CXX)

add_executable(dvipng ${dvipng_sources})

merge_trustinfo_manifest(dvipng asInvoker)

target_link_libraries(dvipng
  ${app_dll_name}
  ${core_dll_name}
  ${freetype2_dll_name}
  ${gd_dll_name}
  ${getopt_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
  ${utf8wrap_dll_name}
)

install(TARGETS dvipng DESTINATION ${bindir})
