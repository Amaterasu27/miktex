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

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/initctangle.c
  ${CMAKE_CURRENT_BINARY_DIR}/ctangle.c
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/initcommon.c
  ${CMAKE_CURRENT_BINARY_DIR}/common.c
)

set(ctangle_sources
  ${CMAKE_CURRENT_BINARY_DIR}/common.c
  ${CMAKE_CURRENT_BINARY_DIR}/ctangle.c
  common.h
  ctangle-version.h
)

if(WIN32)
  set(ctangle_sources ${ctangle_sources} ctangle.rc)
endif(WIN32)

add_executable(ctangle ${ctangle_sources})

set(cweave_sources
  ${CMAKE_CURRENT_BINARY_DIR}/common.c
  ${CMAKE_CURRENT_BINARY_DIR}/cweave.c
  common.h
  cweave-version.h
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/common.c
  ${CMAKE_CURRENT_BINARY_DIR}/cweave.c
  PROPERTIES GENERATED TRUE
)

if(WIN32)
  set(cweave ${cweave_sources} cwaeave.rc)
endif(WIN32)

get_target_property(ctangle_exe ctangle LOCATION)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cweave.c
  COMMAND ${ctangle_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/cweave.w
	-
	${CMAKE_CURRENT_BINARY_DIR}/cweave.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  MAIN_DEPENDENCY cweave.w
  DEPENDENCIES common.w prod.w
  VERBATIM
)

add_executable(cweave ${cweave_sources})

install(TARGETS ctangle cweave DESTINATION "${bindir}")
