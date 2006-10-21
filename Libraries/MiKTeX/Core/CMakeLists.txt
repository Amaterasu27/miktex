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

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_core_inc_dir}/version.h.in
  ${CMAKE_BINARY_DIR}/${rel_core_inc_dir}/version.h
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

add_definitions(-DHAVE_CONFIG_H)

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

set(${core_dll_name}_sources
  CommandLine.cpp
  FileStream.cpp
  FormatInfo.cpp
  MemoryMappedFile.cpp
  Process.cpp
  StdAfx.h
  Thread.cpp
  appnames.cpp
  cfg.cpp
  config.cpp
  core-version.h
  error.cpp
  files.cpp
  filetypes.cpp
  findfile.cpp
  fnamedb.cpp
  fnamedb.h
  fndbmem.h
  fontinfo.cpp
  getdelim.cpp
  graphics.cpp
  gsinfo.cpp
  include/miktex/char.h
  include/miktex/core.h
  include/miktex/env.h
  include/miktex/first.h
  include/miktex/paths.h
  include/miktex/reg.h
  include/miktex/test.h
  include/miktex/trace.h
  internal.h
  log.cpp
  makefndb.cpp
  mfmodes.cpp
  miktex.cpp
  obsolete.cpp
  papersize.cpp
  path.cpp
  runbat.cpp
  runperl.cpp
  scratchdir.cpp
  searchpath.cpp
  tempfile.cpp
  texmfroot.cpp
  uncompress.cpp
  util.cpp
)

if(MIKTEX_WINDOWS)
  set(${core_dll_name}_sources ${${core_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/MiKTeXEvents.h
    core.rc
    win/win.cpp
    win/winDirectoryLister.cpp
    win/winDirectoryLister.h
    win/winFile.cpp
    win/winMemoryMappedFile.cpp
    win/winMemoryMappedFile.h
    win/winProcess.cpp
    win/winProcess.h
    win/winRegistry.cpp
    win/winRegistry.h
    win/winThread.cpp
    win/winThread.h
  )
  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/MiKTeXEvents.h
    PROPERTIES GENERATED TRUE
  )
  add_custom_command(
    OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/MiKTeXEvents.h
		${CMAKE_CURRENT_BINARY_DIR}/MiKTeXEvents.rc
		${CMAKE_CURRENT_BINARY_DIR}/MSG00001.bin
    COMMAND mc ${CMAKE_CURRENT_SOURCE_DIR}/MiKTeXEvents.mc
    MAIN_DEPENDENCY MiKTeXEvents.mc
    DEPENDENCIES MiKTeXEvents.mc
    VERBATIM
  )
else(MIKTEX_WINDOWS)
  set(${core_dll_name}_sources ${${core_dll_name}_sources}
    unx/unx.cpp
    unx/unxDirectoryLister.cpp
    unx/unxDirectoryLister.h
    unx/unxFile.cpp
    unx/unxMemoryMappedFile.cpp
    unx/unxMemoryMappedFile.h
    unx/unxProcess.cpp
    unx/unxProcess.h
    unx/unxThread.cpp
    unx/unxThread.h
  )
endif(MIKTEX_WINDOWS)

add_library(${core_dll_name} SHARED ${${core_dll_name}_sources})

target_link_libraries(${core_dll_name}
  ${CMAKE_THREAD_LIBS_INIT}
  ${bzip2_dll_name}
  ${md5_dll_name}
  ${zlib_dll_name}
)

if(MIKTEX_WINDOWS)
  target_link_libraries(${core_dll_name}
    SHFolder.lib
    htmlhelp.lib
  )
endif(MIKTEX_WINDOWS)

install(TARGETS ${core_dll_name} DESTINATION "${bindir}")
