## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2007-2015 Christian Schenk
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

add_definitions(
  -DFT2_BUILD_LIBRARY
)

include_directories(BEFORE
  ${CMAKE_SOURCE_DIR}/${rel_freetype2_dir}/src/gzip
)

include_directories(BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(ft_modules
  src/base/ftbbox.c
  src/base/ftbdf.c
  src/base/ftmm.c
  src/base/ftpfr.c
  src/base/ftsynth.c
  src/base/fttype1.c
  src/base/ftwinfnt.c
  src/pcf/pcf.c
  src/pfr/pfr.c
  src/psaux/psaux.c
  src/pshinter/pshinter.c
  src/psnames/psmodule.c
  src/raster/raster.c
  src/sfnt/sfnt.c
  src/truetype/truetype.c
  src/type1/type1.c
  src/cid/type1cid.c
  src/type42/type42.c
  src/winfonts/winfnt.c
)

set(ft_sources
  src/autofit/autofit.c
  src/bdf/bdf.c
  src/cff/cff.c
  src/bzip2/ftbzip2.c
  src/base/ftbase.c
  src/base/ftbitmap.c
  src/cache/ftcache.c
  builds/win32/ftdebug.c
  src/base/ftfstype.c
  src/base/ftgasp.c
  src/base/ftglyph.c
  src/gzip/ftgzip.c
  src/base/ftinit.c
  src/lzw/ftlzw.c
  src/base/ftstroke.c
  src/base/ftsystem.c
  src/smooth/smooth.c
)

set(${freetype2_dll_name}_sources
  ${ft_modules}
  ${ft_sources}
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/freetype2.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/freetype2.rc
  )
  set(${freetype2_dll_name}_sources ${${freetype2_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/freetype2.rc
  )
endif()

add_library(${freetype2_dll_name} SHARED ${${freetype2_dll_name}_sources})

target_include_directories(${freetype2_dll_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/${rel_freetype2_dir}/include   
)

target_link_libraries(${freetype2_dll_name}
  PRIVATE
    ${utf8wrap_dll_name}
)

set_target_properties(${freetype2_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${freetype2_dll_name})

install(TARGETS ${freetype2_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
