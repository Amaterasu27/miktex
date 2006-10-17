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

set(${bmeps_dll_name}_sources
  bmeps-version.h
  bmeps.c
  bmeps.h
  bmepsco.h
  bmepsm.c
  bmepsoe.c
  bmepsoe.h
  jpgeps.c
  jpgeps.h
  libbmeps-version.h
  pngeps.c
  pngeps.h
  pnmeps.c
  pnmeps.h
)

if(WIN32)
  set(${bmeps_dll_name}_sources ${${bmeps_dll_name}_sources}
    libbmeps.def
    libbmeps.rc
  )
endif(WIN32)

add_library(${bmeps_dll_name} SHARED ${${bmeps_dll_name}_sources})

target_link_libraries(${bmeps_dll_name}
  ${jpeg_dll_name}
  ${pbm_dll_name}
  ${png_dll_name}
  ${pnm_dll_name}
  ${zlib_dll_name}
)

install(TARGETS ${bmeps_dll_name} DESTINATION "${bindir}")
