## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2013 Christian Schenk
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

set(GRAPHITE_API_CURRENT 3)
set(GRAPHITE_API_REVISION 0)
set(GRAPHITE_API_AGE 1)
set(GRAPHITE_VERSION ${GRAPHITE_API_CURRENT}.${GRAPHITE_API_REVISION}.${GRAPHITE_API_AGE})
set(GRAPHITE_SO_VERSION ${GRAPHITE_API_CURRENT})

include_directories(${CMAKE_SOURCE_DIR}/${rel_graphite2_dir})
include_directories(${CMAKE_BINARY_DIR}/${rel_graphite2_dir})

set(SEGCACHE SegCache.cpp SegCacheEntry.cpp SegCacheStore.cpp)
if (GRAPHITE2_NSEGCACHE)
    add_definitions(-DGRAPHITE2_NSEGCACHE)
    set(SEGCACHE)
endif (GRAPHITE2_NSEGCACHE)

set(FILEFACE FileFace.cpp)
if (GRAPHITE2_NFILEFACE)
    add_definitions(-DGRAPHITE2_NFILEFACE)
    set(FILEFACE)
endif (GRAPHITE2_NFILEFACE)

set(TRACING json.cpp)
if (GRAPHITE2_NTRACING)
    add_definitions(-DGRAPHITE2_NTRACING)
    set(TRACING)
endif (GRAPHITE2_NTRACING)

set(GRAPHITE_HEADERS 
    ../include/graphite2/Font.h
    ../include/graphite2/Segment.h
    ../include/graphite2/Types.h
    ../include/graphite2/Log.h
    )

file(GLOB PRIVATE_HEADERS inc/*.h) 

set(${graphite2_dll_name}_sources
    ${GRAPHITE2_VM_TYPE}_machine.cpp
    gr_char_info.cpp
    gr_features.cpp
    gr_face.cpp
    gr_font.cpp
    gr_logging.cpp
    gr_segment.cpp
    gr_slot.cpp
    Bidi.cpp
    CachedFace.cpp
    CmapCache.cpp
    Code.cpp
    Face.cpp
    FeatureMap.cpp
    Font.cpp
    GlyphFace.cpp
    GlyphCache.cpp
    Justifier.cpp
    NameTable.cpp
    Pass.cpp
    Rule.cpp
    Segment.cpp
    Silf.cpp
    Slot.cpp
    Sparse.cpp
    TtfUtil.cpp
    UtfCodec.cpp
    ${FILEFACE}
    ${SEGCACHE}
    ${TRACING}
)

if(NATIVE_WINDOWS)
  set(${graphite2_dll_name}_sources
    ${${graphite2_dll_name}_sources}
    ${CMAKE_BINARY_DIR}/${rel_graphite2_dir}/graphite2.rc
  )
endif(NATIVE_WINDOWS)

add_library(${graphite2_dll_name}
  SHARED ${${graphite2_dll_name}_sources})

set_target_properties(${graphite2_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set_target_properties(${graphite2_dll_name} PROPERTIES 
    COMPILE_DEFINITIONS "_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;UNICODE;GRAPHITE2_EXPORTING")
endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

rebase(${graphite2_dll_name})

install(TARGETS ${graphite2_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
