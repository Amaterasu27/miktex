## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2008 Christian Schenk
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

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

set(${uriparser_dll_name}_sources
  UriCommon.c
  UriCommon.h
  UriCompare.c
  UriEscape.c
  UriFile.c
  UriIp4.c
  UriIp4Base.c
  UriIp4Base.h
  UriNormalize.c
  UriNormalizeBase.c
  UriNormalizeBase.h
  UriParse.c
  UriParseBase.c
  UriParseBase.h
  UriQuery.c
  UriRecompose.c
  UriResolve.c
  UriShorten.c
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/uriparser.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/uriparser.rc
  )
  set(${uriparser_dll_name}_sources ${${uriparser_dll_name}_sources}
    uriparser.def
    ${CMAKE_CURRENT_BINARY_DIR}/uriparser.rc
  )
endif(NATIVE_WINDOWS)

add_library(${uriparser_dll_name} SHARED ${${uriparser_dll_name}_sources})

set_target_properties(${uriparser_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
    SOVERSION "1"
)

rebase(${uriparser_dll_name})

install(TARGETS ${uriparser_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
