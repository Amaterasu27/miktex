## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2007-2015 Christian Schenk
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
  -DHAVE_CONFIG_H
  -D_UNICODE -DUNICODE
)

set(fc-list_sources
  ${wrapper_cpp}
  fc-list-version.h
  fc-list.c
)

if(NATIVE_WINDOWS)
  set(fc-list_sources ${fc-list_sources}
    fc-list.rc
  )
endif()

add_executable(fc-list ${fc-list_sources})

merge_trustinfo_manifest(fc-list asInvoker)

target_link_libraries(fc-list
  ${app_dll_name}
  ${core_dll_name}
  ${fontconfig_dll_name}
  ${getopt_dll_name}
  ${unxemu_dll_name}
  ${utf8wrap_dll_name}
)

install(TARGETS fc-list DESTINATION ${bindir})
