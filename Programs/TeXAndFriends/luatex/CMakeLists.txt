## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2010-2013 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}/lua52
  ${CMAKE_BINARY_DIR}/${rel_cairo_include_dir}
  ${CMAKE_SOURCE_DIR}/${rel_cairo_include_dir}
  ${CMAKE_SOURCE_DIR}/${rel_synctex_dir}
)

add_definitions(
  -DAPPTAG=luatex
  -DLUA_COMPAT_ALL
  -DLUA_COMPAT_MODULE
  -DMIKTEX_LUATEX
  -DNO_DUMP_SHARE
  -DPDF_PARSER_ONLY
  -DSYNCTEX_ENGINE_H="synctex-luatex.h"
  -DUSE_MIKTEX_EXIT
  -DpdfTeX
)

add_definitions(
  -D_UNICODE -DUNICODE
)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

if(WITH_CONTEXT_SUPPORT)
  add_definitions(-DWITH_CONTEXT_SUPPORT=1)
endif(WITH_CONTEXT_SUPPORT)


###############################################################################
## luatex-lua52-static
###############################################################################

set(lua52_sources
  lua52/lapi.c
  lua52/lapi.h
  lua52/lauxlib.c
  lua52/lauxlib.h
  lua52/lbaselib.c
  lua52/lbitlib.c
  lua52/lcode.c
  lua52/lcode.h
  lua52/lcorolib.c
  lua52/lctype.c
  lua52/lctype.h
  lua52/ldblib.c
  lua52/ldebug.c
  lua52/ldebug.h
  lua52/ldo.c
  lua52/ldo.h
  lua52/ldump.c
  lua52/lfunc.c
  lua52/lfunc.h
  lua52/lgc.c
  lua52/lgc.h
  lua52/linit.c
  lua52/llex.c
  lua52/llex.h
  lua52/llimits.h
  lua52/lmathlib.c
  lua52/lmem.c
  lua52/lmem.h
  lua52/loadlib.c
  lua52/lobject.c
  lua52/lobject.h
  lua52/lopcodes.c
  lua52/lopcodes.h
  lua52/loslib.c
  lua52/lparser.c
  lua52/lparser.h
  lua52/lstate.c
  lua52/lstate.h
  lua52/lstring.c
  lua52/lstring.h
  lua52/lstrlib.c
  lua52/ltable.c
  lua52/ltable.h
  lua52/ltablib.c
  lua52/ltm.c
  lua52/ltm.h
  lua52/lua.h
  lua52/luaconf.h
  lua52/lualib.h
  lua52/lundump.c
  lua52/lundump.h
  lua52/lvm.c
  lua52/lvm.h
  lua52/lzio.c
  lua52/lzio.h
)

add_library(luatex-lua52-static STATIC ${lua52_sources})

###############################################################################
## luatex-luasocket-static
###############################################################################

set(luasocket_sources
  luasocket/src/auxiliar.c
  luasocket/src/auxiliar.h
  luasocket/src/buffer.c
  luasocket/src/buffer.h
  luasocket/src/except.c
  luasocket/src/except.h
  luasocket/src/inet.c
  luasocket/src/inet.h
  luasocket/src/io.c
  luasocket/src/io.h
  luasocket/src/lua_preload.c
  luasocket/src/luasocket.c
  luasocket/src/luasocket.h
  luasocket/src/mime.c
  luasocket/src/mime.h
  luasocket/src/options.c
  luasocket/src/options.h
  luasocket/src/select.c
  luasocket/src/select.h
  luasocket/src/socket.c
  luasocket/src/socket.h
  luasocket/src/tcp.c
  luasocket/src/tcp.h
  luasocket/src/timeout.c
  luasocket/src/timeout.h
  luasocket/src/udp.c
  luasocket/src/udp.h
  luasocket/src/usocket.h
)

add_library(luatex-luasocket-static STATIC ${luasocket_sources})

###############################################################################
## luatex-luamisc-static
###############################################################################

