## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2007-2008 Christian Schenk
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

set (src_dir ${CMAKE_SOURCE_DIR}/${rel_graphite_engine_dir}/src)
set (win32_dir ${CMAKE_SOURCE_DIR}/${rel_graphite_dir}/wrappers/win32)

add_definitions(
  -DGR_NAMESPACE
  -DTRACING
  -DWIN32
  -D_DLL
  -D_UNICODE -DUNICODE
  -D_WINDOWS
)

include_directories(BEFORE
  ${src_dir}/font
  ${src_dir}/painter
  ${src_dir}/segment
  ${src_dir}/textsource
  ${src_dir}/generic
  ${CMAKE_SOURCE_DIR}/${rel_graphite_engine_dir}/include/graphite
  ${win32_dir}
)

include_directories(BEFORE
  ${CMAKE_SOURCE_DIR}/${rel_graphite_engine_dir}
)

set(${graphite_engine_dll_name}_sources
  ${src_dir}/generic/Debug.cpp
  ${src_dir}/generic/Util.cpp
  ${src_dir}/font/Font.cpp
  ${src_dir}/font/FileFont.cpp
  ${src_dir}/segment/FontCache.cpp
  ${src_dir}/segment/FontFace.cpp
  ${src_dir}/segment/FileInput.cpp
  ${src_dir}/segment/GrCharStream.cpp
  ${src_dir}/segment/GrClassTable.cpp
  ${src_dir}/segment/GrEngine.cpp
  ${src_dir}/segment/GrFeature.cpp
  ${src_dir}/segment/GrFSM.cpp
  ${src_dir}/segment/GrGlyphTable.cpp
  ${src_dir}/segment/GrPass.cpp
  ${src_dir}/segment/GrPassActionCode.cpp
  ${src_dir}/segment/GrSlotState.cpp
  ${src_dir}/segment/GrSlotStream.cpp
  ${src_dir}/segment/GrTableManager.cpp
  ${src_dir}/segment/MemoryUsage.cpp
  ${src_dir}/segment/Segment.cpp
  ${src_dir}/segment/SegmentAux.cpp
  ${src_dir}/painter/SegmentPainter.cpp
  ${src_dir}/segment/TransductionLog.cpp
  ${src_dir}/segment/Platform.cpp
  ${src_dir}/font/TtfUtil.cpp
  ${win32_dir}/WinFont.cpp
  ${win32_dir}/WinSegmentPainter.cpp
  ${win32_dir}/win32_dll.cpp
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_SOURCE_DIR}/${rel_graphite_engine_dir}/graphite-engine.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/graphite-engine.rc
  )
  set(${graphite_engine_dll_name}_sources
    ${${graphite_engine_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/graphite-engine.rc
  )
endif(NATIVE_WINDOWS)

add_library(${graphite_engine_dll_name}
  SHARED ${${graphite_engine_dll_name}_sources})

set_target_properties(${graphite_engine_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${graphite_engine_dll_name})

install(TARGETS ${graphite_engine_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
