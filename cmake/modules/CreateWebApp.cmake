## CreateWebApp.cmake
##
## Copyright (C) 2006-2014 Christian Schenk
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

  hide_symbols()

  string(TOLOWER "${_name}" _name_l)
  string(TOUPPER "${_name}" _name_u)

  if(${ARGC} GREATER 1)
    set(_short_name ${ARGV1})
  else(${ARGC} GREATER 1)
    set(_short_name ${_name})
  endif(${ARGC} GREATER 1)

  string(TOLOWER "${_short_name}" _short_name_l)
  string(TOUPPER "${_short_name}" _short_name_u)

  if(${ARGC} GREATER 2)
    set(_invocation_name ${miktex_prefix}${ARGV2})
    set(_alt_invocation_name ${ARGV2})
  else(${ARGC} GREATER 2)
    set(_invocation_name ${miktex_prefix}${_short_name_l})
    set(_alt_invocation_name ${_short_name_l})
  endif(${ARGC} GREATER 2)

  if(${ARGC} GREATER 3)
    set(_do_install ${ARGV3})
  else(${ARGC} GREATER 3)
    set(_do_install TRUE)
  endif(${ARGC} GREATER 3)

  set(${_short_name_l}_app g_${_name_u}App)
  set(${_short_name_l}_class ${_name_u})
  set(${_short_name_l}_entry Run${_name_u})

  if(NOT ${_short_name_l}_web_file)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}.web)
      set(${_short_name_l}_web_file
	${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}.web)
    else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}.web)
      set(${_short_name_l}_web_file
	${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.web)
    endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}.web)
  endif(NOT ${_short_name_l}_web_file)

  if(NOT ${_short_name_l}_change_file)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}-miktex.ch)
      set(${_short_name_l}_change_file
	${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}-miktex.ch)
    else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}-miktex.ch)
      set(${_short_name_l}_change_file ${dev_null})
    endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}-miktex.ch)
  endif(NOT ${_short_name_l}_change_file)

  if(NOT ${_short_name_l}_header_file)
    set(${_short_name_l}_header_file
      ${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.h)
  endif(NOT ${_short_name_l}_header_file)

  if(NOT ${_short_name_l}_include_file)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}-miktex.h)
      set(${_short_name_l}_include_file
	${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}-miktex.h)
    else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}-miktex.h)
      set(${_short_name_l}_include_file
	${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}-miktex.h)
      if(EXISTS ${${_short_name_l}_include_file})
      else(EXISTS ${${_short_name_l}_include_file})
	file(WRITE ${${_short_name_l}_include_file}
	  "#include <miktex/TeXAndFriends/WebApp>
using namespace MiKTeX::TeXAndFriends;
class ${${_short_name_l}_class} : public WebApp {};"
        )
      endif(EXISTS ${${_short_name_l}_include_file})
    endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}-miktex.h)
  endif(NOT ${_short_name_l}_include_file)

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}main.cpp
    "#define C4PEXTERN extern
#include \"${${_short_name_l}_header_file}\"
#include \"${${_short_name_l}_include_file}\"
MIKTEX_DEFINE_WEBAPP(MiKTeX_${_name_u},
		     ${${_short_name_l}_class},
		     ${${_short_name_l}_app},
                     ${${_short_name_l}_entry})
