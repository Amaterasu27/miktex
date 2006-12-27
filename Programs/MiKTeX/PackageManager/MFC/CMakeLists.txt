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
  ${CMAKE_CURRENT_SOURCE_DIR}/res/mpm.manifest.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/mpm.manifest
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_MFC_FLAG 2)

add_definitions(
  -D_AFXDLL
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
  set(mpm_mfc_sources ${mpm_mfc_sources} mpm.rc)
endif(NATIVE_WINDOWS)

add_executable(mpm_mfc WIN32 ${mpm_mfc_sources})

merge_trustinfo_manifest(mpm_mfc asInvoker)

target_link_libraries(mpm_mfc
  ${core_dll_name}
  ${mpm_dll_name}
  ${ui_dll_name}
)

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
