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
  atktopbm
  brushtopbm
  cmuwmtopbm
  g3topbm
  gemtopbm
  icontopbm
  macptopbm
  mgrtopbm
  pbmclean
  pbmlife
  pbmmake
  pbmmask
  pbmpscale
  pbmreduce
  pbmtext
  pbmto10x
  pbmto4425
  pbmtoascii
  pbmtoatk
  pbmtobbnbg
  pbmtocmuwm
  pbmtoepsi
  pbmtoepson
  pbmtog3
  pbmtogem
  pbmtogo
  pbmtoicon
  pbmtolj
  pbmtoln03
  pbmtolps
  pbmtomacp
  pbmtomgr
  pbmtopi3
  pbmtopk
  pbmtoplot
  pbmtoptx
  pbmtox10bm
  pbmtoxbm
  pbmtoybm
  pbmtozinc
  pbmupc
  pi3topbm
  pktopbm
  xbmtopbm
  ybmtopbm
)

set(${pbm_dll_name}_sources
  bitreverse.h
  cmuwm.h
  g3.h
  includes.h
  libpbm.h
  libpbm1.c
  libpbm2.c
  libpbm3.c
  libpbm4.c
  libpbm5.c
  macp.h
  mgr.h
  pbm.h
  pbmfont.h
)

if(NATIVE_WINDOWS)
  set(${pbm_dll_name}_sources ${${pbm_dll_name}_sources}
    libpbm.def
  )
endif(NATIVE_WINDOWS)

add_library(${pbm_dll_name} SHARED ${${pbm_dll_name}_sources})

set_target_properties(${pbm_dll_name}
  PROPERTIES
    VERSION "${MIKTEX_SERIES_STR}"
    SOVERSION "1"
)

rebase(${pbm_dll_name})

foreach(e ${exes})
  add_executable(${e} ${e}.c pbm.h)
  merge_trustinfo_manifest(${e} asInvoker)
  add_dependencies(${e} ${pbm_dll_name})
  target_link_libraries(${e} ${pbm_dll_name})
endforeach(e ${exes})

install(TARGETS ${exes} ${pbm_dll_name}
  RUNTIME DESTINATION "${bindir}"
  LIBRARY DESTINATION "${libdir}"
  ARCHIVE DESTINATION "${libdir}"
)
