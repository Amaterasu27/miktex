## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2008 Christian Schenk
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

set(CMAKE_MFC_FLAG 2)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/mikui.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/mikui.rc
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/res/mikui.rc2.in
  ${CMAKE_CURRENT_BINARY_DIR}/res/mikui.rc2
)

add_definitions(
  -D_AFXDLL
  -D_AFXEXT
)

set(${ui_mfc_dll_name}_sources
  ConnectionSettingsDialog.cpp
  ConnectionSettingsDialog.h
  ErrorDialog.cpp
  InstallPackageDialog.cpp
  InstallPackageDialog.h
  PackageTreeCtrl.cpp
  ProgressDialog.cpp
  PropPageFiles.cpp
  PropPageFiles.h
  PropPagePackage.cpp
  PropPagePackage.h
  PropSheetPackage.cpp
  ProxyAuthenticationDialog.cpp
  ProxyAuthenticationDialog.h
  SearchPackageDialog.cpp
  SearchPackageDialog.h
  SiteWizDrive.cpp
  SiteWizDrive.h
  SiteWizLocal.cpp
  SiteWizLocal.h
  SiteWizRemote.cpp
  SiteWizRemote.h
  SiteWizSheet.cpp
  SiteWizType.cpp
  SiteWizType.h
  StdAfx.cpp
  StdAfx.h
  TextViewerDialog.cpp
  UpdateDialog.cpp
  internal.h
  mikui-version.h
  mikui.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/mikui.rc
  resource.h
)

add_library(${ui_mfc_dll_name} SHARED ${${ui_mfc_dll_name}_sources})

set_target_properties(${ui_mfc_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
    SOVERSION "1"
)

target_link_libraries(${ui_mfc_dll_name}
  ${core_dll_name}
  ${mpm_dll_name}
)

rebase(${ui_mfc_dll_name})

install(TARGETS ${ui_mfc_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

install(
  FILES
    include/miktex/UI/MFC/ErrorDialog
    include/miktex/UI/MFC/PackageTreeCtrl
    include/miktex/UI/MFC/ProgressDialog
    include/miktex/UI/MFC/PropSheetPackage
    include/miktex/UI/MFC/Prototypes
    include/miktex/UI/MFC/SiteWizSheet
    include/miktex/UI/MFC/TextViewerDialog
    include/miktex/UI/MFC/UpdateDialog
  DESTINATION
    ${incdir}/miktex/UI/MFC
)
