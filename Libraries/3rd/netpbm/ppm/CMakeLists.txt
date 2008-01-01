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
  bmptoppm
  gouldtoppm
  hpcdtoppm
  ilbmtoppm
  imgtoppm
  mtvtoppm
  pcxtoppm
  pgmtoppm
  pi1toppm
  picttoppm
  pjtoppm
  ppm3d
  ppmbrighten
  ppmchange
  ppmdim
  ppmdist
  ppmdither
  ppmflash
  ppmforge
  ppmhist
  ppmmake
  ppmmix
  ppmnorm
  ppmntsc
  ppmpat
  ppmquant
  ppmqvga
  ppmrelief
  ppmshift
  ppmspread
  ppmtoacad
  ppmtobmp
  ppmtogif
  ppmtoicr
  ppmtoilbm
  ppmtomap
  ppmtomitsu
  ppmtopcx
  ppmtopgm
  ppmtopi1
  ppmtopict
  ppmtopj
  ppmtopjxl
  ppmtopuzz
  ppmtorgb3
  ppmtosixel
  ppmtotga
  ppmtouil
  ppmtoxpm
  ppmtoyuv
  ppmtoyuvsplit
  qrttoppm
  rawtoppm
  rgb3toppm
  sldtoppm
  spctoppm
  sputoppm
  tgatoppm
  ximtoppm
  xpmtoppm
  xvminitoppm
  yuvsplittoppm
  yuvtoppm
)

set(${ppm_dll_name}_sources
  autocad.h
  bitio.c
  bitio.h
  bmp.h
  ilbm.h
  libppm.h
  libppm1.c
  libppm2.c
  libppm3.c
  libppm4.c
  libppm5.c
  lum.h
  mitsu.h
  ppm.h
  ppmcmap.h
  ppmdraw.h
  tga.h
  xim.h
)

if(NATIVE_WINDOWS)
  set(${ppm_dll_name}_sources ${${ppm_dll_name}_sources}
    libppm.def
  )
endif(NATIVE_WINDOWS)

add_library(${ppm_dll_name} SHARED ${${ppm_dll_name}_sources})

set_target_properties(${ppm_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
    SOVERSION "1"
)

target_link_libraries(${ppm_dll_name}
  ${pbm_dll_name}
  ${pgm_dll_name}
)

rebase(${ppm_dll_name})

foreach(e ${exes})
  add_executable(${e} ${e}.c ppm.h)
  merge_trustinfo_manifest(${e} asInvoker)
  add_dependencies(${e} ${ppm_dll_name})
  target_link_libraries(${e}
    ${pbm_dll_name}
    ${pgm_dll_name}
    ${ppm_dll_name}
  )
endforeach(e ${exes})

install(TARGETS ${exes} ${ppm_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
