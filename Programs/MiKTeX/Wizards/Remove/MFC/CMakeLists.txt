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
  ${CMAKE_CURRENT_SOURCE_DIR}/res/Remove.manifest.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/Remove.manifest
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Remove.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/Remove.rc
)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_MFC_FLAG 1)

set(remove_mfc_sources
  FinishPage.cpp
  LogFile.cpp
  Remove.cpp
  RemoveFilesPage.cpp
  RemoveWizard.cpp
  StdAfx.cpp
  WelcomePage.cpp
)

if(NATIVE_WINDOWS)
  set(remove_mfc_sources ${remove_mfc_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/Remove.rc)
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

add_executable(remove_mfc WIN32 ${remove_mfc_sources})

merge_trustinfo_manifest(remove_mfc asInvoker)

target_link_libraries(remove_mfc ${libs})

get_target_property(remove_mfc_exe remove_mfc LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET remove_mfc
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/Remove.manifest
	-updateresource:${remove_mfc_exe}\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(
  FILES ${remove_mfc_exe}
  DESTINATION ${miktex_internal_bindir}
  RENAME uninstall.exe
)

add_executable(remove_mfc${miktex_admin_suffix} WIN32 ${remove_mfc_sources})

merge_trustinfo_manifest(remove_mfc${miktex_admin_suffix} requireAdministrator)

target_link_libraries(remove_mfc${miktex_admin_suffix} ${libs})

get_target_property(remove_mfc_admin_exe remove_mfc${miktex_admin_suffix} LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET remove_mfc${miktex_admin_suffix}
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/Remove.manifest
	-updateresource:${remove_mfc_admin_exe}\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

install(
  FILES ${remove_mfc_admin_exe}
  DESTINATION ${miktex_internal_bindir}
  RENAME uninstall${miktex_admin_suffix}.exe
)
