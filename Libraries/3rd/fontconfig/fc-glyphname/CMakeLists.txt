## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2008-2015 Christian Schenk
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

add_definitions(
  -DHAVE_CONFIG_H
  -DDONT_USE_MIKTEX_EXIT
  -D_UNICODE -DUNICODE
)

include_directories(
  ${CMAKE_SOURCE_DIR}/${rel_fontconfig_dir}/src
)

set(fc-glyphname_sources
  fc-glyphname.c
)

add_executable(fc-glyphname ${fc-glyphname_sources})

merge_trustinfo_manifest(fc-glyphname asInvoker)

target_link_libraries(fc-glyphname
  ${app_dll_name}
  $(core_dll_name)
  ${getopt_dll_name}
  ${unxemu_dll_name}
  ${utf8wrap_dll_name}
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fcglyphname.h
  COMMAND $<TARGET_FILE:fc-glyphname> ${CMAKE_CURRENT_SOURCE_DIR}/zapfdingbats.txt
	< ${CMAKE_CURRENT_SOURCE_DIR}/fcglyphname.tmpl.h
	> ${CMAKE_CURRENT_BINARY_DIR}/fcglyphname.h
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/zapfdingbats.txt
  DEPENDS
	fc-glyphname
	${CMAKE_CURRENT_SOURCE_DIR}/fcglyphname.tmpl.h
  VERBATIM
)

add_custom_target(fcglyphname_h ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/fcglyphname.h
)
