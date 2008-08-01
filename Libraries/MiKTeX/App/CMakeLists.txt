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

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

set(${app_dll_name}_sources
  StdAfx.cpp
  StdAfx.h
  app-version.h
  app.cpp
  include/miktex/App/Application.h
  internal.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/app.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/app.rc
  )
  set(${app_dll_name}_sources ${${app_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/app.rc
  )
endif(NATIVE_WINDOWS)

add_library(${app_dll_name} SHARED ${${app_dll_name}_sources})

set_target_properties(${app_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
    SOVERSION "1"
)

target_link_libraries(${app_dll_name}
  ${core_dll_name}
  ${mpm_dll_name}
)

if(QT4_FOUND)
  target_link_libraries(${app_dll_name}
    ${ui_qt_dll_name}
  )
endif(QT4_FOUND)

if(MFC_FOUND)
  target_link_libraries(${app_dll_name}
    ${ui_mfc_dll_name}
  )
endif(MFC_FOUND)

delay_load(${app_dll_name}
  ${mpm_dll_name}
  ${ui_qt_dll_name}
  ${ui_mfc_dll_name}
)

rebase(${app_dll_name})

install(TARGETS ${app_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

install(
  FILES
    include/miktex/App/Application
    include/miktex/App/Application.h
  DESTINATION
    ${incdir}/miktex/App
)
