## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2009 Christian Schenk
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
  -DHAVE_CTYPE_H
  -DHAVE_LOCALE_H
  -DHAVE_SETLOCALE
  -DHAVE_STRCHR
  -DHAVE_STRINGS_H
  -DHAVE_STRRCHR
  -DIBM_PC_MICROSOFT
  -DOS_WIN32
  -UDEBUG
)

if(NATIVE_WINDOWS)
  add_definitions(
    -DIBM_PC_MICROSOFT
    -DOS_WIN32
  )
endif(NATIVE_WINDOWS)

set(makeindex_sources
  ${wrapper_cpp}
  genind.c
  genind.h
  makeindex-version.h
  mkind.c
  mkind.h
  qsort.c
  scanid.c
  scanid.h
  scanst.c
  scanst.h
  sortid.c
)

if(NATIVE_WINDOWS)
  set(makeindex_sources ${makeindex_sources} makeindex.rc)
endif(NATIVE_WINDOWS)

add_executable(${miktex_prefix}makeindex ${makeindex_sources})

merge_trustinfo_manifest(${miktex_prefix}makeindex asInvoker)

target_link_libraries(${miktex_prefix}makeindex
  ${app_dll_name}
  ${core_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS ${miktex_prefix}makeindex DESTINATION ${bindir})

add_executable(makeindex ${makeindex_sources})

merge_trustinfo_manifest(makeindex asInvoker)

target_link_libraries(makeindex
  ${app_dll_name}
  ${core_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS makeindex DESTINATION ${bindir})
