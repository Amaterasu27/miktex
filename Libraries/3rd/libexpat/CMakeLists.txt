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

if(WIN32)
  add_definitions(-DCOMPILED_FROM_DSP)
endif(WIN32)

set(${expat_dll_name}_sources
  ascii.h
  asciitab.h
  expat-version.h
  expat.h
  expat_config.h
  expat_external.h
  iasciitab.h
  internal.h
  latin1tab.h
  nametab.h
  utf8tab.h
  xmlparse.c
  xmlrole.c
  xmlrole.h
  xmltok.c
  xmltok.h
  xmltok_impl.h
)

if(WIN32)
  set(${expat_dll_name}_sources ${${expat_dll_name}_sources}
    expat.rc
  )
endif(WIN32)

add_library(${expat_dll_name} SHARED ${${expat_dll_name}_sources})

install(TARGETS ${expat_dll_name} DESTINATION "${bindir}")
