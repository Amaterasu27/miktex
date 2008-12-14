## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007-2008 Christian Schenk
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

include_directories(BEFORE ${CMAKE_BINARY_DIR}/${rel_lzmalib_dir})
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/${rel_lzmalib_dir})

use_static_crt()
use_static_libraries()

add_definitions(
  -D_7ZIP_LARGE_PAGES
)

set(${lzma_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/7zCrc.c
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Alloc.c
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Compress/Lz/MatchFinder.c
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Common/InBuffer.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Common/OutBuffer.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Common/StreamUtils.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Compress/LZ/LZOutWindow.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Compress/RangeCoder/RangeCoderBit.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Compress/LZMA/LZMADecoder.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Compress/LZMA/LZMAEncoder.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Compress/LZMA/lzma-miktex.cpp
)

if(NATIVE_WINDOWS)
  add_definitions(
    -DCOMPRESS_MF_MT
  )
  set(${lzma_lib_name}_sources ${${lzma_lib_name}_sources}
    ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Compress/Lz/MatchFinderMt.c
    ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Threads.c
  )
endif(NATIVE_WINDOWS)

add_library(${lzma_lib_name} STATIC ${${lzma_lib_name}_sources})
