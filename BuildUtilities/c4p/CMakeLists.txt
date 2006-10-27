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

add_definitions(
  -Dalloca=_alloca
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(c4p_sources
  ${CMAKE_CURRENT_BINARY_DIR}/gram.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/gram.h
  ${CMAKE_CURRENT_BINARY_DIR}/scan.cpp
  c4p-version.h
  common.h
  main.cpp
  misc.cpp
  output.cpp
  subrange.cpp
  symtab.cpp
  type.cpp
  unistd.h
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/gram.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/gram.h
  ${CMAKE_CURRENT_BINARY_DIR}/scan.cpp
  PROPERTIES GENERATED TRUE
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/gram.cpp
	${CMAKE_CURRENT_BINARY_DIR}/gram.h
  COMMAND ${BISON_EXE}
	--output=${CMAKE_CURRENT_BINARY_DIR}/gram.cpp
	--defines=${CMAKE_CURRENT_BINARY_DIR}/gram.h
	${CMAKE_CURRENT_SOURCE_DIR}/gram.y
  DEPENDS gram.y
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/scan.cpp
  COMMAND ${FLEX_EXE}
	-o${CMAKE_CURRENT_BINARY_DIR}/scan.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/scan.l
  DEPENDS scan.l
  VERBATIM
)

if(NATIVE_WINDOWS)
  set(c4p_sources ${c4p_sources} c4p.rc)
endif(NATIVE_WINDOWS)

add_executable(c4p ${c4p_sources})

target_link_libraries(c4p ${gnu_lib_name})
