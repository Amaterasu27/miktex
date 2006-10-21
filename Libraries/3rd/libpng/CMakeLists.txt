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

add_definitions(
  -DPNG_BUILD_DLL
  -D_WINDLL
)

set(${png_dll_name}_sources
  libpng-version.h
  png.c
  png.h
  pngconf.h
  pngerror.c
  pngget.c
  pngmem.c
  pngpread.c
  pngread.c
  pngrio.c
  pngrtran.c
  pngrutil.c
  pngset.c
  pngtrans.c
  pngvcrd.c
  pngwio.c
  pngwrite.c
  pngwtran.c
  pngwutil.c
)

if(MIKTEX_WINDOWS)
  set(${png_dll_name}_sources ${${png_dll_name}_sources}
    libpng.def
    libpng.rc
  )
endif(MIKTEX_WINDOWS)

add_library(${png_dll_name} SHARED ${${png_dll_name}_sources})

target_link_libraries(${png_dll_name} ${zlib_dll_name})

install(TARGETS ${png_dll_name} DESTINATION "${bindir}")
