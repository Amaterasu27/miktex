## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2013-2014 Christian Schenk
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

add_definitions(-D__STDC__=1)
remove_definitions(-DHAVE_CONFIG_H)

set(CAIRO_HAS_XLIB_SURFACE FALSE)
set(CAIRO_HAS_XLIB_XRENDER_SURFACE FALSE)
set(CAIRO_HAS_XCB_SURFACE FALSE)
set(CAIRO_HAS_XLIB_XCB_FUNCTIONS FALSE)
set(CAIRO_HAS_XCB_SHM_FUNCTIONS FALSE)
set(CAIRO_HAS_QT_SURFACE FALSE)
set(CAIRO_HAS_QUARTZ_SURFACE FALSE)
set(CAIRO_HAS_QUARTZ_FONT FALSE)
set(CAIRO_HAS_QUARTZ_IMAGE_SURFACE FALSE)
set(CAIRO_HAS_WIN32_SURFACE TRUE)
set(CAIRO_HAS_WIN32_FONT TRUE)
set(CAIRO_HAS_SKIA_SURFACE FALSE)
set(CAIRO_HAS_OS2_SURFACE FALSE)
set(CAIRO_HAS_BEOS_SURFACE FALSE)
set(CAIRO_HAS_DRM_SURFACE FALSE)
set(CAIRO_HAS_GALLIUM_SURFACE FALSE)
set(CAIRO_HAS_PNG_FUNCTIONS TRUE)
set(CAIRO_HAS_GL_SURFACE FALSE)
set(CAIRO_HAS_GLESV2_SURFACE FALSE)
set(CAIRO_HAS_COGL_SURFACE FALSE)
set(CAIRO_HAS_DIRECTFB_SURFACE FALSE)
set(CAIRO_HAS_VG_SURFACE FALSE)
set(CAIRO_HAS_EGL_FUNCTIONS FALSE)
set(CAIRO_HAS_GLX_FUNCTIONS FALSE)
set(CAIRO_HAS_WGL_FUNCTIONS FALSE)
set(CAIRO_HAS_SCRIPT_SURFACE TRUE)
set(CAIRO_HAS_FT_FONT FALSE)
set(CAIRO_HAS_FC_FONT FALSE)
set(CAIRO_HAS_PS_SURFACE TRUE)
set(CAIRO_HAS_PDF_SURFACE TRUE)
set(CAIRO_HAS_SVG_SURFACE TRUE)
set(CAIRO_HAS_TEST_SURFACES FALSE)
set(CAIRO_HAS_TEE_SURFACE FALSE)
set(CAIRO_HAS_XML_SURFACE FALSE)
set(CAIRO_HAS_PTHREAD FALSE)
set(CAIRO_HAS_GOBJECT_FUNCTIONS FALSE)
set(CAIRO_HAS_TRACE FALSE)
set(CAIRO_HAS_INTERPRETER TRUE)
set(CAIRO_HAS_SYMBOL_LOOKUP FALSE)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo.h
  ${CMAKE_BINARY_DIR}/${rel_cairo_include_dir}/cairo.h
  COPYONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cairo-version.h
  ${CMAKE_BINARY_DIR}/${rel_cairo_include_dir}/cairo-version.h
  COPYONLY
)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-deprecated.h
  ${CMAKE_BINARY_DIR}/${rel_cairo_include_dir}/cairo-deprecated.h
  COPYONLY
)

configure_file(
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}/cairo-features.h.cmake
  ${CMAKE_BINARY_DIR}/${rel_cairo_include_dir}/cairo-features.h
)

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/${rel_cairo_source_dir}
  ${CMAKE_BINARY_DIR}/${rel_cairo_include_dir}
  ${CMAKE_SOURCE_DIR}/${rel_pixman_include_dir}
)

include(features.cmake)

set(${cairo_dll_name}_sources
  ${enabled_cairo_sources}
  ${cairo_headers}
  ${cairo_private}
  miktex-cairo-version.h
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cairo.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/cairo.rc
  )
  set(${cairo_dll_name}_sources ${${cairo_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/cairo.rc
  )
endif(NATIVE_WINDOWS)

add_library(${cairo_dll_name} SHARED ${${cairo_dll_name}_sources})

set_target_properties(${cairo_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

target_link_libraries(${cairo_dll_name}
  gdi32.lib
  msimg32.lib
  user32.lib
  ${pixman_lib_name}
  ${png_dll_name}
  ${zlib_dll_name}
)

#delay_load(${cairo_dll_name}
#)

rebase(${cairo_dll_name})

install(TARGETS ${cairo_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
