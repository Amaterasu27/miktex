## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2006-2015 Christian Schenk
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

set(cfg_sources
  cfg-version.h
  cfg.cpp
)

if(NATIVE_WINDOWS)
  set(cfg_sources ${cfg_sources} cfg.rc)
endif()

add_executable(cfg ${cfg_sources})

merge_trustinfo_manifest(cfg asInvoker)

target_link_libraries(cfg
  ${core_lib_name}
  ${popt_lib_name}
)
