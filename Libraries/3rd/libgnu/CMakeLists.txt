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

set(${gnu_dll_name}_sources
  basename.c
  getopt.c
  getopt.h
  getopt1.c
  getopt_int.h
  gnu-miktex.h
  gnu-version.h
  internal.h
  substring.c
)

set(${regex_dll_name}_sources
  regcomp.c
  regex-version.h
  regex.c
  regex.h
  regex_internal.c
  regex_internal.h
  regexec.c
)

set_source_files_properties(
  regcomp.c regex_internal.c regexec.c
  PROPERTIES HEADER_FILE_ONLY TRUE)

if(WIN32)
  set(${gnu_dll_name}_sources ${${gnu_dll_name}_sources}
    gnu.rc
  )
  set(${regex_dll_name}_sources ${${regex_dll_name}_sources}
    regex.rc
  )
endif(WIN32)

add_library(${gnu_dll_name} SHARED ${${gnu_dll_name}_sources})
add_library(${regex_dll_name} SHARED ${${gnu_dll_name}_sources})

install(TARGETS ${gnu_dll_name} ${regex_dll_name} DESTINATION "${bindir}")
