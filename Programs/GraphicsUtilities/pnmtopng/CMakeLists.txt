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
  -D__WIN32DLL__
)

set(pnmtopng_sources
  pnmtopng.c
)

add_executable(pnmtopng ${pnmtopng_sources})

target_link_libraries(pnmtopng
  ${pbm_dll_name}
  ${pgm_dll_name}
  ${png_dll_name}
  ${pnm_dll_name}
  ${ppm_dll_name}
  ${zlib_dll_name}
)

install(TARGETS pnmtopng DESTINATION ${bindir})

set(pngtopnm_sources
  pngtopnm.c
)

add_executable(pngtopnm ${pngtopnm_sources})

target_link_libraries(pngtopnm
  ${pbm_dll_name}
  ${pgm_dll_name}
  ${png_dll_name}
  ${pnm_dll_name}
  ${ppm_dll_name}
  ${zlib_dll_name}
)

install(TARGETS pngtopnm DESTINATION ${bindir})
