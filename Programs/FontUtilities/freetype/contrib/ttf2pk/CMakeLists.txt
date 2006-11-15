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

set(ttf2pk_sources
  ${wrapper_cpp}
  errormsg.c
  filesrch.c
  ligkern.c
  newobj.c
  parse.c
  pklib.c
  subfont.c
  texenc.c
  ttf2pk.c
  ttfenc.c
  ttflib.c
)

if(NATIVE_WINDOWS)
  set(ttf2pk_sources ${ttf2pk_sources}
    ttf2pk.rc
  )
endif(NATIVE_WINDOWS)

add_executable(ttf2pk ${ttf2pk_sources})

target_link_libraries(ttf2pk
  ${app_dll_name}
  ${core_dll_name}
  ${freetype_dll_name}
  ${texmf_dll_name}
)

install(TARGETS ttf2pk DESTINATION ${bindir})

set(ttf2tfm_sources
  ${wrapper_cpp}
  case.c
  errormsg.c
  filesrch.c
  ligkern.c
  newobj.c
  parse.c
  subfont.c
  texenc.c
  tfmaux.c
  ttf2tfm.c
  ttfaux.c
  ttfenc.c
  vplaux.c
)

if(NATIVE_WINDOWS)
  set(ttf2tfm_sources ${ttf2pk_sources}
    ttf2pk.rc
  )
endif(NATIVE_WINDOWS)

add_executable(ttf2tfm ${ttf2pk_sources})

target_link_libraries(ttf2tfm
  ${app_dll_name}
  ${core_dll_name}
  ${freetype_dll_name}
  ${texmf_dll_name}
)

install(TARGETS ttf2tfm DESTINATION ${bindir})
