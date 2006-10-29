## CreateCppFromC.cmake
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

macro(create_cpp_from_c)
  foreach(_c ${ARGN})
    get_filename_component(_n ${_c} NAME_WE)
    set(_cpp ${_n}.cpp)
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_cpp}
      COMMAND ${CP_EXE}
	${CMAKE_CURRENT_SOURCE_DIR}/${_c}
	${CMAKE_CURRENT_BINARY_DIR}/${_cpp}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_c}
      VERBATIM
    )
    set_source_files_properties(
      ${CMAKE_CURRENT_BINARY_DIR}/${_cpp}
      PROPERTIES GENERATED TRUE
    )
  endforeach(_c)
endmacro(create_cpp_from_c)
