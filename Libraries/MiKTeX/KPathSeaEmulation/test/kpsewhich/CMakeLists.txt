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

#add_definitions(
#)

set (kpsewhich_sources
  kpsewhich.c
  ${wrapper_cpp}
  kpsewhich-version.h
)

if(NATIVE_WINDOWS)
  set(kpsewhich_sources ${kpsewhich_sources} kpsewhich.rc)
endif(NATIVE_WINDOWS)

add_executable(${miktex_prefix}kpsewhich ${kpsewhich_sources})

merge_trustinfo_manifest(${miktex_prefix}kpsewhich asInvoker)

target_link_libraries(${miktex_prefix}kpsewhich
  ${app_dll_name}
  ${core_dll_name}
  ${getopt_dll_name}
  ${kpsemu_dll_name}
)

install(TARGETS ${miktex_prefix}kpsewhich DESTINATION ${bindir})

add_executable(kpsewhich ${kpsewhich_sources})

merge_trustinfo_manifest(kpsewhich asInvoker)

target_link_libraries(kpsewhich
  ${app_dll_name}
  ${core_dll_name}
  ${getopt_dll_name}
  ${kpsemu_dll_name}
)

install(TARGETS kpsewhich DESTINATION ${bindir})
