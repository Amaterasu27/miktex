## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2011 Christian Schenk
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

add_definitions(
  -DCPLUSPLUSMAIN
  -Dmain=Main
  -DUSE_MIKTEX_EXIT
)

if(NATIVE_WINDOWS)
  add_definitions(
    -DWIN32
  )
endif(NATIVE_WINDOWS)

set_source_files_properties(
  musixflx.c
  PROPERTIES LANGUAGE CXX
)

set(musixflx_sources
  ${wrapper_cpp}
  musixflx-version.h
  musixflx.c
)

if(NATIVE_WINDOWS)
  set(musixflx_sources ${musixflx_sources}
    musixflx.rc
  )
endif(NATIVE_WINDOWS)

add_executable(musixflx ${musixflx_sources})

merge_trustinfo_manifest(musixflx asInvoker)

target_link_libraries(musixflx
  ${app_dll_name}
  ${core_dll_name}
#  ${kpsemu_dll_name}
#  ${texmf_dll_name}
)

install(TARGETS musixflx DESTINATION ${bindir})
