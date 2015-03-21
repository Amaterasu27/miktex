## CMakeLists.txt						-*- CMake -*-
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

add_definitions(-DLUA_BUILD_AS_DLL -DLUA_COMPAT_MODULE)

include_directories(
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}
)

set(${lua_dll_name}_sources
  liblua-version.h
  src/lapi.c
  src/lapi.h
  src/lauxlib.c
  src/lauxlib.h
  src/lbaselib.c
  src/lbitlib.c
  src/lcode.c
  src/lcode.h
  src/lcorolib.c
  src/lctype.c
  src/lctype.h
  src/ldblib.c
  src/ldebug.c
  src/ldebug.h
  src/ldo.c
  src/ldo.h
  src/ldump.c
  src/lfunc.c
  src/lfunc.h
  src/lgc.c
  src/lgc.h
  src/linit.c
  src/liolib.c
  src/llex.c
  src/llex.h
  src/llimits.h
  src/lmathlib.c
  src/lmem.c
  src/lmem.h
  src/loadlib.c
  src/lobject.c
  src/lobject.h
  src/lopcodes.c
  src/lopcodes.h
  src/loslib.c
  src/lparser.c
  src/lparser.h
  src/lstate.c
  src/lstate.h
  src/lstring.c
  src/lstring.h
  src/lstrlib.c
  src/ltable.c
  src/ltable.h
  src/ltablib.c
  src/ltm.c
  src/ltm.h
  src/lua.h
  src/luaconf.h
  src/lualib.h
  src/lundump.c
  src/lundump.h
  src/lvm.c
  src/lvm.h
  src/lzio.c
  src/lzio.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/liblua.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/liblua.rc
  )
  set(${lua_dll_name}_sources ${${lua_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/liblua.rc
  )
endif(NATIVE_WINDOWS)

add_library(${lua_dll_name} SHARED ${${lua_dll_name}_sources})

target_link_libraries(${lua_dll_name}
  ${utf8wrap_dll_name}
)

set_target_properties(${lua_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${lua_dll_name})

install(TARGETS ${lua_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)

set(lua_sources
  src/lua.c
  lua-version.h
)

if(NATIVE_WINDOWS)
  set(lua_sources ${lua_sources}
    lua.rc
  )
endif(NATIVE_WINDOWS)

add_executable(${miktex_prefix}lua ${lua_sources})

merge_trustinfo_manifest(${miktex_prefix}lua asInvoker)

target_link_libraries(${miktex_prefix}lua ${lua_dll_name})

install(TARGETS ${miktex_prefix}lua DESTINATION ${bindir})

set(includes
  lapi.h
  lauxlib.h
  lcode.h
  lctype.h
  ldebug.h
  ldo.h
  lfunc.h
  lgc.h
  llex.h
  llimits.h
  lmem.h
  lobject.h
  lopcodes.h
  lparser.h
  lstate.h
  lstring.h
  ltable.h
  ltm.h
  lua.h
  lua.hpp
  luaconf.h
  lualib.h
  lundump.h
  lvm.h
  lzio.h
)

foreach(i ${includes})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/${i}
    ${CMAKE_BINARY_DIR}/${rel_lua_include_dir}/${i}
    COPYONLY
  )
endforeach(i ${includes})
