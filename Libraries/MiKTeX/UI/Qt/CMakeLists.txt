## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2008 Christian Schenk
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

include(${QT_USE_FILE})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(${ui_qt_dll_name}_sources
  ConnectionSettingsDialog.cpp
  ConnectionSettingsDialog.h
  ErrorDialog.cpp
  ErrorDialog.h
  InstallPackageDialog.cpp
  InstallPackageDialog.h
  ProxyAuthenticationDialog.cpp
  ProxyAuthenticationDialog.h
  SiteWizSheet.cpp
  SiteWizSheet.h
  SiteWizType.cpp
  SiteWizType.h
  SiteWizRemote.cpp
  SiteWizRemote.h
  StdAfx.cpp
  StdAfx.h
  internal.h
  mikuiqt-version.h
  mikuiqt.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/mikuiqt.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/mikuiqt.rc
  )
  set(${ui_qt_dll_name}_sources ${${ui_qt_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/mikuiqt.rc
  )
endif(NATIVE_WINDOWS)

set(${ui_qt_dll_name}_moc_sources
  ConnectionSettingsDialog.h
  ErrorDialog.h
  InstallPackageDialog.h
  ProxyAuthenticationDialog.h
  SiteWizSheet.h
  SiteWizType.h
  SiteWizRemote.h
)

set(${ui_qt_dll_name}_ui_files
  ConnectionSettingsDialog.ui
  ErrorDialog.ui
  InstallPackageDialog.ui
  ProxyAuthenticationDialog.ui
  SiteWizDrive.ui
  SiteWizLocal.ui
  SiteWizRemote.ui
  SiteWizType.ui
)

qt4_wrap_cpp(${ui_qt_dll_name}_mocs ${${ui_qt_dll_name}_moc_sources})

qt4_wrap_ui(${ui_qt_dll_name}_ui_headers ${${ui_qt_dll_name}_ui_files})

qt4_wrap_cpp(${ui_qt_dll_name}_ui_mocs ${${ui_qt_dll_name}_ui_headers})

add_library(${ui_qt_dll_name} SHARED
  ${${ui_qt_dll_name}_sources}
  ${${ui_qt_dll_name}_mocs}
  ${${ui_qt_dll_name}_ui_mocs}
)

set_target_properties(${ui_qt_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
    SOVERSION "1"
)

target_link_libraries(${ui_qt_dll_name}
  ${core_dll_name}
  ${mpm_dll_name}
  ${QT_LIBRARIES}
)

rebase(${ui_qt_dll_name})

install(TARGETS ${ui_qt_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

install(
  FILES
    include/miktex/UI/Qt/ErrorDialog
    include/miktex/UI/Qt/SiteWizSheet
    include/miktex/UI/Qt/Prototypes
  DESTINATION
    ${incdir}/miktex/UI/Qt
)

install(
  FILES
    ${CMAKE_SOURCE_DIR}/${rel_ui_dir}/include/miktex/UI/UI
  DESTINATION
    ${incdir}/miktex/UI
)
