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

set(gsf2pk_sources
  gsf2pk-version.h
  gsf2pk.cpp
)

if(NATIVE_WINDOWS)
  set(gsf2pk_sources ${gsf2pk_sources} gsf2pk.rc)
endif(NATIVE_WINDOWS)

add_executable(gsf2pk ${gsf2pk_sources})

merge_trustinfo_manifest(gsf2pk asInvoker)

target_link_libraries(gsf2pk
  ${app_dll_name}
  ${core_dll_name}
  ${popt_dll_name}
  ${texmf_dll_name}
)

install(TARGETS gsf2pk DESTINATION ${bindir})
