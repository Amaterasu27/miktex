## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2008-2009 Christian Schenk
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

if(LINK_EVERYTHING_STATICALLY)
  use_static_crt()
  use_static_libraries()
endif(LINK_EVERYTHING_STATICALLY)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(mpm_qt_sources
  MainWindow.cpp
  MainWindow.h
  PackageTableModel.cpp
  PackageTableModel.h
  mpm.cpp
  mpm-version.h
)

if(NATIVE_WINDOWS)
  set(mpm_qt_sources ${mpm_qt_sources}
    ${CMAKE_CURRENT_SOURCE_DIR}/mpm.rc
  )
endif(NATIVE_WINDOWS)

set(mpm_qt_moc_sources
  MainWindow.h
  PackageTableModel.h
)

set(mpm_qt_ui_files
  MainWindow.ui
)

set(mpm_qt_rcs_files
  mpm.qrc
)

qt4_wrap_cpp(mpm_qt_mocs ${mpm_qt_moc_sources})

qt4_wrap_ui(mpm_qt_ui_headers ${mpm_qt_ui_files})

qt4_wrap_cpp(mpm_qt_ui_mocs ${mpm_qt_ui_headers})

qt4_add_resources(mpm_qt_rcc_sources ${mpm_qt_rcs_files})

add_executable(mpm_qt
  ${mpm_qt_sources}
  ${mpm_qt_mocs}
  ${mpm_qt_ui_mocs}
  ${mpm_qt_rcc_sources}
)

if(LINK_EVERYTHING_STATICALLY)
  target_link_libraries(mpm_qt ${MIKTEX_STATIC_LINK_LIBRARIES})
else(LINK_EVERYTHING_STATICALLY)
  target_link_libraries(mpm_qt
    ${core_dll_name}
    ${mpm_dll_name}
    ${popt_dll_name}
    ${ui_qt_dll_name}
    ${QT_LIBRARIES}
  )
endif(LINK_EVERYTHING_STATICALLY)

merge_trustinfo_manifest(mpm_qt asInvoker)

install(TARGETS mpm_qt DESTINATION ${bindir})
