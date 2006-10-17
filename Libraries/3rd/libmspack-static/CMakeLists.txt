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

set(${mspack_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/cab.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/cabc.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/cabd.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/chm.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/chmc.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/chmd.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/des.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/hlp.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/hlpc.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/hlpd.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/kwaj.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/kwajc.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/kwajd.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/lit.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/litc.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/litd.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/lzss.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/lzx.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/lzxc.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/lzxd.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/mspack-version.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/mspack.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/mszip.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/mszipc.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/mszipd.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/qtm.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/qtmc.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/qtmd.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/sha.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/system.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/system.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/szdd.h
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/szddc.c
  ${CMAKE_SOURCE_DIR}/${rel_mspack_dir}/szddd.c
)

add_library(${mspack_lib_name} STATIC ${${mspack_lib_name}_sources})
