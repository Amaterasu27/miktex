## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2009-2014 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}/src/hunspell
)

if(NATIVE_WINDOWS)
  include_directories(BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/win_api
  )
endif(NATIVE_WINDOWS)

add_definitions(-DBUILDING_LIBHUNSPELL)

set(${hunspell_dll_name}_sources
  src/hunspell/affentry.cxx
  src/hunspell/affentry.hxx
  src/hunspell/affixmgr.cxx
  src/hunspell/affixmgr.hxx
  src/hunspell/atypes.hxx
  src/hunspell/baseaffix.hxx
  src/hunspell/csutil.cxx
  src/hunspell/csutil.hxx
  src/hunspell/dictmgr.cxx
  src/hunspell/dictmgr.hxx
  src/hunspell/filemgr.cxx
  src/hunspell/filemgr.hxx
  src/hunspell/hashmgr.cxx
  src/hunspell/hashmgr.hxx
  src/hunspell/htypes.hxx
  src/hunspell/hunspell.cxx
  src/hunspell/hunspell.h
  src/hunspell/hunspell.hxx
  src/hunspell/hunvisapi.h
  src/hunspell/hunzip.cxx
  src/hunspell/hunzip.hxx
  src/hunspell/langnum.hxx
  src/hunspell/phonet.cxx
  src/hunspell/phonet.hxx
  src/hunspell/replist.cxx
  src/hunspell/replist.hxx
  src/hunspell/suggestmgr.cxx
  src/hunspell/suggestmgr.hxx
  src/hunspell/w_char.hxx
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/hunspell.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/hunspell.rc
  )
  set(${hunspell_dll_name}_sources ${${hunspell_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/hunspell.rc
  )
endif(NATIVE_WINDOWS)

add_library(${hunspell_dll_name} SHARED ${${hunspell_dll_name}_sources})

target_link_libraries(${hunspell_dll_name}
  PRIVATE
    ${utf8wrap_dll_name}
)

set_target_properties(${hunspell_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${hunspell_dll_name})

install(TARGETS ${hunspell_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
