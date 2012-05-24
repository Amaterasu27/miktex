## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2012 Christian Schenk
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

default_char_type_is_unsigned()

set(C4P_FLAGS --chars-are-unsigned)

set(tex_changefiles
  ${CMAKE_CURRENT_SOURCE_DIR}/mltex-miktex.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/enctex-miktex.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/tex-miktex-misc.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/tex-miktex-enctex.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/tex-miktex-hash.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/tex-miktex-hyph.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/tex-miktex-pool.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/tex-miktex-quiet.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/tex-miktex-src.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/tex-miktex-stat.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/tex-miktex-write18.ch
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.ch
  COMMAND ${tie_exe}
	-c ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.ch
	${CMAKE_CURRENT_SOURCE_DIR}/tex.web
	${tex_changefiles}
  DEPENDS
    ${tie_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/tex.web
	${tex_changefiles}
  VERBATIM
)

set(tex_change_file ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.ch)

if(LINK_EVERYTHING_STATICALLY)
  set(tex_target_name ${tex_lib_name})
else(LINK_EVERYTHING_STATICALLY)
  set(tex_target_name ${tex_dll_name})
endif(LINK_EVERYTHING_STATICALLY)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/tex_pool.cpp
  COMMAND
	${inipool_exe} 
		${CMAKE_CURRENT_BINARY_DIR}/tex.pool
		tex-miktex.h
		> ${CMAKE_CURRENT_BINARY_DIR}/tex_pool.cpp
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${inipool_exe}
    ${CMAKE_CURRENT_BINARY_DIR}/tex.pool
)

set(${tex_target_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/tex_pool.cpp
)

create_web_app(TeX)

if(NATIVE_WINDOWS)
  create_exe_alias(initex tex --initialize)
  create_exe_alias(virtex tex)
endif(NATIVE_WINDOWS)

set(triptex_dll_name "triptex_")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/triptex-miktex.ch
  COMMAND ${tie_exe}
	-c ${CMAKE_CURRENT_BINARY_DIR}/triptex-miktex.ch
	${CMAKE_CURRENT_SOURCE_DIR}/tex.web
	${tex_changefiles}
  DEPENDS
    ${tie_exe}
	${CMAKE_CURRENT_SOURCE_DIR}/tex.web
	${tex_changefiles}
  VERBATIM
)

set(triptex_web_file ${CMAKE_CURRENT_SOURCE_DIR}/tex.web)
set(triptex_change_file ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.ch)

if(LINK_EVERYTHING_STATICALLY)
  set(triptex_target_name ${triptex_lib_name})
else(LINK_EVERYTHING_STATICALLY)
  set(triptex_target_name ${triptex_dll_name})
endif(LINK_EVERYTHING_STATICALLY)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/triptex_pool.cpp
  COMMAND
	${inipool_exe} 
		${CMAKE_CURRENT_BINARY_DIR}/triptex.pool
		tex-miktex.h
		> ${CMAKE_CURRENT_BINARY_DIR}/triptex_pool.cpp
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${inipool_exe}
    ${CMAKE_CURRENT_BINARY_DIR}/triptex.pool
)

set(${triptex_target_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/triptex_pool.cpp
)

create_web_app(TripTeX)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/triptex_pool.cpp
  PROPERTIES COMPILE_FLAGS
	"-DMIKTEX_TRIPTEX"
)

set(triptex_options)
set(triptex_options "${triptex_options} -buf-size=500")
set(triptex_options "${triptex_options} -error-line=64")
set(triptex_options "${triptex_options} -font-max=75")
set(triptex_options "${triptex_options} -font-mem-size=20000")
set(triptex_options "${triptex_options} -half-error-line=32")
set(triptex_options "${triptex_options} -main-memory=3000")
set(triptex_options "${triptex_options} -max-in-open=6")
set(triptex_options "${triptex_options} -max-print-line=72")
set(triptex_options "${triptex_options} -max-strings=3000")
set(triptex_options "${triptex_options} -mem-bot=1")
set(triptex_options "${triptex_options} -nest-size=40")
set(triptex_options "${triptex_options} -param-size=60")
set(triptex_options "${triptex_options} -pool-size=32000")
set(triptex_options "${triptex_options} -save-size=600")
set(triptex_options "${triptex_options} -stack-size=200")
set(triptex_options "${triptex_options} -strict")
set(triptex_options "${triptex_options} -string-vacancies=8000")
set(triptex_options "${triptex_options} -trie-size=8000")

set(trip_dir ${CMAKE_CURRENT_SOURCE_DIR}/trip)

configure_file(
  triptest.perl.in
  ${CMAKE_CURRENT_BINARY_DIR}/triptest.perl
)

add_custom_target(triptest
  COMMAND ${PERL_EXE} ${CMAKE_CURRENT_BINARY_DIR}/triptest.perl
  DEPENDS triptex
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
)

## dev targets

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/tex-n.web
  COMMAND ${webn_exe}
	< ${tex_web_file}
	> ${CMAKE_CURRENT_BINARY_DIR}/tex-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${webn_exe}
	${tex_web_file}
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
	${CMAKE_CURRENT_BINARY_DIR}/tex-miktex-n.web
  COMMAND ${tie_exe} -m ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
		${tex_web}
		${tex_change_file}
  COMMAND ${webn_exe}
	< ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
	> ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex-n.web
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS
    ${tie_exe}
    ${webn_exe}
	${tex_web_file}
	${tex_change_file}
  VERBATIM
)

add_custom_target(tex-dev ALL
    DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/tex-n.web
	${CMAKE_CURRENT_BINARY_DIR}/tex-miktex-n.web
)
