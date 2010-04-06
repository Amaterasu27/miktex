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

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

if(LINK_EVERYTHING_STATICALLY)
  set(mf_target_name ${mf_lib_name})
else(LINK_EVERYTHING_STATICALLY)
  set(mf_target_name ${mf_dll_name})
endif(LINK_EVERYTHING_STATICALLY)

set(${mf_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/mf.defaults.h
  screen.cpp
  screen.h
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mf.defaults.h
  COMMAND ${cfg_exe} --print-classes ${mf_defaults} > mf.defaults.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${cfg_exe} ${mf_defaults}
  VERBATIM
)

set(mf_changefiles
  ${CMAKE_CURRENT_SOURCE_DIR}/mf-miktex.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/mf-miktex-pool.ch
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mf-miktex_2.ch
  COMMAND ${tie_exe}
	-c ${CMAKE_CURRENT_BINARY_DIR}/mf-miktex_2.ch
	${CMAKE_CURRENT_SOURCE_DIR}/mf.web
	${mf_changefiles}	
  DEPENDS
    ${tie_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/mf.web
	${mf_changefiles}
  VERBATIM
)

set(mf_change_file ${CMAKE_CURRENT_BINARY_DIR}/mf-miktex_2.ch)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/mf_pool.cpp
  COMMAND
	${inipool_exe} 
		${CMAKE_CURRENT_BINARY_DIR}/mf.pool
		mf-miktex.h
		> ${CMAKE_CURRENT_BINARY_DIR}/mf_pool.cpp
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${inipool_exe}
    ${CMAKE_CURRENT_BINARY_DIR}/mf.pool
)

set(${mf_target_name}_sources
  ${${mf_dll_name}_sources}
  ${CMAKE_CURRENT_BINARY_DIR}/mf_pool.cpp
)

create_web_app(METAFONT mf)

set_source_files_properties(
  ${CMAKE_CURRENT_SOURCE_DIR}/screen.cpp
  PROPERTIES COMPILE_FLAGS
	"-DMIKTEX_METAFONT -DMIKTEX_MF -DMFCLASS=${mf_class} -DMFAPP=${mf_app}"
)

create_exe_alias(inimf mf --initialize)
create_exe_alias(virmf mf)

set(trapmf_dll_name "trapmf_")

set(trapmf_web_file ${CMAKE_CURRENT_SOURCE_DIR}/mf.web)
set(trapmf_change_file ${CMAKE_CURRENT_BINARY_DIR}/trapmf-miktex.ch)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/trapmf-miktex.ch
  COMMAND
	${tie_exe} -c ${CMAKE_CURRENT_BINARY_DIR}/trapmf-miktex.ch
		${CMAKE_CURRENT_SOURCE_DIR}/mf.web
		${CMAKE_CURRENT_SOURCE_DIR}/mf-miktex.ch
		${CMAKE_CURRENT_SOURCE_DIR}/mf-miktex-pool.ch
		${CMAKE_CURRENT_SOURCE_DIR}/mf-miktex-trap.ch
  DEPENDS
    ${tie_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/mf.web
	${CMAKE_CURRENT_SOURCE_DIR}/mf-miktex.ch
	${CMAKE_CURRENT_SOURCE_DIR}/mf-miktex-pool.ch
	${CMAKE_CURRENT_SOURCE_DIR}/mf-miktex-trap.ch
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/trapmf-screen.cpp
  COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_CURRENT_SOURCE_DIR}/screen.cpp
	     ${CMAKE_CURRENT_BINARY_DIR}/trapmf-screen.cpp
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/screen.cpp
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/trapmf_pool.cpp
  COMMAND
	${inipool_exe} 
		${CMAKE_CURRENT_BINARY_DIR}/trapmf.pool
		mf-miktex.h
		> ${CMAKE_CURRENT_BINARY_DIR}/trapmf_pool.cpp
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${inipool_exe}
    ${CMAKE_CURRENT_BINARY_DIR}/trapmf.pool
)

set(${trapmf_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/mf.defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/trapmf_pool.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/trapmf-screen.cpp
  screen.h
)

create_web_app(METAFONT trapmf)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/trapmf-screen.cpp
  PROPERTIES COMPILE_FLAGS
	"-DMIKTEX_METAFONT -DMIKTEX_TRAPMF -DTRAPMFCLASS=${trapmf_class} -DTRAPMFAPP=${trapmf_app} -DTRAPMFDATA=${trapmf_data}"
)

set(trapmf_options)
set(trapmf_options "${trapmf_options} -error-line=64")
set(trapmf_options "${trapmf_options} -half-error-line=32")
set(trapmf_options "${trapmf_options} -max-print-line=72")
set(trapmf_options "${trapmf_options} -main-memory=3000")

set(trap_dir ${CMAKE_CURRENT_SOURCE_DIR}/trap)

configure_file(
  traptest.perl.in
  ${CMAKE_CURRENT_BINARY_DIR}/traptest.perl
)

add_custom_target(mf-traptest
  COMMAND ${PERL_EXE} ${CMAKE_CURRENT_BINARY_DIR}/traptest.perl
  DEPENDS trapmf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
)

## dev targets

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/mf-n.web
  COMMAND ${webn_exe}
	< ${mf_web_file}
	> ${CMAKE_CURRENT_BINARY_DIR}/mf-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${webn_exe}
	${mf_web_file}
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/mf-miktex.web
	${CMAKE_CURRENT_BINARY_DIR}/mf-miktex-n.web
  COMMAND ${tie_exe} -m ${CMAKE_CURRENT_BINARY_DIR}/mf-miktex.web
		${mf_web_file}
		${mf_change_file}
  COMMAND ${webn_exe}
	< ${CMAKE_CURRENT_BINARY_DIR}/mf-miktex.web
	> ${CMAKE_CURRENT_BINARY_DIR}/mf-miktex-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${tie_exe}
    ${webn_exe}
	${mf_web_file}
	${mf_change_file}
  VERBATIM
)

add_custom_target(mf-dev ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/mf-n.web
	${CMAKE_CURRENT_BINARY_DIR}/mf-miktex-n.web
)
