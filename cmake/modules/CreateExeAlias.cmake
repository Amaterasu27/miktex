## CreateExeAlias.cmake
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

macro(create_exe_alias _name _exe)
  configure_file(
    ${alias_cpp}
    ${CMAKE_CURRENT_BINARY_DIR}/${_name}alias.cpp
    COPYONLY
  )
  add_executable (${_name} ${CMAKE_CURRENT_BINARY_DIR}/${_name}alias.cpp)
  set(_pre_argv "")
  foreach (_a ${ARGN})
    if(_pre_argv STREQUAL "")
      set(_pre_argv "\\\"${_a}\\\"")
    else(_pre_argv STREQUAL "")
      set(_pre_argv "${_pre_argv},\\\"${_a}\\\"")
    endif(_pre_argv STREQUAL "")
  endforeach(_a)
  if(_pre_argv STREQUAL "")
    set_source_files_properties(
      ${CMAKE_CURRENT_BINARY_DIR}/${_name}alias.cpp
      COMPILE_FLAGS "-DREAL_NAME=\\\"${_exe}\\\""
    )
  else(_pre_argv STREQUAL "")
    set_source_files_properties(
      ${CMAKE_CURRENT_BINARY_DIR}/${_name}alias.cpp
      COMPILE_FLAGS "-DREAL_NAME=\\\"${_exe}\\\" -DPRE_ARGV=${_pre_argv}"
    )
  endif(_pre_argv STREQUAL "")
  target_link_libraries(${_name} ${core_dll_name})
endmacro(create_exe_alias)
