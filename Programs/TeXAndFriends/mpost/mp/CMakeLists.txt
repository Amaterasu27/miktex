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

set(${mp_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/mpost.defaults.h
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mpost.defaults.h
  COMMAND ${cfg_exe} --print-classes ${mpost_defaults} > mpost.defaults.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${mpst_defaults}
  VERBATIM
)

create_web_app(MetaPost mp mpost ${memdir})

set(trapmp_dll_name "trapmp_")

set(trapmp_web_file ${CMAKE_CURRENT_SOURCE_DIR}/mp.web)
set(trapmp_change_file ${CMAKE_CURRENT_BINARY_DIR}/trapmp-miktex.ch)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/trapmp-miktex.ch
  COMMAND
	${tie_exe} -c ${CMAKE_CURRENT_BINARY_DIR}/trapmp-miktex.ch
		${CMAKE_CURRENT_SOURCE_DIR}/mp.web
		${CMAKE_CURRENT_SOURCE_DIR}/mp-miktex.ch
		${CMAKE_CURRENT_SOURCE_DIR}/mp-miktex-trap.ch
  DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/mp.web
	${CMAKE_CURRENT_SOURCE_DIR}/mp-miktex.ch
	${CMAKE_CURRENT_SOURCE_DIR}/mp-miktex-trap.ch
)

create_web_app(MetaPost trapmp trapmp ${memdir})

set(trapmp_options)
set(trapmp_options "${trapmp_options} -error-line=64")
set(trapmp_options "${trapmp_options} -half-error-line=32")
set(trapmp_options "${trapmp_options} -max-print-line=72")
set(trapmp_options "${trapmp_options} -mem-max=4500")
set(trapmp_options "${trapmp_options} -mem-top=4500")

set(trap_dir ${CMAKE_CURRENT_SOURCE_DIR}/trapdir)

configure_file(
  traptest.perl.in
  ${CMAKE_CURRENT_BINARY_DIR}/traptest.perl
)

add_custom_target(traptest
  COMMAND ${PERL_EXE} ${CMAKE_CURRENT_BINARY_DIR}/traptest.perl
  DEPENDS trapmp
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
)
