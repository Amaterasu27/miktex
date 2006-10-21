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
  -DBGDMIKTEX_WINDOWS
  -DHAVE_LIBJPEG
  -DHAVE_LIBPNG
  -DPATHSEPARATOR=\";\"
  -Dinline=__inline
)

set(${gd_dll_name}_sources
  entities.h
  gd-version.h
  gd.c
  gd.h
  gd_gd.c
  gd_gd2.c
  gd_gif_in.c
  gd_gif_out.c
  gd_io.c
  gd_io.h
  gd_io_dp.c
  gd_io_file.c
  gd_io_ss.c
  gd_jpeg.c
  gd_png.c
  gd_ss.c
  gd_topal.c
  gd_wbmp.c
  gdcache.c
  gdcache.h
  gdfontg.c
  gdfontg.h
  gdfontl.c
  gdfontl.h
  gdfontmb.c
  gdfontmb.h
  gdfonts.c
  gdfonts.h
  gdfontt.c
  gdfontt.h
  gdft.c
  gdfx.c
  gdfx.h
  gdhelpers.c
  gdhelpers.h
  gdkanji.c
  gdtables.c
  gdxpm.c
  jisx0208.h
  wbmp.c
  wbmp.h
)

if(MIKTEX_WINDOWS)
  set(${gd_dll_name}_sources ${${gd_dll_name}_sources}
    gd.rc
  )
endif(MIKTEX_WINDOWS)

add_library(${gd_dll_name} SHARED ${${gd_dll_name}_sources})

target_link_libraries(${gd_dll_name}
  ${jpeg_dll_name}
  ${png_dll_name}
  ${zlib_dll_name}
)

install(TARGETS ${gd_dll_name} DESTINATION "${bindir}")
