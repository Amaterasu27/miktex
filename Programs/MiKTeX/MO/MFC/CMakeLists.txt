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

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/res/mo.manifest.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/mo.manifest
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_MFC_FLAG 2)

add_definitions(
  -D_AFXDLL
)

set(mo_mfc_sources
  FormatDefinitionDialog.cpp
  FormatDefinitionDialog.h
  LanguageDefinitionDialog.cpp
  LanguageDefinitionDialog.h
  PropPageFormats.cpp
  PropPageFormats.h
  PropPageGeneral.cpp
  PropPageGeneral.h
  PropPageLanguages.cpp
  PropPageLanguages.h
  PropPagePackages.cpp
  PropPagePackages.h
  PropPageRoots.cpp
  PropPageRoots.h
  PropSheet.cpp
  PropSheet.h
  StdAfx.cpp
  StdAfx.h
  mo-version.h
  mo.cpp
  mo.h
  resource.h
  resource.hm
)

if(NATIVE_WINDOWS)
  set(mo_mfc_sources ${mo_mfc_sources} mo.rc)
endif(NATIVE_WINDOWS)

add_executable(mo WIN32 ${mo_mfc_sources})

target_link_libraries(mo
  ${core_dll_name}
  ${mo_dll_name}
  ${ui_dll_name}
)

get_target_property(mo_mfc_exe mo LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET mo
    POST_BUILD
    COMMAND mt -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/mo.manifest
	-updateresource:${mo_mfc_exe}\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(TARGETS mo DESTINATION ${bindir})