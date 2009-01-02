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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(${dib_dll_name}_sources
  DibChunker.cpp
  StdAfx.cpp
  StdAfx.h
  dib-version.h
  include/miktex/Graphics/DibChunker.h
  internal.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dib.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/dib.rc
  )
  set(${dib_dll_name}_sources ${${dib_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/dib.rc
  )
endif(NATIVE_WINDOWS)

add_library(${dib_dll_name} SHARED ${${dib_dll_name}_sources})

set_target_properties(${dib_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_link_libraries(${dib_dll_name}
  ${core_dll_name}
)

rebase(${dib_dll_name})

add_executable(chunkdib chunkdib.cpp)

merge_trustinfo_manifest(chunkdib asInvoker)

add_dependencies(chunkdib ${dib_dll_name})

target_link_libraries(chunkdib
  ${app_dll_name}
  ${core_dll_name}
  ${dib_dll_name}
  ${getopt_dll_name}
  ${popt_dll_name}
)  

install(TARGETS ${dib_dll_name} chunkdib
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

install(
  FILES
    include/miktex/Graphics/DibChunker
    include/miktex/Graphics/DibChunker.h
  DESTINATION
    ${incdir}/miktex/Graphics
)
