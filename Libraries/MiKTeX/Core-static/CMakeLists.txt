## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2007 Christian Schenk
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

include_directories(BEFORE ${CMAKE_BINARY_DIR}/${rel_core_dir})
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/${rel_core_dir})

use_static_crt()
use_static_libraries()

set(${core_lib_name}_sources
  ${CMAKE_BINARY_DIR}/${rel_core_dir}/StdAfx.h
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/CommandLine.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/FileStream.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/FormatInfo.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/LzmaStream.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/MemoryMappedFile.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/Process.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/Thread.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/appnames.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/cfg.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/config.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/core-version.h
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/error.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/files.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/filetypes.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/findfile.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/fnamedb.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/fnamedb.h
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/fndbmem.h
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/fontinfo.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/getdelim.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/graphics.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/gsinfo.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/MiKTeX/Core/Text
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/MiKTeX/Core/Core
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/MiKTeX/Core/Environment
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/MiKTeX/Core/First
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/MiKTeX/Core/Paths
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/MiKTeX/Core/Registry
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/MiKTeX/Core/Test
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/include/miktex/trace.h
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/internal.h
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/log.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/makefndb.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/mfmodes.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/miktex.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/obsolete.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/papersize.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/path.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/runbat.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/runperl.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/scratchdir.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/searchpath.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/tempfile.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/texmfroot.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/uncompress.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/util.cpp
)

set(${core_lib_name}_sources ${${core_lib_name}_sources}
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/DllProc.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/win.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winDirectoryLister.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winDirectoryLister.h
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winFile.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winMemoryMappedFile.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winMemoryMappedFile.h
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winProcess.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winProcess.h
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winRegistry.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winRegistry.h
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winThread.cpp
  ${CMAKE_SOURCE_DIR}/${rel_core_dir}/win/winThread.h
)

add_library(${core_lib_name} STATIC ${${core_lib_name}_sources})
