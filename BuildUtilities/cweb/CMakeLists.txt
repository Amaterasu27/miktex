## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2010 Christian Schenk
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
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(
  -DUSE_MIKTEX_EXIT
)

set(initctangle_sources
  ${wrapper_cpp}
  ${CMAKE_CURRENT_SOURCE_DIR}/initcommon.c
  ${CMAKE_CURRENT_SOURCE_DIR}/initcweb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/initctangle.c
  common.h
  ctangle-version.h
)

set_source_files_properties(
  ${CMAKE_CURRENT_SOURCE_DIR}/initcommon.c
  ${CMAKE_CURRENT_SOURCE_DIR}/initctangle.c
  PROPERTIES
   LANGUAGE CXX
)

if(NATIVE_WINDOWS)
  set(initctangle_sources ${initctangle_sources} ctangle.rc)
endif(NATIVE_WINDOWS)

add_executable(initctangle ${initctangle_sources})

target_link_libraries(initctangle
  ${kpsemu_dll_name}
  ${w2cemu_dll_name}
  ${core_dll_name}
)

merge_trustinfo_manifest(initctangle asInvoker)

get_target_property(initctangle_exe initctangle LOCATION)

set(ctangle_sources
  ${wrapper_cpp}
  ${CMAKE_CURRENT_BINARY_DIR}/common.c
  ${CMAKE_CURRENT_BINARY_DIR}/ctangle.c
  ${CMAKE_CURRENT_BINARY_DIR}/cweb.h
  common.h
  ctangle-version.h
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/common.c
  ${CMAKE_CURRENT_BINARY_DIR}/ctangle.c
  ${CMAKE_CURRENT_BINARY_DIR}/cweb.h
  PROPERTIES
   GENERATED TRUE
   LANGUAGE CXX
)

set_source_files_properties(
  ${wrapper_cpp}
  PROPERTIES COMPILE_FLAGS "-DCPLUSPLUSMAIN"
)

if(NATIVE_WINDOWS)
  set(ctangle_sources ${ctangle_sources} ctangle.rc)
endif(NATIVE_WINDOWS)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ctangle.c
  COMMAND ${initctangle_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/ctangle.w
	${CMAKE_CURRENT_SOURCE_DIR}/ctang-w2c.ch
	${CMAKE_CURRENT_BINARY_DIR}/ctangle.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/ctangle.w
  DEPENDS
	initctangle
	${CMAKE_CURRENT_SOURCE_DIR}/common.h
	${CMAKE_CURRENT_SOURCE_DIR}/ctang-w2c.ch
	${CMAKE_CURRENT_SOURCE_DIR}/ctangle.w
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/common.c
  COMMAND ${initctangle_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/common.w
	${CMAKE_CURRENT_SOURCE_DIR}/comm-w2c.ch
	${CMAKE_CURRENT_BINARY_DIR}/common.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/common.w
  DEPENDS
	initctangle
	${CMAKE_CURRENT_SOURCE_DIR}/comm-w2c.ch
	${CMAKE_CURRENT_SOURCE_DIR}/common.h
	${CMAKE_CURRENT_SOURCE_DIR}/common.w
  VERBATIM
)

add_executable(ctangle ${ctangle_sources})

merge_trustinfo_manifest(ctangle asInvoker)

target_link_libraries(ctangle
  ${kpsemu_dll_name}
  ${w2cemu_dll_name}
  ${core_dll_name}
)

set(cweave_sources
  ${wrapper_cpp}
  ${CMAKE_CURRENT_BINARY_DIR}/common.c
  ${CMAKE_CURRENT_BINARY_DIR}/cweave.c
  ${CMAKE_CURRENT_BINARY_DIR}/cweb.h
  common.h
  cweave-version.h
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/cweave.c
  PROPERTIES
   GENERATED TRUE
   LANGUAGE CXX
)

if(NATIVE_WINDOWS)
  set(cweave_sources ${cweave_sources} cweave.rc)
endif(NATIVE_WINDOWS)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cweave.c
  COMMAND ${ctangle_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/cweave.w
	${CMAKE_CURRENT_SOURCE_DIR}/cweav-w2c.ch
	${CMAKE_CURRENT_BINARY_DIR}/cweave.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/cweave.w
  DEPENDS
	ctangle
	${CMAKE_CURRENT_SOURCE_DIR}/common.h
	${CMAKE_CURRENT_SOURCE_DIR}/cweav-w2c.ch
	${CMAKE_CURRENT_SOURCE_DIR}/cweave.w
	${CMAKE_CURRENT_SOURCE_DIR}/prod.w
  VERBATIM
)

add_executable(cweave ${cweave_sources})

merge_trustinfo_manifest(cweave asInvoker)

target_link_libraries(cweave
  ${kpsemu_dll_name}
  ${w2cemu_dll_name}
  ${core_dll_name}
)

if(BUILDING_MIKTEX)
  install(TARGETS ctangle cweave DESTINATION "${bindir}")
endif(BUILDING_MIKTEX)
