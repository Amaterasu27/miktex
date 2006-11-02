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

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_goo_dir}
  ${CMAKE_SOURCE_DIR}/${rel_xpdf_xpdf_dir}
)

set(pdftosrc_sources
  pdftosrc.cc
  pdftosrc-version.h
)

if(NATIVE_WINDOWS)
  set(pdftosrc_sources ${pdftosrc_sources}
    pdftosrc.rc
  )
endif(NATIVE_WINDOWS)

add_executable(pdftosrc ${pdftosrc_sources})

target_link_libraries(pdftosrc
  ${app_dll_name}
  ${core_dll_name}
  ${fofi_lib_name}
  ${gnu_dll_name}
  ${goo_lib_name}
  ${xpdf_lib_name}
)

install(TARGETS pdftosrc DESTINATION ${bindir})

create_cpp_from_c(
  ${CMAKE_CURRENT_SOURCE_DIR}/ttf2afm.c
)

set(ttf2afm_sources
  ${wrapper_cpp}
  ttf2afm-version.h
  ${CMAKE_CURRENT_BINARY_DIR}/ttf2afm.cpp
)

set_source_files_properties(
  ${wrapper_cpp}
  PROPERTIES COMPILE_FLAGS "-DCPLUSPLUSMAIN"
)

if(NATIVE_WINDOWS)
  set(ttf2afm_sources ${ttf2afm_sources}
    ttf2afm.rc
  )
endif(NATIVE_WINDOWS)

add_executable(ttf2afm ${ttf2afm_sources})

target_link_libraries(ttf2afm
  ${app_dll_name}
  ${core_dll_name}
  ${gnu_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS ttf2afm DESTINATION ${bindir})
