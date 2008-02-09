## CMakeLists.txt						-*- CMake -*-
##
## Copyright (C) 2006-2007 Christian Schenk
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

set(exes
  bmptopnm
  fitstopnm
  giftopnm
  pnmalias
  pnmarith
  pnmcat
  pnmcomp
  pnmconvol
  pnmcrop
  pnmcut
  pnmdepth
  pnmenlarge
  pnmfile
  pnmflip
  pnmgamma
  pnmhistmap
  pnminvert
  pnmmargin
  pnmnlfilt
  pnmnoraw
  pnmpad
  pnmpaste
  pnmrotate
  pnmscale
  pnmshear
  pnmtile
  pnmtoddif
  pnmtofits
  pnmtops
  pnmtorast
  pnmtosgi
  pnmtosir
  pnmtotiff
  pnmtoxwd
  rasttopnm
  sgitopnm
  sirtopnm
  tifftopnm
  xwdtopnm
  zeisstopnm
)

set(${pnm_dll_name}_sources
  libpnm1.c
  libpnm2.c
  libpnm3.c
  libpnm4.c
  pnm.h
  rast.h
  sgi.h
  x10wd.h
  x11wd.h
)

if(NATIVE_WINDOWS)
  set(${pnm_dll_name}_sources ${${pnm_dll_name}_sources}
    libpnm.def
  )
endif(NATIVE_WINDOWS)

add_library(${pnm_dll_name} SHARED ${${pnm_dll_name}_sources})

target_link_libraries(${pnm_dll_name}
  ${pbm_dll_name}
  ${pgm_dll_name}
  ${ppm_dll_name}
  ${tiff_dll_name}
)

rebase(${pnm_dll_name})

foreach(e ${exes})
  add_executable(${e} ${e}.c pnm.h)
  merge_trustinfo_manifest(${e} asInvoker)
  add_dependencies(${e} ${pnm_dll_name})
  target_link_libraries(${e}
    ${pbm_dll_name}
    ${pgm_dll_name}
    ${ppm_dll_name}
    ${pnm_dll_name}
  )
endforeach(e ${exes})

install(TARGETS ${exes} ${pnm_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
