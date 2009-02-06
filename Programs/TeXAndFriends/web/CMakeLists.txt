## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2009 Christian Schenk
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

if(LINK_EVERYTHING_STATICALLY)
  use_static_crt()
  use_static_libraries()
endif(LINK_EVERYTHING_STATICALLY)
  
include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/.bootstrap
  COMMAND
	${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/inittangle.p
		${CMAKE_CURRENT_BINARY_DIR}/tangle.p
  COMMAND
	${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/inittangle.pool
		${CMAKE_CURRENT_BINARY_DIR}/tangle.pool
  COMMAND
	${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/.bootstrap
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/inittangle.p
	${CMAKE_CURRENT_SOURCE_DIR}/inittangle.pool
  VERBATIM
)

add_custom_target(
  bootstrap
  ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.bootstrap
)

if(BUILDING_MIKTEX)
  create_web_app(Tangle)
  create_web_app(Weave)
else(BUILDING_MIKTEX)
  create_web_app(Tangle Tangle tangle FALSE FALSE)
  create_web_app(Weave Weave weave FALSE FALSE)
endif(BUILDING_MIKTEX)

if(LINK_EVERYTHING_STATICALLY)
  add_dependencies(${tangle_lib_name} bootstrap)
  add_dependencies(${weave_lib_name} ${miktex_prefix}tangle)
else(LINK_EVERYTHING_STATICALLY)
  add_dependencies(${tangle_dll_name} bootstrap)
  add_dependencies(${weave_dll_name} ${miktex_prefix}tangle)
endif(LINK_EVERYTHING_STATICALLY)
