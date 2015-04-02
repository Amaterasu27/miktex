## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2015 Christian Schenk
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
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(
  -D_7ZIP_LARGE_PAGES
)

set(${lzma_dll_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/7zCrc.c
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/7zCrc.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Alloc.c
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Alloc.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Compress/Lz/MatchFinder.c
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Compress/Lz/MatchFinder.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Common/InBuffer.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Common/InBuffer.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Common/OutBuffer.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Common/OutBuffer.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Common/StreamUtils.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Common/StreamUtils.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Compress/LZ/LZOutWindow.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Compress/RangeCoder/RangeCoderBit.cpp
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/7zip/Compress/RangeCoder/RangeCoderBit.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/ComTry.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/Defs.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/Exception.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/MyCom.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/MyException.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/MyGuidDef.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/MyInitGuid.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/MyUnknown.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/MyWindows.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/NewHandler.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/StdAfx.h
  ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/CPP/Common/Types.h
  LZMADecoder.cpp
  LZMAEncoder.cpp
  StdAfx.h
  lzma-miktex.cpp
)

if(NATIVE_WINDOWS)
  add_definitions(
    -DCOMPRESS_MF_MT
  )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/liblzma.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/liblzma.rc
  )
  set(${lzma_dll_name}_sources ${${lzma_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/liblzma.rc
    ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Compress/Lz/LzHash.h
    ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Compress/Lz/MatchFinderMt.c
    ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Threads.c
    ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}/C/Threads.h
  )
endif()

add_library(${lzma_dll_name} SHARED ${${lzma_dll_name}_sources})

target_compile_definitions(${lzma_dll_name}
  PUBLIC
    -DMIKTEX_LZMA_SHARED
)

target_include_directories(${lzma_dll_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_7zip_dir}
    ${CMAKE_SOURCE_DIR}/${rel_lzmalib_dir}/include
)

set_target_properties(${lzma_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${lzma_dll_name})

install(TARGETS ${lzma_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
