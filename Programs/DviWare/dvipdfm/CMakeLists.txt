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
  -DHAVE_BASENAME
)

set(ebb_sources
  ${wrapper_cpp}
  ebb.c
  jpeg.c
  mem.c
  mfileio.c
  numbers.c
  pdfobj.c
  pdfparse.c
  pngimage.c
)

if(NATIVE_WINDOWS)
  set(ebb_sources ${ebb_sources}
    ebb.rc
  )
endif(NATIVE_WINDOWS)

add_executable(ebb ${ebb_sources})

target_link_libraries(ebb
  ${app_dll_name}
  ${core_dll_name}
  ${gnu_dll_name}
  ${kpsemu_dll_name}
  ${png_dll_name}
  ${texmf_dll_name}
  ${zlib_dll_name}
)

install(TARGETS ebb DESTINATION ${bindir})

set(dvipdfm_sources
  ${wrapper_cpp}
  colorsp.c
  dvi.c
  dvipdfm.c
  encodings.c
  epdf.c
  htex.c
  jpeg.c
  mem.c
  mfileio.c
  mpost.c
  numbers.c
  pdfdev.c
  pdfdoc.c
  pdfobj.c
  pdfparse.c
  pdfspecial.c
  pkfont.c
  pngimage.c
  psimage.c
  psspecial.c
  t1crypt.c
  tfm.c
  ttf.c
  thumbnail.c
  tpic.c
  type1.c
  vf.c
)

if(NATIVE_WINDOWS)
  set(dvipdfm_sources ${dvipdfm_sources}
    dvipdfm.rc
  )
endif(NATIVE_WINDOWS)

add_executable(dvipdfm ${dvipdfm_sources})

target_link_libraries(dvipdfm
  ${app_dll_name}
  ${core_dll_name}
  ${gnu_dll_name}
  ${kpsemu_dll_name}
  ${png_dll_name}
  ${texmf_dll_name}
  ${zlib_dll_name}
)

install(TARGETS dvipdfm DESTINATION ${bindir})

set(dvipdft_sources
  dvipdft.cpp
)

if(NATIVE_WINDOWS)
  set(dvipdft_sources ${dvipdft_sources}
    dvipdft.rc
  )
endif(NATIVE_WINDOWS)

add_executable(dvipdft ${dvipdft_sources})

target_link_libraries(dvipdft
  ${app_dll_name}
  ${core_dll_name}
  ${kpsemu_dll_name}
  ${texmf_dll_name}
)

install(TARGETS dvipdft DESTINATION ${bindir})
