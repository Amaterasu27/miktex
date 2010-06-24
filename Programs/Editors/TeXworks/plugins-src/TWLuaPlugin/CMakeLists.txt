## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2010 Christian Schenk
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

#add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
#add_definitions(-DQT_NO_DEBUG)
#add_definitions(-DQT_SHARED)

include_directories(
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/etc
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${CMAKE_CURRENT_BINARY_DIR}/../..
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src
  ${CMAKE_CURRENT_BINARY_DIR}/../../src
)

set(twluaplugin_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWScript.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWScript.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWScriptAPI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWScriptAPI.h
  TWLuaPlugin.cpp
  TWLuaPlugin.h
  twluaplugin-version.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/twluaplugin.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/twluaplugin.rc
  )
  set(twluaplugin_sources ${twluaplugin_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/twluaplugin.rc
  )
endif(NATIVE_WINDOWS)

set(twluaplugin_moc_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWScript.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../../src/TWScriptAPI.h
  TWLuaPlugin.h
)

qt4_wrap_cpp(twluaplugin_mocs ${twluaplugin_moc_sources})

if(LINK_EVERYTHING_STATICALLY)
  set(libs ${MIKTEX_STATIC_LINK_LIBRARIES})
else(LINK_EVERYTHING_STATICALLY)
  set(libs
    ${lua_dll_name}
    ${QT_LIBRARIES}
  )
endif(LINK_EVERYTHING_STATICALLY)

add_library(${twluaplugin_dll_name} SHARED
  ${twluaplugin_sources}
  ${twluaplugin_mocs}
)

target_link_libraries(${twluaplugin_dll_name} ${libs})

rebase(${twluaplugin_dll_name})

install(TARGETS ${twluaplugin_dll_name}
  RUNTIME DESTINATION "${miktex_texworks_plugins_dir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
