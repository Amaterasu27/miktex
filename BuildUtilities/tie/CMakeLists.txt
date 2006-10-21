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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(tie_sources
  ${CMAKE_CURRENT_BINARY_DIR}/tie.c
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/tie.c
  PROPERTIES GENERATED TRUE
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tie.c
  COMMAND ${ctangle_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/tie.w
	${CMAKE_CURRENT_SOURCE_DIR}/tie-miktex.ch 
	${CMAKE_CURRENT_BINARY_DIR}/tie.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  MAIN_DEPENDENCY tie.w
  DEPENDENCIES tie.w tie-miktex.ch
  VERBATIM
)

add_executable(tie ${tie_sources})

install(TARGETS tie DESTINATION "${bindir}")
