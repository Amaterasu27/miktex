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

default_char_type_is_unsigned()

set(C4P_FLAGS
  --chars-are-unsigned
)

create_web_app(DVItoMP)

target_link_libraries(${dvitomp_dll_name}
  ${kpsemu_dll_name}
)

set(makempx_sources
  makempx.cpp
  makempx-version.h
)

if(NATIVE_WINDOWS)
  set(makempx_sources ${makempx_sources}
    makempx.rc
  )
endif(NATIVE_WINDOWS)

add_executable(makempx ${makempx_sources})

merge_trustinfo_manifest(makempx asInvoker)

target_link_libraries(makempx
  ${app_dll_name}
  ${core_dll_name}
  ${popt_dll_name}
  ${texmf_dll_name}
)

install(TARGETS makempx DESTINATION ${bindir})

set(mpto_sources
  mpto.c
  mptotex-version.h
)

if(NATIVE_WINDOWS)
  set(mpto_sources ${mpto_sources}
    mptotex.rc
  )
endif(NATIVE_WINDOWS)

add_executable(mpto ${mpto_sources})

merge_trustinfo_manifest(mpto asInvoker)

target_link_libraries(mpto
  ${core_dll_name}
  ${popt_dll_name}
  ${texmf_dll_name}
)

install(TARGETS mpto DESTINATION ${bindir})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/test1.cmd.in
  ${CMAKE_CURRENT_BINARY_DIR}/test1.cmd)

#todo
#add_test(test1 ${CMAKE_CURRENT_BINARY_DIR}/test1.cmd)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/dvitomp-n.web
  COMMAND ${webn_exe}
	< ${CMAKE_CURRENT_SOURCE_DIR}/dvitomp.web
	> ${CMAKE_CURRENT_BINARY_DIR}/dvitomp-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/dvitomp.web
  VERBATIM
)

add_custom_target(dvitomp-dev ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/dvitomp-n.web
)
