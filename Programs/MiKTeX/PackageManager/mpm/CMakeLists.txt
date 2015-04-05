## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2006-2015 Christian Schenk
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

use_static_crt()
use_static_libraries()

set(mpm_sources
  StdAfx.cpp
  StdAfx.h
  internal.h
  mpm-version.h
  mpm.cpp
)

add_definitions(
  -D_UNICODE -DUNICODE
)

if(NATIVE_WINDOWS)
  set(mpm_sources ${mpm_sources} mpm.rc)
endif()

add_executable(mpm ${mpm_sources})

merge_trustinfo_manifest(mpm asInvoker)

target_link_libraries(mpm
  ${core_lib_name}
  ${mpm_lib_name}
  ${popt_lib_name}
)

if(NATIVE_WINDOWS)
  target_link_libraries(mpm
    Wldap32
    shlwapi
    wininet
    winmm
    ws2_32
  )
endif()

install(TARGETS mpm DESTINATION ${bindir})
