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

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(
  -DUSE_MIKTEX_EXIT
)

###############################################################################
## LUA51
###############################################################################

set(lua51_sources
  lua51/lapi.c
  lua51/lapi.h
  lua51/lauxlib.c
  lua51/lauxlib.h
  lua51/lbaselib.c
  lua51/lcoco.c
  lua51/lcoco.h
  lua51/lcode.c
  lua51/lcode.h
  lua51/ldblib.c
  lua51/ldebug.c
  lua51/ldebug.h
  lua51/ldo.c
  lua51/ldo.h
  lua51/ldump.c
  lua51/lfunc.c
  lua51/lfunc.h
  lua51/lgc.c
  lua51/lgc.h
  lua51/linit.c
  lua51/llex.c
  lua51/llex.h
  lua51/llimits.h
  lua51/lmathlib.c
  lua51/lmem.c
  lua51/lmem.h
  lua51/loadlib.c
  lua51/lobject.c
  lua51/lobject.h
  lua51/lopcodes.c
  lua51/lopcodes.h
  lua51/loslib.c
  lua51/lparser.c
  lua51/lparser.h
  lua51/lstate.c
  lua51/lstate.h
  lua51/lstring.c
  lua51/lstring.h
  lua51/lstrlib.c
  lua51/ltable.c
  lua51/ltable.h
  lua51/ltablib.c
  lua51/ltm.c
  lua51/ltm.h
  lua51/lua.h
  lua51/luaconf.h
  lua51/lualib.h
  lua51/lundump.c
  lua51/lundump.h
  lua51/lvm.c
  lua51/lvm.h
  lua51/lzio.c
  lua51/lzio.h
)

add_library(luatex-lua51-static STATIC ${lua51_sources})

