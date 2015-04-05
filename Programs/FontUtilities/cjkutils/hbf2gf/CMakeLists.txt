## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2006-2015 Christian Schenk
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

add_definitions(
  -DHAVE_MIKTEX
  -DUSE_MIKTEX_EXIT
)

if(NATIVE_WINDOWS)
  add_definitions(-D_WIN32_WINNT=0x0500)
endif()

if(NATIVE_WINDOWS)
  set(hbf2gf_sources ${hbf2gf_sources}
    hbf2gf.rc
  )
endif()

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/hbf2gf.c
  COMMAND ${ctangle_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/hbf2gf.w
	${CMAKE_CURRENT_SOURCE_DIR}/hbf2gf-miktex.ch
  DEPENDS
    ${ctangle_exe} 
	${CMAKE_CURRENT_SOURCE_DIR}/hbf2gf.w
	${CMAKE_CURRENT_SOURCE_DIR}/hbf2gf-miktex.ch
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
)

set(hbf2gf_sources
  ${CMAKE_CURRENT_BINARY_DIR}/hbf2gf.c
  ${wrapper_cpp}
  hbf.c
  hbf2gf-version.h
)

if(NATIVE_WINDOWS)
  set(hbf2gf_sources ${hbf2gf_sources}
    hbf2gf.rc
  )
endif()

add_executable(hbf2gf ${hbf2gf_sources})

merge_trustinfo_manifest(hbf2gf asInvoker)

target_link_libraries(hbf2gf
  ${app_dll_name}
  ${core_dll_name}
  ${texmf_dll_name}
  ${w2cemu_dll_name}
)

install(TARGETS hbf2gf DESTINATION ${bindir})