"
  )

  configure_file(
    ${alias_cpp}
    ${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}wrapper.cpp
    COPYONLY
  )

  if(LINK_EVERYTHING_STATICALLY)
    set(_target_name ${${_short_name_l}_lib_name})
  else(LINK_EVERYTHING_STATICALLY)
    set(_target_name ${${_short_name_l}_dll_name})
  endif(LINK_EVERYTHING_STATICALLY)

  set(${_target_name}_sources
    ${${_target_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.cc
    ${${_short_name_l}_header_file}
    ${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}main.cpp
  )

  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.cc
    ${${_short_name_l}_header_file}
    PROPERTIES GENERATED TRUE
  )

  set_source_files_properties(
    ${${_short_name_l}_header_file}
    PROPERTIES
      GENERATED TRUE
      HEADER_FILE_ONLY TRUE
  )

  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.cc
    ${${_short_name_l}_header_file}
    ${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}main.cpp
    PROPERTIES COMPILE_FLAGS
      "-DMIKTEX_${_name_u} -DMIKTEX_${_short_name_u} -D${_short_name_u}APP=${${_short_name_l}_app} -D${_short_name_u}CLASS=${${_short_name_l}_class}"
  )

  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}wrapper.cpp
    PROPERTIES COMPILE_FLAGS
      "-DDLLMAIN=MiKTeX_${_name_u} -DMIKTEX_${_name_u} -D_UNICODE"
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
	${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.cc
	${${_short_name_l}_header_file}
	${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}defs.h
    COMMAND ${c4p_exe}
	--def-filename=${_short_name_l}defs.h
	--dll
	--entry-name=${${_short_name_l}_entry}
	--include-filename=${${_short_name_l}_include_file}
	--header-file=${${_short_name_l}_header_file}
	--one=${_short_name_l}
	--using-namespace=MiKTeX::TeXAndFriends
	--var-name-prefix=${C4P_VAR_NAME_PREFIX}
	-C
	${C4P_FLAGS}
	${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.p
    COMMAND ${SED_EXE}
		-f ${_sed_script}
		${${_short_name_l}_header_file}
		> tmp
    COMMAND ${CMAKE_COMMAND} -E copy tmp ${${_short_name_l}_header_file}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
	${c4p_exe}
    	${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.p
    VERBATIM
  )

  if(NOT ${_short_name_l} STREQUAL "tangle")
    add_custom_command(
      OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.p
	${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.pool
      COMMAND
	${tangle_exe}
		${TANGLE_FLAGS}
		${${_short_name_l}_web_file}
		${${_short_name_l}_change_file}
		${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.p
		${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.pool
      DEPENDS
	${tangle_exe}
	${${_short_name_l}_web_file}
	${${_short_name_l}_change_file}
      VERBATIM
    )
  endif(NOT ${_short_name_l} STREQUAL "tangle")

  if(NATIVE_WINDOWS)
    if(EXISTS ${_short_name_l}.rc)
      set(${_target_name}_sources
	${${_target_name}_sources}
	${_short_name_l}.rc)
    endif(EXISTS ${_short_name_l}.rc)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}.rc.in)
      configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.rc
      )
      set(${_target_name}_sources
	${${_target_name}_sources}
	${CMAKE_CURRENT_BINARY_DIR}/${_short_name_l}.rc)
    endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_short_name_l}.rc.in)
  endif(NATIVE_WINDOWS)

  if(LINK_EVERYTHING_STATICALLY)
    add_library(${_target_name}
      STATIC ${${_target_name}_sources})
  else(LINK_EVERYTHING_STATICALLY)
    add_library(${_target_name}
      SHARED ${${_target_name}_sources})
    set_target_properties(${_target_name}
      PROPERTIES
      VERSION "${MIKTEX_SERIES_STR}"
      SOVERSION "1"
      )
    target_link_libraries(${_target_name}
      PRIVATE
        ${app_dll_name}
        ${core_dll_name}
        ${popt_dll_name}
        ${texmf_dll_name}
    )
    rebase(${_target_name})
  endif(LINK_EVERYTHING_STATICALLY)

  if(_do_install AND NOT LINK_EVERYTHING_STATICALLY OR INSTALL_STATIC_LIBRARIES)
    install(
      TARGETS ${_target_name}
      RUNTIME DESTINATION "${bindir}"
      LIBRARY DESTINATION "${libdir}"
      ARCHIVE DESTINATION "${libdir}"
    )
  endif(_do_install AND NOT LINK_EVERYTHING_STATICALLY OR INSTALL_STATIC_LIBRARIES)

  add_executable(${_invocation_name} ${_short_name_l}wrapper.cpp)

  set_target_properties(${_invocation_name}
    PROPERTIES
      VERSION "${MIKTEX_SERIES_STR}"
  )

  add_dependencies(${_invocation_name} ${_target_name})

  if(LINK_EVERYTHING_STATICALLY)
    target_link_libraries(${_invocation_name}
	  ${core_lib_name}
      ${_target_name}
    )
  else(LINK_EVERYTHING_STATICALLY)
    target_link_libraries(${_invocation_name}
      ${core_dll_name}
      ${_target_name}
    )
  endif(LINK_EVERYTHING_STATICALLY)

  merge_manifests(${_invocation_name} asInvoker)

  if(_do_install)
    install(
      TARGETS ${_invocation_name}
      RUNTIME DESTINATION "${bindir}"
      LIBRARY DESTINATION "${libdir}"
      ARCHIVE DESTINATION "${libdir}"
    )
  endif(_do_install)

  if(BUILDING_MIKTEX)
    if(${_invocation_name} STREQUAL "${_short_name_l}" OR ${_alt_invocation_name} STREQUAL "${_short_name_l}")
    else(${_invocation_name} STREQUAL "${_short_name_l}" OR ${_alt_invocation_name} STREQUAL "${_short_name_l}")
      add_executable(${_short_name_l} ${_short_name_l}wrapper.cpp)
      set_target_properties(${_short_name_l}
	PROPERTIES
	VERSION "${MIKTEX_SERIES_STR}"
      )
      merge_trustinfo_manifest(${_short_name_l} asInvoker)
      merge_common_controls_manifest(${_short_name_l})
      install(
        TARGETS ${_short_name_l}
	RUNTIME DESTINATION "${bindir}"
	LIBRARY DESTINATION "${libdir}"
	ARCHIVE DESTINATION "${libdir}"
      )
      add_dependencies(${_short_name_l} ${_target_name})
      target_link_libraries(${_short_name_l}
        ${core_dll_name}
        ${_target_name}
      )
    endif(${_invocation_name} STREQUAL "${_short_name_l}" OR ${_alt_invocation_name} STREQUAL "${_short_name_l}")

    add_executable(${_alt_invocation_name} ${_short_name_l}wrapper.cpp)
    set_target_properties(${_alt_invocation_name}
      PROPERTIES
      VERSION "${MIKTEX_SERIES_STR}"
    )
    target_link_libraries(${_alt_invocation_name}
      ${core_dll_name}
      ${_target_name}
    )
    merge_manifests(${_alt_invocation_name} asInvoker)
    install(
      TARGETS ${_alt_invocation_name}
      RUNTIME DESTINATION "${bindir}"
      LIBRARY DESTINATION "${libdir}"
      ARCHIVE DESTINATION "${libdir}"
    )
  endif(BUILDING_MIKTEX)

endmacro(create_web_app _name)
