## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2011 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}/res/mpm.manifest.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/mpm.manifest
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/mpm.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/mpm.rc
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

set(mpm_mfc_sources
  MainFrame.cpp
  MainFrame.h
  MpmDoc.cpp
  MpmDoc.h
  MpmView.cpp
  MpmView.h
  mpm-version.h
  mpm.cpp
  mpm.h
  resource.h
  stdafx.cpp
  stdafx.h
)

if(NATIVE_WINDOWS)
  set(mpm_mfc_sources ${mpm_mfc_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/mpm.rc)
endif(NATIVE_WINDOWS)

set(libs
  ${core_dll_name}
  ${mpm_dll_name}
  ${ui_mfc_dll_name}
)

add_executable(mpm_mfc WIN32 ${mpm_mfc_sources})

merge_trustinfo_manifest(mpm_mfc asInvoker)

if(NATIVE_WINDOWS)
  add_link_flags(mpm_mfc "/ENTRY:\"wWinMainCRTStartup\"")
endif(NATIVE_WINDOWS)

target_link_libraries(mpm_mfc ${libs})

get_target_property(mpm_mfc_exe mpm_mfc LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET mpm_mfc
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/mpm.manifest
	-updateresource:${mpm_mfc_exe}\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(TARGETS mpm_mfc DESTINATION ${bindir})

add_executable(mpm_mfc${miktex_admin_suffix} WIN32 ${mpm_mfc_sources})

merge_trustinfo_manifest(mpm_mfc${miktex_admin_suffix} requireAdministrator)

if(NATIVE_WINDOWS)
  add_link_flags(mpm_mfc${miktex_admin_suffix} "/ENTRY:\"wWinMainCRTStartup\"")
endif(NATIVE_WINDOWS)

target_link_libraries(mpm_mfc${miktex_admin_suffix} ${libs})

get_target_property(mpm_mfc_admin_exe mpm_mfc${miktex_admin_suffix} LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET mpm_mfc${miktex_admin_suffix}
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/mpm.manifest
	-updateresource:${mpm_mfc_admin_exe}\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(TARGETS mpm_mfc${miktex_admin_suffix} DESTINATION ${bindir})
