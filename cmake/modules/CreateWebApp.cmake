## CreateWebApp.cmake
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

macro(create_web_app _name)

  string(TOLOWER "${_name}" _name_l)
  string(TOUPPER "${_name}" _name_u)

  if(NOT ${_name_l}_web_file)
    set(${_name_l}_web_file
      ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}.web)
  endif(NOT ${_name_l}_web_file)

  if(NOT ${_name_l}_change_file)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.ch)
      set(${_name_l}_change_file
	${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.ch)
    else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.ch)
      set(${_name_l}_change_file ${dev_null})
    endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.ch)
  endif(NOT ${_name_l}_change_file)

  if(NOT ${_name_l}_header_file)
    set(${_name_l}_header_file
      ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.h)
  endif(NOT ${_name_l}_header_file)

  if(NOT ${_name_l}_include_file)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.h)
      set(${_name_l}_include_file
	${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.h)
    else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.h)
      set(${_name_l}_include_file
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}-miktex.h)
      if(EXISTS ${${_name_l}_include_file})
      else(EXISTS ${${_name_l}_include_file})
	file(WRITE ${${_name_l}_include_file}
	  "#include <miktex/webapp.h>
using namespace MiKTeX::TeXAndFriends;
class ${_name_u} : public WebApp {};"
        )
      endif(EXISTS ${${_name_l}_include_file})
    endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.h)
  endif(NOT ${_name_l}_include_file)

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}main.cpp
    "#define C4PEXTERN extern
#include \"${${_name_l}_header_file}\"
#include \"${${_name_l}_include_file}\"
MIKTEX_DEFINE_WEBAPP(MiKTeX_${_name_u},
		     ${_name_u},
		     g_${_name_u}App,
                     Run${_name_u},
                     g_${_name_u}Data)"
  )

  configure_file(
    ${alias_cpp}
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}wrapper.cpp
    COPYONLY
  )

  set(${${_name_l}_dll_name}_sources
    ${${${_name_l}_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.cc
    ${${_name_l}_header_file}
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}main.cpp
  )

  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.cc
    ${${_name_l}_header_file}
    PROPERTIES GENERATED TRUE
  )

  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.cc
    ${${_name_l}_header_file}
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}main.cpp
    PROPERTIES COMPILE_FLAGS
      "-DMIKTEX_${_name_u} -D${_name_u}APP=g_${_name_u}App -D${_name_u}CLASS=${_name_u} -D${_name_u}DATA=g_${_name_u}Data"
  )

  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}wrapper.cpp
    PROPERTIES COMPILE_FLAGS
      "-DDLLMAIN=MiKTeX_${_name_u} -DDLLAPI=__stdcall -DMIKTEX_${_name_u}"
  )

  if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed)
    set(_sed_script ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed)
  elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dyn.sed)
    set(_sed_script ${CMAKE_CURRENT_SOURCE_DIR}/dyn.sed)
  else(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed)
    set(_sed_script /dev/null)
  endif(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed)

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.cc
	${${_name_l}_header_file}
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}defs.h
    COMMAND ${c4p_exe}
	--def-filename=${_name_l}defs.h
	--dll
	--entry-name=Run${_name_u}
	--include-filename=${${_name_l}_include_file}
	--header-file=${${_name_l}_header_file}
	--one=${_name_l}
	--var-name-prefix=m_
	--var-struct=g_${_name_u}Data
	-C
	${C4P_FLAGS}
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.p
    COMMAND ${SED_EXE}
		-f ${_sed_script}
		${${_name_l}_header_file}
		> tmp
    COMMAND ${CP_EXE} tmp ${${_name_l}_header_file}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.p
    VERBATIM
  )

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.p
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.pool
    COMMAND
	${tangle_exe}
		${${_name_l}_web_file}
		${${_name_l}_change_file}
		${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.p
		${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.pool
    DEPENDS
	${${_name_l}_web_file}
	${${_name_l}_change_file}
    VERBATIM
  )

  if(NATIVE_WINDOWS)
    if(EXISTS ${_name_l}.rc)
      set(${${_name_l}_dll_name}_sources
	${${${_name_l}_dll_name}_sources}
	${_name_l}.rc)
    endif(EXISTS ${_name_l}.rc)
  endif(NATIVE_WINDOWS)

  add_library(${${_name_l}_dll_name} SHARED ${${${_name_l}_dll_name}_sources})

  target_link_libraries(${${_name_l}_dll_name}
    ${app_dll_name}
    ${core_dll_name}
    ${popt_dll_name}
    ${texmf_dll_name}
  )

  add_executable(${_name_l} ${_name_l}wrapper.cpp)

  add_dependencies(${_name_l} ${${_name_l}_dll_name})

  target_link_libraries(${_name_l}
    ${core_dll_name}
    ${${_name_l}_dll_name}
  )

  install(TARGETS
    ${${_name_l}_dll_name} ${_name_l}
    DESTINATION ${bindir}
  )

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.pool
    DESTINATION ${formatdir}
  )
    
endmacro(create_web_app _name)
