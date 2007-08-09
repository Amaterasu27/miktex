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
  ${CMAKE_CURRENT_SOURCE_DIR}/omega-miktex.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omega-miktex-fmt.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/omega-miktex-misc.ch
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
	${tex_web}
	${tex_fallback_ch}
	${omfiles}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${tex_web} ${tex_fallback_ch} ${omfiles}
  VERBATIM
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/omega-miktex-all.ch
  COMMAND ${tie_exe}
	-c ${CMAKE_CURRENT_BINARY_DIR}/omega-miktex-all.ch
	${CMAKE_CURRENT_BINARY_DIR}/omega.web
	${miktex_changefiles}
  DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/omega.web
	${miktex_changefiles}
  VERBATIM
)

set(omega_change_file ${CMAKE_CURRENT_BINARY_DIR}/omega-miktex-all.ch)

set(${omega_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/omega.defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/omegabis.cpp
  omega-miktex.cc
)

create_cpp_from_c(${CMAKE_CURRENT_SOURCE_DIR}/omegabis.c)

set_source_files_properties(
  ${${omega_dll_name}_sources}
  PROPERTIES COMPILE_FLAGS
	"-DC4PEXTERN=extern -DMIKTEX_OMEGA -DOMEGADATA=g_OMEGAData"

)

file(READ ${tex_dyn_sed} tex_dyn_sed_contents)
file(READ ${omega_dyn_sed} omega_dyn_sed_contents)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${tex_dyn_sed_contents}")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${omega_dyn_sed_contents}")

set(TANGLE_FLAGS -omega)

create_web_app(Omega)

target_link_libraries(${omega_dll_name}
  ${kpsemu_dll_name}
  ${gnu_dll_name}
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
	${omega_web_file}
  VERBATIM
)

add_custom_target(dev ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/omega-n.web
)
