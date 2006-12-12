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

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

if(NATIVE_WINDOWS)
  add_definitions(-D_WIN32_WINNT=0x0400)
endif(NATIVE_WINDOWS)

set(mkocp_sources
  mkocp.cpp
  mkocp-version.h
)

if(NATIVE_WINDOWS)
  set(mkocp_sources ${mkocp_sources}
    mkocp.rc
  )
endif(NATIVE_WINDOWS)

add_executable(mkocp ${mkocp_sources})

target_link_libraries(mkocp
  ${app_dll_name}
  ${core_dll_name}
)

install(TARGETS mkocp DESTINATION ${bindir})

set(outocp_sources
  outocp.c
  outocp-version.h
  ${wrapper_cpp}
)

if(NATIVE_WINDOWS)
  set(outocp_sources ${outocp_sources}
    outocp.rc
  )
endif(NATIVE_WINDOWS)

add_executable(outocp ${outocp_sources})

target_link_libraries(outocp
  ${app_dll_namen}
  ${core_dll_name}
  ${gnu_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS outocp DESTINATION ${bindir})

set(otp2ocp_sources
  ${CMAKE_CURRENT_BINARY_DIR}/y_tab.c
  ${CMAKE_CURRENT_BINARY_DIR}/y_tab.h
  ${CMAKE_CURRENT_BINARY_DIR}/yy.lex.c
  ${wrapper_cpp}
  otp2ocp-version.h
  otp2ocp.c
  routines.c
  otp.h
  yystype.h
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/y_tab.c
  ${CMAKE_CURRENT_BINARY_DIR}/y_tab.h
  ${CMAKE_CURRENT_BINARY_DIR}/yy.lex.c
  PROPERTIES GENERATED TRUE
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/y_tab.c
	${CMAKE_CURRENT_BINARY_DIR}/y_tab.h
  COMMAND ${BISON_EXE}
	--output=${CMAKE_CURRENT_BINARY_DIR}/y_tab.c
	--defines=${CMAKE_CURRENT_BINARY_DIR}/y_tab.h
	${CMAKE_CURRENT_SOURCE_DIR}/otp.y
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/otp.y
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/yy.lex.c
  COMMAND ${FLEX_EXE}
	-o${CMAKE_CURRENT_BINARY_DIR}/yy.lex.c
	${CMAKE_CURRENT_SOURCE_DIR}/otp.l
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/otp.l
  VERBATIM
)

if(NATIVE_WINDOWS)
  set(otp2ocp_sources ${otp2ocp_sources}
    otp2ocp.rc
  )
endif(NATIVE_WINDOWS)

add_executable(otp2ocp ${otp2ocp_sources})

target_link_libraries(otp2ocp
  ${app_dll_namen}
  ${core_dll_name}
  ${gnu_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS otp2ocp DESTINATION ${bindir})
