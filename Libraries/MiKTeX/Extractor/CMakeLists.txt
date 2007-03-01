## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(${extractor_dll_name}_sources
  CabExtractor.cpp
  CabExtractor.h
  Extractor.cpp
  StdAfx.h
  TarBzip2Extractor.cpp
  TarBzip2Extractor.h
  extractor-version.h
  include/miktex/Extractor.h
  internal.h
)

if(NATIVE_WINDOWS)
  set(${extractor_dll_name}_sources ${${extractor_dll_name}_sources}
    win/winCabExtractor.cpp
    win/winCabExtractor.h
  )
endif(NATIVE_WINDOWS)

add_library(${extractor_dll_name} SHARED ${${extractor_dll_name}_sources})

target_link_libraries(${extractor_dll_name}
  ${core_dll_name}
  ${bzip2_dll_name}
  ${mspack_dll_name}
)

if(NATIVE_WINDOWS)
  target_link_libraries(${extractor_dll_name}
    fdi
  )
endif(NATIVE_WINDOWS)

install(TARGETS ${extractor_dll_name} DESTINATION "${bindir}")
