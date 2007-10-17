## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2007 Christian Schenk
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

use_static_crt()
use_static_libraries()

include_directories(BEFORE
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}
)

set(${extractor_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/CabExtractor.cpp
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/CabExtractor.h
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/Extractor.cpp
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/StdAfx.h
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/TarBzip2Extractor.cpp
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/TarBzip2Extractor.h
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/TarExtractor.cpp
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/TarExtractor.h
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/TarLzmaExtractor.cpp
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/TarLzmaExtractor.h
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/extractor-version.h
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/include/MiKTeX/Extractor/Extractor
  ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/internal.h
)

if(NATIVE_WINDOWS)
  set(${extractor_lib_name}_sources ${${extractor_lib_name}_sources}
    ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/win/winCabExtractor.cpp
    ${CMAKE_SOURCE_DIR}/${rel_extractor_dir}/win/winCabExtractor.h
  )
endif(NATIVE_WINDOWS)

add_library(${extractor_lib_name} STATIC ${${extractor_lib_name}_sources})
