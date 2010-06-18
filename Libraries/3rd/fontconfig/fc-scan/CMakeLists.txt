## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2010 Christian Schenk
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
)

set(fc-scan_sources
  ${wrapper_cpp}
  fc-scan.c
)

if(NATIVE_WINDOWS)
  set(fc-scan_sources ${fc-scan_sources}
    fc-scan.rc
  )
endif(NATIVE_WINDOWS)

add_executable(fc-scan ${fc-scan_sources})

merge_trustinfo_manifest(fc-scan asInvoker)

target_link_libraries(fc-scan
  ${app_dll_name}
  ${core_dll_name}
  ${fontconfig_dll_name}
  ${getopt_dll_name}
)

install(TARGETS fc-scan DESTINATION ${bindir})
