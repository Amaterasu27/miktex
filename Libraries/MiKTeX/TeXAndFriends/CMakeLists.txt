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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(${texmf_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/mfapp.defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/texapp.defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/texmfapp.defaults.h
  StdAfx.cpp
  StdAfx.h
  c4plib.cpp
  c4pstart.cpp
  inputline.cpp
  internal.h
  mfapp.cpp
  mfmpi386asm.cpp
  texapp.cpp
  texmf-version.h
  texmfapp.cpp
  texmflib.cpp
  webapp.cpp
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/mfapp.defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/texapp.defaults.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/miktex/texmfapp.defaults.h
  PROPERTIES GENERATED TRUE
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/miktex)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/include/miktex/mfapp.defaults.h
  COMMAND ${cfg_exe}
	--print-classes
	${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/mfapp.defaults
	> mfapp.defaults.h
  DEPENDS
	${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/mfapp.defaults
  MAIN_DEPENDENCY
	${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/mfapp.defaults
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}/include/miktex
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/include/miktex/texapp.defaults.h
  COMMAND ${cfg_exe}
	--print-classes
	${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/texapp.defaults
	> texapp.defaults.h
  DEPENDS
	${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/texapp.defaults
  MAIN_DEPENDENCY
	${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/texapp.defaults
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}/include/miktex
  VERBATIM
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/include/miktex/texmfapp.defaults.h
  COMMAND ${cfg_exe}
	--print-classes
	${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/texmfapp.defaults
	> texmfapp.defaults.h
  DEPENDS
	${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/texmfapp.defaults
  MAIN_DEPENDENCY
	${CMAKE_SOURCE_DIR}/${rel_defaults_dir}/texmfapp.defaults
  WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}/include/miktex
  VERBATIM
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/texmf.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/texmf.rc
  )
  set(${texmf_dll_name}_sources ${${texmf_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/texmf.rc
  )
endif(NATIVE_WINDOWS)

add_library(${texmf_dll_name} SHARED ${${texmf_dll_name}_sources})

target_link_libraries(${texmf_dll_name}
  ${app_dll_name}
  ${bzip2_dll_name}
  ${core_dll_name}
  ${mpm_dll_name}
  ${popt_dll_name}
  ${zlib_dll_name}
)

if(NATIVE_WINDOWS AND MFC_FOUND)
  target_link_libraries(${texmf_dll_name}
    ${ui_dll_name}
  )
endif(NATIVE_WINDOWS AND MFC_FOUND)

install(TARGETS ${texmf_dll_name} DESTINATION "${bindir}")
