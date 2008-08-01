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

add_definitions(
  -DHAVE_CONFIG_H
)

include_directories(
  ${CMAKE_SOURCE_DIR}/${rel_fontconfig_dir}/src
)

set(fc-case_sources
  ${wrapper_cpp}
  fc-case.c
)

add_executable(fc-case ${fc-case_sources})

merge_trustinfo_manifest(fc-case asInvoker)

target_link_libraries(fc-case
  ${app_dll_name}
  $(core_dll_name)
  ${gnu_dll_name}
)

get_target_property(fc-case_exe fc-case LOCATION)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fccase.h
  COMMAND ${fc-case_exe} ${CMAKE_CURRENT_SOURCE_DIR}/CaseFolding.txt
	< ${CMAKE_CURRENT_SOURCE_DIR}/fccase.tmpl.h
	> ${CMAKE_CURRENT_BINARY_DIR}/fccase.h
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/CaseFolding.txt
  DEPENDS
	fc-case
	${CMAKE_CURRENT_SOURCE_DIR}/fccase.tmpl.h
  VERBATIM
)

add_custom_target(fccase_h ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/fccase.h
)