set(luamisc_sources
  luafilesystem/src/lfs.c
  luafilesystem/src/lfs.h
  luamd5/luamd5.h
  luamd5/md5.c
  luamd5/md5_lua.c
  luamd5/md5lib.c
  luapeg/lpeg.c
  luapeg/lpeg.h
  luaprofiler/clocks.c
  luaprofiler/clocks.h
  luaprofiler/core_profiler.c
  luaprofiler/core_profiler.h
  luaprofiler/function_meter.c
  luaprofiler/function_meter.h
  luaprofiler/lua50_profiler.c
  luaprofiler/luaprofiler.h
  luaprofiler/stack.c
  luaprofiler/stack.h
  luazip/src/luazip.c
  luazip/src/luazip.h
  luazlib/lgzip.c
  luazlib/lzlib.c
  slnunicode/slnunico.c
)

add_library(luatex-luamisc-static STATIC ${luamisc_sources})

###############################################################################
## luatex-luafontforge-static
###############################################################################

add_subdirectory(luafontloader)

###############################################################################
## luatex-static
###############################################################################

add_subdirectory(tex)

###############################################################################
## luatex-program
###############################################################################

execute_process(
  COMMAND ${DATE_EXE} +-%Y%m%d%H
  OUTPUT_VARIABLE extra_version_info
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(
  -Dextra_version_info=${extra_version_info}
)

set(luatex_program_sources
  luatex.c
  luatex.h
  luatex_svnversion.h
  luatex-version.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/luatex.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/luatex.rc
    )
  set(${luatex_program_sources}_sources
    ${${luatex_program_sources}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/luatex.rc)
endif(NATIVE_WINDOWS)

add_library(${luatex_program_dll_name} SHARED ${luatex_program_sources})

set_target_properties(${app_dll_name}
      PROPERTIES
      LINK_INTERFACE_LIBRARIES ""
)

target_link_libraries(${luatex_program_dll_name}
  wsock32.lib
  ws2_32.lib

  ${app_dll_name}
  ${cairo_dll_name}
  ${core_dll_name}
  ${jpeg_dll_name}
  ${kpsemu_dll_name}
  ${md5_dll_name}
  ${mp_lib_name}
  ${obsdcompat_dll_name}
  ${png_dll_name}
  ${poppler_lib_name}
  ${zzip_dll_name}

  ${unxemu_dll_name}
  ${utf8wrap_dll_name}
  ${getopt_dll_name}
  ${w2cemu_dll_name}
  ${zlib_dll_name}
  

  luatex-lua52-static
  luatex-luafontforge-static
  luatex-luamisc-static
  luatex-luasocket-static
  luatex-static
)

rebase(${luatex_program_dll_name})

install(TARGETS ${luatex_program_dll_name} DESTINATION "${bindir}")

###############################################################################
## executables
###############################################################################

set(executables
  ${miktex_prefix}luatex
  ${miktex_prefix}texlua
  ${miktex_prefix}texluac
  luatex
  texlua
  texluac
)

foreach(x ${executables})
  add_executable(${x} ${wrapper_cpp})
  target_link_libraries(${x}
    ${app_dll_name}
    ${core_dll_name}
    ${luatex_program_dll_name}
  )
  merge_trustinfo_manifest(${x} asInvoker)
  install(TARGETS ${x} DESTINATION "${bindir}")
endforeach(x)

###############################################################################
## runtexlua
###############################################################################

add_executable(runtexlua runtexlua.cpp)

target_link_libraries(runtexlua
  ${app_dll_name}
  ${core_dll_name}
  ${luatex_program_dll_name}
)

merge_trustinfo_manifest(runtexlua asInvoker)

install(TARGETS runtexlua DESTINATION "${miktex_internal_bindir}")

###############################################################################
## runmtxrun
###############################################################################

if(WITH_CONTEXT_SUPPORT)
  add_executable(runmtxrun runmtxrun.cpp)

  target_link_libraries(runmtxrun
    ${app_dll_name}
    ${core_dll_name}
    ${luatex_program_dll_name}
  )

  merge_trustinfo_manifest(runmtxrun asInvoker)

  install(TARGETS runmtxrun DESTINATION "${miktex_internal_bindir}")
endif(WITH_CONTEXT_SUPPORT)
