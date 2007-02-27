## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2007 Christian Schenk
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
  set(update_mfc_sources ${update_mfc_sources} Update.rc)
endif(NATIVE_WINDOWS)

add_executable(update WIN32 ${update_mfc_sources})

merge_trustinfo_manifest(update asInvoker)

target_link_libraries(update
  ${bzip2_lib_name}
  ${core_lib_name}
  ${curl_lib_name}
  ${expat_lib_name}
  ${gnu_lib_name}
  ${md5_lib_name}
  ${mpm_lib_name}
  ${mspack_lib_name}
  ${ui_lib_name}
  ${zlib_lib_name}
)

if(NATIVE_WINDOWS)
  target_link_libraries(update
    fdi
    version
    wininet
    winmm
    ws2_32
  )
endif(NATIVE_WINDOWS)

get_target_property(update_mfc_exe update LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET update
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/Update.manifest
	-updateresource:${update_mfc_exe}\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

get_target_property(update_exe update LOCATION)

install(
  FILES ${update_exe}
  DESTINATION ${configdir}
  RENAME update.dat
)
