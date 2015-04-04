## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2010-2015 Christian Schenk
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

add_definitions(-DQT_PLUGIN)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${CMAKE_CURRENT_BINARY_DIR}/../..
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src
  ${CMAKE_CURRENT_BINARY_DIR}/../../src
)

set(twluaplugin_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/ConfigurableApp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWScript.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWScript.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWScriptAPI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWScriptAPI.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWSystemCmd.h
  TWLuaPlugin.cpp
  TWLuaPlugin.h
)

add_library(twluaplugin-static STATIC
  ${twluaplugin_sources}
)

target_link_libraries(twluaplugin-static
  PUBLIC
    ${core_dll_name}
    ${lua_dll_name}
    Qt5::Core
    Qt5::Gui
    Qt5::UiTools
    Qt5::Widgets
)
