## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2011-2015 Christian Schenk
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

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/mo.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/mo.rc
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_MFC_FLAG 2)

add_definitions(
  -D_AFXDLL
  -D_UNICODE -DUNICODE
)

set(mo_mfc_sources
  FormatDefinitionDialog.cpp
  FormatDefinitionDialog.h
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
  set(mo_mfc_sources ${mo_mfc_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/mo.rc)
endif(NATIVE_WINDOWS)

set(libs
  ${core_dll_name}
  ${mpm_dll_name}
  ${ui_mfc_dll_name}
)

if(NATIVE_WINDOWS)
  set(libs ${libs}
    htmlhelp.lib
  )
endif(NATIVE_WINDOWS)

add_executable(mo WIN32 ${mo_mfc_sources})

merge_trustinfo_manifest(mo asInvoker)

if(NATIVE_WINDOWS)
  add_link_flags(mo "/ENTRY:\"wWinMainCRTStartup\"")
endif(NATIVE_WINDOWS)

target_link_libraries(mo ${libs})

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET mo
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/mo.manifest
	-updateresource:$<TARGET_FILE:mo>\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(TARGETS mo DESTINATION ${bindir})

add_executable(mo${miktex_admin_suffix} WIN32 ${mo_mfc_sources})

merge_trustinfo_manifest(mo${miktex_admin_suffix} requireAdministrator)

if(NATIVE_WINDOWS)
  add_link_flags(mo${miktex_admin_suffix} "/ENTRY:\"wWinMainCRTStartup\"")
endif(NATIVE_WINDOWS)

target_link_libraries(mo${miktex_admin_suffix} ${libs})

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET mo${miktex_admin_suffix}
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/mo.manifest
	-updateresource:$<TARGET_FILE:mo${miktex_admin_suffix}>\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(TARGETS mo${miktex_admin_suffix} DESTINATION ${bindir})
