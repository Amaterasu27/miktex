## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2006-2015 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

add_definitions(-DHAVE_CONFIG_H)

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_bmeps_dir}
)

add_executable(makebitcounts EXCLUDE_FROM_ALL makebitcounts.cpp)

merge_trustinfo_manifest(makebitcounts asInvoker)

set(${dvi_dll_name}_sources
  ${CMAKE_CURRENT_BINARY_DIR}/bitcounts.h
  Dib.cpp
  Dib.h
  Dvi.cpp
  DviChar.cpp
  DviChar.h
  DviFont.cpp
  DviFont.h
  DviPage.cpp
  Ghostscript.cpp
  Ghostscript.h
  PkChar.cpp
  PkChar.h
  PkFont.cpp
  PkFont.h
  PostScript.cpp
  PostScript.h
  StdAfx.cpp
  StdAfx.h
  Tfm.cpp
  Tfm.h
  VFont.cpp
  VFont.h
  VfChar.cpp
  VfChar.h
  color.cpp
  dvi-version.h
  graphics.cpp
  hypertex.cpp
  include/miktex/DVI/Dvi.h
  inliners.h
  internal.h
  misc.cpp
  ps.cpp
  special.cpp
  src.cpp
  tpic.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dvi.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/dvi.rc
  )
  set(${dvi_dll_name}_sources ${${dvi_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/dvi.rc
  )
endif()

set_source_files_properties(
  ${CMAKE_CURRENT_BINARY_DIR}/bitcounts.h
  PROPERTIES GENERATED TRUE
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bitcounts.h
  COMMAND $<TARGET_FILE:makebitcounts> > bitcounts.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS makebitcounts
  VERBATIM
)

add_library(${dvi_dll_name} SHARED ${${dvi_dll_name}_sources})

set_target_properties(${dvi_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_include_directories(${dvi_dll_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_dvi_dir}/include
)

target_link_libraries(${dvi_dll_name}
  PRIVATE
    ${bmeps_dll_name}
    ${core_dll_name}
  PUBLIC
    ${dib_dll_name}
)

delay_load(${dvi_dll_name} ${bmeps_dll_name})

rebase(${dvi_dll_name})

add_executable(dviscan dviscan.cpp)

merge_trustinfo_manifest(dviscan asInvoker)

add_dependencies(dviscan ${dvi_dll_name})

target_link_libraries(dviscan
  ${app_dll_name}
  ${core_dll_name}
  ${dvi_dll_name}
  ${getopt_dll_name}
  ${popt_dll_name}
)

install(TARGETS ${dvi_dll_name} dviscan
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

install(
  FILES
    include/miktex/DVI/Dvi
    include/miktex/DVI/Dvi.h
  DESTINATION
    ${incdir}/miktex/DVI
)
