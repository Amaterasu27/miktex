## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2009-2015 Christian Schenk
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

include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

include_directories(BEFORE ${CMAKE_BINARY_DIR}/${rel_ui_qt_dir})
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir})

use_static_crt()
use_static_libraries()

set(${ui_qt_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ConnectionSettingsDialog.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ConnectionSettingsDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ErrorDialog.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ErrorDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/InstallPackageDialog.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/InstallPackageDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ProxyAuthenticationDialog.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ProxyAuthenticationDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/RepositoryTableModel.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/RepositoryTableModel.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizDrive.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizDrive.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizLocal.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizLocal.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizRemote.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizRemote.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizSheet.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizSheet.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizType.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizType.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/StdAfx.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/StdAfx.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/UpdateDialog.cpp
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/UpdateDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/include/miktex/UI/Qt/ErrorDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/include/miktex/UI/Qt/Prototypes.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/include/miktex/UI/Qt/SiteWizSheet.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/include/miktex/UI/Qt/UpdateDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/internal.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/mikuiqt-version.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/mikuiqt.cpp
)

set(${ui_qt_lib_name}_moc_sources
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ConnectionSettingsDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ErrorDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/InstallPackageDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ProxyAuthenticationDialog.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/RepositoryTableModel.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizDrive.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizLocal.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizRemote.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizSheet.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizType.h
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/UpdateDialog.h
)

set(${ui_qt_lib_name}_ui_files
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ConnectionSettingsDialog.ui
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ErrorDialog.ui
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/InstallPackageDialog.ui
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/ProxyAuthenticationDialog.ui
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizDrive.ui
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizLocal.ui
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizRemote.ui
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/SiteWizType.ui
  ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/UpdateDialog.ui
)

qt5_wrap_cpp(${ui_qt_lib_name}_mocs ${${ui_qt_lib_name}_moc_sources})

qt5_wrap_ui(${ui_qt_lib_name}_ui_headers ${${ui_qt_lib_name}_ui_files})

qt5_wrap_cpp(${ui_qt_lib_name}_ui_mocs ${${ui_qt_lib_name}_ui_headers})

add_library(${ui_qt_lib_name} STATIC
  ${${ui_qt_lib_name}_sources}
  ${${ui_qt_lib_name}_mocs}
  ${${ui_qt_lib_name}_ui_mocs}
)

target_compile_definitions(${ui_qt_lib_name}
  PUBLIC
    -DMIKTEX_UI_QT
)

target_include_directories(${ui_qt_lib_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_ui_qt_dir}/include
)

if(INSTALL_STATIC_LIBRARIES)
  install(TARGETS ${ui_qt_lib_name}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
endif()
