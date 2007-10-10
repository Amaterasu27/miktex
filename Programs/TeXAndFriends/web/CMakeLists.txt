## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2007 Christian Schenk
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
	${TOUCH_EXE} ${CMAKE_CURRENT_BINARY_DIR}/.bootstrap
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

create_web_app(Tangle)
create_web_app(Weave)

add_dependencies(${tangle_dll_name} bootstrap)
add_dependencies(${weave_dll_name} tangle)
