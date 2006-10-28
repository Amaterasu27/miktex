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

set(${dib_dll_name}_sources
  DibChunker.cpp
  StdAfx.cpp
  StdAfx.h
  dib-version.h
  internal.h
)

if(NATIVE_WINDOWS)
  set(${dib_dll_name}_sources ${${dib_dll_name}_sources}
    dib.rc
  )
endif(NATIVE_WINDOWS)

add_library(${dib_dll_name} SHARED ${${dib_dll_name}_sources})

target_link_libraries(${dib_dll_name}
  ${core_dll_name}
)

add_executable(chunkdib chunkdib.cpp)

add_dependencies(chunkdib ${dib_dll_name})

target_link_libraries(chunkdib
  ${app_dll_name}
  ${core_dll_name}
  ${dib_dll_name}
  ${gnu_dll_name}
  ${popt_dll_name}
)  

install(TARGETS ${dib_dll_name} chunkdib DESTINATION "${bindir}")