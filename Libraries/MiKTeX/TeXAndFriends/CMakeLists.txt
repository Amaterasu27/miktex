## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2009 Christian Schenk
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

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/C4P/config.h.cmake
  ${CMAKE_BINARY_DIR}/${rel_texmf_dir}/include/miktex/C4P/config.h
)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/config.h.cmake
  ${CMAKE_BINARY_DIR}/${rel_texmf_dir}/include/miktex/TeXAndFriends/config.h
)

hide_symbols()

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
  etexapp.cpp
  include/miktex/C4P/C4P.h
  include/miktex/TeXAndFriends/ETeXApp.h
  include/miktex/TeXAndFriends/ETeXApp.inl
  include/miktex/TeXAndFriends/MetafontApp.h
  include/miktex/TeXAndFriends/MetafontApp.inl
  include/miktex/TeXAndFriends/Prototypes.h
  include/miktex/TeXAndFriends/TeXApp.h
  include/miktex/TeXAndFriends/TeXApp.inl
  include/miktex/TeXAndFriends/TeXMFApp.h
  include/miktex/TeXAndFriends/TeXMFApp.inl
  include/miktex/TeXAndFriends/WebApp.h
  include/miktex/TeXAndFriends/WebApp.inl
  include/miktex/TeXAndFriends/WebAppInputLine.h
  include/miktex/TeXAndFriends/WebAppInputLine.inl
  inputline.cpp
  internal.h
  mfapp.cpp
  texapp.cpp
  texmf-version.h
  texmfapp.cpp
  texmflib.cpp
  webapp.cpp
)

if(NATIVE_WINDOWS)
  set(${texmf_dll_name}_sources ${${texmf_dll_name}_sources}
    mfmpi386asm.cpp
  )
endif(NATIVE_WINDOWS)

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

set_target_properties(${texmf_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_link_libraries(${texmf_dll_name}
  ${app_dll_name}
  ${core_dll_name}
  ${popt_dll_name}
)

rebase(${texmf_dll_name})

install(TARGETS ${texmf_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

install(
  FILES
    include/miktex/TeXAndFriends/ETeXApp
    include/miktex/TeXAndFriends/ETeXApp.h
    include/miktex/TeXAndFriends/ETeXApp.inl
    include/miktex/TeXAndFriends/MetafontApp
    include/miktex/TeXAndFriends/MetafontApp.h
    include/miktex/TeXAndFriends/MetafontApp.inl
    include/miktex/TeXAndFriends/Prototypes
    include/miktex/TeXAndFriends/Prototypes.h
    include/miktex/TeXAndFriends/TeXApp
    include/miktex/TeXAndFriends/TeXApp.h
    include/miktex/TeXAndFriends/TeXApp.inl
    include/miktex/TeXAndFriends/TeXMFApp
    include/miktex/TeXAndFriends/TeXMFApp.h
    include/miktex/TeXAndFriends/TeXMFApp.inl
    include/miktex/TeXAndFriends/WebApp
    include/miktex/TeXAndFriends/WebApp.h
    include/miktex/TeXAndFriends/WebApp.inl
    include/miktex/TeXAndFriends/WebAppInputLine
    include/miktex/TeXAndFriends/WebAppInputLine.h
    include/miktex/TeXAndFriends/WebAppInputLine.inl
  DESTINATION
    ${incdir}/miktex/TeXAndFriends
)
