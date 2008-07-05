## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2008 Christian Schenk
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

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

set(${uriparser_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriCommon.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriCommon.h
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriCompare.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriEscape.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriFile.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriIp4.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriIp4Base.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriIp4Base.h
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriNormalize.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriNormalizeBase.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriNormalizeBase.h
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriParse.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriParseBase.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriParseBase.h
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriQuery.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriRecompose.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriResolve.c
  ${CMAKE_SOURCE_DIR}/${rel_uriparser_dir}/UriShorten.c
)

add_library(${uriparser_lib_name} STATIC ${${uriparser_lib_name}_sources})
