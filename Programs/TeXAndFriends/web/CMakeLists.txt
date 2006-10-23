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

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(tangleapp g_TANGLEApp)
set(tangleclass TANGLE)
set(tangledata g_TANGLEData)
set(tangleexport MiKTeX_TANGLE)
set(tanglemain RunTANGLE)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/inittangle.p
  ${CMAKE_CURRENT_BINARY_DIR}/tangle.p
  COPYONLY
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/tanglemain.cpp
  "#define C4PEXTERN extern
#include \"tangle.h\"
#include \"tangle-miktex.h\"
MIKTEX_DEFINE_WEBAPP(${tangleexport},
		     ${tangleclass},
		     ${tangleapp},
                     ${tanglemain},
                     ${tangledata})"
)

add_definitions(
  -DTANGLEAPP=${tangleapp}
  -DTANGLECLASS=${tangleclass}
  -DTANGLEDATA=${tangledata}
)

set(${tangle_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/tangle.cc
  ${CMAKE_CURRENT_BINARY_DIR}/tangle.h
  ${CMAKE_CURRENT_BINARY_DIR}/tanglemain.cpp
)

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/tangle.cc
  ${CMAKE_CURRENT_BINARY_DIR}/tangle.h
  PROPERTIES GENERATED TRUE
)

add_custom_command(
  OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/tangle.cc
	${CMAKE_CURRENT_BINARY_DIR}/tangle.h
  COMMAND ${c4p_exe}
	--dll
	--entry-name=${tanglemain}
	--include-filename=tangle-miktex.h
	--one=tangle
	--var-name-prefix=m_
	--var-struct=${tangledata}
	-C
	${CMAKE_CURRENT_BINARY_DIR}/tangle.p
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tangle.p
  VERBATIM
)

add_library(${tangle_dll_name} SHARED ${${tangle_dll_name}_sources})

target_link_libraries(${tangle_dll_name}
  ${app_dll_name}
  ${core_dll_name}
  ${popt_dll_name}
  ${texmf_dll_name}
)

#add_executable(tangle ${alias_cpp})

#set_source_file_properties(${alias_cpp})
