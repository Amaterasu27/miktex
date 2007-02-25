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
  ${CMAKE_CURRENT_SOURCE_DIR}/res/Remove.manifest.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/Remove.manifest
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
  set(remove_mfc_sources ${remove_mfc_sources} Remove.rc)
endif(NATIVE_WINDOWS)

add_executable(remove WIN32 ${remove_mfc_sources})

merge_trustinfo_manifest(remove requireAdministrator)

target_link_libraries(remove
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
  target_link_libraries(remove
    fdi
    version
    wininet
    winmm
    ws2_32
  )
endif(NATIVE_WINDOWS)

get_target_property(remove_mfc_exe remove LOCATION)

if(NATIVE_WINDOWS)
  add_custom_command(
    TARGET remove
    POST_BUILD
    COMMAND ${MT_EXE} -nologo
	-manifest ${CMAKE_CURRENT_BINARY_DIR}/res/Remove.manifest
	-updateresource:${remove_mfc_exe}\;1
    VERBATIM
  )
endif(NATIVE_WINDOWS)

get_target_property(remove_exe remove LOCATION)

install(
  FILES ${remove_exe}
  DESTINATION ${configdir}
  RENAME uninstall.dat
)
