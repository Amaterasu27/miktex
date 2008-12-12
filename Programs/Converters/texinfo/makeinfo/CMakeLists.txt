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

include_directories(BEFORE
  ${CMAKE_SOURCE_DIR}/${rel_texinfo_dir}
)

add_definitions(
  -DR_OK=0
  -Dalloca=_alloca
  -Dindex=strchr
  -Dmain=Main
  -Dstrcasecmp=_stricmp
  -Dstrncasecmp=_strnicmp
  -Dxexit=exit
  -Dxmalloc=malloc
  -Dxrealloc=realloc
  -Dxstrdup=strdup
)

set(makeinfo_sources
  ${CMAKE_SOURCE_DIR}/${rel_texinfo_dir}/config.h
  ${CMAKE_SOURCE_DIR}/${rel_texinfo_dir}/lib/substring.c
  ${CMAKE_SOURCE_DIR}/${rel_texinfo_dir}/system.h
  ${wrapper_cpp}
  cmds.c
  cmds.h
  defun.c
  defun.h
  files.c
  files.h
  float.c
  float.h
  footnote.c
  footnote.h
  html.c
  html.h
  index.c
  index.h
  insertion.c
  insertion.h
  lang.c
  lang.h
  macro.c
  macro.h
  makeinfo-version.h
  makeinfo.c
  makeinfo.h
  multi.c
  multi.h
  node.c
  node.h
  sectioning.c
  sectioning.h
  toc.c
  toc.h
  xml.c
  xml.h
  xref.c
  xref.h
)

if(NATIVE_WINDOWS)
  set(makeinfo_sources ${makeinfo_sources}
    makeinfo.rc
  )
endif(NATIVE_WINDOWS)

add_executable(makeinfo ${makeinfo_sources})

merge_trustinfo_manifest(makeinfo asInvoker)

target_link_libraries(makeinfo
  ${app_dll_name}
  ${core_dll_name}
  ${gnu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS makeinfo DESTINATION ${bindir})
