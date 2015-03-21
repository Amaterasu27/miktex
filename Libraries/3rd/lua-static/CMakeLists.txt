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

use_static_crt()
use_static_libraries()

add_definitions(-DLUA_COMPAT_MODULE)

include_directories(
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}
)

set(${lua_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/liblua-version.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lapi.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lapi.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lauxlib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lauxlib.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lbaselib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lbitlib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lcode.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lcode.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lcorolib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lctype.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lctype.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ldblib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ldebug.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ldebug.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ldo.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ldo.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ldump.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lfunc.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lfunc.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lgc.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lgc.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/linit.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/liolib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/llex.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/llex.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/llimits.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lmathlib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lmem.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lmem.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/loadlib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lobject.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lobject.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lopcodes.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lopcodes.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/loslib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lparser.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lparser.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lstate.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lstate.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lstring.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lstring.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lstrlib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ltable.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ltable.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ltablib.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ltm.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/ltm.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lua.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/luaconf.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lualib.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lundump.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lundump.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lvm.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lvm.h
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lzio.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/lzio.h
)

add_library(${lua_lib_name} STATIC ${${lua_lib_name}_sources})

if(INSTALL_STATIC_LIBRARIES)
  install(TARGETS ${lua_lib_name}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
endif(INSTALL_STATIC_LIBRARIES)

set(luac_sources
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/src/luac.c
  ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/luac-version.h
)

if(NATIVE_WINDOWS)
  set(luac_sources ${luac_sources}
    ${CMAKE_SOURCE_DIR}/${rel_lua_dir}/luac.rc
  )
endif(NATIVE_WINDOWS)

add_executable(${miktex_prefix}luac ${luac_sources})

merge_trustinfo_manifest(${miktex_prefix}luac asInvoker)

target_link_libraries(${miktex_prefix}luac ${lua_lib_name} ${utf8wrap_lib_name})

install(TARGETS ${miktex_prefix}luac DESTINATION ${bindir})
