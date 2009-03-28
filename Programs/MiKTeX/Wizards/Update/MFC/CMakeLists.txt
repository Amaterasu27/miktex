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

use_static_crt()
use_static_libraries()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/res/Update.manifest.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/Update.manifest
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Update.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/Update.rc
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_MFC_FLAG 1)

set(update_mfc_sources
  ${CMAKE_CURRENT_BINARY_DIR}/ConnectionSettingsDialog.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ConnectionSettingsDialog.h
  ${CMAKE_CURRENT_BINARY_DIR}/ProxyAuthenticationDialog.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ProxyAuthenticationDialog.h
  CdPage.cpp
  FileCopyPage.cpp
  FinishPage.cpp
  LocalPage.cpp
  PackageListPage.cpp
  RemotePage.cpp
  StdAfx.cpp
  TypePage.cpp
  Update.cpp
  UpdateWizard.cpp
  WelcomePage.cpp
)

foreach(f
    ConnectionSettingsDialog.cpp
    ConnectionSettingsDialog.h
    ProxyAuthenticationDialog.cpp
    ProxyAuthenticationDialog.h)
  add_custom_command(
    OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/${f}
    COMMAND
	${CMAKE_COMMAND} -E copy
		${CMAKE_SOURCE_DIR}/${rel_ui_mfc_dir}/${f}
		${CMAKE_CURRENT_BINARY_DIR}/${f}
    DEPENDS
	${CMAKE_SOURCE_DIR}/${rel_ui_mfc_dir}/${f}
    VERBATIM
  )
endforeach(f)

if(NATIVE_WINDOWS)
  set(update_mfc_sources ${update_mfc_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/Update.rc)
endif(NATIVE_WINDOWS)

set(libs
  ${bzip2_lib_name}
  ${core_lib_name}
  ${curl_lib_name}
  ${expat_lib_name}
  ${extractor_lib_name}
  ${getopt_lib_name}
  ${lzma_lib_name}
  ${md5_lib_name}
  ${mpm_lib_name}
  ${mspack_lib_name}
  ${ui_lib_name}
  ${uriparser_lib_name}
  ${zlib_lib_name}
)

if(NATIVE_WINDOWS)
  set(libs ${libs}
    Wldap32
    fdi
    version
    wininet
    winmm
    ws2_32
  )
endif(NATIVE_WINDOWS)

add_executable(update_mfc WIN32 ${update_mfc_sources})

merge_trustinfo_manifest(update_mfc asInvoker)

target_link_libraries(update_mfc ${libs})

get_target_property(update_mfc_exe update_mfc LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET update_mfc
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/Update.manifest
	-updateresource:${update_mfc_exe}\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(
  FILES ${update_mfc_exe}
  DESTINATION ${miktex_internal_bindir}
  RENAME update.exe
)

add_executable(update_mfc_admin WIN32 ${update_mfc_sources})

merge_trustinfo_manifest(update_mfc_admin requireAdministrator)

target_link_libraries(update_mfc_admin ${libs})

get_target_property(update_mfc_admin_exe update_mfc_admin LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET update_mfc_admin
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/Update.manifest
	-updateresource:${update_mfc_admin_exe}\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(
  FILES ${update_mfc_admin_exe}
  DESTINATION ${miktex_internal_bindir}
  RENAME update${miktex_admin_suffix}.exe
)
