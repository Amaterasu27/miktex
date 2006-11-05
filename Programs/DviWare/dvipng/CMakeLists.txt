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
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(
  -Dalloca=_alloca
  -DUSE_MIKTEX_EXIT
)

set(dvipng_sources
  ${wrapper_cpp}
  color.c
  draw.c
  dvi.c
  dvipng.c
  font.c
  misc.c
  papersiz.c
  pk.c
  ppagelist.c
  set.c
  special.c
  vf.c
)

if(NATIVE_WINDOWS)
  set(dvipng_sources ${dvipng_sources}
    dvipng.rc
  )
endif(NATIVE_WINDOWS)

add_executable(dvipng ${dvipng_sources})

target_link_libraries(dvipng
  ${app_dll_name}
  ${core_dll_name}
  ${gd_dll_name}
  ${gnu_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS dvipng DESTINATION ${bindir})
