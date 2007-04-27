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

add_definitions(
  -DMPCLASS=METAPOSTApp
  -DMPDATA=g_METAPOSTData
  -DMIKTEX_METAPOST
)

create_cpp_from_c(
  ${CMAKE_CURRENT_SOURCE_DIR}/avlstuff.c
  ${CMAKE_CURRENT_SOURCE_DIR}/mapfile.c
  ${CMAKE_CURRENT_SOURCE_DIR}/turningnumber.c
  ${CMAKE_CURRENT_SOURCE_DIR}/utils.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writeenc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writefont.c
  ${CMAKE_CURRENT_SOURCE_DIR}/writet1.c
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mpost.defaults.h
  COMMAND ${cfg_exe} --print-classes ${mpost_defaults} > mpost.defaults.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${mpst_defaults}
  VERBATIM
)

set(mp_change_files
  ${CMAKE_CURRENT_SOURCE_DIR}/fntemplate.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/color.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/gstate.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/antepost.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/psout.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/loadpool.ch
  ${CMAKE_CURRENT_SOURCE_DIR}/mpbugs.ch
)

add_executable(makecpool EXCLUDE_FROM_ALL
  makecpool.c
)

get_target_property(makecpool_exe makecpool LOCATION)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/loadpool.cpp
  COMMAND ${makecpool_exe}
    ${CMAKE_CURRENT_BINARY_DIR}/mp.pool mplib.h
    > ${CMAKE_CURRENT_BINARY_DIR}/loadpool.cpp
  DEPENDS
    makecpool
    ${CMAKE_CURRENT_BINARY_DIR}/mp.pool
)

set(${mp_dll_name}_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/avl.c
  ${CMAKE_CURRENT_BINARY_DIR}/avlstuff.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/mapfile.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/mpost.defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/turningnumber.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/utils.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writeenc.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writefont.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writet1.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/loadpool.cpp
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/loadpool.cpp
  PROPERTIES GENERATED TRUE
)

set(mp_web_file ${CMAKE_CURRENT_SOURCE_DIR}/mp.web)
set(mp_header_file ${CMAKE_CURRENT_BINARY_DIR}/mpd.h)

create_web_app(MetaPost mp mpost ${memdir})

target_link_libraries(${mp_dll_name}
  ${kpsemu_dll_name}
)

set(trapmp_dll_name "trapmp_")

set(trapmp_web_file ${CMAKE_CURRENT_SOURCE_DIR}/mp.web)
set(trapmp_header_file ${CMAKE_CURRENT_BINARY_DIR}/trapmpd.h)
set(trapmp_change_file ${CMAKE_CURRENT_BINARY_DIR}/trapmp-miktex.ch)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/trapmp-miktex.ch
  COMMAND
	${tie_exe} -c ${trapmp_change_file}
		${trapmp_web_file}
		${CMAKE_CURRENT_SOURCE_DIR}/mp-miktex.ch
		${CMAKE_CURRENT_SOURCE_DIR}/mp-miktex-trap.ch
  DEPENDS
	${trapmp_web_file}
	${CMAKE_CURRENT_SOURCE_DIR}/mp-miktex.ch
	${CMAKE_CURRENT_SOURCE_DIR}/mp-miktex-trap.ch
)

set(${trapmp_dll_name}_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/avl.c
  ${CMAKE_CURRENT_BINARY_DIR}/avlstuff.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/mapfile.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/mpost.defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/turningnumber.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/utils.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writeenc.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writefont.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/writet1.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/loadpool.cpp
)

create_web_app(MetaPost trapmp trapmp ${memdir})

target_link_libraries(${trapmp_dll_name}
  ${kpsemu_dll_name}
)

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
