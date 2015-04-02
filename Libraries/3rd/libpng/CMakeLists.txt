## CMakeLists.txt					-*- CMake -*-
##
## Copyright (C) 2006-2015 Christian Schenk
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

add_definitions(-DPNG_CONFIGURE_LIBPNG)
add_definitions(-DPNG_NO_CONSOLE_IO)

if(MSVC)
  add_definitions(-DPNG_NO_MODULEDEF -D_CRT_SECURE_NO_DEPRECATE)
endif()

set(${png_dll_name}_sources
  libpng-version.h
  png.c
  png.h
  pngconf.h
  pngerror.c
  pngget.c
  pngmem.c
  pngpread.c
  pngpriv.h
  pngread.c
  pngrio.c
  pngrtran.c
  pngrutil.c
  pngset.c
  pngtrans.c
  pngwio.c
  pngwrite.c
  pngwtran.c
  pngwutil.c
)

if(NATIVE_WINDOWS)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libpng.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libpng.rc
  )
  set(${png_dll_name}_sources ${${png_dll_name}_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/libpng.rc
  )
endif()

add_library(${png_dll_name} SHARED ${${png_dll_name}_sources})

set_target_properties(${png_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

if(WIN32)
  set_target_properties(${png_dll_name}
  PROPERTIES
    DEFINE_SYMBOL PNG_BUILD_DLL)
endif()

target_compile_definitions(${png_dll_name}
  INTERFACE
    -DPNG_USE_DLL
)

target_include_directories(${png_dll_name}
  PUBLIC
    include_directories(${CMAKE_SOURCE_DIR}/${rel_png_dir})
)

target_link_libraries(${png_dll_name}
  PRIVATE
    ${zlib_dll_name}
)

rebase(${png_dll_name})

install(TARGETS ${png_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
