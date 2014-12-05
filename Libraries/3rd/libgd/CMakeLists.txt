## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2014 Christian Schenk
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
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(
  -DHAVE_CONFIG_H
)

if(NATIVE_WINDOWS)
add_definitions(
  -DBGDWIN32
)
endif(NATIVE_WINDOWS)

set(${gd_dll_name}_sources
  gd-version.h

  gd.c
  gdfx.c
  gd_security.c
  gd_gd.c
  gd_gd2.c
  gd_io.c
  gd_io_dp.c
  gd_gif_in.c
  gd_gif_out.c
  gd_io_file.c
  gd_io_ss.c
  gd_jpeg.c
  gd_png.c
  gd_ss.c
  gd_topal.c
  gd_wbmp.c
  gdcache.c
  gdfontg.c
  gdfontl.c
  gdfontmb.c
  gdfonts.c
  gdfontt.c
  gdft.c
  gdhelpers.c
  gdhelpers.h
  gdkanji.c
  gdtables.c
  gdxpm.c
  jisx0208.h
  wbmp.c
  wbmp.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/gd.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/gd.rc
  )
  set(${gd_dll_name}_sources ${${gd_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/gd.rc
  )
endif(NATIVE_WINDOWS)

add_library(${gd_dll_name} SHARED ${${gd_dll_name}_sources})

set_target_properties(${gd_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_link_libraries(${gd_dll_name}
  PRIVATE
    ${fontconfig_dll_name}
    ${freetype2_dll_name}
    ${jpeg_dll_name}
    ${png_dll_name}
    ${utf8wrap_dll_name}
    ${zlib_dll_name}
)

delay_load(${gd_dll_name}
  ${fontconfig_dll_name}
  ${freetype2_dll_name}
  ${jpeg_dll_name}
  ${png_dll_name}
  ${utf8wrap_dll_name}
  ${zlib_dll_name}
)

rebase(${gd_dll_name})

install(TARGETS ${gd_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
