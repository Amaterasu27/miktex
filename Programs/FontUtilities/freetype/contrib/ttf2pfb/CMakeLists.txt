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

add_definitions(
  -DUSE_MIKTEX_EXIT
)

set(getafm_sources
  getafm.cpp
)

if(NATIVE_WINDOWS)
  set(getafm_sources ${getafm_sources}
    getafm.rc
  )
endif(NATIVE_WINDOWS)

add_executable(getafm ${getafm_sources})

install(TARGETS getafm DESTINATION ${bindir})

set(ttf2pfb_sources
  ${wrapper_cpp}
  ttf2pfb.c
)

if(NATIVE_WINDOWS)
  set(ttf2pfb_sources ${ttf2pfb_sources}
    ttf2pfb.rc
  )
endif(NATIVE_WINDOWS)

add_executable(ttf2pfb ${ttf2pfb_sources})

target_link_libraries(ttf2pfb
  ${app_dll_name}
  ${core_dll_name}
  ${freetype_dll_name}
  ${texmf_dll_name}
)

install(TARGETS ttf2pfb DESTINATION ${bindir})

set(t1asm_sources
  t1asm.c
)

if(NATIVE_WINDOWS)
  set(t1asm_sources ${t1asm_sources}
    t1asm.rc
  )
endif(NATIVE_WINDOWS)

add_executable(t1asm ${t1asm_sources})

target_link_libraries(t1asm
  ${app_dll_name}
  ${core_dll_name}
  ${freetype_dll_name}
  ${gnu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS t1asm DESTINATION ${bindir})
