## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2009-2011 Christian Schenk
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

add_definitions(
  -D_UNICODE
  -DUNICODE
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/res/dialog.manifest.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/dialog.manifest
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/dialog.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/dialog.rc
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(libs
  ${botan_lib_name}
  ${bzip2_lib_name}
  ${core_lib_name}
  ${md5_lib_name}
  ${utf8wrap_lib_name}
  ${zlib_lib_name}

  comctl32.lib
)

set(sources
  dialog.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dialog.rc
)

add_executable(${miktex_prefix}taskbar-icon WIN32 ${sources})

merge_trustinfo_manifest(${miktex_prefix}taskbar-icon asInvoker)

target_link_libraries(${miktex_prefix}taskbar-icon ${libs})

install(
  TARGETS ${miktex_prefix}taskbar-icon
  DESTINATION "${miktex_internal_bindir}"
)
