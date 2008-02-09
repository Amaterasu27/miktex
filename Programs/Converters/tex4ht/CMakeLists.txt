## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2008 Christian Schenk
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
  -DANSI
  -DAPPNAME=\\\"TEX4HT\\\"
  -DCDECL=__cdecl
  -DCPLUSPLUSMAIN
  -DDISABLE_INSTALLER
  -DHAVE_IO_H
  -DHAVE_STRING_H
  -DKPATHSEA
  -Dmain=Main
)

if(NATIVE_WINDOWS)
  add_definitions(
    -DPLATFORM=\\\"Windows\\\"
    -DWIN32
  )
endif(NATIVE_WINDOWS)

create_cpp_from_c(
  ${CMAKE_CURRENT_SOURCE_DIR}/htcmd.c
  ${CMAKE_CURRENT_SOURCE_DIR}/t4ht.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tex4ht.c
)

set(htcmd_sources
  ${CMAKE_CURRENT_BINARY_DIR}/htcmd.cpp
  ${wrapper_cpp}
  htcmd-version.h
)

if(NATIVE_WINDOWS)
  set(htcmd_sources ${htcmd_sources}
    htcmd.rc
  )
endif(NATIVE_WINDOWS)

add_executable(htcmd ${htcmd_sources})

merge_trustinfo_manifest(htcmd asInvoker)

target_link_libraries(htcmd
  ${app_dll_name}
  ${core_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS htcmd DESTINATION ${bindir})

set(t4ht_sources
  ${CMAKE_CURRENT_BINARY_DIR}/t4ht.cpp
  ${wrapper_cpp}
  t4ht-version.h
)

if(NATIVE_WINDOWS)
  set(t4ht_sources ${t4ht_sources}
    t4ht.rc
  )
endif(NATIVE_WINDOWS)

add_executable(t4ht ${t4ht_sources})

merge_trustinfo_manifest(t4ht asInvoker)

target_link_libraries(t4ht
  ${app_dll_name}
  ${core_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS t4ht DESTINATION ${bindir})

set(tex4ht_sources
  ${CMAKE_CURRENT_BINARY_DIR}/tex4ht.cpp
  ${wrapper_cpp}
  tex4ht-version.h
)

if(NATIVE_WINDOWS)
  set(tex4ht_sources ${tex4ht_sources}
    tex4ht.rc
  )
endif(NATIVE_WINDOWS)

add_executable(tex4ht ${tex4ht_sources})

merge_trustinfo_manifest(tex4ht asInvoker)

target_link_libraries(tex4ht
  ${app_dll_name}
  ${core_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS tex4ht DESTINATION ${bindir})
