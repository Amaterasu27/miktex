## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2014 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/omega.defaults.h
  COMMAND ${cfg_exe} --print-classes ${omega_defaults} > omega.defaults.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${omega_defaults}
  VERBATIM
)

set(miktex_changefiles
  ${CMAKE_CURRENT_SOURCE_DIR}/omega-miktex-fmt.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omega-miktex-misc.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omega-miktex-pool.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omega-miktex.ch
)

set(omfiles
  ${CMAKE_CURRENT_SOURCE_DIR}/om16bit.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omstr.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omfont.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omchar.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omfi.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/ompar.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omocp.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omfilter.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omtrans.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omdir.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omxml.ch
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/omega.web
  COMMAND ${tie_exe}
	-m ${CMAKE_CURRENT_BINARY_DIR}/omega.web
	${tex98_web}
	${omfiles}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${tie_exe}
    ${tex98_web}
    ${omfiles}
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/omega-miktex-all.ch
  COMMAND ${tie_exe}
	-c ${CMAKE_CURRENT_BINARY_DIR}/omega-miktex-all.ch
	${CMAKE_CURRENT_BINARY_DIR}/omega.web
	${miktex_changefiles}
  DEPENDS
    ${tie_exe}
	${CMAKE_CURRENT_BINARY_DIR}/omega.web
	${miktex_changefiles}
  VERBATIM
)

set(omega_change_file ${CMAKE_CURRENT_BINARY_DIR}/omega-miktex-all.ch)

set(${omega_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/omega.defaults.h
  omegabis.c
  omega-miktex.cc
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/omega_pool.cpp
  COMMAND
	${inipool_exe} 
		${CMAKE_CURRENT_BINARY_DIR}/omega.pool
		omega-miktex.h
		> ${CMAKE_CURRENT_BINARY_DIR}/omega_pool.cpp
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${inipool_exe}
    ${CMAKE_CURRENT_BINARY_DIR}/omega.pool
)

set(${omega_dll_name}_sources
  ${${omega_dll_name}_sources}
  ${CMAKE_CURRENT_BINARY_DIR}/omega_pool.cpp
)

set_source_files_properties(
  omegabis.c
  PROPERTIES LANGUAGE CXX
)

set_source_files_properties(
  ${${omega_dll_name}_sources}
  PROPERTIES COMPILE_FLAGS
	"-DC4PEXTERN=extern -DMIKTEX_OMEGA"

)

file(READ ${tex_dyn_sed} tex_dyn_sed_contents)
file(READ ${omega_dyn_sed} omega_dyn_sed_contents)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${tex_dyn_sed_contents}")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${omega_dyn_sed_contents}")

set(TANGLE_FLAGS -omega)

create_web_app(Omega)

target_link_libraries(${omega_dll_name}
  PRIVATE
    ${kpsemu_dll_name}
    ${w2cemu_dll_name}
    ${getopt_dll_name}
)

## dev targets

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/omega-n.web
  COMMAND ${webn_exe}
	< ${omega_web_file}
	> ${CMAKE_CURRENT_BINARY_DIR}/omega-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${webn_exe}
	${omega_web_file}
  VERBATIM
)

add_custom_target(omega-dev ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/omega-n.web
)
