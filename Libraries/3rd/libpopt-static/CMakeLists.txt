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

use_static_crt()
use_static_libraries()

set(${popt_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/findme.c
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/findme.h
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/popt-miktex.h
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/popt-version.h
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/popt.c
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/popt.h
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/poptconfig.c
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/popthelp.c
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/poptint.h
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/poptparse.c
  ${CMAKE_SOURCE_DIR}/${rel_popt_dir}/system.h
)

add_library(${popt_lib_name} STATIC ${${popt_lib_name}_sources})
