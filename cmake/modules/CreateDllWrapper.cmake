## CreateDllWrapper.cmake
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

macro(create_dll_wrapper _name _dll)
  string(TOLOWER "${_name}" _name_l)
  string(TOUPPER "${_name}" _name_u)
  configure_file(
    ${alias_cpp}
    ${CMAKE_CURRENT_BINARY_DIR}/${_name}wrapper.cpp
    COPYONLY
  )
  add_executable (${_name} ${CMAKE_CURRENT_BINARY_DIR}/${_name}wrapper.cpp)
  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${_name}wrapper.cpp
    COMPILE_FLAGS "-DDLLMAIN=${_name_l}"
  )
  target_link_libraries(${_name} ${_dll})
endmacro(create_dll_wrapper)
