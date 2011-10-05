## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2011 Christian Schenk
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

set(touch_sources
  touch-version.h
  touch.cpp
)

if(NATIVE_WINDOWS)
  set(touch_sources ${touch_sources} touch.rc)
endif(NATIVE_WINDOWS)

add_executable(${miktex_prefix}touch ${touch_sources})

merge_trustinfo_manifest(${miktex_prefix}touch asInvoker)

target_link_libraries(${miktex_prefix}touch
  ${botan_lib_name}
  ${bzip2_lib_name}
  ${core_lib_name}
  ${md5_lib_name}
  ${popt_lib_name}
  ${utf8wrap_lib_name}
  ${zlib_lib_name}
)
