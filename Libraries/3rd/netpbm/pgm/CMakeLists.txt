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
  asciitopgm
  bioradtopgm
  fstopgm
  hipstopgm
  lispmtopgm
  pbmtopgm
  pgmbentley
  pgmcrater
  pgmedge
  pgmenhance
  pgmhist
  pgmkernel
  pgmnoise
  pgmnorm
  pgmoil
  pgmramp
  pgmtexture
  pgmtofs
  pgmtolispm
  pgmtopbm
  psidtopgm
  rawtopgm
  spottopgm
)

set(${pgm_dll_name}_sources
  dithers.h
  libpgm.h
  libpgm1.c
  libpgm2.c
  pgm.h
)

if(NATIVE_WINDOWS)
  set(${pgm_dll_name}_sources ${${pgm_dll_name}_sources}
    libpgm.def
  )
endif(NATIVE_WINDOWS)

add_library(${pgm_dll_name} SHARED ${${pgm_dll_name}_sources})

set_target_properties(${pgm_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}.${MIKTEX_J2000_VERSION}"
    SOVERSION "1"
)

target_link_libraries(${pgm_dll_name}
  ${pbm_dll_name}
)

rebase(${pgm_dll_name})

foreach(e ${exes})
  add_executable(${e} ${e}.c pgm.h)
  merge_trustinfo_manifest(${e} asInvoker)
  add_dependencies(${e} ${pgm_dll_name})
  target_link_libraries(${e}
    ${pbm_dll_name}
    ${pgm_dll_name}
  )
endforeach(e ${exes})

install(TARGETS ${exes} ${pgm_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
