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

set(${app_dll_name}_sources
  StdAfx.cpp
  StdAfx.h
  app-version.h
  app.cpp
  internal.h
)

if(NATIVE_WINDOWS)
  set(${app_dll_name}_sources ${${app_dll_name}_sources}
    app.rc
  )
endif(NATIVE_WINDOWS)

add_library(${app_dll_name} SHARED ${${app_dll_name}_sources})

target_link_libraries(${app_dll_name}
  ${core_dll_name}
  ${mpm_dll_name}
)

if(NATIVE_WINDOWS AND MFC_FOUND)
  target_link_libraries(${app_dll_name}
    ${ui_dll_name}
  )
endif(NATIVE_WINDOWS AND MFC_FOUND)

install(TARGETS ${app_dll_name} DESTINATION "${bindir}")
