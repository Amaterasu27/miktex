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

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}main.cpp
    "#define C4PEXTERN extern
#include \"${_name_l}.h\"
#include \"${_name_l}-miktex.h\"
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

  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}wrapper.cpp
    COMPILE_FLAGS "-DDLLMAIN=MiKTeX_${_name_u} -DDLLAPI=__stdcall"
  )

  add_definitions(
    -D${_name_u}APP=g_${_name_u}App
    -D${_name_u}CLASS=${_name_u}
    -D${_name_u}DATA=g_${_name_u}Data
  )

  set(${${_name_l}_dll_name}_sources
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.cc
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.h
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}main.cpp
  )

  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.cc
    ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.h
    PROPERTIES GENERATED TRUE
  )

  if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.h)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}-miktex.h
    "#include <miktex/webapp.h>
using namespace MiKTeX::TeXAndFriends;
class ${_name_u} : public WebApp {};"
    )
  endif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.h)

  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.cc
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.h
    COMMAND ${c4p_exe}
	--dll
	--entry-name=Run${_name_u}
	--include-filename=${_name_l}-miktex.h
	--one=${_name_l}
	--var-name-prefix=m_
	--var-struct=g_${_name_u}Data
	-C
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.p
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.p
    VERBATIM
  )

  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}.web)
    add_custom_command(
      OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.p
	${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.pool
      COMMAND
	${tangle_exe}
		${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}.web
		${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.ch
		${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.p
		${CMAKE_CURRENT_BINARY_DIR}/${_name_l}.pool
      DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}.web
	${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}-miktex.ch
      VERBATIM
    )
  endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_name_l}.web)

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

  install(
    TARGETS ${${_name_l}_dll_name} ${_name_l}
    DESTINATION ${bindir}
  )
    
endmacro(create_web_app _name)
