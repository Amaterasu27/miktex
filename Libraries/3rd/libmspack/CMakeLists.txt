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

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(${mspack_dll_name}_sources
  cab.h
  cabc.c
  cabd.c
  chm.h
  chmc.c
  chmd.c
  des.h
  hlp.h
  hlpc.c
  hlpd.c
  kwaj.h
  kwajc.c
  kwajd.c
  lit.h
  litc.c
  litd.c
  lzss.h
  lzx.h
  lzxc.c
  lzxd.c
  mspack-version.h
  mspack.h
  mszip.h
  mszipc.c
  mszipd.c
  qtm.h
  qtmc.c
  qtmd.c
  sha.h
  system.c
  system.h
  szdd.h
  szddc.c
  szddd.c
)

if(WIN32)
  set(${mspack_dll_name}_sources ${${mspack_dll_name}_sources}
    mspack.def
    mspack.rc
  )
endif(WIN32)

add_library(${mspack_dll_name} SHARED ${${mspack_dll_name}_sources})

install(TARGETS ${mspack_dll_name} DESTINATION "${bindir}")
