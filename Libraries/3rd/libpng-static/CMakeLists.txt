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

use_static_crt()
use_static_libraries()

add_definitions(-DPNG_CONFIGURE_LIBPNG)
add_definitions(-DPNG_NO_CONSOLE_IO)

if(MSVC)
  add_definitions(-DPNG_NO_MODULEDEF -D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

set(${png_lib_name}_sources
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/libpng-version.h
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/png.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/png.h
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngconf.h
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngerror.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngget.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngmem.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngpread.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngpriv.h
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngread.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngrio.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngrtran.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngrutil.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngset.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngtrans.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngwio.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngwrite.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngwtran.c
  ${CMAKE_SOURCE_DIR}/${rel_png_dir}/pngwutil.c
)

add_library(${png_lib_name} STATIC ${${png_lib_name}_sources})

target_include_directories(${png_lib_name}
  PUBLIC
    include_directories(${CMAKE_SOURCE_DIR}/${rel_png_dir})
)

target_link_libraries(${png_lib_name}
  PUBLIC
    ${zlib_lib_name}
)

if(INSTALL_STATIC_LIBRARIES)
  install(TARGETS ${png_lib_name}
    RUNTIME DESTINATION "${bindir}"
    LIBRARY DESTINATION "${libdir}"
    ARCHIVE DESTINATION "${libdir}"
  )
endif(INSTALL_STATIC_LIBRARIES)
